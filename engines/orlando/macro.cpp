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

Macro::Macro(const Common::String &id) : _id(id) {
}

void Macro::load(TextParser &parser) {
	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty() || id.firstChar() == '[') {
			parser.rewind();
			break;
		}
		MacroCommand cmd;
		cmd.type = kUnknown;
		for (int i = 0; i < ARRAYSIZE(kCommands); i++) {
			if (id == kCommands[i]) {
				cmd.type = (CommandType)(i + 1);
				break;
			}
		}
		if (cmd.type == kUnknown) {
			warning("Macro: Unknown command: %s", id.c_str());
		}
		parser.rewind();
		cmd.args = parser.readLine();
		_commands.push_back(cmd);
	}
}

} // End of namespace Orlando
