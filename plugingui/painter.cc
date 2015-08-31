/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            painter.cc
 *
 *  Wed Oct 12 19:48:45 CEST 2011
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
#include "painter.h"

#include "window.h"

#include <string.h>

GUI::Painter::Painter(GUI::Widget *widget)
{
  this->widget = widget;
  widget->window()->beginPaint();
  pixbuf = &widget->pixbuf;
  colour = Colour(0, 0, 0, 0.5);
}

GUI::Painter::~Painter()
{
  widget->window()->endPaint();
  flush();
}

void GUI::Painter::setColour(Colour colour)
{
  this->colour = colour;
}

void GUI::Painter::plot(int x, int y, double c)
{
  // plot the pixel at (x, y) with brightness c (where 0 ≤ c ≤ 1)
  pixbuf->addPixel(x, y,
                   (unsigned char)(colour.red * 255.0),
                   (unsigned char)(colour.green * 255.0),
                   (unsigned char)(colour.blue * 255.0),
                   (unsigned char)(colour.alpha * 255 * c));

}

#include <math.h>
double GUI::Painter::ipart(double x)
{
  return floor(x); //integer part of x'
}
 
double GUI::Painter::round(double x)
{
  return ipart(x + 0.5);
}
 
double GUI::Painter::fpart(double x)
{
  return x - ipart(x);//'fractional part of x'
}
 
double GUI::Painter::rfpart(double x)
{
  return 1 - fpart(x);
}


#define SWAP(x, y) { int tmp = x; x = y; y = tmp; }
void GUI::Painter::drawLine(int x0, int y0, int x1, int y1)
{
  bool steep = abs(y1 - y0) > abs(x1 - x0);
 
  if(steep) {
    SWAP(x0, y0);
    SWAP(x1, y1);
  }
  if(x0 > x1) {
    SWAP(x0, x1);
    SWAP(y0, y1);
  }
 
  double dx = x1 - x0;
  double dy = y1 - y0;
  double gradient = dy / dx;
 
  // Handle first endpoint:
  double xend = round(x0);
  double yend = y0 + gradient * (xend - x0);
  //double xgap = rfpart(x0 + 0.5);
  double xpxl1 = xend;   //this will be used in the main loop
  double ypxl1 = ipart(yend);

  if(steep) {
    plot(ypxl1, xpxl1, 1);
    //plot(ypxl1,   xpxl1, rfpart(yend) * xgap);
    //plot(ypxl1+1, xpxl1,  fpart(yend) * xgap);
  } else {
    plot(xpxl1, ypxl1, 1);
    //plot(xpxl1, ypxl1  , rfpart(yend) * xgap);
    //plot(xpxl1, ypxl1+1,  fpart(yend) * xgap);
  }

  double intery = yend + gradient; // first y-intersection for the main loop
 
  // Handle second endpoint:
   xend = round(x1);
  yend = y1 + gradient * (xend - x1);
  //xgap = fpart(x1 + 0.5);
  double xpxl2 = xend; //this will be used in the main loop
  double ypxl2 = ipart(yend);

  if(steep) {
    plot(ypxl2, xpxl2, 1);
    //plot(ypxl2  , xpxl2, rfpart(yend) * xgap);
    //plot(ypxl2+1, xpxl2,  fpart(yend) * xgap);
  } else {
    plot(xpxl2, ypxl2, 1);
    //plot(xpxl2, ypxl2,   rfpart(yend) * xgap);
    //plot(xpxl2, ypxl2+1,  fpart(yend) * xgap);
  }

  // main loop
  for(int x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
    if(steep) {
      plot(ipart(intery)  , x, rfpart(intery));
      plot(ipart(intery)+1, x,  fpart(intery));
    } else {
      plot(x, ipart (intery),  rfpart(intery));
      plot(x, ipart (intery)+1, fpart(intery));
    }
    intery += gradient;
  }
}

void GUI::Painter::drawRectangle(int x1, int y1, int x2, int y2)
{
  drawLine(x1, y1, x2 - 1, y1);
  drawLine(x2, y1, x2, y2 - 1);
  drawLine(x1 + 1, y2, x2, y2);
  drawLine(x1, y1 + 1, x1, y2);
}

void GUI::Painter::drawFilledRectangle(int x1, int y1, int x2, int y2)
{
  for(int y = y1; y < y2; y++) {
    drawLine(x1, y, x2, y);
  }
}

void GUI::Painter::clear()
{
  for(int x = 0; x < (int)pixbuf->width; x++) {
    for(int y = 0; y < (int)pixbuf->height; y++) {
      pixbuf->setPixel(x, y, 0, 0, 0, 0);
    }
  }
}

