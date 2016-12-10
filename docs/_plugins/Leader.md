---
title: "Leader keys"
permalink: /plugins/Leader/
excerpt: "VIM-style Leader keys"
modified: 2016-12-10T12:40:00+01:00
---

{% include toc %}

Leader keys are a kind of key where when they are tapped, all following keys are
swallowed, until the plugin finds a matching sequence in the dictionary, it
times out, or fails to find any possibilities. When a sequence is found, the
corresponding action is executed, but the processing still continues. If any key
is pressed that is not the continuation of the existing sequence, processing
aborts, and the key is handled normally.

This behaviour is best described with an example. Suppose we want a behaviour
where `LEAD u` starts unicode input mode, and `LEAD u h e a r t` should result
in a heart symbol being input, and we want `LEAD u 0 0 e 9 SPC` to input `é`,
and any other hex code that follows `LEAD u`, should be handled as-is, and
passed to the host. Obviously, we can't have all of this in a dictionary.

So we put `LEAD u` and `LEAD u h e a r t` in the dictionary only. The first will
start unicode input mode, the second will type in the magic sequence that
results in the symbol, and then aborts the leader sequence processing. With this
setup, if we type `LEAD u 0`, then `LEAD u` will be handled first, and start
unicode input mode. Then, at the `0`, the plugin notices it is not part of any
sequence, so aborts leader processing, and passes the key on as-is, and it ends
up being sent to the host. Thus, we covered all the cases of our scenario!

## Using the plugin

To use the plugin, one needs to include the header, implement some actions,
create a dictionary, and configure the provided `Leader` object to use the
dictionary:

```c++
#include <Akela-Leader.h>

static void leaderA (void) {
  Serial.println ("leaderA");
}

static void leaderTX (void) {
  Serial.println ("leaderTX");
}

static const Akela::Leader::dictionary_t leaderDictionary = LEADER_DICT
  (
    {LEADER_SEQ (LEAD (0), Key_A), leaderA},
    {LEADER_SEQ (LEAD (0), Key_T, Key_X), leaderTX}
  );

void setup () {
  Leader.configure(shapeShiftDictionary);
  Keyboardio.setup(KEYMAP_SIZE);
}
```

The dictionary is made up of a list of keys, and an action callback. Using the
`LEADER_DICT` and `LEADER_SEQ` helpers is recommended.

**Note** that we need to include `Akela-Leader.h` before any other plugin that
adds or changes key behavior! It is best to include it right after
`KeyboardioFirmware.h`. Failing to do so may result in unpredictable behaviour.

## Plugin methods

The plugin provides the `Leader` object, with the following methods:

### `.configure(dictionary)`

> Tells `Leader` to use the specified dictionary. The dictionary is an array of
> `Akela::Leader::dictionary_t` elements. Each element is made up of two
> elements, the first being a list of keys, the second an action to perform when
> the sequence is found.

### `.reset()`

> Finishes the leader sequence processing. This is best called from actions that
> are final actions, where one does not wish to continue the leader sequence
> further in the hopes of finding a longer match.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/algernon/Akela/blob/master/lib/Akela-Leader/examples/Leader/Leader.ino
