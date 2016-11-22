---
title: "Heatmap"
permalink: /plugins/Heatmap/
excerpt: "LED-based Heatmap plugin for the Keyboardio Model 01"
modified: 2016-11-22T13:15:00+01:00
---

{% include toc %}

The `Heatmap` plugin provides a LED effect, that displays a heatmap on the
keyboard. The LEDs under each key will have a color according to how much use
they see. Fewer used keys will have deep blue colors, that gradually turns
lighter, then green, to yellow, and finally red for the most used keys. The
heatmap is not updated on every key press, but periodically. It's precision is
also an approximation, and not a hundred percent exact. Nevertheless, it is a
reasonable estimate.

## Using the plugin

The plugin is not immediately enabled after including the header, it must be
explicitly configured:

```c++
#include <Akela-Heatmap.h>

static Akela::Heatmap heatmap(500);
```

This sets up the heatmap to update every 500 cycles, which is about 2.5 seconds,
and is the default. To make it possible to turn the heatmap display off
(collecting data will continue regardless), it is recommended to use another
plugin, or built-in LED effect, such as `LEDOff`, and either have a key bound to
`Key_LedEffectNext`, or have any other way to switch LED effects.

As a general rule of thumb, the following snippet is a good starting point:

```c++
#include <KeyboardioFirmware.h>
#include <LED-Off.h>
#include <Akela-Heatmap.h>

static LEDOff ledOffEffect;
static Akela::Heatmap heatmap;

void setup () {
  Keyboardio.setup();
  heatmap.activate();
}

void loop () {
  Keyboardio.loop();
}
```

This will activate the heatmap when the keyboard starts, but if you have a key
to switch to another LED effect, `LEDOff` will be available too, and can be used
to temporarily stop the heatmap display.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-Heatmap/examples/Heatmap/Heatmap.ino
