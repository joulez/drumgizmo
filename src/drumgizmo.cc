/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumgizmo.cc
 *
 *  Thu Sep 16 10:24:40 CEST 2010
 *  Copyright 2010 Bent Bisballe Nyeng
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
#include "drumgizmo.h"

#include <math.h>
#include <stdio.h>

#include <event.h>
#include <audiotypes.h>

#include <string.h>

#include <hugin.hpp>

#include <config.h>

#include "drumkitparser.h"
#include "audioinputenginemidi.h"
#include "configuration.h"
#include "configparser.h"

#include "nolocale.h"

DrumGizmo::DrumGizmo(AudioOutputEngine *o, AudioInputEngine *i)
  : MessageReceiver(MSGRCV_ENGINE),
    loader(), oe(o), ie(i)
{
  is_stopping = false;
}

DrumGizmo::~DrumGizmo()
{
}

bool DrumGizmo::loadkit(std::string file)
{
  if(file == "") return 1;

  DEBUG(drumgizmo, "loadkit(%s)\n", file.c_str());

  // Remove all queue AudioFiles from loader before we actually delete them.
  loader.skip();

  // Delete all Channels, Instruments, Samples and AudioFiles.
  kit.clear();

  DrumKitParser parser(file, kit);
  if(parser.parse()) {
    ERR(drumgizmo, "Drumkit parser failed: %s\n", file.c_str());
    return false;
  }

  loader.loadKit(&kit);

#ifdef WITH_RESAMPLER
  for(int i = 0; i < MAX_NUM_CHANNELS; i++) {
    resampler[i].setup(kit.samplerate(), Conf::samplerate);
  }
#endif/*WITH_RESAMPLER*/

  DEBUG(loadkit, "loadkit: Success\n");

  return true;
}

bool DrumGizmo::init()
{
  if(!ie->init(kit.instruments)) return false;
  if(!oe->init(kit.channels)) return false;

  return true;
}

void DrumGizmo::handleMessage(Message *msg)
{
  DEBUG(msg, "got message.");
  switch(msg->type()) {
  case Message::LoadDrumKit:
    {
      DEBUG(msg, "got LoadDrumKitMessage message.");
      LoadDrumKitMessage *m = (LoadDrumKitMessage*)msg;
      loadkit(m->drumkitfile);
      //init(true);
    }
    break;
  case Message::LoadMidimap:
    DEBUG(msg, "got LoadMidimapMessage message.");
    if(!ie->isMidiEngine()) break;
    {
      AudioInputEngineMidi *aim = (AudioInputEngineMidi*)ie;
      LoadMidimapMessage *m = (LoadMidimapMessage*)msg;
      bool ret = aim->loadMidiMap(m->midimapfile, kit.instruments);
      
      LoadStatusMessageMidimap *ls = new LoadStatusMessageMidimap();
      ls->success = ret;
      msghandler.sendMessage(MSGRCV_UI, ls);
    }
    break;
  case Message::EngineSettingsMessage:
    {
      bool mmap_loaded = false;
      std::string mmapfile;
      if(ie->isMidiEngine()) {
        AudioInputEngineMidi *aim = (AudioInputEngineMidi*)ie;
        mmapfile = aim->midimapFile();
        mmap_loaded = aim->isValid();
      }
      
      EngineSettingsMessage *msg = new EngineSettingsMessage();
      msg->midimapfile = mmapfile;
      msg->midimap_loaded = mmap_loaded;
      msg->drumkitfile = kit.file();
      msg->drumkit_loaded = loader.isDone();
      msg->enable_velocity_modifier = Conf::enable_velocity_modifier;
      msg->velocity_modifier_falloff = Conf::velocity_modifier_falloff;
      msg->velocity_modifier_weight = Conf::velocity_modifier_weight;
      msg->enable_velocity_randomiser = Conf::enable_velocity_randomiser;
      msg->velocity_randomiser_weight = Conf::velocity_randomiser_weight;
      msghandler.sendMessage(MSGRCV_UI, msg);
    }
    break;
  case Message::ChangeSettingMessage:
    {
      ChangeSettingMessage *ch = (ChangeSettingMessage*)msg;
      switch(ch->name) {
      case ChangeSettingMessage::enable_velocity_modifier:
        Conf::enable_velocity_modifier = ch->value;
        break;
      case ChangeSettingMessage::velocity_modifier_weight:
        Conf::velocity_modifier_weight = ch->value;
        break;
      case ChangeSettingMessage::velocity_modifier_falloff:
        Conf::velocity_modifier_falloff = ch->value;
        break;
      }
    }
    break;
  default:
    break;
  }
}

