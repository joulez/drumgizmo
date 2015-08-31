/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            output_vst.h
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
#ifndef __DRUMGIZMO_OUTPUT_VST_H__
#define __DRUMGIZMO_OUTPUT_VST_H__

#include <audiooutputengine.h>

class OutputVST : public AudioOutputEngine {
public:
  OutputVST();
  ~OutputVST();

  bool init(Channels channels);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre(size_t nsamples);
  void run(int ch, sample_t *samples, size_t nsamples);
  void post(size_t nsamples);

  void setOutputs(float **outputs);

private:
  sample_t **outputs;
};

#endif/*__DRUMGIZMO_OUTPUT_VST_H__*/
