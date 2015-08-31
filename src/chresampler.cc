/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            chresampler.cc
 *
 *  Tue Sep 23 20:42:14 CEST 2014
 *  Copyright 2014 Bent Bisballe Nyeng
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
#include "chresampler.h"

#include <config.h>
#include <hugin.hpp>
#include <stdio.h>

#ifdef WITH_RESAMPLER

#if defined(USE_ZITA)
  #include <zita-resampler/resampler.h>
#elif defined(USE_SRC)
  #include <samplerate.h>
#else
  #error "No resampler selected"
#endif

class CHResampler::Prv {
public:
#if defined(USE_ZITA)
  Resampler zita;
#elif defined(USE_SRC)
  SRC_STATE *state;
  SRC_DATA data;
#endif
};

CHResampler::CHResampler()
{
  input_fs = 44100;
  output_fs = 44100;

  prv = new Prv();
#if defined(SRC)
  prv->state = NULL;
#endif
}

void CHResampler::setup(double input_fs, double output_fs)
{
  int nchan = 1; // always mono

  this->input_fs = input_fs;
  this->output_fs = output_fs;

#if defined(USE_ZITA)
  DEBUG(resampler, "Using zita-resampler (%d -> %d)", (int)input_fs, (int)output_fs);

  int hlen = 72; // 16 ≤ hlen ≤ 96
  // delay is 2 * hlen, 72 corresponds to delay introduced by SRC.
  prv->zita.setup(input_fs, output_fs, nchan, hlen);
#elif defined(USE_SRC)
  DEBUG(resampler, "Using libsamplerate (%d -> %d)", (int)input_fs, (int)output_fs);

  int err;
  prv->state = src_new(SRC_SINC_BEST_QUALITY, nchan, &err);
  (void)err;
  //  printf("err: %d\n", err);
  src_set_ratio(prv->state, output_fs / input_fs);
  prv->data.src_ratio = output_fs / input_fs;
  prv->data.end_of_input = 0;
#endif
}

CHResampler::~CHResampler()
{
#if defined(USE_ZITA)
#elif defined(USE_SRC)
  if(prv->state) src_delete(prv->state);
#endif
  delete prv;
}

void CHResampler::setInputSamples(float *samples, size_t count)
{
#if defined(USE_ZITA)
  prv->zita.inp_data = samples;
  prv->zita.inp_count = count;
#elif defined(USE_SRC)
  prv->data.data_in = samples;
  prv->data.input_frames = count;
#endif
}

void CHResampler::setOutputSamples(float *samples, size_t count)
{
#if defined(USE_ZITA)
  prv->zita.out_data = samples;
  prv->zita.out_count = count;
#elif defined(USE_SRC)
  prv->data.data_out = samples;
  prv->data.output_frames = count;
#endif
}

void CHResampler::process()
{
#if defined(USE_ZITA)
  prv->zita.process();
#elif defined(USE_SRC)
  src_process(prv->state, &prv->data);
  prv->data.output_frames -= prv->data.output_frames_gen;
  prv->data.data_out +=  prv->data.output_frames_gen;
  prv->data.input_frames -= prv->data.input_frames_used;
  prv->data.data_in += prv->data.input_frames_used;
#endif
}

size_t CHResampler::getInputSampleCount()
{
#if defined(USE_ZITA)
  return prv->zita.inp_count;
#elif defined(USE_SRC)
  return prv->data.input_frames;
#endif
}

size_t CHResampler::getOutputSampleCount()
{
#if defined(USE_ZITA)
  return prv->zita.out_count;
#elif defined(USE_SRC)
  return prv->data.output_frames;
#endif
}

double CHResampler::ratio()
{
  return input_fs / output_fs;
}

#else

// Dummy implementation
CHResampler::CHResampler() {}
CHResampler::~CHResampler() {}
void CHResampler::setup(double, double) {}
void CHResampler::setInputSamples(float *, size_t) {}
void CHResampler::setOutputSamples(float *, size_t) {}
void CHResampler::process() {}
size_t CHResampler::getInputSampleCount() { return 0; }
size_t CHResampler::getOutputSampleCount() { return 0; }
double CHResampler::ratio() { return 1; }

#endif/*WITH_RESAMPLER*/
