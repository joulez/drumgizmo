/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumkitloader.cc
 *
 *  Thu Jan 17 20:54:14 CET 2013
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
#include "drumkitloader.h"

#include <hugin.hpp>

#include "drumkitparser.h"
#include "drumgizmo.h"

DrumKitLoader::DrumKitLoader()
  : semaphore("drumkitloader")
{
  run();
  run_semaphore.wait(); // Wait for the thread to actually start.
}

DrumKitLoader::~DrumKitLoader()
{
  if(running) {
    stop();
  }
}

void DrumKitLoader::stop()
{
  {
    MutexAutolock l(mutex);
    load_queue.clear();
  }

  running = false;
  semaphore.post();
  wait_stop();
}

void DrumKitLoader::skip()
{
  MutexAutolock l(mutex);  
  load_queue.clear();
}

bool DrumKitLoader::isDone()
{
  MutexAutolock l(mutex);
  return load_queue.size() == 0;
}

void DrumKitLoader::loadKit(DrumKit *kit)
{
  MutexAutolock l(mutex);

  DEBUG(loader, "Create AudioFile queue from DrumKit\n");

  total_num_audiofiles = 0;// For UI Progress Messages

  { // Count total number of files that need loading:
    Instruments::iterator i = kit->instruments.begin();
    while(i != kit->instruments.end()) {
      Instrument *instr = *i;
      total_num_audiofiles += instr->audiofiles.size();
      i++;
    }
  }

  fraction = total_num_audiofiles / 200;
  if(fraction == 0) fraction = 1;

  { // Now actually queue them for loading:
    Instruments::iterator i = kit->instruments.begin();
    while(i != kit->instruments.end()) {
      Instrument *instr = *i;
      
      std::vector<AudioFile*>::iterator af = instr->audiofiles.begin();
      while(af != instr->audiofiles.end()) {
        AudioFile *audiofile = *af;
        load_queue.push_back(audiofile);
        af++;
      }
  
      i++;
    }
  }

  loaded = 0; // For UI Progress Messages

  DEBUG(loader, "Queued %d (size: %d) AudioFiles for loading.\n",
        (int)total_num_audiofiles, (int)load_queue.size());

  semaphore.post(); // Start loader loop.
}

void DrumKitLoader::thread_main()
{
  running = true;

  run_semaphore.post(); // Signal that the thread has been started.

  while(running) {
    size_t size;
    {
      MutexAutolock l(mutex);
      size = load_queue.size();
    }

    // Only sleep if queue is empty.
    if(size == 0) semaphore.wait();

    std::string filename;
    {
      MutexAutolock l(mutex);
      if(load_queue.size() == 0) continue;
      AudioFile *audiofile = load_queue.front();
      load_queue.pop_front();
      filename = audiofile->filename;
      audiofile->load();
    }

    loaded++;

    if(loaded % fraction == 0 || loaded == total_num_audiofiles) {
      LoadStatusMessage *ls = new LoadStatusMessage();
      ls->number_of_files = total_num_audiofiles;
      ls->numer_of_files_loaded = loaded;
      ls->current_file = filename;
      msghandler.sendMessage(MSGRCV_UI, ls);
    }
  }

  DEBUG(loader, "Loader thread finished.");
}
