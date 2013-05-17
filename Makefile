# Recursive

SUBDIRS=rrnx

.PHONY: all $(SUBDIRS)
     
all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
