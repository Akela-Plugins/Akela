#! /bin/sh
git submodule sync --recursive
git submodule update --init --recursive

(cd hardware/keyboardio/avr/libraries/KeyboardioScanner && git checkout master)
(cd hardware/keyboardio/avr/libraries/KeyboardioHID && git checkout master)
