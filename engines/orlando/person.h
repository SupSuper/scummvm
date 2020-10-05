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

#ifndef ORLANDO_PERSON_H
#define ORLANDO_PERSON_H

#include "common/str.h"
#include "common/array.h"
#include "common/rect.h"
#include "orlando/vector.h"
#include "orlando/window.h"

namespace Graphics {
	struct Surface;
}

namespace Orlando {

class TextParser;
class Scene;
class Insertion;

enum FacingDirection {
	kDirectionS = 0,
	kDirectionSW,
	kDirectionW,
	kDirectionNW,
	kDirectionN,
	kDirectionNE,
	kDirectionE,
	kDirectionSE,
	kDirectionNone = 100
};

struct PFrame {
	Graphics::Surface *surface;
	int16 offsetX, offsetXFlip, offsetY;
};

/**
 * Represents a walkable character on a scene.
 */
class Person {
protected:
	static const int kDirections = 8;

	Common::String _id;
	Common::Array<PFrame> _framesWalk[kDirections];
	Vector2 _pos, _walk;
	Common::Point _dest;
	Window *_window;
	Insertion *_ins;
	bool _flipped;
	FacingDirection _dir;
	uint32 _time, _delay;
	int _walkSpeed, _perspYMin, _perspYMax, _curFrame, _dirWalk;
	float _scalePersp, _scaleDraw;

	void calcDrawScale();
	void setDirection(int dir);
public:
	Person(const Common::String &id);
	virtual ~Person();
	Insertion *getInsertion() { return _ins; }
	void setInsertion(Insertion *ins) { _ins = ins; }
	Vector2 getPosition() const { return _pos; }
	virtual void setPosition(const Vector2 &pos) { _pos = pos; }
	void setPerspectiveYMin(int perspective) { _perspYMin = perspective; }
	Window *getWindow() { return _window; }
	void setWindow(Window *window) {
		delete _window;
		_window = window;
	}
	bool isWalking() const { return _walk != Vector2(0, 0); }
	uint32 getDelay() const { return _delay; }
	void setDirWalk(bool invert) { _dirWalk = (invert) ? -1 : 1; }

	bool load(TextParser &parser, Scene *scene);
	void setData(uint32 delay, float scale, int perspective, int walk);
	void update(uint32 time);
	void draw(const PFrame &frame, bool flipped);
	virtual void walkTo(Common::Point dest, uint32 time, int dir = kDirectionNone);
	virtual void stay(int dir = kDirectionNone);
};

} // End of namespace Orlando

#endif
