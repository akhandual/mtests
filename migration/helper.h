#define PAGE_SIZE	0x10000UL   /* 64KB */
#define HPAGE_SIZE	0x1000000UL /* 16MB */

#define NR_PAGES	128 /* 100352 */
#define ALLOC_SIZE	NR_PAGES * PAGE_SIZE
#define ALLOC_HSIZE	NR_PAGES * HPAGE_SIZE

#define NODE0		2
#define NODE1		3
#define MAX_NODE	256
#define SIZE_GB		1024UL * 1024UL * 1024UL

#define MPOL_MF_MOVE_MT  (1<<6) /* Use multi-threaded page copy routine */
#define MPOL_MF_MOVE_CONCUR (1<<7)

#define MEM_PATTERN_1	0xa
#define MEM_PATTERN_2	0xb
#define MEM_PATTERN_3	0xc

static void load_pattern(char *addr, unsigned long size, unsigned long pattern)
{
	memset(addr, pattern, size);
}

static void check_pattern(char *addr, unsigned long size, unsigned long pattern)
{
	unsigned long i;

	for (i = 0; i < size; i++) {
		if (addr[i] == pattern)
			continue;
		printf("Mismatch at addr[%lu] (expected: %lu present: %d)\n", i, pattern, addr[i]);
		return;
	}
}
double time_ms(struct timeval begin, struct timeval end)
{
	double total_time = 0;

	total_time = (end.tv_sec - begin.tv_sec) * 1000;	/* In ms */
	total_time += (end.tv_usec - begin.tv_usec) / 1000;	/* In ms */
	return total_time;
}

double get_bandwidth(unsigned long size, unsigned long msecs)
{
	double in_gb = 0;

	in_gb = (double) size / ((double) (SIZE_GB));
	return (in_gb * 1000) / (msecs);
}
