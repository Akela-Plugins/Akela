---
title: "Dual-use modifier & layer keys"
permalink: /plugins/DualUse/
excerpt: "Dual-use modifier and layer key plugin."
modified: 2016-12-05T11:00:00+01:00
---

{% include toc %}

There are a number of keys on our keyboards that we usually hold in combination
with others. We also have keys that we usually press and release, and don't want
to hold them for repeating. Then we have the others which we sometimes hold to
repeat them, such as `Space` held to insert a number of whitespace characters.
The interesting thing that falls out of this, is that there are keys we hold,
but never tap, and others which we never hold. What if we could combine these?

What if we had a key that acted as `Control` when held, but `Esc` when tapped in
isolation? No useful functionality lost, and we have one key less to place on
the keyboard! We can even put `Esc` to a more convenient position, perhaps.

Dual-use keys do just this: if you hold them, and press any other key, they will
act as a modifier or momentary layer switcher. If you hold them for a longer
period, they - again - will act as modifiers / momentary switchers. But if you
tap and release them in isolation, they will act as another key instead.

## Using the plugin

```c++
#include <Akela-DualUse.h>

static Akela::DualUseMods   dualUseMods;
static Akela::DualUseLayers dualUseLayers;

// in the keymap:
CTL_T(Esc), LT(_LAYER, Esc)
```

## Keymap markup

The plugin provides a number of macros one can use in keymap definitions:

* `CTL_T(key)`: A key that acts as `Control` when held, or used in conjunction
  with other keys, but as `key` when tapped in isolation.
* `ALT_T(key)`: A key that acts as `Alt` when held, or used in conjunction with
  other keys, but as `key` when tapped in isolation.
* `SFT_T(key)`: A key that acts as `Shift` when held, or used in conjunction
  with other keys, but as `key` when tapped in isolation.
* `GUI_T(key)`: A key that acts as `GUI` when held, or used in conjunction with
  other keys, but as `key` when tapped in isolation.
* `MT(mod, key)`: A key that acts as `mod` when held, or used in conjunction
  with other keys, but as `key` when tapped in isolation.
* `LT(layer, key)`: A key that momentarily switches to `layer` when held, or
  used in conjunction with other keys, but as `key` when tapped in isolation.
  
All of these, except `MT` and `LT`, use the left modifier. The `key` argument is
always an unmodified, normal key, it can't have any modifiers or special
behaviour applied.

## Plugin methods

```c++
namespace Akela {
  class DualUseMods {
  public:
    DualUseMods (uint8_t defaultAction);
    DualUseMods (void);

    void on (void);
    void off (void);
  };
  class DualUseLayers {
  public:
    DualUseLayers (uint8_t defaultAction);
    DualUseLayers (void);

    void on (void);
    void off (void);
  };
};
```

By default, the `DualUse` plugins will start enabled, like all other plugins.
However, when turned off, they must be aware which of the two actions they
should perform in off state. By default, unless otherwise specified with the
`defaultAction` constructor argument, they will perform the second action, the
single key action. This can be changed to be the modifier or the layer switch,
by setting `defaultAction` to zero.

It is possible to toggle the feature on and off, with the `on()` and `off()`
methods of the appropriate object.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-DualUse/examples/DualUse/DualUse.ino
