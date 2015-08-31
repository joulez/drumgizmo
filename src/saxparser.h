/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            saxparser.h
 *
 *  Tue Jul 22 16:26:21 CEST 2008
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
#ifndef __DRUMGIZMO_SAXPARSER_H__
#define __DRUMGIZMO_SAXPARSER_H__

#include <string>
#include <map>
#include <expat.h>

typedef std::map< std::string, std::string> attr_t;

class SAXParser {
public:
  SAXParser();
  virtual ~SAXParser();

  int parse();
  int parse(std::string buffer);
  
  virtual void characterData(std::string &data) {}
  virtual void startTag(std::string name, attr_t attr) {}
  virtual void endTag(std::string name) {}

  virtual void parseError(char *buf, size_t len, std::string error, int lineno);

protected:
  virtual int readData(char *data, size_t size) { return 0; }

private:
  XML_Parser p;
};

#endif/*__DRUMGIZMO_SAXPARSER_H__*/
