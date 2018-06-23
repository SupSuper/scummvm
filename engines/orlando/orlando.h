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
#include "orlando/debugger.h"

struct ADGameDescription;

namespace Orlando {

class GraphicsManager;
class ResourceManager;
class MainMenu;

/**
 * Engine for the Jack Orlando adventure game.
 */
class OrlandoEngine : public Engine {
	Debugger *_debugger;
	GraphicsManager *_graphics;
	ResourceManager *_resources;
	MainMenu *_menu;

public:
	OrlandoEngine(OSystem *syst, const ADGameDescription *gameDesc);
	~OrlandoEngine();

	Common::Error run() override;
	GUI::Debugger *getDebugger() override { return _debugger; }
	GraphicsManager *getGraphicsManager() { return _graphics; }
	ResourceManager *getResourceManager() { return _resources; }

	// Detection related functions
	const ADGameDescription *_gameDescription;
	bool hasFeature(EngineFeature f) const override;
};

} // End of namespace Orlando

#endif
