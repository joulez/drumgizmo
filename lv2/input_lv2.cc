/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            input_lv2.cc
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
#include "input_lv2.h"

#include "lv2/lv2plug.in/ns/ext/atom/util.h"

#include <midimapparser.h>

#include <hugin.hpp>

InputLV2::InputLV2()
{
  eventPort = NULL;
}

InputLV2::~InputLV2()
{
}

bool InputLV2::init(Instruments &i)
{
  instruments = &i;
  return true;
}

void InputLV2::setParm(std::string parm, std::string value)
{
}

bool InputLV2::start()
{
  return true;
}

void InputLV2::stop()
{
}

void InputLV2::pre()
{
}

event_t *InputLV2::run(size_t pos, size_t len, size_t *nevents)
{
  if(eventPort == NULL) {
    *nevents = 0;
    return NULL;
  }

  event_t *list;
  size_t listsize;

  list = (event_t *)malloc(sizeof(event_t) * 1000);
  listsize = 0;

  LV2_Atom_Event* ev = lv2_atom_sequence_begin(&eventPort->body);

  while(!lv2_atom_sequence_is_end(&eventPort->body,
                                  eventPort->atom.size, 
                                  ev)) {
    uint8_t* const data = (uint8_t*)(ev+1);

    if ((data[0] & 0xF0) == 0x80) { // note off
      int key = data[1];
    
      DEBUG(lv2input, "Event (off) key:%d\n", key);
    }

    if ((data[0] & 0xF0) == 0x90) { // note on
      int key = data[1];
      int velocity = data[2];
    
      DEBUG(lv2input, "Event key:%d vel:%d\n", key, velocity);
    
      int i = mmap.lookup(key);
      if(velocity && i != -1) {
        list[listsize].type = TYPE_ONSET;
        list[listsize].instrument = i;
        list[listsize].velocity = velocity / 127.0;
        list[listsize].offset = ev->time.frames;
        listsize++;
      }
    }
    ev = lv2_atom_sequence_next(ev);
  }

  *nevents = listsize;
  return list;
}

void InputLV2::post()
{
}
