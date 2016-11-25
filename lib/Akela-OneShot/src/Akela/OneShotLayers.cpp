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

#include "OneShotLayers.h"

using namespace Akela::Ranges;

// ---- state ---------

static uint16_t Timer = 0;
static uint16_t TimeOut = DEFAULT_TIMEOUT;
static uint32_t State = 0;
static uint32_t stickyState = 0;
static Key prevKey;
static bool shouldCancel = false;

// --- helper macros ------

#define isOSL(key) (key.raw >= OSL_FIRST && key.raw <= OSL_LAST)
#define isLayerKey(key) (key.flags & (KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP) && key.rawKey >= MOMENTARY_OFFSET)

#define isOneShot(idx) (bitRead (State, idx))
#define setOneShot(idx) (bitWrite (State, idx, 1))
#define clearOneShot(idx) (bitWrite (State, idx, 0))

#define isSticky(idx) (bitRead (stickyState, idx))
#define setSticky(idx) (bitWrite (stickyState, idx, 1))
#define clearSticky(idx) bitWrite (stickyState, idx, 0)

#define isSameAsPrevious(key) (key.raw == prevKey.raw)
#define saveAsPrevious(key) prevKey.raw = key.raw

#define Key_Keymap0_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_0 + MOMENTARY_OFFSET}
#define toNormalKey(key, idx) key.flags = KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP; key.rawKey = KEYMAP_0 + MOMENTARY_OFFSET + idx
#define hasTimedOut() (Timer > TimeOut)

// ----- passthrough ------

static bool
eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!isOSL (mappedKey))
    return false;

  uint8_t layer = mappedKey.rawKey - OSL_FIRST;
  toNormalKey (mappedKey, layer);

  handle_key_event (mappedKey, row, col, keyState | INJECTED);

  return true;
}

static void
loopNoOpHook (void) {
}

// ---- OneShot stuff ----

static bool
shouldInterrupt (Key mappedKey) {
  return !isOSL (mappedKey);
}

static bool
eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!isLayerKey (mappedKey))
    return false;

  // If mappedKey is an injected key, we don't fiddle with those.
  if (keyState & INJECTED)
    return false;

  uint8_t layer = mappedKey.rawKey - OSL_FIRST;
  Key newKey = OSL(layer);

  handle_key_event (newKey, row, col, keyState | INJECTED);
  return true;
}

static bool
eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (State && shouldInterrupt (mappedKey))
    Akela::OneShotLayers::cancel ();

  if (!isOSL (mappedKey))
    return false;

  // Released?
  if (!key_is_pressed (keyState)) {
    if (hasTimedOut ())
      Akela::OneShotLayers::cancel ();

    return true;
  }

  uint8_t idx = mappedKey.rawKey - OSL_FIRST;

  if (isSticky(idx)) {
    clearSticky (idx);
    Akela::OneShotLayers::cancel ();
    return true;
  } else if (isSameAsPrevious (mappedKey)) {
    setSticky (idx);
  }

  saveAsPrevious (mappedKey);
  toNormalKey (mappedKey, idx);
  setOneShot (idx);

  Layer.on (idx);

  return true;
}

static void
loopHook (void) {
  if (!State)
    return;

  Timer++;

  if (hasTimedOut ())
    Akela::OneShotLayers::cancel ();

  for (uint8_t i = 0; i < 8; i++) {
    if (!isOneShot (i))
      continue;

    if (shouldCancel && !isSticky (i)) {
      clearOneShot (i);
      Layer.off (i);
    }
  }

  if (shouldCancel) {
    Timer = 0;
    shouldCancel = false;
  }
}

// --- glue code ---

namespace Akela {
  OneShotLayers::OneShotLayers (uint8_t defaultState) {
    if (defaultState == Default::Off) {
      event_handler_hook_add (eventHandlerPassthroughHook);
      loop_hook_add (loopNoOpHook);
    } else {
      event_handler_hook_add (eventHandlerHook);
      loop_hook_add (loopHook);
    }
  }

  bool
  OneShotLayers::isActive (void) {
    return (State && !hasTimedOut () && !shouldCancel);
  }

  void
  OneShotLayers::cancel (void) {
    shouldCancel = true;
  }

  void
  OneShotLayers::on (void) {
    event_handler_hook_replace (eventHandlerPassthroughHook, eventHandlerHook);
    loop_hook_replace (loopNoOpHook, loopHook);
  }

  void
  OneShotLayers::off (void) {
    event_handler_hook_replace (eventHandlerHook, eventHandlerPassthroughHook);
    loop_hook_replace (loopHook, loopNoOpHook);
  }

  void
  OneShotLayers::enableAuto (void) {
    event_handler_hook_add (eventHandlerAutoHook);
  }
};
