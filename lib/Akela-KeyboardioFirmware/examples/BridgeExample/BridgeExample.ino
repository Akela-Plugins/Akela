// -*- mode: c++ -*-
/* -*- mode: c++ -*-
 * Akela -- Animated Keyboard Extension Library for Arduino
 * Copyright (C) 2016  Gergely Nagy
 * Copyright (C) Keyboardio, inc.
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

#define DEBUG_SERIAL false

#include <Akela-KeyboardioFirmware.h>

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

static Akela::Keyboardio::Keyboard kboard;

bool handle_user_key_event(byte row, byte col, uint8_t currentState, uint8_t previousState) {
  Key mappedKey = lookup_key (temporary_keymap, row, col);

  if (kboard.register_code (mappedKey))
    return true;

  return false;
}

void setup() {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    Mouse.begin();
    AbsoluteMouse.begin();
    KeyboardHardware.setup();
    LEDControl.boot_animation();

    temporary_keymap = primary_keymap = Storage.load_primary_keymap(KEYMAPS);
}


void loop() {
    kboard.loop ();
    KeyboardHardware.scan_matrix();
    LEDControl.update(temporary_keymap);
    Keyboard.sendReport();
    Keyboard.releaseAll();
}
