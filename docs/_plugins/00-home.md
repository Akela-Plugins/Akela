---
title: "Plugins"
permalink: /plugins/
excerpt: "Plugins for the Keyboardio Model 01"
modified: 2016-11-22T16:00:00+01:00
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
$ install -d ~/Arduino/libraries/
$ ln -s $(pwd)/lib/* ~/Arduino/libraries/
```

Of course, the Model 01 hardware support also needs to be installed, but that is
out of the scope for this document, for now. With the Model 01 support installed
for the IDE, and after the above few lines, when we start up the IDE, the
`File/Examples` menu will have all the available examples under the "*Examples
from Custom Libraries*" heading. Select one from there, and we can start playing!

For more in-depth information about each available plugin, please see the
sidebar.
