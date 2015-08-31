/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiooutputenginedl.h
 *
 *  Wed Jul 13 14:40:01 CEST 2011
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
#ifndef __DRUMGIZMO_AUDIOOUTPUTENGINEDL_H__
#define __DRUMGIZMO_AUDIOOUTPUTENGINEDL_H__

#include <string>
#include <stdlib.h>
#include <audiotypes.h>

#include "channel.h"

#include "audiooutputengine.h"
#include "jackclient.h"

typedef void* (*output_create_func_t)(void);
typedef void (*output_destroy_func_t)(void*);
typedef bool (*output_start_func_t)(void*);
typedef void (*output_stop_func_t)(void*);
typedef bool (*output_init_func_t)(void*,int,char**);
typedef void (*output_setparm_func_t)(void*,const char*,const char*);
typedef void (*output_pre_func_t)(void*, size_t);
typedef void (*output_run_func_t)(void*,int,sample_t*,size_t);
typedef void (*output_post_func_t)(void*, size_t);
typedef size_t (*output_bufsize_func_t)(void*);
typedef size_t (*output_samplerate_func_t)(void*);

class AudioOutputEngineDL : public AudioOutputEngine {
public:
  AudioOutputEngineDL(std::string name);
  ~AudioOutputEngineDL();

  bool init(Channels channels);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre(size_t nsamples);
  void run(int ch, sample_t *samples, size_t nsamples);
  void post(size_t nsamples);

  size_t getBufferSize();
  size_t samplerate();

private:
  void *ptr;
  output_create_func_t o_create;
  output_destroy_func_t o_destroy;
  output_init_func_t o_init;
  output_setparm_func_t o_setparm;
  output_start_func_t o_start;
  output_stop_func_t o_stop;
  output_pre_func_t o_pre;
  output_run_func_t o_run;
  output_post_func_t o_post;
  output_bufsize_func_t o_bufsize;
  output_samplerate_func_t o_samplerate;

  bool is_jack_plugin;
  JackClient *jackclient;
};

#endif/*__DRUMGIZMO_AUDIOOUTPUTENGINEDL_H__*/
