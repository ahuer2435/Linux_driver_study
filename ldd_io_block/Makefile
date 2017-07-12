obj-m += test.o

KDIR := /lib/modules/`uname -r`/build
all:
	make -C $(KDIR) M=`pwd` modules
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order
