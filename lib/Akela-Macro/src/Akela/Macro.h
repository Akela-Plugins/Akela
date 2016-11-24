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

#pragma once

#include <Akela.h>
#include <KeyboardioFirmware.h>

enum {
  M_FIRST = AKELA + 8,
  M_LAST  = AKELA + 8 + 31
};

#define M(n) {.raw = M_FIRST + n}

namespace Akela {
  class Macro {
  public:
    typedef void (*MacroHandler) (uint8_t macroIndex, uint8_t currentState, uint8_t previousState);

    Macro (MacroHandler handler);
    Macro (void);
  };
};
