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
	char *ptr;
	int ret = 0, count = 0;

	ptr = mmap(NULL, ALLOC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed for ptr");
		return -1;
	}

	ret = madvise(ptr, ALLOC_SIZE / 2, MADV_HUGEPAGE);
	if (ret) {
		perror("madvise() failed");
		return -1;
	}

	ret = madvise(ptr + ALLOC_SIZE / 2, ALLOC_SIZE / 2, MADV_NOHUGEPAGE);
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
	load_pattern(ptr, ALLOC_SIZE, MEM_PATTERN_1);

	mask = 0;
	mask |= 1UL << NODE1;
	gettimeofday(&begin, NULL);
	ret = mbind(ptr, ALLOC_SIZE, MPOL_BIND, &mask, MAX_NODE, MPOL_MF_STRICT | MPOL_MF_MOVE_ALL | MPOL_MF_MOVE_MT | MPOL_MF_MOVE_CONCUR);
	gettimeofday(&end, NULL);
        if (ret < 0) {
                perror("mbind() failed for ptr");
                return -1;
        }
	check_pattern(ptr, ALLOC_SIZE, MEM_PATTERN_1);

	printf("Moved %lu huge pages in %f msecs %f GBs\n", ALLOC_SIZE / HPAGE_SIZE, time_ms(begin, end), get_bandwidth(NR_PAGES * PAGE_SIZE, time_ms(begin, end)));
	return 0;
}
