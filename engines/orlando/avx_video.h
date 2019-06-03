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

#ifndef ORLANDO_AVX_VIDEO_H
#define ORLANDO_AVX_VIDEO_H

#include "orlando/scene.h"

namespace Common {
	class SeekableReadStream;
}

namespace Orlando {

class OrlandoEngine;
class FlxAnimation;
class MemoryAudioStream;

/**
 * Player for AVX video files, composed of a series of FLX animation
 * and audio file chunks.
 */
class AvxVideo : public Scene {
	static const uint32 kAudioStart = 110272;

	Common::SeekableReadStream *_stream;
	Orlando::MemoryAudioStream *_audio;
	byte *_audioBuffer;
	FlxAnimation *_flx;
	int _flxTotal, _flxCurrent;

public:
	AvxVideo(OrlandoEngine *vm, Common::SeekableReadStream *stream);
	virtual ~AvxVideo();

	bool initialize() override;
	bool run() override;
};

} // End of namespace Orlando

#endif
