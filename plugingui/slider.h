/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            slider.h
 *
 *  Sat Nov 26 18:10:22 CET 2011
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
#ifndef __DRUMGIZMO_SLIDER_H__
#define __DRUMGIZMO_SLIDER_H__

#include "widget.h"

namespace GUI {

class Slider : public Widget {
public:
  Slider(Widget *parent);

  bool catchMouse() { return true; }

  void setValue(float value);
  float value();

  void registerClickHandler(void (*handler)(void *), void *ptr);

  //protected:
  virtual void clicked() {}

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);
  virtual void mouseMoveEvent(MouseMoveEvent *e);

private:
  typedef enum {
    up,
    down
  } state_t;

  float val;
  float maximum;
  float minimum;

  state_t state;

  void (*handler)(void *);
  void *ptr;
};

};

#endif/*__DRUMGIZMO_SLIDER_H__*/
