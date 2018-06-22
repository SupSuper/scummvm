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

#include "common/error.h"
#include "common/events.h"
#include "common/system.h"

#include "orlando/orlando.h"
#include "orlando/debugger.h"
#include "orlando/graphics.h"
#include "orlando/resource.h"
#include "orlando/main_menu.h"

namespace Orlando {

OrlandoEngine::OrlandoEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst), _debugger(nullptr), _menu(nullptr), _gameDescription(gameDesc) {
	_graphics = new GraphicsManager(this);
	_resources = new ResourceManager(this);
}

OrlandoEngine::~OrlandoEngine() {
	delete _menu;
	delete _debugger;
	delete _resources;
	delete _graphics;
}

Common::Error OrlandoEngine::run() {
	if (!_graphics->setupScreen())
		return Common::kUnsupportedColorMode;

	if (!_resources->loadGlobalResources())
		return Common::kNoGameDataFoundError;

	_debugger = new Debugger(this);
	_menu = new MainMenu(this);

	if (!_menu->setup())
		return Common::kUnknownError;

	Common::Event event;
	while (!shouldQuit()) {

		while (_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_KEYDOWN:
				// CTRL+D - open debugger
				if (event.kbd.hasFlags(Common::KBD_CTRL) && event.kbd.keycode == Common::KEYCODE_d) {
					_debugger->attach();
				}
				break;
			default:
				break;
			}
		}

		if (!_menu->run())
			return Common::kUnknownError;

		_debugger->onFrame();
		_graphics->updateScreen();
		_system->delayMillis(10);
	}

	return Common::kNoError;
}

} // End of namespace Orlando
