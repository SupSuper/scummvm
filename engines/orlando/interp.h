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

typedef bool (ScriptInterpreter:: *ScriptHandler)(const MacroCommand &);

/**
 * Interprets and executes scene macros (scripts).
 */
class ScriptInterpreter {
	static const ScriptHandler kCommandHandlers[];
	OrlandoEngine *_vm;
	Macro *_macro;
	uint32 _time;

	/** Every tick is a 1/60th of a second */
	int ticksToMs(int ticks) { return ticks * 1000 / 60; }
	/** Some commands can be passed either a literal or a *VAR reference */
	int getVarOrLiteral(const Common::String &arg) const;
	/** Commands with a - at the end wait for the action to complete, + ignores it */
	bool waitUntilComplete(const MacroCommand &cmd) const;

	bool cmdUnknown(const MacroCommand &cmd);
	/**
	 * CHWINDOW <id> <x1> <y1> <x2> <y2>
	 * Changes the draw window of an element or person.
	 * The draw window acts as a clipping rect, nothing is drawn outside of it.
	 */
	bool cmdChWindow(const MacroCommand &cmd);
	/**
	 * SETPERSONDATA <id> <animSpeed> <scale> <perspectiveY> <walkSpeed>
	 * Sets some parameters for person <id>:
	 * <animSpeed> time between frames
	 * <scale> 0.1-1.0 scaling factor (2.0 = no perspective scaling, 3.0 = ?)
	 * <perspectiveY> Y position to use for perspective scaling
	 * <walkSpeed> pixels walked every tick
	 */
	bool cmdSetPersonData(const MacroCommand &cmd);
	/**
	 * SETPOSITION <person> <x> <y>
	 * Changes a person's position.
	 */
	bool cmdSetPosition(const MacroCommand &cmd);
	/**
	 * ANIMA <id> <dir> <delay> <mode> <rec> <+->
	 * Starts playing animation <id>:
	 * <dir> 0 = play forward, 1 = play backward
	 * <delay> time between frames in ticks
	 * <mode> 1 = play once, 2 = loop, 3 rec once, 4 rec loop, 5 flx once, 6 flx loop
	 * <rec> record to play (optional)
	 */
	bool cmdAnima(const MacroCommand &cmd);
	/**
	 * STOPANIMA <id>
	 * Pauses animation <id>.
	 */
	bool cmdStopAnima(const MacroCommand &cmd);
	/**
	 * WALKTO <person> <x> <y>
	 * Moves a person on the scene.
	 */
	bool cmdWalkTo(const MacroCommand &cmd);
	/**
	 * HIDE <person>
	 * Makes a person invisible.
	 */
	bool cmdHide(const MacroCommand &cmd);
	/**
	 * TALK <person> <dialog> <color> <+->
	 * Plays a dialog line:
	 * <person> person speaking (LEFT = left face, RIGHT = right face, POS = fixed position)
	 * <dialog> dialog line
	 * <color> scene text color
	 */
	bool cmdTalk(const MacroCommand &cmd);
	/**
	 * DIALOG <person> <dialog>
	 * Shows a list of <dialog> options to choose from and <person> speaks the answer.
	 */
	bool cmdDialog(const MacroCommand &cmd);
	/**
	 * ACTIVEMACRO <id>
	 * Enables a script macro.
	 */
	bool cmdActiveMacro(const MacroCommand &cmd);
	/**
	 * IFANSWER <dialog> <option>
	 * Executes block if <dialog> was answered with <option>.
	 */
	bool cmdIfAnswer(const MacroCommand &cmd);
	/**
	 * ENDIF
	 * End of an If block.
	 */
	bool cmdEndIf(const MacroCommand &cmd);
	/**
	 * GOTOSCENE <id>
	 * Changes the active scene.
	 */
	bool cmdGoToScene(const MacroCommand &cmd);
	/**
	 * WALKTODIR <person> <x> <y> <dir>
	 * Moves a person on the scene in a fixed direction.
	 */
	bool cmdWalkToDir(const MacroCommand &cmd);
	/**
	 * LET <var> <value>
	 * Assigns <value> to <var>.
	 */
	bool cmdLet(const MacroCommand &cmd);
	/**
	 * IF <var> <value>
	 * Executes block if <var> is equal to <var>.
	 */
	bool cmdIf(const MacroCommand &cmd);
	/**
	 * EFFECT <id> <arg2> <arg3> <arg4>
	 * Plays a sound effect.
	 */
	bool cmdEffect(const MacroCommand &cmd);
	/**
	 * HIDEE <element>
	 * Makes an element invisible.
	 */
	bool cmdHideE(const MacroCommand &cmd);
	/**
	 * SHOWE <element>
	 * Makes an element visible.
	 */
	bool cmdShowE(const MacroCommand &cmd);
	/**
	 * SETPOSITIONE <element> <x> <y>
	 * Changes an element's position.
	 */
	bool cmdSetPositionE(const MacroCommand &cmd);
	/**
	 * SETPERSPY <person> <y>
	 * Changes a person's Y perspective.
	 */
	bool cmdSetPerspY(const MacroCommand &cmd);
	/**
	 * SETDIALOG <dialog> <option> <enabled>
	 * Toggles a dialog option off (0) or on (1).
	 */
	bool cmdSetDialog(const MacroCommand &cmd);
	/**
	 * MOVEP <person> <x> <y>
	 * Moves a person by a relative amount.
	 */
	bool cmdMoveP(const MacroCommand &cmd);
	/**
	 * DEACTIVESELF
	 * Disables the current macro.
	 */
	bool cmdDeactiveSelf(const MacroCommand &cmd);
	/**
	 * MOVEE <element> <x> <y>
	 * Moves an element by a relative amount.
	 */
	bool cmdMoveE(const MacroCommand &cmd);
	/**
	 * MUSIC <id> <arg2> <arg3>
	 * Plays a music track.
	 */
	bool cmdMusic(const MacroCommand &cmd);
	/**
	 * INC <var>
	 * Increments <var> by 1.
	 */
	bool cmdInc(const MacroCommand &cmd);
	/**
	 * WAITWHILE <var> <value>
	 * Pauses this macro while <var> is equal to <value>.
	 */
	bool cmdWaitWhile(const MacroCommand &cmd);
	/**
	 * IFF <var> <op> <value>
	 * Executes block if <var> is less or greater than <value>.
	 */
	bool cmdIff(const MacroCommand &cmd);
	/**
	 * GETPERSONX <person> <var>
	 * Sets <var> to the <person>'s X position.
	 */
	bool cmdGetPersonX(const MacroCommand &cmd);
	/**
	 * GETPERSONY <person> <var>
	 * Sets <var> to the <person>'s Y position.
	 */
	bool cmdGetPersonY(const MacroCommand &cmd);
	/**
	 * INCC <var> <value>
	 * Increments <var> by <value>.
	 */
	bool cmdIncc(const MacroCommand &cmd);
	/**
	 * CONTINUEANIMA <id>
	 * Resumes animation <id>.
	 */
	bool cmdContinueAnima(const MacroCommand &cmd);
	/**
	 * DONOTHING <time>
	 * Pauses this macro until <time> ticks has elapsed.
	 */
	bool cmdDoNothing(const MacroCommand &cmd);
	/**
	 * DEACTIVEMACRO <id>
	 * Disables a script macro.
	 */
	bool cmdDeactiveMacro(const MacroCommand &cmd);
	/**
	 * RUNAVX <avx> <background>
	 * Plays a AVX video with a background surface.
	 */
	bool cmdRunAvx(const MacroCommand &cmd);
	/**
	 * REACTIVESELF
	 * Resets the macro back to the start.
	 */
	bool cmdReactiveSelf(const MacroCommand &cmd);
public:
	ScriptInterpreter(OrlandoEngine *vm);
	bool runCommand(Macro *macro, const MacroCommand &cmd, uint32 time);
};

} // End of namespace Orlando

#endif
