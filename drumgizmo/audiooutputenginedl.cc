/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiooutputenginedl.cc
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
#include "audiooutputenginedl.h"

#include <dlfcn.h>
#include <stdio.h>
#include <config.h>
#include <string.h>

AudioOutputEngineDL::AudioOutputEngineDL(std::string name)
{
  is_jack_plugin = strstr(name.c_str(), "jack");

  std::string plugin = OUTPUT_PLUGIN_DIR"/" + name + ".so";
  void *lib = dlopen(plugin.c_str(), RTLD_LAZY);
  if(!lib) {
    printf("Cannot load device: %s\n", dlerror());
    return;
  }

  o_create = (output_create_func_t) dlsym(lib, "create");
  const char* dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol create: %s\n", dlsym_error);
    return;
  }

  o_destroy = (output_destroy_func_t) dlsym(lib, "destroy");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_init = (output_init_func_t) dlsym(lib, "init");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_setparm = (output_setparm_func_t) dlsym(lib, "setparm");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_start = (output_start_func_t) dlsym(lib, "start");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_stop = (output_stop_func_t) dlsym(lib, "stop");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_pre = (output_pre_func_t) dlsym(lib, "pre");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_run = (output_run_func_t) dlsym(lib, "run");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_post = (output_post_func_t) dlsym(lib, "post");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  o_bufsize = (output_bufsize_func_t) dlsym(lib, "bufsize");
  dlsym_error = dlerror();
  if(dlsym_error) {
    o_bufsize = NULL;
  }

  o_samplerate = (output_samplerate_func_t) dlsym(lib, "samplerate");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol samplerate: %s\n", dlsym_error);
    return;
  }

  ptr = o_create();

  if(is_jack_plugin) {
    char ptrbuf[32];
    jackclient = init_jack_client();
    sprintf(ptrbuf, "%p", jackclient);
    setParm("jack_client", ptrbuf);
  }
}

AudioOutputEngineDL::~AudioOutputEngineDL()
{
  o_destroy(ptr);
  if(is_jack_plugin) close_jack_client();
}

bool AudioOutputEngineDL::init(Channels channels)
{
  char **n = (char**)malloc(sizeof(char*)*channels.size());
  for(size_t i = 0; i < channels.size(); i++) {
    n[i] = strdup(channels[i].name.c_str());
  }

  bool ret = o_init(ptr, channels.size(), n);

  for(size_t i = 0; i < channels.size(); i++) {
    free(n[i]);
  }
  free(n);

  return ret;
}

void AudioOutputEngineDL::setParm(std::string parm, std::string value)
{
  o_setparm(ptr, parm.c_str(), value.c_str());
}

bool AudioOutputEngineDL::start()
{
  if(is_jack_plugin) jackclient->activate();
  return o_start(ptr);
}

void AudioOutputEngineDL::stop()
{
  return o_stop(ptr);
}

void AudioOutputEngineDL::pre(size_t size)
{
  return o_pre(ptr, size);
}
 
void AudioOutputEngineDL::run(int ch, sample_t *samples, size_t nsamples)
{
  o_run(ptr, ch, samples, nsamples);
}

void AudioOutputEngineDL::post(size_t size)
{
  return o_post(ptr, size);
}

size_t AudioOutputEngineDL::getBufferSize()
{
  if(o_bufsize) return o_bufsize(ptr);
  return 1024;
}

size_t AudioOutputEngineDL::samplerate()
{
  if(o_samplerate) return o_samplerate(ptr);
  return 44100;
}
