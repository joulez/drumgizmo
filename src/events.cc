/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            event.cc
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
#include "events.h"

void EventQueue::post(Event *event, timepos_t time)
{
  MutexAutolock lock(mutex);
  event->offset = time;
  queue.insert(std::pair<timepos_t, Event*>(time, event));
}

Event *EventQueue::take(timepos_t time)
{
  MutexAutolock lock(mutex);
  std::multimap<timepos_t, Event*>::iterator i = queue.find(time);
  if(i == queue.end()) return NULL;
  Event *event = i->second;
  queue.erase(i);
  return event;
}

bool EventQueue::hasEvent(timepos_t time)
{
  MutexAutolock lock(mutex);
  return queue.find(time) != queue.end();
}
