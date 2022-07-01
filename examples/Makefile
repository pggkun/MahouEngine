DIRECTORIES := $(sort $(dir $(wildcard ./*/)))

.PHONY: build, clean

build:
	@for dir in $(DIRECTORIES);\
	do \
		echo $(CURDIR)$${dir/./};  \
		cd $(CURDIR)$${dir/./} && $(MAKE) res && $(MAKE) build;  \
	done; \

clean:
	@for dir in $(DIRECTORIES);\
	do \
		echo $(CURDIR)$${dir/./};  \
		rm -rf $(CURDIR)$${dir/./}build $(CURDIR)$${dir/./}resources/*h;  \
	done; \
