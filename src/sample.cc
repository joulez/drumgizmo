/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            sample.cc
 *
 *  Mon Jul 21 10:23:20 CEST 2008
 *  Copyright 2008 Bent Bisballe Nyeng
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
#include "sample.h"

#include <stdlib.h>
#include <unistd.h>

#include <sndfile.h>

Sample::Sample(std::string name, float power)
{
  this->name = name;
  this->power = power;
}

Sample::~Sample()
{
}

void Sample::addAudioFile(Channel *c, AudioFile *a)
{
  audiofiles[c] = a;
}

AudioFile *Sample::getAudioFile(Channel *c)
{
  /*
  if(audiofiles.find(c) == audiofiles.end()) return NULL;
  return audiofiles[c];
  */

  AudioFiles::iterator i = audiofiles.begin();
  while(i != audiofiles.end()) {
    Channel *ch = i->first;
    if(c->num == ch->num) return i->second;
    i++;
  }

  return NULL;
}

#ifdef TEST_SAMPLE
//deps: channel.cc audiofile.cc
//cflags: $(SNDFILE_CFLAGS)
//libs: $(SNDFILE_LIBS)
#include "test.h"

TEST_BEGIN;

Sample s;
InstrumentChannel c;
InstrumentChannel c2;
AudioFile a("test");

s.addAudioFile(&c, &a);
TEST_EQUAL(s.getAudioFile(&c), &a, "?");
TEST_EQUAL(s.getAudioFile(&c2), NULL, "?");

TEST_END;

#endif/*TEST_SAMPLE*/
