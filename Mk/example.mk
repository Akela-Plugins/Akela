BOARD    						= model01
MCU									= atmega32u4

ARDUINO_PATH				= ${HOME}/install/arduino
ARDUINO_TOOLS_PATH	= $(ARDUINO_PATH)/hardware/tools
FQBN								= keyboardio:avr:model01
BUILD_PATH				 := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH					= $(PWD)/firmware/${LIBRARY}
ARDUINO_IDE_VERSION	= 100607

AVR_SIZE						= $(ARDUINO_TOOLS_PATH)/avr/bin/avr-size

GIT_VERSION				 := $(shell git describe --abbrev=4 --dirty --always)
OUTPUT_FILE_PREFIX	= $(SKETCH)-$(GIT_VERSION)

HEX_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
ELF_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

VERBOSE							?= 0

ifneq (${VERBOSE}, 0)
SC									=
SS									= @ true ||
SR                  =
else
SC									= @
SS									= @
SR                  = | grep "\\(Program\\|Data\\):" | sed -e 's,^,  - ,' && echo
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

compile: ${OUTPUT_PATH}
	${SS} echo "Building firmware/${LIBRARY}/${SKETCH} (${GIT_VERSION}) ..."
	${SC} $(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
		-hardware $(PWD)/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
		-libraries $(PWD)/lib \
		-libraries $(PWD)/hardware \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		-warnings all \
		-prefs "compiler.cpp.extra_flags=-std=c++11 -Woverloaded-virtual -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-qualifiers" \
		${VERBOSE_BUILD} \
		$(SKETCH).ino
	@cp $(BUILD_PATH)/$(SKETCH).ino.hex $(HEX_FILE_PATH)
	@cp $(BUILD_PATH)/$(SKETCH).ino.elf $(ELF_FILE_PATH)
	@ln -sf $(OUTPUT_FILE_PREFIX).hex $(OUTPUT_PATH)/${SKETCH}-latest.hex
	@ln -sf $(OUTPUT_FILE_PREFIX).elf $(OUTPUT_PATH)/${SKETCH}-latest.elf
	@rm -rf "${BUILD_PATH}"

size: compile
	${SS} echo "- Size: firmware/${LIBRARY}/${OUTPUT_FILE_PREFIX}.elf"
	${SC} $(AVR_SIZE) -C --mcu=$(MCU) $(ELF_FILE_PATH) ${SR}

clean:
	rm -rf ${OUTPUT_PATH}

.PHONY: all build compile size clean
