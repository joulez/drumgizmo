/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            jackclient.h
 *
 *  Sun Jul 20 21:48:44 CEST 2008
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
#ifndef __DRUMGIZMO_JACKCLIENT_H__
#define __DRUMGIZMO_JACKCLIENT_H__

#include <jack/jack.h>
#include <set>

class JackProcess {
public:
  virtual ~JackProcess() {}
  virtual void jack_process(jack_nframes_t nframes) = 0;
};

class JackClient {
public:
  JackClient();
  ~JackClient();

  void addJackProcess(JackProcess *process)
  {
    jack_processes.insert(process);
  }

  void removeJackProcess(JackProcess *process);

  void activate();
  int process(jack_nframes_t nframes);

	jack_client_t *jack_client;

  // Sort of private...
  int refcnt;

private:
  std::set<JackProcess *> jack_processes;
  bool active;
};

JackClient *init_jack_client();
void close_jack_client();

#endif/*__DRUMGIZMO_JACKCLIENT_H__*/
