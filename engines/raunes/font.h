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

#ifndef RAUNES_FONT_H
#define RAUNES_FONT_H

#include "common/array.h"
#include "common/str.h"

namespace Common {
class SeekableReadStream;
}
namespace Graphics {
struct Surface;
}

namespace Raunes {

class SnagFont {
	static const int kChars = 256;

	int _width[kChars];
	int _position[kChars];
	Common::Array<uint8> _pixels;
	uint8 _foreColor, _backColor, _shadowColor, _underlineColor;
	bool _shadow, _italic;
	int _underline, _height;

	void drawPixel(Graphics::Surface *dst, int x, int y, uint8 p) const;
	void drawChar(Graphics::Surface *dst, int x, int y, uint8 chr, bool shadow = false) const;

public:
	SnagFont();
	bool open(Common::SeekableReadStream *stream);
	int write(Graphics::Surface *dst, int x, int y, const Common::String &str);
	int writeCenter(Graphics::Surface *dst, int x, int y, const Common::String &str);
};

} // End of namespace Raunes

#endif
