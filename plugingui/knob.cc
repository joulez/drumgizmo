/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            knob.cc
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
#include "knob.h"

#include "painter.h"

#include <hugin.hpp>
#include <stdio.h>
#include <math.h>

GUI::Knob::Knob(Widget *parent)
  : GUI::Widget(parent), img_knob(":knob.png")
{
  state = up;

  val = 0.0;
  maximum = 1.0;
  minimum = 0.0;
  mouse_offset_x = 0;

  handler = NULL;
  ptr = NULL;
}

void GUI::Knob::setValue(float v)
{
  val = v;
  if(handler) handler(ptr);
  repaintEvent(NULL);
}

float GUI::Knob::value()
{
  return val;
}

void GUI::Knob::registerClickHandler(void (*handler)(void *), void *ptr)
{
  this->handler = handler;
  this->ptr = ptr;
}

void GUI::Knob::scrollEvent(ScrollEvent *e)
{
  val -= e->delta / 200.0;
  if(val < 0) val = 0;
  if(val > 1) val = 1;

  if(handler) handler(ptr);

  repaintEvent(NULL);
}

void GUI::Knob::mouseMoveEvent(MouseMoveEvent *e)
{
  if(state == down) {
    if(mouse_offset_x == (e->x + -1*e->y)) return;

    float dval = mouse_offset_x - (e->x + -1*e->y);
    val -= dval / 300.0;

    if(val < 0) val = 0;
    if(val > 1) val = 1;

    if(handler) handler(ptr);
    repaintEvent(NULL);

    mouse_offset_x = e->x + -1*e->y;
  }
}

void GUI::Knob::keyEvent(KeyEvent *e)
{
  if(e->direction != -1) return;

  switch(e->keycode) {
  case GUI::KeyEvent::KEY_UP:
    val += 0.01;
    break;
  case GUI::KeyEvent::KEY_DOWN:
    val -= 0.01;
    break;
  case GUI::KeyEvent::KEY_RIGHT:
    val += 0.01;
    break;
  case GUI::KeyEvent::KEY_LEFT:
    val -= 0.01;
    break;
  case GUI::KeyEvent::KEY_HOME:
    val = 0;
    break;
  case GUI::KeyEvent::KEY_END:
    val = 1;
    break;
  default:
    break;
  }

  if(val < 0) val = 0;
  if(val > 1) val = 1;
 
  repaintEvent(NULL);
}

void GUI::Knob::buttonEvent(ButtonEvent *e)
{
  if(e->direction == 1) {
    state = down;
    mouse_offset_x = e->x + -1*e->y;
    if(handler) handler(ptr);
    repaintEvent(NULL);
  }
  if(e->direction == -1) {
    state = up;
    mouse_offset_x = e->x + -1*e->y;
    repaintEvent(NULL);
    clicked();
    if(handler) handler(ptr);
  }
}

void GUI::Knob::repaintEvent(GUI::RepaintEvent *e)
{
  int diameter = (width()>height()?height():width());
  int radius = diameter / 2;
  int center_x = width() / 2;
  int center_y = height() / 2;

  Painter p(this);

  p.clear();
  p.drawImageStretched(0, 0, &img_knob, diameter, diameter);

  char buf[64];
  sprintf(buf, "%.2f", val * maximum);
  Font font;
  p.drawText(center_x - font.textWidth(buf) / 2 + 1,
             center_y + font.textHeight(buf) / 2 + 1, font, buf);

  double padval = val * 0.8 + 0.1; // Make it start from 20% and stop at 80%

  double from_x = sin((-1 * padval + 1) * 2 * M_PI) * radius * 0.6;
  double from_y = cos((-1 * padval + 1) * 2 * M_PI) * radius * 0.6;

  double to_x = sin((-1 * padval + 1) * 2 * M_PI) * radius * 0.8;
  double to_y = cos((-1 * padval + 1) * 2 * M_PI) * radius * 0.8;

  // Draw "fat" line by drawing 9 lines with moved start/ending points.
  p.setColour(Colour(1, 0, 0, 1));
  for(int _x = -1; _x < 2; _x++) {
    for(int _y = -1; _y < 2; _y++) {
      p.drawLine(from_x + center_x + _x,
                 from_y + center_y + _y,
                 to_x + center_x + _x,
                 to_y + center_y + _y);

    }
  }
}

#ifdef TEST_KNOB
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

#endif/*TEST_KNOB*/
