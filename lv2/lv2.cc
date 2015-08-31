/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lv2.cc
 *
 *  Wed Jul 13 13:50:33 CEST 2011
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
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>

#include <stdlib.h>
#include <string.h>

#include "lv2_gui.h"
#include "lv2_instance.h"

#include <hugin.hpp>

#define DRUMGIZMO_URI "http://drumgizmo.org/lv2"
#define NS_DG DRUMGIZMO_URI "/atom#"

// Stuff to handle DrumGizmo* transmission from instance to GUI.
static LV2_DrumGizmo_Descriptor dg_descriptor;

static DrumGizmo *dg_get_pci(LV2_Handle instance)
{
  DGLV2 *dglv2 = (DGLV2 *)instance;
  return dglv2->dg;
}

LV2_State_Status
dg_save(LV2_Handle                 instance,
        LV2_State_Store_Function   store,
        LV2_State_Handle           handle,
        uint32_t                   flags,
        const LV2_Feature *const * features)
{
  DGLV2 *dglv2 = (DGLV2 *)instance;

  if(!dglv2 || !dglv2->map || !dglv2->map->map) {
    // Missing urid feature?
    return  LV2_STATE_ERR_NO_FEATURE;
  }

  std::string config = dglv2->dg->configString();

  // Backwards compatible fix for errornously stored '\0' byte in < v0.9.8.
  // Remove when we reach v1.0
  config += "\n";

  store(handle,
        dglv2->map->map(dglv2->map->handle, NS_DG "config"),
        config.data(),
        config.length(),
        dglv2->map->map(dglv2->map->handle, LV2_ATOM__Chunk),
        LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE);

  return LV2_STATE_SUCCESS;
}

LV2_State_Status
dg_restore(LV2_Handle                  instance,
           LV2_State_Retrieve_Function retrieve,
           LV2_State_Handle            handle,
           uint32_t                    flags,
           const LV2_Feature *const *  features)
{
  DGLV2 *dglv2 = (DGLV2 *)instance;

  if(!dglv2 || !dglv2->map || !dglv2->map->map) {
    // Missing urid feature?
    return  LV2_STATE_ERR_NO_FEATURE;
  }

  size_t size;
  uint32_t type;

  const char* data =
    (const char*)retrieve(handle,
                          dglv2->map->map(dglv2->map->handle, NS_DG "config"),
                          &size, &type, &flags);

  DEBUG(lv2, "Config string size: %d, data*: %p\n", (int)size, data);

  if(data && size) {
    std::string config;

    // Fix for errornously stored '\0' byte in < v0.9.8.
    // Remove when we reach v1.0
    if(data[size - 1] == '\0') size--;

    config.append(data, size);
    dglv2->dg->setConfigString(config);
  }

  return LV2_STATE_SUCCESS;
}

static LV2_State_Interface dg_persist = {
  dg_save,
  dg_restore
};

LV2_Handle instantiate(const struct _LV2_Descriptor *descriptor,
                       double sample_rate,
                       const char *bundle_path,
                       const LV2_Feature *const *features)
{
  DGLV2 *dglv2 = new DGLV2;

  dglv2->map = NULL;
  for (int i = 0 ; features[i] ; i++) {
    if (!strcmp(features[i]->URI,  LV2_URID_URI "#map")) {
      dglv2->map = (LV2_URID_Map*)features[i]->data;
    }
 }

  dg_descriptor.get_pci = dg_get_pci;

  dglv2->in = new InputLV2();
  dglv2->out = new OutputLV2();

  dglv2->buffer = NULL;
  dglv2->buffer_size = 0;

  dglv2->dg = new DrumGizmo(dglv2->out, dglv2->in);
  dglv2->dg->setSamplerate(sample_rate);

  return (LV2_Handle)dglv2;
}

void connect_port(LV2_Handle instance,
                  uint32_t port,
                  void *data_location)
{
  DGLV2 *dglv2 = (DGLV2 *)instance;

  if(port == 0) {// MIDI in
    dglv2->in->eventPort = (LV2_Atom_Sequence*)data_location;
  } else {// Audio Port
    if(port - 1 < NUM_OUTPUTS) {
      dglv2->out->outputPorts[port - 1].samples = (sample_t*)data_location;
      dglv2->out->outputPorts[port - 1].size = 0;
    }
  }
}

void activate(LV2_Handle instance)
{
  // We don't really need to do anything here.
  DGLV2 *dglv2 = (DGLV2 *)instance;
  (void)dglv2;
}

void run(LV2_Handle instance,
         uint32_t sample_count)
{
  static size_t pos = 0;
  DGLV2 *dglv2 = (DGLV2 *)instance;

  dglv2->dg->run(pos, dglv2->buffer, sample_count);

  pos += sample_count;
}

void deactivate(LV2_Handle instance)
{
  // We don't really need to do anything here.
  DGLV2 *dglv2 = (DGLV2 *)instance;
  dglv2->dg->stop();
}

void cleanup(LV2_Handle instance)
{
  DGLV2 *dglv2 = (DGLV2 *)instance;
  delete dglv2->dg;
  delete dglv2->in;
  delete dglv2->out;
}

const void* extension_data(const char *uri)
{
  if(!strcmp(uri, PLUGIN_INSTANCE_URI)) return &dg_descriptor;
  if(!strcmp(uri, LV2_STATE__interface)) return &dg_persist;
  return NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

static const LV2_Descriptor descriptor = {
	DRUMGIZMO_URI,
	instantiate,
	connect_port,
  activate,
	run,
  deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}

#ifdef __cplusplus
}
#endif
