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
#include "common/util.h"

#include "orlando/interp.h"
#include "orlando/util.h"
#include "orlando/orlando.h"
#include "orlando/scene.h"
#include "orlando/element.h"
#include "orlando/animation.h"
#include "orlando/sound.h"
#include "orlando/avx_video.h"
#include "orlando/person.h"
#include "orlando/dialog.h"
#include "orlando/mouse.h"
#include "orlando/insertion.h"
#include "orlando/window.h"
#include "orlando/jack.h"

namespace Orlando {

const ScriptHandler ScriptInterpreter::kCommandHandlers[] = {
	&ScriptInterpreter::cmdUnknown,
	&ScriptInterpreter::cmdChWindow,
	&ScriptInterpreter::cmdSetPosition,
	&ScriptInterpreter::cmdInitFirst,
	&ScriptInterpreter::cmdSetPersonData,
	&ScriptInterpreter::cmdAnima,
	&ScriptInterpreter::cmdStopAnima,
	&ScriptInterpreter::cmdWalkTo,
	&ScriptInterpreter::cmdRunInsertion,
	&ScriptInterpreter::cmdUnknown, // RunFilm: unused
	&ScriptInterpreter::cmdHide,
	&ScriptInterpreter::cmdTalk,
	&ScriptInterpreter::cmdDialog,
	&ScriptInterpreter::cmdActiveMacro,
	&ScriptInterpreter::cmdIfAnswer,
	&ScriptInterpreter::cmdEndIf,
	&ScriptInterpreter::cmdUnknown, // ShowFrame: unused
	&ScriptInterpreter::cmdUnknown, // HideTalk: unused
	&ScriptInterpreter::cmdUnknown, // ShadowBy
	&ScriptInterpreter::cmdUnknown, // SetShadow
	&ScriptInterpreter::cmdGoToScene,
	&ScriptInterpreter::cmdPUnderP,
	&ScriptInterpreter::cmdPUnderE,
	&ScriptInterpreter::cmdWalkToDir,
	&ScriptInterpreter::cmdUnknown, // ShowFace
	&ScriptInterpreter::cmdUnknown, // HideFace
	&ScriptInterpreter::cmdLet,
	&ScriptInterpreter::cmdIf,
	&ScriptInterpreter::cmdUnknown, // ShowAnimaFrame
	&ScriptInterpreter::cmdPOverE,
	&ScriptInterpreter::cmdEffect,
	&ScriptInterpreter::cmdUnknown, // EffectVolume
	&ScriptInterpreter::cmdUnknown, // TalkRandom
	&ScriptInterpreter::cmdUnknown, // Take
	&ScriptInterpreter::cmdUnknown, // RunFilmP
	&ScriptInterpreter::cmdUnknown, // Extra
	&ScriptInterpreter::cmdUnknown, // MoveAR: unused
	&ScriptInterpreter::cmdUnknown, // SetFrame
	&ScriptInterpreter::cmdUnknown, // NoHave
	&ScriptInterpreter::cmdHideE,
	&ScriptInterpreter::cmdShowE,
	&ScriptInterpreter::cmdUnknown, // SetShiftX
	&ScriptInterpreter::cmdSetPositionE,
	&ScriptInterpreter::cmdSetPositionE, // TODO: SetPositionEV
	&ScriptInterpreter::cmdUnknown, // IfMouseXM
	&ScriptInterpreter::cmdUnknown, // IfMouseXW
	&ScriptInterpreter::cmdUnknown, // ScrollLeft
	&ScriptInterpreter::cmdUnknown, // ScrollRight
	&ScriptInterpreter::cmdUnknown, // SetScrollX1
	&ScriptInterpreter::cmdUnknown, // SetScrollX2
	&ScriptInterpreter::cmdUnknown, // SetPosXY
	&ScriptInterpreter::cmdUnknown, // SetChkPrior
	&ScriptInterpreter::cmdUnknown, // Brightness
	&ScriptInterpreter::cmdSetPerspY,
	&ScriptInterpreter::cmdMoveP,
	&ScriptInterpreter::cmdUnknown, // StopAnimaFrame: unused
	&ScriptInterpreter::cmdSetDialog,
	&ScriptInterpreter::cmdUnknown, // StopAnimaNeg
	&ScriptInterpreter::cmdUnknown, // SetAnimaFrame
	&ScriptInterpreter::cmdDeactiveSelf,
	&ScriptInterpreter::cmdEOverE,
	&ScriptInterpreter::cmdMusic,
	&ScriptInterpreter::cmdMoveE,
	&ScriptInterpreter::cmdUnknown, // BrightnessE
	&ScriptInterpreter::cmdUnknown, // WalkEffect
	&ScriptInterpreter::cmdUnknown, // HaveGun
	&ScriptInterpreter::cmdUnknown, // NoHaveGun
	&ScriptInterpreter::cmdActiveMacro, // TODO: ActiveMacroAt
	&ScriptInterpreter::cmdUnknown, // HaveTool
	&ScriptInterpreter::cmdUnknown, // NoHaveTool
	&ScriptInterpreter::cmdInc,
	&ScriptInterpreter::cmdWaitWhile,
	&ScriptInterpreter::cmdStay,
	&ScriptInterpreter::cmdUnknown, // GameOver: unused
	&ScriptInterpreter::cmdMoveE, // TODO: MoveEP
	&ScriptInterpreter::cmdUnknown, // WaitWhileAnima
	&ScriptInterpreter::cmdUnknown, // ShowFrameDir: unused
	&ScriptInterpreter::cmdStayDef,
	&ScriptInterpreter::cmdUnLock,
	&ScriptInterpreter::cmdUnknown, // CursorAs: unused
	&ScriptInterpreter::cmdUnknown, // AnimaEffect
	&ScriptInterpreter::cmdUnknown, // SetArea
	&ScriptInterpreter::cmdUnknown, // SetSpecArea
	&ScriptInterpreter::cmdUnknown, // WaitToAnimaFrame
	&ScriptInterpreter::cmdUnknown, // ScrollMarginL
	&ScriptInterpreter::cmdUnknown, // ScrollMarginR
	&ScriptInterpreter::cmdOutMacro,
	&ScriptInterpreter::cmdNormalMacro,
	&ScriptInterpreter::cmdIff,
	&ScriptInterpreter::cmdUnknown, // AutoIncRange
	&ScriptInterpreter::cmdLock,
	&ScriptInterpreter::cmdUnknown, // HideFaceAt
	&ScriptInterpreter::cmdUnknown, // Include: unused
	&ScriptInterpreter::cmdUnknown, // LookTo
	&ScriptInterpreter::cmdGetPersonX,
	&ScriptInterpreter::cmdGetPersonY,
	&ScriptInterpreter::cmdIncc,
	&ScriptInterpreter::cmdContinueAnima,
	&ScriptInterpreter::cmdUnknown, // LockWalk
	&ScriptInterpreter::cmdUnknown, // UnLockWalk
	&ScriptInterpreter::cmdUnknown, // WaitWhileEffect
	&ScriptInterpreter::cmdUnknown, // SetDarknessAnima
	&ScriptInterpreter::cmdSetDirWalk,
	&ScriptInterpreter::cmdUnknown, // Message: unused
	&ScriptInterpreter::cmdUnknown, // PointRotate
	&ScriptInterpreter::cmdUnknown, // RunGlobalFilmP
	&ScriptInterpreter::cmdDoNothing,
	&ScriptInterpreter::cmdUnknown, // Slap: unused
	&ScriptInterpreter::cmdUnknown, // Shoot: unused
	&ScriptInterpreter::cmdActiveMacro, // TODO: ActiveMacroAnima
	&ScriptInterpreter::cmdUnknown, // Pause: unused
	&ScriptInterpreter::cmdUnknown, // ExitGame: unused
	&ScriptInterpreter::cmdUnknown, // RotateTo
	&ScriptInterpreter::cmdUnknown, // MacroDestructor
	&ScriptInterpreter::cmdUnknown, // BlockMacro
	&ScriptInterpreter::cmdUnknown, // UnBlockMacro
	&ScriptInterpreter::cmdUnknown, // PinArea
	&ScriptInterpreter::cmdUnknown, // AnimaDelay
	&ScriptInterpreter::cmdUnknown, // Random
	&ScriptInterpreter::cmdDeactiveMacro,
	&ScriptInterpreter::cmdUnknown, // Mixing: unused
	&ScriptInterpreter::cmdUnknown, // RefreshScreen
	&ScriptInterpreter::cmdUnknown, // ClearAnimaBuffer
	&ScriptInterpreter::cmdRunAvx,
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
	&ScriptInterpreter::cmdReactiveSelf,
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

ScriptInterpreter::ScriptInterpreter(OrlandoEngine *vm) : _vm(vm), _macro(nullptr), _time(0) {
}

bool ScriptInterpreter::runCommand(Macro *macro, const MacroCommand &cmd, uint32 time) {
	_macro = macro;
	_time = time;
	ScriptHandler handler = kCommandHandlers[cmd.type];
	bool done = (this->*handler)(cmd);
	if (done)
		debug("ScriptInterpreter: Executing [%s] %s", macro->getId().c_str(), cmd.args[0].c_str());
	return done;
}

int ScriptInterpreter::getVarOrLiteral(const Common::String &arg) const {
	if (arg.firstChar() == '*') {
		Common::String var = arg;
		var.deleteChar(0);
		return _vm->getVariable(var);
	} else {
		return toInt(arg);
	}
}

bool ScriptInterpreter::waitUntilComplete(const MacroCommand &cmd) const {
	return (cmd.args.back() == "-");
}

bool ScriptInterpreter::cmdUnknown(const MacroCommand &cmd) {
	warning("ScriptInterpreter: Unknown command %s", cmd.args[0].c_str());
	return true;
}

bool ScriptInterpreter::cmdChWindow(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];
	int x1 = toInt(cmd.args[2]);
	int y1 = toInt(cmd.args[3]);
	int x2 = toInt(cmd.args[4]);
	int y2 = toInt(cmd.args[5]);
	Common::Rect window(x1, y1, x2, y2);

	Person *person = _vm->getScene()->getPerson(id);
	if (person != nullptr) {
		person->getWindow()->resize(window);
	} else {
		Element *element = _vm->getScene()->getElement(id);
		if (element != nullptr) {
			element->getWindow()->resize(window);
		} else {
			warning("ScriptInterpreter: Window %s not found", id.c_str());
		}
	}
	return true;
}

bool ScriptInterpreter::cmdSetPosition(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	int x = getVarOrLiteral(cmd.args[2]);
	int y = getVarOrLiteral(cmd.args[3]);

	_vm->getScene()->getPerson(person)->setPosition(Vector2(x, y));
	return true;
}

bool ScriptInterpreter::cmdInitFirst(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	Common::String insertion = cmd.args[2];
	bool flip = toInt(cmd.args[3]);

	Insertion *ins = _vm->getScene()->getInsertion(insertion);
	Person *p = _vm->getScene()->getPerson(person);
	p->setInsertion(ins);
	ins->init(false, flip, _time, p);
	return true;
}

bool ScriptInterpreter::cmdSetPersonData(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];
	int delay = ticksToMs(toInt(cmd.args[2]));
	float scale = toFloat(cmd.args[3]);
	int perspY = toInt(cmd.args[4]);
	int walkSpeed = toInt(cmd.args[5]);

