/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            beatmapper.h
 *
 *  Fri Jul 25 11:17:42 CEST 2008
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
#ifndef __DRUMGIZMO_BEATMAPPER_H__
#define __DRUMGIZMO_BEATMAPPER_H__

#include <jack/jack.h>

#include "instrument.h"
#include "event.h"

#include <map>

#define HISTORY_SIZE 200

class BeatMapper {
public:
  BeatMapper(Instrument *instrument);

  Sample *map(jack_nframes_t nframes);

private:
  Instrument *instrument;
  float hist[HISTORY_SIZE];
  float C;

  size_t mindist;
  size_t last;
};

#endif/*__DRUMGIZMO_BEATMAPPER_H__*/
