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

#include "TapDance.h"

using namespace Akela::Ranges;

namespace Akela {
  // --- state ---
  uint16_t TapDance::tapDanceTimer;
  uint16_t TapDance::tapDanceTimeOut = DEFAULT_TIMEOUT;
  uint8_t TapDance::tapDanceCount[32];
  uint32_t TapDance::tapDancePressedState;
  Key TapDance::lastTapDanceKey;

  // --- helpers ---

#define isTapDance(k) (k.raw >= TD_FIRST && k.raw <= TD_LAST)
#define isInSeq(k) (lastTapDanceKey.raw == k.raw)
#define stillHeld(idx) (tapDanceCount[idx])
#define isActive() (lastTapDanceKey.raw != Key_NoKey.raw)

  // --- actions ---

  void
  TapDance::interrupt (void) {
    uint8_t idx = lastTapDanceKey.raw - TD_FIRST;
    tapDanceAction (idx, tapDanceCount[idx], Interrupt);
    lastTapDanceKey.raw = Key_NoKey.raw;
  }

  void
  TapDance::timeOut (void) {
    uint8_t idx = lastTapDanceKey.raw - TD_FIRST;
    tapDanceAction (idx, tapDanceCount[idx], Timeout);
    lastTapDanceKey.raw = Key_NoKey.raw;
  }

  Key
  TapDance::release (uint8_t tapDanceIndex) {
    tapDanceAction (tapDanceIndex, tapDanceCount[tapDanceIndex], Release);
    tapDanceCount[tapDanceIndex] = 0;
    bitClear (tapDancePressedState, tapDanceIndex);
    return Key_NoKey;
  }

  Key
  TapDance::tap (void) {
    uint8_t idx = lastTapDanceKey.raw - TD_FIRST;

    tapDanceCount[idx]++;
    tapDanceTimer = 0;

    tapDanceAction (idx, tapDanceCount[idx], Tap);

    return Key_NoKey;
  }

  // --- api ---

  TapDance::TapDance (void) {
    lastTapDanceKey.raw = Key_NoKey.raw;
  }

  void
  TapDance::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
    loop_hook_add (this->loopHook);
  }

  void
  TapDance::actionKeys (uint8_t tapCount, ActionType tapDanceAction, uint8_t maxKeys, const Key tapKeys[]) {
    if (tapCount >= maxKeys)
      return;

    Key key;
    key.raw = pgm_read_word (tapKeys + tapCount - 1);

    switch (tapDanceAction) {
    case Tap:
      break;
    case Interrupt:
    case Timeout:
      handle_key_event (key, 255, 255, IS_PRESSED | INJECTED);
      break;
    case Hold:
      handle_key_event (key, 255, 255, IS_PRESSED | WAS_PRESSED | INJECTED);
      break;
    case Release:
      handle_key_event (key, 255, 255, WAS_PRESSED | INJECTED);
      break;
    }
  }

  void
  TapDance::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

  // --- hooks ---

  Key
  TapDance::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & INJECTED)
      return mappedKey;

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      if (isTapDance (mappedKey))
        return Key_NoKey;
      return mappedKey;
    }

    if (!isTapDance (mappedKey)) {
      if (!isActive ())
        return mappedKey;

      if (key_toggled_on (keyState))
        interrupt ();

      return mappedKey;
    }

    uint8_t tapDanceIndex = mappedKey.raw - TD_FIRST;

    if (key_toggled_off (keyState))
      bitClear (tapDancePressedState, tapDanceIndex);

    if (!isInSeq (mappedKey)) {
      if (!isActive ()) {
        lastTapDanceKey.raw = mappedKey.raw;
        return tap ();
      } else {
        if (key_toggled_off (keyState) && stillHeld (tapDanceIndex)) {
          return release (tapDanceIndex);
        }

        if (!key_toggled_on (keyState))
          return Key_NoKey;

        interrupt ();
      }
    }

    // in sequence

    if (key_toggled_off (keyState))
      return Key_NoKey;

    lastTapDanceKey.raw = mappedKey.raw;
    bitSet (tapDancePressedState, tapDanceIndex);

    if (key_toggled_on (keyState))
      return tap ();

    tapDanceAction (tapDanceIndex, tapDanceCount[tapDanceIndex], Hold);
    return Key_NoKey;
  }

  void
  TapDance::loopHook (bool postClear) {
    if (!isActive ())
      return;

    if (tapDanceTimer < tapDanceTimeOut)
      tapDanceTimer++;

    if (tapDanceTimer >= tapDanceTimeOut)
      timeOut();
  }
};

__attribute__((weak))
void
tapDanceAction (uint8_t tapDanceIndex, uint8_t tapCount, Akela::TapDance::ActionType tapDanceAction) {
}

Akela::TapDance TapDance;
