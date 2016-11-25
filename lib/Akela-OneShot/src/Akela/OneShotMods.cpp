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

// ---- state ---------

static uint16_t Timer = 0;
static uint16_t TimeOut = DEFAULT_TIMEOUT;
static uint8_t State = 0;
static uint8_t stickyState = 0;
static Key prevKey;
static bool shouldCancel = false;

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

#define toNormalKey(key) key.flags = 0; key.rawKey += Key_LCtrl.rawKey
#define hasTimedOut() (Timer > TimeOut)

// ----- passthrough ------

bool
eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!isOSM (mappedKey))
    return false;

  toNormalKey (mappedKey);

  handle_key_event (mappedKey, row, col, keyState | INJECTED);

  return true;
}

void
loopNoOpHook (void) {
}

// ---- OneShot stuff ----

static bool
shouldInterrupt (Key mappedKey) {
  return !isOSM (mappedKey);
}

static bool
eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!isModifier (mappedKey))
    return false;

  // If mappedKey is an injected key, we don't fiddle with those.
  if (keyState & INJECTED)
    return false;

  Key newKey;
  newKey.raw = OSM_FIRST + newKey.rawKey - Key_LCtrl.rawKey;

  handle_key_event (newKey, row, col, keyState | INJECTED);
  return true;
}

static bool
eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (State && shouldInterrupt (mappedKey))
    Akela::OneShotMods::cancel ();

  if (!isOSM (mappedKey))
    return false;

  // Released?
  if (!key_is_pressed (keyState)) {
    if (hasTimedOut ())
      Akela::OneShotMods::cancel ();

    return true;
  }

  uint8_t idx = mappedKey.rawKey;

  if (isSticky(idx)) {
    clearSticky (idx);
    Akela::OneShotMods::cancel ();
    return true;
  } else if (isSameAsPrevious (mappedKey)) {
    setSticky (idx);
  }

  saveAsPrevious (mappedKey);
  toNormalKey (mappedKey);
  setOneShot (idx);

  handle_key_event (mappedKey, row, col, keyState | INJECTED);
  return true;
}

static void
loopHook (void) {
  if (!State)
    return;

  Timer++;

  if (hasTimedOut ())
    Akela::OneShotMods::cancel ();

  for (uint8_t i = 0; i < 8; i++) {
    if (!isOneShot (i))
      continue;

    if (shouldCancel && !isSticky (i)) {
      clearOneShot (i);
    } else {
      uint8_t m = Key_LCtrl.rawKey + i;
      handle_key_event ({0, m}, 0, 0, IS_PRESSED | INJECTED);
    }
  }

  if (shouldCancel) {
    Timer = 0;
    shouldCancel = false;
  }
}

// --- glue code ---

namespace Akela {
  OneShotMods::OneShotMods (uint8_t defaultState) {
    if (defaultState == Default::Off) {
      event_handler_hook_add (eventHandlerPassthroughHook);
      loop_hook_add (loopNoOpHook);
    } else {
      event_handler_hook_add (eventHandlerHook);
      loop_hook_add (loopHook);
    }
  }

  bool
  OneShotMods::isActive (void) {
    return (State && !hasTimedOut () && !shouldCancel);
  }

  void
  OneShotMods::cancel (void) {
    shouldCancel = true;
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
};
