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

#include "MacPC.h"

namespace Akela {

  MacPC::MacPC (void) {
    event_handler_hook_add (this->eventHandlerHook);
  }

  void
  MacPC::setup (void) {
    ::HostOS.setup ();
  }

  bool
  MacPC::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & INJECTED)
      return false;

    if (mappedKey.raw != Key_AltCmd.raw)
      return false;

    if (::HostOS.os () != Akela::HostOS::OSX)
      return false;

    handle_key_event (Key_LGUI, row, col, keyState | INJECTED);
    return true;
  }
};

Akela::MacPC MacPC;
