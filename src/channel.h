/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            channel.h
 *
 *  Tue Jul 22 17:14:27 CEST 2008
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
#ifndef __DRUMGIZMO_CHANNEL_H__
#define __DRUMGIZMO_CHANNEL_H__

#include <vector>
#include <string>

#include "audio.h"

#define ALL_CHANNELS ((channel_t)0xffffffff)
#define NO_CHANNEL ((channel_t)0xfffffffe)

class Channel {
public:
  Channel(std::string name = "");

  std::string name;
  channel_t num;
};

typedef Channel InstrumentChannel;

typedef std::vector< Channel > Channels;

#endif/*__DRUMGIZMO_CHANNEL_H__*/
