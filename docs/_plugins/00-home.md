---
title: "Plugins"
permalink: /plugins/
excerpt: "Plugins for Keyboardio boards"
modified: 2016-12-05T21:35:00+01:00
read_time: true
---

The Akela project is composed of small libraries, that usually do one tiny
thing. There are example Arduino Sketches included with each and ever library,
and those are a reasonable starting point to discover the library, along with
the documentation.

All of these plugins are built for ease of use first and foremost. For a lot of
them, it is enough to include the header into your Sketch, and declare a
configuration for the plugin. For some, including the header is enough in
itself. This is well documented for each plugin, of course.

The easiest way to use the libraries is via the [Arduino IDE][arduino:ide], but
we need to make the libraries available to the IDE first. To do this from
scratch, we will see an example that clones the Akela project, and sets it up
for use with the Arduino IDE. The example assumes that our Sketchbook location
is set to `~/Arduino`.

 [arduino:ide]: https://www.arduino.cc/en/Main/Software

```
$ git clone --recurse-submodules https://github.com/algernon/Akela
$ cd Akela
$ tools/install-for-arduino ~/Arduino/
```

After these, when we start up the IDE, the `File/Examples` menu will have all
the available examples under the "*Examples from Custom Libraries*" heading.
Select one from there, and we can start playing!

## Plugins and Extensions

There are two main types of libraries included in Akela: *Plugins* and
*Extensions*. The difference between the two is the target audience.

### Plugins

*Plugins* are meant to be used by end-users, who want to customize their
firmware with additional features. Plugins, in general, require very little
coding: usually, they only require you to include a header, configure the
plugin, and perhaps modify your keymap a little, and that's about it.

They are rarely extensible further, as they are meant to be used as-is.

### Extensions

*Extensions* on the other hand, are aimed at developers, who want to write
additional features to extend the firmware with. These are building blocks one
can compose together, to build upon. They typically require a lot more coding
expertise, and are rarely useful out of the box.

## More information

For more in-depth information about each available plugin, please see the
sidebar.
