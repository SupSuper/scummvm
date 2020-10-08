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
#include "common/str.h"
#include "common/file.h"
#include "graphics/surface.h"

#include "orlando/jack.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/graphics.h"

namespace Orlando {

Jack::Jack(OrlandoEngine *vm) : Person("JACK"), _vm(vm) {
}

Jack::~Jack() {
	/*
	for (int i = 0; i < ARRAYSIZE(_framesWalk); i++) {
		if (_framesStay[i].surface != nullptr) {
			_framesStay[i].surface->free();
			delete _framesStay[i].surface;
		}
	}
	*/
}

Common::Array<PFrame> Jack::loadAnimation(const char *id, const int frames) {
	Common::Array<PFrame> anim;

	ResourceManager *resources = _vm->getResourceManager();
	GraphicsManager *graphics = _vm->getGraphicsManager();
	
	bool army = _vm->getVariable("JACKISWOJ") != 0;
	const char *w = (army) ? "W" : "";

	Common::String sfb = Common::String::format("%s%s.SFB", id, w);
	Common::File *offsets = resources->loadResourceFile(sfb);
	if (!offsets)
		return Common::Array<PFrame>();

	for (int i = 1; i <= frames; i++) {
		Common::String pbm = Common::String::format("%s%s%d.PBM", id, w, i);
		PFrame frame;
		frame.surface = graphics->loadPaletteBitmap(resources->loadResourceFile(pbm));
		frame.offsetX = offsets->readSint16LE();
		frame.offsetXFlip = offsets->readSint16LE();
		frame.offsetY = offsets->readSint16LE();
		anim.push_back(frame);
	}

	delete offsets;
	return anim;
}

Graphics::Surface *Jack::loadFrame(const char *id) {
	ResourceManager *resources = _vm->getResourceManager();
	GraphicsManager *graphics = _vm->getGraphicsManager();

	bool army = _vm->getVariable("JACKISWOJ") != 0;
	const char *w = (army) ? "W" : "";

	Common::String pbm = Common::String::format("%s%s.PBM", id, w);
	return graphics->loadPaletteBitmap(resources->loadResourceFile(pbm));
}

bool Jack::initialize() {
	const int kWalkFrames = 12;
	_framesWalk[kDirectionS] = loadAnimation("FRON", kWalkFrames);
	_framesWalk[kDirectionW] = loadAnimation("PROF", kWalkFrames);
	_framesWalk[kDirectionN] = loadAnimation("BACK", kWalkFrames);
	_framesWalk[kDirectionSW] = loadAnimation("SKFR", kWalkFrames);
	_framesWalk[kDirectionNE] = loadAnimation("SKBA", kWalkFrames);

	Graphics::Surface *s;
	_framesStay[kDirectionS] = { s = loadFrame("SSFR2"), 46, s->w - 46, 264 };
	_framesStay[kDirectionW] = { s = loadFrame("SSPR2"), 23, s->w - 23, 262 };
	_framesStay[kDirectionN] = { s = loadFrame("SSBA2"), 50, s->w - 50, 266 };
	_framesStay[kDirectionSW] = { s = loadFrame("SSSP2"), 36, s->w - 36, 264 };
	_framesStay[kDirectionNE] = { s = loadFrame("SSST2"), 46, s->w - 46, 260 };
	return true;
}

void Jack::setPosition(const Vector2 &pos) {
	Person::setPosition(pos);
	_vm->getVariable("JACKX") = pos.x;
	_vm->getVariable("JACKY") = pos.y;
}

void Jack::setPerspective(int yMin, int yMax, float scale) {
	_perspYMin = yMin;
	_perspYMax = yMax;
	_scalePersp = scale;
}

void Jack::walkTo(const Common::Point &dest, uint32 time, int dir) {
	const int kJackDirections[] = { 0, 3, 1, -4, 2, 4, -1, -3 };
	if (dir != kDirectionNone) {
		for (int i = 0; i < kDirections; i++) {
			if (dir == kJackDirections[i]) {
				dir = i;
				break;
			}
		}
	}
	Person::walkTo(dest, time, dir);
}

void Jack::stay(int dir) {
	Person::stay(dir);
	if (dir == kDirectionNone) {
		dir = _dir;
	} else {
		setDirection(dir);
	}
	draw(_framesStay[_dir], _flipped);
}

} // End of namespace Orlando
