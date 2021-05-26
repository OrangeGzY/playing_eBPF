ifneq  ($(KERNELRELEASE),)
obj-m:=kprobe.o
else
KDIR := /root/linux-4.15/ #/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	rm -f *.o *.symvers *.cmd *.cmd.o *.mod.c
	cp ./kprobe.ko /root/xss_pwn/core/
clean:
	rm -f *.ko *.o *.symvers *.cmd *.cmd.o
endif
