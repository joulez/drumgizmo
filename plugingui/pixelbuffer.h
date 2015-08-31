/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            pixelbuffer.h
 *
 *  Thu Nov 10 09:00:37 CET 2011
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
#ifndef __DRUMGIZMO_PIXELBUFFER_H__
#define __DRUMGIZMO_PIXELBUFFER_H__

#include <stdlib.h>

#include "colour.h"

namespace GUI {

class PixelBuffer {
public:
  PixelBuffer(size_t width, size_t height);
  ~PixelBuffer();

  void realloc(size_t width, size_t height);

  void setPixel(size_t x, size_t y,
                unsigned char red,
                unsigned char green,
                unsigned char blue,
                unsigned char alpha);

  unsigned char *buf;
  size_t width;
  size_t height;
};

class PixelBufferAlpha {
public:
  PixelBufferAlpha(size_t width, size_t height);
  ~PixelBufferAlpha();

  int idx;
  size_t x, y;

  void realloc(size_t width, size_t height);

  void setPixel(size_t x, size_t y,
                unsigned char red,
                unsigned char green,
                unsigned char blue,
                unsigned char alpha);

  void addPixel(size_t x, size_t y,
                unsigned char red,
                unsigned char green,
                unsigned char blue,
                unsigned char alpha);

  void addPixel(size_t x, size_t y, Colour c);

  void pixel(size_t x, size_t y,
             unsigned char *red,
             unsigned char *green,
             unsigned char *blue,
             unsigned char *alpha);

  unsigned char *buf;
  size_t width;
  size_t height;
};

};

#endif/*__DRUMGIZMO_PIXELBUFFER_H__*/
