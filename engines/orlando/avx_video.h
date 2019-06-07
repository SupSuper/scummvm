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
#include "graphics/pixelformat.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {
	struct Surface;
}

namespace Orlando {

class OrlandoEngine;
class FlxAnimation;
class GraphicsManager;
class MemoryAudioStream;

/**
 * Represents a DLG subtitle file composed of a header and subtitle frames.
 * Known header format (24 bytes):
 * @li int16: Width in pixels.
 * @li int16: Height in pixels.
 * @li int16: X position in pixels.
 * @li int16: Y position in pixels.
 * @li int16: Start frame.
 * @li int16: End frame.
 * @li int16: FLX index.
 * @li int24: 24-bit text color.
 */
class AvxSubtitles {
	Common::SeekableReadStream *_stream;
	Graphics::PixelFormat _format;
	Common::Point _pos;
	int _frameStart, _frameEnd, _flx;
	Graphics::Surface *_surface;
public:
	AvxSubtitles(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format);
	~AvxSubtitles();
	bool nextFrame();
	void draw(GraphicsManager *graphics, int frame, int flx);
};

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
	AvxSubtitles *_subtitles;

public:
	AvxVideo(OrlandoEngine *vm, const Common::String &id);
	virtual ~AvxVideo();

	bool initialize() override;
	bool run() override;
};

} // End of namespace Orlando

#endif
