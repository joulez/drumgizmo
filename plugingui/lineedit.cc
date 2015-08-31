/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lineedit.cc
 *
 *  Sun Oct  9 13:01:52 CEST 2011
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
#include "lineedit.h"

#include <stdio.h>

#include "window.h"

#include <assert.h>

#include <hugin.hpp>

#define BORDER 10

GUI::LineEdit::LineEdit(Widget *parent)
  : GUI::Widget(parent)
{
  pos = 0;
  offsetpos = 0;
  setReadOnly(false);

  box.topLeft     = new Image(":widget_tl.png");
  box.top         = new Image(":widget_t.png");
  box.topRight    = new Image(":widget_tr.png");
  box.left        = new Image(":widget_l.png");
  box.right       = new Image(":widget_r.png");
  box.bottomLeft  = new Image(":widget_bl.png");
  box.bottom      = new Image(":widget_b.png");
  box.bottomRight = new Image(":widget_br.png");
  box.center      = new Image(":widget_c.png");

  handler = NULL;
}

void GUI::LineEdit::registerEnterPressedHandler(void (*handler)(void *), void *ptr)
{
  this->handler = handler;
  this->ptr = ptr;
}

void GUI::LineEdit::setReadOnly(bool ro)
{
  readonly = ro;
}

bool GUI::LineEdit::readOnly()
{
  return readonly;
}

void GUI::LineEdit::setText(std::string text)
{
  _text = text;
  pos = text.size();
  
  repaintEvent(NULL);
  textChanged();
}

std::string GUI::LineEdit::text()
{
  return _text;
}

void GUI::LineEdit::buttonEvent(ButtonEvent *e)
{
  if(readOnly()) return;


  if(e->direction == 1) {
    for(int i = 0; i < (int)_visibletext.length(); i++) {
      if(e->x < (int)(font.textWidth(_visibletext.substr(0, i)) + BORDER)) {
        pos = i + offsetpos;
        break;
      }
    }
    repaintEvent(NULL);
  }
}

void GUI::LineEdit::keyEvent(GUI::KeyEvent *e)
{
  if(readOnly()) return;

  bool change = false;
  
  if(e->direction == -1) {

    if(e->keycode == GUI::KeyEvent::KEY_LEFT) {
      if(pos) pos--;
      if(offsetpos >= pos) walkstate = WALK_LEFT;
    
    } else if(e->keycode == GUI::KeyEvent::KEY_HOME) {
      pos = 0;

    } else if(e->keycode == GUI::KeyEvent::KEY_END) {
      pos = _text.length();

    } else if(e->keycode == GUI::KeyEvent::KEY_RIGHT) {
      if(pos < _text.length()) pos++;
      if(offsetpos + _visibletext.length() <= pos &&
         pos < _text.length()) walkstate = WALK_RIGHT;
    
    } else if(e->keycode == GUI::KeyEvent::KEY_DELETE) {
      if(pos < _text.length()) {
        std::string t = _text.substr(0, pos);
        t += _text.substr(pos + 1, std::string::npos);
        _text = t;
        change = true;
      }

    } else if(e->keycode == GUI::KeyEvent::KEY_BACKSPACE) {
      if(pos > 0) {
        std::string t = _text.substr(0, pos - 1);
        t += _text.substr(pos, std::string::npos);
        _text = t;
        pos--;
        change = true;
      }

    } else if(e->keycode == GUI::KeyEvent::KEY_CHARACTER) {
      std::string pre = _text.substr(0, pos);
      std::string post = _text.substr(pos, std::string::npos);
      _text = pre + e->text + post;
      change = true;
      pos++;

    } else if(e->keycode == GUI::KeyEvent::KEY_ENTER) {
      if(handler) handler(ptr);
    }
    repaintEvent(NULL);
  }

  if(change) textChanged();
}

void GUI::LineEdit::repaintEvent(GUI::RepaintEvent *e)
{
  Painter p(this);

  p.clear();

  int w = width();
  int h = height();
  if(w == 0 || h == 0) return;
  p.drawBox(0, 0, &box, w, h);

  p.setColour(GUI::Colour(183.0/255.0, 219.0/255.0 , 255.0/255.0, 1));

  if(walkstate == WALK_LEFT) {
    _visibletext = _text.substr(pos, std::string::npos);
    offsetpos = pos;
  }
  else if(walkstate == WALK_RIGHT) {
    int d = (offsetpos < _text.length()) ? 1 : 0;
    _visibletext = _text.substr(offsetpos + d);
    offsetpos = offsetpos + d;
  }
  else {
    _visibletext = _text;
    offsetpos = 0;
  }
  while(true) {
    int textwidth = font.textWidth(_visibletext);
    if(textwidth > w - BORDER - 4 + 3) {
      if(walkstate == WALK_LEFT) {
        _visibletext = _visibletext.substr(0, _visibletext.length()-1);
      }
      else if(walkstate == WALK_RIGHT) {
        _visibletext = _visibletext.substr(0, _visibletext.length()-1);
      }
      else {
        if(offsetpos < pos) {
          _visibletext = _visibletext.substr(1);
          offsetpos++;
        }
        else {
          _visibletext = _visibletext.substr(0, _visibletext.length() - 1);
        }
      }
    }
    else {
      break;
    }
  }

  walkstate = NOOP;

  p.drawText(BORDER - 4 + 3, height()/2+5 + 1 + 1 + 1, font, _visibletext);

  if(readOnly()) return;

  if(hasKeyboardFocus()) {
    size_t px = font.textWidth(_visibletext.substr(0, pos - offsetpos));
    p.drawLine(px + BORDER - 1 - 4 + 3, 6,
               px + BORDER - 1 - 4 + 3, height() - 7);
  }
}

#ifdef TEST_LINEEDIT
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

#endif/*TEST_LINEEDIT*/
