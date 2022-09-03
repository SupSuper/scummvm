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

#include "common/config-manager.h"
#include "common/debug.h"
#include "common/debug-channels.h"
#include "common/error.h"
#include "common/events.h"
#include "common/system.h"

#include "raunes/raunes.h"
#include "raunes/graphics.h"
#include "raunes/font.h"
#include "raunes/logic.h"
#include "raunes/sound.h"

namespace Raunes {

RaunesEngine::RaunesEngine(OSystem *syst, const ADGameDescription *desc)
	: Engine(syst), _gfx(this), _snd(this), _game(nullptr) {
	if (strcmp(desc->gameId, "raunes3") == 0) {
		_game = new Logic_v3(this);
	} else {
		_game = new Logic_v2de(this);
	}
}

RaunesEngine::~RaunesEngine() {
	delete _game;
}

Common::Error RaunesEngine::run() {
	if (!_gfx.load()) {
		return Common::kNoGameDataFoundError;
	}
	_gfx.init();

	_game->logo();
	_game->intro();

	Common::Event evt;
	while (!shouldQuit()) {
		while (g_system->getEventManager()->pollEvent(evt));
		g_system->updateScreen();
		g_system->delayMillis(10);
	}

	return Common::kNoError;
}

bool RaunesEngine::hasFeature(EngineFeature f) const {
	return (f == kSupportsReturnToLauncher) ||
		   (f == kSupportsLoadingDuringRuntime) ||
		   (f == kSupportsSavingDuringRuntime);
}

void RaunesEngine::delay(int ms) {
	int time = 0;
	bool skip = false;
	while (time < ms && !skip) {
		Common::Event evt;
		while (g_system->getEventManager()->pollEvent(evt)) {
			switch (evt.type) {
			case Common::EVENT_QUIT:
			case Common::EVENT_LBUTTONUP:
			case Common::EVENT_RBUTTONUP:
			case Common::EVENT_KEYUP:
				skip = true;
				break;
			default:
				break;
			}
		}
		g_system->updateScreen();
		g_system->delayMillis(10);
		time += 10;
	}
}

} // End of namespace Raunes
