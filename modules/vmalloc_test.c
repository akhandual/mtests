#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <linux/pagemap.h>

static int __init vmalloc_test_init(void)
{
	void *buf;
	int count;

	printk("vmalloc_test loaded\n");
	for (count = 0; count < 33; count++) {
		buf = vmalloc(1UL << count);
		if (!buf) {
			printk("vmalloc() failed for %d order\n", count);
			break;
		}
		memset(buf, 0, 1UL << count);
		vfree(buf);
	}
	return 0;
}

static void __exit vmalloc_test_exit(void)
{
	printk("vmalloc_test unloaded\n");
}
module_init(vmalloc_test_init);
module_exit(vmalloc_test_exit);

MODULE_DESCRIPTION ("vmalloc_test");
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Anshuman Khandual");
