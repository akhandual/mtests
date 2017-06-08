#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>

int main(int argc, char *argv[])
{
	char *ptr;
	unsigned long totalpages, pagesize, allocsize;

	pagesize = getpagesize();
	totalpages = get_phys_pages();
	allocsize = pagesize * totalpages;

	printf("page size: %lu \ntotal pages: %lu \ntotal size: %d GB\n",
					pagesize, totalpages, allocsize  >> 30);
	ptr = mmap(NULL, allocsize, PROT_READ |
			PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed");
		return -1;
	}
	mlock(ptr, allocsize);
	memset(ptr, 0, allocsize);
	return 0;
}
