/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2016, 2017  Gergely Nagy
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
  static uint16_t _magicComboTimeOut;
  static uint16_t _magicComboTimer;

  MagicCombo::MagicCombo (void) {
  }

  void
  MagicCombo::begin (void) {
    loop_hook_add (this->loopHook);
  }

  void
  MagicCombo::configure (const MagicCombo::dictionary_t dictionary[], uint16_t timeOut) {
    _magicDictionary = (dictionary_t *)dictionary;
    _magicComboTimeOut = timeOut;
  }

  void
  MagicCombo::loopHook (bool postClear) {
    if (!_magicDictionary || postClear)
      return;

    if (_magicComboTimer && _magicComboTimer < _magicComboTimeOut)
      _magicComboTimer++;

    for (byte i = 0;; i++) {
      dictionary_t combo;

      combo.leftHand = pgm_read_dword (&(_magicDictionary[i].leftHand));
      combo.rightHand = pgm_read_dword (&(_magicDictionary[i].rightHand));

      if (combo.leftHand == 0 && combo.rightHand == 0)
        break;

      if (KeyboardHardware.leftHandState.all == combo.leftHand &&
          KeyboardHardware.rightHandState.all == combo.rightHand) {
        if (_magicComboTimer == 0 || _magicComboTimer >= _magicComboTimeOut ||
            _magicComboTimeOut == 0) {
          magicComboActions (i, combo.leftHand, combo.rightHand);
          _magicComboTimer = 1;
        }
        break;
      }
    }
  }

};

__attribute__((weak))
void
magicComboActions (uint8_t comboIndex, uint32_t leftHand, uint32_t rightHand) {
}

Akela::MagicCombo MagicCombo;
