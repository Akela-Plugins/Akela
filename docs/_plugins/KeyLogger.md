---
title: "Keylogger"
permalink: /plugins/KeyLogger/
excerpt: "KeyLogger plugin."
modified: 2017-01-03T15:45:00+01:00
---

{% include toc %}

The `KeyLogger` plugin, as the name suggests, implements a key logger for the
Keyboardio firmware. It logs the row and column of every key press and release,
along with the event, and the layer number, in a format that is reasonably easy
to parse, to the Serial interface.

## A word of warning

Having a key logger is as dangerous as it sounds. Anyone who can read the serial
events from the keyboard, will know exactly what keys you press, and when.
Unless you know what you are doing, and can secure your keyboard, do **not**
enable this plugin.

## Using the plugin

There is nothing configurable about the plugin, using it is as simple as
including the header file, and declaring it used:

```c++
#include <Akela-KeyLogger.h>

void setup (void) {
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&KeyLogger, NULL);
}
```

That, in itself, will do all that is necessary to have the key logger active.

## The output

On the host, reading from the keyboard's serial port (or using the *Serial
Monitor* built into the Arduino IDE), one will be able to see the following
output:

```
KL: row=1, col=2, pressed=1, defaultLayer=0, layerState=1, mappedKey.flags=0, mappedKey.keyCode=ff
KL: row=1, col=2, pressed=0, defaultLayer=0, layerState=1, mappedKey.flags=0, mappedKey.keyCode=ff
```
