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

#include "Heatmap.h"

namespace Akela {
  static uint8_t heatmap[ROWS][COLS];
  static uint16_t totalKeys;
  static uint8_t highestCount;
  static uint16_t updateFrequency;
  static uint16_t loopCount;

  static const float heatColors[][3] = {{0.3, 0.3, 0.3}, {0.3, 1, 0.3}, {1, 1, 0.3}, {1, 0.3, 0.3}};

  static void
  shiftStats (void) {
    highestCount = totalKeys = 0;
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        heatmap[r][c] = heatmap[r][c] >> 1;
        totalKeys += heatmap[r][c];
        if (heatmap[r][c] > highestCount)
          highestCount = heatmap[r][c];
      }
    }
  }

  static cRGB
  computeColor (float v) {
    float fb = 0;
    uint8_t idx1, idx2;

    if (v <= 0) {
      idx1 = idx2 = 0;
    } else if (v >= 1) {
      idx1 = idx2 = 3;
    } else {
      float val = v * 3;
      idx1 = int(floor(val));
      idx2 = idx1 + 1;
      fb = val - float(idx1);
    }

    uint8_t r = (int)(((heatColors[idx2][0] - heatColors[idx1][0]) * fb + heatColors[idx1][0]) * 255);
    uint8_t g = (int)(((heatColors[idx2][1] - heatColors[idx1][1]) * fb + heatColors[idx1][1]) * 255);
    uint8_t b = (int)(((heatColors[idx2][2] - heatColors[idx1][2]) * fb + heatColors[idx1][2]) * 255);

    return {r, g, b};
  }

  Heatmap::Heatmap (uint16_t updateFreq) {
    LEDControl.mode_add (this);
    event_handler_hook_add (this->eventHook);
    loop_hook_add (this->loopHook);

    updateFrequency = updateFreq;
  }

  bool
  Heatmap::eventHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    // if it is a synthetic key, skip it.
    if (mappedKey.raw == Key_NoKey.raw && !row && !col)
      return false;

    // if the key is not pressed, return.
    if (key_is_pressed (currentState, previousState))
      return false;

    totalKeys++;
    heatmap[row][col]++;
    if (heatmap[row][col] > highestCount)
      highestCount = heatmap[row][col];

    return false;
  }

  void
  Heatmap::loopHook (void) {
    if (highestCount > 191 || totalKeys > 16000)
      shiftStats();
  }

  void
  Heatmap::update (void) {
    loopCount++;

    if (loopCount < updateFrequency)
      return;

    loopCount = 0;
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        uint8_t cap = totalKeys || 1;
        float v = float(heatmap[r][c]) / cap;
        led_set_crgb_at (r, c, computeColor (v));
      }
    }
  }
};
