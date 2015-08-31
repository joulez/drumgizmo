/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            slider.cc
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
#include "slider.h"

#include "painter.h"

#include <hugin.hpp>
#include <stdio.h>

GUI::Slider::Slider(Widget *parent)
  : GUI::Widget(parent)
{
  state = up;

  val = 0.0;
  maximum = 1.0;
  minimum = 0.0;

  handler = NULL;
  ptr = NULL;
}

void GUI::Slider::setValue(float v)
{
  val = v;
  if(handler) handler(ptr);
  repaintEvent(NULL);
}

float GUI::Slider::value()
{
  return val;
}

void GUI::Slider::registerClickHandler(void (*handler)(void *), void *ptr)
{
  this->handler = handler;
  this->ptr = ptr;
}

void GUI::Slider::mouseMoveEvent(MouseMoveEvent *e)
{
  if(state == down) {
    val = maximum / (float)width() * (float)e->x;

    if(val < 0) val = 0;
    if(val > 1) val = 1;

    if(handler) handler(ptr);
    repaintEvent(NULL);
  }
}

void GUI::Slider::buttonEvent(ButtonEvent *e)
{
  if(e->direction == 1) {
    state = down;
    val = maximum / (float)width() * (float)e->x;

    if(val < 0) val = 0;
    if(val > 1) val = 1;

    if(handler) handler(ptr);
    repaintEvent(NULL);
  }
  if(e->direction == -1) {
    state = up;
    val = maximum / (float)width() * (float)e->x;
    
    if(val < 0) val = 0;
    if(val > 1) val = 1;

    repaintEvent(NULL);
    clicked();
    if(handler) handler(ptr);
  }
}

void GUI::Slider::repaintEvent(GUI::RepaintEvent *e)
{
  //DEBUG(slider, "Slider::repaintEvent (%f)\n", val);

  Painter p(this);

  float alpha = 0.8;

  int xpos = (int)((val / maximum) * (float)(width() - 1));

  if(hasKeyboardFocus()) {
    p.setColour(Colour(0.6, alpha));
  } else {
    p.setColour(Colour(0.5, alpha));
  }
  p.drawFilledRectangle(0,0,width(),height());
  /*
  p.setColour(Colour(0.1, alpha));
  p.drawRectangle(0,0,width()-1,height()-1);
  */
  p.setColour(Colour(1, 0, 0, alpha));
  p.drawLine(xpos, 0, xpos, height()-1);
  /*
  p.setColour(Colour(0.8, alpha));
  switch(state) {
  case up:
    p.drawLine(0,0,0,height()-1);
    p.drawLine(0,0,width()-1,0);
    break;
  case down:
    p.drawLine(width()-1,0, width()-1,height()-1);
    p.drawLine(width()-1,height()-1,0, height()-1);
    break;
  }
  */
  p.setColour(Colour(0.3, alpha));
  p.drawPoint(0,height()-1);
  p.drawPoint(width()-1,0);
}

#ifdef TEST_SLIDER
//Additional dependency files
//deps:
//Required cflags (autoconf vars may be used)
//cflags:
//Required link options (autoconf vars may be used)
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).

TEST_END;

#endif/*TEST_SLIDER*/
