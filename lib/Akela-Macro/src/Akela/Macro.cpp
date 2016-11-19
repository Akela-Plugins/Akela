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

#include "Macro.h"

static Akela::Macro::MacroHandler _macroAction;

#define isMacro(key) (key.raw >= (SYNTHETIC | AKELA | M_FIRST) && key.raw <= (SYNTHETIC | AKELA | M_LAST))

bool
_macroHandler (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
  if (mappedKey.raw == Key_NoKey.raw) {
    mappedKey = lookup_key(temporary_keymap, row, col);
  }

  if (!isMacro (mappedKey))
    return false;

  uint8_t idx = mappedKey.raw - M_FIRST;

  (*_macroAction) (idx, currentState, previousState);

  return true;
}

namespace Akela {
  __attribute__((weak))
  void
  macroAction (uint8_t macroIndex, uint8_t currentState, uint8_t previousState) {
  }

  Macro::Macro (Macro::MacroHandler handler) {
    _macroAction = handler;
    event_handler_hook_add (_macroHandler);
  }

  Macro::Macro (void) : Macro (macroAction) {
  }
};
