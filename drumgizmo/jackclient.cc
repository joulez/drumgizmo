/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            jackclient.cc
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
#include "jackclient.h"

extern "C"
{
  int _wrap_jack_process(jack_nframes_t nframes, void *arg){
    return ((JackClient*)arg)->process(nframes);}
}  // extern "C"

JackClient::JackClient()
  : refcnt(0)
{
	jack_status_t status;

	jack_client = jack_client_open("DrumGizmo", JackNullOption, &status);

  jack_set_process_callback(jack_client, _wrap_jack_process, this);

  active = false;
}

JackClient::~JackClient()
{
	jack_client_close(jack_client);
}

/*
void JackClient::addJackProcess(JackProcess *process)
{
  jack_processes.insert(process);
}
*/

void JackClient::removeJackProcess(JackProcess *process)
{
  jack_processes.erase(process);
}

void JackClient::activate()
{
	if(!active) jack_activate(jack_client);
  active = true;
}

int JackClient::process(jack_nframes_t nframes)
{
  std::set<JackProcess *>::iterator i = jack_processes.begin();
  while(i != jack_processes.end()) {
    JackProcess *jp = *i;
    jp->jack_process(nframes);
    i++;
  }

	return 0;
}

JackClient *jackclient = NULL;

JackClient *init_jack_client()
{
  if(jackclient == NULL) jackclient = new JackClient();
  jackclient->refcnt++;
  return jackclient;

}
void close_jack_client()
{
  if(jackclient) {
    jackclient->refcnt--;
    if(jackclient->refcnt == 0) {
      delete jackclient;
      jackclient = NULL;
    }
  }
}
