/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TRECISION_SOUND_H
#define TRECISION_SOUND_H

#include "trecision/defines.h"
#include "trecision/fastfile.h"
#include "common/file.h"
#include "audio/mixer.h"
#include "audio/audiostream.h"

namespace Trecision {

#define SOUND_OFF 0
#define SOUND_ON 1

#define MAXSOUNDS 4
#define SAMPLEVOICES 6
#define NUMSAMPLES   145            // Maximum number of samples in the game

#define VOLUME(a)	( (a * 255) / 127 )
#define TIME(a)		( (a * 3) / 50 )

struct SSound {
	Common::String _name;
	uint8 _volume;
	uint8 _flag;
	int8  _panning;
};

enum SoundType {
	kSoundTypeMusic = 0,
	kSoundTypeSpeech = 1,
	kSoundTypeSfx = 2,
	kSoundTypeStep = 3
};

struct ActiveSound {
	int soundId;
	Audio::SoundHandle soundHandle;
};

class TrecisionEngine;

class SoundManager {
public:
	SoundManager(TrecisionEngine *vm);
	~SoundManager();

private:
	TrecisionEngine *_vm;
	FastFile _speechFile; // nlspeech.cd0

	ActiveSound _sounds[MAXSOUNDS];
	SSound _gSample[NUMSAMPLES];
	
	Audio::SeekableAudioStream *_stepLeftStream;
	Audio::SeekableAudioStream *_stepRightStream;

public:
	void play(int soundId);
	void stop(int soundId);
	void stopSoundType(SoundType type);
	void stopAll();
	void stopAllExceptMusic();
	void soundStep(int midx, int midz, int act, int frame);
	int32 talkStart(const Common::String &name);
	void loadRoomSounds();

	void loadSamples(Common::File *file);
};


} // End of namespace Trecision

#endif
