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
#include "graphics/cursorman.h"
#include "graphics/surface.h"

#include "orlando/mouse.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/graphics.h"

namespace Orlando {

Mouse::Mouse(OrlandoEngine *vm) : _vm(vm) {
}

Mouse::~Mouse() {
}

bool Mouse::setup() {
	ResourceManager *resources = _vm->getResourceManager();

	if (Common::File *file = resources->loadResourceFile("wsk.bm")) {
		Graphics::Surface *surface = _vm->getGraphicsManager()->loadRawBitmap(file);
		CursorMan.replaceCursor(surface->getPixels(), surface->w, surface->h, 0, 0, 0, false, &surface->format);
		CursorMan.showMouse(true);
		delete surface;
	} else {
		return false;
	}

	return true;
}

} // End of namespace Orlando
