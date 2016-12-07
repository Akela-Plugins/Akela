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

FW_EXAMPLES						= $(filter-out firmware/Examples,$(sort $(subst lib/Akela-,firmware/,$(wildcard lib/Akela-*))))

EXAMPLES							= $(sort $(subst lib/Akela-Examples/examples/,examples/,$(wildcard lib/Akela-Examples/examples/*)))

all: firmware examples

firmware: ${FW_EXAMPLES}
firmware/%: .FORCE
	${MAKE} -C lib/Akela-$*/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=$* SKETCH=$*

examples: ${EXAMPLES}
examples/%: .FORCE
	${MAKE} -C lib/Akela-Examples/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=Examples SKETCH=$*

clean:
	${SS} echo Cleaning in the firmwares ...
	${SC} rm -rf firmware

.PHONY: .FORCE firmware examples all clean
