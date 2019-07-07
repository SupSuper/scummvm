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
	 * SETPOSITION <person> <x> <y>
	 * Changes a person's position.
	 */
	bool cmdSetPosition(Macro *macro, const MacroCommand &cmd);
	/**
	 * ANIMA <id> <dir> <delay> <mode> <rec> <+->
	 * Starts playing animation <id>:
	 * <dir> 0 = play forward, 1 = play backward
	 * <delay> time between frames in 1/60s
	 * <mode> 1 = play once, 2 = loop, 3 rec once, 4 rec loop, 5 flx once, 6 flx loop
	 * <rec> record to play
	 */
	bool cmdAnima(Macro *macro, const MacroCommand &cmd);
	/**
	 * HIDE <person>
	 * Makes a person invisible.
	 */
	bool cmdHide(Macro *macro, const MacroCommand &cmd);
	/**
	 * ACTIVEMACRO <id>
	 * Enables a script macro.
	 */
	bool cmdActiveMacro(Macro *macro, const MacroCommand &cmd);
	/**
	 * ENDIF
	 * End of an If block.
	 */
	bool cmdEndIf(Macro *macro, const MacroCommand &cmd);
	/**
	 * GOTOSCENE <id>
	 * Changes the active scene.
	 */
	bool cmdGoToScene(Macro *macro, const MacroCommand &cmd);
	/**
	 * LET <var> <value>
	 * Assigns <value> to <var>.
	 */
	bool cmdLet(Macro *macro, const MacroCommand &cmd);
	/**
	 * IF <var> <value>
	 * Executes block if <var> is equal to <var>.
	 */
	bool cmdIf(Macro *macro, const MacroCommand &cmd);
	/**
	 * EFFECT <id> <arg2> <arg3> <arg4>
	 * Plays a sound effect.
	 */
	bool cmdEffect(Macro *macro, const MacroCommand &cmd);
	/**
	 * HIDEE <element>
	 * Makes an element invisible.
	 */
	bool cmdHideE(Macro *macro, const MacroCommand &cmd);
	/**
	 * SHOWE <element>
	 * Makes an element visible.
	 */
	bool cmdShowE(Macro *macro, const MacroCommand &cmd);
	/**
	 * SETPOSITIONE <element> <x> <y>
	 * Changes an element's position.
	 */
	bool cmdSetPositionE(Macro *macro, const MacroCommand &cmd);
	/**
	 * MOVEP <person> <x> <y>
	 * Moves a person by a relative amount.
	 */
	bool cmdMoveP(Macro *macro, const MacroCommand &cmd);
	/**
	 * MOVEE <element> <x> <y>
	 * Moves an element by a relative amount.
	 */
	bool cmdMoveE(Macro *macro, const MacroCommand &cmd);
	/**
	 * MUSIC <id> <arg2> <arg3>
	 * Plays a music track.
	 */
	bool cmdMusic(Macro *macro, const MacroCommand &cmd);
	/**
	 * INC <var>
	 * Increments <var> by 1.
	 */
	bool cmdInc(Macro *macro, const MacroCommand &cmd);
	/**
	 * WAITWHILE <var> <value>
	 * Pauses this macro while <var> is equal to <value>.
	 */
	bool cmdWaitWhile(Macro *macro, const MacroCommand &cmd);
	/**
	 * IFF <var> <op> <value>
	 * Executes block if <var> is less or greater than <value>.
	 */
	bool cmdIff(Macro *macro, const MacroCommand &cmd);
	/**
	 * GETPERSONX <person> <var>
	 * Sets <var> to the <person>'s X position.
	 */
	bool cmdGetPersonX(Macro *macro, const MacroCommand &cmd);
	/**
	 * GETPERSONY <person> <var>
	 * Sets <var> to the <person>'s Y position.
	 */
	bool cmdGetPersonY(Macro *macro, const MacroCommand &cmd);
	/**
	 * INCC <var> <value>
	 * Increments <var> by <value>.
	 */
	bool cmdIncc(Macro *macro, const MacroCommand &cmd);
	/**
	 * RUNAVX <avx> <background>
	 * Plays a AVX video with a background surface.
	 */
	bool cmdRunAvx(Macro *macro, const MacroCommand &cmd);
public:
	ScriptInterpreter(OrlandoEngine *vm);
	bool runCommand(Macro *macro, const MacroCommand &cmd);
};

} // End of namespace Orlando

#endif
