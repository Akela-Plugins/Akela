---
title: "Space Cadet Shift"
permalink: /plugins/SpaceCadet/
excerpt: "Space Cadet Shift plugin for the Keyboardio Model 01"
modified: 2016-11-22T13:30:00+01:00
---

{% include toc %}

[Space Cadet][space-cadet] Shift is a way to make it more convenient to input
parens - those `(` and `)` things -, symbols that a lot of programming languages
use frequently. If you are working with Lisp, you are using these all the time.

What it does, is that it turns your left and right `Shift` keys into parens if
you tap and release them, without pressing any other key while holding them.
Therefore, to input, say, `(print foo)`, you don't need to press `Shift`, hold
it, and press `9` to get a `(`, you simply press and release `Shift`, and
continue writing. You use it as if you had a dedicated key for parens!

But if you wish to write capital letters, you hold it, as usual, and you will
not see any parens when you release it. You can also hold it for a longer time,
and it still would act as a `Shift`, without the parens inserted on release:
this is useful when you want to augment some mouse action with `Shift`, to
select text, for example.

 [space-cadet]: https://en.wikipedia.org/wiki/Space-cadet_keyboard

## Using the plugin

```c++
#include <Akela-SpaceCadet.h>

static Akela::SpaceCadetShift spaceCadetShift;
```

There is only one configurable option: whether to enable the new behaviour (the
default), or not. If disabled, the `on()` method can be used later to enable it.
The keymap does not need to change, the plugin overrides the normal `Shift`
behaviour.

## Plugin methods

```c++
namespace Akela {
  class SpaceCadetShift {
  public:
    SpaceCadetShift (uint8_t defaultMode);
    SpaceCadetShift (void) : SpaceCadetShift (Akela::Default::On) {};

    void on (void);
    void off (void);
  };
};
```

The `on()` and `off()` methods can be used in macros, or elsewhere, to turn the
*Space Cadet Shift* behaviour on or off. When turned off, shifts behave as
usual. These can be called at any time, and have the effect one would expect.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-SpaceCadet/examples/SpaceCadet/SpaceCadet.ino
