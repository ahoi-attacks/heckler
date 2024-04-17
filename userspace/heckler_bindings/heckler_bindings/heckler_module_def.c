/*
 * Simple heckler python wrapper
 * around sev-step userspace library.
 * Some boilerplate code is borrowed from sev-step, gpl v2.
 */
#include <linux/kvm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <poll.h>
#include <signal.h>

#include "../../sev-step/sev-step-lib/sev_step_api.h"
#include "../../sev-step/sev-step-lib/ansi-color-codes.h"


#define EXIT_CODE_OK 0
#define EXIT_CODE_ERR 1
#define DEBUG 1

#define CONSTANT(m, x) PyModule_AddIntConstant(m, #x, x);
#define HECKLER_OK 0
#define HECKLER_ERR 1
#define HECKLER_ERR_ABORT 3

usp_poll_api_ctx_t ctx;
bool api_open = false;
bool debug = false;
bool ctx_created = false;

// XXX: We are not thread safe. only allow one vcpu at the time!
usp_event_type_t event_type;
void* event_buffer = NULL;

static PyObject* heckler_clean_up(PyObject* self, PyObject* args);
static PyObject* heckler_ack_event(PyObject* self, PyObject* args);
static PyObject* heckler_block_until_event(PyObject* self, PyObject* args);
static PyObject* heckler_enable_boot_tracking(PyObject* self, PyObject* args);
static PyObject* heckler_enable_tracking(PyObject* self, PyObject* args);
static PyObject* heckler_init(PyObject* self, PyObject* args);
static PyObject* heckler_track_all_pages(PyObject* self, PyObject* args);
static PyObject* heckler_untrack_all_pages(PyObject* self, PyObject* args);
static PyObject* heckler_enable_debug(PyObject* self, PyObject* args);
static PyObject* heckler_track_page(PyObject* self, PyObject* args);
static PyObject* heckler_untrack_page(PyObject* self, PyObject* args);
static PyObject* heckler_event_get_blob(PyObject* self, PyObject* args);
static PyObject* heckler_block_until_event_timeout(PyObject* self, PyObject* args);
static PyObject* heckler_inject(PyObject* self, PyObject* args);


