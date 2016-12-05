---
title: "Shifted symbol replacement"
permalink: /plugins/ShapeShifter/
excerpt: "Replace the shifted symbol on a number of keys."
modified: 2016-12-05T11:20:00+01:00
---

{% include toc %}

`ShapeShifter` is a plugin that makes it considerably easier to change what
symbol is input when a key is pressed together with `Shift`. If one wants to
rearrange the symbols on the number row for example, without modifying the
layout on the operating system side, this plugin is where one can turn to.

What it does, is very simple: if any key in its dictionary is found pressed
while `Shift` is held, it will press another key instead of the one triggering
the event. For example, if it sees `Shift + 1` pressed together, which normally
results in a `!`, it will press `4` instead of `1`, inputting `$`.

## Using the plugin

```c++
#include <Akela-ShapeShifter.h>

static const Akela::ShapeShifter::dictionary_t shapeShiftDictionary[] = {
 {Key_1, Key_4},
 {Key_4, Key_1},
 {Key_NoKey, Key_NoKey},
};

static Akela::ShapeShifter shapeShifter(shapeShiftDictionary);
```

The dictionary is made up of `Key` pairs: the first one is to replace, the
second is the replacement. The dictionary must be closed with a `{Key_NoKey,
Key_NoKey}` pair.

## Plugin methods

```c++
namespace Akela {
  class ShapeShifter {
  public:
    typedef struct {
      Key original, replacement;
    } dictionary_t;

    ShapeShifter (const dictionary_t dictionary[]);

    void on (void);
    void off (void);
  };
};
```

The `on()` and `off()` methods can be called anytime, to turn the plugin on and
off, respectively.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-ShapeShifter/examples/ShapeShifter/ShapeShifter.ino
