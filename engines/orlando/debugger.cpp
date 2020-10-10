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
#include "common/str-array.h"
#include "graphics/surface.h"
#include "graphics/managed_surface.h"

#include "orlando/debugger.h"
#include "orlando/orlando.h"
#include "orlando/macro.h"
#include "orlando/interp.h"
#include "orlando/scene.h"
#include "orlando/element.h"
#include "orlando/area.h"
#include "orlando/window.h"

namespace Orlando {

Debugger::Debugger(OrlandoEngine *vm) : GUI::Debugger(), _vm(vm),
	_showWalks(false), _showElements(false), _showAreas(false), _showWindows(false) {
	registerCmd("run", WRAP_METHOD(Debugger, cmdRun));
	registerCmd("scene", WRAP_METHOD(Debugger, cmdScene));

	registerVar("walks", &_showWalks);
	registerVar("elements", &_showElements);
	registerVar("areas", &_showAreas);
	registerVar("windows", &_showWindows);
}

void Debugger::onFrame() {
	GUI::Debugger::onFrame();

	GraphicsManager *graphics = _vm->getGraphicsManager();
	Scene *scene = _vm->getScene();

	if (_showWalks) {
		for (Common::Array<Triangle>::const_iterator i = scene->_walkRegions.begin(); i != scene->_walkRegions.end(); ++i) {
			graphics->drawPolygon(*i, graphics->RGBToColor(0, 0, 255));
		}
	}

	if (_showElements) {
		for (Common::HashMap<Common::String, Element *>::const_iterator i = scene->_elements.begin(); i != scene->_elements.end(); ++i) {
			Element *element = i->_value;
			if (element->getWindow()->isVisible() && element->_surface != nullptr) {
				graphics->drawPolygon(element->getDrawRegion(), graphics->RGBToColor(0, 255, 0));
				graphics->drawPolygon(element->getWalkRegion(), graphics->RGBToColor(0, 64, 0));
			}
		}
	}

	if (_showAreas) {
		for (Common::HashMap<Common::String, Area *>::const_iterator i = scene->_areas.begin(); i != scene->_areas.end(); ++i) {
			Area *area = i->_value;
			for (Common::Array<Triangle>::const_iterator j = area->_regions.begin(); j != area->_regions.end(); ++j) {
				graphics->drawPolygon(*j, graphics->RGBToColor(0, 255, 255));
			}
		}
	}

	if (_showWindows) {
		for (Common::Array<Window*>::const_iterator i = scene->_windows.begin(); i != scene->_windows.end(); ++i) {
			if ((*i)->isVisible()) {
				Common::Rect rect((*i)->_surface->w, (*i)->_surface->h);
				rect.moveTo((*i)->_pos);
				graphics->drawPolygon(rect, graphics->RGBToColor(255, 0, 255));
			}
		}
	}
}

bool Debugger::cmdRun(int argc, const char **argv) {
	if (!_vm->getJack()) {
		_vm->newGame();
	}
	if (argc >= 2) {
		Common::StringArray args;
		args.reserve(argc - 1);
		for (int i = 1; i < argc; i++) {
			Common::String arg = argv[i];
			arg.toUppercase();
			args.push_back(arg);
		}
		Macro macro("_DEBUG_");
		const MacroCommand &cmd = macro.loadCommand(args);
		if (cmd.type == kCmdUnknown) {
			debugPrintf("Invalid command\n");
		} else {
			_vm->getScriptInterpreter()->runCommand(&macro, cmd, _vm->getTotalPlayTime());
			return false;
		}
	}
	return true;
}

bool Debugger::cmdScene(int argc, const char **argv) {
	debugPrintf("%s\n", _vm->getScene()->getId().c_str());
	return true;
}

} // End of namespace Orlando
