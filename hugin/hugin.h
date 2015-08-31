/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            hugin.h
 *
 *  Thu Nov  1 13:38:47 CET 2012
 *  Copyright 2012 Bent Bisballe Nyeng
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
#ifndef __HUGIN_HUGIN_H__
#define __HUGIN_HUGIN_H__

typedef enum {
  HUG_STATUS_OK = 0,
  HUG_STATUS_UNKNOWN_OPTION,
  HUG_STATUS_ERROR,
} hug_status_t;

#ifdef DISABLE_HUGIN

#define ERR(ch, fmt...)
#define WARN(ch, fmt...)
#define INFO(ch, fmt...)
#define DEBUG(ch, fmt...)
#define hug_init(...) HUG_STATUS_OK
#define hug_close()

#else/*DISABLE_HUGIN*/

enum HUG_FLAG {
  // Features
#ifdef WITH_HUG_THREAD
  HUG_FLAG_USE_THREAD       = 0x00000001,
#endif
#ifdef WITH_HUG_MUTEX
  HUG_FLAG_USE_MUTEX        = 0x00000002,
#endif
#ifdef WITH_HUG_FILTER
  HUG_FLAG_USE_FILTER       = 0x00000004,
#endif

  // Outputs
  HUG_FLAG_OUTPUT_TO_STDOUT = 0x00010000,
  HUG_FLAG_OUTPUT_TO_STDERR = 0x00020000,
  HUG_FLAG_OUTPUT_TO_FD     = 0x00040000,
  HUG_FLAG_OUTPUT_TO_FILE   = 0x00080000,
#ifdef WITH_HUG_SYSLOG
  HUG_FLAG_OUTPUT_TO_SYSLOG = 0x00100000,
#endif

  // Default value of flags
  HUG_FLAG_DEFAULT          = HUG_FLAG_OUTPUT_TO_STDOUT, // Output to stdout
};

enum HUG_OPTION {
  /**
   * No more options / last option.  This is used
   * to terminate the VARARGs list.
   */
  HUG_OPTION_END,

  /**
   * const char* argument containing a filename which will be used for log
   * output. To be used with the HUG_FLAG_OUTPUT_TO_FILE flag.
   */
  HUG_OPTION_FILENAME,

  /**
   * Integer argument describing a file descriptor which will be used for log
   * output. To be used with the HUG_FLAG_OUTPUT_TO_FD flag.
   */
  HUG_OPTION_FD,

  /**
   * Set this option to make the stdout output to be printed without any date
   * information in the header.
   * Parameter is an integer.
   * Values:
   *  0 := use date
   *  1 := do not use date.
   */
  HUG_OPTION_STDOUT_NO_DATE,

  /**
   * Host and port to use when logging on an external server.
   * Host is a const char* argument, port is an integer.
   * To be used with the HUG_FLAG_USE_SYSLOG flag.
   * Linux: If HUG_OPTION_SYSLOG_HOST is not supplied, the local syslog will be
   * used.
   * Windows: If HUG_OPTION_SYSLOG_HOST is not supplied an error will be
   * returned by hugin_init.
   * If HUG_OPTION_SYSLOG_PORT is not supplied, the default syslogd port will
   * be used (port 514).
   */
#ifdef WITH_HUG_SYSLOG
  HUG_OPTION_SYSLOG_HOST,
  HUG_OPTION_SYSLOG_PORT,
#endif

  /**
   * Filter option. Argument is a const char *.
   * fmt := [set[,set]*]*
   * set := [+-]channel
   *     |  class[+-]channel
   *     |  [+-]all   
   */
#ifdef WITH_HUG_FILTER
  HUG_OPTION_FILTER,
#endif
};

/**
 * @param flags combination of HUG_FLAG values
 * @param ... list of options (type-value pairs,
 *        terminated with HUG_OPTION_END).
 * @return 0 on success, 1 on error.
 */
hug_status_t hug_init(unsigned int flags, ...);
void hug_close();

/**
 * Example of usage (use mutex protected calls, send output to file):
 *
 * hug_status_t status;
 * status = hugin_init(HUG_FLAG_OUTPUT_TO_FILE | HUG_FLAG_USE_MUTEX,
 *                     HUG_OPTION_FILENAME, "/tmp/my.log",
                       HUG_OPTION_END);
 * if(status != HUG_STATUS_OK) exit(1);
 * INFO(example, "We are up and running\n");
 * hug_close();
 */

/**
 * Example of usage (simply outputs to stdout):
 *
 * INFO(example, "We are up and running\n");
 */

enum __debug_class
{
  __class_fixme = 0,
	__class_err = 1,
	__class_warn = 2,
	__class_info = 3,
	__class_debug = 4
};

int __debug(const char *func, const int line, enum __debug_class cl,
            const char *ch, const char *fmt, ...)
  __attribute__((format (printf,5,6)));

#define __DEBUG_PRINT(cl, ch, fmt...)	\
	do { __debug(__func__, __LINE__, cl, ch, fmt); } while(0)
#define __DEBUG(cl, ch, fmt...) \
	__DEBUG_PRINT(__class##cl, #ch, fmt)

#define ERR(ch, fmt...) __DEBUG(_err, ch, fmt)
#define WARN(ch, fmt...) __DEBUG(_warn, ch, fmt)
#define INFO(ch, fmt...) __DEBUG(_info, ch, fmt)
#define DEBUG(ch, fmt...) __DEBUG(_debug, ch, fmt)

#endif/*DISABLE_HUGIN*/

#endif/*__HUGIN_HUGIN_H__*/
