/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            channel.cc
 *
 *  Tue Jul 22 17:14:28 CEST 2008
 *  Copyright 2008 Bent Bisballe Nyeng
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
#include "channel.h"

Channel::Channel(std::string name)
{
  this->name = name;
  num = NO_CHANNEL;
}

#ifdef TEST_CHANNEL
//deps:
//cflags:
//libs:
#include "test.h"

TEST_BEGIN;

Channel c1;
TEST_EQUAL_STR(c1.name, "", "Empty name?");
TEST_EQUAL_INT(c1.num, NO_CHANNEL, "No physical channel assigned?");

Channel c2("ch2");
TEST_EQUAL_STR(c2.name, "ch2", "Nonempty name?");
TEST_EQUAL_INT(c2.num, NO_CHANNEL, "No physical channel assigned?");

Channels channels;
channels.push_back(c1);
channels.push_back(c2);

TEST_END;

#endif/*TEST_CHANNEL*/
