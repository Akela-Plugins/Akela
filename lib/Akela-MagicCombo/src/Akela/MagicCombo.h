/* -*- mode: c++ -*-
 * Akela -- Animated Keyboard Extension Library for Arduino
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

#pragma once

#include <KeyboardioFirmware.h>

namespace Akela {
  class MagicCombo {
  public:
    typedef void (*comboCallback_t) (uint32_t leftHand, uint32_t rightHand);
    typedef struct {
      uint32_t leftHand, rightHand;
      comboCallback_t callback;
    } dictionary_t;
    MagicCombo (dictionary_t dictionary[]);

    static bool comboHandler (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState);
  };
};
