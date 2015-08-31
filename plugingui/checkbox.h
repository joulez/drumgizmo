/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            checkbox.h
 *
 *  Sat Nov 26 15:07:44 CET 2011
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
#ifndef __DRUMGIZMO_CHECKBOX_H__
#define __DRUMGIZMO_CHECKBOX_H__

#include "widget.h"
#include "image.h"

namespace GUI {

class CheckBox : public Widget {
public:
  CheckBox(Widget *parent);

  void setText(std::string text);

  bool isFocusable() { return true; }

  bool checked();
  void setChecked(bool checked);

  void registerClickHandler(void (*handler)(void *), void *ptr);

  //protected:
  virtual void clicked() {}

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);
  virtual void keyEvent(KeyEvent *e);

private:
  Image bg_on;
  Image bg_off;
  Image knob;

  bool state;
  bool middle;

  void (*handler)(void *);
  void *ptr;

  std::string _text;
};

};

#endif/*__DRUMGIZMO_CHECKBOX_H__*/
