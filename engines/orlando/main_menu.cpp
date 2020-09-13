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
#include "common/translation.h"
#include "graphics/surface.h"
#include "gui/saveload.h"

#include "orlando/main_menu.h"
#include "orlando/orlando.h"
#include "orlando/graphics.h"
#include "orlando/resource.h"
#include "orlando/sound.h"
#include "orlando/mouse.h"
#include "orlando/element.h"
#include "orlando/animation.h"
#include "orlando/avx_video.h"
#include "orlando/window.h"

namespace Orlando {

MainMenu::MainMenu(OrlandoEngine *vm) : Scene(vm, "MENU") {
}

bool MainMenu::playMenuMusic() {
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
		_vm->getSoundManager()->playFile(music, Audio::Mixer::kMusicSoundType, true);
	} else {
		return false;
	}
	return true;
}

bool MainMenu::initialize() {
	if (!(_pak = _vm->getResourceManager()->loadPakArchive("MENU.PAK")))
		return false;

	GraphicsManager *graphics = _vm->getGraphicsManager();
	uint32 time = _vm->getTotalPlayTime();
	// Background
	if (Common::File *file = loadFile("JACK2.BM")) {
		_background = graphics->loadRawBitmap(file);
	} else {
		return false;
	}

	// Truck
	if (Common::File *file = loadFile("_SAMOCH1.BM")) {
		const char *id = "_SAMOCH1.BM";
		Animation *anim = new Animation(id);

		AFrame frames[] = {
			{ graphics->loadRawBitmap(file) },
			{ nullptr } // Blank frame
		};
		anim->addFrame(frames[0]);
		anim->addFrame(frames[1]);

		anim->play(false, 100, kPlayLoop, 0, time);

		Element *element = new Element(id);
		element->setWindow(addWindow());
		element->setPosition(Common::Point(0, 323));
		element->setAnimation(anim);
		_elements[id] = element;
	} else {
		return false;
	}

	// Smoke
	if (Common::File *file = loadFile("D1.FLX")) {
		const char *id = "D1.FLX";
		Animation *anim = new Animation(id);
		anim->loadFlx(file, this, time);

		Element *element = new Element(id);
		element->setWindow(addWindow());
		element->setPosition(Common::Point(309, 39));
		element->setAnimation(anim);
		_elements[id] = element;
	} else {
		return false;
	}

	// Text colors
	_textColors[0].border = graphics->RGBToColor(0, 0, 0); // black
	_textColors[0].fill = graphics->RGBToColor(255, 255, 0); // yellow
	_textColors[1].border = graphics->RGBToColor(0, 0, 255); // blue
	_textColors[1].fill = graphics->RGBToColor(255, 255, 255); // white

	// Music
	if (!playMenuMusic())
		return false;

	_vm->getMouse()->show(true);

	return true;
}

bool MainMenu::run() {
	Scene::run();

	// Draw UI
	GraphicsManager *graphics = _vm->getGraphicsManager();

	Common::Rect kUiWindow = Common::Rect(0, 0, 230, 235);
	kUiWindow.translate(205, 190);
	Common::Rect kUiButton = Common::Rect(0, 0, 180, 34);
	kUiButton.translate(kUiWindow.left + 25, kUiWindow.top + 60);

	graphics->drawShadowRect(kUiWindow, 0.5f, 5);
	graphics->drawText("\x04=MAIN MENU=\x04", Common::Point(kUiWindow.left, kUiWindow.top + 16), kUiWindow.width(), _textColors[0], Graphics::kTextAlignCenter);
	if (graphics->drawButton("Introduction", kUiButton, _textColors[1])) {
		_vm->gotoScene(new AvxVideo(_vm, "INTRO"));
	}
	kUiButton.translate(0, 38);
	if (graphics->drawButton("New game", kUiButton, _textColors[1])) {
		_vm->gotoScene(new Scene(_vm, "SC001"));
	}
	kUiButton.translate(0, 38);
	if (graphics->drawButton("Load game", kUiButton, _textColors[1])) {
		GUI::SaveLoadChooser dialog = GUI::SaveLoadChooser(_("Load game:"), _("Load"), false);
		dialog.runModalWithCurrentTarget();
	}
	kUiButton.translate(0, 38);
	if (graphics->drawButton("Exit", kUiButton, _textColors[1])) {
		_vm->quitGame();
	}

	return true;
}

} // End of namespace Orlando
