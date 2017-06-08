#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <numaif.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "helper.h"
#include "helper_hugetlb.h"

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	unsigned long mask;
	char *ptr;
	int ret;

	ptr = mmap(NULL, ALLOC_HPAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed");
		return -1;
	}

	mask = 0;
	mask |= 1UL << NODE0;
	ret = mbind(ptr, ALLOC_HPAGE, MPOL_BIND, &mask, MAX_NODE, 0);
        if (ret < 0) {
                perror("mbind() failed");
                return -1;
        }
	load_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);

	gettimeofday(&begin, NULL);
        ret = madvise(ptr, ALLOC_HPAGE, MADV_SOFT_OFFLINE);
	gettimeofday(&end, NULL);
        if (ret) {
                perror("madvise() failed");
                return -1;
        }
	check_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);
	return 0;
}
