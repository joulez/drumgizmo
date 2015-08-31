/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            messagereceiver.h
 *
 *  Sun Jun 16 12:09:06 CEST 2013
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
#ifndef __DRUMGIZMO_MESSAGERECEIVER_H__
#define __DRUMGIZMO_MESSAGERECEIVER_H__

#include <list>

#include "mutex.h"
#include "message.h"
#include "messagehandler.h"

class MessageReceiver {
  friend class MessageHandler;
public:
  MessageReceiver(message_receiver_id_t id);
  ~MessageReceiver();

  /**
   * Receive message from the message queue.
   */
  Message *receiveMessage();

  /**
   * Receive message from the message queue without removing it.
   */
  Message *peekMessage();

  /**
   * Add a message to the message queue.
   */
  void sendMessage(Message *msg);

  /**
   * Handle messages from the event queue.
   * @param max_number_of_events the maximum number of events to be handled in
   * this call. 0 means all.
   */
  void handleMessages(size_t max_number_of_events = 0);

  /**
   * Handler to be implemented in child classes.
   * Handles a single event.
   */
  virtual void handleMessage(Message *msg) = 0;

private:
  Mutex message_mutex;
  std::list<Message *> message_queue;
};

#endif/*__DRUMGIZMO_MESSAGERECEIVER_H__*/
