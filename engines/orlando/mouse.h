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

#ifndef ORLANDO_MOUSE_H
#define ORLANDO_MOUSE_H

#include "common/rect.h"

namespace Common {
	struct Event;
}

namespace Orlando {

class OrlandoEngine;

enum MouseState {
	kButtonNormal,	///< Button is inactive
	kButtonPressed,	///< Button is currently held down
	kButtonReleased	///< Button has just been released (used to detect clicks)
};

/**
 * Represents the mouse cursor and handles associated events.
 */
class Mouse {
	OrlandoEngine *_vm;
	MouseState _leftButton, _rightButton;

public:
	Mouse(OrlandoEngine *vm);
	~Mouse();
	/**
	 * Loads and sets up the various cursor modes.
	 * @return False if a cursor graphic is missing.	
	 */
	bool initialize();
	/** Resets the mouse button states. */
	void reset();
	/** Handles mouse events. */
	void onEvent(const Common::Event &event);
	/**
	 * Gets the current mouse position.
	 * @return Position in screen coordinates.
	 */
	Common::Point getPosition() const;
	/**
	 * Checks if the mouse is over a screen region.
	 * @param rect Region coordinates.
	 * @return True if the mouse is inside.
	 */
	bool isOver(const Common::Rect &rect) const;
	/** Returns the state of the left mouse button. */
	MouseState getLeftButton() const { return _leftButton; }
	/** Returns the state of the right mouse button. */
	MouseState getRightButton() const { return _rightButton; }
};

} // End of namespace Orlando

#endif
