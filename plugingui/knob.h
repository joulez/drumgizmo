/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            knob.h
 *
 *  Thu Feb 28 07:37:27 CET 2013
 *  Copyright 2013 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_KNOB_H__
#define __DRUMGIZMO_KNOB_H__

#include "widget.h"

#include "image.h"

namespace GUI {

class Knob : public Widget {
public:
  Knob(Widget *parent);

  bool catchMouse() { return true; }
  bool isFocusable() { return true; }

  void setValue(float value);
  float value();

  void registerClickHandler(void (*handler)(void *), void *ptr);

  //protected:
  virtual void clicked() {}

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);
  virtual void mouseMoveEvent(MouseMoveEvent *e);
  virtual void scrollEvent(ScrollEvent *e);
  virtual void keyEvent(KeyEvent *e);

private:
  typedef enum {
    up,
    down
  } state_t;

  float val;
  float maximum;
  float minimum;

  state_t state;

  GUI::Image img_knob;

  void (*handler)(void *);
  void *ptr;

  int mouse_offset_x;

};

};

#endif/*__DRUMGIZMO_KNOB_H__*/
