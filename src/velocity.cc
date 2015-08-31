/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            velocity.cc
 *
 *  Tue Jul 22 18:04:58 CEST 2008
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
#include "velocity.h"

#include <stdlib.h>

Velocity::Velocity(unsigned int lower, unsigned int upper)
{
  this->lower = lower;
  this->upper = upper;
}

void Velocity::addSample(Sample *sample, float probability)
{
  if(samples.find(sample) != samples.end()) {
    samples[sample] += probability;
  } else {
    samples[sample] = probability;
  }
}

Sample *Velocity::getSample()
{
  Sample *sample = NULL;

  float x = (float)rand() / (float)RAND_MAX;
  float sum = 0.0;
  
  Samples::iterator i = samples.begin();
  while(i != samples.end() && x > sum) {
    sum += i->second;
    sample = i->first;
    i++;
  }

  return sample;
}
