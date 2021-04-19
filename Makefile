# SPDX-License-Identifier: GPL-2.0
MAKEFLAGS += -rR --no-print-directory

CROSS_COMPILE = aarch64-linux-gnu-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -g -nostdlib -mgeneral-regs-only
CFLAGS += -I $(shell pwd)/include

AFLAGS := -Wall
AFLAGS += -g -I $(shell pwd)/include -D__ASSEMBLY__

LDFLAGS := ?

export CFLAGS LDFLAGS AFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := yios


obj-y += arch/
obj-y += init/


all :
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(LD) -T $(TOPDIR)/arch/arm64/core/vmyios.lds -Map system.map -o $(TOPDIR)/$(TARGET).elf built-in.o
	$(OBJCOPY) $(TOPDIR)/$(TARGET).elf -O binary $(TARGET).bin


clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET).*

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET).*

QEMU_FLAGS  := -machine raspi4
QEMU_FLAGS  += -nographic

run:
	/work/github/qemu/aarch64-softmmu/qemu-system-aarch64 $(QEMU_FLAGS) -kernel $(TARGET).bin
debug:
	/work/github/qemu/aarch64-softmmu/qemu-system-aarch64 $(QEMU_FLAGS) -kernel $(TARGET).bin -S -s
