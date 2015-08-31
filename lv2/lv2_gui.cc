/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lv2.cc
 *
 *  Wed Jul 13 13:50:33 CEST 2011
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
#include "lv2_gui.h"

#include <stdio.h>
#include <string.h>

#include <lv2/lv2plug.in/ns/ext/instance-access/instance-access.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "lv2_instance.h"

// From: http://codesearch.google.com/#50sg5qT6WNE/src/lv2_ui_dssi.c
// git://repo.or.cz/nekobee.git/src/lv2_ui_dssi.c

#define DRUMGIZMO_UI_URI "http://drumgizmo.org/lv2-gui"

#include <plugingui.h>

/**
 * When LV2_EXTERNAL_UI_URI UI is instantiated, the returned
 * LV2UI_Widget handle must be cast to pointer to struct lv2_external_ui.
 * UI is created in invisible state.
 */
struct lv2_external_ui
{
  /**
   * Host calls this function regulary. UI library implementing the
   * callback may do IPC or redraw the UI.
   *
   * @param _this_ the UI context
   */
  void (* run)(struct lv2_external_ui * _this_);

  /**
   * Host calls this function to make the plugin UI visible.
   *
   * @param _this_ the UI context
   */
  void (* show)(struct lv2_external_ui * _this_);

  /**
   * Host calls this function to make the plugin UI invisible again.
   *
   * @param _this_ the UI context
   */
  void (* hide)(struct lv2_external_ui * _this_);
};

/** UI extension suitable for out-of-process UIs */
#define LV2_EXTERNAL_UI_URI "http://lv2plug.in/ns/extensions/ui#external"

/**
 * On UI instantiation, host must supply LV2_EXTERNAL_UI_URI
 * feature. LV2_Feature::data must be pointer to struct lv2_external_ui_host. */
struct lv2_external_ui_host
{
  /**
   * Callback that plugin UI will call
   * when UI (GUI window) is closed by user.
   * This callback wil; be called during execution of lv2_external_ui::run()
   * (i.e. not from background thread).
   *
   * After this callback is called, UI is defunct. Host must call
   * LV2UI_Descriptor::cleanup(). If host wants to make the UI visible
   * again UI must be reinstantiated.
   *
   * @param controller Host context associated with plugin UI, as
   * supplied to LV2UI_Descriptor::instantiate()
   */
  void (* ui_closed)(LV2UI_Controller controller);

  /**
   * Optional (may be NULL) "user friendly" identifier which the UI
   * may display to allow a user to easily associate this particular
   * UI instance with the correct plugin instance as it is represented
   * by the host (e.g. "track 1" or "channel 4").
   *
   * If supplied by host, the string will be referenced only during
   * LV2UI_Descriptor::instantiate()
   */
  const char * plugin_human_id;
};

struct DG_GUI {
  struct lv2_external_ui virt;

  LV2_Handle instance_handle;
  LV2_Extension_Data_Feature *data_access;
  DrumGizmo *instance;
  LV2UI_Controller controller;

  PluginGUI *gui;
  struct lv2_external_ui_host *ui_host_ptr;
};

static void ui_run(struct lv2_external_ui * _this_)
{
  struct DG_GUI *dggui = (struct DG_GUI *)_this_;
  dggui->gui->processEvents();
}

static void ui_show(struct lv2_external_ui * _this_)
{
  struct DG_GUI *dggui = (struct DG_GUI *)_this_;
  dggui->gui->show();
}

static void ui_hide(struct lv2_external_ui * _this_)
{
  struct DG_GUI *dggui = (struct DG_GUI *)_this_;
  if(dggui->gui) dggui->gui->hide();
}

static void closeHandler(void *ptr)
{
  struct DG_GUI *gui = (struct DG_GUI *)ptr;

  if(gui->ui_host_ptr && gui->ui_host_ptr->ui_closed) {
    gui->ui_host_ptr->ui_closed(gui->controller);
  }

  delete gui->gui;
  gui->gui = NULL;
}

static LV2UI_Handle ui_instantiate(const struct _LV2UI_Descriptor * descriptor,
                                   const char * plugin_uri,
                                   const char * bundle_path,
                                   LV2UI_Write_Function write_function,
                                   LV2UI_Controller controller,
                                   LV2UI_Widget * widget,
                                   const LV2_Feature * const * features)
{
  printf("ui_instantiate\n");

  struct DG_GUI* pt = new struct DG_GUI;

  pt->ui_host_ptr = NULL;
  pt->controller = controller;

  while (*features != NULL) {
    std::string uri = (*features)->URI;
    void *data = (*features)->data;

    printf("DGUI: feature: %s\n", uri.c_str());

    if(uri == LV2_INSTANCE_ACCESS_URI) {
      pt->instance_handle = data;
    }

    if(uri == LV2_DATA_ACCESS_URI) {
      pt->data_access = (LV2_Extension_Data_Feature *)data;
    }

    if(uri == LV2_EXTERNAL_UI_URI) {
      pt->ui_host_ptr = (struct lv2_external_ui_host *)data;
    }
    features++;
  }

  LV2_DrumGizmo_Descriptor *dgd =
    (LV2_DrumGizmo_Descriptor *)(*pt->data_access->data_access)(PLUGIN_INSTANCE_URI);

  pt->instance = dgd->get_pci(pt->instance_handle);
  pt->virt.run = ui_run;
  pt->virt.show = ui_show;
  pt->virt.hide = ui_hide;
  pt->gui = new PluginGUI();
  pt->gui->setWindowClosedCallback(closeHandler, pt);

  *widget = (LV2UI_Widget)pt;

  return pt;
}

static void ui_cleanup(LV2UI_Handle ui)
{
  struct DG_GUI* pt = (struct DG_GUI*)ui;
  delete pt->gui;
  pt->gui = NULL;
  delete pt;
}

static void ui_port_event(LV2UI_Handle ui,
                          uint32_t port_index,
                          uint32_t buffer_size,
                          uint32_t format,
                          const void * buffer)
{
}

#ifdef __cplusplus
extern "C" {
#endif

static LV2UI_Descriptor descriptor = {
  DRUMGIZMO_UI_URI,
  ui_instantiate,
  ui_cleanup,
  ui_port_event,
  NULL
};

const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index)
{
  if(index == 0) return &descriptor;
  return NULL;
}

#ifdef __cplusplus
}
#endif
