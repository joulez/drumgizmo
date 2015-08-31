/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audioinputenginemidi.h
 *
 *  Mon Apr  1 20:13:24 CEST 2013
 *  Copyright 2013 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_AUDIOINPUTENGINEMIDI_H__
#define __DRUMGIZMO_AUDIOINPUTENGINEMIDI_H__

#include "audioinputengine.h"

#include <string>

#include "midimapper.h"
#include "instrument.h"

class AudioInputEngineMidi : public AudioInputEngine {
public:
  AudioInputEngineMidi();
  virtual ~AudioInputEngineMidi() {}

  bool isMidiEngine() { return true; } 

  virtual bool init(Instruments &instruments) = 0;

  virtual void setParm(std::string parm, std::string value) = 0;

  virtual bool start() = 0;
  virtual void stop() = 0;

  virtual void pre() = 0;
  virtual event_t *run(size_t pos, size_t len, size_t *nevents) = 0;
  virtual void post() = 0;

  bool loadMidiMap(std::string file, Instruments &i);

  std::string midimapFile();

  bool isValid();

protected:
  MidiMapper mmap;
  std::string file;
  bool is_valid;
};

#endif/*__DRUMGIZMO_AUDIOINPUTENGINEMIDI_H__*/
