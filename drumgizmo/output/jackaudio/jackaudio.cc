/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            jackaudio.cc
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
#include <audiotypes.h>
#include <semaphore.h>
#include <stdlib.h>

#define NOTE_ON 0x90

#include "../../jackclient.h"
#include <stdio.h>

class JackAudio : public JackProcess {
public:
  JackAudio();
  ~JackAudio();
  bool init(int channels, char *cnames[]);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre(size_t size);
  void run(int channel, sample_t* data, size_t size);
  void post(size_t size);

  void jack_process(jack_nframes_t nframes);

  size_t bufsize();
  size_t samplerate();

private:
  JackClient *jackclient;
  jack_port_t *output_port[64];
  size_t nchannels;
  sample_t **channels;
  Semaphore sem;
};

JackAudio::JackAudio() : sem("jackaudio")
{
}

JackAudio::~JackAudio()
{
}

bool JackAudio::init(int nchannels, char *cnames[])
{
  this->nchannels = nchannels;
  channels = (sample_t**)malloc(nchannels * sizeof(sample_t*));

  for(int i = 0; i < nchannels; i++) {
    char buf[32];
    sprintf(buf, "%d", i + 1);
    std::string name;
    name += buf;
    name += "-";
    name += cnames[i];
    output_port[i] = jack_port_register(jackclient->jack_client,
                                        name.c_str(),
                                        JACK_DEFAULT_AUDIO_TYPE,
                                        JackPortIsOutput, 0);
    channels[i] = (sample_t*)malloc(bufsize() * sizeof(sample_t));
  }
  return true;
}

void JackAudio::setParm(std::string parm, std::string value)
{
  if(parm == "jack_client") {
    sscanf(value.c_str(), "%p", &jackclient);
    if(jackclient) jackclient->addJackProcess(this);
  }
}

bool JackAudio::start()
{
  //jackclient->activate();
  return true;
}

void JackAudio::stop()
{
}

void JackAudio::pre(size_t size)
{
}

void JackAudio::run(int channel, sample_t* data, size_t size)
{
  // Copy engine data to ringbuffer.
  for(size_t i = 0; i < size; i++) {
    channels[channel][i] = data[i];
  }
}

void JackAudio::post(size_t size)
{
  sem.wait();
}

void JackAudio::jack_process(jack_nframes_t nframes)
{
  //printf("o"); fflush(stdout);
  for(size_t c = 0; c < nchannels; c++) {
    jack_default_audio_sample_t *out =
      (jack_default_audio_sample_t *) jack_port_get_buffer(output_port[c],
                                                           nframes);
    for(size_t i = 0; i < nframes; i++) {
      out[i] = channels[c][i];
    }
  }
  sem.post();
}

size_t JackAudio::bufsize()
{
  return jack_get_buffer_size(jackclient->jack_client);
}

size_t JackAudio::samplerate()
{
  return jack_get_sample_rate(jackclient->jack_client);
}

extern "C" {
  void *create()
  {
    return new JackAudio();
  }
  
  void destroy(void *h)
  {
    JackAudio *jack = (JackAudio*)h;
    delete jack;
  }

  bool init(void *h, int cs, char *cnames[])
  {
    JackAudio *jack = (JackAudio*)h;
    return jack->init(cs, cnames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    JackAudio *jack = (JackAudio*)h;
    jack->setParm(parm, value);
  }

  bool start(void *h)
  {
    JackAudio *jack = (JackAudio*)h;
    return jack->start();
  }

  void stop(void *h)
  {
    JackAudio *jack = (JackAudio*)h;
    jack->stop();
  }

  void pre(void *h, size_t s)
  {
    JackAudio *jack = (JackAudio*)h;
    jack->pre(s);
  }

  void run(void *h, int ch, sample_t *data, size_t size)
  {
    JackAudio *jack = (JackAudio*)h;
    jack->run(ch, data, size);
  }

  void post(void *h, size_t s)
  {
    JackAudio *jack = (JackAudio*)h;
    jack->post(s);
  }

  size_t bufsize(void *h)
  {
    JackAudio *jack = (JackAudio*)h;
    return jack->bufsize();
  }

  size_t samplerate(void *h)
  {
    JackAudio *jack = (JackAudio*)h;
    return jack->samplerate();
  }
}
