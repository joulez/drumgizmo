/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lv2_test_host.cc
 *
 *  Wed Feb 11 23:11:21 CET 2015
 *  Copyright 2015 Bent Bisballe Nyeng
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
#include "lv2_test_host.h"

#include <serd/serd.h>

#include <string.h>
#include <stdlib.h>

#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>

///////////////////////////////
// Base64 encoder:
//
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <string>
class Base64 {
public:
	Base64()
	{
		mbio = (void*)BIO_new(BIO_s_mem());
		BIO *b64bio = BIO_new(BIO_f_base64());
		bio = (void*)BIO_push(b64bio, (BIO*)mbio);
	}

	~Base64()
	{
		BIO_free_all((BIO*)bio);
	}

	std::string write(std::string in)
	{
		return this->write(in.data(), in.length());
	}

	std::string write(const char *in, size_t size)
	{
		std::string out;
		
		BIO_write((BIO*)bio, in, size);

		size_t osize = BIO_ctrl_pending((BIO*)mbio);
		char *outbuf = (char*)malloc(osize);

		int len = BIO_read((BIO*)mbio, outbuf, osize);
		if(len < 1) return "";
		out.append(outbuf, len);

		free(outbuf);
		
		return out;
	}
	
	std::string flush()
	{
		std::string out;
		
		(void)BIO_flush((BIO*)bio);

		size_t size = BIO_ctrl_pending((BIO*)mbio);
		char *outbuf = (char*)malloc(size);

		int len = BIO_read((BIO*)mbio, outbuf, size);
		if(len < 1) return "";
		out.append(outbuf, len);

		free(outbuf);
		
		return out;
	}

private:
	void *bio;
	void *mbio;
};
//
// Base64 encoder
///////////////////////////////


// TODO: Use map<int, std::string> instead
static char** uris = NULL;
static size_t n_uris = 0;

static LV2_URID map_uri(LV2_URID_Map_Handle handle, const char* uri)
{
	for(size_t i = 0; i < n_uris; ++i) {
		if(!strcmp(uris[i], uri)) {
			return i + 1;
		}
	}

	uris = (char**)realloc(uris, ++n_uris * sizeof(char*));
	uris[n_uris - 1] = strdup(uri);

	return n_uris;
}

static const char* unmap_uri(LV2_URID_Map_Handle handle, LV2_URID urid)
{
	if(urid > 0 && urid <= n_uris) {
		return uris[urid - 1];
	}
	return NULL;
}

LV2_URID_Map       map           = { NULL, map_uri };
LV2_Feature        map_feature   = { LV2_URID_MAP_URI, &map };
LV2_URID_Unmap     unmap         = { NULL, unmap_uri };
LV2_Feature        unmap_feature = { LV2_URID_UNMAP_URI, &unmap };
const LV2_Feature* features[]    = { &map_feature, &unmap_feature, NULL };

LV2TestHost::Sequence::Sequence(void *buffer, size_t buffer_size)
{
  this->buffer = buffer;
  this->buffer_size = buffer_size;

	seq = (LV2_Atom_Sequence *)buffer;

	seq->atom.size = sizeof(LV2_Atom_Sequence_Body);
	seq->atom.type = map.map(map.handle, LV2_ATOM__Sequence);
	seq->body.unit = 0;
	seq->body.pad = 0;
}

// Keep this to support atom extension from lv2 < 1.10
static inline void
_lv2_atom_sequence_clear(LV2_Atom_Sequence* seq)
{
  seq->atom.size = sizeof(LV2_Atom_Sequence_Body);
}

void LV2TestHost::Sequence::clear()
{
  _lv2_atom_sequence_clear(seq);
}

// Keep this to support atom extension from lv2 < 1.10
static inline LV2_Atom_Event*
_lv2_atom_sequence_append_event(LV2_Atom_Sequence*    seq,
                               uint32_t              capacity,
                               const LV2_Atom_Event* event)
{
  const uint32_t total_size = (uint32_t)sizeof(*event) + event->body.size;
  if (capacity - seq->atom.size < total_size) {
    return NULL;
  }

  LV2_Atom_Event* e = lv2_atom_sequence_end(&seq->body, seq->atom.size);
  memcpy(e, event, total_size);
  
  seq->atom.size += lv2_atom_pad_size(total_size);
  
  return e;
}

void LV2TestHost::Sequence::addMidiNote(uint64_t pos,
                                        uint8_t key, int8_t velocity)
{
	typedef struct {
		LV2_Atom_Event event;
		uint8_t        msg[3];
	} MIDINoteEvent;

	uint8_t note_on = 0x90;

	MIDINoteEvent ev;
	ev.event.time.frames = pos;// sample position
	ev.event.body.type = map.map(map.handle, LV2_MIDI__MidiEvent);
	ev.event.body.size = sizeof(MIDINoteEvent);
 
	ev.msg[0] = note_on;
	ev.msg[1] = key;
	ev.msg[2] = velocity;

	LV2_Atom_Event *e =
		_lv2_atom_sequence_append_event(seq, this->buffer_size, &ev.event);
  (void)e;
}

void *LV2TestHost::Sequence::data()
{
  return buffer;
}

