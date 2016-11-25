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

EXAMPLES						= $(sort $(subst lib/Akela-,firmware/,$(wildcard lib/Akela-*)))

all: firmware

firmware: ${EXAMPLES}
firmware/%: .FORCE
	${MAKE} -C lib/Akela-$*/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=$* SKETCH=$*

clean:
	${SS} echo Cleaning in the firmwares ...
	${SC} rm -rf firmware

.PHONY: .FORCE firmware all clean
