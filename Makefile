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

export SC
export SS
export SR

all: model01 tapdance unicode t

model01: model01/Model01 model01/stripped model01/minimal model01/example
tapdance: tapdance/TapDanceKeyboard
unicode: unicode/unicode

tapdance/%:
	${MAKE} -C lib/Akela-TapDance/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=tapdance SKETCH=$*

model01/%:
	${MAKE} -C lib/Akela-Model01/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=model01 SKETCH=$*

unicode/%:
	${MAKE} -C lib/Akela-Unicode/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=unicode SKETCH=$*

user/%:
	${MAKE} -C user/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=user SKETCH=$*

t:
	${SS} echo Building the test suite...
	${MAKE} -C t

check: t
	${SS} echo Running the test suite...
	${MAKE} -C t $@

clean:
	${SS} echo Cleaning in the firmwares ...
	${SC} rm -rf firmware
	${SS} echo Cleaning in t ...
	${SC} ${MAKE} -C t clean

.PHONY: all keyboardio t check clean
