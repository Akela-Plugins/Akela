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

#include <Akela-MagicCombo.h>

namespace Akela {

  static MagicCombo::dictionary_t *_magicDictionary = NULL;

  MagicCombo::MagicCombo (dictionary_t dictionary[]) {
    _magicDictionary = (dictionary_t *)dictionary;
  }

  bool
  MagicCombo::comboHandler (byte row, byte col, uint8_t currentState, uint8_t previousState) {
    for (byte i = 0; _magicDictionary[i].callback != NULL; i++) {
      dictionary_t combo = _magicDictionary[i];
      if (KeyboardHardware.leftHandState.all == combo.leftHand &&
          KeyboardHardware.rightHandState.all == combo.rightHand) {
        (*combo.callback) (combo.leftHand, combo.rightHand);
        return true;
      }
    }

    return false;
  }

};
