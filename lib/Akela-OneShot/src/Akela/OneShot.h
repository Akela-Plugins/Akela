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

#define AKELA B01000000

enum {
  OSM_FIRST = 0,
  OSM_LAST  = 7
};

#define _OSM_HELPER(kc) Key_ ## kc
#define OSM(kc) (Key) {SYNTHETIC | AKELA, OSM_FIRST + _OSM_HELPER(kc).rawKey}

namespace Akela {
  namespace Default {
    enum {
      Off,
      On
    };
  };

  class OneShotMods {
  public:
    OneShotMods (uint8_t defaultState);
    OneShotMods (void) : OneShotMods (Default::On) {};

    static void on (void);
    static void off (void);
    static bool isActive (void);
    static void cancel (void);
  };
};
