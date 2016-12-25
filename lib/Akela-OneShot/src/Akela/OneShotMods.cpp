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

#include "OneShotMods.h"

using namespace Akela::Ranges;

namespace Akela {
  // ---- state ---------

  static uint16_t Timer = 0;
  static uint16_t TimeOut = DEFAULT_TIMEOUT;
  static uint8_t State = 0;
  static uint8_t stickyState = 0;
  static Key prevKey;
  static bool shouldCancel = false;
  static bool shouldCancelStickies = false;

  // --- helper macros ------

#define isOSM(key) (key.raw >= OSM_FIRST && key.raw <= OSM_LAST)
#define isModifier(key) (key.raw >= Key_LCtrl.raw && key.raw <= Key_RGUI.raw)

#define isOneShot(idx) (bitRead (State, idx))
#define setOneShot(idx) (bitWrite (State, idx, 1))
#define clearOneShot(idx) (bitWrite (State, idx, 0))

#define isSticky(idx) (bitRead (stickyState, idx))
#define setSticky(idx) (bitWrite (stickyState, idx, 1))
#define clearSticky(idx) bitWrite (stickyState, idx, 0)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define toNormalMod(key, idx) {key.flags = 0; key.rawKey = Key_LCtrl.rawKey + idx;}
#define hasTimedOut() (Timer > TimeOut)

  // ----- passthrough ------

  Key
  OneShotMods::eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!isOSM (mappedKey))
      return mappedKey;

    uint8_t idx = mappedKey.raw - OSM_FIRST;
    toNormalMod (mappedKey, idx);

    return mappedKey;
  }

  void
  OneShotMods::loopNoOpHook (bool postClear) {
  }

  // ---- OneShot stuff ----

  static bool
  shouldInterrupt (Key mappedKey) {
    return !isOSM (mappedKey);
  }

  Key
  OneShotMods::eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!isModifier (mappedKey))
      return mappedKey;

    // If mappedKey is an injected key, we don't fiddle with those.
    if (keyState & INJECTED)
      return mappedKey;

    uint8_t idx = mappedKey.rawKey - Key_LCtrl.rawKey;

    return (Key){.raw = OSM_FIRST + idx};
  }

  Key
  OneShotMods::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (State && shouldInterrupt (mappedKey) && key_toggled_on (keyState) && !(keyState & INJECTED))
      cancel ();

    if (!isOSM (mappedKey)) {
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

    uint8_t idx = mappedKey.raw - OSM_FIRST;

    if (key_toggled_on (keyState)) {
      if (isSticky(idx)) {
        cancel (true);
        return Key_NoKey;
      } else if (isSameAsPrevious (mappedKey)) {
        setSticky (idx);
      }
    }

    saveAsPrevious (mappedKey);
    toNormalMod (mappedKey, idx);
    setOneShot (idx);

    return mappedKey;
  }

  void
  OneShotMods::loopHook (bool postClear) {
    if (!State)
      return;

    Timer++;

    if (hasTimedOut ())
      cancel ();

    for (uint8_t i = 0; i < 8; i++) {
      if (shouldCancel) {
        if (isSticky (i)) {
          if (shouldCancelStickies) {
            clearSticky (i);
          }
        } else if (isOneShot (i)) {
          clearOneShot (i);
        }
      } else if (isOneShot (i)) {
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

  OneShotMods::OneShotMods (void) {
  }

  void
  OneShotMods::begin (void) {
    event_handler_hook_add (eventHandlerHook);
    loop_hook_add (loopHook);
  }

  bool
  OneShotMods::isActive (void) {
    return (State && !hasTimedOut () && !shouldCancel);
  }

  void
  OneShotMods::cancel (bool withStickies) {
    shouldCancel = true;
    shouldCancelStickies = withStickies;
  }

  void
  OneShotMods::on (void) {
    event_handler_hook_replace (eventHandlerPassthroughHook, eventHandlerHook);
    loop_hook_replace (loopNoOpHook, loopHook);
  }

  void
  OneShotMods::off (void) {
    event_handler_hook_replace (eventHandlerHook, eventHandlerPassthroughHook);
    loop_hook_replace (loopHook, loopNoOpHook);
  }

  void
  OneShotMods::enableAuto (void) {
    event_handler_hook_add (eventHandlerAutoHook);
  }

  void
  OneShotMods::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

};

Akela::OneShotMods OneShotMods;
