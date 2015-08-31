/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumkitparser.cc
 *
 *  Tue Jul 22 16:24:59 CEST 2008
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
#include "drumkitparser.h"

#include <string.h>
#include <stdio.h>
#include <hugin.hpp>

#include "instrumentparser.h"
#include "path.h"

DrumKitParser::DrumKitParser(const std::string &kitfile, DrumKit &k)
  : kit(k)
{
  //  instr = NULL;
  path = getPath(kitfile);

  fd = fopen(kitfile.c_str(), "r");

  //  DEBUG(kitparser, "Parsing drumkit in %s\n", kitfile.c_str());

  if(!fd) return;

  kit._file = kitfile;
}

DrumKitParser::~DrumKitParser()
{
  if(fd) fclose(fd);
}

void DrumKitParser::startTag(std::string name,
                             std::map<std::string, std::string> attr)
{
  if(name == "drumkit") {
    if(attr.find("name") != attr.end())
      kit._name = attr["name"];

    if(attr.find("samplerate") != attr.end()) {
      kit._samplerate = atoi(attr["samplerate"].c_str());
    } else {
      // If 'samplerate' attribute is missing, assume 44k1Hz
      // TODO: Ask instrument what samplerate is in the audiofiles...
      kit._samplerate = 44100;
    }

    if(attr.find("description") != attr.end())
      kit._description = attr["description"];

    if(attr.find("version") != attr.end()) {
      try {
        kit._version = VersionStr(attr["version"]);
      } catch(const char *err) {
        ERR(kitparser, "Error parsing version number: %s, using 1.0\n", err);
        kit._version = VersionStr(1,0,0);
      } 
    } else {
      WARN(kitparser, "Missing version number, assuming 1.0\n");
      kit._version = VersionStr(1,0,0);
    }
  }

  if(name == "channels") {}

  if(name == "channel") {
    if(attr.find("name") == attr.end()) {
      DEBUG(kitparser, "Missing channel name.\n");
      return;
    }
    Channel c(attr["name"]);
    c.num = kit.channels.size();
    kit.channels.push_back(c);
  }

  if(name == "instruments") {
  }

  if(name == "instrument") {
    if(attr.find("name") == attr.end()) {
      DEBUG(kitparser, "Missing name in instrument tag.\n");
      return;
    }
    if(attr.find("file") == attr.end()) {
      DEBUG(kitparser, "Missing file in instrument tag.\n");
      return;
    }

    instr_name = attr["name"];
    instr_file = attr["file"];
    if(attr.find("group") != attr.end()) instr_group = attr["group"];
    else instr_group = "";
  }

  if(name == "channelmap") {
    if(attr.find("in") == attr.end()) {
      DEBUG(kitparser, "Missing 'in' in channelmap tag.\n");
      return;
    }

    if(attr.find("out") == attr.end()) {
      DEBUG(kitparser, "Missing 'out' in channelmap tag.\n");
      return;
    }

    channelmap[attr["in"]] = attr["out"];
  }
}

void DrumKitParser::endTag(std::string name)
{
  if(name == "instrument") {
    Instrument *i = new Instrument();
    i->setGroup(instr_group);
    //    Instrument &i = kit.instruments[kit.instruments.size() - 1];
    InstrumentParser parser(path + "/" + instr_file, *i);
    parser.parse();
    kit.instruments.push_back(i);

    // Assign kit channel numbers to instruments channels.
    std::vector<InstrumentChannel*>::iterator ic = parser.channellist.begin();
    while(ic != parser.channellist.end()) {
      InstrumentChannel *c = *ic;

      std::string cname = c->name;
      if(channelmap.find(cname) != channelmap.end()) cname = channelmap[cname];

      for(size_t cnt = 0; cnt < kit.channels.size(); cnt++) {
        if(kit.channels[cnt].name == cname) c->num = kit.channels[cnt].num;
      }
      if(c->num == NO_CHANNEL) {
        DEBUG(kitparser, "Missing channel '%s' in instrument '%s'\n",
               c->name.c_str(), i->name().c_str());
      } else {
        /*
          DEBUG(kitparser, "Assigned channel '%s' to number %d in instrument '%s'\n",
             c->name.c_str(), c->num, i.name().c_str());
        */
      }
      ic++;
    }

    channelmap.clear();

  }
}

int DrumKitParser::readData(char *data, size_t size)
{
  if(!fd) return -1;
  return fread(data, 1, size, fd);
}

