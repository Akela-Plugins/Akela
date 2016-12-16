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

#include "Jukebox.h"

Akela::LEDEffects::TriColor JukeboxEffect((cRGB){0xee, 0xe8, 0xc8} /* TM */,
                                          (cRGB){0x8c, 0xee, 0xc3} /* VCO */,
                                          (cRGB){0xd7, 0x38, 0x21} /* RN */);

Akela::LEDEffects::TriColor JukeboxAlternateEffect((cRGB){0xee, 0xe8, 0xc8} /* TM */,
                                                   (cRGB){0xd7, 0x38, 0x21} /* RN */,
                                                   (cRGB){0x8c, 0xee, 0xc3} /* VCO */);
