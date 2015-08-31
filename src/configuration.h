/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            configuration.h
 *
 *  Sat Oct  8 14:37:13 CEST 2011
 *  Copyright 2011 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_CONFIGURATION_H__
#define __DRUMGIZMO_CONFIGURATION_H__

namespace Conf {
  extern bool enable_velocity_modifier;
  extern float velocity_modifier_falloff;
  extern float velocity_modifier_weight;

  extern bool enable_velocity_randomiser;
  extern float velocity_randomiser_weight;

  extern int samplerate;

  extern bool enable_resampling;
};


#endif/*__DRUMGIZMO_CONFIGURATION_H__*/
