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

#include "Colormap.h"

#include <avr/pgmspace.h>

namespace Akela {

  static const cRGB *palette;
  static const uint8_t *colorMap;
  static uint32_t previousLayerState;

  ColormapEffect::ColormapEffect (void) {
  }

  void
  ColormapEffect::configure (const cRGB _palette[], const uint8_t _colorMap[][ROWS][COLS]) {
    palette = _palette;
    colorMap = (const uint8_t *)_colorMap;
  }

  void
  ColormapEffect::update (void) {
    uint8_t layerState = Layer.getLayerState ();
    if (previousLayerState == layerState)
      return;
    previousLayerState = layerState;

    for (uint8_t l = 0; l < 32; l++) {
      if (!Layer.isOn (l))
        continue;

      for (uint8_t r = 0; r < ROWS; r++) {
        for (uint8_t c = 0; c < COLS; c++) {
          uint8_t colorIndex;
          cRGB color;

          uint16_t mapIndex = (l * ROWS * COLS + r * COLS + c);
          colorIndex = pgm_read_byte (&(colorMap[mapIndex]));

          if (colorIndex == Transparent)
            continue;

          color.b = pgm_read_byte (&(palette[colorIndex].b));
          color.g = pgm_read_byte (&(palette[colorIndex].g));
          color.r = pgm_read_byte (&(palette[colorIndex].r));

          led_set_crgb_at (r, c, color);
        }
      }
    }
  }

};

Akela::ColormapEffect ColormapEffect;
