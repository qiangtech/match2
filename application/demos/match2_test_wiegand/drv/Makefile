
KERNELDIR := /home/user/NUC970_Buildroot-master/nuc980bsp/linux-4.4.x
CURREN_PATH := $(shell pwd)

obj-m := test_drv.o

build:kernel_modules


kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURREN_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURREN_PATH) clean
	  