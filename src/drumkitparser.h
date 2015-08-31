/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumkitparser.h
 *
 *  Tue Jul 22 16:24:58 CEST 2008
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
#ifndef __DRUMGIZMO_DRUMKITPARSER_H__
#define __DRUMGIZMO_DRUMKITPARSER_H__

#include "saxparser.h"
#include "drumkit.h"

class DrumKitParser : public SAXParser {
public:
  DrumKitParser(const std::string &kitfile, DrumKit &kit);
  ~DrumKitParser();

  void startTag(std::string name,
                std::map< std::string, std::string> attributes);
  void endTag(std::string name);

protected:
  int readData(char *data, size_t size);

private:
  FILE *fd;
  DrumKit &kit;
  //  Instrument *instr;
  std::string path;

  std::map<std::string, std::string> channelmap;
  std::string instr_file;
  std::string instr_name;
  std::string instr_group;
};

#endif/*__DRUMGIZMO_DRUMKITPARSER_H__*/
