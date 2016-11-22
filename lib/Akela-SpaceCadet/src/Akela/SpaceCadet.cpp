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

#include <Akela.h>
#include <Akela-SpaceCadet.h>

namespace Akela {

  static uint8_t parenNeeded = 0;
  static uint8_t timer = 0;
  static const uint8_t timeOut = DEFAULT_TIMEOUT * 2;

  SpaceCadetShift::SpaceCadetShift (uint8_t defaultMode) {
    if (defaultMode == Akela::Default::On)
      event_handler_hook_add (this->eventHandlerHook);
    else
      event_handler_hook_add (this->noOpHook);
  }

  void
  SpaceCadetShift::on (void) {
    event_handler_hook_replace (this->noOpHook, this->eventHandlerHook);
  }

  void
  SpaceCadetShift::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->noOpHook);
  }

  bool
  SpaceCadetShift::noOpHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    return false;
  }

  bool
  SpaceCadetShift::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    // If a key has been just toggled on...
    if (key_toggled_on (currentState, previousState)) { // if it is LShift, remember it
      if (mappedKey.raw == Key_LShift.raw) {
        bitWrite (parenNeeded, 0, 1);
        if (timer < timeOut)
          timer++;
      } else if (mappedKey.raw == Key_RShift.raw) { // if it is RShift, remember it
        bitWrite (parenNeeded, 1, 1);
        if (timer < timeOut)
          timer++;
      } else { // if it is something else, we do not need a paren at the end.
        parenNeeded = 0;
        timer = 0;
      }

      // this is all we need to do on keypress, let the next handler do its thing too.
      return false;
    }

    // if the state is empty, that means that either the shifts weren't pressed,
    // or we used another key in the interim. in both cases, nothing special to do.
    if (!parenNeeded)
      return false;

    // if we did not time out yet, up the timer
    if (timer < timeOut)
      timer++;

    // if we timed out, that means we need to keep pressing shift, but won't
    // need the parens in the end.
    if (timer >= timeOut) {
      parenNeeded = 0;
      timer = 0;
      return false;
    }

    // if we have a state, but the key in question is not either of the shifts,
    // return. This can happen when another key is released, and that should not
    // interrupt us.
    if (mappedKey.raw != Key_LShift.raw &&
        mappedKey.raw != Key_RShift.raw)
      return false;

    // if a key toggled off (and that must be one of the shifts at this point),
    // send the parens too (if we were interrupted, we bailed out earlier).
    if (key_toggled_off (currentState, previousState)) {
      Key paren = Key_9;
      if (bitRead (parenNeeded, 1))
        paren = Key_0;
      handle_key_event (paren, row, col, 1, 1);

      parenNeeded = 0;
      timer = 0;
    }

    return false;
  }

};
