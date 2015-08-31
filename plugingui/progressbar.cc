/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            progressbar.cc
 *
 *  Fri Mar 22 22:07:57 CET 2013
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
#include "progressbar.h"

#include "painter.h"

GUI::ProgressBar::ProgressBar(GUI::Widget *parent) : GUI::Widget(parent)
{
  bar_bg.left    = new Image(":progress_back_l.png");
  bar_bg.right   = new Image(":progress_back_r.png");
  bar_bg.center  = new Image(":progress_back_c.png");

  bar_blue.left    = new Image(":progress_front_blue_l.png");
  bar_blue.right   = new Image(":progress_front_blue_r.png");
  bar_blue.center  = new Image(":progress_front_blue_c.png");

  bar_red.left    = new Image(":progress_front_red_l.png");
  bar_red.right   = new Image(":progress_front_red_r.png");
  bar_red.center  = new Image(":progress_front_red_c.png");

  bar_green.left    = new Image(":progress_front_green_l.png");
  bar_green.right   = new Image(":progress_front_green_r.png");
  bar_green.center  = new Image(":progress_front_green_c.png");

  state = blue;
  _progress = .5;
}

void GUI::ProgressBar::setState(GUI::ProgressBar::state_t state)
{
  if(this->state != state) {
    this->state = state;
    repaintEvent(NULL);
  }
}

float GUI::ProgressBar::progress()
{
  return _progress;
}

void GUI::ProgressBar::setProgress(float progress)
{
  _progress = progress;
  repaintEvent(NULL);
}

void GUI::ProgressBar::repaintEvent(GUI::RepaintEvent *e)
{
  Painter p(this);

  int max = width() * _progress;

  p.clear();

  int brd = 4;
  p.drawBar(0, 0, &bar_bg, width(), height());

  GUI::Painter::Bar *b = NULL;
  switch(state) {
  case red: b = &bar_red; break;
  case green: b = &bar_green; break;
  case blue: b = &bar_blue; break;
  default: break;    
  }
  if(b) p.drawBar(brd, 0, b, max - 2*brd, height());
}

#ifdef TEST_PROGRESSBAR
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

#endif/*TEST_PROGRESSBAR*/