static PyMethodDef module_methods[] = {
	{ "heckler_clean_up", heckler_clean_up, METH_VARARGS, "" },
	{ "heckler_ack_event", heckler_ack_event, METH_VARARGS, "" },
	{ "heckler_block_until_event", heckler_block_until_event, METH_VARARGS, "" },
	{ "heckler_enable_boot_tracking", heckler_enable_boot_tracking, METH_VARARGS, "" },
	{ "heckler_inject", heckler_inject, METH_VARARGS, "" },
	{ "heckler_enable_tracking", heckler_enable_tracking, METH_VARARGS, "" },
	{ "heckler_init", heckler_init, METH_VARARGS, "" },
	{ "heckler_track_all_pages", heckler_track_all_pages, METH_VARARGS, "" },
	{ "heckler_untrack_all_pages", heckler_untrack_all_pages, METH_VARARGS, "" },
	{ "heckler_enable_debug", heckler_enable_debug, METH_VARARGS, "" },
	{ "heckler_track_page", heckler_track_page, METH_VARARGS, "" },
	{ "heckler_untrack_page", heckler_untrack_page, METH_VARARGS, "" },
	{ "heckler_block_until_event_timeout", heckler_block_until_event_timeout, METH_VARARGS, "" },
	{ "heckler_block_until_event_stop", heckler_clean_up, METH_VARARGS, "" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef heckler_module = {
	PyModuleDef_HEAD_INIT,
	"heckler",
	"Python3 module for communication with heckler functions",
	-1,
	module_methods
};

PyMODINIT_FUNC PyInit_heckler_bindings(void)
{
	PyObject* mod = PyModule_Create(&heckler_module);
	CONSTANT(mod, HECKLER_OK);
	CONSTANT(mod, HECKLER_ERR);
	CONSTANT(mod, HECKLER_ERR_ABORT);
	return mod;
}

static void __close_ctx(void)
{
	if (ctx_created)
	{
		printf("closing ctx\n");
		if (SEV_STEP_ERR == usp_close_ctx(&ctx))
		{
			printf(BRED "usp_close_ctx failed." reset " Check dmesg for more information\n");
		}
	}
	else
	{
		printf("ctx was not created!\n");
	}
	ctx_created = false;
}

static void __clean_up(void)
{

	if (api_open)
	{
		api_open = false;
		printf("Closing API...\n");
		__close_ctx();
	}
	api_open = false;

}

void __crash_handler(int code)
{
	if (DEBUG)
	{
		printf("Crash handler called: code: %d\n", code);
	}
	__clean_up();
	exit(code);
}

static PyObject* heckler_enable_debug(PyObject* self, PyObject* args)
{
	debug = true;
	return Py_BuildValue("i", 0);
}

static PyObject* heckler_clean_up(PyObject* self, PyObject* args)
{
	__clean_up();
	return Py_BuildValue("i", 0);
}

static PyObject* heckler_block_until_event_stop(PyObject* self, PyObject* args)
{
	usp_block_stop();
	return Py_BuildValue("i", 0);
}

static PyObject* heckler_ack_event(PyObject* self, PyObject* args)
{
	int ret = 0;
	if (event_buffer != NULL)
	{
		free_usp_event(event_type, event_buffer);
		event_buffer = NULL;
		usp_ack_event(&ctx);
	}
	else
	{
		#if 0
		printf("acking empty event!\n");
		#endif
	}
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_inject(PyObject* self, PyObject* args)
{
	int ret = 0;
	unsigned long number;
	if (!PyArg_ParseTuple(args, "i", &number))
	{
		PyErr_Format(
			PyExc_Exception,
			"Function arguments are of wrong type"
		);
		ret = -1;
		goto end;
	}
	ret = inject_interrupt(&ctx, number);
	if (SEV_STEP_OK != ret)
	{
		ret = EXIT_CODE_ERR;
	}
end:
	return Py_BuildValue("i", ret);
}

struct abort_args
{
	size_t counter;
	size_t max;
};

static bool __should_abort(void* arg)
{
	struct abort_args* a = (struct abort_args*)arg;
	a->counter++;
	#if 0
	if (debug) printf("c: %ld, max: %ld\n", a->counter, a->max);
	#endif
	return a->counter >= a->max;
}

static PyObject* heckler_block_until_event_timeout(PyObject* self, PyObject* args)
{
	int ret = 0;
	unsigned long gpa;
	size_t max_count = 0;
	struct abort_args abort = {
		.counter = 0,
		.max = 0
	};

	if (event_buffer != NULL)
	{
		printf("memory leak? freeing event_buffer\n");
		free_usp_event(event_type, event_buffer);
	}
	if (!PyArg_ParseTuple(args, "K", &abort.max))
	{
		PyErr_Format(
			PyExc_Exception,
			"Function arguments are of wrong type"
		);
		__clean_up();
		ret = -1;
		goto end;
	}
	#if 0
	if (debug) printf("using max: %lld\n", abort.max);
	#endif

	/*
	 * Do not block the GIL on block until event
	 */
	Py_BEGIN_ALLOW_THREADS
		ret = usp_block_until_event_or_cb(&ctx, &event_type, &event_buffer, __should_abort, &abort);
	Py_END_ALLOW_THREADS
	if (ret == SEV_STEP_ERR)
	{
		#if 0
		printf("usp_block_until_event: %s:%d " BHRED "FAILED"reset, __FILE__, __LINE__);
		#endif
		ret = SEV_STEP_ERR;
		__clean_up();
		goto end;
	}
	if (ret == SEV_STEP_ERR_ABORT)
	{
		ret = SEV_STEP_ERR_ABORT;
		goto end;
	}
	if (event_type != PAGE_FAULT_EVENT)
	{
		printf("%s:%d : expected page fault event, got %d\n", __FILE__, __LINE__, event_type);
		ret = EXIT_CODE_ERR;
		__clean_up();
		goto end;
	}
	usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;
	if (debug)
	{
		printf(BHGRN "Pagefault Event: {GPA:0x%lx}\n"reset, pf_event->faulted_gpa);
	}
	gpa = pf_event->faulted_gpa;

end:
	if (ret != 0)
	{
		gpa = 0;
	}
	return Py_BuildValue("{s:i,s:K}",
		"ret", ret, "gpa", gpa);
}

static PyObject* heckler_block_until_event(PyObject* self, PyObject* args)
{
	int ret = 0;
	unsigned long gpa;

	if (event_buffer != NULL)
	{
		printf("memory leak? freeing event_buffer\n");
		free_usp_event(event_type, event_buffer);
	}

	/*
	 * Do not block the GIL on block until event
	 */
	Py_BEGIN_ALLOW_THREADS
		ret = usp_block_until_event(&ctx, &event_type, &event_buffer);
	Py_END_ALLOW_THREADS
	if (ret == SEV_STEP_ERR_ABORT)
	{
		ret = SEV_STEP_ERR_ABORT;
		goto end;
	}
	if (ret == SEV_STEP_ERR)
		if (event_type != PAGE_FAULT_EVENT)
		{
			printf("%s:%d : expected page fault event, got %d\n", __FILE__, __LINE__, event_type);
			ret = EXIT_CODE_ERR;
			__clean_up();
			goto end;
		}
	usp_page_fault_event_t* pf_event = (usp_page_fault_event_t*)event_buffer;

	if (debug)
	{
		printf(BHGRN "Pagefault Event: {GPA:0x%lx}\n"reset, pf_event->faulted_gpa);
	}
	gpa = pf_event->faulted_gpa;
end:
	if (ret != 0)
	{
		gpa = 0;
	}
	return Py_BuildValue("{s:i,s:K}",
		"ret", ret, "gpa", gpa);
}

static PyObject* heckler_enable_boot_tracking(PyObject* self, PyObject* args)
{
	int ret = 0;
	printf("heckler_enable_boot_tracking...\n");
	if (SEV_STEP_ERR == usp_new_ctx_boot(&ctx, false, true))
	{
		printf(BRED "usp_new_ctx failed." reset " Check dmesg for more information\n");
		ret = -1;
	}
	else
	{
		ctx_created = true;
	}
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_enable_tracking(PyObject* self, PyObject* args)
{
	int ret = 0;

	#if 0
	printf("Initializing API...\n");
   if (ctx_created)
   {
	   printf("existing context, closing first");
	   __close_ctx();
   }
	#endif

	if (SEV_STEP_ERR == usp_new_ctx_boot(&ctx, false, false))
	{
		printf(BRED "heckler_enable_tracking failed." reset " Check dmesg for more information\n");
		ret = -1;
	}
	else
	{
		ctx_created = true;
	}
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_track_page(PyObject* self, PyObject* args)
{
	int ret = 0;
	unsigned long gpa = 0;

	if (!PyArg_ParseTuple(args, "K", &gpa))
	{
		PyErr_Format(
			PyExc_Exception,
			"Function arguments are of wrong type"
		);
		__clean_up();
		ret = -1;
		goto end;
	}
	ret = track_page(&ctx, gpa, KVM_PAGE_TRACK_EXEC);
	if (SEV_STEP_OK != ret)
	{
		#if 0
		printf("track_page: %s:%d " BHRED" FAILED\n"reset, __FILE__, __LINE__);
		ret = EXIT_CODE_ERR;
		__clean_up();
		#endif
	}
end:
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_untrack_page(PyObject* self, PyObject* args)
{
	int ret = 0;
	unsigned long gpa = 0;

	if (!PyArg_ParseTuple(args, "K", &gpa))
	{
		PyErr_Format(
			PyExc_Exception,
			"Function arguments are of wrong type"
		);
		__clean_up();
		ret = -1;
		goto end;
	}
	if (SEV_STEP_OK != untrack_page(&ctx, gpa, KVM_PAGE_TRACK_EXEC))
	{
		printf("untrack_page: %s:%d " BHRED" FAILED\n"reset, __FILE__, __LINE__);
		ret = EXIT_CODE_ERR;
		__clean_up();
	}
end:
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_init(PyObject* self, PyObject* args)
{
	#if 0
	signal(SIGTERM, __crash_handler);
	signal(SIGABRT, __crash_handler);
	#endif
	signal(SIGINT, __crash_handler);
	signal(SIGSEGV, __crash_handler);
	api_open = true;
	return Py_BuildValue("i", 0);
}

static PyObject* heckler_track_all_pages(PyObject* self, PyObject* args)
{
	int ret = 0;
	if (SEV_STEP_OK != track_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
	{
		printf("track_all_pages: %s:%d " BHRED" FAILED\n"reset, __FILE__, __LINE__);
		ret = EXIT_CODE_ERR;
		__clean_up();
	}
	return Py_BuildValue("i", ret);
}

static PyObject* heckler_untrack_all_pages(PyObject* self, PyObject* args)
{
	int ret = 0;
	if (SEV_STEP_OK != untrack_all_pages(&ctx, KVM_PAGE_TRACK_EXEC))
	{
		printf("untrack_all_pages: %s:%d " BHRED" FAILED\n"reset, __FILE__, __LINE__);
		ret = EXIT_CODE_ERR;
		__clean_up();
	}
	return Py_BuildValue("i", ret);
}
