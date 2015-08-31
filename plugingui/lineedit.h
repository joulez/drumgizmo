/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lineedit.h
 *
 *  Sun Oct  9 13:01:52 CEST 2011
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
#ifndef __DRUMGIZMO_LINEEDIT_H__
#define __DRUMGIZMO_LINEEDIT_H__


#include <string>

#include "widget.h"
#include "font.h"
#include "painter.h"

namespace GUI {

class LineEdit : public Widget {
public:
  LineEdit(Widget *parent);

  bool isFocusable() { return true; }

  std::string text();
  void setText(std::string text);

  void setReadOnly(bool readonly);
  bool readOnly();

  void registerEnterPressedHandler(void (*handler)(void *), void *ptr);

  //protected:
  virtual void keyEvent(KeyEvent *e);
  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);

protected:
  virtual void textChanged() {}

private:
  Painter::Box box;

  Font font;

  std::string _text;
  size_t pos;
  std::string _visibletext;
  size_t offsetpos;

  enum state_t {
    NOOP = 0,
    WALK_LEFT =  1, 
    WALK_RIGHT = 2
  };
  state_t walkstate;

  bool readonly;

  void (*handler)(void *);
  void *ptr;
};

};

#endif/*__DRUMGIZMO_LINEEDIT_H__*/
