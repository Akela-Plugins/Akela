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

#include <KeyboardioFirmware.h>
#include <Akela-Macro.h>

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = {
    {M(0), Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext, Key_LCtrl, Key_RCtrl, Key_skip, Key_6, Key_7, Key_8, Key_9, Key_0, Key_Keymap2},
    {Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab, Key_Backspace, Key_Space, Key_Enter, Key_Y, Key_U, Key_I, Key_O, Key_P, Key_Equals},
    {Key_PageUp, Key_A, Key_S, Key_D, Key_F, Key_G, Key_Esc, Key_LGUI, Key_RAlt, Key_skip, Key_H, Key_J, Key_K, Key_L, Key_Semicolon, Key_Quote},
    {Key_PageDn, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_KeymapNext_Momentary, Key_LShift, Key_RShift, Key_KeymapNext_Momentary, Key_N, Key_M, Key_Comma, Key_Period, Key_Slash, Key_Minus},
  },
};

namespace Akela {
  void macroAction (uint8_t macroIndex, uint8_t currentState, uint8_t previousState) {
    switch (macroIndex) {
    case 0:
      if (key_toggled_on (currentState, previousState)) {
        handle_key_event (Key_A, 0, 0, 1, 0);
        Keyboard.sendReport ();
        handle_key_event (Key_B, 0, 0, 1, 0);
      }
    }
  }
};

Akela::Macro macros;

void setup() {
  Keyboardio.setup(1, 1);
}

void loop() {
  Keyboardio.loop();
}
