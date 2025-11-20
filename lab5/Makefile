ccflags-y := -I$(PWD)/inc

obj-m := hello1.o hello2.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
