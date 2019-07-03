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

#include "common/str.h"
#include "common/array.h"

namespace Orlando {

class TextParser;

enum CommandType {
	kUnknown = 0,
	kChWindow,
	kSetPosition,
	kInitFirst,
	kSetPersonData,
	kAnima,
	kStopAnima,
	kWalkTo,
	kRunInsertion,
	kRunFilm,
	kHide,
	kTalk,
	kDialog,
	kActiveMacro,
	kIfAnswer,
	kEndIf,
	kShowFrame,
	kHideTalk,
	kShadowBy,
	kSetShadow,
	kGotoScene,
	kPUnderP,
	kPUnderE,
	kWalkToDir,
	kShowFace,
	kHideFace,
	kLet,
	kIf,
	kShowAnimaFrame,
	kPOverE,
	kEffect,
	kEffectVolume,
	kTalkRandom,
	kTake,
	kRunFilmP,
	kExtra,
	kMoveAr,
	kSetFrame,
	kNoHave,
	kHideE,
	kShowE,
	kSetShiftX,
	kSetPositionE,
	kSetPositionEv,
	kIfMouseXM,
	kIfMouseXW,
	kScrollLeft,
	kScrollRight,
	kSetScrollX1,
	kSetScrollX2,
	kSetPosXY,
	kSetChkPrior,
	kBrightness,
	kSetPerspY,
	kMoveP,
	kStopAnimaFrame,
	kSetDialog,
	kStopAnimaNeg,
	kSetAnimaFrame,
	kDeactiveSelf,
	kEOverE,
	kMusic,
	kMoveE,
	kBrightnessE,
	kWalkEffect,
	kHaveGun,
	kNoHaveGun,
	kActiveMacroAt,
	kHaveTool,
	kNoHaveTool,
	kInc,
	kWaitWhile,
	kStay,
	kGameOver,
	kMoveEp,
	kWaitWhileAnima,
	kShowFrameDir,
	kStayDef,
	kUnlock,
	kCursorAs,
	kAnimaEffect,
	kSetArea,
	kSetSpecArea,
	kWaitToAnimaFrame,
	kScrollMarginL,
	kScrollMarginR,
	kOutMacro,
	kNormalMacro,
	kIfF,
	kAutoIncRange,
	kLock,
	kHideFaceAt,
	kInclude,
	kLookTo,
	kGetPersonX,
	kGetPersonY,
	kIncC,
	kContinueAnima,
	kLockWalk,
	kUnlockWalk,
	kWaitWhileEffect,
	kSetDarknessAnima,
	kSetDirWalk,
	kMessage,
	kPointRotate,
	kRunGlobalFilmP,
	kDoNothing,
	kSlap,
	kShoot,
	kActiveMacroAnima,
	kPause,
	kExitGame,
	kRotateTo,
	kMacroDestructor,
	kBlockMacro,
	kUnblockMacro,
	kPinArea,
	kAnimaDelay,
	kRandom,
	kDeactiveMacro,
	kMixing,
	kRefreshScreen,
	kClearAnimaBuffer,
	kRunAvx,
	kJackTempo,
	kClearMenuBar,
	kUnlockCanal,
	kPointInSecArea,
	kTalkRandomIndex,
	kMouseToTool,
	kRelativeMulP,
	kMinShadow,
	kMaxShadow,
	kPutShadow,
	kAnimaShadow,
	kShiftShadowP,
	kPickup,
	kMusicVolume,
	kGive,
	kSlapF,
	kShootF,
	kReactiveSelf,
	kSetPosScroll,
	kChkDistance,
	kPointInWalkArea,
	kSetScroll,
	kSetSpecShadow,
	kShowShadow,
	kShadowStatus,
	kHideShadow,
	kShadowOne,
	kItemPresent,
	kIgnoreArea,
	kSaveable,
	kExitToDos,
	kAnyKey
};

struct MacroCommand {
	CommandType type;
	Common::Array<Common::String> args;
};

/**
 * Represents a sequence of script commands.
 */
class Macro {
	Common::String _id;
	Common::Array<MacroCommand> _commands;

public:
	Macro(const Common::String &id);
	void load(TextParser &parser);
};

} // End of namespace Orlando

#endif
