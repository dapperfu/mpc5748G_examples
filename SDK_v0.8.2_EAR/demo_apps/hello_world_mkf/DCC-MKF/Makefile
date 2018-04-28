### S32 SDK Build Makefile ###
# This makefile will build a bare metal S32 SDK application.
# Some make variables need to be defined here so make targets can be built correctly.
#
# Prerequisites:
#   - an arm toolchain
#   - GNU Make or an Unix-like environment
#
# Application specific variables that must be defined:
#   - CSOURCES, ASMSOURCES: C and assembly sources, respectively. Note: only files, without paths
#   - SOURCEDIR: Locations for all source and header files. The paths defined here will be used:
#       1)to match source files defined in CSOURCES and ASMSOURCES
#       2)as include path for the compiler
#   - OBJECTDIR: object directory to save object and intermediate files. Objects will be generated
#       as a flat list (no subdirectories).
# Common make variables. These are recommended to be defined, but can be omitted:
#   - CFLAGS: flags to pass to the C compiler
#   - ASFLAGS: flags to pass to the assembler
#   - LDFLAGS: flags to pass to the linker
#   - EXECUTABLE: name and location of the executable (.elf file)
# Variables that define the toolchain:
#   - CC: C compiler executable
#   - AS: Assembler executable
#   - LD: Linker executable
#   - RM: removed command (default 'rm -f')
#   - CCSIZE: Size executable (prints sections sizes: text, data, bss)
#   - CCOBJDUMP: Objdump executable
#
# Most variables have default values defined, but it is recommended to define

## Define CSOURCES and ASMSOURCES variables ##
CSOURCES:=main.c
CSOURCES+=system_MPC5748G.c
CSOURCES+=startup.c

ASMSOURCES:=startup_MPC5748G.s
ASMSOURCES+=interrupt_vectors.s
ASMSOURCES+=core0_intc_sw_handlers.s

## Define SOURCEDIR variable ##
# Define two helper vars, BASEDIR (SDK base) and SOURCEDIRSDK (paths inside SDK to be included/searched)
BASEDIR:=../../../../../
SOURCEDIRSDK:=platform/
SOURCEDIRSDK+=platform/drivers/src/
SOURCEDIRSDK+=platform/drivers/src/clock/
SOURCEDIRSDK+=platform/drivers/src/interrupt/
SOURCEDIRSDK+=platform/drivers/src/clock/MPC57xx/
SOURCEDIRSDK+=platform/devices/
SOURCEDIRSDK+=platform/devices/MPC5748G/
SOURCEDIRSDK+=platform/devices/MPC5748G/startup/
SOURCEDIRSDK+=platform/devices/MPC5748G/startup/dcc/
SOURCEDIRSDK+=platform/devices/MPC5748G/include/
SOURCEDIRSDK+=platform/devices/common
SOURCEDIRSDK+=platform/drivers/inc/
SOURCEDIRSDK+=platform/drivers/src/adc/
SOURCEDIRSDK+=platform/drivers/src/flash/
SOURCEDIRSDK+=platform/drivers/src/lpuart/
SOURCEDIRSDK+=platform/drivers/src/lpi2c/
SOURCEDIRSDK+=platform/drivers/src/pins/

# Location of objects and final source directories
OBJECTDIR_NAME:=obj
SOURCEDIR_NAME:=../sources

# Assembler file extension
ASM_EXT:=s

# Map file names
MAP_RAM:=app_ram.map
MAP_FLASH:=app_flash.map

## Elf file names ##
EXECUTABLE_RAM:=app_ram.elf
EXECUTABLE_FLASH:=app_flash.elf

## Toolchain executables ##
CC:=dcc
AS:=dcc
LD:=dld
CCSIZE:=ddump -Z



## Toolchain flags ##
TARGET := -tPPCE200Z4204N3VEN:simple

ASFLAGS:= $(TARGET)
ASFLAGS+=-DTURN_ON_CPU0
ASFLAGS+=-Xpreprocess-assembly


CFLAGS:= $(TARGET)
CFLAGS+= -g3 -Wa,-Xisa-vle
CFLAGS+= -Xdialect-c99
CFLAGS+= -Xsmall-data=0
CFLAGS+= -Xsmall-const=0
CFLAGS+= -DCPU_MPC5748G
CFLAGS+= -DDISABLE_SWT0
CFLAGS+= -Xdebug-dwarf2
CFLAGS+= -Xdebug-local-all
CFLAGS+= -Xdebug-local-cie
CFLAGS+= -Xdebug-struct-all
CFLAGS+= -DTURN_ON_CPU0


## Linker flags
LDFLAGS_RAM:=$(TARGET) -Xremove-unused-sections -lc -Xpreprocess-lecl
LDFLAGS_RAM+= "$(BASEDIR)\platform\devices\MPC5748G\linker\dcc\MPC5748G_ram.dld" -e Reset_Handler
LDFLAGS_RAM+= -m6 > $(MAP_RAM)

LDFLAGS_FLASH:=$(TARGET) -Xremove-unused-sections -lc -Xpreprocess-lecl
LDFLAGS_FLASH+= "$(BASEDIR)\platform\devices\MPC5748G\linker\dcc\MPC5748G_flash.dld" -e Reset_Handler
LDFLAGS_FLASH+= -m6 > $(MAP_FLASH)

include Makefile-power
