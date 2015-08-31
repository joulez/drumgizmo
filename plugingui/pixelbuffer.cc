/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            pixelbuffer.cc
 *
 *  Thu Nov 10 09:00:38 CET 2011
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
#include "pixelbuffer.h"

#include <stdio.h>

GUI::PixelBuffer::PixelBuffer(size_t width, size_t height)
{
  buf = NULL;
  realloc(width, height);
}

GUI::PixelBuffer::~PixelBuffer()
{
  if(buf) free(buf);
}

void GUI::PixelBuffer::realloc(size_t width, size_t height)
{
  if(buf) free(buf);
  buf = (unsigned char *)calloc(width * height, 3);
  this->width = width;
  this->height = height;
}

#define PX(k) (x + y * width) * 3 + k
void GUI::PixelBuffer::setPixel(size_t x, size_t y,
                                unsigned char red,
                                unsigned char green,
                                unsigned char blue,
                                unsigned char alpha)
{
  if(x >= width || y >= height) return;
  /*
  float a,b;
  getAlpha(alpha, 255, a, b);

  buf[PX(0)] = (unsigned char)((float)red   * a + (float)buf[PX(0)] * b);
  buf[PX(0)] /= (a + b);
  buf[PX(1)] = (unsigned char)((float)green * a + (float)buf[PX(1)] * b);
  buf[PX(1)] /= (a + b);
  buf[PX(2)] = (unsigned char)((float)blue  * a + (float)buf[PX(2)] * b);
  buf[PX(2)] /= (a + b);
  */
  unsigned int a = alpha;
  unsigned int b = 255 - alpha;
  
  buf[PX(0)] = (unsigned char)(((int)red   * a + (int)buf[PX(0)] * b) / 255);
  buf[PX(1)] = (unsigned char)(((int)green * a + (int)buf[PX(1)] * b) / 255);
  buf[PX(2)] = (unsigned char)(((int)blue  * a + (int)buf[PX(2)] * b) / 255);

}

static int idx = 0;
GUI::PixelBufferAlpha::PixelBufferAlpha(size_t width, size_t height)
{
  this->idx = ::idx++;
  buf = NULL;
  x = y = 10;
  realloc(width, height);
}

GUI::PixelBufferAlpha::~PixelBufferAlpha()
{
  if(buf) free(buf);
}

void GUI::PixelBufferAlpha::realloc(size_t width, size_t height)
{
  if(buf) free(buf);
  buf = (unsigned char *)calloc(width * height, 4);
  this->width = width;
  this->height = height;
}

#undef PX
#define PX(k) (x + y * width) * 4 + k
void GUI::PixelBufferAlpha::setPixel(size_t x, size_t y,
                                     unsigned char red,
                                     unsigned char green,
                                     unsigned char blue,
                                     unsigned char alpha)
{
  if(x >= width || y >= height) return;

  buf[PX(0)] = red;
  buf[PX(1)] = green;
  buf[PX(2)] = blue;
  buf[PX(3)] = alpha;
}

// http://en.wikipedia.org/wiki/Alpha_compositing
static void getAlpha(unsigned char _a, unsigned char _b, float &a, float &b)
{
  a = _a / 255.0;
  b = _b / 255.0;
  b *= (1-a);
}

void GUI::PixelBufferAlpha::addPixel(size_t x, size_t y,
                                     unsigned char red,
                                     unsigned char green,
                                     unsigned char blue,
                                     unsigned char alpha)
{
  if(x >= width || y >= height) return;

  if(alpha == 0) return;

  float a,b;
  getAlpha(alpha, buf[PX(3)], a, b);

  buf[PX(0)] = (unsigned char)((float)red   * a + (float)buf[PX(0)] * b);
  buf[PX(0)] /= (a + b);
  buf[PX(1)] = (unsigned char)((float)green * a + (float)buf[PX(1)] * b);
  buf[PX(1)] /= (a + b);
  buf[PX(2)] = (unsigned char)((float)blue  * a + (float)buf[PX(2)] * b);
  buf[PX(2)] /= (a + b);

  buf[PX(3)] = (a + b) * 255;
}

void GUI::PixelBufferAlpha::addPixel(size_t x, size_t y, GUI::Colour c)
{
  addPixel(x, y, c.red * 255, c.green * 255, c.blue * 255, c.alpha * 255);
}

void GUI::PixelBufferAlpha::pixel(size_t x, size_t y,
                                  unsigned char *red,
                                  unsigned char *green,
                                  unsigned char *blue,
                                  unsigned char *alpha)
{
  if(x >= width || y >= height) return;
  *red = buf[PX(0)];
  *green = buf[PX(1)];
  *blue = buf[PX(2)];
  *alpha = buf[PX(3)];
}

#ifdef TEST_PIXELBUFFER
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

#endif/*TEST_PIXELBUFFER*/
