/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            scrollbar.cc
 *
 *  Sun Apr 14 12:54:58 CEST 2013
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
#include "scrollbar.h"

#include <hugin.hpp>

#include "painter.h"

GUI::ScrollBar::ScrollBar(GUI::Widget *parent)
  : GUI::Widget(parent), bg_img(":widget_c.png")
{
  handler = NULL;
  ptr = NULL;
}

void GUI::ScrollBar::setRange(int r)
{
  DEBUG(scroll, "%d\n", r);
  ran = r;
  setValue(value());
  repaintEvent(NULL);
}

int GUI::ScrollBar::range()
{
  return ran;
}

void GUI::ScrollBar::setMaximum(int m)
{
  DEBUG(scroll, "%d\n", m);
  max = m;
  if(max < ran) ran = max;
  setValue(value());
  repaintEvent(NULL);
}

int GUI::ScrollBar::maximum()
{
  return max;
}

void GUI::ScrollBar::setValue(int value)
{
  val = value;
  if(val > max - ran) val = max - ran;
  if(val < 0) val = 0;

  if(handler) handler(ptr);

  repaintEvent(NULL);
}

int GUI::ScrollBar::value()
{
  return val;
}

void GUI::ScrollBar::registerValueChangeHandler(void (*handler)(void *),
                                                void *ptr)
{
  this->handler = handler;
  this->ptr = ptr;
}

static void drawArrow(GUI::Painter &p, int x, int y, int w, int h)
{
  if(h < 0) y -= h;

  p.drawLine(x, y, x+(w/2), y+h);
  p.drawLine(x+(w/2), y+h, x+w, y);

  y++;
  p.drawLine(x, y, x+(w/2), y+h);
  p.drawLine(x+(w/2), y+h, x+w, y);
}

void GUI::ScrollBar::repaintEvent(RepaintEvent *e)
{
  GUI::Painter p(this);

  p.clear();

  p.drawImageStretched(0, 0, &bg_img, width(), height());

  p.setColour(GUI::Colour(183.0/255.0, 219.0/255.0 , 255.0/255.0, 1));
  if(!max) return;

  {
  int h = height() - 2 * width() - 3;
  int offset = width() + 2;

  int y_val1 = (val * h) / max; 
  int y_val2 = ((val + ran) * h) / max;  

  p.drawFilledRectangle(2, y_val1 + offset, width() - 1, y_val2 + offset);
  }

  p.drawLine(0, 0, 0, height());

  drawArrow(p, width()/4, width()/4, width()/2, -1 * (width()/3));
  p.drawLine(0, width(), width(), width());

  drawArrow(p, width()/4, height() - width() + width()/4, width()/2, width()/3);
  p.drawLine(0, height() - width(), width(), height() - width());
}

void GUI::ScrollBar::scrollEvent(ScrollEvent *e)
{
  setValue(value() + e->delta);
}

void GUI::ScrollBar::mouseMoveEvent(MouseMoveEvent *e)
{
  if(!dragging) return;

  float delta = yoffset - e->y;

  int h = height() - 2 * width() - 3;
  delta /= (float)h / (float)max;

  int newval = value_offset - delta;
  if(newval != value()) setValue(newval);
}

void GUI::ScrollBar::buttonEvent(ButtonEvent *e)
{
  if(e->y < (int)width() && e->y > 0) {
    if(e->direction == -1) setValue(value() - 1);
    return;
  }

  if(e->y > (int)height() - (int)width() && e->y < (int)height()) {
    if(e->direction == -1) setValue(value() + 1);
    return;
  }

  if(e->direction == 1) {
    yoffset = e->y;
    value_offset = value();
  }

  dragging = (e->direction == 1);
}

#ifdef TEST_SCROLLBAR
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

#endif/*TEST_SCROLLBAR*/