LV2TestHost::LV2TestHost(const char *lv2_path)
{
	if(lv2_path) {
    setenv("LV2_PATH", lv2_path, 1);
  }

	world = lilv_world_new();
  if(world == NULL) return;

	lilv_world_load_all(world);
}

LV2TestHost::~LV2TestHost()
{
	if(world) lilv_world_free(world);
}

int LV2TestHost::open(const char *plugin_uri)
{
  if(world == NULL) return 1;

	plugins = lilv_world_get_all_plugins(world);
  if(plugins == NULL) return 2;

	uri = lilv_new_uri(world, plugin_uri);
	if(uri == NULL) return 3;

	plugin = lilv_plugins_get_by_uri(plugins, uri);
  if(plugin == NULL) return 4;


  return 0;
}

int LV2TestHost::verify()
{
	bool verify = lilv_plugin_verify(plugin);
  if(!verify) return 1;
  return 0;
}

int LV2TestHost::close()
{
  // plugin is a const pointer; nothing to close here.
  return 0;
}

/* // Get metadata

static void dumpNodes(LilvNodes *nodes)
{
	LilvIter* iter = lilv_nodes_begin(nodes);
	while(iter) {
		const LilvNode* node = lilv_nodes_get(nodes, iter);
		printf(" - '%s'\n", lilv_node_as_uri(node));
		iter = lilv_nodes_next(nodes, iter);
	}
}

void getMetadata()
{
	LilvNode* name = lilv_plugin_get_name(plugin);
	if(name) printf("Name: %s\n", lilv_node_as_uri(name));

	// ---> line 731 in lilv.h
	bool has_latency = lilv_plugin_has_latency(plugin);
	printf("Has latency: %d\n", has_latency);

	if(has_latency) {
		uint32_t latency_port_index = lilv_plugin_get_latency_port_index(plugin);
		const LilvPort* port =
			lilv_plugin_get_port_by_index(plugin, latency_port_index);
		// Do something to actually get latency from port....
	}

	LilvNode* author = lilv_plugin_get_author_name(plugin);
	if(author) printf("Author: %s\n", lilv_node_as_uri(author));

	LilvNode* email = lilv_plugin_get_author_email(plugin);
	if(email) printf("Email: %s\n", lilv_node_as_uri(email));

	LilvNode* homepage = lilv_plugin_get_author_homepage(plugin);
	if(homepage) printf("Homepage: %s\n", lilv_node_as_uri(homepage));

	LilvNodes* supported = lilv_plugin_get_supported_features(plugin);
	LilvNodes* required = lilv_plugin_get_required_features(plugin);
	LilvNodes* optional = lilv_plugin_get_optional_features(plugin);

	printf("Supported:\n");
	dumpNodes(supported);

	printf("Required:\n");
	dumpNodes(required);

	printf("Optional:\n");
	dumpNodes(optional);

	lilv_nodes_free(supported);
	lilv_nodes_free(required);
	lilv_nodes_free(optional);
}
*/
/*
int LV2TestHost::getPorts()
{
	// Iterate ports:
	const LilvPort* port;
	uint32_t portidx = 0;
	while( (port = lilv_plugin_get_port_by_index(plugin, portidx)) != 0) {
		printf("Port: %d\n", portidx);

		LilvNode* port_name = lilv_port_get_name(plugin, port);
		if(port_name) printf("  Name: %s\n", lilv_node_as_uri(port_name));

		portidx++;
	}
}
*/
int LV2TestHost::createInstance()
{
	instance = lilv_plugin_instantiate(plugin, 48000, features);
  if(instance == NULL) return 1;
  return 0;
}

int LV2TestHost::destroyInstance()
{
  if(instance) lilv_instance_free(instance);
  return 0;
}

int LV2TestHost::activate()
{
	lilv_instance_activate(instance);
  return 0;
}

int LV2TestHost::deactivate()
{
	lilv_instance_deactivate(instance);
  return 0;
}

int LV2TestHost::loadConfig(const char *config, size_t size)
{
	Base64 b64;
	std::string b64_config = b64.write(config, size);
	b64_config += b64.flush();

	//printf("Base 64 config: [%s]\n", b64_config.c_str());

	const char ttl_config_fmt[] =
		"<http://drumgizmo.org/lv2/atom#config>\n"
		"        a pset:Preset ;\n"
		"        lv2:appliesTo <http://drumgizmo.org/lv2> ;\n"
		"        state:state [\n"
		"                <http://drumgizmo.org/lv2/atom#config> \"\"\"%s\"\"\"^^xsd:base64Binary\n"
		"        ] .\n";

	char ttl_config[sizeof(ttl_config_fmt) * 2 + b64_config.size()];
	sprintf(ttl_config, ttl_config_fmt, b64_config.c_str());

	//printf("ttl config: [%s]\n", ttl_config);

	{
		LilvState* restore_state =
			lilv_state_new_from_string(world, &map, ttl_config);
		
		lilv_state_restore(restore_state, instance, NULL, NULL,
											 LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE,
											 features);
	}

  return 0;
}

int LV2TestHost::connectPort(int port, void *portdata)
{
  //  if(lilv_port_is_a(p, port, lv2_ControlPort)) ...

	lilv_instance_connect_port(instance, port, portdata);

  return 0;
}

int LV2TestHost::run(int num_samples)
{
  lilv_instance_run(instance, num_samples);
  return 0;
}
