
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            listboxbasic.cc
 *
 *  Thu Apr  4 20:28:10 CEST 2013
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
#include "listboxbasic.h"

#include "painter.h"
#include "font.h"

#include <stdio.h>
#include <hugin.hpp>

void scrolled(void *ptr)
{
  GUI::ListBoxBasic *l = (GUI::ListBoxBasic *)ptr;
  l->repaintEvent(NULL);
}

GUI::ListBoxBasic::ListBoxBasic(GUI::Widget *parent)
  : GUI::Widget(parent), scroll(this), bg_img(":widget_c.png")
{
  scroll.move(0,0);
  scroll.resize(18, 100);

  scroll.registerValueChangeHandler(scrolled, this);

  padding = 4;
  btn_size = 18;

  selected = -1;
  marked = -1;

  clk_handler = NULL;
  clk_ptr = NULL;

  sel_handler = NULL;
  sel_ptr = NULL;

  valch_handler = NULL;
  valch_ptr = NULL;
}

GUI::ListBoxBasic::~ListBoxBasic()
{
}

void GUI::ListBoxBasic::setSelection(int index)
{
  selected = index;
  if(valch_handler) valch_handler(valch_ptr);
}

void GUI::ListBoxBasic::addItem(std::string name, std::string value)
{
  std::vector<GUI::ListBoxBasic::Item> items;
  GUI::ListBoxBasic::Item item;
  item.name = name;
  item.value = value;
  items.push_back(item);
  addItems(items);
}

void GUI::ListBoxBasic::addItems(std::vector<GUI::ListBoxBasic::Item> &is)
{
  //  DEBUG(list, "addItems %lu\n", is.size());
  std::vector<GUI::ListBoxBasic::Item>::iterator i = is.begin();
  while(i != is.end()) {
    items.push_back(*i);
    i++;
  }

/*
  // sort
  for(int x = 0; x < (int)items.size(); x++) {
    for(int y = 0; y < (int)items.size(); y++) {
      if(items[x].name < items[y].name) {
        if(x == selected) setSelection(y);
        else if(selected == y) setSelection(x);
        
        GUI::ListBoxBasic::Item tmp = items[x];
        items[x] = items[y];
        items[y] = tmp;
      }
    }
  }
*/

  if(selected == -1) setSelection((int)items.size() - 1);
  setSelection(0);

  int numitems = height() / (font.textHeight() + padding);
  scroll.setRange(numitems);
  scroll.setMaximum(items.size());
}

void GUI::ListBoxBasic::clear()
{
  items.clear();
  setSelection(-1);
  scroll.setValue(0);
  repaintEvent(NULL);
}

bool GUI::ListBoxBasic::selectItem(int index)
{
  if(index < 0 || index > (int)items.size() - 1) return false;
  setSelection(index);
  repaintEvent(NULL);
  return true;
}

std::string GUI::ListBoxBasic::selectedName()
{
  if(selected < 0 || selected > (int)items.size() - 1) return "";
  return items[selected].name;
}

std::string GUI::ListBoxBasic::selectedValue()
{
  if(selected < 0 || selected > (int)items.size() - 1) return "";
  return items[selected].value;
}

void GUI::ListBoxBasic::clearSelectedValue()
{
  setSelection(-1);
}

void GUI::ListBoxBasic::registerClickHandler(void (*handler)(void *), void *ptr)
{
  this->clk_handler = handler;
  this->clk_ptr = ptr;
}

void GUI::ListBoxBasic::registerSelectHandler(void (*handler)(void *), void *ptr)
{
  this->sel_handler = handler;
  this->sel_ptr = ptr;
}

void GUI::ListBoxBasic::registerValueChangeHandler(void (*handler)(void *),
                                              void *ptr)
{
  this->valch_handler = handler;
  this->valch_ptr = ptr;
}

