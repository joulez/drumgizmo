/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            plugingui.cc
 *
 *  Mon Oct  3 13:40:49 CEST 2011
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
#include "plugingui.h"

#include <hugin.hpp>
#include <stdio.h>

#include "knob.h"
#include "verticalline.h"
#include "../version.h"

#include "pluginconfig.h"
#include "messagehandler.h"

static void checkClick(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  ChangeSettingMessage *msg =
    new ChangeSettingMessage(ChangeSettingMessage::enable_velocity_modifier,
                             gui->check->checked());
  msghandler.sendMessage(MSGRCV_ENGINE, msg);
}

static void knobChange(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  ChangeSettingMessage *msg =
    new ChangeSettingMessage(ChangeSettingMessage::velocity_modifier_weight,
                             gui->knob->value());

  msghandler.sendMessage(MSGRCV_ENGINE, msg);

#ifdef STANDALONE
  int i = gui->knob->value() * 4;
  switch(i) {
  case 0: gui->progress->setState(GUI::ProgressBar::off); break;
  case 1: gui->progress->setState(GUI::ProgressBar::blue); break;
  case 2: gui->progress->setState(GUI::ProgressBar::green); break;
  case 3: gui->progress->setState(GUI::ProgressBar::red); break;
  default: break;
  }
#endif
}

static void knobChange2(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  ChangeSettingMessage *msg =
    new ChangeSettingMessage(ChangeSettingMessage::velocity_modifier_falloff,
                             gui->knob2->value());
  msghandler.sendMessage(MSGRCV_ENGINE, msg);

#ifdef STANDALONE
  gui->progress->setProgress(gui->knob2->value());
#endif
}

//static void quit(void *ptr) {
//  PluginGUI *gui = (PluginGUI*)ptr;
//
//  gui->stopThread();
//}

GUI::FileBrowser *fb;
static void selectKitFile(void *ptr, std::string filename)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  gui->lineedit->setText(filename);

  fb->hide();

  std::string drumkit = gui->lineedit->text();

  gui->config->lastkit = drumkit;
  gui->config->save();

  gui->progress->setProgress(0);
  gui->progress->setState(GUI::ProgressBar::blue);

  LoadDrumKitMessage *msg = new LoadDrumKitMessage();
  msg->drumkitfile = drumkit;

  msghandler.sendMessage(MSGRCV_ENGINE, msg);
}

static void kitBrowseClick(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  std::string path = gui->lineedit->text();
  if(path == "") path = gui->config->lastkit;
  if(path == "") path = gui->lineedit2->text();

  fb->setPath(path);
  fb->registerFileSelectHandler(selectKitFile, gui);
  fb->show();
}

static void selectMapFile(void *ptr, std::string filename)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  gui->lineedit2->setText(filename);
  fb->hide();

  std::string midimap = gui->lineedit2->text();

  gui->config->lastmidimap = midimap;
  gui->config->save();

  LoadMidimapMessage *msg = new LoadMidimapMessage();
  msg->midimapfile = midimap;
  msghandler.sendMessage(MSGRCV_ENGINE, msg);

  /*
  if(gui->changeMidimapHandler)
    gui->changeMidimapHandler(gui->changeMidimapPtr, midimap.c_str());
  gui->progress2->setState(GUI::ProgressBar::green);
  */
}

static void midimapBrowseClick(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;

  std::string path = gui->lineedit2->text();
  if(path == "") path = gui->config->lastmidimap;
  if(path == "") path = gui->lineedit->text();

  fb->setPath(path);
  fb->registerFileSelectHandler(selectMapFile, gui);
  fb->show();
}

/*
void closeClick(void *ptr)
{
  PluginGUI *gui = (PluginGUI*)ptr;
  if(gui->windowClosedHandler) gui->windowClosedHandler(gui->windowClosedPtr);
}
*/

PluginGUI::PluginGUI()
  : MessageReceiver(MSGRCV_UI), sem("plugingui")
{
  initialised = false;

  windowClosedHandler = NULL;
  changeMidimapHandler = NULL;

  window = NULL;
 
  running = true;
  closing = false;

#ifdef USE_THREAD
  run();
#else
  init();
#endif/*USE_THREAD*/

  sem.wait();
}

PluginGUI::~PluginGUI()
{
  stopThread();
}

void PluginGUI::stopThread()
{
  if(running) {
    running = false;
    wait_stop();
  }
}


