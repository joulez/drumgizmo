/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            thread.cc
 *
 *  Tue Jan 24 08:11:37 CET 2012
 *  Copyright 2012 Bent Bisballe Nyeng
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
#include "thread.h"

#include <stdio.h>
#include <hugin.hpp>

#ifdef WIN32
static DWORD WINAPI thread_run(void *data)
#else
static void* thread_run(void *data)
#endif/*WIN32*/
{
  DEBUG(thread, "Thread run\n");
  Thread *t = (Thread*)data;
  t->thread_main();
  return 0;
}

Thread::Thread()
{}

Thread::~Thread()
{}

void Thread::run()
{
  DEBUG(thread, "Thread::run()\n");
#ifdef WIN32
  tid = CreateThread(NULL, 0, thread_run, this, 0, NULL);
#else
  pthread_create(&tid, NULL, thread_run, this);
#endif/*WIN32*/
}

void Thread::wait_stop()
{
#ifdef WIN32
  WaitForSingleObject(tid, INFINITE);
#else
  pthread_join(tid, NULL);
#endif/*WIN32*/
}

#ifdef TEST_THREAD
//Additional dependency files
//deps:
//Required cflags (autoconf vars may be used)
//cflags:
//Required link options (autoconf vars may be used)
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).

TEST_END;

#endif/*TEST_THREAD*/
