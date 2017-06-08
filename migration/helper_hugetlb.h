#define HTLB_SIZE	0x1000000UL		/* 16MB */
#define HTLB_NR		1			/* HugeTLB pages */
#define ALLOC_HPAGE	HTLB_NR * HTLB_SIZE	/* HugeTLB alloc size */

#define MADV_HWPOISON 100
#define MADV_SOFT_OFFLINE 101

