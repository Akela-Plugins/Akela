---
title: "Getting started"
permalink: /guide/getting-started/
excerpt: "Getting started with Akela"
modified: 2016-11-09T09:00:00+01:00
---

{% include toc %}

[Akela][akela] is a library, and comes with a number of examples. In this guide,
we'll see how to get started, by setting up the environment, cloning the
repository, and building the examples on both the command line, and from
the [Arduino IDE][arduino:ide].

[akela]: https://github.com/algernon/Akela
[arduino:ide]: https://www.arduino.cc/en/Main/Software

For now, this guide will assume the reader is using a GNU/Linux operating
system.

## Setting up the environment

There are two parts of Akela that may be interesting to us: the test suite, and
the examples. For the test suite, one only needs any reasonable C++ compiler
that supports C++11. Recent versions of both Clang and GCC will do just fine for
this purpose.

To compile the examples, one needs both the [Arduino][arduino:ide] software
package, and a cross-compiler that can target AVR, such as `avr-gcc`. On a
Debian-based system, one can install the cross-compiler by installing the
`gcc-avr` package:

```
$ sudo apt install gcc-avr
```

As for installing the Arduino IDE, please see
their [getting started][arduino:getting-started] page for instructions.

[arduino:getting-started]: https://www.arduino.cc/en/Guide/HomePage

Note down where you installed the Arduino IDE, you will need it later. For the
purpose of these guides, we will assume that the IDE is installed to
`~/install/arduino`. Exporting an `ARDUINO_PATH` environment variable tells the
command-line build system where to find the IDE and its libraries.

## Cloning the repository

The repository uses submodules for the external dependencies, to make it more
convenient to work with it. But this means that cloning the library, and the
dependencies, requires some extra steps:

```
$ git clone --recursive https://github.com/algernon/Akela.git
```

With the pre-requisite software installed, this is all that is needed to get
started with the library itself!

## Building an example

We will cover building an example in two ways: from the command-line, which
requires no additional setup; and from the Arduino IDE, where we will need to
configure some paths to let the IDE know where we have our libraries.

### Building from the command-line

All of the examples included with the library can be built with the same,
generic method: `make MODULE/EXAMPLE`. At the time of this writing, the
following examples are available:

| Module     | Example          | Description                                                                                  |
|------------|------------------|----------------------------------------------------------------------------------------------|
| keyboardio | minimal          | Minimalistic firmware for the [Model 01][m01]: Single layer, no special keys, or effects.    |
| keyboardio | stripped         | A simple firmware for the [Model 01][m01]: Multiple layers, multimedia keys, but no effects. |
| keyboardio | Model01          | Firmware mimicking the default firmware for the [Model 01][m01].                             |
| keyboardio | example          | A more complex example for the [Model 01][m01], that shows how to add some customization.    |
|------------------------------------------------------------------------------------------------------------------------------|
| tapdance   | TapDanceKeyboard | An example showing how to use various TapDance features.                                     |
|------------------------------------------------------------------------------------------------------------------------------|
| unicode    | unicode          | A simple example showing how to use unicode input.                                           |

[m01]: https://shop.keyboard.io/

All of the examples are for the [Keyboardio Model 01][m01], as that is the only
hardware currently supported by the library.

To build the `Model01` example of the `keyboardio` module, simply use the
following command:

```
$ make keyboardio/Model01 ARDUINO_PATH=/path/to/arduino
```

Running `make` without any other arguments will build all the examples, and the
test suite too. This latter one can be run with the `make check` command.

The `ARDUINO_PATH` variable should contain the path leading up to the `arduino`
and `arduino-builder` binaries. Usually, this is the same path you installed the
Arduino IDE under. We can either specify it on the `make` command-line every
time we invoke it, or export it as an environment variable:

```
$ export ARDUINO_PATH=/path/to/arduino
$ make keyboardio/Model01
```

### Building with the Arduino IDE

To build with the Arduino IDE, we will need to let it know where to find the
library, and the hardware specs first. Assuming that we used `~/Arduino` as our
Sketchbook location, all we need is a couple of symbolic links:

```
$ ln -s $(pwd)/lib/Akela-* ~/Arduino/libraries/
$ ln -s $(pwd)/user ~/Arduino/libraries/Akela-User
$ ln -s $(pwd)/hardware ~/Arduino/
```

One these links are in place, we can start up the Arduino IDE, and select any of
the included examples from the `File/Examples` menu. There will be under the
`Examples from Custom Libraries` section. When compiling them, don't forget to
set the board type to `Keyboardio Model 01` in the `Tools` menu.
