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
#include "graphics/surface.h"
#include "orlando/sprite.h"
#include "orlando/text_parser.h"
#include "orlando/graphics.h"

namespace Orlando {

Sprite::Sprite(const Common::String &id) : _id(id), _bpp(16), _scaleX(1.0), _scaleY(1.0), _surface(nullptr) {
}

Sprite::~Sprite() {
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
}

void Sprite::load(TextParser &parser) {
	_bpp = parser.readInt();
	_pos.x = parser.readInt();
	_pos.y = parser.readInt();
	_scaleX = parser.readFloat();
	_scaleY = parser.readFloat();
	for (int i = 0; i < _area.kPoints; i++) {
		_area.p[i].x = _pos.x + parser.readInt();
		_area.p[i].y = _pos.y + parser.readInt();
	}
}

void Sprite::loadSurface(Common::SeekableReadStream *stream, GraphicsManager *graphics) {
	switch (_bpp) {
	case 16:
		_surface = graphics->loadRawBitmap(stream);
		break;
	case 8:
	case -8:
		// TODO: Figure out -8
		_surface = graphics->loadPaletteBitmap(stream);
		break;
	default:
		warning("Sprite: Unknown bpp '%d'", _bpp);
		break;
	}
}

} // End of namespace Orlando
