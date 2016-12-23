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

#include "Cycle.h"

using namespace Akela::Ranges;

namespace Akela {
  // --- state ---
  static Key lastNonCycleKey;
  static uint8_t cycleCount;

  // --- helpers ---

#define isCycle(k) (k.raw == CYCLE)

  // --- api ---

  Cycle::Cycle (void) {
    lastNonCycleKey.raw = Key_NoKey.raw;
    cycleCount = 0;

    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  Cycle::replace (Key key) {
    handle_key_event (Key_Backspace, 255, 255, IS_PRESSED | INJECTED);
    Keyboard.sendReport ();
    handle_key_event (Key_Backspace, 255, 255, WAS_PRESSED | INJECTED);
    Keyboard.sendReport ();

    handle_key_event (key, 255, 255, IS_PRESSED | INJECTED);
    Keyboard.sendReport ();
    handle_key_event (key, 255, 255, WAS_PRESSED | INJECTED);
    Keyboard.sendReport ();
  }

  void
  Cycle::replace (uint8_t cycleSize, const Key cycleSteps[]) {
    uint8_t idx = cycleCount % cycleSize;
    Key key;

    key.raw = pgm_read_word (cycleSteps + idx);
    replace (key);
  }

  // --- hooks ---

  bool
  Cycle::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState))
      return isCycle (mappedKey);

    if (!isCycle (mappedKey)) {
      if (key_toggled_on (keyState)) {
        lastNonCycleKey.raw = mappedKey.raw;
        cycleCount = 0;
      }
      return false;
    }

    if (!key_toggled_off (keyState))
      return true;

    cycleCount++;
    cycleAction (lastNonCycleKey, cycleCount);
    return true;
  }
};

__attribute__((weak))
void
cycleAction (Key previousKey, uint8_t cycleCount) {
}

Akela::Cycle Cycle;
