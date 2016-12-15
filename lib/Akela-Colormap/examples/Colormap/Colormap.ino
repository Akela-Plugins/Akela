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

#include <Akela.h>
#include <Akela-Colormap.h>

enum {
  OFF,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE
};

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   Key_LEDEffectNext, Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick,      Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,        Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDn,        Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Esc,

   Key_LCtrl, Key_Backspace, Key_LGUI, Key_LShift,
   Key_KeymapNext_Momentary,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_Keymap2,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RShift, Key_RAlt, Key_Space, Key_RCtrl,
   Key_KeymapNext_Momentary
  ),
};

const uint8_t colormap[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   YELLOW, GREEN, GREEN, GREEN, GREEN, GREEN, YELLOW,
   CYAN,   BLUE,  BLUE,  BLUE,  BLUE,  BLUE,  WHITE,
   CYAN,   BLUE,  BLUE,  BLUE,  BLUE,  BLUE,
   CYAN,   BLUE,  BLUE,  BLUE,  BLUE,  BLUE,  WHITE,

   YELLOW, WHITE, YELLOW, YELLOW,
   RED,

   OFF,    GREEN, GREEN, GREEN, GREEN, GREEN, OFF,
   WHITE,  BLUE,  BLUE,  BLUE,  BLUE,  BLUE,  CYAN,
           BLUE,  BLUE,  BLUE,  BLUE,  CYAN,  CYAN,
   OFF,    BLUE,  BLUE,  CYAN,  CYAN,  CYAN,  CYAN,

   YELLOW, YELLOW, WHITE, YELLOW,
   RED
   )
};

const cRGB colors[] PROGMEM = {
  {0, 0, 0},
  {0xff, 0x5c, 0x57},
  {0x5a, 0xf7, 0x8e},
  {0xf3, 0xf9, 0x9d},
  {0x57, 0xc7, 0xff},
  {0xff, 0x6a, 0xc1},
  {0x9a, 0xed, 0xfe},
  {0xf1, 0xf1, 0xf0},
};

void setup () {
  Akela::USE (ColormapEffect);

  Keyboardio.setup (KEYMAP_SIZE);
  ColormapEffect.configure (colors, colormap);
  ColormapEffect.activate ();
}

void loop () {
  Keyboardio.loop ();
}
