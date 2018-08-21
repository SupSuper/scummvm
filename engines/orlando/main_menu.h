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

#ifndef ORLANDO_MAIN_MENU_H
#define ORLANDO_MAIN_MENU_H

namespace Common {
	class Archive;
}

namespace Graphics {
	struct Surface;
}

namespace Orlando {

class OrlandoEngine;
class FlxAnimation;

/**
  * Scene representing the menu when the game starts.
  */
class MainMenu {
	OrlandoEngine *_vm;
	Common::Archive *_pak;
	Graphics::Surface *_bg, *_truck;
	FlxAnimation *_smoke;
	uint32 _truckTimer;
	bool _drawTruck;

public:
	MainMenu(OrlandoEngine *vm);
	~MainMenu();

	bool initialize();
	bool run();
};

} // End of namespace Orlando

#endif
