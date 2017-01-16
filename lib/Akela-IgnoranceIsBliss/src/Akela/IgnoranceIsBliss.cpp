/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2017  Gergely Nagy
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

#include "IgnoranceIsBliss.h"

namespace Akela {

  uint32_t IgnoranceIsBliss::leftHandIgnores;
  uint32_t IgnoranceIsBliss::rightHandIgnores;

  IgnoranceIsBliss::IgnoranceIsBliss (void) {
  }

  void
  IgnoranceIsBliss::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  IgnoranceIsBliss::configure (uint32_t leftHandIgnores, uint32_t rightHandIgnores) {
    IgnoranceIsBliss::leftHandIgnores = leftHandIgnores;
    IgnoranceIsBliss::rightHandIgnores = rightHandIgnores;
  }

  Key
  IgnoranceIsBliss::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (row >= ROWS || col >= COLS)
      return mappedKey;

    uint32_t currentHand = leftHandIgnores;

    if (col >= 8) {
      col = col - 8;
      currentHand = rightHandIgnores;
    }

    if (bitRead (currentHand, row * COLS + col))
      return Key_NoKey;

    return mappedKey;
  }
};

Akela::IgnoranceIsBliss IgnoranceIsBliss;
