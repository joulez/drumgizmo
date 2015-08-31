/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            combobox.h
 *
 *  Sun Mar 10 19:04:50 CET 2013
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
#ifndef __DRUMGIZMO_COMBOBOX_H__
#define __DRUMGIZMO_COMBOBOX_H__

#include <string.h>
#include <vector>

#include "widget.h"
#include "font.h"
#include "listboxthin.h"
#include "painter.h"

namespace GUI {

class ComboBox : public Widget {
public:
  ComboBox(Widget *parent);
  ~ComboBox();

  bool isFocusable() { return true; }

  void addItem(std::string name, std::string value);

  void clear();
  bool selectItem(int index);
  std::string selectedName();
  std::string selectedValue();

  void registerValueChangedHandler(void (*handler)(void *), void *ptr);

  virtual void repaintEvent(RepaintEvent *e);
  virtual void buttonEvent(ButtonEvent *e);
  virtual void scrollEvent(ScrollEvent *e);
  virtual void keyEvent(KeyEvent *e);

private:
  Painter::Box box;

  GUI::Font font;
  GUI::ListBoxThin *listbox;

  void (*handler)(void *);
  void *ptr;
};

};

#endif/*__DRUMGIZMO_COMBOBOX_H__*/
