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
#include <Akela-OneShot.h>

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   Key_LEDEffectNext, Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick,      Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,        Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDn,        Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Esc,

   OSM(LCtrl), Key_Backspace, OSM(LGUI), Key_LShift,
   Key_KeymapNext_Momentary,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_Keymap2,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RShift, OSM(RAlt), Key_Space, OSM(RCtrl),
   Key_KeymapNext_Momentary
   ),
};

Akela::OneShotMods oneShotMods (Akela::Default::On);

void setup() {
  oneShotMods.enableAuto();
  Keyboardio.setup(1);
}
void loop() {
  Keyboardio.loop();
}
