/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            listboxthin.cc
 *
 *  Sun Apr  7 19:39:36 CEST 2013
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
#include "listboxthin.h"

#include "painter.h"
#include "font.h"

#include <stdio.h>

GUI::ListBoxThin::ListBoxThin(GUI::Widget *parent)
  : GUI::Widget(parent)
{
  box.topLeft     = new Image(":thinlistbox_tl.png");
  box.top         = new Image(":thinlistbox_t.png");
  box.topRight    = new Image(":thinlistbox_tr.png");
  box.left        = new Image(":thinlistbox_l.png");
  box.right       = new Image(":thinlistbox_r.png");
  box.bottomLeft  = new Image(":thinlistbox_bl.png");
  box.bottom      = new Image(":thinlistbox_b.png");
  box.bottomRight = new Image(":thinlistbox_br.png");
  box.center      = new Image(":thinlistbox_c.png");

  basic = new GUI::ListBoxBasic(this);
  basic->move(box.left->width(), box.top->height());
}

GUI::ListBoxThin::~ListBoxThin()
{
}

void GUI::ListBoxThin::addItem(std::string name, std::string value)
{
  basic->addItem(name, value);
}

void GUI::ListBoxThin::addItems(std::vector<ListBoxBasic::Item> &items)
{
  basic->addItems(items);
}

void GUI::ListBoxThin::clear()
{
  basic->clear();
}

bool GUI::ListBoxThin::selectItem(int index)
{
  return basic->selectItem(index);
}

std::string GUI::ListBoxThin::selectedName()
{
  return basic->selectedName();
}

std::string GUI::ListBoxThin::selectedValue()
{
  return basic->selectedValue();
}

void GUI::ListBoxThin::registerClickHandler(void (*handler)(void *), void *ptr)
{
  basic->registerClickHandler(handler, ptr);
}

void GUI::ListBoxThin::registerSelectHandler(void (*handler)(void *), void *ptr)
{ 
  basic->registerSelectHandler(handler, ptr);
}

void GUI::ListBoxThin::registerValueChangeHandler(void (*handler)(void *),
                                              void *ptr)
{
  basic->registerValueChangeHandler(handler, ptr);
}

void GUI::ListBoxThin::repaintEvent(GUI::RepaintEvent *e)
{
  GUI::Painter p(this);

  p.clear();

  int w = width();
  int h = height();
  if(w == 0 || h == 0) return;
  p.drawBox(0, 0, &box, w, h);
}

void GUI::ListBoxThin::resize(int width, int height)
{
  GUI::Widget::resize(width, height);
  basic->resize(width - (box.left->width() + box.right->width()),
                height - (box.top->height() + box.bottom->height()));
}
