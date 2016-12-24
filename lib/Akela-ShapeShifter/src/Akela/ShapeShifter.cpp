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
#include <Akela-ShapeShifter.h>

namespace Akela {

  static const ShapeShifter::dictionary_t *shapeShiftDictionary = NULL;

  ShapeShifter::ShapeShifter (void) {
  }

  void
  ShapeShifter::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  ShapeShifter::configure (const dictionary_t dictionary[]) {
    shapeShiftDictionary = (const dictionary_t *)dictionary;
  }

  void
  ShapeShifter::on (void) {
    event_handler_hook_replace (this->noOpHook, this->eventHandlerHook);
  }

  void
  ShapeShifter::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->noOpHook);
  }

  bool
  ShapeShifter::noOpHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    return false;
  }

  bool
  ShapeShifter::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!shapeShiftDictionary)
      return false;

    // If Shift is not active, bail out early.
    if (!Keyboard.isModifierActive (Key_LShift.rawKey) &&
        !Keyboard.isModifierActive (Key_RShift.rawKey))
      return false;

    // If the key is injected, bail out too, to avoid getting into an endless
    // loop.
    if (keyState & INJECTED)
      return false;

    uint8_t i = 0;

    // Try to find the current key in the dictionary
    while (shapeShiftDictionary[i].original.raw != Key_NoKey.raw &&
           shapeShiftDictionary[i].original.raw != mappedKey.raw) {
      i++;
    }
    // If not found, bail out.
    if (shapeShiftDictionary[i].original.raw == Key_NoKey.raw)
      return false;

    // If found, handle the alternate key instead
    handle_key_event (shapeShiftDictionary[i].replacement, row, col, keyState | INJECTED);

    return true;
  }

};

Akela::ShapeShifter ShapeShifter;
