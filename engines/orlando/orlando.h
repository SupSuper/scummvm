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

#ifndef ORLANDO_ORLANDO_H
#define ORLANDO_ORLANDO_H

#include "engines/engine.h"
#include "common/hash-str.h"
#include "orlando/debugger.h"

struct ADGameDescription;

namespace Orlando {

class GraphicsManager;
class ResourceManager;
class SoundManager;
class ScriptInterpreter;
class Mouse;
class Scene;
class Jack;

/**
 * Engine for the Jack Orlando adventure game.
 */
class OrlandoEngine : public Engine {
	GraphicsManager *_graphics;
	ResourceManager *_resources;
	SoundManager *_sound;
	ScriptInterpreter *_interp;
	Mouse *_mouse;

	Debugger *_debugger;
	Scene *_scene, *_newScene;
	Common::HashMap<Common::String, int> _vars;
	Jack *_jack;

	void debugScenes();
public:
	OrlandoEngine(OSystem *syst, const ADGameDescription *gameDesc);
	~OrlandoEngine();

	Common::Error run() override;
	bool newGame();
	void gotoScene(Scene *scene);

	GUI::Debugger *getDebugger() override { return _debugger; }
	GraphicsManager *getGraphicsManager() { return _graphics; }
	ResourceManager *getResourceManager() { return _resources; }
	SoundManager *getSoundManager() { return _sound; }
	ScriptInterpreter *getScriptInterpreter() { return _interp; }
	Mouse *getMouse() { return _mouse; }
	Scene *getScene() { return _scene; }
	int &getVariable(const Common::String &id) { return _vars[id]; }
	Jack *getJack() { return _jack; }

	// Detection related functions
	const ADGameDescription *_gameDescription;
	bool isVersionSP() const;
	bool isVersionHP() const;
	bool isVersionDC() const;
	bool hasFeature(EngineFeature f) const override;
};

} // End of namespace Orlando

#endif
