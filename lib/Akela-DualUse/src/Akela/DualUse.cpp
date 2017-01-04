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

#include "DualUse.h"

using namespace Akela::Ranges;

namespace Akela {
  uint32_t DualUse::keyActionNeededMap;
  uint32_t DualUse::pressedMap;
  uint8_t DualUse::timer[32];

  bool DualUse::specDefault;

  // ---- helpers ----
  Key
  DualUse::specialAction (uint8_t specIndex) {
    Key newKey;

    newKey.flags = KEY_FLAGS;
    if (specIndex < 8) {
      newKey.rawKey = Key_LCtrl.rawKey + specIndex;
    } else {
      uint8_t target = specIndex - 8;

      Layer.on (target);

      newKey.rawKey = 0;
    }

    return newKey;
  }

  void
  DualUse::pressAllSpecials (byte row, byte col) {
    for (uint8_t specIndex = 0; specIndex < 32; specIndex++) {
      if (!bitRead (pressedMap, specIndex))
        continue;

      Key newKey = specialAction (specIndex);
      if (newKey.raw != Key_NoKey.raw)
        handle_key_event (newKey, row, col, IS_PRESSED | INJECTED);
    }
  }

  // ---- API ----

  DualUse::DualUse (void) {
  }

  void
  DualUse::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  DualUse::configure (uint8_t offAction) {
    specDefault = !!offAction;
  }

  void
  DualUse::on (void) {
    event_handler_hook_replace (this->disabledHook, this->eventHandlerHook);
  }

  void
  DualUse::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->disabledHook);
  }

  void
  DualUse::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

  // ---- Handlers ----

  Key
  DualUse::disabledHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.raw < DU_FIRST || mappedKey.raw > DU_LAST)
      return mappedKey;

    Key newKey;

    uint8_t specIndex = (mappedKey.raw - DU_FIRST) >> 8;

    if (specDefault) {
      newKey = specialAction (specIndex);
    } else {
      newKey.flags = KEY_FLAGS;
      newKey.rawKey = mappedKey.raw - DU_FIRST - (specIndex << 8);
    }

    return newKey;
  }

  Key
  DualUse::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & INJECTED)
      return mappedKey;

    // If nothing happened, bail out fast.
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      if (mappedKey.raw < DU_FIRST || mappedKey.raw > DU_LAST)
        return mappedKey;
      return Key_NoKey;
    }

    if (mappedKey.raw >= DU_FIRST && mappedKey.raw <= DU_LAST) {
      uint8_t specIndex = (mappedKey.raw - DU_FIRST) >> 8;
      Key newKey = Key_NoKey;

      if (key_toggled_on (keyState)) {
        bitWrite (pressedMap, specIndex, 1);
        bitWrite (keyActionNeededMap, specIndex, 1);
        timer[specIndex] = 0;
      } else if (key_is_pressed (keyState)) {
        if (timer[specIndex] < timeOut)
          timer[specIndex]++;

        if (timer[specIndex] >= timeOut) {
          newKey = specialAction (specIndex);
        }
      } else if (key_toggled_off (keyState)) {
        if (timer[specIndex] < timeOut)
          timer[specIndex]++;

        if ((timer[specIndex] <= timeOut) && bitRead (keyActionNeededMap, specIndex)) {
          uint8_t m = mappedKey.raw - DU_FIRST - (specIndex << 8);
          if (specIndex >= 8)
            m--;

          Key newKey = { KEY_FLAGS, m };

          handle_key_event (newKey, row, col, IS_PRESSED | INJECTED);
          Keyboard.sendReport ();
        } else {
          if (specIndex >= 8) {
            uint8_t target = specIndex - 8;

            Layer.off (target);
          }
        }

        bitWrite (pressedMap, specIndex, 0);
        bitWrite (keyActionNeededMap, specIndex, 0);
        timer[specIndex] = 0;
      }

      return newKey;
    }

    if (pressedMap == 0) {
      return mappedKey;
    }

    pressAllSpecials (row, col);
    keyActionNeededMap = 0;

    if (pressedMap > (1 << 7)) {
      mappedKey = Layer.lookup (row, col);
    }

    return mappedKey;
  }

};

Akela::DualUse DualUse;
