/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            engine.cc
 *
 *  Fri Nov 29 18:09:02 CET 2013
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
#include <cppunit/extensions/HelperMacros.h>

#include <drumgizmo.h>
#include <unistd.h>

class test_engine : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(test_engine);
	CPPUNIT_TEST(loading);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

  void loading() {
    AudioOutputEngine *oe = NULL;
    AudioInputEngine *ie = NULL;
    DrumGizmo dg(oe, ie);

    // Switch kits emmidiately with giving the loader time to work:
    for(int i = 0; i < 100; i++) {
      dg.loadkit("kit/kit1.xml");
      dg.loadkit("kit/kit2.xml");
    }

    // Switch kits with delay with giving the loader time to work a little:
    for(int i = 0; i < 100; i++) {
      dg.loadkit("kit/kit1.xml");
      usleep(100);
      dg.loadkit("kit/kit2.xml");
      usleep(100);
    }

    // Switch kits with more delay with giving the loader time to finish
    for(int i = 0; i < 100; i++) {
      dg.loadkit("kit/kit1.xml");
      usleep(10000);
      dg.loadkit("kit/kit2.xml");
      usleep(10000);
    }
	}
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(test_engine);