bool DrumGizmo::run(size_t pos, sample_t *samples, size_t nsamples)
{
  // Handle engine messages, at most one in each iteration:
  handleMessages(1);

  ie->pre();
  oe->pre(nsamples);

  //
  // Read new events
  //

  //DEBUG(engine, "Number of active events: %d\n", activeevents[0].size());

  size_t nev;
  event_t *evs = ie->run(pos, nsamples, &nev);

  for(size_t e = 0; e < nev; e++) {
    if(evs[e].type == TYPE_ONSET) {
      Instrument *i = NULL;
      int d = evs[e].instrument;
      /*
        Instruments::iterator it = kit.instruments.begin();
        while(d-- && it != kit.instruments.end()) {
        i = &(it->second);
        it++;
        }
      */
      
      if(!kit.isValid()) continue;

      if(d < (int)kit.instruments.size()) {
        i = kit.instruments[d];
      }

      if(i == NULL || !i->isValid()) {
        ERR(drumgizmo, "Missing Instrument %d.\n", evs[e].instrument);
        continue;
      }

      if(i->group() != "") {
        // Add event to ramp down all existing events with the same groupname.
        Channels::iterator j = kit.channels.begin();
        while(j != kit.channels.end()) {
          Channel &ch = *j;
          std::list< Event* >::iterator evs = activeevents[ch.num].begin();
          while(evs != activeevents[ch.num].end()) {
            Event *ev = *evs;
            if(ev->type() == Event::sample) {
              EventSample *sev = (EventSample*)ev;
              if(sev->group == i->group() && sev->instrument != i) {
                sev->rampdown = 3000; // Ramp down 3000 samples
                // TODO: This must be configurable at some point...
                // ... perhaps even by instrument (ie. in the xml file)
                sev->ramp_start = sev->rampdown;
              }
            }
            evs++;
          }
          j++;
        }
      }

      Sample *s = i->sample(evs[e].velocity, evs[e].offset + pos);
      
      if(s == NULL) {
        ERR(drumgizmo, "Missing Sample.\n");
        continue;
      }
      
      Channels::iterator j = kit.channels.begin();
      while(j != kit.channels.end()) {
        Channel &ch = *j;
        AudioFile *af = s->getAudioFile(&ch);
        if(af) {
          // LAZYLOAD:
          // DEBUG(drumgizmo,"Requesting preparing of audio file\n");
          // loader.prepare(af);
        }
        if(af == NULL || !af->isValid()) {
          //DEBUG(drumgizmo,"Missing AudioFile.\n");
        } else {
          //DEBUG(drumgizmo, "Adding event %d.\n", evs[e].offset);
          Event *evt = new EventSample(ch.num, 1.0, af, i->group(), i);
          evt->offset = (evs[e].offset + pos) * resampler[0].ratio();
          activeevents[ch.num].push_back(evt);
        }
        j++;
      }
    }
    
    if(evs[e].type == TYPE_STOP) {
      is_stopping = true;
    }

    if(is_stopping) {
      // Count the number of active events.
      int num_active_events = 0;
      Channels::iterator j = kit.channels.begin();
      while(j != kit.channels.end()) {
        Channel &ch = *j;
        num_active_events += activeevents[ch.num].size();
        j++;
      }

      if(num_active_events == 0) {
        // No more active events - now we can stop the engine.
        return false;
      }
    }
    
  }
    
  free(evs);

  //
  // Write audio
  //
#ifdef WITH_RESAMPLER
  if(Conf::enable_resampling == false ||
     resampler[0].ratio() == 1.0) { // No resampling needed
#endif
    for(size_t c = 0; c < kit.channels.size(); c++) {
      sample_t *buf = samples;
      bool internal = false;
      if(oe->getBuffer(c)) {
        buf = oe->getBuffer(c);
        internal = true;
      }
      if(buf) {
        memset(buf, 0, nsamples * sizeof(sample_t));

        getSamples(c, pos, buf, nsamples);

        if(!internal) oe->run(c, samples, nsamples);
      }
    }
#ifdef WITH_RESAMPLER
  } else {
  // Resampling needed

  //
  // NOTE: Channels must be processed one buffer at a time on all channels in
  // parallel - NOT all buffers on one channel and then all buffer on the next
  // one since this would mess up the event queue (it would jump back and forth
  // in time)
  //

  // Prepare output buffer
  for(size_t c = 0; c < kit.channels.size(); c++) {
    resampler[c].setOutputSamples(resampler_output_buffer[c], nsamples);
  }

  // Process channel data
  size_t kitpos = pos * resampler[0].ratio();
  size_t insize = sizeof(resampler_input_buffer[0]) / sizeof(sample_t);

  //printf("ratio: %f\n", resampler[c].ratio());
  while(resampler[0].getOutputSampleCount() > 0) {
    for(size_t c = 0; c < kit.channels.size(); c++) {
      if(resampler[c].getInputSampleCount() == 0) {
        sample_t *sin = resampler_input_buffer[c];
        memset(resampler_input_buffer[c], 0,
               sizeof(resampler_input_buffer[c]));
        getSamples(c, kitpos, sin, insize);

        resampler[c].setInputSamples(sin, insize);
      }
      resampler[c].process();
    }
    kitpos += insize;
  }

  // Write output data to output engine.
  for(size_t c = 0; c < kit.channels.size(); c++) {
    oe->run(c, resampler_output_buffer[c], nsamples);
  }

  }
#endif/*WITH_RESAMPLER*/
  
  ie->post();
  oe->post(nsamples);
  
  pos += nsamples;

  return true;
}

