obj-m += mydrv.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) install
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
