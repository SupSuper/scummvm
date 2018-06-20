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

namespace Graphics {
	class Surface;
}

namespace Orlando {

class OrlandoEngine;

/**
 * Manages all graphics operations for the Orlando engine.
 * The game runs at 640x480x16 using RGB565 pixels.
 * All graphics have the same format, no conversions necessary.
 * Drawing is performed on a buffer surface which is later blit to the screen.
 */
class GraphicsManager {
	friend class OrlandoEngine;

	static const int kScreenWidth = 640;
	static const int kScreenHeight = 480;
	static const Graphics::PixelFormat kScreenFormat;

	OrlandoEngine *_vm;
	Graphics::Surface *_screenBuffer;

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
};

} // End of namespace Orlando

#endif