void DrumGizmo::run(int endpos)
{
  size_t pos = 0;
  size_t nsamples = oe->getBufferSize();
  sample_t *samples = (sample_t *)malloc(nsamples * sizeof(sample_t));

  ie->start();
  oe->start();

  while(run(pos, samples, nsamples) == true) {
    pos += nsamples;
    if(endpos != -1 && pos >= (size_t)endpos) break;
  }

  ie->stop();
  oe->stop();

  free(samples);
}

#ifdef SSE
#define N 8
typedef float vNsf __attribute__ ((vector_size(sizeof(float)*N)));
#endif/*SSE*/

void DrumGizmo::getSamples(int ch, int pos, sample_t *s, size_t sz)
{
  std::list< Event* >::iterator i = activeevents[ch].begin();
  while(i != activeevents[ch].end()) {
    bool removeevent = false;

    Event *event = *i;

    Event::type_t type = event->type();
    switch(type) {
    case Event::sample:
      {
        EventSample *evt = (EventSample *)event;
        AudioFile *af = evt->file;

        if(!af->isLoaded() || !af->isValid() || s == NULL) {
          removeevent = true;
          break;
        }

        {
        MutexAutolock l(af->mutex);

        size_t n = 0;
        if(evt->offset > (size_t)pos) n = evt->offset - pos;
        size_t end = sz;
        if((evt->t + end - n) > af->size) end = af->size - evt->t + n;
        if(end > sz) end = sz;

        if(evt->rampdown == NO_RAMPDOWN) {
#ifdef SSE
//          DEBUG(drumgizmo,"%d\n", evt->t); fflush(stdout);
         size_t optend = ((end - n) / N) * N + n;
         for(; n < optend; n += N) {
            *(vNsf*)&(s[n]) += *(vNsf*)&(af->data[evt->t]);
            evt->t += N;
          }
#endif
          for(; n < end; n++) {
            s[n] += af->data[evt->t];
            evt->t++;
          }
        } else { // Ramp down in progress.
          for(; n < end && evt->rampdown; n++) {
            float scale = (float)evt->rampdown/(float)evt->ramp_start;
            s[n] += af->data[evt->t] * scale;
            evt->t++;
            evt->rampdown--;
          }
          
          if(evt->rampdown == 0) {
            removeevent = true; // Down ramp done. Remove event.
          }
        }

        if(evt->t >= af->size) { 
          removeevent = true;
        }

        }
      }
      break;
    }

    if(removeevent) {
      delete event;
      i = activeevents[ch].erase(i);
      continue;
    }
    i++;
  }
}

void DrumGizmo::stop()
{
  // engine.stop();
}

int DrumGizmo::samplerate()
{
  return Conf::samplerate;
}

void DrumGizmo::setSamplerate(int samplerate)
{
  Conf::samplerate = samplerate;
#ifdef WITH_RESAMPLER
  for(int i = 0; i < MAX_NUM_CHANNELS; i++) {
    resampler[i].setup(kit.samplerate(), Conf::samplerate);
  }
#endif/*WITH_RESAMPLER*/

}

std::string float2str(float a)
{
  char buf[256];
  snprintf_nol(buf, sizeof(buf) - 1, "%f", a);
  return buf;
}

