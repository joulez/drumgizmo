/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiooutputengine.h
 *
 *  Thu Sep 16 10:27:01 CEST 2010
 *  Copyright 2010 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_AUDIOOUTPUTENGINE_H__
#define __DRUMGIZMO_AUDIOOUTPUTENGINE_H__

#include <string>
#include <stdlib.h>
#include <audiotypes.h>

#include "channel.h"

class AudioOutputEngine {
public:
  virtual ~AudioOutputEngine() {}

  virtual bool init(Channels channels) = 0;

  virtual void setParm(std::string parm, std::string value) = 0;

  virtual bool start() = 0;
  virtual void stop() = 0;

  virtual void pre(size_t nsamples) = 0;
  virtual void run(int ch, sample_t *samples, size_t nsamples) = 0;
  virtual void post(size_t nsamples) = 0;

  // Reimplement this if you wish to use internal buffer directly.
  virtual sample_t *getBuffer(int ch) { return NULL; }

  /*
   * Overload this method to force engine to use different buffer size.
   */
  virtual size_t getBufferSize() { return 1024; }
};

#endif/*__DRUMGIZMO_AUDIOOUTPUTENGINE_H__*/
