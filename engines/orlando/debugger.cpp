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

#include "orlando/debugger.h"
#include "orlando/orlando.h"
#include "orlando/macro.h"
#include "orlando/interp.h"

namespace Orlando {

Debugger::Debugger(OrlandoEngine *vm) : GUI::Debugger(), _vm(vm) {
	registerCmd("run", WRAP_METHOD(Debugger, cmdRun));
}

bool Debugger::cmdRun(int argc, const char **argv) {
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

} // End of namespace Orlando
