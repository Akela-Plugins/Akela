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

#include "KeyLogger.h"

namespace Akela {

  KeyLogger::KeyLogger (void) {
    Serial.begin (9600);
    event_handler_hook_add (this->logger);
  }

  bool
  KeyLogger::logger (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    if (!key_toggled_on (currentState, previousState) && !key_toggled_off (currentState, previousState))
      return false;

    Serial.print ("KL: row=");
    Serial.print (row, DEC);
    Serial.print (", col=");
    Serial.print (col, DEC);
    Serial.print (", pressed=");
    Serial.print (key_toggled_on (currentState, previousState), DEC);
    Serial.print (", layer=");
    Serial.println (temporary_keymap, DEC);

    return false;
  }
};

static Akela::KeyLogger _keyLogger;
