PROJECT = meas
DEV_CPU = 0
PLACE=3

ifeq ($(PLACE),1)
	DIR_MBED = d:/blinky/mbed-os
	TOOLPATH = d:/usr/gcc-arm/bin/arm-none-eabi
	OPENOCD  = d:/usr/openocd/bin/openocd.exe
	OPENOCD_SCRIPTS = d:/usr/openocd/share/openocd/scripts/
endif
ifeq ($(PLACE),2)
	DIR_MBED = c:/home/mbed/blinky/mbed-os
	TOOLPATH = c:/usr/gcc-arm/bin/arm-none-eabi
	OPENOCD  = c:/usr/openocd/bin/openocd.exe
	OPENOCD_SCRIPTS = c:/usr/openocd/share/openocd/scripts/
endif
ifeq ($(PLACE),3)
	DIR_MBED = ../../blinky/mbed-os
	TOOLPATH = /mnt/bin/gcc-arm/bin/arm-none-eabi
	OPENOCD  = openocd
	OPENOCD_SCRIPTS = /usr/openocd/share/openocd/scripts/
endif

ifeq ($(DEV_CPU),0)
#	DEV_DEVICE = 072
#	DEV_FLASH  = 072RBTx
	DEV_DEVICE = 051
	DEV_FLASH  = 051R8Tx
	DEV_FPU = 
endif
ifeq ($(DEV_CPU),4)
	DEV_DEVICE = 407
	DEV_FLASH  = 407VGTx
	DEV_FPU = -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
endif
ifeq ($(DEV_CPU),7)
	DEV_DEVICE = 767
	DEV_FLASH  = 767ZITx
	DEV_FPU = -mfpu=fpv5-d16 -mfloat-abi=softfp
endif


OBJECTS += main.o
OBJECTS += hal.o
OBJECTS += test.o
OBJECTS += stm32f$(DEV_CPU)xx_it.o
OBJECTS += system_stm32f$(DEV_CPU)xx.o
OBJECTS += startup_stm32f$(DEV_DEVICE)x8.o
OBJECTS += ../drivers/buffer.o
OBJECTS += ../drivers/mysensor.o
OBJECTS += ../drivers/outsens.o
OBJECTS += ../drivers/ds1820.o
OBJECTS += ../drivers/sump.o
OBJECTS += ../drivers/rfm12b.o
OBJECTS += ../drivers/logging.o
OBJECTS += ../lib/stm32f$(DEV_CPU)/stm32f$(DEV_CPU)xx_ll_utils.c

INC_PATH += -I.

INC_PATH += -I../drivers
INC_PATH += -I../lib/stm32f$(DEV_CPU)
INC_PATH += -I../lib/cmsis/stm32f$(DEV_CPU)
INC_PATH += -I$(DIR_MBED)/cmsis
INC_PATH += -I$(DIR_MBED)/cmsis/TARGET_CORTEX_M
INC_PATH += -I$(DIR_MBED)/cmsis/TARGET_CORTEX_M/TOOLCHAIN_GCC

LINKER_SCRIPT =  STM32F$(DEV_FLASH)_FLASH.ld


FLAGS += -mcpu=cortex-m$(DEV_CPU)
FLAGS += -mthumb
FLAGS += $(DEV_FPU)

COMMON_FLAGS += -Wall
COMMON_FLAGS += -Wextra
COMMON_FLAGS += -Wno-unused-parameter
COMMON_FLAGS += -Wno-missing-field-initializers
COMMON_FLAGS += -fmessage-length=0
COMMON_FLAGS += -fno-exceptions
COMMON_FLAGS += -fno-builtin
COMMON_FLAGS += -ffunction-sections
COMMON_FLAGS += -fdata-sections
COMMON_FLAGS += -funsigned-char
COMMON_FLAGS += -MMD
COMMON_FLAGS += -fno-delete-null-pointer-checks
COMMON_FLAGS += -fomit-frame-pointer

