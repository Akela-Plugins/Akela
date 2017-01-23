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

AKELA_PLUGIN_PATH			= hardware/keyboardio/avr/libraries

PLUGIN_EXAMPLES				= $(filter-out plugin/Core,$(sort $(subst ${AKELA_PLUGIN_PATH}/Akela-,plugin/,$(wildcard ${AKELA_PLUGIN_PATH}/Akela-*))))

EXAMPLES							= $(sort $(wildcard examples/*))

all: plugins examples

plugins: ${PLUGIN_EXAMPLES}
plugin/%: .FORCE
	${MAKE} -C ${AKELA_PLUGIN_PATH}/Akela-$*/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=$* SKETCH=$*

examples: ${EXAMPLES}
examples/%: .FORCE
	${MAKE} -C examples/$* -f ${PWD}/Mk/example.mk LIBRARY=Examples SKETCH=$*

clean:
	${SS} echo Cleaning in the firmwares ...
	${SC} rm -rf firmware

.PHONY: .FORCE plugins examples all clean
