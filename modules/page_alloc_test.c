#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <linux/pagemap.h>

static int __init page_alloc_test_init(void)
{
	struct page *page;
	int count;
	void *vaddr;

	printk("page_alloc_test loaded\n");
	for (count = 0; count < 40; count++) {
		page = alloc_pages(GFP_KERNEL | __GFP_NOWARN, count);
		if (!page) {
			printk("alloc_pages() failed for %d order\n", count);
			break;
		}
		vaddr = __va(page_to_pfn(page) << PAGE_SHIFT);
		memset(vaddr, 0, PAGE_SIZE << count);
		/*
		memset(page, 0, PAGE_SIZE * (1UL << count));
		*/
		__free_pages(page, count);
	}
	return 0;
}

static void __exit page_alloc_test_exit(void)
{
	printk("page_alloc_test unloaded\n");
}
module_init(page_alloc_test_init);
module_exit(page_alloc_test_exit);

MODULE_DESCRIPTION ("page_alloc_test");
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Anshuman Khandual");

