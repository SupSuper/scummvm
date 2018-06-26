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
#include "common/events.h"
#include "graphics/cursorman.h"
#include "graphics/surface.h"

#include "orlando/mouse.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/graphics.h"

namespace Orlando {

Mouse::Mouse(OrlandoEngine *vm) : _vm(vm), _leftButton(kButtonNormal), _rightButton(kButtonNormal) {
}

Mouse::~Mouse() {
}

bool Mouse::initialize() {
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

void Mouse::reset() {
	if (_leftButton == kButtonReleased)
		_leftButton = kButtonNormal;
	if (_rightButton == kButtonReleased)
		_rightButton = kButtonNormal;
}

void Mouse::onEvent(const Common::Event &event) {
	switch (event.type) {
	case Common::EVENT_LBUTTONDOWN:
		_leftButton = kButtonPressed;
		break;
	case Common::EVENT_LBUTTONUP:
		_leftButton = kButtonReleased;
		break;
	case Common::EVENT_RBUTTONDOWN:
		_rightButton = kButtonPressed;
		break;
	case Common::EVENT_RBUTTONUP:
		_rightButton = kButtonReleased;
		break;
	default:
		break;
	}
}

Common::Point Mouse::getPosition() const {
	return _vm->getEventManager()->getMousePos();
}

bool Mouse::isOver(const Common::Rect &rect) const {
	return rect.contains(getPosition());
}

} // End of namespace Orlando
