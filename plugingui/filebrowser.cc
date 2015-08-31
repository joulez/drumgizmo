/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            filebrowser.cc
 *
 *  Mon Feb 25 21:09:44 CET 2013
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
#include "filebrowser.h"

#include "painter.h"
#include "button.h"

#include "directory.h"

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <hugin.hpp>

#ifdef __MINGW32__
#include <direct.h>
#endif

struct GUI::FileBrowser::private_data {
  GUI::LineEdit *lineedit;
  GUI::ListBox *listbox;
  void (*filesel_handler)(void *, std::string);
  void *ptr;
  Directory *dir;
#ifdef WIN32
  bool above_root;
  bool in_root;
#endif
};

static void cancel(void *ptr)
{
  GUI::FileBrowser *fp = (GUI::FileBrowser *)ptr;
  fp->hide();
}

static void changeDir(void *ptr)
{
  struct GUI::FileBrowser::private_data *prv =
    (struct GUI::FileBrowser::private_data *) ptr;
  
  GUI::ListBox *lb = prv->listbox;
  GUI::LineEdit *le = prv->lineedit;
  std::string value = lb->selectedValue(); 
  Directory* dir = prv->dir;

//  if(!Directory::isDir(dir->path() + dir->seperator())) {
//    return;
//  }

  lb->clear();
  
  INFO(filebrowser, "Changing path to '%s'\n",
       (dir->path() + dir->seperator() + value).c_str());
 
#ifdef WIN32
  if(prv->above_root && !value.empty()) {
    dir->setPath(value + dir->seperator());
    value.clear();
    prv->above_root = false;
  }
#endif

  if(value.empty() && !dir->isDir() && Directory::exists(dir->path())) {
    DEBUG(filebrowser, "Selecting file '%s'\n", dir->path().c_str());
    if(prv->filesel_handler) prv->filesel_handler(prv->ptr, dir->path().c_str());
    return;
  }

  if(!value.empty() && dir->fileExists(value)) {
    std::string file = dir->path() + dir->seperator() + value;
    DEBUG(filebrowser, "Selecting file '%s'\n", file.c_str());
    if(prv->filesel_handler) prv->filesel_handler(prv->ptr, file);
    return;
  }

  std::vector<GUI::ListBoxBasic::Item> items;

#ifdef WIN32
  if(Directory::isRoot(dir->path()) && value == "..") {
   DEBUG(filebrowser, "Showing partitions...\n");
   Directory::DriveList entries = dir->drives();
   for(Directory::DriveList::iterator it = entries.begin();
       it != entries.end(); it++) {  
      GUI::ListBoxBasic::Item item;
      std::string name = (*it).name;
      item.name = name;
      item.value = name;
      items.push_back(item);
    } 
    prv->above_root = true;
  } else {
#endif
    
    if(!value.empty() && !dir->cd(value)) {
      DEBUG(filebrowser, "Error changing to '%s'\n", 
            (dir->path() + dir->seperator() + value).c_str());
      return;
    }
    
    Directory::EntryList entries = dir->entryList();

    if(entries.empty()) {
      dir->cdUp();
      entries = dir->entryList();
    }

    DEBUG(filebrowser, "Setting path of lineedit to %s\n",
          dir->path().c_str()); 
    le->setText(dir->path());

    for(Directory::EntryList::iterator it = entries.begin();
        it != entries.end(); it++) { 
      GUI::ListBoxBasic::Item item;
      std::string name = *it;
      item.name = name;
      item.value = name;
      items.push_back(item);
    }
#ifdef WIN32
  }
#endif
  lb->addItems(items);
}

static void handleKeyEvent(void *ptr) {
  struct GUI::FileBrowser::private_data *prv =
    (struct GUI::FileBrowser::private_data *) ptr;

  GUI::ListBox *lb = prv->listbox;
  lb->clearSelectedValue();  
  GUI::LineEdit *le = prv->lineedit;

  prv->dir->setPath(le->text());
  changeDir(ptr);
}

GUI::FileBrowser::FileBrowser(GUI::Widget *parent)
  : GUI::Widget(parent),
    lbl_path(this), lineedit(this), listbox(this), btn_sel(this), btn_esc(this),
    back(":bg.png")
{
  prv = new struct GUI::FileBrowser::private_data();
  prv->filesel_handler = NULL;

  prv->dir = new Directory(Directory::cwd());
#ifdef WIN32  
  prv->above_root = false;
#endif

  lbl_path.setText("Path:");

//  lineedit.setReadOnly(true);
  prv->lineedit = &lineedit;
  prv->lineedit->registerEnterPressedHandler(handleKeyEvent, prv);

  prv->listbox = &listbox;
  listbox.registerSelectHandler(changeDir, prv);

  btn_sel.setText("Select");
  btn_sel.registerClickHandler(changeDir, prv);

  btn_esc.setText("Cancel");
  btn_esc.registerClickHandler(cancel, this);

  changeDir(prv);

  resize(200, 190);
}

GUI::FileBrowser::~FileBrowser()
{
  //  delete prv->listbox;
  delete prv;
}

void GUI::FileBrowser::setPath(std::string path)
{
  INFO(filebrowser, "Setting path to '%s'\n", path.c_str());
  if(path.empty()) path = Directory::cwd();

  prv->dir->setPath(Directory::pathDirectory(path));
  prv->listbox->clear();

  changeDir(prv);
}

void GUI::FileBrowser::resize(int w, int h)
{
  GUI::Widget::resize(w,h);

  int offset = 0;
  int brd = 5; // border
  int btn_h = 30;

  offset += brd;

  lbl_path.move(0, offset);
  lineedit.move(60, offset);

  offset += btn_h;

  lbl_path.resize(60, btn_h);
  lineedit.resize(w - 60 - brd, btn_h);

  offset += brd;

  listbox.move(brd, offset);
  listbox.resize(w - 1 - 2*brd, h - btn_h - 2*brd - offset);

  btn_esc.move(brd, h - btn_h - brd);
  btn_esc.resize((w - 1 - 2*brd) / 2 - brd / 2, btn_h);

  btn_sel.move(brd + w / 2 - brd / 2, h - btn_h - brd);
  btn_sel.resize((w - 1 - 2*brd) / 2, btn_h);
}

void GUI::FileBrowser::registerFileSelectHandler(void (*handler)(void *,
                                                                 std::string),
                                                 void *ptr)
{
  prv->filesel_handler = handler;
  prv->ptr = ptr;
}

void GUI::FileBrowser::repaintEvent(GUI::RepaintEvent *e)
{
  Painter p(this);
  p.drawImageStretched(0,0, &back, width(), height());
}
