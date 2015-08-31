/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            font.cc
 *
 *  Sat Nov 12 11:13:41 CET 2011
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
#include "font.h"

#ifdef X11
#include <X11/Xlib.h>
#endif/*X11*/

GUI::Font::Font(std::string fontfile)
  : img_font(fontfile)
{
  int px = 0;
  int c;
  for(c = 0; c < 255 && px < (int)img_font.width(); c++) {
    character_offset[c] = px + 1;
    character_pre_bias[c] = 0;
    character_post_bias[c] = 0;
    if(c > 0) {
      character_width[c - 1] =
        character_offset[c] - character_offset[c - 1] - 1;
    }
    px++;
    GUI::Colour c;
    while(px < (int)img_font.width()) {
      c = img_font.getPixel(px, 0);
      // Find next purple pixel in top row:
      if(c.red == 1 && c.green == 0 && c.blue == 1 && c.alpha == 1) break;
      px++;
    } 
  }
  c++;
  character_width[c - 1] = character_offset[c] - character_offset[c - 1] - 1;
}
  
void GUI::Font::setFace(std::string face)
{
  face = face;
}

std::string GUI::Font::face()
{
  return "";//face;
}

void GUI::Font::setSize(size_t points)
{
  points = points;
}

size_t GUI::Font::size()
{
  return 0;
}

size_t GUI::Font::textWidth(std::string text)
{
  size_t len = 0;

  for(size_t i = 0; i < text.length(); i++) {
    unsigned int cha = text[i];
    len += character_width[(char)cha] + 1 + character_post_bias[(char)cha];
  }

  return len;
}

size_t GUI::Font::textHeight(std::string text)
{
  return img_font.height();
}

GUI::PixelBufferAlpha *GUI::Font::render(std::string text)
{
  int border = 1;
  PixelBufferAlpha *pb =
    new PixelBufferAlpha(textWidth(text), textHeight(text));

  int x_offset = 0;
  for(size_t i = 0; i < text.length(); i++) {
    unsigned int cha = text[i];
    for(size_t x = 0; x < character_width[(char)cha]; x++) {
      for(size_t y = 0; y < img_font.height(); y++) {
        Colour c = img_font.getPixel(x + (character_offset[(char)cha]), y);
        pb->setPixel(x + x_offset + character_pre_bias[(char)cha], y,
                     c.red * 255, c.green * 255, c.blue * 255, c.alpha * 255);
      }
    }
    x_offset += character_width[(char)cha] + border +
      character_post_bias[(char)cha];
  }

  return pb;
}

#ifdef TEST_FONT
//deps:
//cflags:
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).

TEST_END;

#endif/*TEST_FONT*/
