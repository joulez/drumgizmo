/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audioinputenginedl.cc
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
#include "audioinputenginedl.h"

#include <stdio.h>

#include <dlfcn.h>

#include <config.h>
#include <string.h>
#include <stdlib.h>

#include "jackclient.h"

AudioInputEngineDL::AudioInputEngineDL(std::string name)
{
  is_jack_plugin = strstr(name.c_str(), "jack");

  std::string plugin = INPUT_PLUGIN_DIR"/" + name + ".so";
  void *lib = dlopen(plugin.c_str(), RTLD_LAZY);
  if(!lib) {
    printf("Cannot load device: %s\n", dlerror());
    return;
  }

  i_create = (input_create_func_t) dlsym(lib, "create");
  const char* dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol create: %s\n", dlsym_error);
    return;
  }

  i_destroy = (input_destroy_func_t) dlsym(lib, "destroy");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return;
  }

  i_init = (input_init_func_t) dlsym(lib, "init");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol init: %s\n", dlsym_error);
    return;
  }

  i_setparm = (input_setparm_func_t) dlsym(lib, "setparm");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol setparm: %s\n", dlsym_error);
    return;
  }

  i_start = (input_start_func_t) dlsym(lib, "start");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol start: %s\n", dlsym_error);
    return;
  }

  i_stop = (input_stop_func_t) dlsym(lib, "stop");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol stop: %s\n", dlsym_error);
    return;
  }

  i_pre = (input_pre_func_t) dlsym(lib, "pre");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol pre: %s\n", dlsym_error);
    return;
  }

  i_run = (input_run_func_t) dlsym(lib, "run");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol run: %s\n", dlsym_error);
    return;
  }

  i_post = (input_post_func_t) dlsym(lib, "post");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol post: %s\n", dlsym_error);
    return;
  }

  ptr = i_create();

  if(is_jack_plugin) {
    char ptrbuf[32];
    jackclient = init_jack_client();
    sprintf(ptrbuf, "%p", jackclient);
    setParm("jack_client", ptrbuf);
  }
}

AudioInputEngineDL::~AudioInputEngineDL()
{
  i_destroy(ptr);
  if(is_jack_plugin) close_jack_client();
}

bool AudioInputEngineDL::init(Instruments &instruments)
{
  char **n = (char**)malloc(sizeof(char*)*instruments.size());
  for(size_t i = 0; i < instruments.size(); i++) {
    n[i] = strdup(instruments[i]->name().c_str());
  }

  bool ret = i_init(ptr, instruments.size(), n);

  for(size_t i = 0; i < instruments.size(); i++) {
    free(n[i]);
  }
  free(n);

  return ret;
}

void AudioInputEngineDL::setParm(std::string parm, std::string value)
{
  i_setparm(ptr, parm.c_str(), value.c_str());
}

bool AudioInputEngineDL::start()
{
  if(is_jack_plugin) jackclient->activate();
  return i_start(ptr);
}

void AudioInputEngineDL::stop()
{
  return i_stop(ptr);
}

void AudioInputEngineDL::pre()
{
  return i_pre(ptr);
}

event_t *AudioInputEngineDL::run(size_t pos, size_t len, size_t *nevents)
{
  return i_run(ptr, pos, len, nevents);
}

void AudioInputEngineDL::post()
{
  return i_post(ptr);
}

//#include "audioinputenginedummy.h"
//#include "audioinputenginejackmidi.h"
//#include "audioinputenginemidifile.h"

/*

typedef Device* (*create_func_t)(void);
typedef void (*destroy_func_t)(Device*);

struct device_t {
  Device* dev;
  destroy_func_t destroyer;
  void* lib;
};

int load_shared_device(device_t &dev, std::string devlib,
                             std::string devfile, ConfMap devconfmap) {
  // load library
  dev.lib = dlopen(devlib.c_str(), RTLD_LAZY);
  if(!dev.lib ) {
    printf("Cannot load device: %s\n", dlerror());
    return -1;
  }

  create_func_t create_device = (create_func_t) dlsym(dev.lib, "create");
  const char* dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol create: %s\n", dlsym_error);
    return -1;
  }

  dev.destroyer = (destroy_func_t) dlsym(dev.lib, "destroy");
  dlsym_error = dlerror();
  if(dlsym_error) {
    printf("Cannot load symbol destroy: %s\n", dlsym_error);
    return -1;
  }

  dev.dev = create_device();

  // initialize device 
  DevData devdata = dev.dev->init(devfile, devconfmap);
  if(devdata.retval != DevData::VALUE_SUCCESS) {
    printf("Error while initializing device: %s\n",  devdata.msg.c_str());
    return -1;
  }

  return 0;
}

void unload_shared_device(device_t &dev) {
  
  dev.destroyer(dev.dev);
  dlclose(dev.lib);
}
*/

#ifdef TEST_AUDIOINPUTENGINEDL
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

#endif/*TEST_AUDIOINPUTENGINEDL*/
