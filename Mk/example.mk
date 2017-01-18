BOARD    						= model01
MCU									= atmega32u4

ARDUINO_PATH				= ${HOME}/install/arduino
ARDUINO_TOOLS_PATH	= $(ARDUINO_PATH)/hardware/tools
ARDUINO_BUILDER			= $(ARDUINO_PATH)/arduino-builder
FQBN								= keyboardio:avr:model01
BUILD_PATH				 := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH					= $(PWD)/firmware/${LIBRARY}
ARDUINO_IDE_VERSION	= 100607

AVR_SIZE						= $(ARDUINO_TOOLS_PATH)/avr/bin/avr-size
AVR_NM							= $(ARDUINO_TOOLS_PATH)/avr/bin/avr-nm

GIT_VERSION				 := $(shell git describe --abbrev=4 --dirty --always)
LIB_VERSION				 := $(shell (grep version= ../../library.properties 2>/dev/null || echo version=0.0.0) | cut -d= -f2)-g${GIT_VERSION}
OUTPUT_FILE_PREFIX	= $(SKETCH)-$(LIB_VERSION)

HEX_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
ELF_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

VERBOSE							?= 0

SIZE_TOOL						= $(dir $(lastword ${MAKEFILE_LIST}))/firmware-size

ifneq (${VERBOSE}, 0)
SC									=
SS									= @ true ||
else
SC									= @
SS									= @
MAKEFLAGS += -s
endif

ifeq (${VERBOSE},2)
VERBOSE_BUILD				= -verbose
endif

# SKETCH
# LIBRARY

all: build
build: compile size

${OUTPUT_PATH}:
	install -d $@

ARDUINO_TOOLS_PARAM = -tools $(ARDUINO_TOOLS_PATH)
ifeq ($(ARDUINO_TOOLS_PATH),)
	ARDUINO_TOOLS_PARAM =
endif

ifdef AVR_GCC_PREFIX
	ARDUINO_AVR_GCC_PREFIX_PREF = -prefs "runtime.tools.avr-gcc.path=$(AVR_GCC_PREFIX)"
endif

compile: ${OUTPUT_PATH}
	${SS} echo "Building firmware/${LIBRARY}/${SKETCH} (${LIB_VERSION}) ..."
	${SC} $(ARDUINO_BUILDER) \
		-hardware $(ARDUINO_PATH)/hardware \
		-hardware $(PWD)/hardware \
		$(ARDUINO_TOOLS_PARAM) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(PWD)/lib \
		-libraries $(PWD)/lib/KeyboardioFirmware/libraries \
		-libraries $(PWD)/hardware \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		-warnings all \
		-quiet \
		-prefs "compiler.cpp.extra_flags=-std=c++11 -Woverloaded-virtual -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-qualifiers" \
		$(ARDUINO_AVR_GCC_PREFIX_PREF) \
		${VERBOSE_BUILD} \
		$(SKETCH).ino
	@cp $(BUILD_PATH)/$(SKETCH).ino.hex $(HEX_FILE_PATH)
	@cp $(BUILD_PATH)/$(SKETCH).ino.elf $(ELF_FILE_PATH)
	@ln -sf $(OUTPUT_FILE_PREFIX).hex $(OUTPUT_PATH)/${SKETCH}-latest.hex
	@ln -sf $(OUTPUT_FILE_PREFIX).elf $(OUTPUT_PATH)/${SKETCH}-latest.elf
	@rm -rf "${BUILD_PATH}"

size: compile
	${SS} echo "- Size: firmware/${LIBRARY}/${OUTPUT_FILE_PREFIX}.elf"
	${SC} $(SIZE_TOOL) $(AVR_SIZE) -C --mcu=$(MCU) $(ELF_FILE_PATH)
	@echo

size-map: compile
	$(AVR_NM) --size-sort -C -r -l $(ELF_FILE_PATH)

clean:
	rm -rf ${OUTPUT_PATH}

.PHONY: all build compile size clean
