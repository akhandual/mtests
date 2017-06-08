#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <numaif.h>
#include <sys/mman.h>
#include <sys/time.h>

#define PAGE_SIZE 0x10000UL

static void last_alloc_attempt(unsigned long mapsize)
{
	unsigned long attempt, max;
	char *ptr;

	for (attempt = mapsize / 2; attempt < mapsize; attempt += PAGE_SIZE) {
		ptr = mmap(NULL, attempt, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED) {
			printf("0x%016lx request failed\n", attempt);
			break;
		}
		munmap(ptr, attempt);
		max = attempt;
	}
	printf("Max mmap size %lf GB\n", ((double) attempt / 1024 / 1024 / 1024));
}

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	unsigned long mapsize;
	char *ptr;
	int ret, i;

	mapsize = PAGE_SIZE;
	while (1) {
		ptr = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED) {
			printf("0x%016lx request failed\n", mapsize);
			last_alloc_attempt(mapsize);
			break;
		}
		munmap(ptr, mapsize);
		mapsize = mapsize * 2;
	}
	return 0;
}
