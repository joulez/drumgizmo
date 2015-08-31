/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            progressbar.h
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
#ifndef __DRUMGIZMO_PROGRESSBAR_H__
#define __DRUMGIZMO_PROGRESSBAR_H__

#include "widget.h"

#include <string>

#include "guievent.h"
#include "painter.h"

namespace GUI {

class ProgressBar : public Widget {
public:
  ProgressBar(Widget *parent);

  float progress();
  void setProgress(float progress);

  typedef enum {
    red,
    green,
    blue,
    off
  } state_t;
  void setState(state_t state);

  //protected:
  virtual void repaintEvent(RepaintEvent *e);

private:
  state_t state;

  Painter::Bar bar_bg;

  Painter::Bar bar_green;
  Painter::Bar bar_blue;
  Painter::Bar bar_red;

  float _progress;
};

};

#endif/*__DRUMGIZMO_PROGRESSBAR_H__*/
