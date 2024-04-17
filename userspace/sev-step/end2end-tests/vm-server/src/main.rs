use actix_web::{web, App, HttpServer, Either, HttpResponse};
use nix::sys::mman::{MapFlags, ProtFlags};
use serde::{Deserialize, Serialize};
use vm_server::aligned_exec_fns::{self, SingleStepTarget};
use core::panic;
use std::collections::HashSet;
use std::str::FromStr;
use std::sync::Mutex;
use std::thread::{self,JoinHandle};
use std::time::Duration;
use anyhow::{Result,bail,Context};
use std::sync::mpsc::{self, TryRecvError,Sender};
use vm_server::linux_memory::{MemoryBuffer, LinuxPageMap};

///Represents an async PingPong Job
struct PingPongerJob {
    handle : JoinHandle<()>,
    stop_signal_tx : Sender<()>,
    start_signal_tx : Sender<()>,
    gpa1 : u64,
    gpa2 : u64,
}

///Selects which kind of access a PingPongerJob should perform to its two pages
#[derive(Clone,Copy)]
enum AccessType {
    READ,
    WRITE,
    EXEC,
}

impl ToString for AccessType {
    fn to_string(&self) -> String {
        match self {
            AccessType::READ => String::from("READ"),
            AccessType::WRITE => String::from("WRITE"),
            AccessType::EXEC => String::from("EXEC"),
        }
    }
}

impl FromStr for AccessType {
    type Err = anyhow::Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "READ" => Ok(AccessType::READ),
            "WRITE" => Ok(AccessType::WRITE),
            "EXEC" => Ok(AccessType::EXEC),
            _ => bail!("not a valid access type"),

        }
    }

}


/// Alternates accessing two pages with a configureable access type.
/// The accesses run in the background
struct PagePingPonger {
    job : Option<PingPongerJob>,
}

impl PagePingPonger {
    ///Initializes a new PingPonger that will issues the given access_type to its two pages once started.
    /// The PingPonger is in a halted state until `PagePingPonger::run` is called
    fn new(access_type : AccessType) -> Result<PagePingPonger>{

        let (stop_signal_tx,stop_signal_rx) = mpsc::channel::<()>();
        let (start_signal_tx,start_signal_rx) = mpsc::channel::<()>();
        let (gpa_tx,gpa_rx) = mpsc::channel();

        let thread_handle = thread::spawn(move || {

            //allocate memory pages that we want to access

            let mut memory_buffer = MemoryBuffer::new(
                2* 4096,
                 ProtFlags::PROT_READ | ProtFlags::PROT_WRITE, 
                 MapFlags::MAP_PRIVATE | MapFlags::MAP_ANONYMOUS | MapFlags::MAP_POPULATE
                )
                .expect("failed to alloc pages");

                let page1 = memory_buffer.offset(0).expect("handle error");
                let page2 = memory_buffer.offset(4096).expect("handle err");

                //send gpas to parent
                match access_type {
                    AccessType::READ => {
                        gpa_tx.send((page1.phys,page2.phys)).unwrap();

                    },
                    AccessType::WRITE => {
                        gpa_tx.send((page1.phys,page2.phys)).unwrap();

                    }
                    AccessType::EXEC => {
                        let mut pagemap_parser = LinuxPageMap::new().unwrap();

                        //Calling foo and bar once ensures that they are faulted into memory and not just lazily mapped
                        aligned_exec_fns::foo();
                        aligned_exec_fns::bar();

                        let gpa1 = pagemap_parser.get_phys(aligned_exec_fns::foo as u64).unwrap();
                        let gpa2 = pagemap_parser.get_phys(aligned_exec_fns::bar as u64).unwrap();
                        gpa_tx.send((gpa1,gpa2)).unwrap();
                    }
                }
                
                //wait for parent to send us the start signal
                match start_signal_rx.recv() {
                    Ok(_) => eprint!("worker thread received start signal"),
                    Err(_) => panic!("failed to read from channel")
                }

            //access memory pages in a loop until we get the stop signal
            let mut iteration_counter = 1;
            loop {
                match stop_signal_rx.try_recv() {
                    Ok(_) | Err(TryRecvError::Disconnected) => {
                        println!("worker thread received stop signal");
                        break;
                    }
                    Err(TryRecvError::Empty) => {}
                }

                if iteration_counter == 6 {
                    //TODO: make amount of iterations part of the rest api
                    eprintln!("Thread done with active accesses. TODO: make this part of the API");
                    thread::sleep(Duration::from_millis(100));
                } else if iteration_counter > 6 {
                    thread::sleep(Duration::from_millis(100));
                } else {
                    match access_type {
                        AccessType::READ => {
                            unsafe{
                                page1.ptr.read_volatile();
                                page2.ptr.read_volatile();
                            }
                        },
                        AccessType::WRITE => {
                            unsafe{
                                page1.ptr.write_volatile(42);
                                page2.ptr.write_volatile(42);
                            }
                        }
                        AccessType::EXEC => {
                            aligned_exec_fns::foo();
                            aligned_exec_fns::bar();
                        }
                    }
                }
           
                iteration_counter += 1;
            };

            eprintln!("worker thread terminated");
           
        });

        let(gpa1,gpa2) = gpa_rx.recv().context("failed to get gpas")?;

    Ok(
        PagePingPonger { 
            job: Some(
                PingPongerJob { handle: thread_handle, stop_signal_tx, start_signal_tx,gpa1,gpa2 }
            )
        }
    )
}

