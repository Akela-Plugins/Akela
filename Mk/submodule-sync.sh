#! /bin/sh
echo Syncing submodules...
git submodule --quiet sync --recursive

echo Updating submodules...
git submodule --quiet update --init --recursive

echo

for lib in hardware/keyboardio/avr/libraries/* lib/*; do
    echo Updating $(basename ${lib})...
    (cd $lib; git checkout -q master; git pull -q --ff)
done
