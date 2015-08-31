/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            output_vst.cc
 *
 *  Tue Sep 20 10:40:14 CEST 2011
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
#include "output_vst.h"

#include <string.h>

#include "constants.h"

OutputVST::OutputVST()
{
  outputs = NULL;
}

OutputVST::~OutputVST()
{
}

bool OutputVST::init(Channels channels)
{
  return true;
}

void OutputVST::setParm(std::string parm, std::string value)
{
}

bool OutputVST::start()
{
  return true;
}

void OutputVST::stop()
{
}

void OutputVST::pre(size_t nsamples)
{
  if(!outputs) return;

  for(size_t ch = 0; ch < NUM_OUTPUTS; ch++) {
    memset(outputs[ch], 0, nsamples * sizeof(sample_t));
  }
}

void OutputVST::run(int ch, sample_t *samples, size_t nsamples)
{
  if(!outputs) return;

  if(ch < NUM_OUTPUTS) {
    memcpy(outputs[ch], samples, nsamples * sizeof(sample_t));
  }
}

void OutputVST::post(size_t nsamples)
{
  outputs = NULL;
}

void OutputVST::setOutputs(float **outputs)
{
  this->outputs = outputs;
}
