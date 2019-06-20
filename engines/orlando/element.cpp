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
#include "common/file.h"
#include "graphics/surface.h"
#include "orlando/element.h"
#include "orlando/text_parser.h"
#include "orlando/graphics.h"
#include "orlando/scene.h"
#include "orlando/animation.h"

namespace Orlando {

Element::Element(const Common::String &id) : _id(id), _bpp(16), _surface(nullptr), _anim(nullptr) {
}

Element::~Element() {
	delete _anim;
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
}

bool Element::load(TextParser &parser, Scene *scene) {
	_bpp = parser.readInt();
	_pos.x = parser.readInt();
	_pos.y = parser.readInt();
	parser.readFloat();
	parser.readFloat();
	for (int i = 0; i < _region.kPoints; i++) {
		_region.p[i].x = _pos.x + parser.readInt();
		_region.p[i].y = _pos.y + parser.readInt();
	}

	if (!_id.hasPrefix("FLX")) {
		_surface = loadSurface(_id, scene);
		if (_surface == nullptr)
			return false;
	}
	return true;
}

Graphics::Surface *Element::loadSurface(const Common::String &name, Scene *scene) {
	// TODO: Figure out -8, seems to make the sprite half-transparent
	return scene->loadSurface(name, ABS(_bpp));
}

void Element::draw(GraphicsManager *graphics, uint32 time) const {
	if (_anim != nullptr) {
		AFrame frame = _anim->nextFrame(time);
		Graphics::Surface *surface = frame.surface;
		if (surface != nullptr) {
			graphics->drawTransparent(*surface, _pos + frame.offset);
		}
	} else if (_surface != nullptr) {
		graphics->drawTransparent(*_surface, _pos);
	}
}

} // End of namespace Orlando
