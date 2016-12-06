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

#pragma once

#include "Akela/HostOS.h"
#include "Akela/HostOS-Tracker.h"
#include "Akela/HostOS-Guesser.h"

#ifdef AKELA_HOSTOS_GUESSER
static Akela::HostOS::Guesser HostOS;
#else
static Akela::HostOS::Tracker HostOS;
#endif
