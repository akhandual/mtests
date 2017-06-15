#include <linux/module.h>
#include <linux/mm.h>
#include <asm/errno.h>
#include <linux/pagemap.h>
#include <linux/export.h>
#include <linux/memblock.h>

/*
extern int memblock_debug;
*/

static int __init memblock_dump_init(void)
{
	printk("memblock_dump loaded\n");

/*
	memblock_debug = 1;
*/

	memblock_dump_all();
	return 0;
}

static void __exit memblock_dump_exit(void)
{
	printk("memblock_dump unloaded\n");
}
module_init(memblock_dump_init);
module_exit(memblock_dump_exit);

MODULE_DESCRIPTION ("memblock_dump");
MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Anshuman Khandual");

