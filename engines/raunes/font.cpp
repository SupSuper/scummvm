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
#include "common/textconsole.h"
#include "common/util.h"
#include "graphics/surface.h"

#include "raunes/font.h"
#include "raunes/graphics.h"

namespace Raunes {

using Gfx = GraphicsManager;

SnagFont::SnagFont() : _foreColor(15), _backColor(255), _shadowColor(8), _underlineColor(1),
	_shadow(false), _italic(false), _underline(0), _height(0), _width{0}, _position{0} 
{}

uint16 read32(Common::ReadStream *s) {
	return ((s->readByte() << 24) +
			(s->readByte() << 16) +
			(s->readByte() << 8) +
			(s->readByte()));
}
uint16 read16(Common::ReadStream *s) {
	return ((s->readByte() << 8) +
			(s->readByte()));
}

bool SnagFont::open(Common::SeekableReadStream *stream) {
	if (stream == nullptr) {
		return false;
	}

	// Validate header
	const uint32 kHeader = MKTAG('S', 'F', '1', '\0');
	uint32 tag = stream->readUint32BE();
	if (tag != kHeader) {
		warning("SnagFont: No header found");
		return false;
	}
	stream->seek(-1, SEEK_CUR);

	// Read char table
	_height = read16(stream);
	for (int i = 0; i < kChars; i++) {
		_position[i] = read32(stream);
		_width[i] = read16(stream);
	}

	// Read font data
	/*for (int i = 0; i < kChars; i++) {
		for (int j = 0; j < _width[i] * _height; j++) {
			_pixels.push_back(stream->readByte());
		}
	}*/
	while (!stream->eos()) {
		uint8 p = stream->readByte();
		if (!stream->eos()) {
			_pixels.push_back(p);
		}
	}

	return true;
}

void SnagFont::drawPixel(Graphics::Surface *dst, int x, int y, uint8 p) const {
	if (x < 0 || x >= dst->w || y < 0 || y >= dst->h)
		return;

	if (p == Gfx::kTransparent) {
		if (_backColor != Gfx::kTransparent) {
			dst->setPixel(x, y, _backColor);
		}
	} else if (p < 240) {
		dst->setPixel(x, y, p);
	} else {
		dst->setPixel(x, y, _foreColor + p - 247);
	}
}
void SnagFont::drawChar(Graphics::Surface *dst, int x, int y, uint8 chr, bool shadow) const {
	if (dst == nullptr)
		return;
	if (_shadow && !shadow) {
		drawChar(dst, x + 2, y + 2, chr, true);
	}
	for (int yi = 0; yi < _height; yi++) {
		for (int xi = 0; xi < _width[chr]; xi++) {
			uint8 p = _pixels[_position[chr] + yi * _width[chr] + xi];
			if (shadow && p != Gfx::kTransparent) {
				p = _shadowColor;
			}
			int xp = x + xi;
			if (_italic) {
				xp += _height - yi;
			}
			int yp = y + yi;
			drawPixel(dst, xp, yp, p);
		}
	}
	for (int i = 1; i <= _underline; i++) {
		for (int xi = 0; xi < _width[chr]; xi++) {
			int xp = x + xi;
			int yp = y + _height + 2 * i;
			drawPixel(dst, xp, yp, _underlineColor);
		}
	}
}

int SnagFont::write(Graphics::Surface *dst, int x, int y, const Common::String &str) {
	int xi = x;
	Common::String num;
	for (uint i = 0; i < str.size(); i++) {
		if (str[i] == '#') {
			i++;
			switch (str[i]) {
			case '#':
				drawChar(dst, xi, y, str[i]);
				xi += _width[(uint8)str[i]];
				break;
			case 'f':
				num = str.substr(i + 1, 3);
				_foreColor = atoi(num.c_str());
				i += 3;
				break;
			case 'b':
				num = str.substr(i + 1, 3);
				_backColor = atoi(num.c_str());
				i += 3;
				break;
			case 'u':
				num = str.substr(i + 1, 3);
				_underlineColor = atoi(num.c_str());
				i += 3;
				break;
			case 's':
				num = str.substr(i + 1, 3);
				_shadowColor = atoi(num.c_str());
				i += 3;
				break;
			case 'U':
				_underline = (str[i + 1] == '1');
				i++;
				break;
			case 'S':
				_shadow = (str[i + 1] == '1');
				i++;
				break;
			case 'K': // Kursiv
				_italic = (str[i + 1] == '1');
				i++;
				break;
			}
		} else {
			drawChar(dst, xi, y, str[i]);
			xi += _width[(uint8)str[i]];
		}
	}
	return xi;
}

int SnagFont::writeCenter(Graphics::Surface *dst, int x, int y, const Common::String &str) {
	int width = write(nullptr, 0, 0, str);
	int xi = x - width / 2;
	if (xi < 0) {
		xi = 0;
	}
	if (xi + width >= Gfx::kScreenW) {
		xi = Gfx::kScreenW - 1 - width;
	}
	int yi = y - _height / 2;
	return write(dst, xi, yi, str);
}

} // End of namespace Raunes
