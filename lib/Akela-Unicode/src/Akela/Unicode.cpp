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

#include "Unicode.h"

namespace Akela {

  Unicode::Unicode (void) {
  }

  void
  Unicode::setup (void) {
    ::HostOS.setup ();
  }

  void
  Unicode::start (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
      Keyboard.press (Key_LCtrl.rawKey);
      Keyboard.press (Key_LShift.rawKey);
      Keyboard.press (Key_U.rawKey);
      Keyboard.sendReport ();
      Keyboard.release (Key_LCtrl.rawKey);
      Keyboard.release (Key_LShift.rawKey);
      Keyboard.release (Key_U.rawKey);
      Keyboard.sendReport ();
      break;
    case HostOS::WINDOWS:
      Keyboard.press (Key_RAlt.rawKey);
      Keyboard.sendReport ();
      Keyboard.release (Key_RAlt.rawKey);
      Keyboard.sendReport ();
      Keyboard.press (Key_U.rawKey);
      Keyboard.sendReport ();
      Keyboard.release (Key_U.rawKey);
      Keyboard.sendReport ();
      break;
    case HostOS::OSX:
      Keyboard.press (Key_LAlt.rawKey);
      break;
    default:
      unicodeCustomStart ();
      break;
    }
  }

  void
  Unicode::input (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
    case HostOS::WINDOWS:
      break;
    case HostOS::OSX:
      Keyboard.press (Key_LAlt.rawKey);
      break;
    default:
      unicodeCustomInput ();
      break;
    }
  }

  void
  Unicode::end (void) {
    switch (::HostOS.os ()) {
    case HostOS::LINUX:
      Keyboard.press (Key_Space.rawKey);
      Keyboard.sendReport ();
      Keyboard.release (Key_Space.rawKey);
      Keyboard.sendReport ();
      break;
    case HostOS::WINDOWS:
      break;
    case HostOS::OSX:
      Keyboard.release (Key_LAlt.rawKey);
      Keyboard.sendReport ();
      break;
    default:
      unicodeCustomEnd ();
      break;
    }
  }

  void
  Unicode::type_code (uint32_t unicode) {
    bool onZeroStart = true;

    for (int8_t i = 7; i >= 0; i--) {
      if (i <= 3) {
        onZeroStart = false;
      }

      uint8_t digit = ((unicode >> (i * 4)) & 0xF);
      if (digit == 0) {
        if (onZeroStart == false) {
          Key key = hexToKey (digit);

          input ();
          Keyboard.press (key.rawKey);
          Keyboard.sendReport ();
          input ();
          Keyboard.release (key.rawKey);
          Keyboard.sendReport ();
        }
      } else {
        Key key = hexToKey (digit);

        input ();
        Keyboard.press (key.rawKey);
        Keyboard.sendReport ();
        input ();
        Keyboard.release (key.rawKey);
        Keyboard.sendReport ();

        onZeroStart = false;
      }
    }
  }

  void
  Unicode::type (uint32_t unicode) {
    start ();
    type_code (unicode);
    end ();
  }
};

__attribute__((weak))
Key
hexToKey (uint8_t hex) {
  uint8_t m;

  if (hex == 0x0) {
    return Key_0;
  }

  if (hex < 0xA) {
    m = Key_1.rawKey + (hex - 0x1);
  } else {
    m = Key_A.rawKey + (hex - 0xA);
  }

  return { KEY_FLAGS, m};
}

__attribute__((weak))
void
unicodeCustomStart (void) {
}

__attribute__((weak))
void
unicodeCustomEnd (void) {
}

__attribute__((weak))
void
unicodeCustomInput (void) {
}

Akela::Unicode Unicode;