void GUI::Painter::drawText(int x0, int y0, GUI::Font &font, std::string text,
                            bool nocolour)
{
  PixelBufferAlpha *textbuf = font.render(text);
  if(nocolour) {
    for(size_t x = 0; x < textbuf->width; x++) {
      for(size_t y = 0; y < textbuf->height; y++) {
        unsigned char r,g,b,a;
        textbuf->pixel(x, y, &r, &g, &b, &a);
        pixbuf->addPixel(x + x0, y + y0 - textbuf->height, r,g,b,a);
      }
    }
  } else {
    for(size_t x = 0; x < textbuf->width; x++) {
      for(size_t y = 0; y < textbuf->height; y++) {
        unsigned char r,g,b,a;
        textbuf->pixel(x, y, &r, &g, &b, &a);
        pixbuf->addPixel(x + x0, y + y0 - textbuf->height,
                         colour.red * 255,
                         colour.green * 255,
                         colour.blue * 255,
                         colour.alpha * a);
      }
    }
  }

  delete textbuf;
}

#include <stdio.h>
void GUI::Painter::drawPoint(int x, int y)
{
  pixbuf->setPixel(x, y,
                   (unsigned char)(colour.red * 255.0),
                   (unsigned char)(colour.green * 255.0),
                   (unsigned char)(colour.blue * 255.0),
                   (unsigned char)(colour.alpha * 255.0));
}

#if 0
static double distance(double r, double y)
{
  double real_point = sqrt(pow(r, 2) - pow(y, 2));
  return ceil(real_point) - real_point;
}

double new_color(double i) {
 return i * 127;
}

void GUI::Painter::drawCircle(int cx, int cy, double radius)
{
  // wu_circle($image, $r, $color, $offset_x = null, $offset_y = null) {
  //$red = $color["red"];
  //$green = $color["green"];
  //$blue = $color["blue"];
  int offset_x = cx;
  int offset_y = cy;
  int x = radius;
  //  int xx = radius;
  int y = -1;
  //  int yy = -1;
  double t = 0;
  //$color = imagecolorallocate($image, $red, $green, $blue);
  while(x > y) {
    y++;
    double current_distance = distance(radius, y);
    if(current_distance < t) {
      x--;
    }

    double trasparency = new_color(current_distance);
    double alpha = trasparency;
    double alpha2 = 127.0 - trasparency;

    double color = 1;

    plot(x + offset_x, y + offset_y, color);
    plot(x + offset_x - 1, y + offset_y, alpha2 );
    plot(x + offset_x + 1, y + offset_y, alpha );

    plot(y + offset_x, x + offset_y, color);
    plot(y + offset_x, x + offset_y - 1, alpha2);
    plot(y + offset_x, x + offset_y + 1, alpha);

    plot(offset_x - x , y + offset_y, color);
    plot(offset_x - x + 1, y + offset_y, alpha2);
    plot(offset_x - x - 1, y + offset_y, alpha);

    plot(offset_x - y, x + offset_y, color);
    plot(offset_x - y, x + offset_y - 1, alpha2);
    plot(offset_x - y, x + offset_y + 1, alpha);

    plot(x + offset_x, offset_y - y, color);
    plot(x + offset_x - 1, offset_y - y, alpha2);
    plot(x + offset_x + 1, offset_y - y, alpha);

    plot(y + offset_x, offset_y - x, color);
    plot(y + offset_x, offset_y - x - 1, alpha);
    plot(y + offset_x, offset_y - x + 1, alpha2);

    plot(offset_x - y, offset_y - x, color);
    plot(offset_x - y, offset_y - x - 1, alpha);
    plot(offset_x - y, offset_y - x + 1, alpha2);

    plot(offset_x - x, offset_y - y, color);
    plot(offset_x - x - 1, offset_y - y, alpha);
    plot(offset_x - x + 1, offset_y - y, alpha2);

    t = current_distance;
  }
}
#else
static void plot4points(GUI::Painter *p, int cx, int cy, int x, int y)
{
  p->drawPoint(cx + x, cy + y);
  if(x != 0) p->drawPoint(cx - x, cy + y);
  if(y != 0) p->drawPoint(cx + x, cy - y);
  if(x != 0 && y != 0) p->drawPoint(cx - x, cy - y);
}

void GUI::Painter::drawCircle(int cx, int cy, double radius)
{
  int error = -radius;
  int x = radius;
  int y = 0;
 
  while(x >= y) {
    plot4points(this, cx, cy, x, y);
    if(x != y) plot4points(this, cx, cy, y, x);
 
    error += y;
    ++y;
    error += y;
 
    if(error >= 0) {
      --x;
      error -= x;
      error -= x;
    }
  }
}
#endif

