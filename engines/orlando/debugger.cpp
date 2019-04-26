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

#include "orlando/debugger.h"
#include "orlando/orlando.h"
#include "orlando/scene.h"

namespace Orlando {

Debugger::Debugger(OrlandoEngine *vm) : GUI::Debugger(), _vm(vm) {
	registerCmd("music", WRAP_METHOD(Debugger, cmdMusic));
	registerCmd("sound", WRAP_METHOD(Debugger, cmdSfx));
	registerCmd("voice", WRAP_METHOD(Debugger, cmdSpeech));
	registerCmd("scene", WRAP_METHOD(Debugger, cmdScene));
}

bool Debugger::cmdMusic(int argc, const char **argv) {
	if (argc == 2) {
		_vm->getScene()->playMusic(argv[1]);
	}
	return true;
}

bool Debugger::cmdSfx(int argc, const char **argv) {
	if (argc == 2) {
		_vm->getScene()->playSfx(argv[1]);
	}
	return true;
}

bool Debugger::cmdSpeech(int argc, const char **argv) {
	if (argc == 2) {
		_vm->getScene()->playSpeech(argv[1]);
	}
	return true;
}

bool Debugger::cmdScene(int argc, const char **argv) {
	if (argc == 2) {
		_vm->gotoScene(argv[1]);
	}
	return false;
}

} // End of namespace Orlando
