/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audioinputenginemidi.cc
 *
 *  Mon Apr  1 20:13:25 CEST 2013
 *  Copyright 2013 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of DrumGizmo.
 *
 *  DrumGizmo is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  DrumGizmo is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DrumGizmo; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "audioinputenginemidi.h"

#include "midimapparser.h"

#include <hugin.hpp>

AudioInputEngineMidi::AudioInputEngineMidi()
{
  is_valid = false;
}

bool AudioInputEngineMidi::loadMidiMap(std::string f, Instruments &instruments)
{
  file = "";
  is_valid = false;

  DEBUG(mmap, "loadMidiMap(%s, i.size() == %d)\n", f.c_str(),
        (int)instruments.size());

  if(f == "") return false;

  MidiMapParser p(f);
  if(p.parse()) {
    return false;
  }

  mmap.clear();
  mmap.midimap = p.midimap;

  for(size_t i = 0; i < instruments.size(); i++) {
    mmap.instrmap[instruments[i]->name()] = i;
  }

  file = f;
  is_valid = true;

  return true;
}

std::string AudioInputEngineMidi::midimapFile()
{
  return file;
}

bool AudioInputEngineMidi::isValid()
{
  return is_valid;
}

#ifdef TEST_AUDIOINPUTENGINEMIDI
//Additional dependency files
//deps:
//Required cflags (autoconf vars may be used)
//cflags:
//Required link options (autoconf vars may be used)
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).

TEST_END;

#endif/*TEST_AUDIOINPUTENGINEMIDI*/
