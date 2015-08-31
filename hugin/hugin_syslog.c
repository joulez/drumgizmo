/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set et sw=2 ts=2: */
/***************************************************************************
 *            hugin_syslog.c
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
#include "hugin_syslog.h"

#ifndef DISABLE_HUGIN

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <ws2tcpip.h>
#include <WinSock2.h>
typedef SOCKET socket_t;
#else
#include <syslog.h>
typedef int socket_t;
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <unistd.h>
#include <errno.h>

#include "hugin_util.h"

#ifndef WITH_HUG_SYSLOG
#warning hugin_syslog.c compiled but WITH_HUG_SYSLOG not defined
#endif

#define DO_NOT_USE_REMOTE -2

#define SYSLOG_MSGLEN 1024
#define SYSLOG_PRILEN 5
#define SYSLOG_TIMELEN 32 
#define SYSLOG_TAGLEN 32
#define SYSLOG_CONTENTLEN SYSLOG_MSGLEN - SYSLOG_PRILEN - SYSLOG_TIMELEN - SYSLOG_TAGLEN
//#define SYSLOG_CONTENTLEN SYSLOG_MSGLEN - SYSLOG_PRILEN - SYSLOG_TAGLEN - SYSLOG_HEADERLEN -1

#define SYSLOG_LENOFEXECNAME 256

static int hug_syslog_sock;
static struct sockaddr_in hug_syslog_sockaddr;
static pid_t pid;
static char execname[SYSLOG_LENOFEXECNAME];

#ifdef WIN32
static void wsastartup()
{
  WORD wVersionRequested = MAKEWORD(2, 0);
  WSADATA wsaData;

  int ret = WSAStartup(wVersionRequested, &wsaData);
  if(ret != 0) {
    fprintf(stderr, "WSAStartup failed.\n");
  }
}
#endif

void hug_syslog_init(const char* host, int port) 
{
  hug_syslog_sock = DO_NOT_USE_REMOTE;

  if(host == NULL) return;

#ifdef WIN32
  wsastartup();
#endif

  //  printf("Initializing syslog module remote %s:%d\n", host, port);
  if ( (hug_syslog_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "Failed to create socket\n");
    return;  
  }
  memset(&hug_syslog_sockaddr, 0, sizeof(hug_syslog_sockaddr));
  hug_syslog_sockaddr.sin_family = AF_INET;
//  hug_syslog_sockaddr.sin_addr.s_addr = inet_addr(host);
  struct hostent *hp = gethostbyname(host);
  memcpy(&(hug_syslog_sockaddr.sin_addr),*(hp->h_addr_list),sizeof(struct in_addr));
  hug_syslog_sockaddr.sin_port = htons(port);

  // This implementation has all kind of possible errors:
  //   * It is Linux only
  //   * Max length of execname (including path, avoid this by cmdline file in reverse order) 
  //     is set to SYSLOG_LENOFEXECNAME
  //   * If not found execname is ""
  pid = getpid();
  char buf[SYSLOG_LENOFEXECNAME];
  FILE* f = fopen("/proc/self/cmdline", "r");
  if(f) {
    char* s = NULL;
    s = fgets(buf, SYSLOG_LENOFEXECNAME, f);
    (void)s;
    fclose(f);
  }
  
  char* bufptr = strrchr(buf, '/');
  if(bufptr) {
    strncpy(execname, bufptr+1, SYSLOG_LENOFEXECNAME-1);
  }
  else {
    strcpy(execname, "\0");
  }

  //  printf("\tRunning as %s with pid %d\n", execname, pid);
}

/*
void hug_syslog_createheader() {
  const time_t rawtime = time(NULL);
  struct tm time;
  localtime_r(&rawtime, &time);
  char timebuf[256];
  strftime(timebuf, 256, "%b %e %H:%M:%S ", &time);

  char bufpri[SYSLOG_PRILEN] = "<20>";
  char buftag[SYSLOG_TAGLEN] = "PROGRAM[PID]: ";

}
*/

