---
title: "LED Effects"
permalink: /plugins/LEDEffects/
excerpt: "An assorted collection of LED effects."
modified: 2016-12-16T12:20:00+01:00
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
`Akela::USE(pluginEffect)`, with the selected effect objects.

## Included effects

All of these effects will scan the active layers, and apply effects based on
what keys are active on each position, thus, it needs no hints or configuration
to figure out our layout!

### `MiamiEffect`

Applies a color effect to the keyboard, inspired by the popular Miami keyset:

![Miami]({{ '/assets/images/LEDEffects/MiamiEffect.png' | absolute_url }})

Alphas, punctuation, numbers, the space bar, the numbers and the dot on the
keypad, and half the function keys will be in a cyan-ish color, the rest in
magenta.

### `JukeBoxEffect`

Applies a color effect to the keyboard, inspired by the JukeBox keyset:

![Jukebox]({{ '/assets/images/LEDEffects/JukeboxEffect.png' | absolute_url }})

Alphas, punctuation, numbers, the space bar, the numbers and the dot on the
keypad, and half the function keys will be in a beige-ish color, the rest in
light green, except for the `Esc` key, which will be in red.

An alternative color scheme exists under the `JukeBoxAlternateEffect` name,
where the light green and red colors are swapped.

## Plugin methods

The plugin provides a single method on each of the included effect objects:

### `.activate()`

> When called, immediately activates the effect. Mostly useful in the `setup()`
> method of the Sketch, or in macros that are meant to switch to the selected
> effect, no matter where we are in the list.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-LEDEffects/examples/LEDEffects/LEDEffects.ino
