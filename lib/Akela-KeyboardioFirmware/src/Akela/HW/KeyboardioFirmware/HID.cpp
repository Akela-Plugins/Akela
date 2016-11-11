/*
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

#include <Akela-KeyboardioFirmware.h>

namespace Akela {
  namespace Keyboardio {

    void
    HID::press (uint8_t code) {
      ::Keyboard.press (code);
    }

    void
    HID::release (uint8_t code) {
      ::Keyboard.release (code);
    }

    void
    HID::sendReport () {
      ::Keyboard.sendReport ();
    }

    bool
    HID::isModifierActive (uint8_t code) {
      return ::Keyboard.isModifierActive (code);
    }

  };
};
