/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            saxparser.cc
 *
 *  Tue Jul 22 16:26:22 CEST 2008
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
#include "saxparser.h"

#include <stdio.h>
#include <string.h>
#include <hugin.hpp>

static void character_hndl(void *p, const XML_Char *s, int len)
{
  SAXParser *parser = (SAXParser*)XML_GetUserData(p);
  std::string chars;
  chars.append(s, len);
  parser->characterData(chars);
}

static void start_hndl(void *p, const char *el, const char **attr)
{
  SAXParser *parser = (SAXParser*)XML_GetUserData(p);

  // Convert to comfy C++ values...
  std::string name = el;
  std::map< std::string, std::string > attributes;

  while(*attr) {
    std::string at_name = *attr;
    attr++;
    std::string at_value = *attr;
    attr++;

    attributes.insert(make_pair(at_name, at_value));
  }

  parser->startTag(name, attributes);
}

static void end_hndl(void *p, const char *el)
{
  SAXParser *parser = (SAXParser*)XML_GetUserData(p);
  std::string name = el;
  parser->endTag(name);
}


SAXParser::SAXParser()
{
  p = XML_ParserCreate(NULL);
  if(!p) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    // throw Exception(...);
    return;
  }

  XML_SetUserData(p, this);
  XML_UseParserAsHandlerArg(p);
  XML_SetElementHandler(p, start_hndl, end_hndl);
  XML_SetCharacterDataHandler(p, character_hndl);
}

SAXParser::~SAXParser()
{
  XML_ParserFree(p);
}

int SAXParser::parse()
{
  DEBUG(sax, "parse()\n");

  char buf[32];
  int len;
  
  do {
    len = readData(buf, sizeof(buf) - 1);
    if(len == -1) {
      parseError((char*)"", 0, "Could not read data", 0);
      return 1;
    }
    if(!XML_Parse(p, buf, len, len == 0)) {
      parseError(buf, len, XML_ErrorString(XML_GetErrorCode(p)),
                 (int)XML_GetCurrentLineNumber(p));
      return 1;
    }

    memset(buf, 0, sizeof(buf));
  } while(len);

  return 0;
}

int SAXParser::parse(std::string buffer)
{
  DEBUG(sax, "parse(buffer %d bytes)\n", (int)buffer.length());

  if(!XML_Parse(p, buffer.c_str(), buffer.length(), true)) {
    parseError((char*)buffer.c_str(), buffer.length(),
               XML_ErrorString(XML_GetErrorCode(p)),
               (int)XML_GetCurrentLineNumber(p));
    return 1;
  }

  return 0;
}

void SAXParser::parseError(char *buf, size_t len, std::string error, int lineno)
{
  fprintf(stderr, "SAXParser error at line %d: %s\n", lineno, error.c_str());
  fprintf(stderr, "\tBuffer %u bytes: [", (int)len);
  if(fwrite(buf, len, 1, stderr) != len) {}
  fprintf(stderr, "]\n");
  fflush(stderr);
}