void GUI::ListBoxBasic::repaintEvent(GUI::RepaintEvent *e)
{
  DEBUG(list, "repaint\n");
  GUI::Painter p(this);

  p.clear();

  int w = width();
  int h = height();
  if(w == 0 || h == 0) return;

  p.drawImageStretched(0, 0, &bg_img, w, h);

  p.setColour(GUI::Colour(183.0/255.0, 219.0/255.0 , 255.0/255.0, 1));

  int yoffset = padding / 2;
  int skip = scroll.value();
  int numitems = height() / (font.textHeight() + padding) + 1;
  for(int idx = skip; idx < (int)items.size() && idx < skip + numitems; idx++) {
    GUI::ListBoxBasic::Item *i = &items[idx];
    if(idx == selected) {
      p.setColour(GUI::Colour(183.0/255.0, 219.0/255.0 , 255.0/255.0, 0.5));
      p.drawFilledRectangle(0,
                            yoffset - (padding / 2),
                            width() - 1,
                            yoffset + (font.textHeight() + 1));
    }

    if(idx == marked) {
      p.drawRectangle(0,
                      yoffset - (padding / 2),
                      width() - 1,
                      yoffset + (font.textHeight() + 1));
    }

    p.setColour(GUI::Colour(183.0/255.0, 219.0/255.0 , 255.0/255.0, 1));

    p.drawText(2, yoffset + font.textHeight(), font, i->name);
    yoffset += font.textHeight() + padding;
  }
}

void GUI::ListBoxBasic::scrollEvent(ScrollEvent *e)
{
  scroll.scrollEvent(e);
}

void GUI::ListBoxBasic::keyEvent(GUI::KeyEvent *e)
{
  if(e->direction != -1) return;

  switch(e->keycode) {
  case GUI::KeyEvent::KEY_UP:
    {
      marked--;
      if(marked < 0) marked = 0;

      if(marked < scroll.value()) {
        scroll.setValue(marked);
      }
    }
    break;
  case GUI::KeyEvent::KEY_DOWN:
    {
      // Number of items that can be displayed at a time.
      int numitems = height() / (font.textHeight() + padding);

      marked++;
      if(marked > ((int)items.size() - 1)) marked = (int)items.size() - 1;

      if(marked > (scroll.value() + numitems - 1)) {
        scroll.setValue(marked - numitems + 1);
      }
    }
    break;
  case GUI::KeyEvent::KEY_HOME:
    marked = 0;
    if(marked < scroll.value()) {
      scroll.setValue(marked);
    }
    break;
  case GUI::KeyEvent::KEY_END:
    {
      // Number of items that can be displayed at a time.
      int numitems = height() / (font.textHeight() + padding);

      marked = (int)items.size() - 1;
      if(marked > (scroll.value() + numitems - 1)) {
        scroll.setValue(marked - numitems + 1);
      }
    }
    break;
  case GUI::KeyEvent::KEY_CHARACTER:
    if(e->text == " ") {
      setSelection(marked);
      // if(sel_handler) sel_handler(sel_ptr);
    }
    break;
  case GUI::KeyEvent::KEY_ENTER:
    setSelection(marked);
    if(sel_handler) sel_handler(sel_ptr);
    break;
  default:
    break;
  }
  
  repaintEvent(NULL);
}

void GUI::ListBoxBasic::buttonEvent(ButtonEvent *e)
{
  if(e->x > ((int)width() - btn_size) && e->y < ((int)width() - 1)) {
    if(e->y > 0 && e->y < btn_size) {
      if(e->direction == -1) return;
      scroll.setValue(scroll.value() - 1);
      return;
    }
    
    if(e->y > ((int)height() - btn_size) && e->y < ((int)height() - 1)) {
      if(e->direction == -1) return;
      scroll.setValue(scroll.value() + 1);
      return;
    }
  }

  if(e->direction == -1) {
    int skip = scroll.value();
    size_t yoffset = padding / 2;
    for(int idx = skip; idx < (int)items.size(); idx++) {
      yoffset += font.textHeight() + padding;
      if(e->y < (int)yoffset - (padding / 2)) {
        setSelection(idx);
        marked = selected;
        if(clk_handler) clk_handler(clk_ptr);
        break;
      }
    }

    repaintEvent(NULL);
  }

  if(e->direction != -1) {
    int skip = scroll.value();
    size_t yoffset = padding / 2;
    for(int idx = skip; idx < (int)items.size(); idx++) {
      yoffset += font.textHeight() + padding;
      if(e->y < (int)yoffset - (padding / 2)) {
        marked = idx;
        break;
      }
    }

    repaintEvent(NULL);
  }

  if(e->doubleclick && sel_handler) sel_handler(sel_ptr);
}

void GUI::ListBoxBasic::resize(int w, int h)
{
  GUI::Widget::resize(w,h);
  scroll.move(w - scroll.width(), 0);
  scroll.resize(scroll.width(), h);
}
