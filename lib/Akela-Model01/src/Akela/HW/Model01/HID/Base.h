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

#include "Akela.h"

namespace M01 {
  namespace HID {
    class Skeleton {
      virtual void press (Page page, uint8_t code) = 0;
      virtual void release (Page page, uint8_t code) = 0;
    };

    class Base : public Akela::AbstractHID,
                 public Skeleton,
                 public AbstractMouseControl {
    public:
      Base () {};

      virtual void press (uint8_t code);
      virtual void release (uint8_t code);

      virtual void press (Page page, uint8_t code);
      virtual void release (Page page, uint8_t code);

      virtual void setup ();
      virtual void loop ();

      virtual void move (int8_t, int8_t) {};
      virtual void warp (uint8_t) {};

      virtual void sendReport ();
      virtual bool isModifierActive (uint8_t code);
    };
  };
};
