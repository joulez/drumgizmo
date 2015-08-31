/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            widget.cc
 *
 *  Sun Oct  9 13:01:44 CEST 2011
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
#include "widget.h"

#include "painter.h"
#include "window.h"

#include <stdio.h>

GUI::Widget::Widget(Widget *parent)
  : pixbuf(1, 1)
{
  _width = _height = 10;

  this->parent = parent;
  if(parent) {
    parent->addChild(this);
    _window = parent->window();
  }
  _width = _height = 0;
  _visible = true;
}

GUI::Widget::~Widget()
{
  if(parent) parent->removeChild(this);
}

void GUI::Widget::show()
{
  setVisible(true);
}

void GUI::Widget::hide()
{
  setVisible(false);
}

void GUI::Widget::setVisible(bool v)
{
  _visible = v;
  repaintEvent(NULL);
}

bool GUI::Widget::visible()
{
  return _visible;
}

void GUI::Widget::addChild(GUI::Widget *widget)
{
  children.push_back(widget);
}

void GUI::Widget::removeChild(GUI::Widget *widget)
{
  std::vector<Widget *>::iterator i = children.begin();
  while(i != children.end()) {
    if(*i == widget) {
      children.erase(i);
      return;
    }
    i++;
  }
}

void GUI::Widget::resize(int width, int height)
{
  if(width < 1 || height < 1) return;
  _width = width;
  _height = height;
  pixbuf.realloc(width, height);
}

void GUI::Widget::move(size_t x, size_t y)
{
  _x = x;
  _y = y;
}

size_t GUI::Widget::x() { return _x; }
size_t GUI::Widget::y() { return _y; }
size_t GUI::Widget::width() { return _width; }
size_t GUI::Widget::height() { return _height; }

size_t GUI::Widget::windowX()
{
  size_t window_x = x();
  if(parent) window_x += parent->windowX();
  return window_x;
}

size_t GUI::Widget::windowY()
{
  size_t window_y = y();
  if(parent) window_y += parent->windowY();
  return window_y;
}

GUI::Widget *GUI::Widget::find(size_t x, size_t y)
{
  std::vector<Widget*>::reverse_iterator i = children.rbegin();
  while(i != children.rend()) {
    Widget *w = *i;
    if(w->visible()) {
      if(w->x() <= x && (w->x() + w->width()) >= x &&
         w->y() <= y && w->y() + w->height() >= y)
        return w->find(x - w->x(), y - w->y());
    }
    i++;
  }

  if(x > width() || x < 0 || y > height() || y < 0) return NULL;
  return this;
}

GUI::Window *GUI::Widget::window()
{
  return _window;
}

void GUI::Widget::repaint_r(GUI::RepaintEvent *e)
{
  Painter p(this); // make sure pixbuf refcount is incremented.

  repaintEvent(e);

  std::vector<Widget*>::iterator i = children.begin();
  while(i != children.end()) {
    Widget *w = *i;
    w->repaint_r(e);
    i++;
  }
}

std::vector<GUI::PixelBufferAlpha *> GUI::Widget::getPixelBuffers()
{
  std::vector<PixelBufferAlpha *> pbs;

  pixbuf.x = windowX();
  pixbuf.y = windowY();

  pbs.push_back(&pixbuf);

  std::vector<Widget*>::iterator i = children.begin();
  while(i != children.end()) {
    Widget *w = *i;
    if(w->visible()) {
      std::vector<PixelBufferAlpha *> pbs0 = w->getPixelBuffers();
      pbs.insert(pbs.end(), pbs0.begin(), pbs0.end());
    }
    i++;
  }

  return pbs;
}

bool GUI::Widget::hasKeyboardFocus()
{
  return window()->keyboardFocus() == this;
}

#ifdef TEST_WIDGET
//deps: window.cc globalcontext.cc
//cflags:
//libs:
#include "test.h"

#include "window.h"

TEST_BEGIN;

GUI::Window w1(NULL);
w1.move(0,0);
w1.resize(100, 100);

GUI::Widget w2(&w1);
w2.resize(40,40);
w2.move(10,10);

GUI::Widget w3(&w2);
w3.resize(20,20);
w3.move(10,10);

TEST_EQUAL_PTR(w1.find(101,0), NULL, "Miss?");
TEST_EQUAL_PTR(w1.find(0,0), &w1, "Hit w1?");
TEST_EQUAL_PTR(w1.find(100,100), &w1, "Hit w1?");
TEST_EQUAL_PTR(w1.find(0,0), &w1, "Hit w1?");
TEST_EQUAL_PTR(w1.find(11,11), &w2, "Hit w2?");
TEST_EQUAL_PTR(w1.find(22,22), &w3, "Hit w3?");

TEST_END;

#endif/*TEST_WIDGET*/
