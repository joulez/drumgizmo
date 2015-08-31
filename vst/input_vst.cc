/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            input_vst.cc
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
#include "input_vst.h"

#include <midimapparser.h>
#include <hugin.hpp>

InputVST::InputVST()
{
  list = NULL;
  listsize = 0;
}

InputVST::~InputVST()
{
}

bool InputVST::init(Instruments &i)
{
  DEBUG(inputvst, "init\n");
  instruments = &i;
  return true;
}

void InputVST::setParm(std::string parm, std::string value)
{
}

bool InputVST::start()
{
  return true;
}

void InputVST::stop()
{
}

void InputVST::pre()
{
}

event_t *InputVST::run(size_t pos, size_t len, size_t *nevents)
{
  *nevents = listsize;
  return list;
}

void InputVST::post()
{
  list = NULL;
  listsize = 0;
}

void InputVST::processEvents(VstEvents* ev)
{
  if(list == NULL) {
    list = (event_t *)malloc(sizeof(event_t) * 1000);
    //    listsize = 0;
  }

	for(VstInt32 i = 0; i < ev->numEvents; i++)	{
		if(ev->events[i]->type != kVstMidiType)	continue;

		VstMidiEvent* event =(VstMidiEvent*)ev->events[i];
		char* midiData = event->midiData;
		VstInt32 status = midiData[0] & 0xf0; // ignoring channel
		if(status == 0x90) { // we only look at notes
			VstInt32 note = midiData[1] & 0x7f;
			VstInt32 velocity = midiData[2] & 0x7f;

      int i = mmap.lookup(note);
      DEBUG(inputvst, "Note: %d -> %d\n", note, i);
      if(velocity && i != -1) {
        list[listsize].type = TYPE_ONSET;
        list[listsize].instrument = i;
        list[listsize].velocity = velocity / 127.0;
        list[listsize].offset = event->deltaFrames;
        listsize++;
      }

		}
		event++;
	}
}
