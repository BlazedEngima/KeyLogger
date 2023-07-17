ifneq ($(KERNELRELEASE),)

include Kbuild

else

KVERSION := $(shell uname -r)
PWD	:= $(shell pwd)
KBUILD ?= /lib/modules/$(KVERSION)/build

all:
	$(MAKE) -C $(KBUILD) M=$(PWD)

clean:
	$(MAKE) -C $(KBUILD) M=$(PWD) clean

.PHONY: all clean

endif
