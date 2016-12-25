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

#include <Akela-MagicCombo.h>

namespace Akela {

  static const MagicCombo::dictionary_t *_magicDictionary = NULL;

  MagicCombo::MagicCombo (void) {
  }

  void
  MagicCombo::begin (void) {
    event_handler_hook_add (this->comboHandler);
  }

  void
  MagicCombo::configure (const MagicCombo::dictionary_t dictionary[]) {
    _magicDictionary = (dictionary_t *)dictionary;
  }

  Key
  MagicCombo::comboHandler (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!_magicDictionary)
      return mappedKey;

    for (byte i = 0;; i++) {
      dictionary_t combo;

      combo.leftHand = pgm_read_dword (&(_magicDictionary[i].leftHand));
      combo.rightHand = pgm_read_dword (&(_magicDictionary[i].rightHand));

      if (combo.leftHand == 0 && combo.rightHand == 0)
        break;

      if (KeyboardHardware.leftHandState.all == combo.leftHand &&
          KeyboardHardware.rightHandState.all == combo.rightHand) {
        magicComboActions (i, combo.leftHand, combo.rightHand);
        break;
      }
    }

    return mappedKey;
  }

};

__attribute__((weak))
void
magicComboActions (uint8_t comboIndex, uint32_t leftHand, uint32_t rightHand) {
}

Akela::MagicCombo MagicCombo;
