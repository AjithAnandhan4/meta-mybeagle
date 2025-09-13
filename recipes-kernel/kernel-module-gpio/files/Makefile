# Makefile for BeagleBone Black GPIO Driver - Yocto compatible

obj-m := gpio_driver.o

# Use Yocto's kernel build directory (set by BitBake)
KERNEL_SRC ?= $(KERNEL_DIR)

# If KERNEL_DIR is not set, fall back to system default
ifndef KERNEL_DIR
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
endif

# Build targets
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

modules_install: all
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules_install

install: modules_install

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

.PHONY: all clean install modules_install
