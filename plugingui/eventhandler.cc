/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            eventhandler.cc
 *
 *  Sun Oct  9 18:58:29 CEST 2011
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
#include "eventhandler.h"

#include "window.h"
#include "painter.h"

GUI::EventHandler::EventHandler(GUI::NativeWindow *n, GUI::Window *w)
{
  native = n;
  window = w;

  last_click = 0;
  last_was_dbl_click = false;
}

bool GUI::EventHandler::hasEvent()
{
  return native->hasEvent();
}

GUI::Event *GUI::EventHandler::getNextEvent()
{
  return native->getNextEvent();
}

void GUI::EventHandler::registerCloseHandler(void (*handler)(void *), void *ptr)
{
  this->closeHandler = handler;
  this->closeHandlerPtr = ptr;
}

void GUI::EventHandler::processEvents()
{
  while(hasEvent()) {
    Painter p(window); // Make sure we only redraw buffer one time.

    Event *event = getNextEvent();

    if(event == NULL) continue;

    //    Widget *widget = gctx->widgets[event->window_id];
    switch(event->type()) {
    case Event::Repaint:
      //      window->repaint((RepaintEvent*)event);
      window->redraw();
      break;
    case Event::Resize:
      {
        //      window->repaint((RepaintEvent*)event)
        ResizeEvent *re = (ResizeEvent*)event;
        if(re->width != window->width() || re->height != window->height()) {
          window->resized(re->width, re->height);
          //window->repaint_r(NULL);
        }
      }
      break;
    case Event::MouseMove:
      {
        MouseMoveEvent *me = (MouseMoveEvent*)event;

        Widget *w = window->find(me->x, me->y);
        Widget *oldw = window->mouseFocus();
        if(w != oldw) {
          // Send focus leave to oldw
          if(oldw) oldw->mouseLeaveEvent();
          // Send focus enter to w
          if(w) w->mouseEnterEvent();

          window->setMouseFocus(w);
        }

        if(window->buttonDownFocus()) {
          Widget *w = window->buttonDownFocus();
          /*
          if(me->x < w->x()) me->x = w->x();
          if(me->x > w->x() + w->width()) me->x = w->x() + w->width();
          if(me->y < w->y()) me->y = w->y();
          if(me->y > w->y() + w->height()) me->y = w->y() + w->height();
          */
          me->x -= w->windowX();
          me->y -= w->windowY();

          window->buttonDownFocus()->mouseMoveEvent(me);
          break;
        }

        if(w) {
          me->x -= w->windowX();
          me->y -= w->windowY();
          w->mouseMoveEvent(me);
        }
      }
      break;
    case Event::Button:
      {
        if(last_was_dbl_click) {
          last_was_dbl_click = false;
          continue;
        }
        ButtonEvent *be = (ButtonEvent *)event;

        last_was_dbl_click = be->doubleclick;

        Widget *w = window->find(be->x, be->y);

        if(window->buttonDownFocus()) {
          if(be->direction == -1) {
            Widget *w = window->buttonDownFocus();
            /*
            if(be->x < w->x()) be->x = w->x();
            if(be->x > w->x() + w->width()) be->x = w->x() + w->width();
            if(be->y < w->y()) be->y = w->y();
            if(be->y > w->y() + w->height()) be->y = w->y() + w->height();
            */
            be->x -= w->windowX();
            be->y -= w->windowY();

            w->buttonEvent(be);
            break;
          } else {
            window->setButtonDownFocus(NULL);
          }
        }

        if(w) {
          be->x -= w->windowX();
          be->y -= w->windowY();

          w->buttonEvent(be);

          if(be->direction == 1) {
            if(w->catchMouse()) window->setButtonDownFocus(w);
          }

          if(w->isFocusable()) window->setKeyboardFocus(w);
        }
      }
      break;
    case Event::Scroll:
      {
        ScrollEvent *se = (ScrollEvent *)event;

        Widget *w = window->find(se->x, se->y);

        //printf("scroller (%d,%d) %p\n", se->x, se->y, w);

        if(w) {
          se->x -= w->windowX();
          se->y -= w->windowY();

          w->scrollEvent(se);
        }
      }
      break;
    case Event::Key:
      //      window->key((KeyEvent*)event);
      //      lineedit->keyEvent((KeyEvent*)event);
      if(window->keyboardFocus())
        window->keyboardFocus()->keyEvent((KeyEvent*)event);
      break;
    case Event::Close:
      if(closeHandler) closeHandler(closeHandlerPtr);
      //delete window;
      //window = NULL;
      break;
    }
    delete event;
  }
}
