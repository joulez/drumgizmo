/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audioinputenginedummy.cc
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

class Test {
public:
  Test() { p = 0.1; instr = -1; len = -1; }
  ~Test() {}

  bool init(int instruments, char *inames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();
  
  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

private:
  float p;
  int instr;
  int len;
};

bool Test::init(int instruments, char *inames[])
{
  return true;
}

void Test::setParm(std::string parm, std::string value)
{
  if(parm == "p") p = atof(value.c_str());
  if(parm == "instr") instr = atoi(value.c_str());
  if(parm == "len") len = atoi(value.c_str());
}

bool Test::start()
{
  return true;
}

void Test::stop()
{
}

void Test::pre()
{
}

event_t *Test::run(size_t pos, size_t nsamples, size_t *nevents)
{
  if((float)rand() / (float)RAND_MAX > p) {
    *nevents = 0;
    return NULL;
  }

  *nevents = 1;
  event_t *evs = (event_t *)malloc(sizeof(event_t));
  evs[0].type = TYPE_ONSET;
  if(len != -1 && (int)pos > len * 44100) evs[0].type = TYPE_STOP;

  if(instr != -1) evs[0].instrument = instr;
  else evs[0].instrument = rand() % 32;

  evs[0].velocity = (float)rand()/(float)RAND_MAX;
  evs[0].offset = nsamples?rand()%nsamples:0;
  return evs;
}

void Test::post()
{
}

extern "C" {
  void *create()
  {
    return new Test();
  }
  
  void destroy(void *h)
  {
    Test *test = (Test*)h;
    delete test;
  }

  bool init(void *h, int i, char *inames[])
  {
    Test *test = (Test*)h;
    return test->init(i, inames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    Test *test = (Test*)h;
    test->setParm(parm, value);
  }

  bool start(void *h)
  {
    Test *test = (Test*)h;
    return test->start();
  }

  void stop(void *h)
  {
    Test *test = (Test*)h;
    test->stop();
  }

  void pre(void *h)
  {
    Test *test = (Test*)h;
    test->pre();
  }

  event_t *run(void *h, size_t pos, size_t len, size_t *nev)
  {
    Test *test = (Test*)h;
    return test->run(pos, len, nev);
  }

  void post(void *h)
  {
    Test *test = (Test*)h;
    test->post();
  }
}

#ifdef TEST_AUDIOINPUTENGINETEST
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

#endif/*TEST_AUDIOINPUTENGINETEST*/