static void plot4lines(GUI::Painter *p, int cx, int cy, int x, int y)
{
  p->drawLine(cx + x, cy + y, cx - x, cy + y);
  if(x != 0) p->drawLine(cx - x, cy + y, cx + x, cy + y);
  if(y != 0) p->drawLine(cx + x, cy - y, cx - x, cy - y);
  if(x != 0 && y != 0) p->drawLine(cx - x, cy - y, cx + x, cy - y);
}

void GUI::Painter::drawFilledCircle(int cx, int cy, int radius)
{
  int error = -radius;
  int x = radius;
  int y = 0;
 
  while(x >= y) {
    plot4lines(this, cx, cy, x, y);
    if(x != y) plot4lines(this, cx, cy, y, x);
 
    error += y;
    ++y;
    error += y;
 
    if(error >= 0) {
      --x;
      error -= x;
      error -= x;
    }
  }
}

void GUI::Painter::drawImage(int x0, int y0, GUI::Image *image)
{
  size_t fw = image->width();
  size_t fh = image->height();

  for(size_t x = 0; x < fw; x++) {
    for(size_t y = 0; y < fh; y++) {
      GUI::Colour c = image->getPixel(x, y);
      pixbuf->addPixel(x0 + x, y0 + y, c);
    }
  }
}

void GUI::Painter::drawImageStretched(int x0, int y0, GUI::Image *image,
                                      int w, int h)
{
  if(w < 1 || h < 1) return;

  float fw = image->width();
  float fh = image->height();

  for(int x = 0; x < w; x++) {
    for(int y = 0; y < h; y++) {
      int lx = ((float)x/(float)w)*fw;
      int ly = ((float)y/(float)h)*fh;
      GUI::Colour c = image->getPixel(lx, ly);
      pixbuf->addPixel(x0 + x, y0 + y, c);
    }
  }
}

void GUI::Painter::drawBox(int x, int y, Box *box, int width, int height)
{
  int dx = x;
  int dy = y;

  // Top:

  drawImage(dx, dy, box->topLeft);

  dx += box->topLeft->width();
  if(dx < 0 || dy < 0) return;

  drawImageStretched(dx, dy, box->top,
                     width - box->topRight->width() - box->topLeft->width(),
                     box->top->height());

  dx = x + width - box->topRight->width();
  if(dx < 0 || dy < 0) return;

  drawImage(dx, dy, box->topRight);

  // Center
  dy = y + box->topLeft->height();
  dx = x + box->left->width();
  if(dx < 0 || dy < 0) return;

  drawImageStretched(dx, dy, box->center,
                     width - box->left->width() - box->right->width(),
                     height - box->topLeft->height() - box->bottomLeft->height());

  // Mid:
  dx = x;
  dy = y + box->topLeft->height();
  if(dx < 0 || dy < 0) return;

  drawImageStretched(dx, dy, box->left, box->left->width(),
                     height - box->topLeft->height() - box->bottomLeft->height());

  dx = x + width - box->right->width();
  dy = y + box->topRight->height();
  if(dx < 0 || dy < 0) return;

  drawImageStretched(dx, dy, box->right,
                     box->right->width(),
                     height - box->topRight->height() - box->bottomRight->height());

  // Bottom:
  dx = x;
  dy = y + height - box->bottomLeft->height();
  if(dx < 0 || dy < 0) return;

  drawImage(dx, dy, box->bottomLeft);

  dx += box->bottomLeft->width();
  if(dx < 0 || dy < 0) return;

  drawImageStretched(dx, dy, box->bottom,
                     width - box->bottomRight->width() - box->bottomLeft->width(),
                     box->bottom->height());

  dx = x + width - box->bottomRight->width();
  if(dx < 0 || dy < 0) return;

  drawImage(dx, dy, box->bottomRight);
}

void GUI::Painter::drawBar(int x, int y, Bar *bar, int width, int height)
{
  if(width < ((int)bar->left->width() + (int)bar->right->width() + 1)) {
    width = bar->left->width() + bar->right->width() + 1;
  }
  drawImageStretched(x, y,
                     bar->left,
                     bar->left->width(), height);

  drawImageStretched(x + bar->left->width(), y,
                     bar->center,
                     width - bar->left->width() - bar->right->width(), height);

  drawImageStretched(x + width - bar->left->width(), y,
                     bar->right,
                     bar->right->width(), height);
}

void GUI::Painter::flush()
{
#ifdef X11
  // Send the "DrawLine" request to the server
  //XFlush(gctx->display);
#endif/*X11*/
}

#ifdef TEST_PAINTER
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

#endif/*TEST_PAINTER*/
