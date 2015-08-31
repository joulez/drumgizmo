/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            hugin_syslog.h
 *
 *  Fri Dec  7 14:24:54 CET 2012
 *  Copyright 2012 Jonas Suhr Christensen
 *  jsc@umbraculum.org
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
#ifndef __HUGIN_HUGIN_SYSLOG_H__
#define __HUGIN_HUGIN_SYSLOG_H__

#ifndef DISABLE_HUGIN

void hug_syslog_init(const char* host, int port);

void hug_syslog_output(char* msg);

void hug_syslog_close();

#endif/*DISABLE_HUGIN*/

#endif/*__HUGIN_HUGIN_SYSLOG_H__*/
