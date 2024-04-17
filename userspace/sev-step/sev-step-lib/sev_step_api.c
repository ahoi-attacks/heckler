/*
 * Taken from sev-step
 */
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include "sev_step_api.h"
#include "raw_spinlock.h"

#include "sev_step_print_macros.h"

const int SHARED_MEM_BYTES = 20 * 4096;

static volatile int keepRunning = 1;

void sig_handler(int dummy);

void free_usp_event(usp_event_type_t type, void* event)
{
	if (type == SEV_STEP_EVENT)
	{
		sev_step_event_t* step_event = (sev_step_event_t*)event;
		if (step_event->cache_attack_timings != NULL)
		{
			free(step_event->cache_attack_timings);
		}
		if (step_event->cache_attack_perf_values != NULL)
		{
			free(step_event->cache_attack_perf_values);
		}
	}
	free(event);
}

int
usp_poll_event(usp_poll_api_ctx_t* ctx, int* got_event, usp_event_type_t* event_type, void** event)
{

	raw_spinlock_lock(&ctx->shared_mem_region->spinlock);
	if (!ctx->shared_mem_region->have_event)
	{
		*got_event = 0;
		raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
		//no an event is NOT an error
		return SEV_STEP_OK;
	}

	//if we are here, we hold the lock and there was an event
	*got_event = 1;
	ctx->vm_is_paused = true;
	*event_type = ctx->shared_mem_region->event_type;

	uint64_t event_bytes;
	uint64_t event_buffer_offset = 0;
	if (get_size_for_event(*event_type, &event_bytes))
	{
		fprintf(stderr, "get_size_for_event failed\n");
		return SEV_STEP_ERR;
	}
	*event = malloc(event_bytes);
	memcpy(*event, ctx->shared_mem_region->event_buffer + event_buffer_offset, event_bytes);
	event_buffer_offset += event_bytes;

	#if 0
	if (*event_type == SEV_STEP_EVENT)
	{
		sev_step_event_t* step_event = (sev_step_event_t * )(*event);
		if (step_event->cache_attack_timings != NULL)
		{
			//fprintf(stderr,"step event with timing data. %lu entries\n",step_event->cache_attack_data_len);
			uint64_t cache_attack_timings_bytes =
				sizeof(uint64_t) * step_event->cache_attack_data_len;
			//the ptr is still from kernel space, we need to copy the data out of the event buffer anyways
			step_event->cache_attack_timings = malloc(cache_attack_timings_bytes);
			memcpy(step_event->cache_attack_timings,
				ctx->shared_mem_region->event_buffer + event_buffer_offset,
				cache_attack_timings_bytes
			);
			event_buffer_offset += cache_attack_timings_bytes;
		}
		if (step_event->cache_attack_perf_values != NULL)
		{
			//fprintf(stderr,"step event with perf data. %lu entries\n",step_event->cache_attack_data_len);
			uint64_t cache_attack_perf_bytes = sizeof(uint64_t) * step_event->cache_attack_data_len;
			//the ptr is still from kernel space, we need to copy the data out of the event buffer anyways
			step_event->cache_attack_perf_values = malloc(cache_attack_perf_bytes);
			memcpy(step_event->cache_attack_perf_values,
				ctx->shared_mem_region->event_buffer + event_buffer_offset,
				cache_attack_perf_bytes
			);
			event_buffer_offset += cache_attack_perf_bytes;
		}
	}
	#endif

	raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
	return SEV_STEP_OK;

}

void sig_handler(int dummy)
{
	(void)dummy;
	keepRunning = 0;
}

void usp_block_stop(void)
{
	keepRunning = 0;
	printf("keep running = 0\n");
}

