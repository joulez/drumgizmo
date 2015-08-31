/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            mutex.h
 *
 *  Thu Nov 12 10:51:32 CET 2009
 *  Copyright 2009 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of Pracro.
 *
 *  Pracro is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Pracro is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Pracro; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#ifndef __PRACRO_MUTEX_H__
#define __PRACRO_MUTEX_H__

struct mutex_private_t;

class Mutex {
public:
  Mutex();
  ~Mutex();

  bool trylock();
  void lock();
  void unlock();

private:
  struct mutex_private_t* prv;
};

class MutexAutolock {
public:
  MutexAutolock(Mutex &mutex);
  ~MutexAutolock();

private:
  Mutex &mutex;
};

#endif/*__PRACRO_MUTEX_H__*/
