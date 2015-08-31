/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiofile.cc
 *
 *  Tue Jul 22 17:14:11 CEST 2008
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
 *
 *  Multichannel feature by John Hammen copyright 2014
 */
#include "audiofile.h"

#include <config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sndfile.h>
#include <hugin.hpp>

#include "configuration.h"

AudioFile::AudioFile(std::string filename, int filechannel)
{
  is_loaded = false;
  this->filename = filename;
  this->filechannel = filechannel;

  data = NULL;
  size = 0;

#ifdef LAZYLOAD
  preloaded_data = NULL;
#endif/*LAZYLOAD*/

  magic = this;
}

AudioFile::~AudioFile()
{
  magic = NULL;
  unload();
}

bool AudioFile::isValid()
{
  return this == magic;
}

void AudioFile::unload()
{
  // Make sure we don't unload the object while loading it...
  MutexAutolock l(mutex);

  is_loaded = false;

#ifdef LAZYLOAD
  if(data == preloaded_data) {
    delete[] data;
    data = NULL;
    size = 0;
  } else {
    size = 0;
    delete[] data;
    data = NULL;
    delete preloaded_data;
    preloaded_data = NULL;
  }
#else
  delete[] data;
  data = NULL;
  size = 0;
#endif/*LAZYLOAD*/
}

#define	BUFFER_SIZE	4092

void AudioFile::load(int num_samples)
{
  // Make sure we don't unload the object while loading it...
  MutexAutolock l(mutex);

 /*
  Lazy load of drum kits
  init();
  return;
  */

  if(data) return;

  SF_INFO sf_info;
  SNDFILE *fh = sf_open(filename.c_str(), SFM_READ, &sf_info);
  if(!fh) {
    ERR(audiofile,"SNDFILE Error (%s): %s\n",
        filename.c_str(), sf_strerror(fh));
    return;
  }
 
  size = sf_info.frames;

  double ratio = (double)Conf::samplerate / (double)sf_info.samplerate;

  if(num_samples != ALL_SAMPLES) {
    // Make sure we read enough samples, even after conversion.
    num_samples /= ratio;
    if((int)size > num_samples) size = num_samples;
  }

  sample_t* data = new sample_t[size]; 
  if(sf_info.channels == 1) {
    size = sf_read_float(fh, data, size);
  }
  else {
    // check filechannel exists
    if(filechannel >= sf_info.channels) {
        filechannel = sf_info.channels - 1;
    }
    sample_t buffer[BUFFER_SIZE];
    int readsize = BUFFER_SIZE / sf_info.channels;
    int totalread = 0;
    int read;
    do {
      read = sf_readf_float(fh, buffer, readsize);
      for (int i = 0; i < read; i++) {
        data[totalread++] = buffer[i * sf_info.channels + filechannel];
      }
    } while(read > 0 && totalread < (int)size);
    // set data size to total bytes read
    size = totalread;
  }
  
  DEBUG(audiofile,"Loaded %d samples %p\n", (int)size, this);
  
  sf_close(fh);

  this->data = data;
  is_loaded = true;

  //DEBUG(audiofile, "Loading of %s completed.\n", filename.c_str());
}

bool AudioFile::isLoaded()
{
  return is_loaded;
}

#ifdef LAZYLOAD
#define SIZE 512*4 
void AudioFile::init()
{
  //DEBUG(audiofile,"Initializing %p\n", this);
  if(data) { 
    //DEBUG(audiofile,"\t already initialized\n");
    return;
  }

  SF_INFO sf_info;
  SNDFILE *fh = sf_open(filename.c_str(), SFM_READ, &sf_info);
  if(!fh) {
    ERR(audiofile,"SNDFILE Error (%s): %s\n",
        filename.c_str(), sf_strerror(fh));
    return;
  }
 
  int size = SIZE;

  sample_t* data = new sample_t[size];
  
  size = sf_read_float(fh, data, size); 

  //DEBUG(audiofile,"Lazy loaded %d samples\n", size);
  sf_close(fh);

  mutex.lock();
  this->data = data;
  this->size = size;
  this->preloaded_data = data;
  this->is_loaded = true;
  mutex.unlock();
}

void AudioFile::loadNext()
{
  if(this->data != this->preloaded_data) {
    //DEBUG(audiofile,"Already completely loaded %p\n", this);
    return;
  }

  SF_INFO sf_info;
  SNDFILE *fh = sf_open(filename.c_str(), SFM_READ, &sf_info);
  if(!fh) {
    ERR(audiofile,"SNDFILE Error (%s): %s\n",
        filename.c_str(), sf_strerror(fh));
    return;
  }

  int r;
//  int size_accum = 0;
  sample_t* data = new sample_t[sf_info.frames];
  memcpy(data, this->preloaded_data, this->size * sizeof(sample_t));
  this->data = data;
  sf_seek(fh, this->size, SEEK_SET);
//  sample_t* data_buf = new sample_t[SIZE];
  while(this->size < sf_info.frames) {
    //DEBUG(audiofile,"Accumulated %d of %llu\n", size_accum, sf_info.frames);
    //if( (r = sf_read_float(fh, data_buf, SIZE)) < 0) {
    if( (r = sf_read_float(fh, &data[this->size], SIZE)) < 0) {
      ERR(audiofile,"Error reading sound file\n");
      break;
    }
    //size_accum += r;
    //memcpy(data+size_accum, data_buf, sizeof(sample_t) * r);
    this->size += r;
  }
  //delete data_buf;
  
  //DEBUG(audiofile,"Finished loading %d samples %p\n", size, this);
  sf_close(fh);

  //mutex.lock();
  //this->data = data;
  //this->size = size;
  //mutex.unlock();
}

void AudioFile::reset()
{
  //DEBUG(audiofile,"Resetting audio file %p\n", this);
  if(this->data == this->preloaded_data) {
    //DEBUG(audiofile,"\tNot completely loaded - skipping %p\n", this);
    return;
  }

  mutex.lock();
  volatile sample_t* old_data = data;
  this->size = SIZE;
  this->data = this->preloaded_data;
  //DEBUG(audiofile,"Deleting data %p\n", this);
  delete old_data; 
  mutex.unlock();
}
#endif
