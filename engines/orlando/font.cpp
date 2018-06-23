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

#include "common/scummsys.h"
#include "common/stream.h"
#include "graphics/surface.h"
#include "orlando/font.h"

namespace Orlando {

struct FontChar {
	int offset;
	int width;
	int height;
	byte *data;
};

Font::Font(Common::SeekableReadStream *stream) : _chars(nullptr), _width(0), _height(0) {
	_chars = new FontChar[kNumChars];
	// Read character metrics
	for (int i = 0; i < kNumChars; i++) {
		_chars[i].offset = stream->readSint16LE();
		_chars[i].width = stream->readByte();
		_chars[i].height = stream->readByte();
		_chars[i].data = new byte[_chars[i].width * _chars[i].height];

		_width = MAX(_width, _chars[i].width);
		_height = MAX(_height, _chars[i].height);
	}
	// Read pixel data
	for (int i = 0; i < kNumChars; i++) {
		stream->seek(_chars[i].offset);
		stream->read(_chars[i].data, _chars[i].width * _chars[i].height);
	}
	delete stream;
}

Font::~Font() {
	for (int i = 0; i < kNumChars; i++) {
		delete _chars[i].data;
	}
	delete[] _chars;
}

int Font::getCharWidth(uint32 chr) const
{
	if (chr >= kNumChars)
		return 0;
	return _chars[chr].width;
}

void Font::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const
{
	if (chr >= kNumChars)
		return;

	uint16 fill = color; // high 16 bits
	uint16 border = color >> 16; // low 16 bits

	byte *data = _chars[chr].data;
	for (int yy = 0; yy < _chars[chr].height; yy++) {
		for (int xx = 0; xx < _chars[chr].width; xx++) {
			if (*data > 0 && x + xx >= 0 && x + xx < dst->w && y + yy >= 0 && y + yy < dst->h) {
				uint16 *pixel = (uint16*)dst->getBasePtr(x + xx, y + yy);
				if (*data == 1)
					*pixel = border;
				else if (*data == 2)
					*pixel = fill;
			}
			data++;
		}
	}
}

} // End of namespace Orlando
