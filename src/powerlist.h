/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            powerlist.h
 *
 *  Sun Jul 28 19:45:47 CEST 2013
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
#ifndef __DRUMGIZMO_POWERLIST_H__
#define __DRUMGIZMO_POWERLIST_H__

#include <vector>

#include "sample.h"

class PowerList {
public:
  PowerList();

  void add(Sample *s);
  void finalise(); ///< Call this when no more samples will be added.

  Sample *get(level_t velocity);

private:
  class PowerListItem {
  public:
    Sample *sample;
    float power;
  };

  std::vector<PowerListItem> samples;
  float power_max;
  float power_min;

  Channel *getMasterChannel();
  Sample *lastsample;
};

#endif/*__DRUMGIZMO_POWERLIST_H__*/