#ifdef TEST_DRUMKITPARSER
//deps: drumkit.cc saxparser.cc instrument.cc sample.cc audiofile.cc channel.cc
//cflags: $(EXPAT_CFLAGS) $(SNDFILE_CFLAGS)
//libs: $(EXPAT_LIBS) $(SNDFILE_LIBS)
#include "test.h"

const char xml[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<drumkit name=\"The Aasimonster\"\n"
"         description=\"A large deathmetal drumkit\">\n"
"  <channels>\n"
"    <channel name=\"Alesis\"/>\n"
"    <channel name=\"Kick-L\"/>\n"
"    <channel name=\"Kick-R\"/>\n"
"    <channel name=\"SnareTop\"/>\n"
"    <channel name=\"SnareTrigger\"/>\n"
"    <channel name=\"SnareBottom\"/>\n"
"    <channel name=\"OH-L\"/>\n"
"    <channel name=\"OH-R\"/>\n"
"    <channel name=\"Hihat\"/>\n"
"    <channel name=\"Ride\"/>\n"
"    <channel name=\"Tom1\"/>\n"
"    <channel name=\"Tom2\"/>\n"
"    <channel name=\"Tom3\"/>\n"
"    <channel name=\"Tom4\"/>\n"
"    <channel name=\"Amb-R\"/>\n"
"    <channel name=\"Amb-L\"/>\n"
"  </channels>\n"
"  <instruments>\n"
"    <instrument name=\"Ride\" file=\"ride.xml\">\n"
"      <channelmap in=\"Alesis\" out=\"Alesis\" gain=\"1.5\"/>\n"
"      <channelmap in=\"Kick-L\" out=\"Kick-L\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Kick-R\" out=\"Kick-R\"/>\n"
"      <channelmap in=\"SnareTop\" out=\"SnareTop\" gain=\"0.5\"/>\n"
"      <channelmap in=\"SnareTrigger\" out=\"SnareTrigger\" gain=\"0.5\"/>\n"
"      <channelmap in=\"SnareBottom\" out=\"SnareBottom\" gain=\"0.5\"/>\n"
"      <channelmap in=\"OH-L\" out=\"OH-L\"/>\n"
"      <channelmap in=\"OH-R\" out=\"OH-R\"/>\n"
"      <channelmap in=\"Hihat\" out=\"Hihat\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Ride\" out=\"Ride\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom1\" out=\"Tom1\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom2\" out=\"Tom2\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom3\" out=\"Tom3\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom4\" out=\"Tom4\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Amb-R\" out=\"Amb-R\"/>\n"
"      <channelmap in=\"Amb-L\" out=\"Amb-L\"/>\n"
"    </instrument>\n"
"    <instrument name=\"Snare\" file=\"snare.xml\">\n"
"      <channelmap in=\"Alesis\" out=\"Alesis\" gain=\"1.5\"/>\n"
"      <channelmap in=\"Kick-L\" out=\"Kick-L\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Kick-R\" out=\"Kick-R\"/>\n"
"      <channelmap in=\"SnareTop\" out=\"SnareTop\" gain=\"0.5\"/>\n"
"      <channelmap in=\"SnareTrigger\" out=\"SnareTrigger\" gain=\"0.5\"/>\n"
"      <channelmap in=\"SnareBottom\" out=\"SnareBottom\" gain=\"0.5\"/>\n"
"      <channelmap in=\"OH-L\" out=\"OH-L\"/>\n"
"      <channelmap in=\"OH-R\" out=\"OH-R\"/>\n"
"      <channelmap in=\"Hihat\" out=\"Hihat\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Ride\" out=\"Ride\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom1\" out=\"Tom1\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom2\" out=\"Tom2\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom3\" out=\"Tom3\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Tom4\" out=\"Tom4\" gain=\"0.5\"/>\n"
"      <channelmap in=\"Amb-R\" out=\"Amb-R\"/>\n"
"      <channelmap in=\"Amb-L\" out=\"Amb-L\"/>\n"
"    </instrument>\n"
"  </instruments>\n"
"</drumkit>\n"
  ;

#define FNAME "/tmp/drumkittest.xml"

TEST_BEGIN;

FILE *fp = fopen(FNAME, "w");
fprintf(fp, "%s", xml);
fclose(fp);

DrumKit kit;
DrumKitParser p(FNAME, kit);
TEST_EQUAL_INT(p.parse(), 0, "Parsing went well?");

TEST_EQUAL_STR(kit.name(), "The Aasimonster", "Compare name");
TEST_EQUAL_INT(kit.instruments.size(), 2, "How many instruments?");

unlink(FNAME);

TEST_END;

#endif/*TEST_DRUMKITPARSER*/
