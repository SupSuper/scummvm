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
#include "common/math.h"
#include "graphics/surface.h"

#include "orlando/person.h"
#include "orlando/text_parser.h"
#include "orlando/scene.h"
#include "orlando/graphics.h"
#include "orlando/insertion.h"
#include "orlando/util.h"
#include "orlando/window.h"

namespace Orlando {

Person::Person(const Common::String &id) : _id(id), _window(nullptr), _ins(nullptr), _flipped(false), _dir(kDirectionS), _time(0), _delay(0),
	_walkSpeed(0), _perspYMin(0), _perspYMax(0), _curFrame(0), _scalePersp(2.0f), _scaleDraw(1.0f) {
}

Person::~Person() {
	for (int j = 0; j < ARRAYSIZE(_framesWalk); j++) {
		for (Common::Array<PFrame>::const_iterator i = _framesWalk[j].begin(); i != _framesWalk[j].end(); ++i) {
			if (i->surface != nullptr) {
				i->surface->free();
				delete i->surface;
			}
		}
	}
	delete _window;
}

bool Person::load(TextParser &parser, Scene *scene) {
	parser.readInt();
	int frames = parser.readInt();
	parser.readInt();
	parser.readFloat();
	parser.readInt();
	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty() || id.firstChar() == '[') {
			parser.rewind();
			break;
		}
		int direction = toInt(id);
		for (int i = 0; i < frames; i++) {
			PFrame frame;
			frame.surface = scene->loadSurface(parser.readString(), 8);
			if (!frame.surface)
				return false;
			frame.offsetX = parser.readInt();
			frame.offsetXFlip = parser.readInt();
			frame.offsetY = parser.readInt();
			_framesWalk[direction].push_back(frame);
		}
		_dir = (FacingDirection)direction;
	}
	_perspYMin = scene->getPerspectiveYMin();
	_perspYMax = scene->getPerspectiveYMax();
	_scalePersp = scene->getPerspectiveScale();
	return true;
}

void Person::calcDrawScale() {
	if (_scalePersp == 3.0f) {
		// TODO
	} else if (_scalePersp == 2.0f) {
		_scaleDraw = 1.0f;
	} else {
		_scaleDraw = (_pos.y - _perspYMin) * _scalePersp / (_perspYMax - _perspYMin);
	}
	_scaleDraw = CLIP(_scaleDraw, 0.1f, 1.0f);
}

void Person::setData(uint32 delay, float scale, int perspective, int walk) {
	_delay = delay;
	_scalePersp = scale;
	_perspYMax = perspective;
	_walkSpeed = walk;
}

void Person::update(uint32 time) {
	while (isWalking() && time >= _time + _delay) {
		_time += _delay;
		_curFrame = (_curFrame + 1) % _framesWalk[_dir].size();

		setPosition(_pos + _walk * _scaleDraw);
		draw(_framesWalk[_dir][_curFrame], _flipped);
		if (ABS(_pos.x - _dest.x) < 10 && ABS(_pos.y - _dest.y) < 5) {
			stay();
		}
	}
	if (_ins != nullptr) {
		_ins->nextFrame(time, this);
	}
}

void Person::draw(const PFrame &frame, bool flipped) {
	calcDrawScale();

	Vector2 offset(frame.offsetX, frame.offsetY);
	if (flipped)
		offset.x = frame.offsetXFlip;
	Vector2 drawPos = _pos - offset * _scaleDraw;
	_window->drawFrom(frame.surface, (Common::Point)drawPos, flipped, _scaleDraw);
}

void Person::walkTo(Common::Point dest, uint32 time, int dir) {
	const float kScaleY = 0.3f;
	const float kDirAngles[] = { 22.0f, 67.0f, 112.0f, 157.0f, 202.0f, 248.0f, 292.0f, 338.0f };

	_dest = dest;
	_time = time;
	_curFrame = 0;
	_ins = nullptr;

	float angle = atan2((_dest.y - _pos.y) / kScaleY, _dest.x - _pos.x);
	_walk.x = cos(angle) * _walkSpeed;
	_walk.y = sin(angle) * _walkSpeed * kScaleY;

	if (dir == kDirectionNone) {
		// Determine direction based on angle
		float angleDeg = Common::rad2deg(angle);
		if (angleDeg < 0.0f)
			angleDeg += 360.0f;
		for (int i = 0; i < kDirections - 1; i++) {
			if (angleDeg >= kDirAngles[i] && angleDeg <= kDirAngles[i + 1]) {
				dir = (i + 7) % kDirections;
				break;
			}
		}
		if (dir == kDirectionNone) {
			dir = kDirectionE;
		}
	}
	setDirection(dir);
	draw(_framesWalk[_dir][_curFrame], _flipped);
}

void Person::setDirection(int dir) {
	const int kDirAlternatives[][5] = {
		{ 1, 7, 100, 100, 100 },
		{ -7, 0, 2, -6, 100 },
		{ -6, 1, 3, -5, -7 },
		{ -5, 2, 4, -6, 100 },
		{ 3, 5, 100, 100, 100 },
		{ -3, 4, 6, -2, 100 },
		{ -2, 5, 7, -1, -3 },
		{ -1, 0, 6, -2, 100 }
	};
	// Find closest sprite
	if (_framesWalk[ABS(dir)].empty()) {
		for (int i = 0; i < 5; i++) {
			int newDir = kDirAlternatives[ABS(dir)][i];
			if (newDir == kDirectionNone) {
				break;
			}
			if (!_framesWalk[ABS(newDir)].empty()) {
				dir = newDir;
				break;
			}
		}
	}
	_flipped = (dir < 0);
	_dir = (FacingDirection)ABS(dir);
}

void Person::stay(int dir) {
	_walk = Vector2(0, 0);
}

} // End of namespace Orlando
