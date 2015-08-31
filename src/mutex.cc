/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            mutex.cc
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
#include "mutex.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

struct mutex_private_t {
#ifdef WIN32
  HANDLE mutex; 
#else
  pthread_mutex_t mutex;
#endif
};

Mutex::Mutex()
{
  prv = new struct mutex_private_t();
#ifdef WIN32
  prv->mutex = CreateMutex(NULL,  // default security attributes
                           FALSE, // initially not owned
                           NULL); // unnamed mutex
#else
  pthread_mutex_init (&prv->mutex, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
  CloseHandle(prv->mutex);
#else
  pthread_mutex_destroy(&prv->mutex);
#endif

  if(prv) delete prv;
}

void Mutex::lock()
{
#ifdef WIN32
  WaitForSingleObject(prv->mutex, // handle to mutex
                      INFINITE);  // no time-out interval
#else
  pthread_mutex_lock(&prv->mutex);
#endif
}

void Mutex::unlock()
{
#ifdef WIN32
  ReleaseMutex(prv->mutex);
#else
  pthread_mutex_unlock(&prv->mutex);
#endif
}

MutexAutolock::MutexAutolock(Mutex &m)
  : mutex(m)
{
  mutex.lock();
}

MutexAutolock::~MutexAutolock()
{
  mutex.unlock();
}

#ifdef TEST_MUTEX
//deps:
//cflags: $(PTHREAD_CFLAGS)
//libs: $(PTHREAD_LIBS)
#include <test.h>

#include <unistd.h>

volatile int cnt = 0;

static void* thread_run(void *data)
{
  Mutex *mutex = (Mutex*)data;
  mutex->lock();
  cnt++;
  mutex->unlock();
  return NULL;
}

TEST_BEGIN;

Mutex mutex;

mutex.lock();
TEST_FALSE(mutex.trylock(), "Testing if trylock works negative.");
mutex.unlock();
TEST_TRUE(mutex.trylock(), "Testing if trylock works positive.");
mutex.unlock();

mutex.lock();

pthread_attr_t attr;
pthread_t tid;
pthread_attr_init(&attr);
pthread_create(&tid, &attr, thread_run, &mutex);

sleep(1);
TEST_EQUAL_INT(cnt, 0, "Testing if lock prevent cnt from increasing.");
mutex.unlock();

sleep(1);
TEST_EQUAL_INT(cnt, 1, "Testing if unlock makes cnt increase.");

pthread_join(tid, NULL);
pthread_attr_destroy(&attr);

{
  TEST_TRUE(mutex.trylock(), "Testing if autolock has not yet locked the mutex.");
  mutex.unlock();
  MutexAutolock mlock(mutex);
  TEST_FALSE(mutex.trylock(), "Testing if autolock worked.");
}

TEST_TRUE(mutex.trylock(), "Testing if autolock has released the lock on the mutex.");
mutex.unlock();

TEST_END;

#endif/*TEST_MUTEX*/
