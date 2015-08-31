/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            hugin_util.h
 *
 *  Fri Jan  4 10:10:03 CET 2013
 *  Copyright 2013 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of Hugin.
 *
 *  Hugin is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Hugin is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Hugin; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#ifndef __HUGIN_HUGIN_UTIL_H__
#define __HUGIN_HUGIN_UTIL_H__

#ifndef DISABLE_HUGIN

#include <time.h>

/**
 * Threadsafe version of localtime (if compiled with mutex support).
 * See man page for the posix localtime_r function.
 */
struct tm *hug_localtime(const time_t *timep, struct tm *result);

#endif/*DISABLE_HUGIN*/

#endif/*__HUGIN_HUGIN_UTIL_H__*/
