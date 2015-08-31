/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            nativewindow.h
 *
 *  Fri Dec 28 18:46:01 CET 2012
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
#ifndef __DRUMGIZMO_NATIVEWINDOW_H__
#define __DRUMGIZMO_NATIVEWINDOW_H__

#include <string>

#include "guievent.h"

namespace GUI {

class NativeWindow {
public:
  NativeWindow() {}
  virtual ~NativeWindow() {}

  virtual void setFixedSize(int width, int height) = 0;
  virtual void resize(int width, int height) = 0;
  virtual void move(int x, int y) = 0;
  virtual void show() = 0;
  virtual void setCaption(const std::string &caption) = 0;
  virtual void hide() = 0;
  virtual void handleBuffer() = 0;
  virtual void redraw() = 0;
  virtual void grabMouse(bool grab) = 0;

  virtual bool hasEvent() = 0;
  virtual Event *getNextEvent() = 0;
};

};

#endif/*__DRUMGIZMO_NATIVEWINDOW_H__*/
