use std::arch::{asm, global_asm};
use std::str::FromStr;
use crate::linux_memory::{ LinuxPageMap};


use anyhow::{bail, Context};

//dirty hack: sending memory or dyn trait objects betweens threads turned out to be cumbersome
//thus we use a global slice for now. Return to this later. Make sure to use an aligned
//ptr into the slice if you want to ensure cache line alignment
static UNALIGNED_MEM_BUFFER : &'static [u8] = &[0xff; 4* 4096];

#[derive(Hash,PartialEq, Eq,Clone, Copy)]
pub enum SingleStepTarget {
    NopSlide,
    AddSlide,
    RclSlide,
    LarSlide,
    Rdrand64Slide,
    Rdrand32Slide,
    DivSlideR16,
    DivSlideR32,
    DivSlide0R64,
    DivSlide1R64,
    DivSlide2R64,
    DivSlide3R64,
    DivSlide4R64,
    MulSlideR64,

    SimpleCacheTarget,
    SimpleCacheTargetLfence,
    EvalCacheTarget,
    EvalCacheTargetLfence,
}

pub struct CacheAttackTarget {
    /// Instruction offsets at which memory accesses are performed
    pub offsets_with_mem_access: Vec<u64>,
    /// Stores which offset in lookup table was accessed by the coresponding
    /// offsets_with_mem_access entry
    pub mem_access_target_offset: Vec<u64>,
    pub lookup_table_gpa: u64,
    pub lookup_table_vaddr: u64,
    pub lookup_table_bytes: u64
}

pub struct SingleStepTargetDesc {
    pub expected_offsets: Vec<u64>,
    pub execute:  Box< dyn Fn() -> ()>,
    pub fn_vaddr : u64,
    pub cache_attack_target : Option<CacheAttackTarget>,
}

impl ToString for SingleStepTarget {
    fn to_string(&self) -> String {
        match self {
            Self::NopSlide => String::from("NopSlide"),
            Self::AddSlide => String::from("AddSlide"),
            Self::RclSlide => String::from("RclSlide"),
            Self::LarSlide => String::from("LarSlide"),
            Self::Rdrand32Slide => String::from("Rdrand32Slide"),
            Self::Rdrand64Slide => String::from("Rdrand64Slide"),
            Self::DivSlideR16 => String::from("DivSlideR16"),
            Self::DivSlideR32 => String::from("DivSlideR32"),
            Self::DivSlide0R64 => String::from("DivSlide0R64"),
            Self::DivSlide1R64 => String::from("DivSlide1R64"),
            Self::DivSlide2R64 => String::from("DivSlide2R64"),
            Self::DivSlide3R64 => String::from("DivSlide3R64"),
            Self::DivSlide4R64 => String::from("DivSlide4R64"),
            Self::MulSlideR64 => String::from("MulSlideR64"),

            Self::SimpleCacheTarget => String::from("SimpleCacheTarget"),
            Self::SimpleCacheTargetLfence => String::from("SimpleCacheTargetLfence"),
            Self::EvalCacheTarget => String::from("EvalCacheTarget"),
            Self::EvalCacheTargetLfence => String::from("EvalCacheTargetLfence"),
        }
    }
}

impl FromStr for SingleStepTarget {
    type Err = anyhow::Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "NopSlide" => Ok(SingleStepTarget::NopSlide),
            "AddSlide" => Ok(SingleStepTarget::AddSlide),
            "RclSlide" => Ok(SingleStepTarget::RclSlide),
            "LarSlide" => Ok(SingleStepTarget::LarSlide),
            "Rdrand64Slide" => Ok(SingleStepTarget::Rdrand64Slide),
            "Rdrand32Slide" => Ok(SingleStepTarget::Rdrand32Slide),
            "DivSlideR16" => Ok(SingleStepTarget::DivSlideR16),
            "DivSlideR32" => Ok(SingleStepTarget::DivSlideR32),
            "DivSlide0R64" => Ok(SingleStepTarget::DivSlide0R64),
            "DivSlide1R64" => Ok(SingleStepTarget::DivSlide1R64),
            "DivSlide2R64" => Ok(SingleStepTarget::DivSlide2R64),
            "DivSlide3R64" => Ok(SingleStepTarget::DivSlide3R64),
            "DivSlide4R64" => Ok(SingleStepTarget::DivSlide4R64),
            "MulSlideR64" => Ok(SingleStepTarget::MulSlideR64),

            "SimpleCacheTarget" => Ok(SingleStepTarget::SimpleCacheTarget),
            "SimpleCacheTargetLfence" => Ok(SingleStepTarget::SimpleCacheTargetLfence),
            "EvalCacheTarget" => Ok(Self::EvalCacheTarget),
            "EvalCacheTargetLfence" => Ok(Self::EvalCacheTargetLfence),
            _ => bail!("not a valid victim program type"),
        }
    }
}

