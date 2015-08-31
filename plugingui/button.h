/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            button.h
 *
 *  Sun Oct  9 13:01:56 CEST 2011
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
#ifndef __DRUMGIZMO_BUTTON_H__
#define __DRUMGIZMO_BUTTON_H__

#include <string>

#include "widget.h"
#include "painter.h"

namespace GUI {

class Button : public Widget {
public:
  Button(Widget *parent);

  bool isFocusable() { return true; }
  bool catchMouse() { return true; }

  void setText(std::string text);

  void registerClickHandler(void (*handler)(void *), void *ptr);

  //protected:
  virtual void clicked() {}

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);

  virtual void mouseLeaveEvent();
  virtual void mouseEnterEvent();
  virtual void mouseMoveEvent(MouseMoveEvent *e);

private:
  bool in_button;

  Painter::Box box_up;
  Painter::Box box_down;

  typedef enum {
    up,
    down
  } state_t;

  std::string text;

  state_t draw_state;
  state_t button_state;

  void (*handler)(void *);
  void *ptr;
};

};

#endif/*__DRUMGIZMO_BUTTON_H__*/
