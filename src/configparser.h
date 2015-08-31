/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            configparser.h
 *
 *  Sat Jun 29 21:55:02 CEST 2013
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
#ifndef __DRUMGIZMO_CONFIGPARSER_H__
#define __DRUMGIZMO_CONFIGPARSER_H__

#include <map>

#include "saxparser.h"

class ConfigParser : public SAXParser {
public:
  ConfigParser();

  void characterData(std::string &data);
  void startTag(std::string name, attr_t attr);
  void endTag(std::string name);
  std::string value(std::string name, std::string def = "");
  void parseError(char *buf, size_t len, std::string error, int lineno);

private:
  std::map<std::string, std::string> values;
  std::string *str;
};

#endif/*__DRUMGIZMO_CONFIGPARSER_H__*/
