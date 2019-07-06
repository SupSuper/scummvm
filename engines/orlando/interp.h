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

#ifndef ORLANDO_INTERP_H
#define ORLANDO_INTERP_H

#include "orlando/macro.h"

namespace Orlando {

class OrlandoEngine;

typedef bool (ScriptInterpreter:: *ScriptHandler)(Macro *, const MacroCommand &);

/**
 * Interprets and executes scene macros (scripts).
 */
class ScriptInterpreter {
	OrlandoEngine *_vm;
	static const ScriptHandler kCommandHandlers[];

	/** Some commands can be passed either a literal or a *VAR value */
	int varOrLiteral(const Common::String &arg) const;

	bool cmdUnknown(Macro *macro, const MacroCommand &cmd);
	/**
	 * ANIMA <id> <dir> <delay> <mode> <rec>
	 * Starts playing animation <id>:
	 * <dir> 0 = play forward, 1 = play backward
	 * <delay> time between frames in 1/60s
	 * <mode> 1 = play once, 2 = loop, 3 rec once, 4 rec loop, 5 flx once, 6 flx loop
	 * <rec> record to play
	 */
	bool cmdAnima(Macro *macro, const MacroCommand &cmd);
	/** End of an IF block */
	bool cmdEndIf(Macro *macro, const MacroCommand &cmd);
	/**
	 * LET <var> <value>
	 * Assigns <value> to <var>
	 */
	bool cmdLet(Macro *macro, const MacroCommand &cmd);
	/**
	 * IF <var> <value>
	 * Checks if <var> is equal to <var>
	 */
	bool cmdIf(Macro *macro, const MacroCommand &cmd);
	/**
	 * INC <var>
	 * Increments <var> by 1
	 */
	bool cmdInc(Macro *macro, const MacroCommand &cmd);
	/**
	 * IFF <var> <op> <value>
	 * Checks if <var> is less or greater than <value>
	 */
	bool cmdIff(Macro *macro, const MacroCommand &cmd);
	/**
	 * INCC <var> <value>
	 * Increments <var> by <value>
	 */
	bool cmdIncc(Macro *macro, const MacroCommand &cmd);
public:
	ScriptInterpreter(OrlandoEngine *vm);
	bool runCommand(Macro *macro, const MacroCommand &cmd);
};

} // End of namespace Orlando

#endif