C_FLAGS += -Dstm32f$(DEV_CPU)

ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += -DTRANSACTION_QUEUE_SIZE_SPI=2
ASM_FLAGS += -D__CORTEX_M$(DEV_CPU)
ASM_FLAGS += -DUSB_STM_HAL
ASM_FLAGS += -DARM_MATH_CM4
ASM_FLAGS += -D__FPU_PRESENT=1

LD_FLAGS += -Wl,--gc-sections
#LD_FLAGS += -Wl,--wrap,main
LD_FLAGS += -Wl,--wrap,_malloc_r
LD_FLAGS += -Wl,--wrap,_free_r
LD_FLAGS += -Wl,--wrap,_realloc_r
LD_FLAGS += -Wl,--wrap,_memalign_r
LD_FLAGS += -Wl,--wrap,_calloc_r
LD_FLAGS += -Wl,--wrap,exit
LD_FLAGS += -Wl,--wrap,atexit
LD_FLAGS += -Wl,-n  

AS      = $(TOOLPATH)-gcc -x assembler-with-cppd -c $(COMMON_FLAGS) -O0 -g3 $(FLAGS)
CC      = $(TOOLPATH)-gcc -std=gnu99 -c $(COMMON_FLAGS) -O0 -g3 $(FLAGS)
CPP     = $(TOOLPATH)-g++ -std=gnu++98 -fno-rtti -Wvla -c $(COMMON_FLAGS) -O0 -g3 $(FLAGS)
LD      = $(TOOLPATH)-gcc $(LD_FLAGS)  $(FLAGS)
ELF2BIN = $(TOOLPATH)-objcopy
PREPROC = $(TOOLPATH)-cpp -E -P $(LD_FLAGS) $(FLAGS)
SIZE    = $(TOOLPATH)-size

###############################################################################
.PHONY: all lst size debug flash tags clean info ocd

all: info $(PROJECT).bin 

info:
	@echo STM32F$(DEV_CPU)
	@echo "DEV_FLASH  : $(DEV_FLASH)"
	@echo ""

size:
	$(SIZE) $(PROJECT).elf

clean:
	rm $(PROJECT).bin $(PROJECT).elf *.o *.d ../drivers/*.o
tags:
	etags *.[ch] ../drivers/*.[ch] ../lib/stm32f0/*.h ../lib/cmsis/stm32f0/*.h

#flash:
#	xcopy /Y $(PROJECT).bin e:

ocd:
	$(OPENOCD) -s $(OPENOCD_SCRIPTS) -f board/stm32f0discovery.cfg

flash:
	$(OPENOCD) -s $(OPENOCD_SCRIPTS) -f board/stm32f0discovery.cfg \
		-c init \
		-c "reset init" \
		-c "sleep 200" \
		-c "wait_halt 2" \
		-c "flash write_image erase $(PROJECT).bin 0x08000000" \
		-c "sleep 200" \
		-c "verify_image $(PROJECT).bin 0x08000000" \
		-c "sleep 200" \
		-c "reset halt" \
		-c "shutdown" \
		-c "reset init"


debug:
	$(OPENOCD) -s $(OPENOCD_SCRIPTS) -f board/stm32f0discovery.cfg -c init -c "reset init"
#	$(OPENOCD) -s $(OPENOCD_SCRIPTS) -f board/stm32f4discovery.cfg -c init -c "reset init"
#	$(OPENOCD) -s $(OPENOCD_SCRIPTS) -f board/st_nucleo_f7.cfg -c init -c "reset init"

.s.o:
	+@echo "AS $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $(INC_PATH) -o $@ $<

.c.o:
	+@echo "CC $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INC_PATH) -o $@ $<

$(PROJECT).ld: $(LINKER_SCRIPT)
	@$(PREPROC) $< -o $@

$(PROJECT).elf: $(OBJECTS) $(PROJECT).ld
	+@echo "LN $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(PROJECT).ld $(LIB_PATH) $(INC_PATH)  --output $@ $(OBJECTS) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	+@echo "BN $<"
	@$(ELF2BIN) -O binary $< $@

