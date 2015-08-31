/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            nativewindow_x11.cc
 *
 *  Fri Dec 28 18:45:57 CET 2012
 *  Copyright 2012 Bent Bisballe Nyeng
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
#include "nativewindow_x11.h"

#ifdef X11
#include <X11/Xutil.h>
#include <stdlib.h>

#include "window.h"

GUI::NativeWindowX11::NativeWindowX11(GUI::Window *window)
  : GUI::NativeWindow()
{
  display = XOpenDisplay(NULL);

  this->window = window;
  buffer = NULL;

  // Get some colors
  int blackColor = BlackPixel(display, DefaultScreen(display));
  
  ::Window w = DefaultRootWindow(display);

  // Create the window
  xwindow = XCreateSimpleWindow(display,
                                w,
                                window->x(), window->y(),
                                window->width(), window->height(),
                                0,
                                blackColor, blackColor);

  XSelectInput(display, xwindow,
               StructureNotifyMask |
               PointerMotionMask |
               ButtonPressMask |
               ButtonReleaseMask |
               KeyPressMask |
               KeyReleaseMask|
               ExposureMask |
               StructureNotifyMask |
               SubstructureNotifyMask);

  // register interest in the delete window message
  wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(display, xwindow, &wmDeleteMessage, 1);

  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(display, xwindow);

  // Create a "Graphics Context"
  gc = XCreateGC(display, xwindow, 0, NULL);
}

GUI::NativeWindowX11::~NativeWindowX11()
{
  XDestroyWindow(display, xwindow);
  //widgets.erase(window);
  XCloseDisplay(display);
}

void GUI::NativeWindowX11::setFixedSize(int width, int height)
{
  resize(width, height);

  XSizeHints *size_hints;
  size_hints = XAllocSizeHints();

  if(size_hints == NULL) {
    //fprintf(stderr,"XMallocSizeHints() failed\n");
    //exit(1);
    return;
  }

  size_hints->flags = USPosition | PMinSize | PMaxSize;
  size_hints->min_width = size_hints->max_width = width;
  size_hints->min_height = size_hints->max_height = height;
  /*
    size_hints->min_aspect.x = window->width()/window->height();
    size_hints->max_aspect.x = window->width()/window->height();
    size_hints->min_aspect.y = window->width()/window->height();
    size_hints->max_aspect.y = size_hints->min_aspect.y;
  */
  XSetWMNormalHints(display, xwindow, size_hints);
}

void GUI::NativeWindowX11::resize(int width, int height)
{
  XResizeWindow(display, xwindow, width, height);
}

void GUI::NativeWindowX11::move(int x, int y)
{
  XMoveWindow(display, xwindow, x, y);
}

void GUI::NativeWindowX11::show()
{
  XMapWindow(display, xwindow);
}

void GUI::NativeWindowX11::hide()
{
  XUnmapWindow(display, xwindow);
}

static int get_byte_order (void)
{
	union {
		char c[sizeof(short)];
		short s;
	} order;

	order.s = 1;
	if ((1 == order.c[0])) {
		return LSBFirst;
	} else {
		return MSBFirst;
	}
}

static XImage *create_image_from_buffer(Display *dis, int screen,
                                        unsigned char *buf,
                                        int width, int height)
{
	int depth;
	XImage *img = NULL;
	Visual *vis;
	double rRatio;
	double gRatio;
	double bRatio;
	int outIndex = 0;	
	int i;
	int numBufBytes = (3 * (width * height));
		
	depth = DefaultDepth(dis, screen);
	vis = DefaultVisual(dis, screen);

	rRatio = vis->red_mask / 255.0;
	gRatio = vis->green_mask / 255.0;
	bRatio = vis->blue_mask / 255.0;
		
	if (depth >= 24) {
		size_t numNewBufBytes = (4 * (width * height));
		u_int32_t *newBuf = (u_int32_t *)malloc (numNewBufBytes);
	
		for (i = 0; i < numBufBytes; ++i) {
			unsigned int r, g, b;
			r = (buf[i] * rRatio);
			++i;
			g = (buf[i] * gRatio);
			++i;
			b = (buf[i] * bRatio);
					
			r &= vis->red_mask;
			g &= vis->green_mask;
			b &= vis->blue_mask;
			
			newBuf[outIndex] = r | g | b;
			++outIndex;
		}		
		
		img = XCreateImage (dis, 
			CopyFromParent, depth, 
			ZPixmap, 0, 
			(char *) newBuf,
			width, height,
			32, 0
		);
		
	} else if (depth >= 15) {
		size_t numNewBufBytes = (2 * (width * height));
		u_int16_t *newBuf = (u_int16_t *)malloc (numNewBufBytes);
		
		for (i = 0; i < numBufBytes; ++i) {
			unsigned int r, g, b;

			r = (buf[i] * rRatio);
			++i;
			g = (buf[i] * gRatio);
			++i;
			b = (buf[i] * bRatio);
					
			r &= vis->red_mask;
			g &= vis->green_mask;
			b &= vis->blue_mask;
			
			newBuf[outIndex] = r | g | b;
			++outIndex;
		}		
		
		img = XCreateImage(dis, CopyFromParent, depth, ZPixmap, 0, (char *) newBuf,
                        width, height, 16, 0);
	} else {
		//fprintf (stderr, "This program does not support displays with a depth less than 15.");
		return NULL;				
	}

	XInitImage (img);
	/*Set the client's byte order, so that XPutImage knows what to do with the data.*/
	/*The default in a new X image is the server's format, which may not be what we want.*/
	if ((LSBFirst == get_byte_order ())) {
		img->byte_order = LSBFirst;
	} else {
		img->byte_order = MSBFirst;
	}
	
	/*The bitmap_bit_order doesn't matter with ZPixmap images.*/
	img->bitmap_bit_order = MSBFirst;

	return img;
}		

