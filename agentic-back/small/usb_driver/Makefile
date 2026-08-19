obj-m := simple_usb_driver.o

KVER := $(shell uname -r)

all:
	make -C /lib/modules/$(KVER)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVER)/build M=$(PWD) clean

install:
	make -C /lib/modules/$(KVER)/build M=$(PWD) modules_install

.PHONY: all clean install
