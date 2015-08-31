/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            button.cc
 *
 *  Sun Oct  9 13:01:56 CEST 2011
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
#include "button.h"

#include "painter.h"

#include <stdio.h>
#include <hugin.hpp>

GUI::Button::Button(Widget *parent)
  : GUI::Widget(parent)
{
  box_up.topLeft     = new Image(":pushbutton_tl.png");
  box_up.top         = new Image(":pushbutton_t.png");
  box_up.topRight    = new Image(":pushbutton_tr.png");
  box_up.left        = new Image(":pushbutton_l.png");
  box_up.right       = new Image(":pushbutton_r.png");
  box_up.bottomLeft  = new Image(":pushbutton_bl.png");
  box_up.bottom      = new Image(":pushbutton_b.png");
  box_up.bottomRight = new Image(":pushbutton_br.png");
  box_up.center      = new Image(":pushbutton_c.png");

  box_down.topLeft     = new Image(":pushbuttondown_tl.png");
  box_down.top         = new Image(":pushbuttondown_t.png");
  box_down.topRight    = new Image(":pushbuttondown_tr.png");
  box_down.left        = new Image(":pushbuttondown_l.png");
  box_down.right       = new Image(":pushbuttondown_r.png");
  box_down.bottomLeft  = new Image(":pushbuttondown_bl.png");
  box_down.bottom      = new Image(":pushbuttondown_b.png");
  box_down.bottomRight = new Image(":pushbuttondown_br.png");
  box_down.center      = new Image(":pushbuttondown_c.png");

  draw_state = up;
  button_state = up;

  handler = NULL;
  ptr = NULL;
}

void GUI::Button::registerClickHandler(void (*handler)(void *), void *ptr)
{
  this->handler = handler;
  this->ptr = ptr;
}

void GUI::Button::buttonEvent(ButtonEvent *e)
{
  if(e->direction == 1) {
    draw_state = down;
    button_state = down;
    in_button = true;
    repaintEvent(NULL);
  }
  if(e->direction == -1) {
    draw_state = up;
    button_state = up;
    repaintEvent(NULL);
    if(in_button) {
      clicked();
      if(handler) handler(ptr);
    }
  }
}

void GUI::Button::repaintEvent(GUI::RepaintEvent *e)
{
  Painter p(this);

  p.clear();

  int w = width();
  int h = height();
  if(w == 0 || h == 0) return;

  switch(draw_state) {
  case up:
    p.drawBox(0, 0, &box_up, w, h);
    break;
  case down:
    p.drawBox(0, 0, &box_down, w, h);
    break;
  }    

  Font font(":fontemboss.png");
  p.setColour(Colour(0.1));
  p.drawText(width()/2-(text.length()*3)+(draw_state==up?0:1),
             height()/2+5+1+(draw_state==up?0:1), font, text, true);
}

void GUI::Button::setText(std::string text)
{
  this->text = text;
  repaintEvent(NULL);
}

void GUI::Button::mouseLeaveEvent()
{
  in_button = false;
  if(button_state == down) {
    draw_state = up;
    repaintEvent(NULL);
  }
}

void GUI::Button::mouseEnterEvent()
{
  in_button = true;
  if(button_state == down) {
    draw_state = down;
    repaintEvent(NULL);
  }
}

void GUI::Button::mouseMoveEvent(MouseMoveEvent *e)
{
}

#ifdef TEST_BUTTON
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

#endif/*TEST_BUTTON*/
