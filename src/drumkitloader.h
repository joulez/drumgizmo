/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumkitloader.h
 *
 *  Thu Jan 17 20:54:13 CET 2013
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
#ifndef __DRUMGIZMO_DRUMKITLOADER_H__
#define __DRUMGIZMO_DRUMKITLOADER_H__

#include <string>
#include <list>

#include "thread.h"
#include "semaphore.h"
#include "mutex.h"

#include "drumkit.h"

/**
 * This class is responsible for loading the drumkits in its own thread.
 * All interaction calls are simply modifying queues and not doing any
 * work in-sync with the caller.
 * This means that if loadKit(...) is called, one cannot assume that the
 * drumkit has actually been loaded when the call returns.
 */
class DrumKitLoader : public Thread {
public:
  /**
   * The constrcutor starts the loader thread.
   */
  DrumKitLoader();

  /**
   * The destructor signals the thread to stop and waits to merge before
   * returning (ie. deleting the object will garantuee that the thread has
   * been stopped).
   */
  ~DrumKitLoader();

  /**
   * Signal the loader to start loading all audio files contained in kit.
   * All other AudioFiles in queue will be removed before the new ones are
   * scheduled.
   */
  void loadKit(DrumKit *kit);
  
  // I have no idea what this does..
  //void reset(AudioFile* af);

  void thread_main();

  /**
   * Simply reports if the load queue is empty (i.e. all AudioFiles has been
   * loaded).
   */
  bool isDone();

  /**
   * Signal the loader to stop and wait until it has.
   */
  void stop();

  /**
   * Skip all queued AudioFiles.
   */
  void skip();

private:
  Semaphore run_semaphore;
  Semaphore semaphore;
  Mutex mutex;
  volatile bool running;
  std::list<AudioFile*> load_queue;
  size_t total_num_audiofiles;
  size_t fraction;
  size_t loaded;
};

#endif/*__DRUMGIZMO_DRUMKITLOADER_H__*/
