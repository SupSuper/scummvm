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
#include "common/rect.h"
#include "graphics/surface.h"

#include "orlando/main_menu.h"
#include "orlando/orlando.h"
#include "orlando/graphics.h"
#include "orlando/resource.h"
#include "orlando/sound.h"
#include "orlando/flx_anim.h"

namespace Orlando {

MainMenu::MainMenu(OrlandoEngine *vm) : Scene(vm, "MENU"), _bg(nullptr), _truck(nullptr),
	_smoke(nullptr), _truckTimer(0), _drawTruck(false) {
}

MainMenu::~MainMenu() {
	delete _smoke;
	_truck->free();
	delete _truck;
	_bg->free();
	delete _bg;
}

void MainMenu::playMenuMusic() {
	ResourceManager *resources = _vm->getResourceManager();

	Common::File *music = nullptr;
	if (_vm->isVersionSP()) {
		music = resources->loadPakFile("MUSIC.PAK", "TRACK-02.MUS");
	} else if (_vm->isVersionHP()) {
		music = resources->loadPakFile("MUSIC16.PAK", "TRACK-02.PMS");
	} else if (_vm->isVersionDC()) {
		music = resources->loadRawFile("TR02-22.PMS");
	}

	if (music != nullptr) {
		_vm->getSoundManager()->playFile(music, Audio::Mixer::kMusicSoundType);
	}
}

bool MainMenu::initialize() {
	if (!(_pak = _vm->getResourceManager()->loadPakArchive("MENU.PAK"))) {
		return false;
	}

	if (Common::File *file = loadFile("jack2.bm")) {
		_bg = _vm->getGraphicsManager()->loadRawBitmap(file);
	} else {
		return false;
	}

	if (Common::File *file = loadFile("_samoch1.bm")) {
		_truck = _vm->getGraphicsManager()->loadRawBitmap(file);
	} else {
		return false;
	}

	if (Common::File *file = loadFile("d1.flx")) {
		_smoke = new FlxAnimation(file, GraphicsManager::kScreenFormat);
	} else {
		return false;
	}

	_truckTimer = _vm->getTotalPlayTime();

	playMenuMusic();

	return true;
}

bool MainMenu::run() {
	const int kTruckDelay = 100;
	if (_vm->getTotalPlayTime() - _truckTimer >= kTruckDelay) {
		_truckTimer = _vm->getTotalPlayTime();
		_drawTruck = !_drawTruck;
		_smoke->nextFrame();
	}

	GraphicsManager *graphics = _vm->getGraphicsManager();

	graphics->draw(*_bg);
	if (_drawTruck)
		graphics->drawTransparent(*_truck, Common::Point(0, 323));
	//graphics->drawTransparent(*_smoke->getSurface(), Common::Point(309, 39));

	const uint16 kColorYellow = graphics->RGBToColor(255, 255, 0);
	const uint16 kColorBlack = graphics->RGBToColor(0, 0, 0);
	const uint16 kColorBlue = graphics->RGBToColor(0, 0, 255);
	const uint16 kColorWhite = graphics->RGBToColor(255, 255, 255);
	Common::Rect kUiWindow = Common::Rect(0, 0, 230, 235);
	kUiWindow.translate(205, 190);
	Common::Rect kUiButton = Common::Rect(0, 0, 180, 34);
	kUiButton.translate(kUiWindow.left + 25, kUiWindow.top + 60);

	graphics->drawBlendedRect(kUiWindow, kColorBlack, 0.5f);
	graphics->drawText("\x04=MAIN MENU=\x04", Common::Point(kUiWindow.left, kUiWindow.top + 16), kUiWindow.width(), kColorYellow, kColorBlack, Graphics::kTextAlignCenter);
	graphics->drawButton("Introduction", kUiButton, kColorWhite, kColorBlue);
	kUiButton.translate(0, 38);
	if (graphics->drawButton("New game", kUiButton, kColorWhite, kColorBlue)) {
		_vm->gotoScene("SC001");
	}
	kUiButton.translate(0, 38);
	graphics->drawButton("Load game", kUiButton, kColorWhite, kColorBlue);
	kUiButton.translate(0, 38);
	if (graphics->drawButton("Exit", kUiButton, kColorWhite, kColorBlue)) {
		_vm->quitGame();
	}

	return true;
}

} // End of namespace Orlando
