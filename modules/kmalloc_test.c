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

static int __init kmalloc_test_init(void)
{
	void *buf;
	int count;

	printk("kmalloc_test loaded\n");
	for (count = 0; count < 25; count++) {	/* FIXME: 2 ^ 25 = 32MB is failing now */
		buf = kmalloc(1UL << count, GFP_KERNEL | __GFP_NOWARN);
		if (!buf) {
			printk("kmalloc() failed for %d order\n", count);
			break;
		}
		memset(buf, 0, 1UL << count);
		kfree(buf);
	}
	return 0;
}

static void __exit kmalloc_test_exit(void)
{
	printk("kmalloc_test unloaded\n");
}
module_init(kmalloc_test_init);
module_exit(kmalloc_test_exit);

MODULE_DESCRIPTION ("kmalloc_test");
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Anshuman Khandual");

