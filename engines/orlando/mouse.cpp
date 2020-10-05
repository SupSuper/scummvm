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

Mouse::Mouse(OrlandoEngine *vm) : _vm(vm), _leftButton(kButtonNormal), _rightButton(kButtonNormal), _cursor(kCursorPointer) {
}

Mouse::~Mouse() {
	for (int i = 0; i < kCursorModes; i++) {
		_cursorSurfaces[i]->free();
		delete _cursorSurfaces[i];
	}
}

static const char *cursorMacros[] = {
    "POKAZANIE",  // kCursorPointer
    "PATRZENIE",  // kCursorExamine
    "UDERZENIE",  // kCursorHit
    "STRZELANIE", // kCursorGun
    "BRANIE",     // kCursorTake
    "PIERDOLA",   // kCursorItem
    "LEWO",       // kCursorArrowLeft
    "PRAWO",      // kCursorArrowRight
    "DOL",        // kCursorArrowDown
    "TO_IN",      // kCursorArrowIn
    "UP",         // kCursorArrowUp
    "TO_OUT",     // kCursorArrowOut
    "",           // kCursorScrollLeft
    "",           // kCursorScrollRight
};

static const char *cursorGraphics[] = {
    "WSK08.BM",    // kCursorPointer
    "BLIR00.BM",   // kCursorExamine
    "FIS09.BM",    // kCursorHit
    "KOLT01.BM",   // kCursorGun
    "HAN10.BM",    // kCursorTake
    "RAMKA.BM",    // kCursorItem
    "LEFT015.BM",  // kCursorArrowLeft
    "RIGHT15.BM",  // kCursorArrowRight
    "STDN0001.BM", // kCursorArrowDown
    "TOIN0001.BM", // kCursorArrowIn
    "STUP0001.BM", // kCursorArrowUp
    "TOUT0015.BM", // kCursorArrowOut
    "SCLF0001.BM", // kCursorScrollLeft
    "SCRG0001.BM", // kCursorScrollRight
};

void Mouse::updateCursor() const {
	Graphics::Surface *surface = _cursorSurfaces[_cursor];
	CursorMan.replaceCursor(surface->getPixels(), surface->w, surface->h, 0, 0, 0, false, &surface->format);
}

bool Mouse::initialize() {
	ResourceManager *resources = _vm->getResourceManager();

	for (int i = 0; i < kCursorModes; i++) {
		if (Common::File *file = resources->loadResourceFile(cursorGraphics[i])) {
			_cursorSurfaces[i] = _vm->getGraphicsManager()->loadRawBitmap(file);
		} else {
			return false;
		}
	}
	updateCursor();
	setVisible(true);

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
		setCursor((CursorMode)((_cursor + 1) % kCursorModes));
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

bool Mouse::getVisible() const {
	return CursorMan.isVisible();
}

void Mouse::setVisible(bool visible) const {
	CursorMan.showMouse(visible);
}

Common::String Mouse::getCursorMacroName(const Common::String &id) const {
	Common::String macroName = cursorMacros[_cursor];
	macroName += "-";
	macroName += id;
	return macroName;
}

} // End of namespace Orlando
