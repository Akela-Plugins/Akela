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

#include "KeyLogger.h"

namespace Akela {

  KeyLogger::KeyLogger (void) {
  }

  void
  KeyLogger::begin (void) {
    Serial.begin (9600);
    event_handler_hook_add (this->logger);
  }

  Key
  KeyLogger::logger (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!key_toggled_on (keyState) && !key_toggled_off (keyState))
      return mappedKey;
    if (keyState & INJECTED)
      return mappedKey;

    Serial.print (F("KL: row="));
    Serial.print (row, DEC);
    Serial.print (F(", col="));
    Serial.print (col, DEC);
    Serial.print (F(", pressed="));
    Serial.print (key_toggled_on (keyState), DEC);
    Serial.print (F(", defaultLayer="));
    Serial.print (Layer.defaultLayer (), DEC);
    Serial.print (F(", layerState="));
    Serial.print (Layer.getLayerState (), BIN);
    Serial.print (F(", mappedKey.flags="));
    Serial.print (mappedKey.flags, BIN);
    Serial.print (F(", mappedKey.keyCode="));
    Serial.println (mappedKey.keyCode, HEX);

    return mappedKey;
  }
};

Akela::KeyLogger KeyLogger;
