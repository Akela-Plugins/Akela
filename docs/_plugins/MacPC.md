---
title: "Mac/PC behaviour switching"
permalink: /plugins/MacPC/
excerpt: "A small plugin that helps switching between Mac and PC"
modified: 2016-12-13T09:05:00+01:00
---

{% include toc %}

Ever had to switch between Mac and PC? Ever had the misfortune of either missing
the `⌘` key, or `Alt`? Worry no more. With this plugin, if you just use
`Key_AltCmd` in your keymap, all will be well. When plugged into a Mac, it will
act as a `Command` key, otherwise as a normal `Alt`.

Uses the [HostOS](../HostOS/) plugin under the hood, all detection and setup is
the responsibility of that extension.

## Using the plugin

Using the plugin is as easy as including the header, and using the `Key_AltCmd`
key in your keymap in a position where you want `⌘` on Mac, and `Alt` on PC.

```c++
#include <Akela-MacPC.h>

void setup(void) {
  MacPC.setup();
  Keyboardio.setup(KEYMAP_SIZE);
}
```

**Note**: We must call `MacPC.setup()` before anything else in the `setup`
method of our Sketch.

## Plugin methods

The plugin provides one method on the `MacPC` object:

### `.setup()`

> Initializes the plugin, by trying to detect the host operating system.
> See [HostOS](../HostOS/) for details.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-MacPC/examples/MacPC/MacPC.ino
