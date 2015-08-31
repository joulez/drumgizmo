/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            listboxbasic.h
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
#ifndef __DRUMGIZMO_LISTBOXBASIC_H__
#define __DRUMGIZMO_LISTBOXBASIC_H__

#include <string.h>
#include <vector>

#include "widget.h"
#include "font.h"
#include "painter.h"

#include "scrollbar.h"

namespace GUI {

class ListBoxBasic : public Widget {
public:
  class Item {
  public:
    std::string name;
    std::string value;
  };

  ListBoxBasic(Widget *parent);
  ~ListBoxBasic();

  bool isFocusable() { return true; }

  void addItem(std::string name, std::string value);
  void addItems(std::vector<Item> &items);

  void clear();
  bool selectItem(int index);
  std::string selectedName();
  std::string selectedValue();

  void clearSelectedValue();

  void registerSelectHandler(void (*handler)(void *), void *ptr);
  void registerClickHandler(void (*handler)(void *), void *ptr);
  void registerValueChangeHandler(void (*handler)(void *), void *ptr);

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);
  virtual void scrollEvent(ScrollEvent *e);
  virtual void keyEvent(KeyEvent *e);
  virtual void resize(int w, int h);

private:
  ScrollBar scroll;

  Image bg_img;

  void setSelection(int index);

  std::vector<Item> items;

  int selected;
  int marked;
  GUI::Font font;
  int padding;
  int btn_size;

  void (*sel_handler)(void *);
  void *sel_ptr;

  void (*clk_handler)(void *);
  void *clk_ptr;

  void (*valch_handler)(void *);
  void *valch_ptr;
};

};

#endif/*__DRUMGIZMO_LISTBOXBASIC_H__*/
