/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            window.h
 *
 *  Sun Oct  9 13:11:52 CEST 2011
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
#ifndef __DRUMGIZMO_WINDOW_H__
#define __DRUMGIZMO_WINDOW_H__

#include "widget.h"

#include "pixelbuffer.h"
#include "nativewindow.h"
#include "image.h"
#include "eventhandler.h"

namespace GUI {

class Window : public Widget {
public:
  Window();
  ~Window();

  void show();
  void hide();

  void setFixedSize(int width, int height);
  void resize(int width, int height);
  void move(size_t x, size_t y);

  size_t x();
  size_t y();
  size_t windowX();
  size_t windowY();
  size_t width();
  size_t height();

  void setCaption(std::string caption);

  void addChild(Widget *widget);

  void repaintEvent(GUI::RepaintEvent *e);

  void beginPaint();
  void endPaint();

  Window *window();

  EventHandler *eventHandler();

  // handlers
  virtual void redraw();
  void resized(size_t w, size_t h);

  Widget *keyboardFocus();
  void setKeyboardFocus(Widget *widget);

  Widget *buttonDownFocus();
  void setButtonDownFocus(Widget *widget);

  Widget *mouseFocus();
  void setMouseFocus(Widget *widget);

  PixelBuffer wpixbuf;
  void updateBuffer();

protected:
  size_t refcount;

  Widget *_keyboardFocus;
  Widget *_buttonDownFocus;
  Widget *_mouseFocus;

  NativeWindow *native;
  EventHandler *eventhandler;

  Image back;
  Image logo;

  size_t max_refcount;
};

};

#endif/*__DRUMGIZMO_WINDOW_H__*/
