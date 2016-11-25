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

#include <KeyboardioFirmware.h>
#include <Akela.h>

#define _MT_KEY(key) Key_ ## key
#define _MT_MOD(mod) ((_MT_KEY(mod).rawKey - Key_LCtrl.rawKey) << 8)
#define MT(mod, key) (Key){ .raw = Akela::Ranges::DUM_FIRST + _MT_MOD(mod) + _MT_KEY(key).rawKey }
#define SFT_T(key) MT(LShift, key)
#define CTL_T(key) MT(LCtrl, key)
#define ALT_T(key) MT(LAlt, key)
#define GUI_T(key) MT(LGUI, key)

namespace Akela {
  class DualUseMods {
  public:
    DualUseMods (uint8_t defaultMode, uint8_t defaultAction);
    DualUseMods (uint8_t defaultAction) : DualUseMods (Akela::Default::Off, defaultAction) {};
    DualUseMods (void) : DualUseMods (Akela::Default::On, 1) {};

    void on (void);
    void off (void);

  private:
    static bool eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static bool disabledHook (Key, byte row, byte col, uint8_t keyState);
  };
};
