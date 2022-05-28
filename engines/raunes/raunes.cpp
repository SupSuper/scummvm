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
#include "raunes/sound.h"

namespace Raunes {

RaunesEngine::RaunesEngine(OSystem *syst)
	: Engine(syst), _gfx(this), _snd(this) {
}

RaunesEngine::~RaunesEngine() {
}

Common::Error RaunesEngine::run() {
	if (!_gfx.load()) {
		return Common::kNoGameDataFoundError;
	}
	_gfx.init();

	logo();
	_snd.play("raum1.snd", 22050);
	intro();

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

void RaunesEngine::logo() {
	_gfx.setPage(0);
	_gfx.showPage(1);
	_gfx.clearScreen();
	_gfx.showPcx("LOGO.PCX");
	_gfx.swapPage();
}

void RaunesEngine::intro() {

}

} // End of namespace Raunes
