---
title: "One-shot modifiers"
permalink: /plugins/OneShotMods/
excerpt: "One-shot modifier plugin for the Keyboardio Model 01"
modified: 2016-11-23T20:50:00+01:00
---

{% include toc %}

One-shot modifiers are a new kind of behaviour for your standard modifiers:
instead of having to hold them while pressing other keys, they can be tapped and
released, and will remain active until any other key is pressed. In short, they
turn `Shift+A` into `Shift, A`. The main advantage is that this allows us to
place the modifiers to positions that would otherwise be awkward when chording.
Nevertheless, they still act as normal modifiers when held, that behaviour is
not lost.

Furthermore, if a one-shot modifier is tapped two times in quick succession, it
becomes sticky, and remains active until disabled with a third tap. This can be
useful when one needs to input a number of keys with the modifier active, and
still does not wish to hold the modifier down.

To make multi-modifier shortcuts possible, one-shot modifiers remain active if
another one-shot modifier is tapped, so `Ctrl+Alt+b` becomes `Ctrl, Alt, b`.

## Using the plugin

```c++
#include <Akela-OneShot.h>

static Akela::OneShotMods oneShotMods;

void setup () {
  oneShotMods.enableAuto();
  Keyboardio.setup(1);
}
```

To use the plugin, the header needs to be included, and some behaviour
configured by instantiating an `Akela::OneShotMods` object with the desired
parameters. By default, one-shot mods start enabled, meaning any modifier marked
up with the `OSM()` macro will have the one-shot behaviour. If told to start
disabled, then these modifiers will behave as usual, without the added
behaviour.

## Keymap markup

There is only one macro the plugin provides, `OSM(mod)`, that takes a single
argument, the name of the modifier: `LCtrl`, `LShift`, `LAlt`, `LGUI` or their
right-side variant. When marked up with this macro, the modifier will act as a
one-shot modifier.

To avoid having to change the keymap to add `OSM()` markup, the `enableAuto()`
method can be used to turn all modifiers into one-shots.

## Plugin methods

```c++
namespace Akela {
  class OneShotMods {
  public:
    OneShotMods (uint8_t defaultState);
    OneShotMods (void) : OneShotMods (Default::On) {};

    static void on (void);
    static void off (void);
    static void enableAuto (void);
    static bool isActive (void);
    static void cancel (void);
  };
};
```

The `on()` and `off()` methods can be used in macros, or elsewhere, to turn the
*one-shot* behaviour on or off. When turned off, then modifiers that were marked
as one-shot in the keymap, will behave as if they were just normal modifiers.
These can be called at any time, and have the effect one would expect.

Similar to these, `enableAuto()` turns all the modifiers on the keymap into
one-shot modifiers - but only those on the keymap. Any injected modifiers are
ignored. This operation cannot later be reversed, but one can still turn the
one-shot behaviour off with the `off()` method. Furthermore, for the
transformation to work, this method needs to be called before
`Keyboardio.setup()`. Its purpose is to make it possible to re-use a key layout
without one-shot modifiers, as-is, but still have one-shot behaviour.

The `isActive()` method can be used to check if any one-shot modifiers are
active, to differentiate between having a modifier active, and active only until
the next key is pressed. This becomes important for macros that need to change
which modifiers are pressed: if they want to restore the previous state of
modifiers, they need to know if one-shot was in play. Because if it was, they
need not restore the modifiers their former state.

Similarly, the `cancel()` method can be used to immediately cancel any one-shot
modifiers. Useful in case one wants to be able to change their minds, and cancel
a one-shot modifier pressed by mistake.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-OneShot/examples/OneShot/OneShot.ino
