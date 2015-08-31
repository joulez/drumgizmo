/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            wavfile.cc
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

#include <sndfile.h>

#define T(x, msg) if(x < 0) { printf("%s failed: %s\n", msg, snd_strerror(x)); fflush(stdout); return false; }

class WavFile {
public:
  WavFile();
  ~WavFile();
  bool init(int channels, char *cnames[]);
  void setParm(std::string parm, std::string value);
  bool start();
  void stop();
  void pre(size_t size);
  void run(int channel, sample_t* data, size_t size);
  void post(size_t size);
  size_t samplerate();

private:
  SF_INFO sf_info;
  SNDFILE **fh;
  size_t channels;

  // Parameters
  std::string filename;
};

WavFile::WavFile()
{
  fh = NULL;
  filename = "output";

  sf_info.channels = 1;//channels;
  sf_info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  sf_info.samplerate = 44100;
}

WavFile::~WavFile()
{
  if(fh == NULL) return;

  for(size_t i = 0; i < channels; i++) {
    if(fh[i]) sf_close(fh[i]);
  }

  if(fh) free(fh);
}

bool WavFile::init(int channels, char *cnames[])
{
  this->channels = channels;

  fh = (SNDFILE **)malloc(sizeof(SNDFILE *)*channels);

  for(size_t i = 0; i < this->channels; i++) fh[i] = NULL;

  for(size_t i = 0; i < this->channels; i++) {
    char fname[512];

    sprintf(fname, "%s%s-%d.wav", filename.c_str(), cnames[i], (int)i);
    //    printf("[%s]\n", fname);

    fh[i] = sf_open(fname, SFM_WRITE, &sf_info);
    if(!fh[i]) {
      printf("Write error...\n");
      return false;
    }
  }

  return true;
}

void WavFile::setParm(std::string parm, std::string value)
{
  if(parm == "file") filename = value;
  if(parm == "srate") sf_info.samplerate = atoi(value.c_str());
}

bool WavFile::start()
{
  return true;
}

void WavFile::stop()
{
}

void WavFile::pre(size_t size)
{
}

void WavFile::run(int channel, sample_t* cdata, size_t csize)
{
  if(channel < (int)channels) sf_writef_float(fh[channel], cdata, csize); 
}

void WavFile::post(size_t size)
{
}

size_t WavFile::samplerate()
{
  return sf_info.samplerate;
}

extern "C" {
  void *create()
  {
    return new WavFile();
  }
  
  void destroy(void *h)
  {
    WavFile *sndfile = (WavFile*)h;
    delete sndfile;
  }

  bool init(void *h, int cs, char *cnames[])
  {
    WavFile *sndfile = (WavFile*)h;
    return sndfile->init(cs, cnames);
  }

  void setparm(void *h, const char *parm, const char *value)
  {
    WavFile *sndfile = (WavFile*)h;
    sndfile->setParm(parm, value);
  }

  bool start(void *h)
  {
    WavFile *sndfile = (WavFile*)h;
    return sndfile->start();
  }

  void stop(void *h)
  {
    WavFile *sndfile = (WavFile*)h;
    sndfile->stop();
  }

  void pre(void *h, size_t s)
  {
    WavFile *sndfile = (WavFile*)h;
    sndfile->pre(s);
  }

  void run(void *h, int ch, sample_t *data, size_t size)
  {
    WavFile *sndfile = (WavFile*)h;
    sndfile->run(ch, data, size);
  }

  void post(void *h, size_t s)
  {
    WavFile *sndfile = (WavFile*)h;
    sndfile->post(s);
  }

  size_t samplerate(void *h)
  {
    WavFile *wavfile = (WavFile*)h;
    return wavfile->samplerate();
  }
}
