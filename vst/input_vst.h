/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            input_vst.h
 *
 *  Tue Sep 20 10:40:10 CEST 2011
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
#ifndef __DRUMGIZMO_INPUT_VST_H__
#define __DRUMGIZMO_INPUT_VST_H__

#include <audioinputenginemidi.h>

#include <public.sdk/source/vst2.x/audioeffectx.h>

class InputVST : public AudioInputEngineMidi {
public:
  InputVST();
  virtual ~InputVST();

  bool init(Instruments &instruments);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

  void processEvents(VstEvents* ev);

private:
  event_t *list;
  size_t listsize;

  Instruments *instruments;
};

#endif/*__DRUMGIZMO_INPUT_VST_H__*/
