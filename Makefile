
BOOT_PATH=src/boot
KERNEL_PATH=src/kernel
LIB_PATH=src/lib
BIN_PATH=bin
#============================================
BOOTLOADER_NAME=boot.bin
BOOTLOADER_FILE=boot_stage0.asm
#============================================
IMAGE_NAME=ganjax.flp
#============================================
ASM=nasm
ASM_FLAGS = -f bin
#============================================
KERNEL_NAME=kernel.c
KERNEL_FILE=kernel.o
KERNEL_BIN=KERNEL.SYS
#============================================
WATCOM_CFLAGS=-0 -d0 -ms -s -wx -zl 
WATCOM_BIN=$(WATCOM)/binl
WATCOM_CC=$(WATCOM_BIN)/./wcc
WATCOM_LINK=$(WATCOM_BIN)/./wlink
WATCOM_ASM=$(WATCOM_BIN)/./wasm
WATCOM_LINK_SCRIPT=$(KERNEL_PATH)/watcom_link.lnk
WLOG=-fr=watcom_logs
WOUT=-fo=bin
#============================================

all: startup internals utils kernel_binary image
#startup internals usermode
bootloader: 
	@echo "======================BOOT================================"
	$(ASM) $(ASM_FLAGS) $(BOOT_PATH)/$(BOOTLOADER_FILE) -o $(BIN_PATH)/$(BOOTLOADER_NAME)
	@echo "=========================================================="
kernel: 
	@echo "=====================KERNEL==============================="
	$(WATCOM_CC) $(WATCOM_CFLAGS) $(WLOG)/kernel.errorz $(WOUT)/kernel.o $(KERNEL_PATH)/kernel.c 
	@echo "=========================================================="
startup:
	@echo "=====================STARTUP==============================="
	nasm $(KERNEL_PATH)/wstartup.asm -f obj -o $(BIN_PATH)/wstartup.o
	@echo "=========================================================="
kernel_binary: kernel
	@echo "====================LINK=================================="
	$(WATCOM_LINK) @$(WATCOM_LINK_SCRIPT)
	@echo "=========================================================="
	
internals:	_kernel_internal_io _kernel_internal_termio _kernel_internal_fat12
utils: _utils_stringlib

#============================================
_kernel_internal_io: 
	@echo "=========================IO==============================="
	$(WATCOM_CC) $(WATCOM_CFLAGS) $(WLOG)/io.errorz $(WOUT)/io.o $(LIB_PATH)/kernel/io/io.c
	@echo "=========================================================="
_kernel_internal_termio:
	@echo "=========================TERM==============================="
	$(WATCOM_CC) $(WATCOM_CFLAGS) $(WLOG)/termio.errorz $(WOUT)/termio.o $(LIB_PATH)/kernel/io/terminal.c
	@echo "=========================================================="
_kernel_internal_fat12:
	@echo "=========================FAT12==============================="
	$(WATCOM_CC) $(WATCOM_CFLAGS) $(WLOG)/fat12.errorz $(WOUT)/fat12.o $(LIB_PATH)/kernel/io/fat12.c
	@echo "=========================================================="

#============================================
_utils_stringlib:
	@echo "=========================STRING============================"
	$(WATCOM_CC) $(WATCOM_CFLAGS) $(WLOG)/string.errorz $(WOUT)/string.o $(LIB_PATH)/string.c
	@echo "=========================================================="

#============================================
image:
	@echo "=========================IMAGE==============================="
	dd if=/dev/zero of=$(BIN_PATH)/temp_img count=2879 bs=512
	cat $(BIN_PATH)/$(BOOTLOADER_NAME) | cat - $(BIN_PATH)/temp_img > temp && mv temp $(BIN_PATH)/temp_img
	mcopy -i $(BIN_PATH)/temp_img $(BIN_PATH)/$(KERNEL_BIN) ::/
	mcopy -i $(BIN_PATH)/temp_img $(BIN_PATH)/TEST.TXT ::/
	mdir  -i $(BIN_PATH)/temp_img
	cat $(BIN_PATH)/temp_img > $(BIN_PATH)/$(IMAGE_NAME)
	#mv $(BIN_PATH)/temp_img /home/zb1ggy/Pulpit/polygon/training/c++
	#rm $(BIN_PATH)/temp_img
	@echo "============================================================"
#============================================
clean:
	rm $(BINARY_PATH)/*

