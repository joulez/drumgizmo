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

class Dummy {
public:
  Dummy() {}
  ~Dummy() {}

  bool init(int instruments, char *inames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();
  
  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();
};

bool Dummy::init(int instruments, char *inames[])
{
  return true;
}

void Dummy::setParm(std::string parm, std::string value)
{
}

bool Dummy::start()
{
  return true;
}

void Dummy::stop()
{
}

void Dummy::pre()
{
}

event_t *Dummy::run(size_t pos, size_t len, size_t *nevents)
{
  *nevents = 0;
  return NULL;
  /*
  if(rand() % 10 != 0) return;

  Instrument *i = NULL;
  int d = rand() % drumkit->instruments.size();
  Instruments::iterator it = drumkit->instruments.begin();
  while(d--) {
    i = &(it->second);
    it++;
  }

  if(i == NULL) return;

  Sample *s = i->sample((double)rand()/(double)RAND_MAX);
    
  if(s == NULL) {
    printf("Missing Sample.\n");
    //    continue;
  }

  Channels::iterator j = drumkit->channels.begin();
  while(j != drumkit->channels.end()) {
    Channel &ch = *j;
    AudioFile *af = s->getAudioFile(&ch);
    if(af == NULL) {
      printf("Missing AudioFile.\n");
    } else {
      printf("Adding event.\n");
      Event *evt = new EventSample(ch.num, 1.0, af);
      eventqueue->post(evt, pos);
    }
    j++;
  }
  */
}

void Dummy::post()
{
}

extern "C" {
  void *create()
  {
    return new Dummy();
  }
  
  void destroy(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    delete dummy;
  }

  bool init(void *h, int i, char *inames[])
  {
    Dummy *dummy = (Dummy*)h;
    return dummy->init(i, inames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->setParm(parm, value);
  }

  bool start(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    return dummy->start();
  }

  void stop(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->stop();
  }

  void pre(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->pre();
  }

  event_t *run(void *h, size_t pos, size_t len, size_t *nev)
  {
    Dummy *dummy = (Dummy*)h;
    return dummy->run(pos, len, nev);
  }

  void post(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->post();
  }
}

#ifdef TEST_AUDIOINPUTENGINEDUMMY
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

#endif/*TEST_AUDIOINPUTENGINEDUMMY*/
