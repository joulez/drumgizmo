/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            resampler.cc
 *
 *  Sun Oct  5 20:16:22 CEST 2014
 *  Copyright 2014 Bent Bisballe Nyeng
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

#include "../src/chresampler.h"
#include <unistd.h>

#define BUFSZ 500

static float round(float a) { return a<0.5?0:1; }

class test_resampler : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(test_resampler);
	CPPUNIT_TEST(resampling);
	CPPUNIT_TEST(resampling_buffer_sizes);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

  void resampling()
  {
    CHResampler r;
    CPPUNIT_ASSERT_EQUAL(1.0, r.ratio());
    
    r.setup(44100, 48000);
    CPPUNIT_ASSERT_EQUAL(44100.0/48000.0, r.ratio());

    float in[BUFSZ];
    for(int i = 0; i < BUFSZ; i++) in[i] = 0;//(float)i/(float)BUFSZ;
    in[100] = 1.0;

    float out[BUFSZ];
    r.setInputSamples(in, sizeof(in) / sizeof(float));
    r.setOutputSamples(out, sizeof(out) / sizeof(float));
    r.process();
    CPPUNIT_ASSERT_EQUAL((size_t)0, r.getInputSampleCount());

    //    CPPUNIT_ASSERT_EQUAL(, r.getOutputSampleCount());

    int outidx = -1;
    int inidx = -1;
    for(int i = 0; i < BUFSZ - (int)r.getOutputSampleCount(); i++) {
      if(in[i] == 1.0) inidx = i;
      if(round(out[i]) == 1.0) outidx = i;
      //printf("in[% 4d]\t= %f\t", i, in[i]);
      //printf("out[% 4d]\t= %f\n", i, out[i]);
    }

    CPPUNIT_ASSERT(inidx != -1);
    CPPUNIT_ASSERT(outidx != -1);

    //printf("inidx: %d - outidx: %d\n", inidx, outidx);
    //CPPUNIT_ASSERT_EQUAL(71, inidx - outidx); // This does not make sense...
	}

  void resampling_buffer_sizes()
  {
    CHResampler r;
    CPPUNIT_ASSERT_EQUAL(1.0, r.ratio());
    
    double infs = 24000;
    double outfs = 48000;
    r.setup(infs, outfs);
    CPPUNIT_ASSERT_EQUAL(infs / outfs, r.ratio());

    float in[BUFSZ];
    float out[(int)(BUFSZ / r.ratio())];

    // Preload resampler
    r.setOutputSamples(out, 1);
    while(r.getOutputSampleCount()) {
      r.setInputSamples(in, 1);
      r.process();
    }

    r.setInputSamples(in, sizeof(in) / sizeof(float));
    r.setOutputSamples(out, sizeof(out) / sizeof(float));
    r.process();
    CPPUNIT_ASSERT_EQUAL((size_t)0, r.getInputSampleCount());
    CPPUNIT_ASSERT_EQUAL((size_t)0, r.getOutputSampleCount());
  }
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(test_resampler);