	_vm->getScene()->getPerson(id)->setData(delay, scale, perspY, walkSpeed);

	return true;
}

bool ScriptInterpreter::cmdAnima(const MacroCommand &cmd) {
	Common::String anim = cmd.args[1];
	bool reverse = (cmd.args[2] == "1");
	int delay = ticksToMs(toInt(cmd.args[3]));
	PlayMode mode = (PlayMode)toInt(cmd.args[4]);
	int rec = toInt(cmd.args[5]);
	bool wait = waitUntilComplete(cmd);

	Animation *anima = _vm->getScene()->getElement(anim)->getAnimation();
	if (!_macro->isWaiting()) {
		anima->play(reverse, delay, mode, rec, _time);
		_macro->setWaiting(wait);
		return !wait;
	} else {
		if (anima->isPlaying()) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdStopAnima(const MacroCommand &cmd) {
	Common::String anim = cmd.args[1];

	_vm->getScene()->getElement(anim)->getAnimation()->setPlaying(false);
	return true;
}

bool ScriptInterpreter::cmdWalkTo(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];
	int x = getVarOrLiteral(cmd.args[2]);
	int y = getVarOrLiteral(cmd.args[3]);
	bool wait = waitUntilComplete(cmd);

	Person *person = _vm->getScene()->getPerson(id);
	if (!_macro->isWaiting()) {
		person->walkTo(Common::Point(x, y), _time);
		_macro->setWaiting(wait);
		return !wait;
	} else {
		if (person->isWalking()) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdRunInsertion(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	Common::String insertion = cmd.args[2];
	bool flip = toInt(cmd.args[4]);
	bool wait = waitUntilComplete(cmd);
	// TODO: Extra arguments

	Insertion *ins = _vm->getScene()->getInsertion(insertion);
	Person *p = _vm->getScene()->getPerson(person);
	if (!_macro->isWaiting()) {
		p->setInsertion(ins);
		ins->init(true, flip, _time, p);
		_macro->setWaiting(wait);
		return !wait;
	} else {
		if (ins->isPlaying()) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
	return true;
}

bool ScriptInterpreter::cmdHide(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];

	_vm->getScene()->getPerson(person)->getWindow()->setVisible(false);
	return true;
}

bool ScriptInterpreter::cmdTalk(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	int id = toInt(cmd.args[2]);
	// TODO: Show text
	int color = toInt(cmd.args[3]);
	bool wait = waitUntilComplete(cmd);

	Dialog *dialog = _vm->getScene()->getDialog(id);
	if (!_macro->isWaiting()) {
		dialog->talk(_vm);
		_macro->setWaiting(wait);
		return !wait;
	} else {
		// Skip dialog
		if (_vm->getMouse()->getLeftButton() == kButtonReleased) {
			dialog->skip(_vm);
		}
		if (dialog->isTalking(_vm)) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdDialog(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	int id = toInt(cmd.args[2]);
	// TODO: Show text

	Dialog *dialog = _vm->getScene()->getDialog(id);
	if (!_macro->isWaiting()) {
		dialog->talk(_vm);
		_macro->setWaiting(true);
		return false;
	} else {
		// Skip dialog
		if (_vm->getMouse()->getLeftButton() == kButtonReleased) {
			dialog->skip(_vm);
		}
		if (dialog->isTalking(_vm)) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdActiveMacro(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];

	_vm->getScene()->getMacro(id)->setActive(true);
	return true;
}

bool ScriptInterpreter::cmdIfAnswer(const MacroCommand &cmd) {
	int dialog = toInt(cmd.args[1]);
	int value = toInt(cmd.args[2]);

	bool condition = (_vm->getScene()->getDialog(dialog)->getAnswer() == value);
	if (condition) {
		return true;
	} else {
		_macro->skipIf();
		return false;
	}
}

bool ScriptInterpreter::cmdEndIf(const MacroCommand &cmd) {
	return true;
}

bool ScriptInterpreter::cmdGoToScene(const MacroCommand &cmd) {
	Common::String scene = cmd.args[1];
	// Remove extension if any
	if (scene.hasSuffix(".CFG")) {
		scene.erase(scene.size() - 4);
	}

	_vm->gotoScene(new Scene(_vm, scene));
	return true;
}

bool ScriptInterpreter::cmdPUnderP(const MacroCommand &cmd) {
	Common::String id1 = cmd.args[1];
	Common::String id2 = cmd.args[2];
	Person *person1 = _vm->getScene()->getPerson(id1);
	Person *person2 = _vm->getScene()->getPerson(id2);

	_vm->getScene()->moveWindowUnder(person1->getWindow(), person2->getWindow());
	return true;
}

bool ScriptInterpreter::cmdPUnderE(const MacroCommand &cmd) {
	Common::String id1 = cmd.args[1];
	Common::String id2 = cmd.args[2];
	Person *person = _vm->getScene()->getPerson(id1);
	Element *element = _vm->getScene()->getElement(id2);

	_vm->getScene()->moveWindowUnder(person->getWindow(), element->getWindow());
	return true;
}

bool ScriptInterpreter::cmdWalkToDir(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];
	int x = getVarOrLiteral(cmd.args[2]);
	int y = getVarOrLiteral(cmd.args[3]);
	int dir = toInt(cmd.args[4]);
	bool wait = waitUntilComplete(cmd);

	Person *person = _vm->getScene()->getPerson(id);
	if (!_macro->isWaiting()) {
		person->walkTo(Common::Point(x, y), _time, dir);
		_macro->setWaiting(wait);
		return !wait;
	} else {
		if (person->isWalking()) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdLet(const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = getVarOrLiteral(cmd.args[2]);

	_vm->getVariable(var) = value;
	return true;
}

bool ScriptInterpreter::cmdIf(const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = toInt(cmd.args[2]);

	bool condition = (_vm->getVariable(var) == value);
	if (condition) {
		return true;
	} else {
		_macro->skipIf();
		return false;
	}
}

bool ScriptInterpreter::cmdPOverE(const MacroCommand &cmd) {
	Common::String id1 = cmd.args[1];
	Common::String id2 = cmd.args[2];
	Person *person = _vm->getScene()->getPerson(id1);
	Element *element = _vm->getScene()->getElement(id2);

	_vm->getScene()->moveWindowOver(person->getWindow(), element->getWindow());
	return true;
}

bool ScriptInterpreter::cmdEffect(const MacroCommand &cmd) {
	Common::String effect = cmd.args[1];
	// TODO: Extra arguments

	Common::String filename = _vm->getSoundManager()->getSfx(effect);
	Common::File *audio = _vm->getScene()->loadFile(filename);
	_vm->getSoundManager()->playFile(audio, Audio::Mixer::kSFXSoundType);
	return true;
}

bool ScriptInterpreter::cmdHideE(const MacroCommand &cmd) {
	Common::String element = cmd.args[1];

	_vm->getScene()->getElement(element)->getWindow()->setVisible(false);
	return true;
}

bool ScriptInterpreter::cmdShowE(const MacroCommand &cmd) {
	Common::String element = cmd.args[1];

	_vm->getScene()->getElement(element)->getWindow()->setVisible(true);
	return true;
}

bool ScriptInterpreter::cmdSetPositionE(const MacroCommand &cmd) {
	Common::String element = cmd.args[1];
	int x = toInt(cmd.args[2]);
	int y = toInt(cmd.args[3]);

	_vm->getScene()->getElement(element)->setPosition(Common::Point(x, y));
	return true;
}

bool ScriptInterpreter::cmdSetPerspY(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	int y = toInt(cmd.args[2]);

	_vm->getScene()->getPerson(person)->setPerspectiveYMin(y);
	return true;
}

bool ScriptInterpreter::cmdSetDialog(const MacroCommand &cmd) {
	int dialog = toInt(cmd.args[1]);
	int option = toInt(cmd.args[2]);
	bool enabled = toInt(cmd.args[3]) != 0;

	_vm->getScene()->getDialog(dialog)->setOption(option, enabled);
	return true;
}

bool ScriptInterpreter::cmdMoveP(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	int x = toInt(cmd.args[2]);
	int y = toInt(cmd.args[3]);

	Person *p = _vm->getScene()->getPerson(person);
	p->setPosition(p->getPosition() + Vector2(x, y));
	return true;
}

bool ScriptInterpreter::cmdDeactiveSelf(const MacroCommand &cmd) {
	_macro->setActive(false);
	return true;
}

bool ScriptInterpreter::cmdEOverE(const MacroCommand &cmd) {
	Common::String id1 = cmd.args[1];
	Common::String id2 = cmd.args[2];
	Element *element1 = _vm->getScene()->getElement(id1);
	Element *element2 = _vm->getScene()->getElement(id2);

	_vm->getScene()->moveWindowOver(element1->getWindow(), element2->getWindow());
	return true;
}

bool ScriptInterpreter::cmdMoveE(const MacroCommand &cmd) {
	Common::String element = cmd.args[1];
	int x = toInt(cmd.args[2]);
	int y = toInt(cmd.args[3]);

	Element *e = _vm->getScene()->getElement(element);
	e->setPosition(e->getPosition() + Common::Point(x, y));
	return true;
}

bool ScriptInterpreter::cmdMusic(const MacroCommand &cmd) {
	Common::String music = cmd.args[1];
	// TODO: Extra arguments

	_vm->getSoundManager()->playMusic(music);
	return true;
}

bool ScriptInterpreter::cmdInc(const MacroCommand &cmd) {
	Common::String var = cmd.args[1];

	_vm->getVariable(var)++;
	return true;
}

bool ScriptInterpreter::cmdWaitWhile(const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = toInt(cmd.args[2]);

	if (_vm->getVariable(var) == value)
		return false;
	else
		return true;
}

bool ScriptInterpreter::cmdStay(const MacroCommand &cmd) {
	const char *kDirStrings[] = { "D", "LD", "L", "LU", "U", "RU", "R", "RD" };
	Common::String dir = cmd.args[1];
	if (dir.lastChar() == '*') {
		// TODO: What does this do
		dir.deleteLastChar();
	}

	int newDir = kDirectionNone;
	for (int i = 0; i < ARRAYSIZE(kDirStrings); i++) {
		if (dir == kDirStrings[i]) {
			newDir = i;
			break;
		}
	}
	_vm->getJack()->stay(newDir);
	return true;
}

bool ScriptInterpreter::cmdStayDef(const MacroCommand &cmd) {
	_vm->getJack()->stay();
	return true;
}

bool ScriptInterpreter::cmdUnLock(const MacroCommand &cmd) {
	_macro->setLocked(false);
	return true;
}

bool ScriptInterpreter::cmdOutMacro(const MacroCommand &cmd) {
	_macro->setOut(true);
	return true;
}

bool ScriptInterpreter::cmdNormalMacro(const MacroCommand &cmd) {
	_macro->setOut(false);
	return true;
}

bool ScriptInterpreter::cmdIff(const MacroCommand &cmd) {
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
		_macro->skipIf();
		return false;
	}
}

bool ScriptInterpreter::cmdLock(const MacroCommand &cmd) {
	_macro->setLocked(true);
	return true;
}

bool ScriptInterpreter::cmdGetPersonX(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	Common::String var = cmd.args[2];

	_vm->getVariable(var) = _vm->getScene()->getPerson(person)->getPosition().x;
	return true;
}

bool ScriptInterpreter::cmdGetPersonY(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	Common::String var = cmd.args[2];

	_vm->getVariable(var) = _vm->getScene()->getPerson(person)->getPosition().y;
	return true;
}

bool ScriptInterpreter::cmdIncc(const MacroCommand &cmd) {
	Common::String var = cmd.args[1];
	int value = toInt(cmd.args[2]);

	_vm->getVariable(var) += value;
	return true;
}

bool ScriptInterpreter::cmdContinueAnima(const MacroCommand &cmd) {
	Common::String anim = cmd.args[1];

	_vm->getScene()->getElement(anim)->getAnimation()->setPlaying(true);
	return true;
}

bool ScriptInterpreter::cmdSetDirWalk(const MacroCommand &cmd) {
	Common::String person = cmd.args[1];
	bool invert = toInt(cmd.args[2]);

	_vm->getScene()->getPerson(person)->setDirWalk(invert);
	return true;
}

bool ScriptInterpreter::cmdDoNothing(const MacroCommand &cmd) {
	uint32 delay = ticksToMs(toInt(cmd.args[1]));

	if (!_macro->isWaiting()) {
		_macro->setTime(_time + delay);
		_macro->setWaiting(true);
		return false;
	} else {
		if (_time < _macro->getTime()) {
			return false;
		} else {
			_macro->setWaiting(false);
			return true;
		}
	}
}

bool ScriptInterpreter::cmdDeactiveMacro(const MacroCommand &cmd) {
	Common::String id = cmd.args[1];

	_vm->getScene()->getMacro(id)->setActive(false);
	return true;
}

bool ScriptInterpreter::cmdRunAvx(const MacroCommand &cmd) {
	Common::String avx = cmd.args[1];
	// Remove extension if any
	if (avx.hasSuffix(".AVX")) {
		avx.erase(avx.size() - 4);
	}
	avx += "16";

	_vm->gotoScene(new AvxVideo(_vm, avx));
	return true;
}

bool ScriptInterpreter::cmdReactiveSelf(const MacroCommand &cmd) {
	_macro->reset();
	return true;
}

} // End of namespace Orlando