/*
 * Syslog message specification (based on rfc3164). 
 * Modified after reading syslog v2. documentation
 *
 * MSG: "<%d%d>%s %s %s%c%s", FACILITY, SEVERITY, TIME, CLIENT, TAG, DELIM, CONTENT
 * 
 * Example:
 *   <34>Oct 11 22:14:15 mymachine su: 'su root' failed for lonvick on /dev/pts/8
 *
 *
 * NOTE! Length of MSG must not exceed 1024 characters!
 *
 * FACILITY:
 *      0             kernel messages
 *      1             user-level messages
 *      2             mail system
 *      3             system daemons
 *      4             security/authorization messages (note 1)
 *      5             messages generated internally by syslogd
 *      6             line printer subsystem
 *      7             network news subsystem
 *      8             UUCP subsystem
 *      9             clock daemon (note 2)
 *      10            security/authorization messages (note 1)
 *      11            FTP daemon
 *      12            NTP subsystem
 *      13            log audit (note 1)
 *      14            log alert (note 1)
 *      15            clock daemon (note 2)
 *      16            local use 0  (local0)
 *      17            local use 1  (local1)
 *      18            local use 2  (local2)
 *      19            local use 3  (local3)
 *      20            local use 4  (local4)
 *      21            local use 5  (local5)
 *      22            local use 6  (local6)
 *      23            local use 7  (local7)
 *
 * SEVERITY:
 *      0       Emergency: system is unusable
 *      1       Alert: action must be taken immediately
 *      2       Critical: critical conditions
 *      3       Error: error conditions
 *      4       Warning: warning conditions
 *      5       Notice: normal but significant condition
 *      6       Informational: informational messages
 *      7       Debug: debug-level messages
 *
 * TIME:
 *     Time of message formatted like: Mmm dd hh:mm:ss
 *
 * CLIENT:
 *     Hostname or
 *     IPv4 address or
 *     IPv6 address
 *
 * TAG: format: PROGRAM[PID]
 *     Program or process information
 *     PROGRAM is Alphanumeric 
 *     PID is numeric 
 *     Must not exceed 32 characters
 * 
 * DELIM:
 *     A nonn-alphanumeric character (eg. ':', '[' ...)
 *     In implementation DELIM is the first character of CONTENT
 *
 * CONTENT:
 *     The message to log.
 *     String of printable characters
 *
 * */

void hug_syslog_output(char* msg) 
{
  if(hug_syslog_sock == DO_NOT_USE_REMOTE) {
#ifdef WIN32
    // TODO: We need to do something reasoanble on win32:
    // See Windows Event Log Functions:
    // http://msdn.microsoft.com/en-us/library/windows/desktop/aa385784.aspx
#else
    syslog(LOG_DEBUG, "%s", msg);
#endif
    return;
  }

  if(hug_syslog_sock < 0) return;

  const time_t rawtime = time(NULL);
  struct tm time;
  hug_localtime(&rawtime, &time);
  char buftime[SYSLOG_TIMELEN];
  strftime(buftime, SYSLOG_TIMELEN, "%b %e %H:%M:%S ", &time);

  // Currently everything is mapped to local facility 4 as debug
  char bufpri[SYSLOG_PRILEN];
  strncpy(bufpri, "<167>", SYSLOG_PRILEN);

  char buftag[SYSLOG_TAGLEN];
  snprintf(buftag, SYSLOG_TAGLEN, "%s[%d]: ", execname, pid);

  char buf[SYSLOG_MSGLEN];
  memset(buf, 0, sizeof(buf));
  strncat(buf, bufpri, SYSLOG_PRILEN);
  strncat(buf, buftime, SYSLOG_TIMELEN);
  strncat(buf, buftag, SYSLOG_TAGLEN);
  strncat(buf, msg, SYSLOG_CONTENTLEN);
  strcat(buf, "\n");


//  printf("Sending to syslog: %s\n", buf);
  int buf_len = strlen(buf);
  if((sendto(hug_syslog_sock, buf, buf_len, 0, (struct sockaddr *) &hug_syslog_sockaddr, 
             sizeof(hug_syslog_sockaddr))) != buf_len) {
    fprintf(stderr, "Failed to send message successfully: %s\n", strerror(errno));
  }
}

void hug_syslog_close() {
  //  printf("Closing syslog module\n");
  if(hug_syslog_sock < 0) return;

#ifdef WIN32
  closesocket(hug_syslog_sock);
  WSACleanup();
#else
  close(hug_syslog_sock);
#endif
}

#endif/*DISABLE_HUGIN*/

#ifdef TEST_HUGIN_SYSLOG
//Additional dependency files
//deps:
//Required cflags (autoconf vars may be used)
//cflags:
//Required link options (autoconf vars may be used)
//libs:
#include "test.h"

TEST_BEGIN;

// TODO: Put some testcode here (see test.h for usable macros).
TEST_TRUE(false, "No tests yet!");

TEST_END;

#endif/*TEST_HUGIN_SYSLOG*/
