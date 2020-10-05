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
#include "orlando/macro.h"
#include "orlando/text_parser.h"
#include "orlando/interp.h"
#include "orlando/util.h"

namespace Orlando {

const char *kCommands[] = {
	"CHWINDOW",
	"SETPOSITION",
	"INITFIRST",
	"SETPERSONDATA",
	"ANIMA",
	"STOPANIMA",
	"WALKTO",
	"RUNINSERTION",
	"RUNFILM",
	"HIDE",
	"TALK",
	"DIALOG",
	"ACTIVEMACRO",
	"IFANSWER",
	"ENDIF",
	"SHOWFRAME",
	"HIDETALK",
	"SHADOWBY",
	"SETSHADOW",
	"GOTOSCENE",
	"PUNDERP",
	"PUNDERE",
	"WALKTODIR",
	"SHOWFACE",
	"HIDEFACE",
	"LET",
	"IF",
	"SHOWANIMAFRAME",
	"POVERE",
	"EFFECT",
	"EFFECTVOLUME",
	"TALKRANDOM",
	"TAKE",
	"RUNFILMP",
	"EXTRA",
	"MOVEAR",
	"SETFRAME",
	"NOHAVE",
	"HIDEE",
	"SHOWE",
	"SETSHIFTX",
	"SETPOSITIONE",
	"SETPOSITIONEV",
	"IFMOUSEXM",
	"IFMOUSEXW",
	"SCROLLLEFT",
	"SCROLLRIGHT",
	"SETSCROLLX1",
	"SETSCROLLX2",
	"SETPOSXY",
	"SETCHKPRIOR",
	"BRIGHTNESS",
	"SETPERSPY",
	"MOVEP",
	"STOPANIMAFRAME",
	"SETDIALOG",
	"STOPANIMANEG",
	"SETANIMAFRAME",
	"DEACTIVESELF",
	"EOVERE",
	"MUSIC",
	"MOVEE",
	"BRIGHTNESSE",
	"WALKEFFECT",
	"HAVEGUN",
	"NOHAVEGUN",
	"ACTIVEMACROAT",
	"HAVETOOL",
	"NOHAVETOOL",
	"INC",
	"WAITWHILE",
	"STAY",
	"GAMEOVER",
	"MOVEEP",
	"WAITWHILEANIMA",
	"SHOWFRAMEDIR",
	"STAYDEF",
	"UNLOCK",
	"CURSORAS",
	"ANIMAEFFECT",
	"SETAREA",
	"SETSPECAREA",
	"WAITTOANIMAFRAME",
	"SCROLLMARGINL",
	"SCROLLMARGINR",
	"OUTMACRO",
	"NORMALMACRO",
	"IFF",
	"AUTOINCRANGE",
	"LOCK",
	"HIDEFACEAT",
	"INCLUDE",
	"LOOKTO",
	"GETPERSONX",
	"GETPERSONY",
	"INCC",
	"CONTINUEANIMA",
	"LOCKWALK",
	"UNLOCKWALK",
	"WAITWHILEEFFECT",
	"SETDARKNESSANIMA",
	"SETDIRWALK",
	"MESSAGE",
	"POINTROTATE",
	"RUNGLOBALFILMP",
	"DONOTHING",
	"SLAP",
	"SHOOT",
	"ACTIVEMACROANIMA",
	"PAUSE",
	"EXITGAME",
	"ROTATETO",
	"MACRODESTRUCTOR",
	"BLOCKMACRO",
	"UNBLOCKMACRO",
	"PINAREA",
	"ANIMADELAY",
	"RANDOM",
	"DEACTIVEMACRO",
	"MIXING",
	"REFRESHSCREEN",
	"CLEARANIMABUFFER",
	"RUNAVX",
	"JACKTEMPO",
	"CLEARMENUBAR",
	"UNLOCKCANAL",
	"POINTINSECAREA",
	"TALKRANDOMINDEX",
	"MOUSETOTOOL",
	"RELATIVEMULP",
	"MINSHADOW",
	"MAXSHADOW",
	"PUTSHADOW",
	"ANIMASHADOW",
	"SHIFTSHADOWP",
	"PICKUP",
	"MUSICVOLUME",
	"GIVE",
	"SLAPF",
	"SHOOTF",
	"REACTIVESELF",
	"SETPOSSCROLL",
	"CHKDISTANCE",
	"POINTINWALKAREA",
	"SETSCROLL",
	"SETSPECSHADOW",
	"SHOWSHADOW",
	"SHADOWSTATUS",
	"HIDESHADOW",
	"SHADOWONE",
	"ITEMPRESENT",
	"IGNOREAREA",
	"SAVEABLE",
	"EXITTODOS",
	"ANYKEY",
};

Macro::Macro(const Common::String &id) : _id(id), _active(false), _waiting(false), _locked(true), _out(false), _line(0), _time(0) {
}

CommandType Macro::getType(const Common::String &id) const {
	CommandType type = kCmdUnknown;
	for (int i = 0; i < ARRAYSIZE(kCommands); i++) {
		if (id == kCommands[i]) {
			type = (CommandType)(i + 1);
			break;
		}
	}
	return type;
}

void Macro::reset() {
	_line = 0;
}

void Macro::load(TextParser &parser) {
	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty() || id.firstChar() == '[') {
			parser.rewind();
			break;
		}
		MacroCommand cmd;
		cmd.type = getType(id);
		if (cmd.type == kCmdUnknown) {
			warning("Macro: Unknown command: %s", id.c_str());
		}
		parser.rewind();
		cmd.args = parser.readLine();
		_commands.push_back(cmd);
	}
}

const MacroCommand &Macro::loadCommand(Common::StringArray args) {
	MacroCommand cmd;
	cmd.type = getType(args[0]);
	cmd.args = args;
	_commands.push_back(cmd);
	return _commands.front();
}

void Macro::execute(ScriptInterpreter *interp, uint32 time) {
	if (_active) {
		bool done = interp->runCommand(this, _commands[_line], time);
		if (done)
			_line++;
		if (_line >= _commands.size()) {
			_active = false;
			_line = 0;
		}
	}
}

void Macro::skipIf() {
	int ifLevel = 1;
	do {
		_line++;
		if (_line >= _commands.size()) {
			_active = false;
			warning("Macro: No matching EndIf found");
			break;
		}
		CommandType type = _commands[_line].type;
		switch (type) {
		case kCmdEndIf:
			ifLevel--;
			break;
		case kCmdIf:
		case kCmdIfAnswer:
		case kCmdIff:
		case kCmdIfMouseXM:
		case kCmdIfMouseXW:
			ifLevel++;
			break;
		}
	} while (ifLevel != 0);
}

} // End of namespace Orlando
