/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2016  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OneShot.h"

using namespace Akela::Ranges;

namespace Akela {
  // ---- state ---------

  static uint16_t Timer = 0;
  static uint16_t TimeOut = DEFAULT_TIMEOUT;
  static uint32_t State = 0;
  static uint32_t stickyState = 0;
  static Key prevKey;
  static bool shouldCancel = false;
  static bool shouldCancelStickies = false;

  // --- helper macros ------

#define isOS(key) (key.raw >= OS_FIRST && key.raw <= OS_LAST)
#define isModifier(key) (key.raw >= Key_LCtrl.raw && key.raw <= Key_RGUI.raw)
#define isLayerKey(key) (key.flags & (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.rawKey >= MOMENTARY_OFFSET)

#define isOneShot(idx) (bitRead (State, idx))
#define setOneShot(idx) (bitWrite (State, idx, 1))
#define clearOneShot(idx) (bitWrite (State, idx, 0))

#define isSticky(idx) (bitRead (stickyState, idx))
#define setSticky(idx) (bitWrite (stickyState, idx, 1))
#define clearSticky(idx) bitWrite (stickyState, idx, 0)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define toNormalMod(key, idx) {key.flags = 0; key.rawKey = Key_LCtrl.rawKey + idx;}
#define toNormalMT(key, idx) { key.raw = Key_NoKey.raw; Layer.on (idx - 8); }
#define hasTimedOut() (Timer > TimeOut)

  // ----- passthrough ------

  Key
  OneShot::eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!isOS (mappedKey))
      return mappedKey;

    uint8_t idx = mappedKey.raw - OS_FIRST;

    if (idx >= 8) {
      toNormalMT (mappedKey, idx);
    } else {
      toNormalMod (mappedKey, idx);
    }

    return mappedKey;
  }

  void
  OneShot::loopNoOpHook (bool postClear) {
  }

  // ---- OneShot stuff ----

  static bool
  shouldInterrupt (Key mappedKey) {
    return !isOS (mappedKey);
  }

  Key
  OneShot::eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!isModifier (mappedKey) && !isLayerKey (mappedKey))
      return mappedKey;

    // If mappedKey is an injected key, we don't fiddle with those.
    if (keyState & INJECTED)
      return mappedKey;

    if (isModifier (mappedKey)) {
      uint8_t idx = mappedKey.rawKey - Key_LCtrl.rawKey;

      return (Key){.raw = OSM_FIRST + idx};
    } else {
      uint8_t idx = mappedKey.rawKey - MOMENTARY_OFFSET;

      return (Key){.raw = OSL_FIRST + idx};
    }
  }

  Key
  OneShot::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (State && shouldInterrupt (mappedKey) && key_toggled_on (keyState) && !(keyState & INJECTED))
      cancel ();

    if (!isOS (mappedKey)) {
      if (!(keyState & INJECTED) && key_toggled_on (keyState))
        saveAsPrevious (mappedKey);
      return mappedKey;
    }

    // If nothing happened, bail out fast.
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      return Key_NoKey;
    }

    // Released?
    if (!key_is_pressed (keyState)) {
      if (hasTimedOut ())
        cancel ();

      return Key_NoKey;
    }

    uint8_t idx = mappedKey.raw - OS_FIRST;

    if (key_toggled_on (keyState)) {
      if (isSticky(idx)) {
        cancel (true);
        return Key_NoKey;
      } else if (isSameAsPrevious (mappedKey)) {
        setSticky (idx);
      }
    }

    saveAsPrevious (mappedKey);
    if (idx >= 8) {
      toNormalMT (mappedKey, idx);
    } else {
      toNormalMod (mappedKey, idx);
    }
    setOneShot (idx);

    return mappedKey;
  }

  void
  OneShot::loopHook (bool postClear) {
    if (!State)
      return;

    Timer++;

    if (hasTimedOut ())
      cancel ();

    for (uint8_t i = 0; i < 32; i++) {
      if (shouldCancel) {
        if (isSticky (i)) {
          if (shouldCancelStickies) {
            clearSticky (i);
          }
        } else if (isOneShot (i)) {
          if (postClear) {
            clearOneShot (i);
            if (i >= 8) {
              Layer.off (i - 8);
            }
          }
        }
      } else if (isOneShot (i) && i < 8) {
        uint8_t m = Key_LCtrl.rawKey + i;
        handle_key_event ({0, m}, 0, 0, IS_PRESSED | INJECTED);
      }
    }

    if (shouldCancel) {
      Timer = 0;
      shouldCancel = false;
      shouldCancelStickies = false;
    }
  }

  // --- glue code ---

  OneShot::OneShot (void) {
  }

  void
  OneShot::begin (void) {
    event_handler_hook_add (eventHandlerHook);
    loop_hook_add (loopHook);
  }

  bool
  OneShot::isActive (void) {
    return (State && !hasTimedOut () && !shouldCancel);
  }

  void
  OneShot::cancel (bool withStickies) {
    shouldCancel = true;
    shouldCancelStickies = withStickies;
  }

  void
  OneShot::on (void) {
    event_handler_hook_replace (eventHandlerPassthroughHook, eventHandlerHook);
    loop_hook_replace (loopNoOpHook, loopHook);
  }

  void
  OneShot::off (void) {
    event_handler_hook_replace (eventHandlerHook, eventHandlerPassthroughHook);
    loop_hook_replace (loopHook, loopNoOpHook);
  }

  void
  OneShot::enableAuto (void) {
    event_handler_hook_add (eventHandlerAutoHook);
  }

  void
  OneShot::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

};

Akela::OneShot OneShot;
