# Makefile for the 'hello' kernel module (Lab 6 modified)

obj-m := hello.o

ccflags-y := -g

all:
	make -C $(KDIR) M=$(PWD) modules
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f hello.ko.unstripped
