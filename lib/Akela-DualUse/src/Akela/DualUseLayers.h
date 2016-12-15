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

#define LT(layer, key) (Key){ .raw = Akela::Ranges::DUL_FIRST + (layer << 8) + (Key_ ## key).rawKey }

namespace Akela {
  class DualUseLayers : public Plugin {
  public:
    DualUseLayers (void);

    static void configure (uint8_t offAction);

    void on (void);
    void off (void);

    void inject (Key key, uint8_t keyState);
  };
};

extern Akela::DualUseLayers DualUseLayers;
