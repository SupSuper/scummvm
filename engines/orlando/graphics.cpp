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

#include "common/rect.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/surface.h"

#include "orlando/graphics.h"
#include "orlando/orlando.h"

namespace Orlando {

const Graphics::PixelFormat GraphicsManager::kScreenFormat(2, 5, 6, 5, 0, 11, 5, 0, 0); // RGB565

GraphicsManager::GraphicsManager(OrlandoEngine *vm) : _vm(vm), _screenBuffer(nullptr) {
}

GraphicsManager::~GraphicsManager() {
	if (_screenBuffer != nullptr) {
		_screenBuffer->free();
	}
}

bool GraphicsManager::setupScreen() {
	initGraphics(kScreenWidth, kScreenHeight, &kScreenFormat);
	if (_vm->_system->getScreenFormat() != kScreenFormat) {
		return false;
	}

	_screenBuffer = new Graphics::Surface();
	_screenBuffer->create(kScreenWidth, kScreenHeight, kScreenFormat);
	return true;
}

void GraphicsManager::updateScreen() {
	OSystem *system = _vm->_system;

	Graphics::Surface *screen = system->lockScreen();
	screen->copyRectToSurface(*_screenBuffer, 0, 0, Common::Rect(0, 0, _screenBuffer->w, _screenBuffer->h));
	system->unlockScreen();

	system->updateScreen();
}

} // End of namespace Orlando
