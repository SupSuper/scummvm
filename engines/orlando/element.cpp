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
#include "orlando/window.h"

namespace Orlando {

Element::Element(const Common::String &id) : _id(id), _bpp(16), _window(nullptr), _surface(nullptr), _anim(nullptr) {
}

Element::~Element() {
	delete _anim;
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
	delete _window;
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
	draw();
	return true;
}

Graphics::Surface *Element::loadSurface(const Common::String &name, Scene *scene) {
	// TODO: Figure out -8, seems to make the sprite half-transparent
	return scene->loadSurface(name, ABS(_bpp));
}

void Element::update(uint32 time) const {
	if (_anim != nullptr) {
		_anim->nextFrame(time, this);
	}
}

void Element::draw() const {
	if (_anim != nullptr) {
		const AFrame *frame = _anim->getFrame();
		Graphics::Surface *surface = frame->surface;
		_window->drawFrom(surface, _pos + frame->offset);
	} else if (_surface != nullptr) {
		_window->drawFrom(_surface, _pos);
	}
}

bool Element::contains(const Common::Point &p) const {
	if (!_window->isVisible())
		return false;
	if (_surface != nullptr) {
		Common::Rect rect(_surface->w, _surface->h);
		rect.moveTo(_pos);
		return rect.contains(p);
	}
	return false;
}

bool Element::isOver(const Common::Point &pos) const {
	int16 baseY = _region.p[0].y;
	for (uint32 i = 1; i < _region.kPoints; i++) {
		baseY = MAX(baseY, _region.p[i].y);
	}
	return baseY > pos.y;
}

bool Element::isUnder(const Common::Point &pos) const {
	int16 baseY = _region.p[0].y;
	for (uint32 i = 1; i < _region.kPoints; i++) {
		baseY = MAX(baseY, _region.p[i].y);
	}
	return baseY < pos.y;
}

} // End of namespace Orlando