    /// Get the two physical addresses that the ping ponger will access.
    /// Note: As this function is intendedd to run inside a VM, we talk about GPAs in the code
    /// instead of physical addresses
    fn get_gpas(&self) -> Option<(u64,u64)> {
        match  &self.job {
            None => None,
            Some(v) => Some((v.gpa1,v.gpa2))
        }
    }

    ///Starts the ping pong accesses (in the background)
    fn run(&mut self) -> Result<()>{
        match &mut self.job {
            None => bail!("not initialized"),
            Some(v) => v.start_signal_tx.send(()).context("failed to send start signal")?
       }
       Ok(())
    }

    ///Stops the ping pong accesses. To start again, a new PingPonger needs to be created
    fn stop(mut self) -> Result<()> {
        let job = match  self.job.take() {
            None => bail!("there is no job to stop"),
            Some(v) => v,
        };
        job.start_signal_tx.send(()).context("failed to send start signal")?;
        job.stop_signal_tx.send(()).context("failed to send stop signal")?;
       
        if let Err(e) = job.handle.join() {
            bail!("worker thread panicked on {:?}",e);
        };
        Ok(())
    }

}

struct AppState {
    page_pingponger: Mutex<Option<PagePingPonger>>,
    running_victim_programs: Mutex<HashSet<SingleStepTarget>>,
}

#[derive(Deserialize,Serialize)]
struct PingPongPageTrackInitResp {
    gpa1 : u64,
    gpa2 : u64,
}

#[derive(Serialize,Deserialize)]
struct PingPongPageTrackReq {
    access_type : String,
}



///create new ping ponger or return error if it already exists
async fn handle_pingpong_pagetrack_init(req: web::Json<PingPongPageTrackReq>, data: web::Data<AppState>) -> Either<HttpResponse,web::Json<PingPongPageTrackInitResp>> {
    eprintln!("handle_pingpong_pagetrack_init");

    let access_type = match AccessType::from_str(&req.access_type) {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::BadRequest().body(e.to_string()))
    };

    let mut page_pingponger = match data.page_pingponger.lock() {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::InternalServerError().body(e.to_string()))
    };

  
    if let Some(_) = *page_pingponger {
        return Either::Left(HttpResponse::BadRequest().body("there is already a job"));
    }

    let pingponger = match PagePingPonger::new(access_type) {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::InternalServerError().body(e.to_string())),
    };

    let (gpa1,gpa2) = match pingponger.get_gpas() {
        Some(v) => v,
        None => return Either::Left(HttpResponse::InternalServerError().body("failed to get gpas"))
    };

    *page_pingponger = Some(pingponger);

    Either::Right(
        web::Json(
            PingPongPageTrackInitResp{       
                gpa1,gpa2,
            }
        )
    )
}

