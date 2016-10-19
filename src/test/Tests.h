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

#pragma once

#include "Akela.h"

#define LAYOUT_SIZE 64

#include <iostream>
#include <iomanip>

#include "stubs/HID/HID.h"
#include "stubs/KeyMap/Printer.h"
#include "stubs/keymaps.h"
#include "stubs/Keyboard/PressRelease.h"
#include "stubs/EventHandler/FnPrinter.h"

int TestBasics ();
int TestMods ();
int TestFn ();
int TestFnHandler ();
