---
title: "One-shot layers"
permalink: /plugins/OneShotLayers/
excerpt: "One-shot layer plugin for the Keyboardio Model 01"
modified: 2016-11-25T18:15:00+01:00
---

{% include toc %}

One-shot layers are another kind of layer switching mechanism: instead of having
to hold them while pressing other keys, or toggling to another layer, they can
be tapped and released, and will remain active until any other key is pressed.
They can still be used as normal, momentary switchers while held.

Furthermore, if a one-shot layer is tapped two times in quick succession, it
becomes sticky, like a layer toggle, and remains active until disabled with a
third tap. This can be useful when one needs to input a number of keys with the
layer active, and still does not wish to hold it down.

## Using the plugin

```c++
#include <Akela-OneShot.h>

static Akela::OneShotLayers oneShotLayers;

void setup () {
  oneShotLayers.enableAuto();
  Keyboardio.setup(1);
}
```

To use the plugin, the header needs to be included, and some behaviour
configured by instantiating an `Akela::OneShotLayers` object with the desired
parameters. By default, one-shot layers start enabled, meaning any key marked up
with the `OSL()` macro will have the one-shot behaviour, targeting the layer
given in the argument. If told to start disabled, then these keys will behave as
usual momentary toggles, without the added one-shot behaviour.

## Keymap markup

There is only one macro the plugin provides, `OSL(layer)`, that takes a single
argument, the number of the layer to switch to. When marked up with this macro,
the key will act as a one-shot layer switcher, targeting the specified layer.

To avoid having to change the keymap to add `OSL()` markup, the `enableAuto()`
method can be used to turn all momentary switchers into one-shots.

## Plugin methods

```c++
namespace Akela {
  class OneShotLayers {
  public:
    OneShotLayers (uint8_t defaultState);
    OneShotLayers (void);

    static void on (void);
    static void off (void);
    static void enableAuto (void);
    static void cancel (void);
  };
};
```

The `on()` and `off()` methods can be used in macros, or elsewhere, to turn the
*one-shot* behaviour on or off. When turned off, then keys that were marked as
one-shot in the keymap, will behave as if they were normal momentary switchers.
These can be called at any time, and have the effect one would expect.

Similar to these, `enableAuto()` turns all momentary switchers on the keymap
into one-shot layer keys - but only those on the keymap. Any injected switchers
are ignored. This operation cannot later be reversed, but one can still turn the
one-shot behaviour off with the `off()` method. Furthermore, for the
transformation to work, this method needs to be called before
`Keyboardio.setup()`. Its purpose is to make it possible to re-use a key layout
without one-shot layer keys, as-is, but still have one-shot behaviour.

The `cancel()` method can be used to immediately cancel any one-shot layer keys.
Useful in case one wants to be able to change their minds, and cancel a one-shot
layer key pressed by mistake.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-OneShot/examples/OneShot/OneShot.ino
