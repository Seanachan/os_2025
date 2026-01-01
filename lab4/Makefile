KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += osfs.o

osfs-objs := super.o inode.o file.o dir.o osfs_init.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

mount:
	sudo insmod osfs.ko
	sudo mount -t osfs none mnt/
	sudo dmesg

unmount:
	sudo umount none
	sudo rmmod osfs.ko
	
test:
	@cd mnt
	@sudo touch test1.txt
	@sudo bash -c "echo 'I LOVE OSLAB' > test1.txt"
	cat test1.txt


