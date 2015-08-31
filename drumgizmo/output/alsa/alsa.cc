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

// Use the newer ALSA API
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <asoundlib.h>

#define T(x, msg) if(x < 0) { printf("%s failed: %s\n", msg, snd_strerror(x)); fflush(stdout); return false; }

#define BUFSZ 40960

class Alsa {
public:
  Alsa();
  ~Alsa();
  bool init(int channels, char *cnames[]);
  void setParm(std::string parm, std::string value);
  bool start();
  void stop();
  void pre(size_t size);
  void run(int channel, sample_t* data, size_t size);
  void post(size_t size);
  size_t samplerate();

private:
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  sample_t *data;
  size_t channels;

  // Parameters
  std::string device;
  unsigned int srate;
  snd_pcm_uframes_t frames;
};

Alsa::Alsa()
{
  handle = NULL;
  data = NULL;

  device = "default";
  srate = 44100;
  frames = 32;
}

Alsa::~Alsa()
{
  if(handle) snd_pcm_close(handle);
  if(data) free(data);
}

bool Alsa::init(int channels, char *cnames[])
{
  int rc;

  rc = snd_pcm_open(&handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0);
  T(rc, "snd_pcm_open");

  this->channels = channels;
  if(!handle) {
    printf("No handle!\n");
    return false;
  }

  // Allocate a hardware parameters object.
  snd_pcm_hw_params_alloca(&params);
  //  if(rc < 0) return false;

  // Fill it in with default values.
  rc = snd_pcm_hw_params_any(handle, params);
  T(rc, "snd_pcm_hw_params_any");

  rc = snd_pcm_hw_params_set_access(handle, params,
                                    SND_PCM_ACCESS_RW_INTERLEAVED);
  T(rc, "snd_pcm_hw_params_set_access");

  rc = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT);
  T(rc, "snd_pcm_hw_params_set_format");

  rc = snd_pcm_hw_params_set_channels(handle, params, channels);
  T(rc, "snd_pcm_hw_params_set_channels");

  rc = snd_pcm_hw_params_set_rate_near(handle, params, &srate, 0);
  T(rc, "snd_pcm_hw_params_set_rate_near");

  rc = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, 0);
  T(rc, "snd_pcm_hw_params_set_period_size_near");
  
  rc = snd_pcm_hw_params(handle, params);
  T(rc, "snd_pcm_hw_params");

  data = (sample_t*)malloc(sizeof(sample_t) * BUFSZ * channels);

  return true;
}

void Alsa::setParm(std::string parm, std::string value)
{
  if(parm == "dev") device = value;
  if(parm == "frames") frames = atoi(value.c_str());
  if(parm == "srate") srate = atoi(value.c_str());
}

bool Alsa::start()
{
  return true;
}

void Alsa::stop()
{
}

void Alsa::pre(size_t size)
{
}

void Alsa::run(int channel, sample_t* cdata, size_t csize)
{
  // Write channel data in interleaved buffer.
  for(size_t i = 0; i < csize; i++) {
    data[i * channels + channel] = cdata[i];
  }
}

void Alsa::post(size_t size)
{
  // Write the interleaved buffer to the soundcard
  snd_pcm_writei(handle, data, size);
}

size_t Alsa::samplerate()
{
  return srate;
}

extern "C" {
  void *create()
  {
    return new Alsa();
  }
  
  void destroy(void *h)
  {
    Alsa *alsa = (Alsa*)h;
    delete alsa;
  }

  bool init(void *h, int cs, char *cnames[])
  {
    Alsa *alsa = (Alsa*)h;
    return alsa->init(cs, cnames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    Alsa *alsa = (Alsa*)h;
    alsa->setParm(parm, value);
  }

  bool start(void *h)
  {
    Alsa *alsa = (Alsa*)h;
    return alsa->start();
  }

  void stop(void *h)
  {
    Alsa *alsa = (Alsa*)h;
    alsa->stop();
  }

  void pre(void *h, size_t s)
  {
    Alsa *alsa = (Alsa*)h;
    alsa->pre(s);
  }

  void run(void *h, int ch, sample_t *data, size_t size)
  {
    Alsa *alsa = (Alsa*)h;
    alsa->run(ch, data, size);
  }

  void post(void *h, size_t s)
  {
    Alsa *alsa = (Alsa*)h;
    alsa->post(s);
  }

  size_t samplerate(void *h)
  {
    Alsa *alsa = (Alsa*)h;
    return alsa->samplerate();
  }
}
