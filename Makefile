# SPDX-License-Identifier: GPL-2.0
MAKEFLAGS += -rR --no-print-directory

ifeq ("$(origin V)", "command line")
	  MBUILD_VERBOSE = $(V)
  endif
  ifndef MBUILD_VERBOSE
	  MBUILD_VERBOSE = 0
  endif

ifeq ($(MBUILD_VERBOSE),1)
	  quiet =
	    Q =
    else
	  quiet=-s
	    Q = @
    endif

ifeq ("$(origin O)", "command line")
		O_LEVEL = $(O)
	endif
	ifndef O_LEVEL
		O_LEVEL = 2
	endif

export quiet Q MBUILD_VERBOSE

CROSS_COMPILE ?= aarch64-linux-gnu-
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
CFLAGS += -I $(TOPDIR)/include -I $(TOPDIR)/arch/arm64/include
CFLAGS += -fno-builtin -Werror-implicit-function-declaration

AFLAGS := -Wall
AFLAGS += -g -I $(TOPDIR)/arch/arm64/include -I $(TOPDIR)/include -D__ASSEMBLY__

LDFLAGS := --no-undefined

export CFLAGS LDFLAGS AFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := yios


obj-y += arch/
obj-y += init/
obj-y += lib/
obj-y += kernel/
obj-y += drivers/


all :
	$(Q) make $(quiet)  -f $(TOPDIR)/scripts/Makefile.build
	$(Q) echo "  LD      $(TARGET).elf"
	$(Q) $(LD) $(LDFLAGS) -T $(TOPDIR)/arch/arm64/lds/vmyios.lds -Map system.map -o $(TOPDIR)/$(TARGET).elf built-in.o
	$(Q) echo "  OBJCOPY $(TARGET).bin"
	$(Q) $(OBJCOPY) $(TOPDIR)/$(TARGET).elf -O binary $(TARGET).bin


clean:
	$(Q) echo "  CLEAN   all .o  *.dtb built-in.o"
	$(Q) echo "  CLEAN   yios.bin yios.elf"
	$(Q) rm -f $(shell find -name "*.o")
	$(Q) rm -f $(shell find -name "*.lds")
	$(Q) rm -f $(TARGET).*

distclean:
	$(Q) echo "  CLEAN   all .o .*.d *.dtb built-in.o"
	$(Q) echo "  CLEAN   yios.bin yios.elf"
	$(Q) rm -f $(shell find -name "*.o")
	$(Q) rm -f $(shell find -name "*.lds")
	$(Q) rm -f $(shell find -name "*.d")
	$(Q) rm -f $(TARGET).*

QEMU_FLAGS  := -machine raspi4
QEMU_FLAGS  += -nographic

run:
	/work/github/qemu/aarch64-softmmu/qemu-system-aarch64 $(QEMU_FLAGS) -kernel $(TARGET).bin
debug:
	/work/github/qemu/aarch64-softmmu/qemu-system-aarch64 $(QEMU_FLAGS) -kernel $(TARGET).bin -S -s
