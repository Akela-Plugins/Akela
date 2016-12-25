#!/bin/sh

echo "Press ENTER when ready..."
read a

stty -F /dev/ttyACM* 1200 hupcl
sleep 3s
avrdude -v -patmega32u4 -cavr109 -D -P /dev/ttyACM* -b57600 -Uflash:w:firmware/$1/$1-latest.hex:i
