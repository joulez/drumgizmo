/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audioinputenginedl.h
 *
 *  Wed Jul 13 14:39:54 CEST 2011
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
#ifndef __DRUMGIZMO_AUDIOINPUTENGINEDL_H__
#define __DRUMGIZMO_AUDIOINPUTENGINEDL_H__

#include "audioinputengine.h"
#include "jackclient.h"

typedef void* (*input_create_func_t)(void);
typedef void (*input_destroy_func_t)(void*);
typedef bool (*input_init_func_t)(void*,int,char**);
typedef void (*input_setparm_func_t)(void*,const char*,const char*);
typedef bool (*input_start_func_t)(void*);
typedef void (*input_stop_func_t)(void*);
typedef void (*input_pre_func_t)(void*);
typedef event_t* (*input_run_func_t)(void*,size_t,size_t,size_t*);
typedef void (*input_post_func_t)(void*);
  
class AudioInputEngineDL : public AudioInputEngine {
public:
  AudioInputEngineDL(std::string name);
  ~AudioInputEngineDL();

  bool init(Instruments &instruments);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

private:
  void *ptr;
  input_create_func_t i_create;
  input_destroy_func_t i_destroy;
  input_init_func_t i_init;
  input_setparm_func_t i_setparm;
  input_start_func_t i_start;
  input_stop_func_t i_stop;
  input_pre_func_t i_pre;
  input_run_func_t i_run;
  input_post_func_t i_post;

  bool is_jack_plugin;
  JackClient *jackclient;
};

#endif/*__DRUMGIZMO_AUDIOINPUTENGINEDL_H__*/
