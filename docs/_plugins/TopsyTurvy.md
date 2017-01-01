---
title: "Selective shift inverter"
permalink: /plugins/TopsyTurvy/
excerpt: "Invert the Shift behaviour for selected keys."
modified: 2017-01-01T13:35:00+01:00
---

{% include toc %}

`TopsyTurvy` is a plugin that inverts the behaviour of the `Shift` key for some
selected keys. That is, if configured so, it will input `!` when pressing the
`1` key without `Shift`, but with the modifier pressed, it will input the
original `1` symbol.

## Using the plugin

To use the plugin, one needs to include the header, create a list, and configure
the provided `TopsyTurvy` object to use the dictionary:

```c++
#include <Akela-TopsyTurvy.h>

static const Key topsyTurvyList[] PROGMEM = {
  Key_1, Key_2, Key_3, Key_4, Key_5,
  Key_6, Key_7, Key_8, Key_9, Key_0,
  Key_NoKey
};

void setup () {
  TopsyTurvy.configure (topsyTurvyList);
  
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&TopsyTurvy, NULL);
}
```

The list of keys **must** be terminated with a `Key_NoKey`, and **must** reside
in `PROGMEM`.

## Plugin methods

The plugin provides the `TopsyTurvy` object, with the following methods:

### `.configure(list)`

> Tells `TopsyTurvy` to use the specified list of keys.

### `.on()`

> Turns the shift-inversion functionality on.

### `.off()`

> Turns the shift-inversion functionality off. In this case, no transformations
> will be applied, and even if keys pressed appear in the list, they will be
> ignored until the plugin is turned back on.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-TopsyTurvy/examples/TopsyTurvy/TopsyTurvy.ino
