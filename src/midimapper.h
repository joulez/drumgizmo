/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            midimapper.h
 *
 *  Mon Jul 21 15:24:07 CEST 2008
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
#ifndef __DRUMGIZMO_MIDIMAPPER_H__
#define __DRUMGIZMO_MIDIMAPPER_H__

#include <map>
#include <string>

typedef std::map<int, std::string> midimap_t;
typedef std::map<std::string, int> instrmap_t;

class MidiMapper {
public:
  void clear();

  int lookup(int note);

  instrmap_t instrmap;
  midimap_t midimap;
};

#endif/*__DRUMGIZMO_MIDIMAPPER_H__*/
