/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            rangemap.h
 *
 *  Wed Sep 22 19:17:49 CEST 2010
 *  Copyright 2010 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_RANGEMAP_H__
#define __DRUMGIZMO_RANGEMAP_H__

#include <vector>
#include <map>

template<typename T1, typename T2> 
class RangeMap {
public:
  void insert(T1 from, T1 to, T2 value);
  std::vector<T2> get(T1 from, T1 to);
  std::vector<T2> get(T1 at);

private:
  std::multimap<std::pair<T1, T1>, T2> values;
};

template<typename T1, typename T2>
void RangeMap<T1, T2>::insert(T1 from, T1 to, T2 value)
{
  if(from < to) values.insert(std::make_pair(std::make_pair(from, to), value));
  else values.insert(std::make_pair(std::make_pair(to, from), value));
}

template<typename T1, typename T2> 
std::vector<T2> RangeMap<T1, T2>::get(T1 from, T1 to)
{
  std::vector<T2> res;

  typename std::multimap<std::pair<T1, T1>, T2>::iterator i = values.begin();
  while(i != values.end()) {
    T1 a = i->first.first;
    T1 b = i->first.second;
    if(
       (from >= a && to <= b) || // inside
       (from <= a && to >= b) || // containing
       (from <= a && to >= a && to <= b) || // overlapping lower
       (from >= a && from <= b && to >= b)  // overlapping upper
       )
      res.push_back(i->second);
    i++;
  }

  return res;
}

template<typename T1, typename T2> 
std::vector<T2> RangeMap<T1, T2>::get(T1 at)
{
  std::vector<T2> res;

  typename std::multimap<std::pair<T1, T1>, T2>::iterator i = values.begin();
  while(i != values.end()) {
    T1 a = i->first.first;
    T1 b = i->first.second;
    if(at >= a && at <= b)
      res.push_back(i->second);
    i++;
  }

  return res;
}


#endif/*__DRUMGIZMO_RANGEMAP_H__*/
