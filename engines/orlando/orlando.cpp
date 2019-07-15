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

#include "common/archive.h"
#include "common/config-manager.h"
#include "common/fs.h"
#include "common/file.h"
#include "common/error.h"
#include "common/events.h"
#include "common/system.h"

#include "orlando/orlando.h"
#include "orlando/debugger.h"
#include "orlando/graphics.h"
#include "orlando/resource.h"
#include "orlando/sound.h"
#include "orlando/interp.h"
#include "orlando/mouse.h"
#include "orlando/main_menu.h"
#include "orlando/scene.h"
#include "orlando/text_parser.h"
#include "orlando/jack.h"

namespace Orlando {

OrlandoEngine::OrlandoEngine(OSystem *syst, const ADGameDescription *gameDesc) : Engine(syst),
	_graphics(new GraphicsManager(this)), _resources(new ResourceManager(this)), _sound(new SoundManager(this)), _interp(new ScriptInterpreter(this)),
	_mouse(new Mouse(this)), _debugger(nullptr), _scene(nullptr), _newScene(nullptr), _jack(nullptr), _gameDescription(gameDesc) {

	// Search in subfolders
	const Common::FSNode gameDataDir(ConfMan.get("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "ACTION");
	SearchMan.addSubDirectoryMatching(gameDataDir, "DATA");
	SearchMan.addSubDirectoryMatching(gameDataDir, "MUSIC");
	SearchMan.addSubDirectoryMatching(gameDataDir, "MOVIES");
	SearchMan.addSubDirectoryMatching(gameDataDir, "RESOURCE");
}

OrlandoEngine::~OrlandoEngine() {
	delete _jack;
	delete _newScene;
	delete _scene;
	delete _debugger;
	delete _mouse;
	delete _interp;
	delete _sound;
	delete _resources;
	delete _graphics;
}

void OrlandoEngine::debugScenes() {
	// Debug load every scene
	Common::FSList files;
	Common::FSNode dir(ConfMan.get("path"));
	if (dir.getChild("action").getChildren(files, Common::FSNode::kListFilesOnly)) {
		for (Common::FSList::iterator i = files.begin(); i != files.end(); ++i) {
			Common::String id = i->getName();
			id.erase(id.size() - 4);
			Scene scene(this, id);
			scene.initialize();
		}
	}
}

Common::Error OrlandoEngine::run() {
	if (!_graphics->setupScreen())
		return Common::kUnsupportedColorMode;

	if (!_resources->loadCommonResources())
		return Common::kNoGameDataFoundError;
	
	if (!_mouse->initialize())
		return Common::kNoGameDataFoundError;

	if (!newGame())
		return Common::kNoGameDataFoundError;

	_debugger = new Debugger(this);
	gotoScene(new MainMenu(this));

	Common::Event event;
	while (!shouldQuit()) {
		if (_newScene != nullptr) {
			delete _scene;
			_scene = _newScene;
			_newScene = nullptr;
			if (!_scene->initialize())
				return Common::kNoGameDataFoundError;
		}
		_mouse->reset();
		while (_system->getEventManager()->pollEvent(event)) {
			_mouse->onEvent(event);
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

		if (!_scene->run())
			return Common::kUnknownError;

		_debugger->onFrame();
		_graphics->updateScreen();
		_system->delayMillis(10);
	}

	return Common::kNoError;
}

bool OrlandoEngine::newGame() {
	TextParser varlist = TextParser(_resources->loadResourceFile("VARIABLE.ALL"), false);
	while (!varlist.eof()) {
		Common::String id = varlist.readString();
		if (id.empty())
			break;
		_vars[id] = varlist.readInt();
	}

	_jack = new Jack(this);
	if (!_jack->initialize())
		return false;

	return true;
}

void OrlandoEngine::gotoScene(Scene *scene) {
	delete _newScene;
	_newScene = scene;
}

} // End of namespace Orlando
