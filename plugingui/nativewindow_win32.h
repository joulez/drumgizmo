/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            nativewindow_win32.h
 *
 *  Fri Dec 28 18:45:51 CET 2012
 *  Copyright 2012 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_NATIVEWINDOW_WIN32_H__
#define __DRUMGIZMO_NATIVEWINDOW_WIN32_H__

#ifdef WIN32

#include "nativewindow.h"

#include <windows.h>
typedef HWND WNDID;

namespace GUI {

class Window;
class Event;

class NativeWindowWin32 : public NativeWindow {
public:
  NativeWindowWin32(GUI::Window *window);
  ~NativeWindowWin32();

  void setFixedSize(int width, int height);
  void resize(int width, int height);
  void move(int x, int y);
  void show();
  void setCaption(const std::string &caption);
  void hide();
  void handleBuffer();
  void redraw();
  void grabMouse(bool grab);

  bool hasEvent();
  Event *getNextEvent();

  // Needed by dialogProc
  GUI::Window *window;
	WNDID	m_hwnd;
	char *m_className;
  Event *event;
};

};

#endif/*WIN32*/

#endif/*__DRUMGIZMO_NATIVEWINDOW_WIN32_H__*/
