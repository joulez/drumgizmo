/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            output_lv2.h
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
#ifndef __DRUMGIZMO_OUTPUT_LV2_H__
#define __DRUMGIZMO_OUTPUT_LV2_H__

#include <audiooutputengine.h>

#define NUM_OUTPUTS 64

class OutputPort {
public:
  size_t size;
  sample_t *samples;
};

class OutputLV2 : public AudioOutputEngine {
public:
  OutputLV2();
  ~OutputLV2();

  bool init(Channels channels);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre(size_t nsamples);
  void run(int ch, sample_t *samples, size_t nsamples);
  void post(size_t nsamples);

  sample_t *getBuffer(int c);

  OutputPort outputPorts[NUM_OUTPUTS];
};

#endif/*__DRUMGIZMO_OUTPUT_LV2_H__*/
