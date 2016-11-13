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

#include <KeyboardioFirmware.h>
#include <Akela-MagicCombo.h>

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

void _print_example (uint32_t leftHand, uint32_t rightHand) {
  Serial.print ("It's a kind of magic!");
}

Akela::MagicCombo::dictionary_t dictionary[] = {
  {0, 0, _print_example}
};

Akela::MagicCombo exampleMagic(dictionary);

void setup() {
  Keyboardio.setup();
}

void loop() {
  Keyboardio.loop();
}
