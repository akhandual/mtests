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
	char *ptr, *tmp;
	int ret, i;

	ptr = mmap(NULL, ALLOC_HPAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("map() failed for ptr");
		return -1;
	}

	load_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);

	ret = madvise(ptr, ALLOC_HPAGE, MADV_SOFT_OFFLINE);
	if (ret) {
		perror("madvise() failed for ptr");
		return -1;
	}

	/*
	tmp = ptr;
	for(i = 0; i < 255; i++) {
		ret = madvise(tmp + i * PAGE_SIZE, PAGE_SIZE, MADV_SOFT_OFFLINE);
	        if (ret) {
	                perror("madvise() failed for ptr");
			return -1;
		}
	}
	*/
	check_pattern(ptr, ALLOC_HPAGE, MEM_PATTERN_1);
	return 0;
}
