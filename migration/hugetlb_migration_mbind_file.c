#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <numaif.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "helper.h"
#include "helper_hugetlb.h"

int main(int argc, char *argv[])
{
	unsigned long mask;
	char *ptr;
	int fd, ret;
	struct timeval begin, end;

	fd = open("/mnt/hugepages/test", O_CREAT | O_RDWR, 0755);
	if (fd == -1) {
		perror("opne() failed");
		return -1;
	}

	ptr = mmap(NULL, ALLOC_HPAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_HUGETLB, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed for ptr");
		return -1;
	}

	mask = 0;
	mask |= 1UL << NODE0;
        ret = mbind(ptr, ALLOC_HPAGE, MPOL_BIND, &mask, MAX_NODE, 0);
        if (ret < 0) {
                perror("mbind() failed for");
                return -1;
        }
	load_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);

	mask = 0;
	mask |= 1UL << NODE1;
	gettimeofday(&begin, NULL);
        ret = mbind(ptr, ALLOC_SIZE, MPOL_BIND, &mask, MAX_NODE, MPOL_MF_STRICT | MPOL_MF_MOVE_ALL);
	gettimeofday(&end, NULL);
        if (ret < 0) {
                perror("mbind() failed for ptr");
                return -1;
        }
	check_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);
	close(fd);
	unlink("/mnt/hugepages/test");
	printf("Moved %d normal pages in %f msecs %f GBs\n", HTLB_NR, time_ms(begin, end), get_bandwidth(NR_PAGES * PAGE_SIZE, time_ms(begin, end)));
	return 0;
}
