/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            instrument.cc
 *
 *  Tue Jul 22 17:14:20 CEST 2008
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
#include "instrument.h"

#include <stdlib.h>
#include <stdio.h>

#include <hugin.hpp>

#include "sample.h"
#include "configuration.h"

Instrument::Instrument()
{
  DEBUG(instrument, "new %p\n", this);
  mod = 1.0;
  lastpos = 0;

  magic = this;
}

Instrument::~Instrument()
{
  magic = NULL;

  DEBUG(instrument, "delete %p\n", this);
  std::vector<AudioFile*>::iterator i = audiofiles.begin();
  while(i != audiofiles.end()) {
    delete *i;
    i++;
  }
}

bool Instrument::isValid()
{
  return this == magic;
}

Sample *Instrument::sample(level_t level, size_t pos)
{
  Sample *sample = NULL;

  if(Conf::enable_velocity_modifier == false) {
    mod = 1.0;
    lastpos = 0;
  }

  if(Conf::enable_velocity_randomiser) {
    float r = (float)rand() / (float)RAND_MAX; // random number: [0;1]
    r -= 0.5; // random number [-0.5;0.5]
    r *= Conf::velocity_randomiser_weight * 2; // ex. random number [-0.1;0.1]
    level += r;
    if(level > 1.0) level = 1.0;
    if(level < 0.0) level = 0.0;
  }

  if(Conf::enable_velocity_modifier) {
    mod += (pos - lastpos) /
      (Conf::samplerate * Conf::velocity_modifier_falloff);
    if(mod > 1.0) mod = 1.0;
  }

  if(version >= VersionStr("2.0")) {
    // Version 2.0
    sample = powerlist.get(level * mod);
  } else {
    // Version 1.0
    std::vector<Sample*> s = samples.get(level * mod);
    if(s.size() == 0) return NULL;
    size_t idx = rand()%(s.size());
    sample = s[idx];
  }

  if(Conf::enable_velocity_modifier) {
    lastpos = pos;
    mod *= Conf::velocity_modifier_weight;
  }

  return sample;
}

void Instrument::addSample(level_t a, level_t b, Sample *s)
{
  samples.insert(a, b, s);
}

void Instrument::finalise()
{
  if(version >= VersionStr("2.0")) {
    std::vector<Sample*>::iterator s = samplelist.begin();
    while(s != samplelist.end()) {
      powerlist.add(*s);
      s++;
    }

    powerlist.finalise();
  }
}

std::string Instrument::name()
{
  return _name;
}

std::string Instrument::description()
{
  return _description;
}

std::string Instrument::group()
{
  return _group;
}

void Instrument::setGroup(std::string g)
{
  _group = g;
}

#ifdef TEST_INSTRUMENT
//deps: channel.cc sample.cc audiofile.cc
//cflags: $(SNDFILE_CFLAGS)
//libs: $(SNDFILE_LIBS)
#include "test.h"

TEST_BEGIN;

Instrument i("test");

Sample *a = new Sample();
i.addSample(0.0, 1.0, a);

Sample *b = new Sample();
i.addSample(0.0, 1.0, b);

Sample *c = new Sample();
i.addSample(1.5, 1.7, c);

TEST_EQUAL(i.sample(0.0), b, "?");
TEST_EQUAL(i.sample(0.0), a, "?");
TEST_EQUAL(i.sample(0.0), b, "?");
TEST_EQUAL(i.sample(0.0), b, "?");
TEST_EQUAL(i.sample(0.0), b, "?");
TEST_EQUAL(i.sample(0.0), b, "?");
TEST_EQUAL(i.sample(0.0), a, "?");
TEST_EQUAL(i.sample(0.0), a, "?");

TEST_EQUAL(i.sample(2.0), NULL, "?");

TEST_EQUAL(i.sample(1.6), c, "?");
TEST_EQUAL(i.sample(1.6), c, "?");
TEST_EQUAL(i.sample(1.6), c, "?");

TEST_END;

#endif/*TEST_INSTRUMENT*/
