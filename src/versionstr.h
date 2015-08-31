/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            versionstr.h
 *
 *  Wed Jul 22 11:41:32 CEST 2009
 *  Copyright 2009 Bent Bisballe Nyeng
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
#ifndef __PRACRO_VERSIONSTR_H__
#define __PRACRO_VERSIONSTR_H__

#include <string>

// Workaround - major, minor and patch are defined as macros when using _GNU_SOURCES
#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif
#ifdef patch
#undef patch
#endif

/**
 * VersionStr class.
 * It hold a version number and is capable of correct sorting, as well as string 
 * conversion both ways.
 */
class VersionStr {
public:
  /**
   * Constructor.
   * Throws an exeption if the string does not parse.
   * @param v A std::string containing a version string on the form a.b or a.b.c
   */
  VersionStr(std::string v) throw(const char *);

  /**
   * Constructor.
   * @param major A size_t containing the major version number.
   * @param minor A size_t containing the minor version number.
   * @param patch A size_t containing the patch level.
   */
  VersionStr(size_t major = 0, size_t minor = 0, size_t patch = 0);

  /**
   * Typecast to std::string operator.
   * It simply converts the version numbers into a string of the form major.minor
   * (if patch i 0) or major.minor.patch
   */
  operator std::string() const;

  /**
   * Assignment from std::string operator.
   * Same as in the VersionStr(std::string v) constructor.
   * Throws an exeption if the string does not parse.
   */
  void operator=(std::string v) throw(const char *);

  /**
   * Comparison operator.
   * The version objects are sorted according to their major, minor and patch
   * level numbers.
   */
  bool operator<(const VersionStr &other) const;
  bool operator==(const VersionStr &other) const;
  bool operator>(const VersionStr &other) const;
  bool operator>=(const VersionStr &other) const;
  bool operator<=(const VersionStr &other) const;


  /**
   * @return Major version number.
   */
  size_t major() const;

  /**
   * @return Minor version number.
   */
  size_t minor() const;

  /**
   * @return Patch level.
   */
  size_t patch() const;

private:
  void set(std::string v) throw(const char *);
  size_t version[3];
};

#endif/*__PRACRO_VERSIONSTR_H__*/
