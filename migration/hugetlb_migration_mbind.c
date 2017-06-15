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
	unsigned long mask;
	char *ptr;
	int ret;
	struct timeval begin, end;

	ptr = mmap(NULL, ALLOC_HPAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed");
		return -1;
	}

	printf("Check 1\n");
	mask = 0;
	mask |= 1UL << NODE0;
        ret = mbind(ptr, ALLOC_HPAGE, MPOL_BIND, &mask, MAX_NODE, 0);
        if (ret < 0) {
                perror("mbind() failed");
                return -1;
        }
	load_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);

	printf("Check 2\n");
	mask = 0;
	mask |= 1UL << NODE1;
	gettimeofday(&begin, NULL);
        ret = mbind(ptr, ALLOC_HPAGE, MPOL_BIND, &mask, MAX_NODE, MPOL_MF_STRICT | MPOL_MF_MOVE_ALL);
	gettimeofday(&end, NULL);
        if (ret) {
                perror("mbind() failed");
                return -1;
        }
	printf("Check 3\n");
	check_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);
	printf("Moved %d normal pages in %f msecs %f GBs\n", HTLB_NR, time_ms(begin, end), get_bandwidth(ALLOC_HPAGE, time_ms(begin, end)));
	return 0;
}
