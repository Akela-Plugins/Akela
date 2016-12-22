---
title: "Per-layer color maps"
permalink: /plugins/Colormap/
excerpt: "Per-layer color map extension."
modified: 2016-12-22T15:50:00+01:00
---

{% include toc %}

The `ColorMap` extension provides an easier way to set up a different - static -
color map per-layer. This means that we can set up a map of colors for each key,
on a per-layer basis, and whenever a layer becomes active, the color map for
that layer is applied on top of everything else. The extension supports
transparent colors, to make things easier.

To save precious space, the extension uses a palette, limited to 255 colors (one
reserved for transparency).

## Using the extension

To set up a colormap, include the header, create a palette, and a colormap
description, and let the extension know that it shall use both of these:

```c++
#include <Akela-Colormap.h>

static const cRGB palette[] PROGMEM = {
  {0, 0, 0},
  {0xff, 0xff, 0xff},
};

static const uint8_t colormap[KEYMAP_SIZE][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED ( ... )
};

void setup (void) {
  Akela::USE (ColorMap);
  
  Colormap.configure (palette, colormap);
  Keyboardio.setup (KEYMAP_SIZE);
}
``` 

The palette is just an array of RGB colors, stored in `PROGMEM`. The index of
each entry will be used in the colormap itself.

The colormap should be specified very similarly to a keymap, except it contains
palette indexes instead of keys. Nevertheless, it should be the same size as the
keymap itself, otherwise the firmware may randomly crash. It is recommended to
use the `KEYMAP_SIZE` macro to make sure the colormap is the right size.

## Extension methods

The extension provides a `Colormap` singleton object, with a single method:

### `.configure(palette, colormap)`

> Tells the extension to use the given palette and colormap.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-Colormap/examples/Colormap/Colormap.ino
