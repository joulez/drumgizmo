/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            messagehandler.h
 *
 *  Fri Jun 14 20:30:43 CEST 2013
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
#ifndef __DRUMGIZMO_MESSAGEHANDLER_H__
#define __DRUMGIZMO_MESSAGEHANDLER_H__

#include <map>

#include "message.h"
#include "mutex.h"

typedef enum {
  MSGRCV_ENGINE = 1,
  MSGRCV_UI = 2,
  MSGRCV_LOADER = 3,
} message_receiver_id_t;

class MessageReceiver;

class MessageHandler {
public:
  MessageHandler();

  void addReceiver(message_receiver_id_t id, MessageReceiver *receiver);
  void removeReceiver(MessageReceiver *receiver);

  /**
   * Send Message to receiver with specified id.
   * @return Return true if id is registered. Return false if id is not
   * currently registered.
   */
  bool sendMessage(message_receiver_id_t id, Message* msg);

private:
  std::map<message_receiver_id_t, MessageReceiver *> receivers;

  Mutex mutex;
};

// Global MessageHandler;
extern MessageHandler msghandler;

#endif/*__DRUMGIZMO_MESSAGEHANDLER_H__*/
