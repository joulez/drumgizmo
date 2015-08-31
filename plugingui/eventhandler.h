/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            eventhandler.h
 *
 *  Sun Oct  9 18:58:29 CEST 2011
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
#ifndef __DRUMGIZMO_EVENTHANDLER_H__
#define __DRUMGIZMO_EVENTHANDLER_H__

#include "guievent.h"
#include "nativewindow.h"
//#include "window.h"

namespace GUI {
class Window;

class EventHandler {
public:
  EventHandler(NativeWindow *native, Window *window);

  void processEvents();

  bool hasEvent();
  Event *getNextEvent();

  void registerCloseHandler(void (*handler)(void *), void *ptr);

private:
  Window *window;
  int last_click;
  void (*closeHandler)(void *);
  void *closeHandlerPtr;

  // Used to ignore mouse button release after a double click.
  bool last_was_dbl_click;

  NativeWindow *native;
};

};

#endif/*__DRUMGIZMO_EVENTHANDLER_H__*/
