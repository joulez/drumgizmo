/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            messagehandler.cc
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
#include "messagehandler.h"

#include <hugin.hpp>

#include "messagereceiver.h"

// Global messagehandler:
MessageHandler msghandler;

MessageHandler::MessageHandler()
{
}

void MessageHandler::addReceiver(message_receiver_id_t id,
                                 MessageReceiver *receiver)
{
  MutexAutolock l(mutex);

  receivers[id] = receiver;
}

void MessageHandler::removeReceiver(MessageReceiver *receiver)
{
  MutexAutolock l(mutex);

  std::map<message_receiver_id_t, MessageReceiver *>::iterator i =
    receivers.begin();
  while(i != receivers.end()) {
    if(i->second == receiver) {
      receivers.erase(i);
      break;
    }
    i++;
  }
}

bool MessageHandler::sendMessage(message_receiver_id_t id, Message* msg)
{
  MutexAutolock l(mutex);

  if(receivers.find(id) == receivers.end()) {
    //WARN(msghandler, "Could not find id %d\n", id);
    delete msg;
    return false;
  }

  //DEBUG(msghandler, "Sending message to id %d\n", id);

  MessageReceiver *receiver = receivers[id];
  /* // This code causes sporadic segfaults on windows.
  if(msg->processing_mode() == Message::FilterMultiple) {
    Message *pmsg;
    MutexAutolock lock(receiver->message_mutex); // Make peek/receive atomic.
    while( (pmsg = receiver->peekMessage()) != NULL) {
      if(pmsg->type() != msg->type()) break;
      // Remove all old messages with same type.
      delete receiver->receiveMessage();
    }
  }
  */
  receiver->sendMessage(msg);
  return true;
}
