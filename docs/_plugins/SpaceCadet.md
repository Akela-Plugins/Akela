---
title: "Space Cadet Shift"
permalink: /plugins/SpaceCadet/
excerpt: "Space Cadet Shift plugin for the Keyboardio Model 01"
modified: 2016-11-23T12:40:00+01:00
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

There are two properties one can configure: the default behaviour, and the paren
keys. See the constructors [below](#plugin-methods) to see how to set these
properties.

The first property, `defaultMode` controls whether the new behaviour is on or
off when the keyboard starts. It defaults to *on*, but if set to *off*, it can
later be enabled with the `on()` method. The keymap does not need to be updated
at all, the plugin overrides the normal `Shift` keys when the functionality is
enabled.

The other option is the keys to use for the parens. The plugin defaults to `9`
and `0`, which, when shifted, mean the opening and closing parens in US QWERTY,
and a lot of other layouts. If one uses an OS-side layout where the parens are
on different keys, the `left` and `right` options are the ones to change.

## Plugin methods

```c++
namespace Akela {
  class SpaceCadetShift {
  public:
    SpaceCadetShift (uint8_t defaultMode, Key left, Key right);
    SpaceCadetShift (uint8_t defaultMode);
    SpaceCadetShift (Key left, Key right);
    SpaceCadetShift (void);

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
