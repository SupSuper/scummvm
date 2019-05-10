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

#ifndef ORLANDO_FONT_H
#define ORLANDO_FONT_H

#include "graphics/font.h"

namespace Common {
	class SeekableReadStream;
}

namespace Graphics {
	struct Surface;
}

namespace Orlando {

struct FontChar;

/**
 * Represents a variable-width bitmap font with border and fill.
 * Assumes 16-bit colors.
 */
class Font : public Graphics::Font {
	static const int kNumChars = 256;
	FontChar *_chars;
	int _width, _height;

public:
	/**
	 * Loads the font from a .FNT file. If an error occurs, the font will be empty.
	 * Header format (256 entries for each ASCII value):
	 * @li int16: Starting position of character pixels in file.
	 * @li byte: Character width.
	 * @li byte: Character height.
	 * Pixel data (bytes):
	 * @li 0: transparent
	 * @li 1: border
	 * @li 2: fill
	 * @param stream Stream to load from. Deleted after loading.
	 */
	Font(Common::SeekableReadStream *stream);
	~Font();
	int getFontHeight() const override { return _height; }
	int getMaxCharWidth() const override { return _width; }
	int getCharWidth(uint32 chr) const override;
	int getKerningOffset(uint32 left, uint32 right) const override { return 1; }
	/**
	 * Draw a character at a specific point on a surface.
	 *
	 * Because of API limitations, the color includes both a
	 * 16-bit border color and a 16-bit fill color.
	 *
	 * @param dst The surface to drawn on.
	 * @param chr The character to draw.
	 * @param x   The x coordinate where to draw the character.
	 * @param y   The y coordinate where to draw the character.
	 * @param color The border color (high bits) and fill color (low bits) of the character.
	 */
	void drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const override;
};

} // End of namespace Orlando

#endif