std::string bool2str(bool a)
{
  return a?"true":"false";
}

float str2float(std::string a)
{
  if(a == "") return 0.0;
  return atof_nol(a.c_str());
}

std::string DrumGizmo::configString()
{
  std::string mmapfile;
  if(ie->isMidiEngine()) {
    AudioInputEngineMidi *aim = (AudioInputEngineMidi*)ie;
    mmapfile = aim->midimapFile();
  }

  return
    "<config>\n"
    "  <value name=\"drumkitfile\">" + kit.file() + "</value>\n"
    "  <value name=\"midimapfile\">" + mmapfile + "</value>\n"
    "  <value name=\"enable_velocity_modifier\">" +
    bool2str(Conf::enable_velocity_modifier) + "</value>\n"
    "  <value name=\"velocity_modifier_falloff\">" +
    float2str(Conf::velocity_modifier_falloff) + "</value>\n"
    "  <value name=\"velocity_modifier_weight\">" +
    float2str(Conf::velocity_modifier_weight) + "</value>\n"
    "  <value name=\"enable_velocity_randomiser\">" +
    bool2str(Conf::enable_velocity_randomiser) + "</value>\n"
    "  <value name=\"velocity_randomiser_weight\">" +
    float2str(Conf::velocity_randomiser_weight) + "</value>\n"
    "</config>";
}


bool DrumGizmo::setConfigString(std::string cfg)
{
  DEBUG(config, "Load config: %s\n", cfg.c_str());

  std::string dkf;
  ConfigParser p;
  if(p.parse(cfg)) {
    ERR(drumgizmo, "Config parse error.\n");
    return false;
  }

  if(p.value("enable_velocity_modifier") != "") {
    Conf::enable_velocity_modifier =
      p.value("enable_velocity_modifier") == "true";
  }

  if(p.value("velocity_modifier_falloff") != "") {
    Conf::velocity_modifier_falloff =
      str2float(p.value("velocity_modifier_falloff"));
  }

  if(p.value("velocity_modifier_weight") != "") {
    Conf::velocity_modifier_weight =
      str2float(p.value("velocity_modifier_weight"));
  }

  if(p.value("enable_velocity_randomiser") != "") {
    Conf::enable_velocity_randomiser =
      p.value("enable_velocity_randomiser") == "true";
  }

  if(p.value("velocity_randomiser_weight") != "") {
    Conf::velocity_randomiser_weight =
      str2float(p.value("velocity_randomiser_weight"));
  }

  if(p.value("enable_resampling") != "") {
    Conf::enable_resampling =
      p.value("enable_resampling") == "true";
  }

  std::string newkit = p.value("drumkitfile");
  if(newkit != "" && kit.file() != newkit) {
    /*
    if(!loadkit(p.values["drumkitfile"])) return false;
    init(true);
    */
    LoadDrumKitMessage *msg = new LoadDrumKitMessage();
    msg->drumkitfile = newkit;
    msghandler.sendMessage(MSGRCV_ENGINE, msg);
  }

  std::string newmidimap = p.value("midimapfile");
  if(newmidimap != "") {
    //midimapfile = newmidimap;
    LoadMidimapMessage *msg = new LoadMidimapMessage();
    msg->midimapfile = newmidimap;
    msghandler.sendMessage(MSGRCV_ENGINE, msg);
  }

  return true;
}

#ifdef TEST_DRUMGIZMO
//deps: instrument.cc sample.cc channel.cc audiofile.cc drumkit.cc drumkitparser.cc configuration.cc saxparser.cc instrumentparser.cc path.cc
//cflags: $(SNDFILE_CFLAGS) $(EXPAT_CFLAGS) -I../include -DSSE -msse -msse2 -msse3
//libs: $(SNDFILE_LIBS) $(EXPAT_LIBS)
#include "test.h"

static float f(size_t x)
{
  return x + 1.0;
}

class AITest : public AudioInputEngine {
public:
  bool init(Instruments &instruments) { return true; }
  void setParm(std::string parm, std::string value) {}
  bool start() { return true; }
  void stop() {}
  void pre() {}
  event_t *run(size_t pos, size_t len, size_t *nevents)
  {
    event_t *e = NULL;
    *nevents = 0;

    if(pos <= offset && offset < pos + len) {
      e = new event_t;

      e->type = TYPE_ONSET;
      e->instrument = 0;
      e->velocity = 1.0;
      e->offset = offset - pos;

      *nevents = 1;
    }
    return e;
  }
  void post() {}
  size_t offset;
};

