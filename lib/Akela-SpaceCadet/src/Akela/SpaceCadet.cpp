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

#include <Akela-SpaceCadet.h>

namespace Akela {

  static uint8_t spaceCadetState = 0;
  static uint8_t spaceCadetTimer = 0;
  static const uint8_t spaceCadetTimeOut = 80;
  static uint8_t spaceCadetMode;

  SpaceCadetShift::SpaceCadetShift (uint8_t mode) {
    spaceCadetMode = mode;
    event_handler_hook_add (this->eventHandlerHook);
  }

  bool
  SpaceCadetShift::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    // If a key has been just toggled on...
    if (key_toggled_on (currentState, previousState)) { // if it is LShift, remember it, and start the timer
      if (mappedKey.raw == Key_LShift.raw) {
        bitWrite (spaceCadetState, 0, 1);
        if (spaceCadetTimer < spaceCadetTimeOut)
          spaceCadetTimer++;
      } else if (mappedKey.raw == Key_RShift.raw) { // if it is RShift, remember it, and start the timer
        bitWrite (spaceCadetState, 1, 1);
        if (spaceCadetTimer < spaceCadetTimeOut)
          spaceCadetTimer++;
      } else { // if it is something else, clear the state, and stop the timer
        spaceCadetState = 0;
        spaceCadetTimer = 0;
      }

      // this is all we need to do on keypress, let the next handler do its thing too.
      return false;
    }

    // if the state is empty, that means that either the shifts weren't pressed,
    // or we used another key in the interim. in both cases, nothing special to do.
    if (!spaceCadetState)
      return false;

    // up the timer, if we did not time out yet
    if (spaceCadetTimer < spaceCadetTimeOut)
      spaceCadetTimer++;

    // if we have a state, and we timed out, and the mode is repeat parens, register the paren too.
    if (spaceCadetMode == RepeatParens && spaceCadetTimer > spaceCadetTimeOut) {
      Key paren = Key_9;
      if (bitRead (spaceCadetState, 1))
        paren = Key_0;
      handle_key_event (paren, row, col, 1, 1);
      return false;
    }

    // if we have a state, but the key in question is not either of the shifts,
    // return. This can happen when another key is released, and that should not
    // interrupt us.
    if (mappedKey.raw != Key_LShift.raw &&
        mappedKey.raw != Key_RShift.raw)
      return false;

    // if a key toggled off (and that must be one of the shifts at this point), send the parens too.
    if (key_toggled_off (currentState, previousState)) {
      // if shift is released after being held for a longer period, don't register the parens.
      if (spaceCadetTimer > spaceCadetTimeOut && spaceCadetMode == HoldShift)
        return false;

      Key paren = Key_9;
      if (bitRead (spaceCadetState, 1))
        paren = Key_0;
      handle_key_event (paren, row, col, 1, 1);
    }

    return false;
  }

};
