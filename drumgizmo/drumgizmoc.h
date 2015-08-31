/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumgizmoc.h
 *
 *  Mon Jan 12 00:36:37 CET 2015
 *  Copyright 2015 Sergey 'Jin' Bostandzhyan
 *  jin@mediatomb.cc
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

#ifndef __DRUMGIZMOC_H__
#define __DRUMGIZMOC_H__

#include "messagereceiver.h"

class CliMain : public MessageReceiver {
public:
  CliMain();
  virtual ~CliMain();

  int run(int argc, char *argv[]);
  void handleMessage(Message *msg);
private:
  bool loading;
};

#endif//__DRUMGIZMOC_H__

