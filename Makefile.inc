# This file will be included in project's Makefile
#
# The Makefile has to define:
#   DENSITY: md or hd or cl
#   MODE:    flash or ram
#   OBJS:    the list of object files
#   PROGRAM: the final program name

MKDIR_P = mkdir -p

CC = ccache arm-none-eabi-gcc
CFLAGS = -std=c99  -Wall -Werror -fasm -fdata-sections -ffunction-sections \
	-fno-strict-aliasing \
	$(INCCFLAGS) $(ARCHFLAGS) $(OPTFLAGS) $(DEBUGFLAGS)
INCCFLAGS = -DGCC_ARMCM3 -I. -I$(FREERTOS_INCDIR) -I$(FW_INCDIR) \
	-I$(STM32_DIR) -I$(FIRMWARE_DIR) $(PROGRAM_MODE)
ARCHFLAGS = -mcpu=cortex-m3 -mthumb
OPTFLAGS = -g -O2 -fmerge-all-constants -fsee

ifeq ($(MODE),flash)
PROGRAM_MODE=-DPROGRAM_MODE_FLASH
else
PROGRAM_MODE=-DPROGRAM_MODE_RAM
endif

ifeq ($(DENSITY),hd)
INCCFLAGS += -DSTM32F10X_HD
else ifeq ($(DENSITY),md)
INCCFLAGS += -DSTM32F10X_MD
else
INCCFLAGS += -DSTM32F10X_CL
endif

STARTUP_OBJECT = startup_stm32f10x_$(DENSITY).o
LINKER_SCRIPT = stm32f10x_$(MODE)_$(DENSITY).ld

OBJS += $(STARTUP_OBJECT)

AS = arm-none-eabi-as
ASFLAGS = $(ARCHFLAGS)

LD = arm-none-eabi-gcc
LDFLAGS = -L$(STM32_DIR) -T $(LINKER_SCRIPT) \
	-Wl,-Map=$(PROGRAM).map \
	-Wl,--gc-sections \
	$(ARCHFLAGS)

OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

FREERTOS_DIR = ../Source
FREERTOS_INCDIR = $(FREERTOS_DIR)/include
FREERTOS_PLATDIR = $(FREERTOS_DIR)/portable/GCC/ARM_CM3
FREERTOS_MEMDIR = $(FREERTOS_DIR)/portable/MemMang

FIRMWARE_DIR = ../firmware

FW_DIR = $(STM32_DIR)/STM32_USB-FS-Device_Lib_V3.1.0/Libraries
FW_CORE_DIR = $(FW_DIR)/CMSIS/Core/CM3
FW_STARTUP_DIR = $(FW_CORE_DIR)/startup/gcc
FW_STDDRIVER_DIR = $(FW_DIR)/STM32F10x_StdPeriph_Driver
FW_STDDRIVER_SRCDIR = $(FW_STDDRIVER_DIR)/src
FW_STDDRIVER_INCDIR = $(FW_STDDRIVER_DIR)/inc
FW_USB_DIR = $(FW_DIR)/Libraries/STM32_USB-FS-Device_Driver
FW_USB_SRCDIR = $(FW_USB_DIR)/src
FW_USB_INCDIR = $(FW_USB_DIR)/inc
INCCFLAGS += -I$(FW_CORE_DIR) -I$(FW_STARTUP_DIR) -I$(FW_STDDRIVER_INCDIR) \
	-I$(FW_USB_INCDIR)
VPATH = $(FW_CORE_DIR) $(FW_STARTUP_DIR) $(FW_STDDRIVER_SRCDIR) \
	$(FW_USB_SRCDIR) $(FREERTOS_DIR) $(FREERTOS_PLATDIR) $(FREERTOS_MEMDIR) \
	$(STM32_DIR) $(FIRMWARE_DIR)

FW_SRCDIR = $(FW_DIR)/src
FW_INCDIR = $(FW_DIR)/inc

STM32_DIR = ../STM32

all:: $(PROGRAM)

clean::
	$(RM) $(PROGRAM) *.o $(PROGRAM).map $(PROGRAM).bin *~ \#*\#
	@$(RM) -r .deps

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) -o $(PROGRAM) $(OBJS)

info::
	$(OBJDUMP) --section-headers $(PROGRAM)

%.bin: %
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $<
	@$(MKDIR_P) .deps
	@mv $(patsubst %.o,%.d,$@) .deps/

%.o: %.raw
	$(OBJCOPY) -B arm -I binary -O elf32-littlearm \
	--rename-section .data=.rodata,alloc,load,readonly,data,contents $< $@

*.o: Makefile ../Makefile.inc

DEPS := $(patsubst %.o,.deps/%.d,$(OBJS))

-include .deps/*.d
