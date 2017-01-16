#! /bin/sh
echo Syncing submodules...
git submodule --quiet sync --recursive

echo Updating submodules...
git submodule --quiet update --init --recursive

for hwlib in KeyboardioScanner KeyboardioHID; do
    echo Updating ${hwlib}...
    (cd hardware/keyboardio/avr/libraries/$hwlib && git checkout -q master; git pull -q --ff)
done

for plugin in lib/Akela-*; do
    echo Updating ${plugin}...
    (cd $plugin; git checkout -q master; git pull -q --ff)
done