void PluginGUI::handleMessage(Message *msg)
{
  GUI::Painter p(window);// Make sure we only redraw buffer one time.

  switch(msg->type()) {
  case Message::LoadStatus:
    {
      LoadStatusMessage *ls = (LoadStatusMessage*)msg;
      progress->setProgress((float)ls->numer_of_files_loaded /
                            (float)ls->number_of_files);
      if(ls->numer_of_files_loaded == ls->number_of_files) {
        progress->setState(GUI::ProgressBar::green);
      }
    }
    break;
  case Message::LoadStatusMidimap:
    {
      LoadStatusMessageMidimap *ls = (LoadStatusMessageMidimap*)msg;
      progress2->setProgress(1);
      if(ls->success) {
        progress2->setState(GUI::ProgressBar::green);
      } else {
        progress2->setState(GUI::ProgressBar::red);
      }
    }
    break;
  case Message::EngineSettingsMessage:
    {
      EngineSettingsMessage *settings = (EngineSettingsMessage *)msg;
      lineedit->setText(settings->drumkitfile);
      if(settings->drumkit_loaded) {
        progress->setProgress(1);
        progress->setState(GUI::ProgressBar::green);
      } else {
        progress->setProgress(0);
        progress->setState(GUI::ProgressBar::blue);
      }
      lineedit2->setText(settings->midimapfile);
      if(settings->midimap_loaded) {
        progress2->setProgress(1);
        progress2->setState(GUI::ProgressBar::green);
      } else {
        progress2->setProgress(0);
        progress2->setState(GUI::ProgressBar::blue);
      }
      check->setChecked(settings->enable_velocity_modifier);
      knob->setValue(settings->velocity_modifier_weight);
      knob2->setValue(settings->velocity_modifier_falloff);
      
    }
  default:
    break;
  }
}

void PluginGUI::thread_main()
{
  init();

  { // Request all engine settings
    EngineSettingsMessage *msg = new EngineSettingsMessage();
    msghandler.sendMessage(MSGRCV_ENGINE, msg);
  }

  while(running) {
#ifdef WIN32
    SleepEx(50, FALSE);
#else
    usleep(50000);
#endif/*WIN32*/

    //    DEBUG(gui, "loop");

    window->eventHandler()->processEvents();
    handleMessages();
  }

  deinit();
}

void PluginGUI::deinit()
{
  if(config) {
    config->save();
    delete config;
  }
  if(window) delete window;
}

void closeEventHandler(void *ptr)
{
  volatile bool *closing = (volatile bool*)ptr;
  *closing = true;
}