int usp_block_until_event(usp_poll_api_ctx_t* ctx,
	usp_event_type_t* event_type,
	void** event)
{
	int err;
	int got_event;

	if (ctx->vm_is_paused)
	{
		flf_printf("ctx-vm_is_paused is set to true, this will probably deadlock!\n");
	}

	// interrupt while loop
	signal(SIGINT, sig_handler);
	keepRunning = 1;
	while (keepRunning)
	{
		err = usp_poll_event(ctx, &got_event, event_type, event);
		if (err)
		{
			fprintf(stderr, "usp_poll_event failed with %d\n", err);
			return SEV_STEP_ERR;
		}
		if (got_event)
		{
			return SEV_STEP_OK;
		}
	}
	printf("Abort to prevent freeze.\n");
	return SEV_STEP_ERR_ABORT;
}

int usp_block_until_event_or_cb(usp_poll_api_ctx_t* ctx,
	usp_event_type_t* event_type,
	void** event, bool(should_abort)(void*),
	void* should_abort_args)
{
	int err;
	int got_event;

	if (ctx->vm_is_paused)
	{
		flf_printf("ctx-vm_is_paused is set to true, this will probably deadlock!\n");
	}

	// interrupt while loop
	signal(SIGINT, sig_handler);
	keepRunning = 1;
	while (keepRunning)
	{
		usleep(1000);
		err = usp_poll_event(ctx, &got_event, event_type, event);
		if (err)
		{
			fprintf(stderr, "usp_poll_event failed with %d\n", err);
			return SEV_STEP_ERR;
		}
		if (got_event)
		{
			return SEV_STEP_OK;
		}
		if (should_abort(should_abort_args))
		{
			return SEV_STEP_ERR_ABORT;
		}
	}
	printf("Abort to prevent freeze.\n");
	return SEV_STEP_ERR_ABORT;
}

bool is_vm_paused(usp_poll_api_ctx_t* ctx)
{
	return ctx->vm_is_paused;
}

void usp_ack_event(usp_poll_api_ctx_t* ctx)
{
	raw_spinlock_lock(&ctx->shared_mem_region->spinlock);
	if (ctx->shared_mem_region->event_acked)
	{
		fprintf(stderr, "usp_ack_event was called but ctx->shared_mem_region->event_acked was true");
	}

	// set status flags
	ctx->shared_mem_region->event_acked = 1;
	ctx->shared_mem_region->have_event = 0;
	ctx->vm_is_paused = false;
	raw_spinlock_unlock(&ctx->shared_mem_region->spinlock);
}

int get_size_for_event(usp_event_type_t event_type, uint64_t* size)
{
	switch (event_type)
	{
	case PAGE_FAULT_EVENT:
		*size = sizeof(usp_page_fault_event_t);
		return SEV_STEP_OK;
	case SEV_STEP_EVENT:
		*size = sizeof(sev_step_event_t);
		return SEV_STEP_OK;
	default:
		return SEV_STEP_ERR;
	}
}

int track_page(usp_poll_api_ctx_t* ctx, uint64_t gpa, enum kvm_page_track_mode mode)
{
	track_page_param_t tp = {
		.gpa = gpa,
		.track_mode = mode,
	};
	if (ioctl(ctx->kvm_fd, KVM_TRACK_PAGE, &tp) < 0)
	{
		perror("track_page: Error calling KVM_TRACK_PAGE ioctl");
		return SEV_STEP_ERR;
	}
	return SEV_STEP_OK;
}

int inject_interrupt(usp_poll_api_ctx_t* ctx, int vector)
{
	inject_interrupt_t v = {
		.vector = vector
	};
	if (ioctl(ctx->kvm_fd, KVM_INJECT_INTERRUPT, &v) < 0)
	{
		perror("inject_interrupt: Error calling KVM_INJECT_INTERRUPT ioctl");
		return SEV_STEP_ERR;
	}
	return SEV_STEP_OK;
}

int untrack_page(usp_poll_api_ctx_t* ctx, uint64_t gpa, enum kvm_page_track_mode mode)
{
	track_page_param_t tp = {
		.gpa = gpa,
		.track_mode = mode,
	};
	if (ioctl(ctx->kvm_fd, KVM_UNTRACK_PAGE, &tp) < 0)
	{
		perror("untrack_page: Error calling KVM_UNTRACK_PAGE ioctl");
		return SEV_STEP_ERR;
	}
	return SEV_STEP_OK;
}

