/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            input_lv2.h
 *
 *  Wed Jul 13 14:27:02 CEST 2011
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
#ifndef __DRUMGIZMO_INPUT_LV2_H__
#define __DRUMGIZMO_INPUT_LV2_H__

#include <audioinputenginemidi.h>

#include <lv2/lv2plug.in/ns/ext/atom/atom.h>

class InputLV2 : public AudioInputEngineMidi {
public:
  InputLV2();
  ~InputLV2();

  bool init(Instruments &instruments);

  void setParm(std::string parm, std::string value);

  bool start();
  void stop();

  void pre();
  event_t *run(size_t pos, size_t len, size_t *nevents);
  void post();

  LV2_Atom_Sequence *eventPort;

private:
  Instruments *instruments;
};

#endif/*__DRUMGIZMO_INPUT_LV2_H__*/
