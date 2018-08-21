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

#ifndef ORLANDO_FLX_ANIM_H
#define ORLANDO_FLX_ANIM_H

#include "graphics/pixelformat.h"

namespace Common {
	class SeekableReadStream;
	class ReadStream;
}

namespace Graphics {
	struct Surface;
}

namespace Orlando {

/**
 * Represents a FLX animation file composed of a header and typed chunks containing frame data.
 * Known header format:
 * @li char[]: "FLX\0" code.
 * @li int16: Number of frames in animation (unused).
 * @li int16: Width in pixels.
 * @li int16: Height in pixels.
 */
class FlxAnimation {
	const int kHeaderSize = 20;
	Common::SeekableReadStream *_stream;
	Graphics::Surface *_surface;
	uint16 _palette[256];

	/**
	 * Decodes the given frame data to a surface:
	 * @li uint16: Pixel to start at.
	 * Followed by chunks of pixels:
	 * @li uint8: Amount of pixels to skip.
	 * @li uint8: Amount of pixels to write.
	 * @li uint8[]: Palette indices of pixels.
	 * @param frame Stream to frame data.
	 */
	void decodeFrame(Common::ReadStream *frame);
public:
	/**
	 * Loads a FLX animation from a stream. If an error occurs, the animation will be empty.
	 * @param stream Pointer to file stream. The animation takes ownership of the stream.
	 */
	FlxAnimation(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format);
	~FlxAnimation();
	/**
	 * Moves to the next frame chunk in the animation sequence.
	 * Each chunk contains a header defining the type and size. Chunk types:
	 * @li 0 - Empty frame.
	 * @li 1 - Frame.
	 * @li 2 - RLE frame.
	 * @li 3 - Unused.
	 * @li 4 - Palette, 16-bit RGB565 values.
	 */
	void nextFrame();
	/**
	 * Returns a pointer to the last decoded frame contents.
	 */
	const Graphics::Surface *getSurface() const { return _surface; }
};

} // End of namespace Orlando

#endif
