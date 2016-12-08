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

#include "Miami.h"

#define CYAN (cRGB){0x4e, 0xd6, 0xd6}
#define MAGENTA (cRGB){0xfa, 0x67, 0xaf}

namespace Akela {
  namespace LEDEffects {
    Miami::Miami (void) {
      LEDControl.mode_add (this);
    }

    void
    Miami::update (void) {
      for (uint8_t r = 0; r < ROWS; r++) {
        for (uint8_t c = 0; c < COLS; c++) {
          Key k = Layer.lookup (r, c);

          // Special keys are always MAGENTA
          if (k.flags != 0) {
            led_set_crgb_at (r, c, MAGENTA);
            continue;
          }

          cRGB color = MAGENTA;

          switch (k.rawKey) {
          case Key_A.rawKey ... Key_0.rawKey:
          case Key_Space.rawKey:
          case Key_Minus.rawKey ... Key_Slash.rawKey:
          case Key_Keypad1.rawKey ... Key_KeypadDot.rawKey:
          case Key_F1.rawKey ... Key_F4.rawKey:
          case Key_F9.rawKey ... Key_F12.rawKey:
            color = CYAN;
            break;
          }

          led_set_crgb_at (r, c, color);
        }
      }
    }

    void
    Miami::enable (void) {
    }
  };
};

Akela::LEDEffects::Miami MiamiEffect;
