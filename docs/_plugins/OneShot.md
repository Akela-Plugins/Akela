---
title: "One-shot keys"
permalink: /plugins/OneShot/
excerpt: "One-shot modifiers & layers"
modified: 2016-12-28T08:25:00+01:00
---

{% include toc %}

![status](https://img.shields.io/badge/buggy----black.png?style=flat&colorA=dfb317&colorB=494e52)

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

There are two major ways in which the plugin can be used: one is to turn
existing modifiers or momentary layer toggles into one-shot keys:

```c++
#include <Akela-OneShot.h>

void setup () {
  OneShot.enableAuto ();
  
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&OneShot, NULL);
}
```

The other is to explicitly mark keys as one-shot in the keymap:

```c++
#include <Akela-OneShot.h>

// somewhere in the keymap...
OSM(LCtrl), OSL(_FN)

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&OneShot, NULL);
}
```

## Keymap markup

There are two macros the plugin provides: 

### `OSM(mod)`

> A macro that takes a single argument, the name of the modifier: `LCtrl`,
> `LShift`, `LAlt`, `LGUI` or their right-side variant. When marked up with this
> macro, the modifier will act as a one-shot modifier.

### `OSL(layer)`

> Takes a layer number as argument, and sets up the key to act as a one-shot
> layer key.
>
> Please note that while `KeyboardioFirmware` supports more, one-shot layers are
> limited to 24 layers only.

## Plugin methods

The plugin provides one object, `OneShot`, which implements both one-shot
modifiers and one-shot layer keys. It has the following methods:

### `.enableAuto()`

> Automatically turns modifiers and momentary layer switches into their one-shot
> variant. It will only turn keys on the keymap into one-shots: if any macro
> injects a modifier or a momentary layer switch key, those will be left alone,
> as-is.
>
> This **must** be called before any `Keyboardio.use()` call in the `setup()`
> method of your Sketch.

### `.isActive()`

> Returns if any one-shot key is in flight. This makes it possible to
> differentiate between having a modifier or layer active, versus having them
> active only until after the next key getting pressed. And this, in turn, is
> useful for macros that need to fiddle with either modifier or layer state: if
> one-shots are not active, they need not restore the original state.

### `.cancel([withStickies])`

> The `cancel()` method can be used to cancel any pending one-shot effects,
> useful when one changed their minds, and does not wish to wait for the
> timeout.
>
> The optional `withStickies` argument, if set to `true`, will also cancel
> sticky one-shot effects. If omitted, it defaults to `false`, and not canceling
> stickies.

### `.on()`

> Turns the one-shot behaviour on. This method is idempotent, you can call it
> any number of times, at any time.

### `.off()`

> The counterpart of the `on()` method, this turns off one-shot behaviour.
> Modifiers will act as normal modifiers, and one-shot layer keys will act as
> momentary layer switchers. As `on()`, this method is idempotent, and can be
> called at any time, from anywhere.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-OneShot/examples/OneShot/OneShot.ino
