/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            image.cc
 *
 *  Sat Mar 16 15:05:09 CET 2013
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
#include "image.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include <hugin.hpp>

#include "resource.h"
// http://blog.hammerian.net/2009/reading-png-images-from-memory/

#include "lodepng/lodepng.h"

GUI::Image::Image(const char* data, size_t size)
{
  load(data, size);
}

GUI::Image::Image(std::string filename)
{
  GUI::Resource rc(filename);
  load(rc.data(), rc.size());
}

GUI::Image::~Image()
{
  free(image_data);
}

void GUI::Image::setError(int err)
{
  GUI::Resource rc(":png_error");

  const unsigned char *p = (const unsigned char *)rc.data();

  uint32_t iw, ih;

  memcpy(&iw, p, sizeof(uint32_t)); p += sizeof(uint32_t);
  memcpy(&ih, p, sizeof(uint32_t)); p += sizeof(uint32_t);

  w = iw;
  h = ih;

  DEBUG(image, "w:%d, h:%d\n", (int)w, (int)h);

  image_data = (unsigned char*)malloc(rc.size() - 8);
  memcpy(image_data, p, rc.size() - 8);
}

void GUI::Image::load(const char* data, size_t size)
{
  //unsigned lodepng_decode32(unsigned char** out, unsigned* w, unsigned* h,
  //                          const unsigned char* in, size_t insize);
  unsigned iw, ih;
  unsigned res = lodepng_decode32((unsigned char**)&image_data, &iw, &ih,
                                  (const unsigned char*)data, size);
  w = iw;
  h = ih;

  if(res != 0) {
    ERR(image, "[read_png_file] Error during init_io");
    setError(3);
    return;
  }
}

size_t GUI::Image::width()
{
  return w;
}

size_t GUI::Image::height()
{
  return h;
}

GUI::Colour GUI::Image::getPixel(size_t x, size_t y)
{
  if(x > width() || y > height()) return GUI::Colour(0,0,0,0);
  unsigned char *ptr = &image_data[(x + y * width()) * 4];
  float r = ptr[0];
  float g = ptr[1];
  float b = ptr[2];
  float a = ptr[3];
  GUI::Colour c(r / 255.0,
                g / 255.0,
                b / 255.0,
                a / 255.0);
  return c;
}
