---
title: "Host OS detection & tracking"
permalink: /plugins/HostOS/
excerpt: "Utilities to help guess and/or track the Host OS."
modified: 2016-12-22T11:30:00+01:00
---

{% include toc %}

The `HostOS` extension is not all that useful in itself, rather, it is a
building block other plugins and extensions can use to not repeat the same
guesswork and logic. Its primary purpose is to help either detect, or keep track
of the host operating system. The detection part is not the most reliable thing,
mind you.

The goal is to have a single place that remembers the host OS, either detected,
or set by the end-user, in a Sketch, or via a macro, or some other way. This
information can then be reused by other plugins.

See the [Unicode][plugin:unicode] extension for an example about how to use
`HostOS` in practice.

 [plugin:unicode]: {{ site.baseurl }}/plugins/Unicode/

## Using the extension

The extension provides a `HostOS` singleton object. It can either be a simple
one without auto-detection (the default), or one that will try to detect the
Host OS, using the [FingerprintUSBHost][fprdetect] library. To enable
auto-detection, `AKELA_HOSTOS_GUESSER` must be defined before including the
`HostOS` library header.

 [fprdetect]: https://github.com/keyboardio/FingerprintUSBHost

```c++
#define AKELA_HOSTOS_GUESSER 1

#include <Akela-HostOS.h>

void someFunction (void) {
  if (HostOS.os() == Akela::HostOS::LINUX) {
    // do something linux-y
  }
  if (HostOS.os() == Akela::HostOS::OSX) {
    // do something OSX-y
  }
}

void setup (void) {
  HostOS.setup ();
  
  Keyboardio.setup (KEYMAP_SIZE);
}
```

## Extension methods

The extension provides the following methods on the `HostOS` singleton:

### `setup()`

> Initializes the plugin, by reading the `HostOS` setting from EEPROM. If set to
> `AUTO`, it will attempt to do some auto-detection, if the
> `AKELA_HOSTOS_GUESSER` flag has been set before including the HostOS library
> header. Otherwise no auto-detection is done.
>
> This method should be called as early as possible.

### `os()`

> Returns the stored type of the Host OS.

### `os(type)`

> Sets the type of the host OS, overriding any previous value. The type is then
> stored in EEPROM for persistence.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the extension.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-HostOS/examples/HostOS/HostOS.ino
