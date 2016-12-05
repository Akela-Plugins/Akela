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

#define _OSM_HELPER(kc) Key_ ## kc
#define OSM(kc) (Key) {.raw = Akela::Ranges::OSM_FIRST + _OSM_HELPER(kc).rawKey - Key_LCtrl.rawKey}

namespace Akela {
  class OneShotMods {
  public:
    OneShotMods (void);

    static void on (void);
    static void off (void);
    static void enableAuto (void);
    static bool isActive (void);
    static void cancel (void);

  private:
    static bool eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopNoOpHook (void);

    static bool eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static bool eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (void);
  };
};
