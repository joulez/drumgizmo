/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            instrument.h
 *
 *  Tue Jul 22 17:14:19 CEST 2008
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
#ifndef __DRUMGIZMO_INSTRUMENT_H__
#define __DRUMGIZMO_INSTRUMENT_H__

#include <string>
#include <vector>

#include "rangemap.h"
#include "powerlist.h"

#include "sample.h"
#include "versionstr.h"

class InstrumentParser;
class Instrument {
  friend class InstrumentParser;
public:
  Instrument();
  ~Instrument();

  Sample *sample(level_t level, size_t pos);

  std::string name();
  std::string description();
  std::string group();

  void setGroup(std::string group);

  //  std::map<std::string, std::string> channelmap;

  std::vector<AudioFile*> audiofiles;

  bool isValid();

private:
  void *magic;

  std::string _group;
  std::string _name;
  std::string _description;

  VersionStr version;

  RangeMap<level_t, Sample*> samples;
  PowerList powerlist;

  void addSample(level_t a, level_t b, Sample *s);
  void finalise(); ///< Signal instrument that no more samples will be added.

  std::vector<Sample*> samplelist;

  size_t lastpos;
  float mod;
};

//typedef std::map< std::string, Instrument > Instruments;
typedef std::vector< Instrument* > Instruments;

#endif/*__DRUMGIZMO_INSTRUMENT_H__*/
