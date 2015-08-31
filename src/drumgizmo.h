/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumgizmo.h
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
#ifndef __DRUMGIZMO_DRUMGIZMO_H__
#define __DRUMGIZMO_DRUMGIZMO_H__

#include <string>
#include <list>

#include "audiooutputengine.h"
#include "audioinputengine.h"

#include "events.h"
#include "audiofile.h"
#include "drumkit.h"

#include "drumkitloader.h"

#include "mutex.h"

#include "message.h"

#include "messagereceiver.h"

#include "chresampler.h"

#define MAX_NUM_CHANNELS 64

class DrumGizmo : public MessageReceiver {
public:
  DrumGizmo(AudioOutputEngine *outputengine, AudioInputEngine *inputengine);
  virtual ~DrumGizmo();

  bool loadkit(std::string kitfile);

  bool init();

  /**
   * @param endpos number of samples to process, -1 := never stop.
   */
  void run(int endpos);
  bool run(size_t pos, sample_t *samples, size_t nsamples);
  void stop();

  void getSamples(int ch, int pos, sample_t *s, size_t sz);

  std::string configString();
  bool setConfigString(std::string cfg);

  void handleMessage(Message *msg);

  int samplerate();
  void setSamplerate(int samplerate);

private:
  DrumKitLoader loader;

  Mutex mutex;
  bool is_stopping; ///< Is set to true when a TYPE_STOP event has been seen.

  AudioOutputEngine *oe;
  AudioInputEngine *ie;

  std::list< Event* > activeevents[MAX_NUM_CHANNELS];

  CHResampler resampler[MAX_NUM_CHANNELS];
  sample_t resampler_output_buffer[MAX_NUM_CHANNELS][4096];
  sample_t resampler_input_buffer[MAX_NUM_CHANNELS][64];

  std::map<std::string, AudioFile *> audiofiles;

#ifdef TEST_DRUMGIZMO
public:
#endif
  DrumKit kit;
};


#endif/*__DRUMGIZMO_DRUMGIZMO_H__*/
