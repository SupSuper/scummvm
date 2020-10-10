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

#ifndef ORLANDO_ELEMENT_H
#define ORLANDO_ELEMENT_H

#include "common/str.h"
#include "common/rect.h"
#include "orlando/polygon.h"
#include "orlando/window.h"

namespace Graphics {
	struct Surface;
}

namespace Orlando {

class Animation;
class TextParser;
class Scene;

/**
 * Represents a graphical element on a scene.
 */
class Element {
	friend class Debugger;

	Common::String _id;
	int _bpp;
	Common::Point _pos;
	Window *_window;
	Quad _walkRegion;
	Graphics::Surface *_surface;
	Animation *_anim;

public:
	Element(const Common::String &id);
	~Element();
	Common::String getId() const { return _id; }
	Animation *getAnimation() { return _anim; }
	void setAnimation(Animation *anim) {
		_anim = anim;
		draw();
	}
	Common::Point getPosition() const { return _pos; }
	void setPosition(const Common::Point &pos) {
		_pos = pos;
		draw();
	}
	Window *getWindow() { return _window; }
	void setWindow(Window *window) {
		delete _window;
		_window = window;
	}

	bool load(TextParser &parser, Scene *scene);
	Graphics::Surface *loadSurface(const Common::String &name, Scene *scene);
	void update(uint32 time) const;
	void draw() const;
	bool contains(const Common::Point &pos) const;
	bool isOver(const Common::Point &pos) const;
	bool isUnder(const Common::Point &pos) const;
	Common::Rect getDrawRegion() const;
	Quad getWalkRegion() const;
};

} // End of namespace Orlando

#endif