async fn handle_pingpong_pagetrack_start(data: web::Data<AppState>) -> HttpResponse {
    eprintln!("handle_pingpong_pagetrack_start");

    let mut page_pingponger = match data.page_pingponger.lock() {
        Ok(v) => v,
        Err(e) => return HttpResponse::InternalServerError().body(e.to_string())
    };

    match  &mut *page_pingponger {
        None => HttpResponse::BadRequest().body("create new job first"),
        Some(v) => match v.run() {
                Ok(_) => HttpResponse::Ok().finish(),
                Err(e) => HttpResponse::InternalServerError().body(e.context("failed to start job").to_string()),
        }
    }

}

async fn handle_pingpong_pagetrack_teardown(data: web::Data<AppState>) -> HttpResponse {
    eprintln!("handle_pingpong_pagetrack_teardown");

    let mut page_pingponger = match data.page_pingponger.lock() {
        Ok(v) => v,
        Err(e) => return HttpResponse::InternalServerError().body(e.to_string())
    };

    match (*page_pingponger).take() {
        None => HttpResponse::BadRequest().body("create new job first"),
        Some( v) => match v.stop() {
            Ok(_) => HttpResponse::Ok().finish(),
            Err(e) => HttpResponse::InternalServerError().body(e.context("failed to stop job").to_string()),
        },
    }
}

#[derive(Serialize,Deserialize)]
struct SingleStepVictimStartReq {
    victim_program : String
}






async fn handle_single_step_victim_start(req: web::Json<SingleStepVictimStartReq>, data: web::Data<AppState>)
    -> HttpResponse {
        eprintln!("handle_single_step_victim_start");
    //Parse Params
    let program_type = match SingleStepTarget::from_str(&req.victim_program) {
        Ok(v) => v,
        Err(e) => return HttpResponse::BadRequest().body(e.to_string())
    };

    //Ensure That Program is not currently Running
    let mut running_victims = match data.running_victim_programs.lock() {
        Ok(v) => v,
        Err(e) => return HttpResponse::InternalServerError().body(e.to_string()),
    };

    if running_victims.contains(&program_type) {
        return HttpResponse::BadRequest().body("requested program is already running");
    }

    running_victims.insert(program_type);
    drop(running_victims); //release lock

    //run program in background


    //TODO: ideally we would save the thread handle somewhere to be able to check that is finished succesfully
    thread::spawn(move || {
        let program_desc = match program_type.to_single_step_target_desc() {
            Ok(v) => v,
            Err(e) => {
                eprintln!("Failed to fetch program: {:?}",e);
                panic!("failed to fetch program");
            }
        };

        
            let mut pagemap_parser = match LinuxPageMap::new() {
                Ok(v) => v,
                Err(_) => panic!("pagemap parser failed\n"),
            };

            let gpa = match pagemap_parser.get_phys(program_desc.fn_vaddr) {
                Ok(v) => v,
                Err(_) => panic!("get phys failed\n")
            };

            eprintln!("GPA:0x{:x}, Vaddr:0x{:x}",gpa,program_desc.fn_vaddr);
            if let Some(v) = &program_desc.cache_attack_target {
                eprintln!("Lookup table: vaddr 0x{:x}, gpa 0x{:x}",v.lookup_table_vaddr,v.lookup_table_gpa);
            }

        eprintln!("handle_single_step_victim_start worker thread: Starting program...");
        (*(program_desc.execute))();
        eprintln!("handle_single_step_victim_start worker thread: Program done");
        //mark program as no longer running

        let mut running_victims = match data.running_victim_programs.lock() {
            Ok(v) => v,
            Err(e) => panic!("background thread failed to mark {} as no longer running : {}",
            program_type.to_string(),e.to_string()),
        };
        running_victims.remove(&program_type);

    });
    HttpResponse::Ok().finish()
}




#[derive(Serialize,Deserialize)]
struct SingleStepVictimInitReq {
    victim_program : String
}

#[derive(Serialize,Deserialize)]
struct SingleStepVictimInitResp {
    /// (Guest) physical address at which the targeted function is mapped
    gpa : u64,
    /// virtual address at which the targeted function is mapped
    vaddr : u64,
    /// all instr offsets of the targeted function (on assembly level), relative to vaddr
    expected_offsets : Vec<u64>,

