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

#ifndef ORLANDO_GRAPHICS_H
#define ORLANDO_GRAPHICS_H

#include "graphics/pixelformat.h"

namespace Common {
	struct Point;
	class ReadableSeekStream;
}

namespace Graphics {
	struct Surface;
	class ManagedSurface;
}

namespace Orlando {

class OrlandoEngine;

/**
 * Manages all graphics operations for the Orlando engine.
 * The game runs at 640x480x16 using RGB565 pixels. 0 is transparent.
 * All graphics have the same format, no conversions necessary.
 * Drawing is performed on a buffer surface which is later blit to the screen.
 */
class GraphicsManager {
	friend class OrlandoEngine;

	static const int kScreenWidth = 640;
	static const int kScreenHeight = 480;
	static const Graphics::PixelFormat kScreenFormat;

	OrlandoEngine *_vm;
	Graphics::ManagedSurface *_screenBuffer;

public:
	GraphicsManager(OrlandoEngine *vm);
	~GraphicsManager();
	/**
	 * Initializes the system screen and buffer surface with the required resolution.
	 * @return False if the pixel format isn't supported.
	 */
	bool setupScreen();
	/**
	 * Refreshes the screen contents with our buffer surface.
	 */
	void updateScreen();
	/**
	* Draws a surface to the screen.
	* @param surface Source surface.
	* @param pos Position on screen to draw to.
	*/
	void drawSurface(const Graphics::Surface &surface, const Common::Point &pos = Common::Point(0, 0));
	/**
	 * Draws a transparent sprite to the screen.
	 * @param surface Source surface.
	 * @param pos Position on screen to draw to.
	 */
	void drawSprite(const Graphics::Surface &surface, const Common::Point &pos = Common::Point(0, 0));
	/**
	 * Loads a raw bitmap (normally *.BM) into a surface. They are uncompressed
	 * images composed of:
	 * @li int16: Width in pixels.
	 * @li int16: Height in pixels.
	 * @li int16[]: Row-first colors of each pixel.
	 * @param stream Stream to load from. Deleted after loading.
	 * @return New image surface. Must be free()d manually.
	 */	 
	Graphics::Surface *loadRawBitmap(Common::SeekableReadStream *stream);
	/**
	* Loads a palette bitmap (normally *.PBM) into a surface. They are RLE-compressed
	* images composed of:
	* @li int16: Width in pixels.
	* @li int16: Height in pixels.
	* @li int16[]: 128-color palette.
	* @li byte[]: Row-first indices of each pixel. The first 7 bits are the index and
	* the last bit is a RLE flag (if 1, repeat index by the value of the next byte).
	* @param stream Stream to load from. Deleted after loading.
	* @return New image surface. Must be free()d manually.
	*/
	Graphics::Surface *loadPaletteBitmap(Common::SeekableReadStream *stream);
};

} // End of namespace Orlando

#endif
