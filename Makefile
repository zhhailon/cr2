name := cr2
obj-m += $(name).o
kobj := $(name).ko
KDIR := /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	insmod $(kobj)

remove:
	rmmod $(name)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
