/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            verticalline.h
 *
 *  Sat Apr  6 12:59:43 CEST 2013
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
#ifndef __DRUMGIZMO_VERTICALLINE_H__
#define __DRUMGIZMO_VERTICALLINE_H__

#include "widget.h"
#include "image.h"

namespace GUI {

class VerticalLine : public Widget {
public:
  VerticalLine(Widget *parent);

  //protected:
  virtual void repaintEvent(RepaintEvent *e);

private:
  Image vline;
};

};

#endif/*__DRUMGIZMO_VERTICALLINE_H__*/
