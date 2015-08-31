/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            channelmixer.cc
 *
 *  Sun Oct 17 10:14:34 CEST 2010
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
#include "channelmixer.h"

ChannelMixer::ChannelMixer(Channels &c, Channel *defc, float defg)
  : channels(c)
{
  setDefaults(defc, defg);
}

void ChannelMixer::setDefaults(Channel *defc, float defg)
{
  defaultchannel = defc;
  if(defc == NULL && channels.size() > 0) defaultchannel = &channels[0];

  defaultgain = defg;
}

MixerSettings &ChannelMixer::lookup(InstrumentChannel *c)
{
  std::map<InstrumentChannel *, MixerSettings>::iterator mi = mix.find(c);

  if(mi == mix.end()) {
    MixerSettings m;
    m.gain = defaultgain;
    m.output = defaultchannel;
    mix[c] = m;
    return mix[c];
  }

  return mi->second;
}


#ifdef TEST_CHANNELMIXER
//deps: channel.cc
//cflags:
//libs:
#include "test.h"

TEST_BEGIN;

Channel ch1;
Channel ch2;
Channels channels;
channels.push_back(ch1);
channels.push_back(ch2);

{
  ChannelMixer mixer(channels, NULL, 1.0);

  InstrumentChannel ich;
  MixerSettings &m = mixer.lookup(&ich);
  TEST_EQUAL(m.output, &channels[0], "Default channel?");
  TEST_EQUAL_FLOAT(m.gain, 1.0, "Default gain?");
}

{
  ChannelMixer mixer(channels, &channels[1]);

  InstrumentChannel ich;
  MixerSettings &m = mixer.lookup(&ich);
  TEST_EQUAL(m.output, &channels[1], "Default channel?");
  TEST_EQUAL_FLOAT(m.gain, 1.0, "Default gain?");
}

{
  ChannelMixer mixer(channels, &channels[1]);

  InstrumentChannel ich;
  MixerSettings &m = mixer.lookup(&ich);
  TEST_EQUAL(m.output, &channels[1], "Default channel?");
  TEST_EQUAL_FLOAT(m.gain, 1.0, "Default gain?");

  m.output = &channels[0];
  m.gain = 0.5;

  MixerSettings &m2 = mixer.lookup(&ich);
  TEST_EQUAL(m2.output, &channels[0], "Changed channel?");
  TEST_EQUAL_FLOAT(m2.gain, 0.5, "Changed gain?");
}


TEST_END;

#endif/*TEST_CHANNELMIXER*/
