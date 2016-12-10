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

#define LEADER_MAX_SEQUENCE_LENGTH 16

#define LEAD(n) (Key){ .raw = Akela::Ranges::LEAD_FIRST + n }

#define LEADER_SEQ(...) { __VA_ARGS__, Key_NoKey }
#define LEADER_DICT(...) { __VA_ARGS__, {{Key_NoKey}, NULL} }

namespace Akela {
  class Leader {
  public:
    typedef void (*action_t) (uint8_t seqIndex);
    typedef struct {
      Key sequence[LEADER_MAX_SEQUENCE_LENGTH + 1];
      action_t action;
    } dictionary_t;

    Leader (void);

    static void configure (const dictionary_t dictionary[]);
    static void reset (void);

  private:
    static bool eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (void);
    static int8_t lookup (void);
  };
};

static Akela::Leader Leader;
