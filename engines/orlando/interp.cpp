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
 * This program is distributed in the hope that it will be useful
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
#include "common/debug.h"
#include "common/file.h"

#include "orlando/interp.h"
#include "orlando/util.h"
#include "orlando/orlando.h"
#include "orlando/scene.h"
#include "orlando/element.h"
#include "orlando/animation.h"
#include "orlando/sound.h"
#include "orlando/avx_video.h"

namespace Orlando {

const ScriptHandler ScriptInterpreter::kCommandHandlers[] = {
	&ScriptInterpreter::cmdUnknown,
	&ScriptInterpreter::cmdUnknown, // ChWindow
	&ScriptInterpreter::cmdUnknown, // SetPosition
	&ScriptInterpreter::cmdUnknown, // InitFirst
	&ScriptInterpreter::cmdUnknown, // SetPersonData
	&ScriptInterpreter::cmdAnima,
	&ScriptInterpreter::cmdUnknown, // StopAnima
	&ScriptInterpreter::cmdUnknown, // WalkTo
	&ScriptInterpreter::cmdUnknown, // RunInsertion
	&ScriptInterpreter::cmdUnknown, // RunFilm: unused
	&ScriptInterpreter::cmdUnknown, // Hide
	&ScriptInterpreter::cmdUnknown, // Talk
	&ScriptInterpreter::cmdUnknown, // Dialog
	&ScriptInterpreter::cmdUnknown, // ActiveMacro
	&ScriptInterpreter::cmdUnknown, // IfAnswer
	&ScriptInterpreter::cmdEndIf,
	&ScriptInterpreter::cmdUnknown, // ShowFrame: unused
	&ScriptInterpreter::cmdUnknown, // HideTalk: unused
	&ScriptInterpreter::cmdUnknown, // ShadowBy
	&ScriptInterpreter::cmdUnknown, // SetShadow
	&ScriptInterpreter::cmdGoToScene,
	&ScriptInterpreter::cmdUnknown, // PUnderP
	&ScriptInterpreter::cmdUnknown, // PUnderE
	&ScriptInterpreter::cmdUnknown, // WalkToDir
	&ScriptInterpreter::cmdUnknown, // ShowFace
	&ScriptInterpreter::cmdUnknown, // HideFace
	&ScriptInterpreter::cmdLet,
	&ScriptInterpreter::cmdIf,
	&ScriptInterpreter::cmdUnknown, // ShowAnimaFrame
	&ScriptInterpreter::cmdUnknown, // POverE
	&ScriptInterpreter::cmdEffect,
	&ScriptInterpreter::cmdUnknown, // EffectVolume
	&ScriptInterpreter::cmdUnknown, // TalkRandom
	&ScriptInterpreter::cmdUnknown, // Take
	&ScriptInterpreter::cmdUnknown, // RunFilmP
	&ScriptInterpreter::cmdUnknown, // Extra
	&ScriptInterpreter::cmdUnknown, // MoveAR: unsed
	&ScriptInterpreter::cmdUnknown, // SetFrame
	&ScriptInterpreter::cmdUnknown, // NoHave
	&ScriptInterpreter::cmdUnknown, // HideE
	&ScriptInterpreter::cmdUnknown, // ShowE
	&ScriptInterpreter::cmdUnknown, // SetShiftX
	&ScriptInterpreter::cmdUnknown, // SetPositionE
	&ScriptInterpreter::cmdUnknown, // SetPositionEV
	&ScriptInterpreter::cmdUnknown, // IfMouseXM
	&ScriptInterpreter::cmdUnknown, // IfMouseXW
	&ScriptInterpreter::cmdUnknown, // ScrollLeft
	&ScriptInterpreter::cmdUnknown, // ScrollRight
	&ScriptInterpreter::cmdUnknown, // SetScrollX1
	&ScriptInterpreter::cmdUnknown, // SetScrollX2
	&ScriptInterpreter::cmdUnknown, // SetPosXY
	&ScriptInterpreter::cmdUnknown, // SetChkPrior
	&ScriptInterpreter::cmdUnknown, // Brightness
	&ScriptInterpreter::cmdUnknown, // SetPerspY
	&ScriptInterpreter::cmdUnknown, // MoveP
	&ScriptInterpreter::cmdUnknown, // StopAnimaFrame: unused
	&ScriptInterpreter::cmdUnknown, // SetDialog
	&ScriptInterpreter::cmdUnknown, // StopAnimaNeg
	&ScriptInterpreter::cmdUnknown, // SetAnimaFrame
	&ScriptInterpreter::cmdUnknown, // DeactiveSelf
	&ScriptInterpreter::cmdUnknown, // EOverE
	&ScriptInterpreter::cmdMusic,
	&ScriptInterpreter::cmdUnknown, // MoveE
	&ScriptInterpreter::cmdUnknown, // BrightnessE
	&ScriptInterpreter::cmdUnknown, // WalkEffect
	&ScriptInterpreter::cmdUnknown, // HaveGun
	&ScriptInterpreter::cmdUnknown, // NoHaveGun
	&ScriptInterpreter::cmdUnknown, // ActiveMacroAt
	&ScriptInterpreter::cmdUnknown, // HaveTool
	&ScriptInterpreter::cmdUnknown, // NoHaveTool
	&ScriptInterpreter::cmdInc,
	&ScriptInterpreter::cmdUnknown, // WaitWhile
	&ScriptInterpreter::cmdUnknown, // Stay
	&ScriptInterpreter::cmdUnknown, // GameOver: unused
	&ScriptInterpreter::cmdUnknown, // MoveEP
	&ScriptInterpreter::cmdUnknown, // WaitWhileAnima
	&ScriptInterpreter::cmdUnknown, // ShowFrameDir: unused
	&ScriptInterpreter::cmdUnknown, // StayDef
	&ScriptInterpreter::cmdUnknown, // UnLock
	&ScriptInterpreter::cmdUnknown, // CursorAs: unused
	&ScriptInterpreter::cmdUnknown, // AnimaEffect
	&ScriptInterpreter::cmdUnknown, // SetArea
	&ScriptInterpreter::cmdUnknown, // SetSpecArea
	&ScriptInterpreter::cmdUnknown, // WaitToAnimaFrame
	&ScriptInterpreter::cmdUnknown, // ScrollMarginL
	&ScriptInterpreter::cmdUnknown, // ScrollMarginR
	&ScriptInterpreter::cmdUnknown, // OutMacro
	&ScriptInterpreter::cmdUnknown, // NormalMacro
	&ScriptInterpreter::cmdIff,
	&ScriptInterpreter::cmdUnknown, // AutoIncRange
	&ScriptInterpreter::cmdUnknown, // Lock
	&ScriptInterpreter::cmdUnknown, // HideFaceAt
	&ScriptInterpreter::cmdUnknown, // Include: unused
	&ScriptInterpreter::cmdUnknown, // LookTo
	&ScriptInterpreter::cmdUnknown, // GetPersonX
	&ScriptInterpreter::cmdUnknown, // GetPersonY
	&ScriptInterpreter::cmdIncc,
	&ScriptInterpreter::cmdUnknown, // ContinueAnima
	&ScriptInterpreter::cmdUnknown, // LockWalk
	&ScriptInterpreter::cmdUnknown, // UnLockWalk
	&ScriptInterpreter::cmdUnknown, // WaitWhileEffect
	&ScriptInterpreter::cmdUnknown, // SetDarknessAnima
	&ScriptInterpreter::cmdUnknown, // SetDirWalk
	&ScriptInterpreter::cmdUnknown, // Message: unused
	&ScriptInterpreter::cmdUnknown, // PointRotate
	&ScriptInterpreter::cmdUnknown, // RunGlobalFilmP
	&ScriptInterpreter::cmdUnknown, // DoNothing
	&ScriptInterpreter::cmdUnknown, // Slap: unused
	&ScriptInterpreter::cmdUnknown, // Shoot: unused
	&ScriptInterpreter::cmdUnknown, // ActiveMacroAnima
	&ScriptInterpreter::cmdUnknown, // Pause: unused
	&ScriptInterpreter::cmdUnknown, // ExitGame: unused
	&ScriptInterpreter::cmdUnknown, // RotateTo
	&ScriptInterpreter::cmdUnknown, // MacroDestructor
	&ScriptInterpreter::cmdUnknown, // BlockMacro
	&ScriptInterpreter::cmdUnknown, // UnBlockMacro
	&ScriptInterpreter::cmdUnknown, // PinArea
	&ScriptInterpreter::cmdUnknown, // AnimaDelay
	&ScriptInterpreter::cmdUnknown, // Random
	&ScriptInterpreter::cmdUnknown, // DeactiveMacro
	&ScriptInterpreter::cmdUnknown, // Mixing: unused
	&ScriptInterpreter::cmdUnknown, // RefreshScreen
	&ScriptInterpreter::cmdUnknown, // ClearAnimaBuffer
	&ScriptInterpreter::cmdRunAvx, // RunAvx
	&ScriptInterpreter::cmdUnknown, // JackTempo
	&ScriptInterpreter::cmdUnknown, // ClearMenuBar
	&ScriptInterpreter::cmdUnknown, // UnLockCanal: unused
	&ScriptInterpreter::cmdUnknown, // PointInSecArea
	&ScriptInterpreter::cmdUnknown, // TalkRandomIndex
	&ScriptInterpreter::cmdUnknown, // MouseToTool
	&ScriptInterpreter::cmdUnknown, // RelativeMulP
	&ScriptInterpreter::cmdUnknown, // MinShadow
	&ScriptInterpreter::cmdUnknown, // MaxShadow
	&ScriptInterpreter::cmdUnknown, // PutShadow
	&ScriptInterpreter::cmdUnknown, // AnimaShadow: unused
	&ScriptInterpreter::cmdUnknown, // ShiftShadowP
	&ScriptInterpreter::cmdUnknown, // Pickup
	&ScriptInterpreter::cmdUnknown, // MusicVolume
	&ScriptInterpreter::cmdUnknown, // Give
	&ScriptInterpreter::cmdUnknown, // SlapF
	&ScriptInterpreter::cmdUnknown, // ShootF
	&ScriptInterpreter::cmdUnknown, // ReactiveSelf
	&ScriptInterpreter::cmdUnknown, // SetPosScroll
	&ScriptInterpreter::cmdUnknown, // ChkDistance
	&ScriptInterpreter::cmdUnknown, // PointInWalkArea
	&ScriptInterpreter::cmdUnknown, // SetScroll
	&ScriptInterpreter::cmdUnknown, // SetSpecShadow
	&ScriptInterpreter::cmdUnknown, // ShowShadow
	&ScriptInterpreter::cmdUnknown, // ShadowStatus
	&ScriptInterpreter::cmdUnknown, // HideShadow
	&ScriptInterpreter::cmdUnknown, // ShadowOnE
	&ScriptInterpreter::cmdUnknown, // ItemPresent
	&ScriptInterpreter::cmdUnknown, // IgnoreArea
	&ScriptInterpreter::cmdUnknown, // SaveAble
	&ScriptInterpreter::cmdUnknown, // ExitToDos: unused
	&ScriptInterpreter::cmdUnknown, // AnyKey: unused
};

ScriptInterpreter::ScriptInterpreter(OrlandoEngine *vm) : _vm(vm) {
}

bool ScriptInterpreter::runCommand(Macro *macro, const MacroCommand &cmd) {
	debug("ScriptInterpreter: Executing [%s] %s", macro->getId().c_str(), cmd.args[0].c_str());
	ScriptHandler handler = kCommandHandlers[cmd.type];
	return (this->*handler)(macro, cmd);
}

int ScriptInterpreter::varOrLiteral(const Common::String &arg) const {
	if (arg.firstChar() == '*') {
		Common::String var = arg;
		var.deleteChar(0);
		return _vm->getVariable(var);
	} else {
		return toInt(arg);
	}
}

bool ScriptInterpreter::cmdUnknown(Macro *macro, const MacroCommand &cmd) {
	warning("ScriptInterpreter: Unknown command %s", cmd.args[0].c_str());
	return true;
}

bool ScriptInterpreter::cmdAnima(Macro *macro, const MacroCommand &cmd) {
	Common::String anim = cmd.args[1];
	bool reverse = (cmd.args[2] == "1");
	int delay = toInt(cmd.args[3]) * 1000 / 60;
	PlayMode mode = (PlayMode)toInt(cmd.args[4]);
	int rec = toInt(cmd.args[5]);
	// TODO: + -

	_vm->getScene()->getElement(anim)->getAnimation()->play(reverse, delay, mode, rec, _vm->getTotalPlayTime());
	return true;
}

bool ScriptInterpreter::cmdEndIf(Macro *macro, const MacroCommand &cmd) {
	return true;
}

bool ScriptInterpreter::cmdGoToScene(Macro *macro, const MacroCommand &cmd) {
	Common::String scene = cmd.args[1];
	// Remove extension if any
	if (scene.hasSuffix(".CFG")) {
		scene.erase(scene.size() - 4);
	}

	_vm->gotoScene(new Scene(_vm, scene));
	return true;
}

bool ScriptInterpreter::cmdLet(Macro *macro, const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = varOrLiteral(cmd.args[2]);

	_vm->getVariable(var) = value;
	return true;
}

bool ScriptInterpreter::cmdIf(Macro *macro, const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = toInt(cmd.args[2]);

	bool condition = (_vm->getVariable(var) == value);
	if (condition) {
		return true;
	} else {
		macro->skipIf();
		return false;
	}
}

bool ScriptInterpreter::cmdEffect(Macro *macro, const MacroCommand &cmd) {
	Common::String effect = cmd.args[1];
	// TODO: Extra arguments

	Common::String filename = _vm->getSoundManager()->getSfx(effect);
	Common::File *audio = _vm->getScene()->loadFile(filename);
	_vm->getSoundManager()->playFile(audio, Audio::Mixer::kSFXSoundType);
	return true;
}

bool ScriptInterpreter::cmdMusic(Macro *macro, const MacroCommand &cmd) {
	Common::String music = cmd.args[1];
	// TODO: Extra arguments

	_vm->getSoundManager()->playMusic(music);
	return true;
}

bool ScriptInterpreter::cmdInc(Macro *macro, const MacroCommand &cmd) {
	Common::String var = cmd.args[1];

	_vm->getVariable(var)++;
	return true;
}

bool ScriptInterpreter::cmdIff(Macro *macro, const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	Common::String op = cmd.args[2];
	int value = toInt(cmd.args[3]);

	bool condition = false;
	if (op == "<")
		condition = (_vm->getVariable(var) < value);
	else if (op == ">")
		condition = (_vm->getVariable(var) > value);

	if (condition) {
		return true;
	} else {
		macro->skipIf();
		return false;
	}
}

bool ScriptInterpreter::cmdIncc(Macro *macro, const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = toInt(cmd.args[2]);

	_vm->getVariable(var) += value;
	return true;
}

bool ScriptInterpreter::cmdRunAvx(Macro *macro, const MacroCommand &cmd) {
	Common::String avx = cmd.args[1];
	// Remove extension if any
	if (avx.hasSuffix(".AVX")) {
		avx.erase(avx.size() - 4);
	}
	avx += "16";

	_vm->gotoScene(new AvxVideo(_vm, avx));
	return true;
}

} // End of namespace Orlando
