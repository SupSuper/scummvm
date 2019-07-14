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

namespace Audio {
	class SeekableAudioStream;
}

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
	Audio::SoundHandle _music;

	/**
	 * Removes the header from a stream. Shortcut for substream since most decoders expect an audio stream to start with data.
	 * @param stream Source stream.
	 * @param headerSize Number of bytes to skip.
	 * @return SubStream starting after the header.
	 */
	Common::SeekableReadStream *makeHeaderless(Common::SeekableReadStream *stream, int headerSize) const;
	/**
	 * Creates a raw PCM stream based on the type of audio passed:
	 * @li Music (.mus) - 8bit 16khz Stereo
	 * @li SFX Standard (.snd) - 8bit 16khz Mono
	 * @li SFX High (.s16) - 16bit 22khz Mono
	 * First 32-bit is the size of the data.
	 * @param stream Source file stream.
	 * @param type Type of audio (Music/SFX/Speech).
	 * @return Audio stream.
	 */
	Audio::SeekableAudioStream *loadRawAudio(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type) const;
	/**
	 * Creates an encoded audio stream based on the type (32-bit integer) defined by the header:
	 * @li 50 - ADPCM 22/44khz Mono (SFX - .psd)
	 * @li 51 - ADPCM 22/44khz Stereo (Music - .pms)
	 * @li 52 - GSM 16khz (Speech Standard - .snd)
	 * @li 53 - GSM 22khz (Speech High - .s22)
	 * @param stream Source file stream.
	 * @param type Type of audio (Music/SFX/Speech).
	 * @return Audio stream.
	 */
	Audio::SeekableAudioStream *loadHeaderAudio(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type) const;
public:
	SoundManager(OrlandoEngine *vm);
	/**
	 * Plays a sound file. The format is automatically determined.
	 * The DOS version uses raw PCM while the Windows version uses encoded audio.
	 * @param stream Stream to load data from. Freed after usage.
	 * @param type Mixer channel.
	 * @param loop Looping track.
	 * @return Handle to control the sound.
	 */
	Audio::SoundHandle playFile(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type, bool loop = false);
	/**
	 * Plays a music track. The location is determined based on the game version.
	 * @param filename Source filename.
	 */
	void playMusic(const Common::String &filename);
	/**
	 * Gets the filename of a sound effect based on the game version.
	 * @param filename Source filename.
	 */
	Common::String getSfx(const Common::String &filename);
	/**
	 * Gets the filename of a speech sample based on the game version.
	 * @param filename Source filename.
	 */
	Common::String getSpeech(const Common::String &filename);
};

} // End of namespace Orlando

#endif
