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
#include "common/debug.h"

#include "orlando/interp.h"
#include "orlando/orlando.h"

namespace Orlando {

const ScriptHandler ScriptInterpreter::kCommandHandlers[] = {
	&ScriptInterpreter::unknown,
	&ScriptInterpreter::unknown, // kChWindow,
	&ScriptInterpreter::unknown, // kSetPosition,
	&ScriptInterpreter::unknown, // kInitFirst,
	&ScriptInterpreter::unknown, // kSetPersonData,
	&ScriptInterpreter::unknown, // kAnima,
	&ScriptInterpreter::unknown, // kStopAnima,
	&ScriptInterpreter::unknown, // kWalkTo,
	&ScriptInterpreter::unknown, // kRunInsertion,
	&ScriptInterpreter::unknown, // kRunFilm,
	&ScriptInterpreter::unknown, // kHide,
	&ScriptInterpreter::unknown, // kTalk,
	&ScriptInterpreter::unknown, // kDialog,
	&ScriptInterpreter::unknown, // kActiveMacro,
	&ScriptInterpreter::unknown, // kIfAnswer,
	&ScriptInterpreter::unknown, // kEndIf,
	&ScriptInterpreter::unknown, // kShowFrame,
	&ScriptInterpreter::unknown, // kHideTalk,
	&ScriptInterpreter::unknown, // kShadowBy,
	&ScriptInterpreter::unknown, // kSetShadow,
	&ScriptInterpreter::unknown, // kGotoScene,
	&ScriptInterpreter::unknown, // kPUnderP,
	&ScriptInterpreter::unknown, // kPUnderE,
	&ScriptInterpreter::unknown, // kWalkToDir,
	&ScriptInterpreter::unknown, // kShowFace,
	&ScriptInterpreter::unknown, // kHideFace,
	&ScriptInterpreter::unknown, // kLet,
	&ScriptInterpreter::unknown, // kIf,
	&ScriptInterpreter::unknown, // kShowAnimaFrame,
	&ScriptInterpreter::unknown, // kPOverE,
	&ScriptInterpreter::unknown, // kEffect,
	&ScriptInterpreter::unknown, // kEffectVolume,
	&ScriptInterpreter::unknown, // kTalkRandom,
	&ScriptInterpreter::unknown, // kTake,
	&ScriptInterpreter::unknown, // kRunFilmP,
	&ScriptInterpreter::unknown, // kExtra,
	&ScriptInterpreter::unknown, // kMoveAr,
	&ScriptInterpreter::unknown, // kSetFrame,
	&ScriptInterpreter::unknown, // kNoHave,
	&ScriptInterpreter::unknown, // kHideE,
	&ScriptInterpreter::unknown, // kShowE,
	&ScriptInterpreter::unknown, // kSetShiftX,
	&ScriptInterpreter::unknown, // kSetPositionE,
	&ScriptInterpreter::unknown, // kSetPositionEv,
	&ScriptInterpreter::unknown, // kIfMouseXM,
	&ScriptInterpreter::unknown, // kIfMouseXW,
	&ScriptInterpreter::unknown, // kScrollLeft,
	&ScriptInterpreter::unknown, // kScrollRight,
	&ScriptInterpreter::unknown, // kSetScrollX1,
	&ScriptInterpreter::unknown, // kSetScrollX2,
	&ScriptInterpreter::unknown, // kSetPosXY,
	&ScriptInterpreter::unknown, // kSetChkPrior,
	&ScriptInterpreter::unknown, // kBrightness,
	&ScriptInterpreter::unknown, // kSetPerspY,
	&ScriptInterpreter::unknown, // kMoveP,
	&ScriptInterpreter::unknown, // kStopAnimaFrame,
	&ScriptInterpreter::unknown, // kSetDialog,
	&ScriptInterpreter::unknown, // kStopAnimaNeg,
	&ScriptInterpreter::unknown, // kSetAnimaFrame,
	&ScriptInterpreter::unknown, // kDeactiveSelf,
	&ScriptInterpreter::unknown, // kEOverE,
	&ScriptInterpreter::unknown, // kMusic,
	&ScriptInterpreter::unknown, // kMoveE,
	&ScriptInterpreter::unknown, // kBrightnessE,
	&ScriptInterpreter::unknown, // kWalkEffect,
	&ScriptInterpreter::unknown, // kHaveGun,
	&ScriptInterpreter::unknown, // kNoHaveGun,
	&ScriptInterpreter::unknown, // kActiveMacroAt,
	&ScriptInterpreter::unknown, // kHaveTool,
	&ScriptInterpreter::unknown, // kNoHaveTool,
	&ScriptInterpreter::unknown, // kInc,
	&ScriptInterpreter::unknown, // kWaitWhile,
	&ScriptInterpreter::unknown, // kStay,
	&ScriptInterpreter::unknown, // kGameOver,
	&ScriptInterpreter::unknown, // kMoveEp,
	&ScriptInterpreter::unknown, // kWaitWhileAnima,
	&ScriptInterpreter::unknown, // kShowFrameDir,
	&ScriptInterpreter::unknown, // kStayDef,
	&ScriptInterpreter::unknown, // kUnlock,
	&ScriptInterpreter::unknown, // kCursorAs,
	&ScriptInterpreter::unknown, // kAnimaEffect,
	&ScriptInterpreter::unknown, // kSetArea,
	&ScriptInterpreter::unknown, // kSetSpecArea,
	&ScriptInterpreter::unknown, // kWaitToAnimaFrame,
	&ScriptInterpreter::unknown, // kScrollMarginL,
	&ScriptInterpreter::unknown, // kScrollMarginR,
	&ScriptInterpreter::unknown, // kOutMacro,
	&ScriptInterpreter::unknown, // kNormalMacro,
	&ScriptInterpreter::unknown, // kIfF,
	&ScriptInterpreter::unknown, // kAutoIncRange,
	&ScriptInterpreter::unknown, // kLock,
	&ScriptInterpreter::unknown, // kHideFaceAt,
	&ScriptInterpreter::unknown, // kInclude,
	&ScriptInterpreter::unknown, // kLookTo,
	&ScriptInterpreter::unknown, // kGetPersonX,
	&ScriptInterpreter::unknown, // kGetPersonY,
	&ScriptInterpreter::unknown, // kIncC,
	&ScriptInterpreter::unknown, // kContinueAnima,
	&ScriptInterpreter::unknown, // kLockWalk,
	&ScriptInterpreter::unknown, // kUnlockWalk,
	&ScriptInterpreter::unknown, // kWaitWhileEffect,
	&ScriptInterpreter::unknown, // kSetDarknessAnima,
	&ScriptInterpreter::unknown, // kSetDirWalk,
	&ScriptInterpreter::unknown, // kMessage,
	&ScriptInterpreter::unknown, // kPointRotate,
	&ScriptInterpreter::unknown, // kRunGlobalFilmP,
	&ScriptInterpreter::unknown, // kDoNothing,
	&ScriptInterpreter::unknown, // kSlap,
	&ScriptInterpreter::unknown, // kShoot,
	&ScriptInterpreter::unknown, // kActiveMacroAnima,
	&ScriptInterpreter::unknown, // kPause,
	&ScriptInterpreter::unknown, // kExitGame,
	&ScriptInterpreter::unknown, // kRotateTo,
	&ScriptInterpreter::unknown, // kMacroDestructor,
	&ScriptInterpreter::unknown, // kBlockMacro,
	&ScriptInterpreter::unknown, // kUnblockMacro,
	&ScriptInterpreter::unknown, // kPinArea,
	&ScriptInterpreter::unknown, // kAnimaDelay,
	&ScriptInterpreter::unknown, // kRandom,
	&ScriptInterpreter::unknown, // kDeactiveMacro,
	&ScriptInterpreter::unknown, // kMixing,
	&ScriptInterpreter::unknown, // kRefreshScreen,
	&ScriptInterpreter::unknown, // kClearAnimaBuffer,
	&ScriptInterpreter::unknown, // kRunAvx,
	&ScriptInterpreter::unknown, // kJackTempo,
	&ScriptInterpreter::unknown, // kClearMenuBar,
	&ScriptInterpreter::unknown, // kUnlockCanal,
	&ScriptInterpreter::unknown, // kPointInSecArea,
	&ScriptInterpreter::unknown, // kTalkRandomIndex,
	&ScriptInterpreter::unknown, // kMouseToTool,
	&ScriptInterpreter::unknown, // kRelativeMulP,
	&ScriptInterpreter::unknown, // kMinShadow,
	&ScriptInterpreter::unknown, // kMaxShadow,
	&ScriptInterpreter::unknown, // kPutShadow,
	&ScriptInterpreter::unknown, // kAnimaShadow,
	&ScriptInterpreter::unknown, // kShiftShadowP,
	&ScriptInterpreter::unknown, // kPickup,
	&ScriptInterpreter::unknown, // kMusicVolume,
	&ScriptInterpreter::unknown, // kGive,
	&ScriptInterpreter::unknown, // kSlapF,
	&ScriptInterpreter::unknown, // kShootF,
	&ScriptInterpreter::unknown, // kReactiveSelf,
	&ScriptInterpreter::unknown, // kSetPosScroll,
	&ScriptInterpreter::unknown, // kChkDistance,
	&ScriptInterpreter::unknown, // kPointInWalkArea,
	&ScriptInterpreter::unknown, // kSetScroll,
	&ScriptInterpreter::unknown, // kSetSpecShadow,
	&ScriptInterpreter::unknown, // kShowShadow,
	&ScriptInterpreter::unknown, // kShadowStatus,
	&ScriptInterpreter::unknown, // kHideShadow,
	&ScriptInterpreter::unknown, // kShadowOne,
	&ScriptInterpreter::unknown, // kItemPresent,
	&ScriptInterpreter::unknown, // kIgnoreArea,
	&ScriptInterpreter::unknown, // kSaveable,
	&ScriptInterpreter::unknown, // kExitToDos,
	&ScriptInterpreter::unknown, // kAnyKey
};

ScriptInterpreter::ScriptInterpreter(OrlandoEngine *vm) : _vm(vm) {
}

bool ScriptInterpreter::runCommand(Macro *macro, const MacroCommand &cmd) {
	debug("ScriptInterpreter: Executing [%s] %s", macro->getId().c_str(), cmd.args[0].c_str());
	ScriptHandler handler = kCommandHandlers[cmd.type];
	return (this->*handler)(macro, cmd);
}

bool ScriptInterpreter::unknown(Macro *macro, const MacroCommand &cmd) {
	warning("ScriptInterpreter: Unknown command %s", cmd.args[0].c_str());
	return true;
}

} // End of namespace Orlando
