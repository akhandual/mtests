SUBDIRS := hugetlb migration oom thp misc mprotect

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ all

.PHONY: all $(SUBDIRS)

clean:
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean ); done

.PHONY: all clean $(SUBDIRS)
