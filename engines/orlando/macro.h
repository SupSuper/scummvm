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

#ifndef ORLANDO_MACRO_H
#define ORLANDO_MACRO_H

#include "common/str-array.h"

namespace Orlando {

class TextParser;
class ScriptInterpreter;

enum CommandType {
	kCmdUnknown = 0,
	kCmdChWindow,
	kCmdSetPosition,
	kCmdInitFirst,
	kCmdSetPersonData,
	kCmdAnima,
	kCmdStopAnima,
	kCmdWalkTo,
	kCmdRunInsertion,
	kCmdRunFilm,
	kCmdHide,
	kCmdTalk,
	kCmdDialog,
	kCmdActiveMacro,
	kCmdIfAnswer,
	kCmdEndIf,
	kCmdShowFrame,
	kCmdHideTalk,
	kCmdShadowBy,
	kCmdSetShadow,
	kCmdGoToScene,
	kCmdPUnderP,
	kCmdPUnderE,
	kCmdWalkToDir,
	kCmdShowFace,
	kCmdHideFace,
	kCmdLet,
	kCmdIf,
	kCmdShowAnimaFrame,
	kCmdPOverE,
	kCmdEffect,
	kCmdEffectVolume,
	kCmdTalkRandom,
	kCmdTake,
	kCmdRunFilmP,
	kCmdExtra,
	kCmdMoveAR,
	kCmdSetFrame,
	kCmdNoHave,
	kCmdHideE,
	kCmdShowE,
	kCmdSetShiftX,
	kCmdSetPositionE,
	kCmdSetPositionEV,
	kCmdIfMouseXM,
	kCmdIfMouseXW,
	kCmdScrollLeft,
	kCmdScrollRight,
	kCmdSetScrollX1,
	kCmdSetScrollX2,
	kCmdSetPosXY,
	kCmdSetChkPrior,
	kCmdBrightness,
	kCmdSetPerspY,
	kCmdMoveP,
	kCmdStopAnimaFrame,
	kCmdSetDialog,
	kCmdStopAnimaNeg,
	kCmdSetAnimaFrame,
	kCmdDeactiveSelf,
	kCmdEOverE,
	kCmdMusic,
	kCmdMoveE,
	kCmdBrightnessE,
	kCmdWalkEffect,
	kCmdHaveGun,
	kCmdNoHaveGun,
	kCmdActiveMacroAt,
	kCmdHaveTool,
	kCmdNoHaveTool,
	kCmdInc,
	kCmdWaitWhile,
	kCmdStay,
	kCmdGameOver,
	kCmdMoveEP,
	kCmdWaitWhileAnima,
	kCmdShowFrameDir,
	kCmdStayDef,
	kCmdUnLock,
	kCmdCursorAs,
	kCmdAnimaEffect,
	kCmdSetArea,
	kCmdSetSpecArea,
	kCmdWaitToAnimaFrame,
	kCmdScrollMarginL,
	kCmdScrollMarginR,
	kCmdOutMacro,
	kCmdNormalMacro,
	kCmdIff,
	kCmdAutoIncRange,
	kCmdLock,
	kCmdHideFaceAt,
	kCmdInclude,
	kCmdLookTo,
	kCmdGetPersonX,
	kCmdGetPersonY,
	kCmdIncc,
	kCmdContinueAnima,
	kCmdLockWalk,
	kCmdUnLockWalk,
	kCmdWaitWhileEffect,
	kCmdSetDarknessAnima,
	kCmdSetDirWalk,
	kCmdMessage,
	kCmdPointRotate,
	kCmdRunGlobalFilmP,
	kCmdDoNothing,
	kCmdSlap,
	kCmdShoot,
	kCmdActiveMacroAnima,
	kCmdPause,
	kCmdExitGame,
	kCmdRotateTo,
	kCmdMacroDestructor,
	kCmdBlockMacro,
	kCmdUnBlockMacro,
	kCmdPinArea,
	kCmdAnimaDelay,
	kCmdRandom,
	kCmdDeactiveMacro,
	kCmdMixing,
	kCmdRefreshScreen,
	kCmdClearAnimaBuffer,
	kCmdRunAvx,
	kCmdJackTempo,
	kCmdClearMenuBar,
	kCmdUnLockCanal,
	kCmdPointInSecArea,
	kCmdTalkRandomIndex,
	kCmdMouseToTool,
	kCmdRelativeMulP,
	kCmdMinShadow,
	kCmdMaxShadow,
	kCmdPutShadow,
	kCmdAnimaShadow,
	kCmdShiftShadowP,
	kCmdPickup,
	kCmdMusicVolume,
	kCmdGive,
	kCmdSlapF,
	kCmdShootF,
	kCmdReactiveSelf,
	kCmdSetPosScroll,
	kCmdChkDistance,
	kCmdPointInWalkArea,
	kCmdSetScroll,
	kCmdSetSpecShadow,
	kCmdShowShadow,
	kCmdShadowStatus,
	kCmdHideShadow,
	kCmdShadowOnE,
	kCmdItemPresent,
	kCmdIgnoreArea,
	kCmdSaveAble,
	kCmdExitToDos,
	kCmdAnyKey
};

struct MacroCommand {
	CommandType type;
	Common::StringArray args;
};

/**
 * Represents a sequence of script commands.
 */
class Macro {
	Common::String _id;
	Common::Array<MacroCommand> _commands;
	bool _active;
	uint _line;

	CommandType getType(const Common::String &id) const;
public:
	Macro(const Common::String &id);
	Common::String getId() const { return _id; }
	bool isActive() const { return _active; }
	void setActive(bool active) { _active = active; }

	void load(TextParser &parser);
	const MacroCommand &loadCommand(Common::StringArray args);
	void execute(ScriptInterpreter *interp, uint32 time);
	void skipIf();
};

} // End of namespace Orlando

#endif
