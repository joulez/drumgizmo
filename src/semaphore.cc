/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            semaphore.cc
 *
 *  Sat Oct  8 17:44:13 CEST 2005
 *  Copyright  2005 Bent Bisballe Nyeng
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
#include "semaphore.h"

#include <hugin.hpp>

#ifdef WIN32
#include <windows.h>
#else
// Make sure we don't include /this/ file...
#include <../include/semaphore.h>
#endif

struct semaphore_private_t {
#ifdef WIN32
  HANDLE semaphore; 
#else
  sem_t semaphore;
#endif
};

Semaphore::Semaphore(const char *name)
{
  this->name = name;
  DEBUG(semaphore, "Create [%s]\n", name);

  prv = new struct semaphore_private_t();

#ifdef WIN32
  prv->semaphore = CreateSemaphore(NULL,  // default security attributes
                                   0, // initial count
                                   2147483647, // maximum count (Max LONG)
                                   NULL); // unnamed semaphore
#else
  sem_init(&prv->semaphore, 0, 0);
#endif
}

Semaphore::~Semaphore()
{
  DEBUG(semaphore, "Delete [%s]\n", name);

#ifdef WIN32
  CloseHandle(prv->semaphore);
#else
  sem_destroy(&prv->semaphore);
#endif

  if(prv) delete prv;
}

void Semaphore::post()
{
  DEBUG(semaphore, "Post [%s]\n", name);

#ifdef WIN32
  ReleaseSemaphore(prv->semaphore, 1, NULL);
#else
  sem_post(&prv->semaphore);
#endif
}

void Semaphore::wait()
{
  DEBUG(semaphore, "Wait [%s]\n", name);

#ifdef WIN32
  WaitForSingleObject(prv->semaphore, INFINITE);
#else
  sem_wait(&prv->semaphore);
#endif
}

#ifdef TEST_SEMAPHORE
//deps:
//cflags: -I.. $(PTHREAD_CFLAGS)
//libs: $(PTHREAD_LIBS)
#include <test.h>

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).
TEST_TRUE(false, "No tests yet!");

TEST_END;

#endif/*TEST_SEMAPHORE*/
