/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            dummy.cc
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

#include <string>
#include <event.h>

#include <smf.h>

#include <midimapper.h>
#include <midimapparser.h>

#define NOTE_ON 0x90

class MidiFile {
public:
  MidiFile();
  ~MidiFile() {}

  bool init(int instruments, char *inames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();
  
  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

private:
  smf_t *smf;
  smf_event_t *cur_event;

  MidiMapper mmap;

  // Parameters
  std::string filename;
  float speed;
  int track;
  std::string midimapfile;
  bool loop;
  double offset;
};

MidiFile::MidiFile()
{
 cur_event = NULL;
 smf = NULL;
 
 speed = 1.0;
 track = -1; // -1 is OMNI/all tracks
 loop = false;
 offset = 0;
}
 
bool MidiFile::init(int instruments, char *inames[])
{
  if(filename == "") {
    fprintf(stderr, "Missing midifile argument 'file'\n");
    return false;
  }

  if(midimapfile == "") {
    fprintf(stderr, "Missing midimapfile argument 'midimap'.\n");
    return false;
  }

  smf = smf_load(filename.c_str());

  if(!smf) {
    fprintf(stderr, "Could not open midifile '%s'.\n", filename.c_str());
    return false;
  }

  MidiMapParser p(midimapfile);
  if(p.parse()) {
    fprintf(stderr, "Could not parse midimapfile '%s'.\n", midimapfile.c_str());
    return false;
  }
  mmap.midimap = p.midimap;

  for(int i = 0; i < instruments; i++) {
    mmap.instrmap[inames[i]] = i;
  }

  return true;
}

void MidiFile::setParm(std::string parm, std::string value)
{
  if(parm == "file") filename = value;
  if(parm == "speed") speed = atof(value.c_str());
  if(parm == "track") track = atoi(value.c_str());
  if(parm == "midimap") midimapfile = value;
  if(parm == "loop") loop = true;
}

bool MidiFile::start()
{
  return true;
}

void MidiFile::stop()
{
}

void MidiFile::pre()
{
}

event_t *MidiFile::run(size_t pos, size_t len, size_t *nevents)
{
  event_t *evs = NULL;
  size_t nevs = 0;

  double cur_max_time = (double)(pos + len) / (44100.0 / speed);
  cur_max_time -= offset;
  //  double cur_min_time = (double)(pos) / (44100.0 / speed);

  if(!cur_event) cur_event = smf_get_next_event(smf);

  while(cur_event && cur_event->time_seconds < cur_max_time) {
    if(!smf_event_is_metadata(cur_event)) {
      if( (cur_event->midi_buffer_length == 3) &&
          ((cur_event->midi_buffer[0] & NOTE_ON) == NOTE_ON) &&
          (track == -1 || cur_event->track_number == track) &&
          cur_event->midi_buffer[2] > 0) {
        
        if(evs == NULL) evs = (event_t *)malloc(sizeof(event_t) * 1000);

        int key = cur_event->midi_buffer[1];
        int velocity = cur_event->midi_buffer[2];

        evs[nevs].type = TYPE_ONSET;
        size_t evpos = cur_event->time_seconds * (44100.0 / speed);
        evs[nevs].offset = evpos - pos;

        int i = mmap.lookup(key);
        if(i != -1) {
          evs[nevs].instrument = i;
          evs[nevs].velocity = velocity / 127.0;
        
          nevs++;
          if(nevs > 999) {
            fprintf(stderr, "PANIC!\n");
            break;
          }
        }
      }
    }
    
    cur_event = smf_get_next_event(smf);
  }

  if(!cur_event) {
    if(loop) {
       smf_rewind(smf);
       offset += cur_max_time;
    } else {
      if(evs == NULL) evs = (event_t *)malloc(sizeof(event_t) * 1000);
      evs[nevs].type = TYPE_STOP;
      evs[nevs].offset = len - 1;
      nevs++;
    }
  }

  *nevents = nevs;

  return evs;
}

void MidiFile::post()
{
}

extern "C" {
  void *create()
  {
    return new MidiFile();
  }
  
  void destroy(void *h)
  {
    MidiFile *midifile = (MidiFile*)h;
    delete midifile;
  }

  bool init(void *h, int i, char *inames[])
  {
    MidiFile *midifile = (MidiFile*)h;
    return midifile->init(i, inames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    MidiFile *midifile = (MidiFile*)h;
    midifile->setParm(parm, value);
  }

  bool start(void *h)
  {
    MidiFile *midifile = (MidiFile*)h;
    return midifile->start();
  }

  void stop(void *h)
  {
    MidiFile *midifile = (MidiFile*)h;
    midifile->stop();
  }

  void pre(void *h)
  {
    MidiFile *midifile = (MidiFile*)h;
    midifile->pre();
  }

  event_t *run(void *h, size_t pos, size_t len, size_t *nev)
  {
    MidiFile *midifile = (MidiFile*)h;
    return midifile->run(pos, len, nev);
  }

  void post(void *h)
  {
    MidiFile *midifile = (MidiFile*)h;
    midifile->post();
  }
}

#ifdef TEST_AUDIOINPUTENGINEMIDIFILE
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

#endif/*TEST_AUDIOINPUTENGINEMIDIFILE*/