void PluginGUI::init()
{
  DEBUG(gui, "init");

  config = new Config();
  config->load();

  window = new GUI::Window();
  window->eventHandler()->registerCloseHandler(closeEventHandler,
                                               (void*)&closing);

  window->setFixedSize(370, 330);
  window->setCaption("DrumGizmo v"VERSION);

  GUI::Label *lbl_title = new GUI::Label(window);
  lbl_title->setText("DrumGizmo");
  lbl_title->move(127, 7);
  lbl_title->resize(200, 20);
  
  GUI::VerticalLine *l1 = new GUI::VerticalLine(window);
  l1->move(20, 30);
  l1->resize(window->width() - 40, 2);

#define OFFSET1 17
#define OFFSET2 38
#define OFFSET3 20

#define XOFFSET 20
  // Drumkit file
  {
    int y = 37;
    GUI::Label *lbl = new GUI::Label(window);
    lbl->setText("Drumkit file:");
    lbl->move(XOFFSET - 4, y);
    lbl->resize(100, 20);

    y += OFFSET1;
    lineedit = new GUI::LineEdit(window);
    lineedit->move(XOFFSET, y);
    lineedit->resize(243, 29);
    lineedit->setReadOnly(true);

    GUI::Button *btn_brw = new GUI::Button(window);
    btn_brw->setText("Browse...");
    btn_brw->move(266, y - 6 + 4);
    btn_brw->resize(85, 35 + 6 - 4);
    btn_brw->registerClickHandler(kitBrowseClick, this);

    y += OFFSET2;
    progress = new GUI::ProgressBar(window);
    progress->move(XOFFSET, y);
    progress->resize(window->width() - 2*XOFFSET, 11);

    y += OFFSET3;
    GUI::VerticalLine *l = new GUI::VerticalLine(window);
    l->move(XOFFSET, y);
    l->resize(window->width() - 2*XOFFSET, 2);
  }

  // Midimap file
  {
    int y = 120;
    lbl2 = new GUI::Label(window);
    lbl2->setText("Midimap file:");
    lbl2->move(XOFFSET - 4, y);
    lbl2->resize(100, 20);
    
    y += OFFSET1;
    lineedit2 = new GUI::LineEdit(window);
    lineedit2->move(XOFFSET, y);
    lineedit2->resize(243, 29);
    lineedit2->setReadOnly(true);

    GUI::Button *btn_brw = new GUI::Button(window);
    btn_brw->setText("Browse...");
    btn_brw->move(266, y - 6 + 4);
    btn_brw->resize(85, 35 + 6 - 4);
    btn_brw->registerClickHandler(midimapBrowseClick, this);

    y += OFFSET2;
    progress2 = new GUI::ProgressBar(window);
    progress2->move(XOFFSET, y);
    progress2->resize(window->width() - 2*XOFFSET, 11);

    y += OFFSET3;
    GUI::VerticalLine *l = new GUI::VerticalLine(window);
    l->move(XOFFSET, y);
    l->resize(window->width() - 2*XOFFSET, 2);
  }

  {
    int y = 203;
#define OFFSET4 21

    // Enable Velocity
    GUI::Label *lbl_velocity = new GUI::Label(window);
    lbl_velocity->resize(78 ,20);
    lbl_velocity->move(16, y);
    lbl_velocity->setText("Humanizer");

    check = new GUI::CheckBox(window);
    //check->setText("Enable Velocity Modifier");
    check->move(26, y + OFFSET4);
    check->resize(59,38);
    check->registerClickHandler(checkClick, this);

    // Velocity Weight Modifier:
    {
      GUI::Label *lbl_weight = new GUI::Label(window);
      lbl_weight->setText("Attack");
      lbl_weight->move(107, y);
      lbl_weight->resize(100, 20);
      
      knob = new GUI::Knob(window);
      knob->move(109, y + OFFSET4 - 4);
      knob->resize(57, 57);
      knob->registerClickHandler(knobChange, this);
    }
    
    // Velocity Falloff Modifier:
    {
      GUI::Label *lbl_falloff = new GUI::Label(window);
      lbl_falloff->setText("Release");
      lbl_falloff->move(202 - 17 - 7, y);
      lbl_falloff->resize(100, 20);
      
      knob2 = new GUI::Knob(window);
      knob2->move(202 - 13 - 5,  y + OFFSET4 - 4);
      knob2->resize(57, 57);
      knob2->registerClickHandler(knobChange2, this);
    }
  }

  GUI::VerticalLine *l2 = new GUI::VerticalLine(window);
  l2->move(20, 310 - 15 - 9);
  l2->resize(window->width() - 40, 2);

  GUI::Label *lbl_version = new GUI::Label(window);
  lbl_version->setText(".::. v"VERSION"  .::.  http://www.drumgizmo.org  .::.  GPLv3 .::.");
  lbl_version->move(16, 300);
  lbl_version->resize(window->width(), 20);
  /*
  {
    GUI::ComboBox *cmb = new GUI::ComboBox(window);
    cmb->addItem("Foo", "Bar");
    cmb->addItem("Hello", "World");
    cmb->move(10,10);
    cmb->resize(70, 30);
  }
  */
  // Create filebrowser
  filebrowser = new GUI::FileBrowser(window);
  filebrowser->move(0, 0);
  filebrowser->resize(window->width() - 1, window->height() - 1);
  filebrowser->hide();
  fb = filebrowser;

  // Enable quit button
//  GUI::Button *btn_quit = new GUI::Button(window);
//  btn_quit->setText("Quit");
//  btn_quit->move(50,280);
//  btn_quit->resize(80,80);
//  btn_quit->registerClickHandler(quit, this);

  window->show();

  sem.post();

  initialised = true;
}

void PluginGUI::show()
{
  while(!initialised) usleep(10000);

  if(!window) init();

  window->show();
}

void PluginGUI::hide()
{
  while(!initialised) usleep(10000);

  if(window) window->hide();
}

void PluginGUI::processEvents()
{
  if(!initialised) return;

  if(closing) {
    if(windowClosedHandler) windowClosedHandler(windowClosedPtr);
    closing = false;
  }

#ifndef USE_THREAD
  window->eventHandler()->processEvents(window);
#endif/*USE_THREAD*/
}

void PluginGUI::setWindowClosedCallback(void (*handler)(void *), void *ptr)
{
  windowClosedHandler = handler;
  windowClosedPtr = ptr;
}

#ifdef STANDALONE

class Engine : public MessageHandler {
public:
  void handleMessage(Message *msg) {}
};


void stop(void *ptr)
{
  DEBUG(stop, "Stopping...\n");
  bool *running = (bool*)ptr;
  *running = false;
}

int main()
{
/*
  hug_status_t status = hug_init(HUG_FLAG_OUTPUT_TO_SYSLOG,
                                 HUG_OPTION_SYSLOG_HOST, "192.168.0.10",
                                 HUG_OPTION_SYSLOG_PORT, 514,
                                 HUG_OPTION_END);

  if(status != HUG_STATUS_OK) {
    printf("Error: %d\n", status);
    return 1;
  }
*/
  INFO(example, "We are up and running");

  bool running = true;

  PluginGUI gui;
  gui.setWindowClosedCallback(stop, &running);

  // gui.show();

  while(running) {
    //    gui.processEvents();
#ifdef WIN32
    SleepEx(1000, FALSE);
#else
    //    usleep(10000);
    sleep(1);
#endif
  }

  return 0;
}

#endif/*STANDALONE*/
