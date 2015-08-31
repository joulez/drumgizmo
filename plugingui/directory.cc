/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            directory.cc
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
#include "directory.h"

#include <dirent.h>
#include <stdio.h>
#include <string> 
#include <algorithm>
#include <vector>
#include <string.h>

#ifdef WIN32
#include <direct.h>
#include <windows.h>
#endif

#include <hugin.hpp>

#define DRUMKIT_SUFFIX ".xml"

// http://en.wikipedia.org/wiki/Path_(computing)
#ifdef WIN32
  #define SEP "\\"
#else
  #define SEP "/"
#endif

Directory::Directory(std::string path)
{
  setPath(path);
}

Directory::~Directory()
{
}

std::string Directory::seperator()
{
  return SEP;
}

void Directory::setPath(std::string path)
{
  DEBUG(directory, "Setting path to '%s'\n", path.c_str());
  this->_path = cleanPath(path);
  refresh();
}

size_t Directory::count()
{
  return _files.size();
}

void Directory::refresh()
{
  _files = listFiles(_path, DIRECTORY_HIDDEN);
//  _files = listFiles(_path);
}

bool Directory::cd(std::string dir)
{
  //TODO: Should this return true or false?
  if(dir.empty() || dir == ".") return true;

  DEBUG(directory, "Changing to '%s'\n", dir.c_str());
  if(exists(_path + SEP + dir)) {
    std::string path = _path + SEP + dir;
    setPath(path);
    refresh();
    return true;
  } else {
    return false;
  }
}

bool Directory::cdUp()
{
  return this->cd("..");
}

std::string Directory::path()
{
  return cleanPath(_path);
}

Directory::EntryList Directory::entryList()
{
  return _files;
}

#define MAX_FILE_LENGTH 1024
std::string Directory::cwd() {
  char path[MAX_FILE_LENGTH];
  char* c = getcwd(path, MAX_FILE_LENGTH);

  if(c) return c;
  else return "";
}

std::string Directory::cleanPath(std::string path)
{
  DEBUG(directory, "Cleaning path '%s'\n", path.c_str());

  Directory::Path pathlst = parsePath(path);
  return Directory::pathToStr(pathlst);  
}

Directory::EntryList Directory::listFiles(std::string path, unsigned char filter)
{
  DEBUG(directory, "Listing files in '%s'\n", path.c_str());

  Directory::EntryList entries;
  DIR *dir = opendir(path.c_str());
  if(!dir) {
    DEBUG(directory, "Couldn't open directory '%s\n", path.c_str()); 
    return entries;
  }

  std::vector<std::string> directories;
  std::vector<std::string> files;

  struct dirent *entry;
  while((entry = readdir(dir)) != NULL) {
    std::string name = entry->d_name;
    if(name == ".") continue;

    if(Directory::isRoot(path) && name == "..") continue;
    
    unsigned char entryinfo = 0;
    if(isHidden(path + SEP + name)) {
      entryinfo |= DIRECTORY_HIDDEN;
    }

    std::string entrypath = path;
    entrypath += SEP;
    entrypath += entry->d_name;
    if(Directory::isDir(entrypath)) {
      if(!(entryinfo && filter)) {
        if(name == "..") directories.push_back(entry->d_name);
        else directories.push_back(std::string(SEP) + entry->d_name);
      }
    }
    else {
      int drumkit_suffix_length = strlen(DRUMKIT_SUFFIX);
      if((int)name.size() < drumkit_suffix_length) continue;
      if(name.substr(name.length() - drumkit_suffix_length, 
          drumkit_suffix_length) != DRUMKIT_SUFFIX) {
        continue;
      }


//      if(!(entryinfo && filter)) {
        files.push_back(entry->d_name);
//      }
    }
  }


#ifdef WIN32
	DEBUG(directory, "Root is %s\n", Directory::root(path).c_str()); 
	DEBUG(directory, "Current path %s is root? %d", path.c_str(), Directory::isRoot(path)); 
if(Directory::isRoot(path)) entries.push_back(".."); 
#endif

  // sort
  for(int x = 0; x < (int)directories.size(); x++) {
    for(int y = 0; y < (int)directories.size(); y++) {
      if(directories[x] < directories[y]) {
        
        std::string tmp = directories[x];
        directories[x] = directories[y];
        directories[y] = tmp;
      }
    }
  }

  for(int x = 0; x < (int)files.size(); x++) {
    for(int y = 0; y < (int)files.size(); y++) {
      if(files[x] < files[y]) {
        
        std::string tmp = files[x];
        files[x] = files[y];
        files[y] = tmp;
      }
    }
  }


  for(std::vector<std::string>::iterator it = directories.begin(); it != directories.end(); it++) {
    entries.push_back(*it);
  }

  for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
    entries.push_back(*it);
  }


  return entries;
}

bool Directory::isRoot(std::string path)
{
#ifdef WIN32
  std::transform(path.begin(), path.end(), path.begin(), ::tolower);
  std::string root_str = Directory::root(path); 
  std::transform(root_str.begin(), root_str.end(), root_str.begin(), ::tolower);
  // TODO: This is not a correct root calculation, but works with partitions
  if(path.size() == 2) {
    if(path == root_str) return true;
    else return false;
  } else if (path.size() == 3) {
    if(path == root_str + SEP) return true;
    return false;
  } else {
    return false;
  }
#else
  if(path == SEP) return true;
  else return false;
#endif
}

