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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ORLANDO_SOUND_H
#define ORLANDO_SOUND_H

#include "audio/mixer.h"

namespace Common {
	class SeekableReadStream;
}

namespace Orlando {

class OrlandoEngine;

/**
  * Handles decoding and playback of all game audio.
  */
class SoundManager {
	OrlandoEngine *_vm;
	Audio::SoundHandle *_handle;

public:
	SoundManager(OrlandoEngine *vm);
	~SoundManager();
	/**
	 * Plays a sound file. The format is automatically determined based on the header.
	 * @param stream Stream to load data from. Freed after usage.
	 * @param type Channel to use for playback.
	 */
	void playFile(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type);
};

} // End of namespace Orlando

#endif
