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
#include "engines/util.h"

#include "orlando/orlando.h"

namespace Orlando {

OrlandoEngine::OrlandoEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst), _gameDescription(gameDesc) {
}

OrlandoEngine::~OrlandoEngine() {
}

Common::Error OrlandoEngine::run() {
	Graphics::PixelFormat pixelFormat16(2, 5, 6, 5, 0, 11, 5, 0, 0);
	initGraphics(640, 480, &pixelFormat16);

	bool quit = false;

	while (!quit) {
		Common::Event event;
		while (g_system->getEventManager()->pollEvent(event)) {
			switch (event.type) {
			case Common::EVENT_QUIT:
			case Common::EVENT_RTL:
				quit = true;
				break;
			default:
				break;
			}
		}

		g_system->updateScreen();
		g_system->delayMillis(10);
	}

	return Common::kNoError;
}

} // End of namespace Orlando
