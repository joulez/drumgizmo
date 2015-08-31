/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            instrumentparser.h
 *
 *  Wed Mar  9 13:22:24 CET 2011
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
#ifndef __DRUMGIZMO_INSTRUMENTPARSER_H__
#define __DRUMGIZMO_INSTRUMENTPARSER_H__

#include "saxparser.h"
#include "instrument.h"

#include <vector>

class InstrumentParser : public SAXParser {
public:
  InstrumentParser(const std::string &instrfile, Instrument &instrument);
  ~InstrumentParser();

  void startTag(std::string name,
                std::map< std::string, std::string> attributes);
  void endTag(std::string name);

  std::vector<InstrumentChannel *> channellist;

protected:
  int readData(char *data, size_t size);

private:
  FILE *fd;
  Instrument &instrument;
  Sample *s;

  std::string path;

  level_t lower;
  level_t upper;
};

#endif/*__DRUMGIZMO_INSTRUMENTPARSER_H__*/
