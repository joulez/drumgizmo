/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            event.h
 *
 *  Sat Sep 18 22:02:16 CEST 2010
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
#ifndef __DRUMGIZMO_EVENTS_H__
#define __DRUMGIZMO_EVENTS_H__

#include <map>
#include <stdio.h>
#include <string>
#include <sndfile.h>

#include "audiofile.h"
#include "audio.h"
#include "mutex.h"

typedef unsigned int timepos_t;

class Event {
public:
  virtual ~Event() {}

  typedef enum {
    sample
  } type_t;
  
  virtual type_t type() = 0;

  channel_t channel;
  timepos_t offset;
};

#define NO_RAMPDOWN -1
class EventSample : public Event {
public:
  EventSample(channel_t c, float g, AudioFile *af, std::string grp,
              void *instr)
  {
    channel = c;
    gain = g;
    t = 0;
    file = af;
    group = grp;
    instrument = instr;
    rampdown = NO_RAMPDOWN;
    ramp_start = 0;
  }

  Event::type_t type() { return Event::sample; }

  float gain;
  unsigned int t;
  AudioFile *file;
  std::string group;
  void *instrument;
  int rampdown;
  int ramp_start;
};

class EventQueue {
public:
  void post(Event *event, timepos_t time);
  Event *take(timepos_t time);
  bool hasEvent(timepos_t time);
  size_t size() { return queue.size(); }

private:
  std::multimap< timepos_t, Event* > queue;
  Mutex mutex;
};

#endif/*__DRUMGIZMO_EVENTS_H__*/
