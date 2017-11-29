KERNELDIR=/root/linux

obj-m:=led_dev.o

KDIR:=/root/linux
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
	rm -rf .tmp_versions
	rm -rf *.order
	rm -rf *.symvers
