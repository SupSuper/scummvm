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

Jack::Jack() : Person("JACK") {
}

bool Jack::loadWalk(const char *id, OrlandoEngine *vm, FacingDirection dir) {
	ResourceManager *resources = vm->getResourceManager();
	GraphicsManager *graphics = vm->getGraphicsManager();
	
	bool army = vm->getVariable("JACKISWOJ") != 0;
	const char *w = (army) ? "W" : "";

	const int kFrames = 12;
	for (int i = 1; i <= kFrames; i++) {
		Common::String filename = Common::String::format("%s%s%d.PBM", id, w, i);
		PFrame frame;
		frame.surface = graphics->loadPaletteBitmap(resources->loadResourceFile(filename));
		if (!frame.surface)
			return false;
		frame.offsetX = frame.surface->w / 2;
		frame.offsetXFlip = frame.surface->w / 2;
		frame.offsetY = frame.surface->h;
		_frames[dir].push_back(frame);
	}
	return true;
}

bool Jack::initialize(OrlandoEngine *vm) {
	if (!loadWalk("FRON", vm, kDirectionS) ||
		!loadWalk("PROF", vm, kDirectionE) ||
		!loadWalk("BACK", vm, kDirectionN) ||
		!loadWalk("SKFR", vm, kDirectionSW) ||
		!loadWalk("SKBA", vm, kDirectionNE))
		return false;
	return true;
}

} // End of namespace Orlando