impl SingleStepTarget {
    pub fn to_single_step_target_desc(&self) -> anyhow::Result<SingleStepTargetDesc> {
        match self {
            SingleStepTarget::NopSlide => Ok(SingleStepTargetDesc {
                fn_vaddr: nop_slide_asm as u64,
                //these are obtained by manual analysis of nop_slide_asm
                expected_offsets: (1..0x3e8).collect(),
                execute: Box::new(|| { unsafe {nop_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::AddSlide => Ok(SingleStepTargetDesc {
                fn_vaddr: add_slide_asm as u64,
                // 4000 = 1000 * 4 bytes
                expected_offsets: (1..0xfa0).step_by(4).collect(),
                execute: Box::new(|| { unsafe {add_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::RclSlide => Ok(SingleStepTargetDesc {
                fn_vaddr: rcl_slide_asm as u64,
                // 4000 = 1000 * 4 bytes
                expected_offsets: (1..0xfa0).step_by(4).collect(),
                execute: Box::new(|| { unsafe {rcl_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::LarSlide => Ok(SingleStepTargetDesc {
                fn_vaddr: lar_slide_asm as u64,
                // 4000 = 1000 * 4 bytes
                expected_offsets: (1..0xfa0).step_by(4).collect(),
                execute: Box::new(|| { unsafe {lar_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::Rdrand64Slide => Ok(SingleStepTargetDesc {
                fn_vaddr: rdrand_slide_asm as u64,
                // 1000 * 4 bytes
                expected_offsets: (1..0xfa0).step_by(4).collect(),
                execute: Box::new(|| { unsafe {rdrand_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::Rdrand32Slide => Ok(SingleStepTargetDesc {
                fn_vaddr: rdrand32_slide_asm as u64,
                // 1000 * 3 bytes
                expected_offsets: (1..0xbb8).step_by(3).collect(),
                execute: Box::new(|| { unsafe {rdrand32_slide_asm()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::DivSlideR32 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide_asm_r32 as u64,
                // 240 * 17 bytes
                expected_offsets: vec![0x5, 0x5, 0x5, 0x2].into_iter().cycle().take(240*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide_asm_r32()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::DivSlide0R64 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide_asm_r64 as u64,
                // 170 * 24 bytes
                expected_offsets: vec![0x7, 0x7, 0x7, 0x3].into_iter().cycle().take(170*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide_asm_r64()}}),
                cache_attack_target: None
            }),
           SingleStepTarget::DivSlide1R64 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide1_asm_r64 as u64,
                // 170 * 24 bytes
                expected_offsets: vec![0x7, 0x7, 0x7, 0x3].into_iter().cycle().take(170*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide1_asm_r64()}}),
                cache_attack_target: None
            }),
           SingleStepTarget::DivSlide2R64 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide2_asm_r64 as u64,
                // 136 * 24 bytes
                expected_offsets: vec![0xa, 0x7, 0x7, 0x3].into_iter().cycle().take(136*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide2_asm_r64()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::DivSlide3R64 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide3_asm_r64 as u64,
                // 150 * 27
                expected_offsets: vec![0xa, 0x7, 0x7, 0x3].into_iter().cycle().take(150*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide3_asm_r64()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::DivSlide4R64 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide4_asm_r64 as u64,
                // 136 * 30
                expected_offsets: vec![0xa, 0x7, 0xa, 0x3].into_iter().cycle().take(136*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide4_asm_r64()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::DivSlideR16 => Ok(SingleStepTargetDesc {
                fn_vaddr: div_slide_asm_r16 as u64,
                // 227 * 18 
                expected_offsets: vec![0x5, 0x5, 0x5, 0x3].into_iter().cycle().take(227*4).collect::<Vec<_>>().iter().scan(0, |state, &x| {*state += x; Some(*state)}).collect(),
                execute: Box::new(|| { unsafe {div_slide_asm_r16()}}),
                cache_attack_target: None
            }),
            SingleStepTarget::MulSlideR64 => Ok(SingleStepTargetDesc {
                fn_vaddr: mul_slide_asm_r64 as u64,
                // 1000 * 3 bytes
                expected_offsets: (1..0xbb8).step_by(3).collect(),
                execute: Box::new(|| { unsafe {mul_slide_asm_r64()}}),
                cache_attack_target: None
            }),

            SingleStepTarget::SimpleCacheTarget => {
              
                    let table_vaddr;
                    unsafe {
                        let aligned_membuffer_ptr = UNALIGNED_MEM_BUFFER.as_ptr().add(UNALIGNED_MEM_BUFFER.as_ptr().align_offset(4096));
                        table_vaddr = aligned_membuffer_ptr as u64;
                    }
                    let table_bytes = 16*64;
                    let mut pagemap_parser =  LinuxPageMap::new().context("failed to instantiate pagemap parser")?;

                    let table_gpa = pagemap_parser
                        .get_phys(table_vaddr)
                        .context( "failed to get gpa for table vaddr")?;

                    unsafe {
                        simple_cache_target_asm(table_vaddr as u64, table_bytes);
                    }

                Ok(
                    SingleStepTargetDesc {
                        fn_vaddr: simple_cache_target_asm as u64,
                        expected_offsets: vec![
                            0x01,
                            0x02,
                            0x09,
                            0x0d,
                            0x14,
                            0x1b,
                            ],
                        execute: Box::new(move || { unsafe{
                            simple_cache_target_asm(table_vaddr as u64, table_bytes)
                        }}),
                    cache_attack_target: Some(CacheAttackTarget{
                        lookup_table_gpa: table_gpa,
                        lookup_table_vaddr: table_vaddr,
                        lookup_table_bytes: table_bytes,
                        offsets_with_mem_access: vec![
                            0x09,
                            0x0d,
                            0x14,
                            ],
                        mem_access_target_offset: vec![
                            1* 64,
                            5* 64,
                            14 * 64,
                            ]
                    }),
                }
                )
            },
            SingleStepTarget::SimpleCacheTargetLfence => {
              
                let table_vaddr;
                unsafe {
                    let aligned_membuffer_ptr = UNALIGNED_MEM_BUFFER.as_ptr().add(UNALIGNED_MEM_BUFFER.as_ptr().align_offset(4096));
                    table_vaddr = aligned_membuffer_ptr as u64;
                }
                let table_bytes = 16*64;
                let mut pagemap_parser =  LinuxPageMap::new().context("failed to instantiate pagemap parser")?;

                let table_gpa = pagemap_parser
                    .get_phys(table_vaddr)
                    .context( "failed to get gpa for table vaddr")?;

                unsafe {
                    simple_cache_target_lfence_asm(table_vaddr as u64, table_bytes);
                }

            Ok(
                SingleStepTargetDesc {
                    fn_vaddr: simple_cache_target_lfence_asm as u64,
                    expected_offsets: vec![
                        0x01,
                        0x02,
                        0x09,
                        0x0c,
                        0x10,
                        0x13,
                        0x1a,
                        0x1d,
                        0x24,
                        0x27,
                        ],
                    execute: Box::new(move || { unsafe{
                        simple_cache_target_lfence_asm(table_vaddr as u64, table_bytes)
                    }}),
                cache_attack_target: Some(CacheAttackTarget{
                    lookup_table_gpa: table_gpa,
                    lookup_table_vaddr: table_vaddr,
                    lookup_table_bytes: table_bytes,
                    offsets_with_mem_access: vec![
                        0x0c,
                        0x13,
                        0x1d,
                        ],
                    mem_access_target_offset: vec![
                        1* 64,
                        5* 64,
                        14 * 64,
                        ]
                }),
            }
            )
            },
            SingleStepTarget::EvalCacheTarget => {
              
                let table_vaddr;
                unsafe {
                    let aligned_membuffer_ptr = UNALIGNED_MEM_BUFFER.as_ptr().add(UNALIGNED_MEM_BUFFER.as_ptr().align_offset(4096));
                    table_vaddr = aligned_membuffer_ptr as u64;
                }
                let table_bytes = 16*64;
                let mut pagemap_parser =  LinuxPageMap::new().context("failed to instantiate pagemap parser")?;

                let table_gpa = pagemap_parser
                    .get_phys(table_vaddr)
                    .context( "failed to get gpa for table vaddr")?;

                unsafe {
                    eval_cache_target_asm(table_vaddr as u64, table_bytes);
                }

                Ok(
                    SingleStepTargetDesc {
                        fn_vaddr: eval_cache_target_asm as u64,
                        expected_offsets: vec![
                            0x01,
                            0x02,
                            0x09,
                            0x0d,
                            0x14,
                            0x18,
                            0x1f,
                            0x23,
                            0x2a,
                            0x2e,
                            0x35,
                            0x39,
                            0x40,
                            0x44,
                            0x4b,
                            0x4f,
                            0x56,
                            0x5a,
                            0x61,
                            0x65,
                            0x6c,
                            0x70,
                            0x77,
                            ],
                        execute: Box::new(move || { unsafe{
                            eval_cache_target_asm(table_vaddr as u64, table_bytes)
                        }}),
                    cache_attack_target: Some(CacheAttackTarget{
                        lookup_table_gpa: table_gpa,
                        lookup_table_vaddr: table_vaddr,
                        lookup_table_bytes: table_bytes,
                        offsets_with_mem_access: vec![
                            0x09,
                            0x0d,
                            0x14,
                            0x18,
                    
                            0x1f,
                            0x23,
                            0x2a,
                            0x2e,

                            0x35,
                            0x39,
                            0x40,
                            0x44,

                            0x4b,
                            0x4f,
                            0x56,
                            0x5a,

                            0x61,
                            0x65,
                            0x6c,
                            0x70,
                            ],
                        mem_access_target_offset: vec![
                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,
         
                            ]
                        }),
                })
            },
            SingleStepTarget::EvalCacheTargetLfence => {
              
                let table_vaddr;
                unsafe {
                    let aligned_membuffer_ptr = UNALIGNED_MEM_BUFFER.as_ptr().add(UNALIGNED_MEM_BUFFER.as_ptr().align_offset(4096));
                    table_vaddr = aligned_membuffer_ptr as u64;
                }
                let table_bytes = 16*64;
                let mut pagemap_parser =  LinuxPageMap::new().context("failed to instantiate pagemap parser")?;

                let table_gpa = pagemap_parser
                    .get_phys(table_vaddr)
                    .context( "failed to get gpa for table vaddr")?;

                unsafe {
                    eval_cache_target_lfence_asm(table_vaddr as u64, table_bytes);
                }

                Ok(
                    SingleStepTargetDesc {
                        fn_vaddr: eval_cache_target_lfence_asm as u64,
                        expected_offsets: vec![
                            0x01,
                            0x02,
                            0x09,
                            0x0d,
                            0x10,
                            0x17,
                            0x1a,
                            0x1e,
                            0x21,
                            0x28,
                            0x2b,
                            0x2f,
                            0x32,
                            0x39,
                            0x3c,
                            0x40,
                            0x43,
                            0x4a,
                            0x4d,
                            0x51,
                            0x54,
                            0x5b,
                            0x5e,
                            0x62,
                            0x65,
                            0x6c,
                            0x6f,
                            0x73,
                            0x76,
                            0x7d,
                            0x80,
                            0x84,
                            0x87,
                            0x8e,
                            0x91,
                            0x95,
                            0x98,
                            0x9f,
                            0xa2,
                            0xa6,
                            0xa9,
                            0xb0,
                            0xb3,
                            ],
                        execute: Box::new(move || { unsafe{
                            eval_cache_target_lfence_asm(table_vaddr as u64, table_bytes)
                        }}),
                    cache_attack_target: Some(CacheAttackTarget{
                        lookup_table_gpa: table_gpa,
                        lookup_table_vaddr: table_vaddr,
                        lookup_table_bytes: table_bytes,
                        offsets_with_mem_access: vec![
                            0x09,
                            0x10,
                            0x1a,
                            0x21,
                            0x2b,
                            0x32,
                            0x3c,                    
                            0x43,
                            0x4d,
                            0x54,
                            0x5e,
                            0x65,
                            0x6f,
                            0x76,
                            0x80,
                            0x87,
                            0x91,
                            0x98,
                            0xa2,
                            0xa9,
                            ],
                        mem_access_target_offset: vec![
                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,

                            1  * 64,
                            15 * 64,
                            1  * 64,
                            15 * 64,
         
                            ]
                        }),
                })
            },
        }
    }
}


global_asm!(include_str!("aligned_exec_fns.s"));
extern "C" {
    fn nop_slide_asm();
    /// NEMESIS
    fn add_slide_asm();
    fn rcl_slide_asm();
    fn lar_slide_asm();
    fn rdrand32_slide_asm();
    fn rdrand_slide_asm();
    fn div_slide_asm_r16();
    fn div_slide_asm_r32();
    fn div_slide_asm_r64();
    fn div_slide1_asm_r64();
    fn div_slide2_asm_r64();
    fn div_slide3_asm_r64();
    fn div_slide4_asm_r64();
    fn mul_slide_asm_r64();

    fn simple_cache_target_lfence_asm(lookup_table_addr : u64, size : u64);
    fn simple_cache_target_asm(lookup_table_addr : u64, size : u64);
    fn eval_cache_target_asm(lookup_table_addr : u64, size : u64);
    fn eval_cache_target_lfence_asm(lookup_table_addr : u64, size : u64);
}


#[inline(never)]
pub fn foo() {
    unsafe {
        asm!(".align 4096","nop");
    };
}

#[inline(never)]
pub fn bar() {
    unsafe {
        asm!(".align 4096","nop","nop");
    };
}
