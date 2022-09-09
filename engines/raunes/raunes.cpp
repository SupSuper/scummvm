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
#include "raunes/debugger.h"
#include "raunes/graphics.h"
#include "raunes/font.h"
#include "raunes/logic.h"
#include "raunes/sound.h"

namespace Raunes {

RaunesEngine::RaunesEngine(OSystem *syst, const ADGameDescription *desc)
	: Engine(syst), _gfx(this), _snd(this), _game(nullptr) {
	if (strcmp(desc->gameId, "raunes3") == 0) {
		_game = new Logic_v3(this);
	} else if (desc->language == Common::EN_ANY) {
		_game = new Logic_v2en(this);
	} else {
		_game = new Logic_v2de(this);
	}
}

RaunesEngine::~RaunesEngine() {
	delete _game;
}

Common::Error RaunesEngine::run() {
	if (!_gfx.init()) {
		return Common::kNoGameDataFoundError;
	}

	setDebugger(new Debugger(this));

	_game->load();
	_game->start();

	Common::Event ev;
	while (!shouldQuit()) {
		while (g_system->getEventManager()->pollEvent(ev)) {
			switch (ev.type) {
			case Common::EVENT_KEYDOWN:
				// CTRL+D - open debugger
				if (ev.kbd.hasFlags(Common::KBD_CTRL) && ev.kbd.keycode == Common::KEYCODE_d) {
					getDebugger()->attach();
				}
				break;
			default:
				break;
			}
		}
		_game->run();
		g_system->updateScreen();
		g_system->delayMillis(10);
	}

	_game->end();

	return Common::kNoError;
}

bool RaunesEngine::hasFeature(EngineFeature f) const {
	return (f == kSupportsReturnToLauncher) ||
		   (f == kSupportsLoadingDuringRuntime) ||
		   (f == kSupportsSavingDuringRuntime);
}

bool RaunesEngine::delay(int ms) {
	int time = 0;
	bool skip = false;
	Common::Event evt;
	while (time < ms && !skip) {
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
	return skip;
}

} // End of namespace Raunes