void GUI::NativeWindowX11::handleBuffer()
{
  if(buffer) XDestroyImage(buffer);
  buffer =
    create_image_from_buffer(display, DefaultScreen(display),
                             window->wpixbuf.buf,
                             window->wpixbuf.width,
                             window->wpixbuf.height);
}

void GUI::NativeWindowX11::redraw()
{
  // http://stackoverflow.com/questions/6384987/load-image-onto-a-window-using-xlib
  if(buffer == NULL) window->updateBuffer();
  XPutImage(display, xwindow, gc, buffer, 0, 0, 0, 0,
            window->width(), window->height());
  XFlush(display);
}

void GUI::NativeWindowX11::setCaption(const std::string &caption)
{
  XStoreName(display, xwindow, caption.c_str());
}

void GUI::NativeWindowX11::grabMouse(bool grab)
{
  (void)grab;
  // Don't need to do anything on this platoform...
}

bool GUI::NativeWindowX11::hasEvent()
{
  return XPending(display);
}

GUI::Event *GUI::NativeWindowX11::getNextEvent()
{
  Event *event = NULL;

  XEvent xe;
  XNextEvent(display, &xe);

  if(xe.type == MotionNotify) {
    while(true) { // Hack to make sure only the last event is played.
      if(!hasEvent()) break;
      XEvent nxe;
      XPeekEvent(display, &nxe);
      if(nxe.type != MotionNotify) break;
      XNextEvent(display, &xe);
    }

    MouseMoveEvent *e = new MouseMoveEvent();
    e->window_id = xe.xmotion.window;
    e->x = xe.xmotion.x;
    e->y = xe.xmotion.y;
    event = e;
  }

  if(xe.type == Expose && xe.xexpose.count == 0) {
    RepaintEvent *e = new RepaintEvent();
    e->window_id = xe.xexpose.window;
    e->x = xe.xexpose.x;
    e->y = xe.xexpose.y;
    e->width = xe.xexpose.width;
    e->height = xe.xexpose.height;
    event = e;
  }

  if(xe.type == ConfigureNotify) {
    ResizeEvent *e = new ResizeEvent();
    e->window_id = xe.xconfigure.window;
    //    e->x = xe.xconfigure.x;
    //    e->y = xe.xconfigure.y;
    e->width = xe.xconfigure.width;
    e->height = xe.xconfigure.height;
    event = e;
  }

  if(xe.type == ButtonPress || xe.type == ButtonRelease) {
    if(xe.xbutton.button == 4 || xe.xbutton.button == 5) {
      int scroll = 1;
      while(true) { // Hack to make sure only the last event is played.
        if(!hasEvent()) break;
        XEvent nxe;
        XPeekEvent(display, &nxe);
        if(nxe.type != ButtonPress && nxe.type != ButtonRelease) break;
        scroll += 1;
        XNextEvent(display, &xe);
      }
      ScrollEvent *e = new ScrollEvent();
      e->window_id = xe.xbutton.window;
      e->x = xe.xbutton.x;
      e->y = xe.xbutton.y;
      e->delta = scroll * (xe.xbutton.button==4?-1:1);
      event = e;
    } else {
      ButtonEvent *e = new ButtonEvent();
      e->window_id = xe.xbutton.window;
      e->x = xe.xbutton.x;
      e->y = xe.xbutton.y;
      e->button = 0;
      e->direction = xe.type == ButtonPress?1:-1;
      e->doubleclick =
        xe.type == ButtonPress && (xe.xbutton.time - last_click) < 200;
      
      if(xe.type == ButtonPress) last_click = xe.xbutton.time;
      event = e;
    }
  }

  if(xe.type == KeyPress || xe.type == KeyRelease) {
    //printf("key: %d\n", xe.xkey.keycode);
    KeyEvent *e = new KeyEvent();
    e->window_id = xe.xkey.window;

    switch(xe.xkey.keycode) {
    case 113: e->keycode = KeyEvent::KEY_LEFT; break;
    case 114: e->keycode = KeyEvent::KEY_RIGHT; break;
    case 111: e->keycode = KeyEvent::KEY_UP; break;
    case 116: e->keycode = KeyEvent::KEY_DOWN; break;
    case 119: e->keycode = KeyEvent::KEY_DELETE; break;
    case 22: e->keycode = KeyEvent::KEY_BACKSPACE; break;
    case 110: e->keycode = KeyEvent::KEY_HOME; break;
    case 115: e->keycode = KeyEvent::KEY_END; break;
    case 117: e->keycode = KeyEvent::KEY_PGDOWN; break;
    case 112: e->keycode = KeyEvent::KEY_PGUP; break;
    case 36: e->keycode = KeyEvent::KEY_ENTER; break;
    default: e->keycode = KeyEvent::KEY_UNKNOWN; break;
    }

    char buf[1024];
    int sz = XLookupString(&xe.xkey, buf, sizeof(buf),  NULL, NULL);
    if(sz && e->keycode == KeyEvent::KEY_UNKNOWN) {
      e->keycode = KeyEvent::KEY_CHARACTER;
    }
    e->text.append(buf, sz);

    e->direction = xe.type == KeyPress?1:-1;
    event = e;
  }

  if(xe.type == ClientMessage &&
     (unsigned int)xe.xclient.data.l[0] == wmDeleteMessage) {
    CloseEvent *e = new CloseEvent();
    event = e;
  }

  return event;
}

#endif/*X11*/

