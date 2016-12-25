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
#include <Akela-DualUse.h>

using namespace Akela::Ranges;

namespace Akela {

  static uint8_t altActionNeededMap;
  static uint8_t timer[8];
  static const uint8_t timeOut = DEFAULT_TIMEOUT * 2;
  static bool modifierDefault;

  DualUseMods::DualUseMods (void) {
  }

  void
  DualUseMods::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  DualUseMods::configure (uint8_t offAction) {
    modifierDefault = !!offAction;
  }

  void
  DualUseMods::on (void) {
    event_handler_hook_replace (this->disabledHook, this->eventHandlerHook);
  }

  void
  DualUseMods::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->disabledHook);
  }

  void
  DualUseMods::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

  Key
  DualUseMods::disabledHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.raw < DUM_FIRST || mappedKey.raw > DUM_LAST)
      return mappedKey;

    Key newKey = { KEY_FLAGS, mappedKey.rawKey };
    if (modifierDefault)
      newKey.rawKey = Key_LCtrl.rawKey + (mappedKey.flags & ~(B00000111));

    return newKey;
  }

  Key
  DualUseMods::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If nothing happened, bail out fast.
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      if (mappedKey.raw < DUM_FIRST || mappedKey.raw > DUM_LAST)
        return mappedKey;
      return Key_NoKey;
    }

    // If a key has been just toggled on...
    if (key_toggled_on (keyState)) {
      if (mappedKey.raw < DUM_FIRST || mappedKey.raw > DUM_LAST) {
        // Not a Dual-use key
        altActionNeededMap = 0;
        memset (timer, 0, 8);

        // We do not need to register modifiers here, the dual-use keys will be looped over, and we'll register it then.
        // FIXME: We might want to register anyway, to make sure the key gets sent, and something before that does not trigger a sendReport.
        return mappedKey;
      }

      uint8_t modIndex = mappedKey.flags & ~(B00000111);
      bitWrite (altActionNeededMap, modIndex, 1);
      if (timer[modIndex] < timeOut)
        timer[modIndex]++;

      // We do not immediately register the modifier, so lets return here fast!
      return Key_NoKey;
    }

    // So the key that was pressed is not toggled on, but either released or
    // held. If released or held, we don't care it it is not a DualUse key.
    if (mappedKey.raw < DUM_FIRST || mappedKey.raw > DUM_LAST)
      return mappedKey;

    // Now, we know it is a dual-use key, either released or held.

    uint8_t modIndex = mappedKey.flags & ~(B00000111);

    // If we have not timed out yet, up the timer.
    if (timer[modIndex] < timeOut)
      timer[modIndex]++;

    // If we timed out, then we do not need the alt action.
    if (timer[modIndex] > timeOut)
      bitWrite (altActionNeededMap, modIndex, 0);

    // If we need no action for the key, then handle it as a modifier. The
    // states will decide if it is a hold or a release, we do not need to care.
    if (!bitRead (altActionNeededMap, modIndex)) {
      Key newKey;

      newKey.flags = KEY_FLAGS;
      newKey.rawKey = Key_LCtrl.rawKey + modIndex;
      return newKey;
    }

    // We do need an alt action for the key, and we did not time out.

    // If the key was just released, then register the alternate code, but don't register the modifier.
    if (key_toggled_off (keyState)) {
      return (Key) { KEY_FLAGS, mappedKey.rawKey };
    }

    // if the key is still held, we have not timed out yet, and an action is
    // needed, then we don't do anything, but wait.

    return Key_NoKey;
  }

};

Akela::DualUseMods DualUseMods;
