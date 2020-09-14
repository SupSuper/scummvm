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

bool Jack::loadWalk(const char *id, FacingDirection dir) {
	ResourceManager *resources = _vm->getResourceManager();
	GraphicsManager *graphics = _vm->getGraphicsManager();
	
	bool army = _vm->getVariable("JACKISWOJ") != 0;
	const char *w = (army) ? "W" : "";


	Common::String sfb = Common::String::format("%s%s.SFB", id, w);
	Common::File *offsets = resources->loadResourceFile(sfb);
	if (!offsets)
		return false;

	const int kFrames = 12;
	for (int i = 1; i <= kFrames; i++) {
		Common::String pbm = Common::String::format("%s%s%d.PBM", id, w, i);
		PFrame frame;
		frame.surface = graphics->loadPaletteBitmap(resources->loadResourceFile(pbm));
		if (!frame.surface)
			return false;
		frame.offsetX = offsets->readSint16LE();
		frame.offsetXFlip = offsets->readSint16LE();
		frame.offsetY = offsets->readSint16LE();
		_frames[dir].push_back(frame);
	}

	delete offsets;
	return true;
}

bool Jack::initialize() {
	if (!loadWalk("FRON", kDirectionS) ||
		!loadWalk("PROF", kDirectionW) ||
		!loadWalk("BACK", kDirectionN) ||
		!loadWalk("SKFR", kDirectionSW) ||
		!loadWalk("SKBA", kDirectionNE))
		return false;
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
	calcDrawScale();
}

} // End of namespace Orlando