    /// If true, the following cache attack fields contain valid data
    has_cache_attack_data : bool,
    // Start of cache attack fields

    /// subset of expected_offsets at which relevant memory accesses take place
    offsets_with_mem_access: Vec<u64>,
    /// store for each offsets_with_mem_access which offset in the lookup table is accessed
    mem_access_target_offset: Vec<u64>,
    lookup_table_gpa : u64,
    lookup_table_vaddr : u64,
    /// length of lookup table in bytes
    lookup_table_bytes : u64,
    //end of cache attack fields
}

async fn handle_single_step_victim_init(req : web::Json<SingleStepVictimInitReq>) 
-> Either<HttpResponse,web::Json<SingleStepVictimInitResp>> {
    eprintln!("handle_single_step_victim_init");

    //Parse Params
    let program_type = match SingleStepTarget::from_str(&req.victim_program) {
        Ok(v) => v,
        Err(e) => {
            eprintln!("failed to parse param: {}",e);
            return Either::Left(HttpResponse::BadRequest().body(e.to_string()))
        }
    };

    eprintln!("Requested program: {}",program_type.to_string());
    let program_desc = match program_type.to_single_step_target_desc() {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::InternalServerError().body(e.to_string()))
    };

    let mut pagemap_parser = match LinuxPageMap::new() {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::InternalServerError().body(e.to_string()))
    };

    let gpa = match pagemap_parser.get_phys(program_desc.fn_vaddr) {
        Ok(v) => v,
        Err(e) => return Either::Left(HttpResponse::InternalServerError().body(e.to_string()))
    };

    eprintln!("GPA:0x{:x}, Vaddr:0x{:x}",gpa,program_desc.fn_vaddr);
    if let Some(v) = &program_desc.cache_attack_target {
        eprintln!("Lookup table: vaddr 0x{:x}, gpa 0x{:x}",v.lookup_table_vaddr,v.lookup_table_gpa);
    }
    match program_desc.cache_attack_target {
        None => {
            Either::Right(
                web::Json(
                    SingleStepVictimInitResp{
                        gpa,
                        vaddr:program_desc.fn_vaddr,
                        expected_offsets: program_desc.expected_offsets,
                        has_cache_attack_data: false,
                        lookup_table_bytes: 0,
                        lookup_table_gpa: 0,
                        lookup_table_vaddr: 0,
                        mem_access_target_offset: vec![],
                        offsets_with_mem_access: vec![],
                    }
                )
            )
        },
        Some(v) => {
            Either::Right(
                web::Json(
                    SingleStepVictimInitResp{
                        gpa,
                        vaddr:program_desc.fn_vaddr,
                        expected_offsets: program_desc.expected_offsets,
                        has_cache_attack_data: true,
                        lookup_table_bytes: v.lookup_table_bytes,
                        lookup_table_gpa: v.lookup_table_gpa,
                        lookup_table_vaddr: v.lookup_table_vaddr,
                        mem_access_target_offset: v.mem_access_target_offset,
                        offsets_with_mem_access: v.offsets_with_mem_access,
                    }
                )
            )
        }
    }  
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    // Note: web::Data created _outside_ HttpServer::new closure
    let state = web::Data::new(AppState {
        page_pingponger: Mutex::new(None),
        running_victim_programs: Mutex::new(HashSet::new()),
    });

    const LISTEN : &str = "0.0.0.0:8080";
    eprintln!("Starting server on {}",LISTEN);
    HttpServer::new(move || {
        // move counter into the closure
        App::new()
            .app_data(state.clone()) // <- register the created data
            .route("/pingpong-pagetrack/init", web::post().to(handle_pingpong_pagetrack_init))
            .route("/pingpong-pagetrack/start", web::post().to(handle_pingpong_pagetrack_start))
            .route("/pingpong-pagetrack/teardown", web::post().to(handle_pingpong_pagetrack_teardown))
            .route("/single-step-victim/init", web::post().to(handle_single_step_victim_init))
            .route("/single-step-victim/start", web::post().to(handle_single_step_victim_start))
    })
    .bind(LISTEN)?
    .run()
    .await
}

