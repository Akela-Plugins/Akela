/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2016, 2017  Gergely Nagy
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

  uint16_t OneShot::Timer = 0;
  uint16_t OneShot::TimeOut = DEFAULT_TIMEOUT;
  uint32_t OneShot::State = 0;
  uint32_t OneShot::stickyState = 0;
  uint32_t OneShot::pressedState = 0;
  Key OneShot::prevKey;
  bool OneShot::shouldCancel = false;
  bool OneShot::shouldCancelStickies = false;

  // --- helper macros ------

#define isOS(key) (key.raw >= OS_FIRST && key.raw <= OS_LAST)
#define isModifier(key) (key.raw >= Key_LCtrl.raw && key.raw <= Key_RGUI.raw)
#define isLayerKey(key) (key.flags & (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.rawKey >= MOMENTARY_OFFSET && key.rawKey <= MOMENTARY_OFFSET + 23)

#define isOneShot(idx) (bitRead (State, idx))
#define setOneShot(idx) (bitWrite (State, idx, 1))
#define clearOneShot(idx) (bitWrite (State, idx, 0))

#define isSticky(idx) (bitRead (stickyState, idx))
#define setSticky(idx) (bitWrite (stickyState, idx, 1))
#define clearSticky(idx) bitWrite (stickyState, idx, 0)

#define setPressed(idx) bitWrite(pressedState, idx, 1)
#define clearPressed(idx) bitWrite(pressedState, idx, 0)
#define isPressed(idx) bitRead (pressedState, idx)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define toNormalMod(key, idx) {key.flags = 0; key.rawKey = Key_LCtrl.rawKey + idx;}
#define toNormalMT(key, idx) { key.raw = Key_NoKey.raw; Layer.on (idx - 8); }
#define hasTimedOut() (Timer >= TimeOut)

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

  Key
  OneShot::eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If mappedKey is an injected key, we don't fiddle with those.
    if (keyState & INJECTED)
      return mappedKey;

    if (!isModifier (mappedKey) && !isLayerKey (mappedKey))
      return mappedKey;

    if (isModifier (mappedKey)) {
      uint8_t idx = mappedKey.rawKey - Key_LCtrl.rawKey;

      return (Key){.raw = OSM_FIRST + idx};
    } else {
      uint8_t idx = mappedKey.rawKey - MOMENTARY_OFFSET;

      return (Key){.raw = OSL_FIRST + idx};
    }
  }

  void
  OneShot::injectNormalKey (uint8_t idx, uint8_t keyState) {
    Key key;

    if (idx < 8) {
      key.flags = Key_LCtrl.flags;
      key.rawKey = Key_LCtrl.rawKey + idx;
    } else {
      key.flags = KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP;
      key.rawKey = MOMENTARY_OFFSET + idx - 8;
    }

    handle_key_event (key, 255, 255, keyState | INJECTED);
  }

  void
  OneShot::activateOneShot (uint8_t idx) {
    injectNormalKey (idx, IS_PRESSED);
  }

  void
  OneShot::cancelOneShot (uint8_t idx) {
    clearOneShot (idx);
    injectNormalKey (idx, WAS_PRESSED);
  }

  Key
  OneShot::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    uint8_t idx;

    if (keyState & INJECTED)
      return mappedKey;

    if (!State) {
      if (!isOS (mappedKey))
        return mappedKey;

      idx = mappedKey.raw - OS_FIRST;
      if (key_toggled_off (keyState)) {
        clearPressed (idx);
      } else if (key_toggled_on (keyState)){
        Timer = 0;
        setPressed (idx);
        setOneShot (idx);
        saveAsPrevious (mappedKey);

        activateOneShot (idx);
      }

      return Key_NoKey;
    }

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState))
      return mappedKey;

    if (isOS (mappedKey)) {
      idx = mappedKey.raw - OS_FIRST;

      if (isSticky (idx)) {
        if (key_toggled_on (keyState)) { // maybe on _off instead?
          saveAsPrevious (mappedKey);
          clearSticky (idx);
          cancelOneShot (idx);
        }
      } else {
        if (key_toggled_off (keyState)) {
          clearPressed (idx);
          if (hasTimedOut ()) {
            cancelOneShot (idx);
          }
        }

        if (key_toggled_on (keyState)) {
          setPressed (idx);
          if (isSameAsPrevious (mappedKey)) {
            setSticky (idx);

            saveAsPrevious (mappedKey);
          }
        }
      }

      return Key_NoKey;
    }

    // ordinary key here, with some event

    if (key_is_pressed (keyState)) {
      saveAsPrevious (mappedKey);
    }

    if (key_toggled_off (keyState)) {
      shouldCancel = true;
    }

    return mappedKey;
  }

  void
  OneShot::loopHook (bool postClear) {
    if (!State)
      return;

    if (!postClear) {
      if (Timer < TimeOut)
        Timer++;

      if (hasTimedOut ())
        cancel ();

      for (uint8_t i = 0; i < 32; i++) {
        if (shouldCancel) {
          if (isSticky (i)) {
            if (shouldCancelStickies) {
              Timer = 0;
              clearSticky (i);
            }
          } else if (isOneShot (i) && !isPressed (i)) {
            Timer = 0;
            cancelOneShot (i);
          }
        }
      }

      if (shouldCancel) {
        shouldCancel = false;
        shouldCancelStickies = false;
      }
    } else {
      for (uint8_t i = 0; i < 8; i++) {
        if (isOneShot (i)) {
          activateOneShot (i);
        }
      }
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
