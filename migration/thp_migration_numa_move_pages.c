#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <numaif.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "helper.h"

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	unsigned long mask;
	char *ptr, *addr[NR_PAGES];
	int nodes[NR_PAGES], status[NR_PAGES], ret;

	ptr = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed for ptr");
		return -1;
	}

	ret = madvise(ptr, ALLOC_SIZE, MADV_HUGEPAGE);
	if (ret) {
		perror("madvise() failed");
		return -1;
	}

	mask = 0;
	mask |= 1UL << NODE0;
        ret = mbind(ptr, ALLOC_SIZE, MPOL_BIND, &mask, MAX_NODE, MPOL_MF_STRICT);
        if (ret < 0) {
                perror("mbind() failed for ptr");
                return -1;
        }
	mlock(ptr, ALLOC_SIZE);
	memset(ptr, 1, ALLOC_SIZE);

	for (ret = 0; ret < NR_PAGES; ret++) {
		addr[ret] = ptr + ret * PAGE_SIZE;
		nodes[ret] = NODE1;
		status[ret] = -123;
	}

	load_pattern(ptr, ALLOC_SIZE, MEM_PATTERN_1);
	gettimeofday(&begin, NULL);
	ret = numa_move_pages(0, NR_PAGES, (void **) addr, nodes, status, MPOL_MF_MOVE_ALL);
	gettimeofday(&end, NULL);
	if (ret < 0) {
		perror("numa_move_pages() failed");
		return -1;
	}
	check_pattern(ptr, ALLOC_SIZE, MEM_PATTERN_1);

	printf("Moved %lu huge pages in %f msecs %f GBs\n", ALLOC_SIZE / HPAGE_SIZE, time_ms(begin, end), get_bandwidth(NR_PAGES * PAGE_SIZE, time_ms(begin, end)));
	return 0;
}
