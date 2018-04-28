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
SOURCEDIRSDK+=platform/devices/MPC5748G/startup/ghs/
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
CC:=ccppc
AS:=ccppc
LD:=ccppc
STRIP:=gstrip
CCSIZE:=gsize
CCOBJDUMP:=gdump
RM:=rm -f

## Toolchain flags ##
ASFLAGS:=-preprocess_assembly_files
ASFLAGS+=-cpu=ppc5748gz4204
ASFLAGS+=-G
ASFLAGS+=-vle
ASFLAGS+=-C99
ASFLAGS+=--gnu_asm
ASFLAGS+=-Ogeneral
ASFLAGS+=-gdwarf-2
ASFLAGS+=-DCPU_MPC5748G
ASFLAGS+=-DDISABLE_SWT0
ASFLAGS+=-dual_debug
ASFLAGS+=-noSPE
ASFLAGS+=-nostartfile
ASFLAGS+=-sda=0
ASFLAGS+=-DTURN_ON_CPU0

CFLAGS:=-cpu=ppc5748gz4204
CFLAGS+=-G
CFLAGS+=-vle
CFLAGS+=-C99
CFLAGS+=--gnu_asm
CFLAGS+=-O0
CFLAGS+=-gdwarf-2
CFLAGS+=-DCPU_MPC5748G
CFLAGS+=-DDISABLE_SWT0
CFLAGS+=-dual_debug
CFLAGS+=-noSPE
CFLAGS+=-nostartfile
CFLAGS+=-sda=0

STRIP_FLAGS:=-r=.debug_frame

## Linker flags
LDFLAGS_RAM:=-Wl,-Map=$(MAP_RAM)
LDFLAGS_RAM+=-nostartfiles -nostdlib -keepmap -Mn -T "$(BASEDIR)\platform\devices\MPC5748G\linker\ghs\MPC5748G_ram.ld" -entry=Reset_Handler
LDFLAGS_FLASH:=-Wl,-Map=$(MAP_FLASH)
LDFLAGS_FLASH+=-nostartfiles -nostdlib -keepmap -Mn -T "$(BASEDIR)\platform\devices\MPC5748G\linker\ghs\MPC5748G_flash.ld" -entry=Reset_Handler


include Makefile-power
