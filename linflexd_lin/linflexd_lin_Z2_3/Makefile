CPU=e200z2

SYMBOLS += MPC574xG

SRC_S += ${realpath ${wildcard Project_Settings/Startup_Code/*.S}}
SRC_LD += ${realpath ${wildcard Project_Settings/Linker_Files/libs.ld}}

OPT_FLAGS+=message-length=0
OPT_FLAGS+=signed-char
OPT_FLAGS+=function-sections
OPT_FLAGS+=data-sections

MACH_OPTS+=cpu=${CPU}
MACH_OPTS+=big
MACH_OPTS+=vle
MACH_OPTS+=regnames
MACH_OPTS+=hard-float

C_STD=c99
OPT_LVL=0
DEBUG_LVL=3
WARNINGS=all

PROJ_DIR:=$(realpath $(dir $(realpath ${firstword ${MAKEFILE_LIST}})))
E200MK_DIR:=$(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST))))/../..)

VLE_TOOLCHAIN_DIR := ${E200MK_DIR}/powerpc-eabivle-4_9
VLE_EWL_DIR:=${E200MK_DIR}/e200_ewl2

# Prefix for cross compiler
CROSS_PREFIX:=powerpc-eabivle-
# Standard compiler utils
CC      := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}gcc
CXX     := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}g++
LD     := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}ld
OBJCOPY := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}objcopy
OBJDUMP := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}objdump
SIZE    := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}size
NM      := ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}nm

SYSROOT=${VLE_EWL_DIR}
SPECS=ewl_c9x_noio.specs

# Gather source files.
SRC_C += ${realpath ${wildcard ${PROJ_DIR}/src/*.c}}
SRC_S += ${realpath ${wildcard ${PROJ_DIR}/src/*.S}}

# Calculate
OBJ_C = $(patsubst %.c,%.o,${SRC_C})
OBJ_S = $(patsubst %.S,%.o,${SRC_S})
OBJS = ${OBJ_C} ${OBJ_S}

# Include Directories
INCLUDE += ${PROJ_DIR}/include
INCLUDE += ${VLE_EWL_DIR}/EWL_C/include
INCLUDE += ${VLE_EWL_DIR}/EWL_C/include/pa

# C Flags
C_FLAGS += -std=${C_STD}
C_FLAGS += $(foreach d, $(SYMBOLS),-D$d)
C_FLAGS += $(foreach d, $(OPT_FLAGS),-f$d)
C_FLAGS += $(foreach d, $(MACH_OPTS),-m$d)
C_FLAGS += -W${WARNINGS}
C_FLAGS += -g${DEBUG_LVL}
C_FLAGS += -specs=${SPECS}
C_FLAGS += --sysroot="${SYSROOT}"
C_FLAGS += $(foreach d, $(INCLUDE),-I"$d")

# Linker Flags
LD_FLAGS += $(foreach d, $(SRC_LD),-T"$d")
LD_FLAGS += -Wl,-Map,"${BIN_ROOT}.map"
LD_FLAGS += -Xlinker
LD_FLAGS += --gc-sections
LD_FLAGS += $(foreach d, $(MACH_OPTS),-m$d)
LD_FLAGS += -specs=${SPECS}
LD_FLAGS += --sysroot="${SYSROOT}"

%.o: %.c
	$(file >$@.in) $(foreach O,${C_FLAGS},$(file >>$@.in,$O))
	$(CC) -o "$@" -c "$<" @$@.in 
	
%.o: %.S
	$(file >$@.in) $(foreach O,${C_FLAGS},$(file >>$@.in,$O))
	$(CC) $(C_FLAGS) -o "$@" -c "$<"
	
# Determine the root binary project name.
BIN_ROOT:=${notdir ${PROJ_DIR}}
ELF:=${BIN_ROOT}.elf
HEX:=${BIN_ROOT}.hex
BIN:=${BIN_ROOT}.bin
S19:=${BIN_ROOT}.S19

.PHONY: elf hex bin s19
elf: ${ELF}
hex: ${HEX}
bin: ${BIN}
s19: ${S19}

.DEFAULT: all
.PHONY: all
all: ${ELF} ${HEX} ${BIN} ${S19}
	
${ELF}: ${OBJS}
	${CC} ${LD_FLAGS} ${OBJS} -o "$@"

${HEX}: ${ELF}
	${OBJCOPY} --strip-all --output-target ihex $< "$@"
	
${BIN}: ${ELF}
	${OBJCOPY} --strip-all --output-target binary $< "$@"
	
${S19}: ${ELF}
	${OBJCOPY} --strip-all --output-target srec $< $@

.PHONY: clean
clean:
	-${RM} ${ELF}
	-${RM} ${HEX}
	-${RM} ${BIN}
	-${RM} ${S19}
	
.PHONY: debug
debug:
	$(info $${INCLUDE}=${INCLUDE})
	$(info $${ELF}=${ELF})
	$(info $${E200MK_DIR}=${E200MK_DIR})
	$(info $${SRC_S}=${SRC_S})
	$(info $${SRC_LD}=${SRC_LD})