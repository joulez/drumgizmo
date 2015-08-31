/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            resource.cc
 *
 *  Sun Mar 17 19:38:04 CET 2013
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
#include "resource.h"

#include <stdio.h>
#include <hugin.hpp>
#include "resource_data.h"

GUI::Resource::Resource(std::string name)
{
  is_valid = false;
  is_internal = false;

  if(name.length() == 0) return;
  if(name[0] == ':') {
    i_data = NULL;
    i_size = 0;

    // Use internal resource:
    const rc_data_t *p = rc_data;
    while(p->name[0] == ':') {
      if(std::string(p->name) == name) {
        i_data = p->data;
        i_size = p->size;
        break;
      }
      p++;
    }

    // We did not find the named resource.
    if(i_data == NULL) {
      ERR(rc, "Could not find '%s'\n", name.c_str());
      return;
    }

    is_internal = true;
  } else {
    // Read from file:
    FILE *fp = fopen(name.c_str(), "r");
    if(!fp) return;
    char buf[32];
    while(!feof(fp)) {
      size_t sz = fread(buf, 1, sizeof(buf), fp);
      e_data.append(buf, sz);
    }
    fclose(fp);
    is_internal = false;
 }

  is_valid = true;
}

const char *GUI::Resource::data()
{
  if(is_valid == false) return NULL;
  if(is_internal) {
    return i_data;
  } else {
    return e_data.data();
  }
  return NULL;
}

size_t GUI::Resource::size()
{
  if(is_valid == false) return 0;
  if(is_internal) {
    return i_size;
  } else {
    return e_data.length();
  }
  return 0;
}

bool GUI::Resource::valid()
{
  return is_valid;
}

#ifdef TEST_RESOURCE
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

#endif/*TEST_RESOURCE*/
