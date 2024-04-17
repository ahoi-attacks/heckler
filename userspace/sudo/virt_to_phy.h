/*
* Borrowed from Gist: https://gist.github.com/pleiadesian/41f00ebc41dbe91417866d466ecc62b7
*/
#ifndef _VIRT_TO_PHY_H_
#define _VIRT_TO_PHY_H_
#include <fcntl.h> /* open */
#include <stdint.h> /* uint64_t  */
#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <unistd.h> /* pread, sysconf */

typedef struct
{
	uint64_t pfn: 55;
	unsigned int soft_dirty: 1;
	unsigned int file_page: 1;
	unsigned int swapped: 1;
	unsigned int present: 1;
} PagemapEntry;

int pagemap_get_entry(PagemapEntry* entry, int pagemap_fd, uintptr_t vaddr);
int virt_to_phys_user(uintptr_t* paddr, uintptr_t vaddr);

#endif //_VIRT_TO_PHY_H_
