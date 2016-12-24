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

#define _MT_MOD(mod) (((Key_ ## mod).rawKey - Key_LCtrl.rawKey) << 8)
#define MT(mod, key) (Key){ .raw = Akela::Ranges::DUM_FIRST + _MT_MOD(mod) + (Key_ ## key).rawKey }
#define SFT_T(key) MT(LShift, key)
#define CTL_T(key) MT(LCtrl, key)
#define ALT_T(key) MT(LAlt, key)
#define GUI_T(key) MT(LGUI, key)

namespace Akela {
  class DualUseMods : public KeyboardioPlugin {
  public:
    DualUseMods (void);

    virtual void begin (void) final;

    static void configure (uint8_t offAction);

    void on (void);
    void off (void);

    void inject (Key key, uint8_t keyState);

  private:
    static bool eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static bool disabledHook (Key, byte row, byte col, uint8_t keyState);
  };
};

extern Akela::DualUseMods DualUseMods;
