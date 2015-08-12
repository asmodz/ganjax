
BOOT_PATH=src/boot
KERNEL_PATH=src/kernel
LIB_PATH=src/lib
BINARY_PATH=bin

BOOTLOADER_NAME=boot.bin
BOOTLOADER_FILE=boot_stage0.asm

KERNEL_NAME=KERNEL.SYS
KERNEL_FILE=kernel.asm


IMAGE_NAME=ganjax.flp

ASM=nasm
ASM_FLAGS = -f bin

CLANG_FLAGS= -Os -std=gnu99 -ffreestanding -fno-builtin -m16
CC=clang-3.6

KERNEL_NAME=kernel.c
KERNEL_FILE=kernel.o
KERNEL_BIN=KERNEL.SYS

SYSROOT_INCLUDE=src/lib

LD=ld
LD_SCRIPT_PATH=$(KERNEL_PATH)/kernel.ld
LD_FLAGS = -nostdlib --nmagic 
LD_LINKLIB = $(BINARY_PATH)/tinylibc.o \
	$(BINARY_PATH)/_int_k_io.o

OBJCOPY=objcopy
OBJCOPY_FLAGS=-O binary

all: bootloader _internal_kernel_io tinyc kernel image

bootloader: $(BOOT_PATH)/$(BOOTLOADER_FILE)
	$(ASM) $(ASM_FLAGS) -o $(BINARY_PATH)/$(BOOTLOADER_NAME) \
	$(BOOT_PATH)/$(BOOTLOADER_FILE)

kernel:
	$(CC) -isysroot=$(SYSROOT_INCLUDE) -I $(SYSROOT_INCLUDE) \
	-c $(CLANG_FLAGS) $(KERNEL_PATH)/kernel.c -o $(BINARY_PATH)/kernel.o 
	
	$(LD) -static -T $(LD_SCRIPT_PATH) \
	$(LD_FLAGS) -o $(BINARY_PATH)/kernel.elf $(BINARY_PATH)/kernel.o \
	$(LD_LINKLIB)
	
	
	$(OBJCOPY) $(OBJCOPY_FLAGS) bin/kernel.elf $(BINARY_PATH)/$(KERNEL_BIN)
	
	#echo "objdump -D -z -b binary -M intel -m i8086 bin/KERNEL.SYS" > w00t      
addit: _internal_kernel_io

image:
	dd if=/dev/zero of=$(BINARY_PATH)/temp_img count=2879 bs=512
	cat $(BINARY_PATH)/$(BOOTLOADER_NAME) | cat - $(BINARY_PATH)/temp_img > temp && mv temp $(BINARY_PATH)/temp_img
	mcopy -i $(BINARY_PATH)/temp_img $(BINARY_PATH)/$(KERNEL_BIN) ::/
	mdir  -i $(BINARY_PATH)/temp_img
	cat $(BINARY_PATH)/temp_img > $(BINARY_PATH)/$(IMAGE_NAME)
	rm $(BINARY_PATH)/temp_img
	
_internal_kernel_io:
	$(CC) -isysroot=$(SYSROOT_INCLUDE) -I $(SYSROOT_INCLUDE) -c $(CLANG_FLAGS) src/lib/kernel/io/io.c -o $(BINARY_PATH)/_int_k_io.o
	
tinyc:
	clang-3.6 -isysroot=$(SYSROOT_INCLUDE) -I $(SYSROOT_INCLUDE) -c $(CLANG_FLAGS) src/lib/tinylibc.c -o bin/tinylibc.o

clean:
	rm $(BINARY_PATH)/*

