---
title: "One-shot keys"
permalink: /plugins/OneShot/
excerpt: "One-shot modifiers & layers"
modified: 2016-12-04T12:15:00+01:00
---

{% include toc %}

One-shots are a new kind of behaviour for your standard modifier and momentary
layer keys: instead of having to hold them while pressing other keys, they can
be tapped and released, and will remain active until any other key is pressed.
In short, they turn `Shift, A` into `Shift+A`, and `Fn, 1` to `Fn+1`. The main
advantage is that this allows us to place the modifiers and layer keys to
positions that would otherwise be awkward when chording. Nevertheless, they
still act as normal when held, that behaviour is not lost.

Furthermore, if a one-shot key is tapped two times in quick succession, it
becomes sticky, and remains active until disabled with a third tap. This can be
useful when one needs to input a number of keys with the modifier or layer
active, and still does not wish to hold the key down.

To make multi-modifier, or multi-layer shortcuts possible, one-shot keys remain
active if another one-shot of the same type is tapped, so `Ctrl, Alt, b` becomes
`Ctrl+Alt+b`, and `L1, L2, c` is turned into `L1+L2+c`.

## Using the plugin

```c++
#include <Akela-OneShot.h>

static Akela::OneShotMods   oneShotMods;
static Akela::OneShotLayers oneShotLayers;

void setup () {
  oneShotMods.enableAuto();
  oneShotLayers.enableAuto();
  Keyboardio.setup(KEYMAP_SIZE);
}
```

To use the plugin, the header needs to be included, and some behaviour
configured by instantiating an `Akela::OneShotMods` or `Akela::OneShotLayers`
(or both) object with the desired parameters. 

By default, one-shot keys start enabled, meaning any modifier marked up with the
`OSM()` or `OSL()` macros will have the one-shot behaviour. If told to start
disabled, then these keys will behave as usual, without the added behaviour.

## Keymap markup

There are two macros the plugin provides: `OSM(mod)`, that takes a single
argument, the name of the modifier: `LCtrl`, `LShift`, `LAlt`, `LGUI` or their
right-side variant. When marked up with this macro, the modifier will act as a
one-shot modifier. The other is `OSL(layer)`, that takes a layer number, and
turns the key into a one-shot layer key.

To avoid having to change the keymap to add `OSM()` or `OSL()` markup, the
`enableAuto()` method of either class discussed above, can be used to turn all
modifiers, or momentary layer keys into one-shots.

## Plugin methods

```c++
namespace Akela {
  class OneShotMods {
  public:
    OneShotMods (uint8_t defaultState);
    OneShotMods (void);

    static void on (void);
    static void off (void);
    static void enableAuto (void);
    static bool isActive (void);
    static void cancel (void);
  };

  class OneShotLayers {
  public:
    OneShotLayers (uint8_t defaultState);
    OneShotLayers (void);

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
as one-shot in the keymap, will behave as if they were just normal modifiers;
and layers that were marked as one-shot will behave as momentary layer switcher.
These can be called at any time, and have the effect one would expect.

Similar to these, `enableAuto()` turns all the modifiers - or all the momentary
layer keys - on the keymap into one-shot keys - but only those on the keymap.
Any injected modifiers or layer switchers are ignored. This operation cannot
later be reversed, but one can still turn the one-shot behaviour off with the
`off()` method. Furthermore, for the transformation to work, this method needs
to be called before `Keyboardio.setup()`. Its purpose is to make it possible to
re-use a key layout without one-shot keys, as-is, but still have one-shot
behaviour.

The `isActive()` method can be used to check if any one-shot keys are active, to
differentiate between having a modifier or layer active, and active only until
the next key is pressed. This becomes important for macros that need to change
which modifiers (or layers) are active: if they want to restore the previous
state, they need to know if one-shot was in play. Because if it was, they need
not do the restoration.

Similarly, the `cancel()` method can be used to immediately cancel any one-shot
modifiers or layers. Useful in case one wants to be able to change their minds,
and cancel a one-shot pressed by mistake.

All of these must be called on the appropriate `Akela::OneShotMods` or
`Akela::OneShotLayers` instance.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-OneShot/examples/OneShot/OneShot.ino
