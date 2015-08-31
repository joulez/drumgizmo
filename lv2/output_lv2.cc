/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            output_lv2.cc
 *
 *  Wed Jul 13 14:27:06 CEST 2011
 *  Copyright 2011 Bent Bisballe Nyeng
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
#include "output_lv2.h"

#include <string.h>

OutputLV2::OutputLV2()
{
  for(size_t i = 0; i < NUM_OUTPUTS; i++) {
    outputPorts[i].size = 0;
    outputPorts[i].samples = NULL;
  }
}

OutputLV2::~OutputLV2()
{
}

bool OutputLV2::init(Channels channels)
{
  return true;
}

void OutputLV2::setParm(std::string parm, std::string value)
{
}

bool OutputLV2::start()
{
  return true;
}

void OutputLV2::stop()
{
}

void OutputLV2::pre(size_t nsamples)
{
}

void OutputLV2::run(int ch, sample_t *samples, size_t nsamples)
{
  if(ch < NUM_OUTPUTS) {
    if(outputPorts[ch].samples) {
      memcpy(outputPorts[ch].samples, samples, nsamples * sizeof(sample_t));
    }
  }
}

void OutputLV2::post(size_t nsamples)
{
}

sample_t *OutputLV2::getBuffer(int ch)
{
  if(ch < NUM_OUTPUTS) return outputPorts[ch].samples;
  return NULL;
}
