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

namespace Akela {

  static uint8_t altActionNeededMap;
  static uint8_t timer[8];
  static const uint8_t timeOut = DEFAULT_TIMEOUT * 2;
  static bool modifierDefault;

  DualUseMods::DualUseMods (uint8_t defaultMode, uint8_t defaultAction) {
    modifierDefault = !!defaultAction;

    if (defaultMode == Akela::Default::On)
      event_handler_hook_add (this->eventHandlerHook);
    else
      event_handler_hook_add (this->disabledHook);
  }

  void
  DualUseMods::on (void) {
    event_handler_hook_replace (this->disabledHook, this->eventHandlerHook);
  }

  void
  DualUseMods::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->disabledHook);
  }

  bool
  DualUseMods::disabledHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    if (mappedKey.flags < (SYNTHETIC | AKELA | MT_FIRST) ||
        mappedKey.flags > (SYNTHETIC | AKELA | MT_LAST))
      return false;

    Key newKey = { KEY_FLAGS, mappedKey.rawKey };
    if (modifierDefault)
      newKey.rawKey = Key_LCtrl.rawKey + mappedKey.flags - (SYNTHETIC | AKELA | MT_FIRST);

    handle_key_event (newKey, row, col, currentState, previousState);
    return true;
  }

  bool
  DualUseMods::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t currentState, uint8_t previousState) {
    // If a key has been just toggled on...
    if (key_toggled_on (currentState, previousState)) {
      if (mappedKey.flags < (SYNTHETIC | AKELA | MT_FIRST) ||
          mappedKey.flags > (SYNTHETIC | AKELA | MT_LAST)) {
        // Not a Dual-use key
        altActionNeededMap = 0;
        memset (timer, 0, 8);

        // We do not need to register modifiers here, the dual-use keys will be looped over, and we'll register it then.
        // FIXME: We might want to register anyway, to make sure the key gets sent, and something before that does not trigger a sendReport.
        return false;
      }

      uint8_t modIndex = mappedKey.flags & ~(SYNTHETIC | AKELA);
      bitWrite (altActionNeededMap, modIndex, 1);
      if (timer[modIndex] < timeOut)
        timer[modIndex]++;

      // We do not immediately register the modifier, so lets return here fast!
      return true;
    }

    // So the key that was pressed is not toggled on, but either released or
    // held. If released or held, we don't care it it is not a DualUse key.
    if (mappedKey.flags < (SYNTHETIC | AKELA | MT_FIRST) ||
        mappedKey.flags > (SYNTHETIC | AKELA | MT_LAST))
      return false;

    // Now, we know it is a dual-use key, either released or held.

    uint8_t modIndex = mappedKey.flags & ~(SYNTHETIC | AKELA);

    // If we have not timed out yet, up the timer.
    if (timer[modIndex] < timeOut)
      timer[modIndex]++;

    // If we timed out, then we do not need the alt action.
    if (timer[modIndex] > timeOut)
      bitWrite (altActionNeededMap, modIndex, 0);

    // If we need no action for the key, then handle it as a modifier. The
    // states will decide if it is a hold or a release, we do not need to care.
    if (!bitRead (altActionNeededMap, modIndex)) {
      uint8_t m = Key_LCtrl.rawKey + modIndex;
      handle_key_event ({ KEY_FLAGS, m }, row, col, currentState, previousState);
      return true;
    }

    // We do need an alt action for the key, and we did not time out.

    // If the key was just released, then register the alternate code, but don't registering the modifier.
    if (key_toggled_off (currentState, previousState)) {
      handle_key_event ({ KEY_FLAGS, mappedKey.rawKey }, row, col, 1, 0);
    }

    // if the key is still held, we have not timed out yet, and an action is
    // needed, then we don't do anything, but wait.

    return true;
  }

};
