/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            jackmidi.cc
 *
 *  Sat Apr 30 21:11:54 CEST 2011
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
#include <stdlib.h>

#include <event.h>
#include <string>

#include <stdio.h>
#include <midimapper.h>
#include <midimapparser.h>

#define NOTE_ON 0x90

#include "../../jackclient.h"

#include <jack/midiport.h>

class JackMidi : public JackProcess {
public:
  JackMidi();
  virtual ~JackMidi();

  bool init(int instruments, char *inames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

  void jack_process(jack_nframes_t nframes);

private:
  void loadMap(std::string map) {}

  JackClient *jackclient;
  jack_port_t *midi_port;

  size_t pos;

  event_t *list;
  size_t listsize;

  std::string midimapfile;
  MidiMapper mmap;
};

JackMidi::JackMidi()
{
  pos = 0;

  list = (event_t *)malloc(sizeof(event_t) * 1000);
  listsize = 0;
}

JackMidi::~JackMidi()
{
}

bool JackMidi::init(int instruments, char *inames[])
{
	midi_port = jack_port_register(jackclient->jack_client,
                                 "drumgizmo_midiin",
                                 JACK_DEFAULT_MIDI_TYPE,
                                 JackPortIsInput,// | JackPortIsTerminal,
                                 0);

  MidiMapParser p(midimapfile);
  if(p.parse()) return false;
  mmap.midimap = p.midimap;

  for(int i = 0; i < instruments; i++) {
    mmap.instrmap[inames[i]] = i;
  }

  return true;
}

void JackMidi::setParm(std::string parm, std::string value)
{
  if(parm == "midimap") midimapfile = value;
  if(parm == "jack_client") {
    sscanf(value.c_str(), "%p", &jackclient);
    if(jackclient) jackclient->addJackProcess(this);
  }
}

bool JackMidi::start()
{
  // jackclient->activate();
  return true;
}

void JackMidi::stop()
{
}

void JackMidi::pre()
{
}

event_t *JackMidi::run(size_t pos, size_t len, size_t *nevents)
{
  *nevents = listsize;
  event_t *l = list;
  list = (event_t *)malloc(sizeof(event_t) * 1000);
  listsize = 0;
  return l;
}

void JackMidi::jack_process(jack_nframes_t nframes)
{
  // printf("i"); fflush(stdout);

  void *midibuffer = jack_port_get_buffer(midi_port, nframes);

  jack_nframes_t midievents = jack_midi_get_event_count(midibuffer);

  for(jack_nframes_t i = 0; i < midievents; i++) {
    jack_midi_event_t event;
    jack_midi_event_get(&event, midibuffer, i);
    
    if(event.size != 3) continue;
    if((event.buffer[0] & NOTE_ON) != NOTE_ON) continue;
    
    int key = event.buffer[1];
    int velocity = event.buffer[2];
    
    printf("Event key:%d vel:%d\n", key, velocity);

    int k = mmap.lookup(key);
    if(k != -1) {

      if(velocity) {
        list[listsize].type = TYPE_ONSET;
        list[listsize].instrument = k;
        list[listsize].velocity = velocity / 127.0;
        list[listsize].offset = event.time;
        listsize++;
      }
    }
  }
  
  jack_midi_clear_buffer(midibuffer);
    
  pos += nframes;
}

void JackMidi::post()
{
}

extern "C" {
  void *create()
  {
    return new JackMidi();
  }
  
  void destroy(void *h)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    delete jackmidi;
  }

  bool init(void *h, int i, char *inames[])
  {
    JackMidi *jackmidi = (JackMidi*)h;
    return jackmidi->init(i, inames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    jackmidi->setParm(parm, value);
  }

  bool start(void *h)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    return jackmidi->start();
  }

  void stop(void *h)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    jackmidi->stop();
  }

  void pre(void *h)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    jackmidi->pre();
  }

  event_t *run(void *h, size_t pos, size_t len, size_t *nev)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    return jackmidi->run(pos, len, nev);
  }

  void post(void *h)
  {
    JackMidi *jackmidi = (JackMidi*)h;
    jackmidi->post();
  }
}

#ifdef TEST_AUDIOINPUTENGINEJACKMIDI
//Additional dependency files
//deps:
//Required cflags (autoconf vars may be used)
//cflags:
//Required link options (autoconf vars may be used)
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).

TEST_END;

#endif/*TEST_AUDIOINPUTENGINEJACKMIDI*/
