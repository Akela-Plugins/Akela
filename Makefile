all: examples

examples: examples/KeyLogger

examples/%:
	${MAKE} -C lib/Akela-$*/examples/$* -f ${PWD}/Mk/example.mk LIBRARY=$* SKETCH=$*

clean:
	${SS} echo Cleaning in the firmwares ...
	${SC} rm -rf firmware

.PHONY: all examples clean
