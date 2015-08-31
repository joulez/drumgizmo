/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            event.h
 *
 *  Sun Oct  9 16:11:47 CEST 2011
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
#ifndef __DRUMGIZMO_GUI_EVENT_H__
#define __DRUMGIZMO_GUI_EVENT_H__

#include <unistd.h>

#include <string>

#ifdef X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif/*X11*/

namespace GUI {

class Event {
public:
  typedef enum {
    MouseMove,
    Repaint,
    Button,
    Scroll,
    Key,
    Close,
    Resize
  } Type;
  virtual ~Event() {}

  virtual Type type() = 0;

#ifdef X11
  ::Window window_id;
#endif/*X11*/
};

class MouseMoveEvent : public Event {
public:
  Type type() { return MouseMove; }

  int x;
  int y;
};

class ButtonEvent : public Event {
public:
  Type type() { return Button; }

  int x;
  int y;

  int direction;
  int button;
  int doubleclick;
};

class ScrollEvent : public Event {
public:
  Type type() { return Scroll; }

  int x;
  int y;

  int delta;
};

class RepaintEvent : public Event {
public:
  Type type() { return Repaint; }

  int x;
  int y;
  size_t width;
  size_t height;
};

class KeyEvent : public Event {
public:
  Type type() { return Key; }

  int direction;
  int keycode;
  std::string text;

  enum {
    KEY_UNKNOWN   =-1,
    KEY_LEFT      = 1,
    KEY_RIGHT     = 2,
    KEY_UP        = 3,
    KEY_DOWN      = 4,
    KEY_DELETE    = 5,
    KEY_BACKSPACE = 6,
    KEY_HOME      = 7,
    KEY_END       = 8,
    KEY_PGDOWN    = 9,
    KEY_PGUP      = 10,
    KEY_ENTER     = 11,
    KEY_CHARACTER = 0xffff // character data is stored in 'text'
  };
};

class CloseEvent : public Event {
public:
  Type type() { return Close; }
};

class ResizeEvent : public Event {
public:
  Type type() { return Resize; }

  size_t width;
  size_t height;
};

};

#endif/*__DRUMGIZMO_GUI_EVENT_H__*/