int track_all_pages(usp_poll_api_ctx_t* ctx, enum kvm_page_track_mode mode)
{
	track_all_pages_t param = {
		.track_mode = mode,
	};
	if (ioctl(ctx->kvm_fd, KVM_TRACK_ALL_PAGES, &param) < 0)
	{
		perror("track_all_pages: Error calling KVM_TRACK_ALL_PAGES ioctl");
		return SEV_STEP_ERR;
	}

	return SEV_STEP_OK;
}

int untrack_all_pages(usp_poll_api_ctx_t* ctx, enum kvm_page_track_mode mode)
{
	track_all_pages_t param = {
		.track_mode = mode,
	};
	if (ioctl(ctx->kvm_fd, KVM_UNTRACK_ALL_PAGES, &param) < 0)
	{
		perror("untrack_all_pages: Error calling KVM_UNTRACK_ALL_PAGES ioctl");
		return SEV_STEP_ERR;
	}
	return SEV_STEP_OK;
}

int usp_new_ctx_boot(usp_poll_api_ctx_t* ctx, bool debug_mode, bool track_boot)
{
	//alloc memory for shared mem
	int shared_mem_struct_bytes = sizeof(shared_mem_region_t);
	if (shared_mem_struct_bytes > SHARED_MEM_BYTES)
	{
		fprintf(stderr, "shared_mem_region_t struct has %d bytes which is larger "
						"than the %d shared mem backing buffer\n",
			shared_mem_struct_bytes, SHARED_MEM_BYTES);
		return SEV_STEP_ERR;
	}
	ctx->shared_mem_region = mmap(NULL, SHARED_MEM_BYTES,
		PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	if (ctx->shared_mem_region == NULL)
	{
		fprintf(stderr, "Failed to mmap memory for shared memory region\n");
		return SEV_STEP_ERR;
	}
	memset((void*)ctx->shared_mem_region, 0, SHARED_MEM_BYTES);

	//initialize shared mem struct
	INIT_SPINLOCK(&ctx->shared_mem_region->spinlock);
	ctx->shared_mem_region->event_acked = 1;
	ctx->shared_mem_region->have_event = 0;

	usp_init_poll_api_t params = {
		.pid = getpid(),
		.user_vaddr_shared_mem = (uint64_t)ctx->shared_mem_region,
		.decrypt_vmsa = debug_mode,
		.track_boot = track_boot,
	};

	//init user/kernel poll api
	int kvm = open("/dev/kvm", O_RDWR | O_CLOEXEC);
	if (kvm < 0)
	{
		perror("Error calling open /dev/kvm");
		return SEV_STEP_ERR;
	}

	ctx->kvm_fd = kvm;
	int err = ioctl(ctx->kvm_fd, KVM_USP_INIT_POLL_API, &params);
	if (err < 0)
	{
		perror("Error calling KVM_USP_INIT_POLL_API ioctl");
		return SEV_STEP_ERR;
	}
	ctx->vm_is_paused = false;
	return SEV_STEP_OK;
}

int usp_new_ctx(usp_poll_api_ctx_t* ctx, bool debug_mode)
{
	return usp_new_ctx_boot(ctx, debug_mode, false);
}

int usp_close_ctx(usp_poll_api_ctx_t* ctx)
{
	enum kvm_page_track_mode modes[] = {
		KVM_PAGE_TRACK_EXEC
	};
	for (uint64_t i = 0; i < sizeof(modes) / sizeof(modes[0]); i++)
	{
		if (SEV_STEP_OK != untrack_all_pages(ctx, modes[i]))
		{
			flf_printf("untrack_all_pages with mode %d failed\n", modes[i]);
		}
	}

	int err = ioctl(ctx->kvm_fd, KVM_USP_CLOSE_POLL_API, NULL);
	if (err < 0)
	{
		perror("Error calling KVM_USP_INIT_POLL_API ioctl");
		return SEV_STEP_ERR;
	}

	close(ctx->kvm_fd);
	munmap(ctx->shared_mem_region, SHARED_MEM_BYTES);
	ctx->vm_is_paused = false;
	return SEV_STEP_OK;
}