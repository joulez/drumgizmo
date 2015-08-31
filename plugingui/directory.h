/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            directory.h
 *
 *  Tue Apr 23 22:01:07 CEST 2013
 *  Copyright 2013 Jonas Suhr Christensen
 *  jsc@umbraculum.org
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
#ifndef __DRUMGIZMO_DIRECTORY_H__
#define __DRUMGIZMO_DIRECTORY_H__

#include <string>
#include <list>

#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIRECTORY_HIDDEN 1

class Directory {

  public:
    typedef struct drive {
      int number;
      std::string name;
    } drive_t;

    typedef std::list<std::string> EntryList;
    typedef std::list<drive> DriveList;
    
    Directory(std::string path);
    ~Directory();

    std::string seperator();

    size_t count();
    void refresh();
    std::string path();
    bool cdUp();
    bool cd(std::string dir);
    bool isDir();
    void setPath(std::string path);
    bool fileExists(std::string file);

    // Add filter, ie. directories or files only
    EntryList entryList();

    //void setSorting();

    static std::string cwd();
    static std::string root();
    static std::string root(std::string path); 
    static std::string cleanPath(std::string path);
    static Directory::EntryList listFiles(std::string path, unsigned char filter = 0);
    static bool isRoot(std::string path);
    static Directory::DriveList drives();
    static bool isDir(std::string path);
    static bool isHidden(std::string entry);
    static bool exists(std::string path);
    static std::string pathDirectory(std::string filepath);
     
  private:
    std::string _path;
    EntryList _files;
    DriveList _drives;

    typedef std::list<std::string> Path;
    static Path parsePath(std::string path);
    static std::string pathToStr(Path &path);
};

#endif/*__DRUMGIZMO_DIRECTORY_H__*/
