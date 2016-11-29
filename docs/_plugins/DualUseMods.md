---
title: "Dual-use modifier keys"
permalink: /plugins/DualUseMods/
excerpt: "Dual-use modifier key plugin."
modified: 2016-11-29T10:45:00+01:00
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
act as a modifier. If you hold them for a longer period, they - again - will act
as modifiers. But if you tap and release them in isolation, they will act as
another key instead.

## Using the plugin

```c++
#include <Akela-DualUse.h>

static Akela::DualUseMods dualUseMods;

// in the keymap:
CTL_T(ESC)
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
  
All of these, except `MT`, use the left modifier. The `key` argument is always
an unmodified, normal key, it can't have any modifiers or special behaviour
applied.

## Plugin methods

```c++
namespace Akela {
  class DualUseMods {
  public:
    DualUseMods (uint8_t defaultMode, uint8_t defaultAction);
    DualUseMods (uint8_t defaultAction);
    DualUseMods (void);

    void on (void);
    void off (void);
};
```

Like most plugins that add new behaviour, this one can be configured to start
with the new behaviour disabled. However, to do so, we must know what the
disabled behaviour should be: act as a modifier, or as a normal key?

With `defaultMode` set to *off*, the key will act as a normal key, sending
either the first (the modifier) or the second (the alternate key) to the host,
depending on the value of `defaultAction`. If the `defaultMode` is *on* (the
default), the key will behave as explained above.

It is possible to toggle the feature on and off, with the `on()` and `off()`
methods.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-DualUse/examples/DualUse/DualUse.ino