class AOTest : public AudioOutputEngine {
public:
  bool init(Channels channels) { return true; }
  void setParm(std::string parm, std::string value) {}
  bool start() { return true; }
  void stop() {}
  void pre(size_t nsamples) {}
  void run(int ch, sample_t *samples, size_t nsamples)
  {
  }
  void post(size_t nsamples) {}
};

const char xml_kit[] =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
  "<drumkit name=\"test\" description=\"\">\n"
  "  <channels>\n"
  "    <channel name=\"ch1\"/>\n"
  "  </channels>\n"
  "  <instruments>\n"
  "    <instrument name=\"instr1\" file=\"instr1.xml\">\n"
  "      <channelmap in=\"ch1\" out=\"ch1\"/>\n"
  "		</instrument>\n"
  "	</instruments>\n"
  "</drumkit>";

const char xml_instr[] =
  "<?xml version='1.0' encoding='UTF-8'?>\n"
  "<instrument name=\"instr1\">\n"
  " <samples>\n"
  "  <sample name=\"sample1\">\n"
  "   <audiofile channel=\"ch1\" file=\"instr1.wav\"/>\n"
  "  </sample>\n"
  " </samples>\n"
  " <velocities>\n"
  "  <velocity lower=\"0\" upper=\"1.0\">\n"
  "   <sampleref name=\"sample1\"/>\n"
  "  </velocity>\n"
  " </velocities>\n"
  "</instrument>";

#define PCM_SIZE 100

void createTestKit()
{
  FILE *fp;
  fp = fopen("/tmp/kit.xml", "w");
  fwrite(xml_kit, strlen(xml_kit), 1, fp);
  fclose(fp);

  fp = fopen("/tmp/instr1.xml", "w");
  fwrite(xml_instr, strlen(xml_instr), 1, fp);
  fclose(fp);

  SF_INFO sf_info;
  sf_info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
  sf_info.samplerate = 44100;
  sf_info.channels = 1;

  SNDFILE *fh = sf_open("/tmp/instr1.wav", SFM_WRITE, &sf_info);
  if(!fh) {
    printf("Error: %s\n", sf_strerror(fh));
  }

  size_t size = PCM_SIZE;
  sample_t samples[size];

  for(size_t i = 0; i < size; i++) {
    samples[i] = f(i);//(float)i / (float)size;
  }

  sf_write_float(fh, samples, size); 
  sf_close(fh);
}

void deleteTestKit()
{
  unlink("/tmp/kit.xml");
  unlink("/tmp/instr1.xml");
  unlink("/tmp/instr1.wav");
}

TEST_BEGIN;

createTestKit();

size_t size = PCM_SIZE;
//for(size_t chunksz = 1; chunksz < size + 1; chunksz++) {
size_t chunksz = 16; {
  sample_t samples[chunksz];

  for(size_t offset = 0; offset < chunksz + size + 1; offset++) {
    //size_t offset = 5; {
    for(size_t padding = 0; padding < chunksz + size + offset + 1; padding++) {
      //size_t padding = 2; {
      TEST_MSG("Values (offset %d, padding %d, chunksz %d)",
               offset, padding, chunksz);
      
      AOTest ao;
      AITest ai; ai.offset = offset;
      DrumGizmo dg(&ao, &ai);
      dg.loadkit("/tmp/kit.xml");
      
      size_t pos = 0;
      //      sample_t samples[chunksz];
      while(pos < offset + size + padding) {
        dg.run(pos, samples, chunksz);
        
        float err = 0;
        size_t errcnt = 0;
        for(size_t i = 0; i < chunksz && pos < offset + size + padding; i++) {
          float val = 0.0;
          if(pos >= offset && pos < (offset + size)) val = f(pos - offset);
          float diff = samples[i] - val;
          /*
          if(diff != 0.0) {
            TEST_EQUAL_FLOAT(samples[i], val,
                           "samples[%d] ?= val, pos %d", i, pos);
          }
          */
          if(diff != 0.0) errcnt++;

          err += fabs(diff);
          pos++;
        }

        TEST_EQUAL_FLOAT(err, 0.0,
                         "Compare error (offset %d, padding %d, chunksz %d)",
                         offset, padding, chunksz);
        TEST_EQUAL_INT(errcnt, 0,
                       "Compare count (offset %d, padding %d, chunksz %d)",
                       offset, padding, chunksz);
      }

    }
  }
}

deleteTestKit();

TEST_END;

#endif/*TEST_DRUMGIZMO*/
