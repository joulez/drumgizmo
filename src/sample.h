/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            sample.h
 *
 *  Mon Jul 21 10:23:20 CEST 2008
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
#ifndef __DRUMGIZMO_SAMPLE_H__
#define __DRUMGIZMO_SAMPLE_H__

#include <map>
#include <string>

#include "channel.h"
#include "audiofile.h"

typedef std::map< Channel*, AudioFile* > AudioFiles;

class InstrumentParser;
class Sample {
  friend class InstrumentParser;
  friend class PowerList;
public:
  Sample(std::string name, float power);
  ~Sample();

  AudioFile *getAudioFile(InstrumentChannel *c);

private:
  void addAudioFile(InstrumentChannel *c, AudioFile *a);

  std::string name;
  float power;
  AudioFiles audiofiles;
};

/*
 *   <sample name="kick-r-1">
 *    <audiofile channel="Alesis-3" file="samples/1-kick-r-Alesis-3.wav"/>
 *    <audiofile channel="Amb L-3" file="samples/1-kick-r-Amb L-3.wav"/>
 *    <audiofile channel="Amb R-3" file="samples/1-kick-r-Amb R-3.wav"/>
 *    <audiofile channel="Kick L-3" file="samples/1-kick-r-Kick L-3.wav"/>
 *    <audiofile channel="Kick R-3" file="samples/1-kick-r-Kick R-3.wav"/>
 *   </sample>
 */
#endif/*__DRUMGIZMO_SAMPLE_H__*/
