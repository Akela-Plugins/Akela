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

#define OSL(n) (Key) {.raw = Akela::Ranges::OSL_FIRST + n}

namespace Akela {
  class OneShotLayers : public KeyboardioPlugin {
  public:
    OneShotLayers (void);

    virtual void begin (void) final;

    static void on (void);
    static void off (void);
    static void enableAuto (void);
    static bool isActive (void);
    static void cancel (bool withStickies);
    static void cancel (void) { cancel (false); };

    void inject (Key key, uint8_t keyState);

  private:
    static Key eventHandlerPassthroughHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopNoOpHook (bool postClear);

    static Key eventHandlerAutoHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    static void loopHook (bool postClear);
  };
};

extern Akela::OneShotLayers OneShotLayers;
