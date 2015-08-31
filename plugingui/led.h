/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            led.h
 *
 *  Sat Oct 15 19:12:33 CEST 2011
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
#ifndef __DRUMGIZMO_LED_H__
#define __DRUMGIZMO_LED_H__

#include "widget.h"

namespace GUI {

class LED : public Widget {
public:
  typedef enum {
    red,
    green,
    blue,
    off
  } state_t;

  LED(Widget *parent);

  void setState(state_t state);

  //protected:
  void repaintEvent(RepaintEvent *e);

private:
  state_t state;
};

};

#endif/*__DRUMGIZMO_LED_H__*/
