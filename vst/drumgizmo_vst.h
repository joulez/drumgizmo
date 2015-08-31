/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            drumgizmo_vst.h
 *
 *  Tue Sep 20 08:22:48 CEST 2011
 *  Copyright 2011 Bent Bisballe Nyeng
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
#ifndef __DRUMGIZMO_DRUMGIZMO_VST_H__
#define __DRUMGIZMO_DRUMGIZMO_VST_H__

#include <public.sdk/source/vst2.x/audioeffectx.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>

#include <drumgizmo.h>
#include <plugingui.h>

#include "input_vst.h"
#include "output_vst.h"

class DGEditor;

class DrumGizmoVst : public AudioEffectX
{
public:
	DrumGizmoVst(audioMasterCallback audioMaster);
	~DrumGizmoVst();

	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);
	VstInt32 processEvents(VstEvents* events);

	void setProgram(VstInt32 program);
	void setProgramName(char* name);
	void getProgramName(char* name);
	bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text);

	void setParameter(VstInt32 index, float value);
	float getParameter(VstInt32 index);
	void getParameterLabel(VstInt32 index, char* label);
	void getParameterDisplay(VstInt32 index, char* text);
	void getParameterName(VstInt32 index, char* text);
	
	void setSampleRate(float sampleRate);
	void setBlockSize(VstInt32 blockSize);
	
	bool getOutputProperties(VstInt32 index, VstPinProperties* properties);
		
	bool getEffectName(char* name);
	bool getVendorString(char* text);
	bool getProductString(char* text);
	VstInt32 getVendorVersion();
	VstInt32 canDo(char* text);

	VstInt32 getNumMidiInputChannels();
	VstInt32 getNumMidiOutputChannels();

	VstInt32 getMidiProgramName(VstInt32 channel,
                              MidiProgramName* midiProgramName);
	VstInt32 getCurrentMidiProgram(VstInt32 channel,
                                 MidiProgramName* currentProgram);
	VstInt32 getMidiProgramCategory(VstInt32 channel,
                                  MidiProgramCategory* category);
	bool hasMidiProgramsChanged(VstInt32 channel);
	bool getMidiKeyName(VstInt32 channel, MidiKeyName* keyName);

  VstInt32 getChunk(void **data, bool isPreset);
  VstInt32 setChunk(void *data, VstInt32 byteSize, bool isPreset);

  DrumGizmo *drumgizmo;
  InputVST *input;
  OutputVST *output;

private:
	void initProcess();
  //	void noteOn(VstInt32 note, VstInt32 velocity, VstInt32 delta);
  //	void noteOff();
	void fillProgram(VstInt32 channel, VstInt32 prg, MidiProgramName* mpn);

  size_t pos;
  sample_t *buffer;
  size_t buffer_size;

  DGEditor *editor;
};

class DGEditor : public AEffEditor {
public:
  DGEditor(AudioEffect* effect);

  bool open(void* ptr);
  void close();
  bool isOpen();
	void idle();

private:
  DrumGizmoVst* dgeff;
  PluginGUI *plugingui;
  DrumGizmo *drumgizmo;
};

#endif/*__DRUMGIZMO_DRUMGIZMO_VST_H__*/
