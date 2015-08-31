/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiofile.h
 *
 *  Tue Jul 22 17:14:11 CEST 2008
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
#ifndef __DRUMGIZMO_AUDIOFILE_H__
#define __DRUMGIZMO_AUDIOFILE_H__

#include <string>
#include <map>
#include <vector>

#include <sndfile.h>

#include "mutex.h"
#include "audio.h"

/*
  Plan for lazy loading of audio (Brainstorming)
    * Encapsulate data array?
      - Speed issues?
      - Other suggestion
        * Trigger on read begin and read done
          - readnext(instrument)?
        * size_t current latest loaded sample
        * run in own thread? threads in drumgizmo??
          - Add soundfile-loader-class which run in its own thread
    * Add pre-loading constant
    * Pointer to pos in audio stream (maybe just last position read)
    * Strategy for how to handle pre-loading of remaining file
      - Is it acceptable only to handle sequential reading of data (no random access)?

   Thread A                                                  Thread B

   :preload constant (user defined)
   :speed modifier constant (in which time must 
    sample n be loaded relative to trigger time) 
  ----------                                                           ------
  | Loader |   <------- Trigger load of InstrumentSample n  --------- | DG   | 
  ----------                                                           ------
    Load                  (int- right most loaded sample --> If current sample pos loaded
      |            --------- |                                   |
    Wave Into --> | SndFile | <----- Read data (directly from array)
                   ---------  
*/

//#define LAZYLOAD

#define ALL_SAMPLES -1

class AudioFile {
public:
  AudioFile(std::string filename, int filechannel);
  ~AudioFile();

  void load(int num_samples = ALL_SAMPLES);
  void unload();

  bool isLoaded();

  volatile size_t size;
  volatile sample_t *data;

  std::string filename;

#ifdef LAZYLOAD
//  SF_INFO sf_info;
//  SNDFILE *fh;
//  bool completely_loaded;
  void init();
  void reset();
  void loadNext();
  sample_t* preloaded_data; 
#endif/*LAZYLOAD*/

  bool isValid();

  Mutex mutex;

private:
  void *magic;
  volatile bool is_loaded;
  int filechannel;
};

#endif/*__DRUMGIZMO_AUDIOFILE_H__*/
