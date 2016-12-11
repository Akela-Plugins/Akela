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

  static uint32_t altActionNeededMap;
  static uint8_t timer[32];
  static const uint8_t timeOut = DEFAULT_TIMEOUT * 2;
  static bool layerDefault;

  static bool
  disabledHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (mappedKey.raw < DUL_FIRST || mappedKey.raw > DUL_LAST)
      return false;

    Key newKey = { KEY_FLAGS, mappedKey.rawKey };
    if (layerDefault) {
      uint8_t m = (mappedKey.flags & ~0b11100000) + MOMENTARY_OFFSET;
      newKey = { KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, m };
    }

    handle_key_event (newKey, row, col, keyState | INJECTED);
    return true;
  }

  static bool
  eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If nothing happened, bail out fast.
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      if (mappedKey.raw < DUL_FIRST || mappedKey.raw > DUL_LAST)
        return false;
      return true;
    }

    // If a key has been just toggled on...
    if (key_toggled_on (keyState)) {
      if (mappedKey.raw < DUL_FIRST || mappedKey.raw > DUL_LAST) {
        // Not a Dual-use key
        altActionNeededMap = 0;
        memset (timer, 0, 8);

        return false;
      }

      uint8_t layerIndex = mappedKey.flags & ~0b11100000;
      bitWrite (altActionNeededMap, layerIndex, 1);
      if (timer[layerIndex] < timeOut)
        timer[layerIndex]++;

      // We do not immediately switch layers, so lets return here fast!
      return true;
    }

    // So the key that was pressed is not toggled on, but either released or
    // held. If released or held, we don't care it it is not a DualUse key.
    if (mappedKey.raw < DUL_FIRST || mappedKey.raw > DUL_LAST)
      return false;

    // Now, we know it is a dual-use key, either released or held.
    uint8_t layerIndex = mappedKey.flags & ~0b11100000;

    // If we have not timed out yet, up the timer.
    if (timer[layerIndex] < timeOut)
      timer[layerIndex]++;

    // If we timed out, then we do not need the alt action.
    if (timer[layerIndex] > timeOut)
      bitWrite (altActionNeededMap, layerIndex, 0);

    // If we need no action for the key, then handle it as a momentary layer
    // switch. The states will decide if it is a hold or a release, we do not
    // need to care.
    if (!bitRead (altActionNeededMap, layerIndex)) {
      uint8_t m = layerIndex + MOMENTARY_OFFSET;
      handle_key_event ({ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, m }, row, col, keyState | INJECTED);
      return true;
    }

    // We do need an alt action for the key, and we did not time out.

    // If the key was just released, then register the alternate code.
    if (key_toggled_off (keyState)) {
      handle_key_event ({ KEY_FLAGS, mappedKey.rawKey }, row, col, IS_PRESSED | INJECTED);
    }

    // if the key is still held, we have not timed out yet, and an action is
    // needed, then we don't do anything, but wait.

    return true;
  }

  DualUseLayers::DualUseLayers (void) {
    event_handler_hook_add (eventHandlerHook);
  }

  void
  DualUseLayers::configure (uint8_t offAction) {
    layerDefault = !!offAction;
  }

  void
  DualUseLayers::on (void) {
    event_handler_hook_replace (disabledHook, eventHandlerHook);
  }

  void
  DualUseLayers::off (void) {
    event_handler_hook_replace (eventHandlerHook, disabledHook);
  }

  void
  DualUseLayers::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

};
