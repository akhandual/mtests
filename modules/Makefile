obj-m = vmalloc_test.o kmalloc_test.o page_alloc_test.o memblock_dump.o

KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
