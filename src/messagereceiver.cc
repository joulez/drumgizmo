/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            messagereceiver.cc
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
#include "messagereceiver.h"

#include <hugin.hpp>

MessageReceiver::MessageReceiver(message_receiver_id_t id)
{
  msghandler.addReceiver(id, this);
}

MessageReceiver::~MessageReceiver()
{
  msghandler.removeReceiver(this);
}

void MessageReceiver::sendMessage(Message *msg)
{
  MutexAutolock l(message_mutex);

  message_queue.push_back(msg);
}

Message *MessageReceiver::receiveMessage()
{
  Message *msg = NULL;
  if(message_queue.size()) {
    msg = message_queue.front();
    message_queue.pop_front();
  }
  return msg;
}

Message *MessageReceiver::peekMessage()
{
  Message *msg = NULL;
  if(message_queue.size()) {
    msg = message_queue.front();
  }
  return msg;
}

void MessageReceiver::handleMessages(size_t max)
{
  MutexAutolock l(message_mutex);
  bool process_all = false;
  if(max == 0) process_all = true;

  while((process_all || max--) && peekMessage()) {
    Message *msg = receiveMessage();
    handleMessage(msg);
    delete msg;
  }
}
