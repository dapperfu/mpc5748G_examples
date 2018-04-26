# Get all subdirectories that contain a makefile.
SUBMAKE=$(wildcard */Makefile)
# Artificially append a .clean to the end of it to clean.
SUBCLEAN = $(addsuffix .clean,$(SUBMAKE))
# Register the phony files.
.PHONY: force clean $(SUBCLEAN)

# make all & make clean.
all: $(SUBMAKE)
clean: $(SUBCLEAN)


$(SUBMAKE): force
	-@$(MAKE) -C $(@D)

clean:
	git clean -xfd

## Config

# Base URL. Set to local host for faster CI builds
BASE_URL?=http://172.16.0.35/e200

TOOLS:=AMMCLIB powerpc-eabivle-4_9 S32_SDK_MPC574xx_BETA_0.9.0

TOOL_ARCHIVES:=$(foreach TOOL,${TOOLS},${TOOL}.tar.xz)
LOCAL_TOOL_ARCHIVES:=$(foreach TOOL,${TOOL_ARCHIVES},.env/${TOOL})
TOOL_DIRS:=$(foreach TOOL,${TOOLS},.env/${TOOL})

# Targets
.PHONY: env
env: ${TOOL_DIRS}

.env:
	mkdir -p ${@}

${LOCAL_TOOL_ARCHIVES}: .env
	@echo --- Downloading ${@} ---
	curl --silent --location --output ${@} ${BASE_URL}/$(shell basename ${@})

${TOOL_DIRS}: ${LOCAL_TOOL_ARCHIVES}
	@echo --- Extracting ${@} ---
	tar -xJf ${@}.tar.xz -C .env/
