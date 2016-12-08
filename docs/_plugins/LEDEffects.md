---
title: "LED Effects"
permalink: /plugins/LEDEffects/
excerpt: "An assorted collection of LED effects."
modified: 2016-12-08T19:45:00+01:00
---

{% include toc %}

The `LEDEffects` plugin provides a selection of LED effects, each of them fairly
simple, simple enough to not need a plugin of their own. 

## Using the plugin

Because of containing a number of different effects, and not wanting to force
all of them on the user, the plugin behaves differently than most others: it
does not enable anything by default, and we will have to selectively enable the
effects we are interested in.

```c++
#include <Akela-LEDEffects.h>
```

Then, in the `setup()` method of our Sketch, we will call
the [`.enable()`](#enable) method of the appropriate effect.

## Included effects

### `MiamiEffect`

Applies a color effect to the keyboard inspired by the popular Miami keyset:

![Miami]({{ '/assets/images/LEDEffects/MiamiEffect.png' | absolute_url }})

Alphas, punctuation, numbers, the space bar, the numbers and the dot on the
keypad, and half the modifiers will be in a cyan-ish color, the rest in magenta.
The effect will scan the active layers, and apply effects based on what keys are
active on each position, thus, it needs no hints or configuration to figure out
our layout!

## Plugin methods

The plugin provides two methods on each of the included effect objects:

### `.enable()`

> Enable the effect. Without either this or [.activate()](#activate), the effect
> will not be included in our firmware. This method simply makes sure that the
> effect is registered, it will not activate it.

### `.activate()`

> When called, immediately activates the effect. Mostly useful in the `setup()`
> method of the Sketch, or in macros that are meant to switch to the selected
> effect, no matter where we are in the list.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-LEDEffects/examples/LEDEffects/LEDEffects.ino
