/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            chresampler.h
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
#ifndef __DRUMGIZMO_CHRESAMPLER_H__
#define __DRUMGIZMO_CHRESAMPLER_H__

#include <stdlib.h>

/**
 * Channel resampler class using either zita-resampler or secret rabbit code
 * (really!!) depending on the value of the WITH_RESAMPLER macro.
 * If WITH_RESAMPLER is unset the resampler is disabled entirely.
 * If WITH_RESAMPLER=="zita" zita-resampler will be used.
 * If WITH_RESAMPLER=="src" Secret Rabbit Code will be used.
 */
class CHResampler {
public:
  CHResampler();
  ~CHResampler();

  void setup(double input_fs, double output_fs);

  void setInputSamples(float *samples, size_t count);
  void setOutputSamples(float *samples, size_t count);

  void process();

  size_t getInputSampleCount();
  size_t getOutputSampleCount();

  double ratio();

private:
  class Prv;
  Prv *prv;

  double input_fs;
  double output_fs;
};


#endif/*__DRUMGIZMO_CHRESAMPLER_H__*/