std::string Directory::root()
{
  return root(cwd());
}

std::string Directory::root(std::string path)
{
#ifdef WIN32
  if(path.size() < 2) {
    return "c:"; // just something default when input is bad
  } else {
    return path.substr(0, 2);
  }
#else
  return SEP;
#endif
}

Directory::DriveList Directory::drives()
{
  Directory::DriveList drives;
#ifdef WIN32
  unsigned int d = GetLogicalDrives();
  for(int i = 0; i < 32; i++) {
    if(d & (1 << i)) {
      drive_t drive;
      char name[] = "x:";
      name[0] = i + 'a';

      drive.name = name;
      drive.number = i;
      drives.push_back(drive);
    }
  }
#endif
  return drives;
}

bool Directory::isDir()
{
  return isDir(_path);
}

bool Directory::isDir(std::string path)
{
  DEBUG(directory, "Is '%s' a directory?\n", path.c_str());
  struct stat st;
  if(stat(path.c_str(), &st) == 0) {
    if((st.st_mode & S_IFDIR) != 0) {
      DEBUG(directory, "\t...yes!\n");
      return true;
    }
  }
  DEBUG(directory, "\t...no!\n");
  return false;
}

bool Directory::fileExists(std::string filename)
{
  return !isDir(_path + SEP + filename);
}

bool Directory::exists(std::string path)
{
  struct stat st;
  if(stat(path.c_str(), &st) == 0) {
    return true;
  } else {
    return false;
  }
}

bool Directory::isHidden(std::string path) 
{
  DEBUG(directory, "Is '%s' hidden?\n", path.c_str());
#ifdef WIN32
  // We dont want to filter out '..' pointing to root of a partition
  unsigned pos = path.find_last_of("/\\");
  std::string entry = path.substr(pos+1);
  if(entry == "..") {
    return false;
  }

  DWORD fattribs = GetFileAttributes(path.c_str());
  if(fattribs & FILE_ATTRIBUTE_HIDDEN) {
    DEBUG(directory, "\t...yes!\n");
    return true;
  }
  else if(fattribs & FILE_ATTRIBUTE_SYSTEM) {
    DEBUG(directory, "\t...yes!\n");
    return true;
  }
  else {
    DEBUG(directory, "\t...no!\n");
    return false;
  }
#else
  unsigned pos = path.find_last_of("/\\");
  std::string entry = path.substr(pos+1);
  if(entry.size() > 1 &&
     entry.at(0) == '.' &&
     entry.at(1) != '.') {
    DEBUG(directory, "\t...yes!\n");
    return true;
  }
  else {
    DEBUG(directory, "\t...no!\n");
    return false;
  }
#endif
}

Directory::Path Directory::parsePath(std::string path_str)
{
  //TODO: Handle "." input and propably other special cases

  DEBUG(directory, "Parsing path '%s'", path_str.c_str());
  Directory::Path path;
  
  std::string current_char;
  std::string prev_char;
  std::string dir;
  for(size_t c = 0; c < path_str.size(); c++) {
    current_char = path_str.at(c);

    if(current_char == SEP) {
      if(prev_char == SEP) {
        dir.clear();
        prev_char = current_char;
        continue;
      } else if(prev_char == ".") {
        prev_char = current_char;
        continue;
      }
      
      if(!dir.empty()) path.push_back(dir);
      dir.clear();
      continue;
    } else if(current_char == ".") {
      if(prev_char == ".") {
        dir.clear();
        if(!path.empty()) path.pop_back();
        continue;
      }
    }
    
    dir += current_char;
    prev_char = current_char;
  }

  if(!dir.empty()) path.push_back(dir);

  return path;
}

std::string Directory::pathToStr(Directory::Path& path)
{
  std::string cleaned_path;
  DEBUG(directory, "Number of directories in path is %d\n", (int)path.size());

  for(Directory::Path::iterator it = path.begin();
      it != path.end(); it++) {
    std::string dir = *it;
    DEBUG(directory, "\tDir '%s'\n", dir.c_str());
#ifdef WIN32
    if(it != path.begin()) cleaned_path += SEP;
    cleaned_path += dir;
#else
    cleaned_path += SEP + dir;
#endif
  }

  DEBUG(directory, "Cleaned path '%s'\n", cleaned_path.c_str());

  if(cleaned_path.empty()) { 
    cleaned_path = Directory::root();
#ifdef WIN32
    cleaned_path += SEP;
#endif  
  }

#ifdef WIN32
  if(cleaned_path.size() == 2) cleaned_path += SEP;
#endif

  return cleaned_path; 
}

std::string Directory::pathDirectory(std::string filepath)
{
  if(Directory::isDir(filepath)) return filepath;

  Directory::Path path = parsePath(filepath);
  if(path.size() > 0) path.pop_back();

  return Directory::pathToStr(path);
}
