/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            midimapparser.h
 *
 *  Mon Aug  8 16:55:30 CEST 2011
 *  Copyright 2011 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_MIDIMAPPARSER_H__
#define __DRUMGIZMO_MIDIMAPPARSER_H__

#include <stdio.h>

#include "saxparser.h"

#include "midimapper.h"

class MidiMapParser : public SAXParser {
public:
  MidiMapParser(std::string file);
  ~MidiMapParser();

  void startTag(std::string name, attr_t attr);

  midimap_t midimap;

protected:
  int readData(char *data, size_t size);

private:
  FILE *fd;
};

#endif/*__DRUMGIZMO_MIDIMAPPARSER_H__*/
