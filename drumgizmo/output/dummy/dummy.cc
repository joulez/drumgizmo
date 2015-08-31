/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiooutputenginedummy.cc
 *
 *  Sat Apr 30 21:12:02 CEST 2011
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

#include <audiotypes.h>
#include <string>

class Dummy {
public:
  Dummy();
  ~Dummy();
  bool init(int channels, char *cnames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre(size_t size);
  void run(int channel, sample_t* data, size_t size);
  void post(size_t size);

  size_t samplerate();
};

Dummy::Dummy()
{
}

Dummy::~Dummy()
{
}

bool Dummy::init(int channels, char *cnames[])
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

void Dummy::pre(size_t size)
{
}

void Dummy::run(int channel, sample_t* data, size_t size)
{
}

void Dummy::post(size_t size)
{
}

size_t Dummy::samplerate()
{
  return 44100;
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

  bool init(void *h, int cs, char *cnames[])
  {
    Dummy *dummy = (Dummy*)h;
    return dummy->init(cs, cnames);
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

  void pre(void *h, size_t size)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->pre(size);
  }

  void run(void *h, int ch, sample_t *data, size_t size)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->run(ch, data, size);
  }

  void post(void *h, size_t size)
  {
    Dummy *dummy = (Dummy*)h;
    dummy->post(size);
  }

  size_t samplerate(void *h)
  {
    Dummy *dummy = (Dummy*)h;
    return dummy->samplerate();
  }
}
