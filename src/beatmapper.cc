/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            beatmapper.cc
 *
 *  Fri Jul 25 11:17:42 CEST 2008
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
#include "beatmapper.h"

#include <stdio.h>

#define DEF 2.0

BeatMapper::BeatMapper(Instrument *instrument)
{
  this->instrument = instrument;
  for(size_t i = 0; i < HISTORY_SIZE; i++) hist[i] = DEF;
  C = 1.3;
  mindist = 4;
  last = mindist;
}


Sample *BeatMapper::map(jack_nframes_t nframes)
{
  return NULL;
  Sample *sample = NULL;
  
  jack_default_audio_sample_t *buffer;
  buffer = (jack_default_audio_sample_t *)jack_port_get_buffer(instrument->port, nframes);
  
  float e = 0.0;
  for(size_t i = 0; i < nframes; i++) {
    e += buffer[i] * buffer[i];
  }

  float E = 0.0;
  for(size_t i = 0; i < HISTORY_SIZE; i++) E += hist[i] / (float)HISTORY_SIZE;
  if(E == 0) E = DEF; // We do not have a connection

  //  printf("last: %d, E: %f,  e: %f - threshold: %f\n", last, E, e, 1.3 * E);

  // Shift history and save new value
  for(size_t i = 0; i < HISTORY_SIZE - 1; i++) hist[i] = hist[i+1];
  hist[HISTORY_SIZE - 1] = e>DEF?e:DEF;

  if(instrument->name == "hihat" && e > 0) printf("e: %f\n", e);

  if(e > C * E && last > mindist) {
    Velocity *v = instrument->getVelocity(127);
    if(v) sample = v->getSample();
  }

  last++;
  if(sample) last = 0;

  return sample;
}
