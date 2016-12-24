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

#include "Leader.h"

using namespace Akela::Ranges;

namespace Akela {
  // --- state ---
  static Key leaderSeq[LEADER_MAX_SEQUENCE_LENGTH + 1];
  static uint8_t leaderSeqPos;
  static uint16_t leaderTimer;
  static uint16_t leaderTimeOut = DEFAULT_TIMEOUT;
  static const Leader::dictionary_t *leaderDictionary;

  // --- helpers ---

#define isLeader(k) (k.raw >= LEAD_FIRST && k.raw <= LEAD_LAST)
#define isActive() (leaderSeq[0].raw = Key_NoKey.raw)

  // --- actions ---
  int8_t
  Leader::lookup (void) {
    bool match;

    for (uint8_t seqIndex = 0; ; seqIndex++) {
      match = true;

      if (pgm_read_word (&(leaderDictionary[seqIndex].sequence[0].raw)) == Key_NoKey.raw)
        break;

      Key seqKey;
      for (uint8_t i = 0; i <= leaderSeqPos; i++) {
        seqKey.raw = pgm_read_word (&(leaderDictionary[seqIndex].sequence[i].raw));

        if (leaderSeq[i].raw != seqKey.raw) {
          match = false;
          break;
        }
      }

      if (!match)
        continue;

      seqKey.raw = pgm_read_word (&(leaderDictionary[seqIndex].sequence[leaderSeqPos + 1].raw));
      if (seqKey.raw == Key_NoKey.raw) {
        return seqIndex;
      }
    }

    return -1;
  }

  // --- api ---

  Leader::Leader (void) {
  }

  void
  Leader::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
    loop_hook_add (this->loopHook);
  }

  void
  Leader::configure (const Leader::dictionary_t dictionary[]) {
    leaderDictionary = dictionary;
  }

  void
  Leader::reset (void) {
    leaderTimer = 0;
    leaderSeqPos = 0;
    leaderSeq[0].raw = Key_NoKey.raw;
  }

  void
  Leader::inject (Key key, uint8_t keyState) {
    eventHandlerHook (key, 255, 255, keyState);
  }

  // --- hooks ---
  bool
  Leader::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & INJECTED)
      return false;

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState))
      return isLeader (mappedKey);

    if (!isActive () && !isLeader (mappedKey))
      return false;

    if (!isActive ()) {
      // Must be a leader key!

      if (key_toggled_off (keyState)) {
        // not active, but a leader key = start the sequence on key release!
        leaderSeqPos = 0;
        leaderSeq[leaderSeqPos].raw = mappedKey.raw;
      }

      // If the sequence was not active yet, ignore the key.
      return true;
    }

    // active
    int8_t actionIndex = lookup ();

    if (key_toggled_on (keyState)) {
      leaderSeqPos++;
      if (leaderSeqPos > LEADER_MAX_SEQUENCE_LENGTH) {
        reset ();
        return false;
      }

      leaderTimer = 0;
      leaderSeq[leaderSeqPos].raw = mappedKey.raw;
      actionIndex = lookup ();

      if (actionIndex < 0) {
        // No match, abort and pass it through.
        reset ();
        return false;
      }
      return true;
    } else if (key_is_pressed (keyState)) {
      // held, no need for anything here.
      return true;
    }

    if (actionIndex < 0) {
      // No match, abort and pass it through.
      reset ();
      return false;
    }

    action_t leaderAction = (action_t) pgm_read_ptr (&(leaderDictionary[actionIndex].action));
    (*leaderAction) (actionIndex);
    return true;
  }

  void
  Leader::loopHook (void) {
    if (!isActive ())
      return;

    if (leaderTimer < leaderTimeOut)
      leaderTimer++;

    if (leaderTimer >= leaderTimeOut)
      reset ();
  }
};

Akela::Leader Leader;
