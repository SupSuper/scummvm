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
#include "graphics/cursorman.h"

#include "raunes/graphics.h"
#include "raunes/logic.h"
#include "raunes/raunes.h"
#include "raunes/sound.h"

namespace Raunes {

void Logic_v3::initVars() {
	//_item = {0};
	_numItems = 0;
	_itemInv = 1;
	_item[0] = 4;
	_itemName[0] = "";
	_itemName[30] = "Udoiana Raunes";
	_itemName[100] = "";
	_itemName[101] = "albatross beak scissors";
	_itemName[102] = "albatross beak";
	_itemName[103] = "nail";
	_itemName[104] = "whip";
	_itemName[105] = "nut";
	_itemName[106] = "SchoolOrderStone";
	_itemName[107] = "PDA";
	_itemName[108] = "full battery";
	_itemName[109] = "empty battery";
	_itemName[110] = "strange pills";
	_itemName[111] = "squirrel tooth";
	_itemName[112] = "key";
	_itemName[113] = "greencard";
	_itemName[114] = "rubber duck";
	_itemName[115] = "lanced rubber duck";
	_itemName[116] = "promise ring";
	_itemName[117] = "picture";
	_itemName[118] = "strange book";
	_itemName[119] = "money";
	_itemName[120] = "Indy4 screenplay";
	_itemName[121] = "Indy's heart (TM)";
	_itemName[122] = "lightsaber";
	_itemName[123] = "chewbacca fur";
	_itemName[124] = "Indy hat";
	_itemName[125] = "action scene";
	_itemName[126] = "adventure scene";
	_itemName[127] = "love scene";
	_itemName[128] = "imcomplete screenplay";
	_itemName[129] = "PDA with batteries";
	_itemName[130] = "fish";
	_itemName[131] = "drink";
	_itemName[132] = "pizza";
	_itemName[133] = "salami";
	_itemName[134] = "pearl";
	_itemName[135] = "frog eyes";
	_itemName[136] = "albatross head";

	_commandName[0] = "#f171walk to ";
	_commandName[1] = "use ";
	_commandName[2] = "pick up ";
	_commandName[3] = "push ";
	_commandName[4] = "talk to ";
	_commandName[5] = "give ";
	_commandName[6] = "detach ";
	_commandName[7] = "look at ";
	_commandName[8] = "open ";
	//_gameFlag = {0};

	_mouseMode = kMouseWalkTo;
}

void Logic_v3::initGfx() {
	_vm->_gfx.loadGrf("MENUE.GRF", &_menu);
	_vm->_gfx.loadGrf("CONVMEN.GRF", &_convMenu);
	_vm->_gfx.loadGrf("LINE.GRF", &_menuLine);
	for (int i = 0; i < 50; i++) {
		_vm->_gfx.loadGrf(Common::String::format("ITEM%d.GRF", i), &_itemPic[i]);
	}
	for (int i = 0; i < 9; i++) {
		_vm->_gfx.loadGrf(Common::String::format("BUT%d.GRF", i + 1), &_buttonPic[i]);
	}
	_vm->_gfx.setCursor("ITEM0A.GRF");

	_rau._numPics[0] = 12;
	_rau._numPics[1] = 12;
	_rau._numPics[2] = 8;
	_rau._numPics[3] = 8;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j <= _rau._numPics[i]; j++) {
			_vm->_gfx.loadGrf(Common::String::format("RAU%d11%d.GRF", i, j), &_rau._walkPic[i][j]);
		}
	}
	int numSpec[] = {3, 2, 2, 2, 2, 6, 5, 5};
	for (int i = 0; i < ARRAYSIZE(numSpec); i++) {
		for (int j = 0; j < numSpec[i]; j++) {
			_vm->_gfx.loadGrf(Common::String::format("RAU11%d%d.GRF", i + 2, j + 1), &_rau._specPic[i][j]);
		}
	}
}

void Logic_v3::load() {
	initVars();
	initGfx();
}

void Logic_v3::start() {
	logo();
	if (_vm->shouldQuit()) return;
	intro();
	if (_vm->shouldQuit()) return;
	setRoom(1);
	_vm->_gfx.write(0, 0, "#f171#b000");
}

void Logic_v3::logo() {
	_vm->_gfx.setPage(0);
	_vm->_gfx.showPage(1);
	_vm->_gfx.clearScreen();
	_vm->_gfx.showPcx("LOGO.PCX");
	_vm->_gfx.swapPage();
	_vm->delay(15000);
}

void Logic_v3::intro() {
	_vm->_snd.play("RAUM1.SND", 22050);
	_vm->_gfx.clearScreen();

	_vm->_gfx.setPage(2);
	_vm->_gfx.showPcx("INTRO.PCX");
	_vm->_gfx.write(0, 0, "#b255#f255");

	int y = 5;
	bool skip = _vm->shouldQuit();
	while (!skip && y <= 280) {
		_vm->_gfx.swapPage();
		_vm->_gfx.blockMove(2, 0, 0, _vm->_gfx.getPage(), 0, 0, 200, 200);
		_vm->_gfx.blockMove(2, 200, 0, _vm->_gfx.getPage(), 200, 0, 120, 200);
		introWriteCenter(160, 210 - y, "It's spring 2005,");

		introWriteCenter(160, 230 - y, "Eight years after Dr. Udoiana Raunes from Germany");
		introWriteCenter(160, 240 - y, "was the leading actor");
		introWriteCenter(160, 250 - y, "in an Indiana Jones fan game.");

		introWriteCenter(160, 270 - y, "He restored the school order");
		introWriteCenter(160, 280 - y, "in his small high school in Munich.");

		introWriteCenter(160, 300 - y, "Eight years full of peace and harmony.");

		introWriteCenter(160, 320 - y, "Now Dr. Raunes is looking for his retirement.");
		introWriteCenter(160, 330 - y, "(poor guy, he invested");
		introWriteCenter(160, 340 - y, "in the german state-run pension insurance)");

		introWriteCenter(160, 360 - y, "Feeling happy that he never would have to appear");
		introWriteCenter(160, 370 - y, "in a lousy fan game again");
		introWriteCenter(160, 380 - y, "he enjoys nature");
		introWriteCenter(160, 390 - y, "and walks through beautiful german forests");

		introWriteCenter(160, 410 - y, "...until today...");

		skip = _vm->delay(100);
		y++;
	}
}

void Logic_v3::introWriteCenter(int x, int y, const Common::String &str) {
	const Common::String s = "#f255";
	if (y > 140 && y < 200) {
		_vm->_gfx.writeCenter(x, y, s + str);
	}
}

void Logic_v3::setRoom(int room) {
	_vm->_gfx.clearScreen();
	_vm->_gfx.setPage(3);
	_vm->_gfx.showPcx(Common::String::format("RAUM%d.PCX", room));
	_vm->_gfx.updatePage2();
	_vm->_gfx.loadPcx(Common::String::format("RAUM%dS.PCX", room), &_moveMap);
	_vm->_gfx.loadPcx(Common::String::format("RAUM%dD.PCX", room), &_deckMap);
	_vm->_gfx.loadPcx(Common::String::format("RAUM%dC.PCX", room), &_clickMap);
	_vm->_snd.play(Common::String::format("RAUM%d.SND", room), 22050);

	_room = room;
	switch (room) {
	case 1:
		_rau._dir = 1;
		_rau._pos = RAU_POS_XY(11, 139);
		if (_gameFlag[3]) {
			_rau._pos = RAU_POS_XY(263, 132);
			_rau._dir = 0;
		}

		_numSprites = 5;
		for (int i = 0; i < _numSprites; i++) {
			Sprite &s = _sprite[i];
			switch (i) {
			case 0:
				s._curPos = {280, 90};
				s._pos = s._curPos;
				s.setNumPics(2, 2, 3, 6);
				break;
			case 1:
				s._curPos = {320-120, 25};
				s._pos = s._curPos;
				s.setNumPics(1);
				break;
			case 2:
				s._curPos = {280, 90};
				s._pos = s._curPos;
				s.setNumPics(1);
				break;
			case 3:
				s._curPos = {160, 80};
				s._pos = s._curPos;
				s.setNumPics(2, 2, 1);
				break;
			case 4:
				s._curPos = {280, 35};
				s._pos = s._curPos;
				s.setNumPics(16);
				break;
			}
		}
		break;
	}
	loadSprites();
	updateMenu();
	CursorMan.showMouse(true);
}

void Logic_v3::loadSprites() {
	for (int n = 0; n < _numSprites; n++) {
		Sprite &s = _sprite[n];
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < s._numPics[i]; j++) {
				_vm->_gfx.loadGrf(Common::String::format("SPR%d%d%d%d.GRF", _room, n + 1, i + 1, j + 1), &s._pics[i][j]);
			}
		}
	}
}


void Logic_v3::updateLine() {
	Common::String s = _commandName[_mouseMode];
	s += _itemName[0];
	writeLine(s);
}

void Logic_v3::writeLine(const Common::String &str) {
	if (str != _lastLine) {
		_vm->_gfx.setPage(2);
		_vm->_gfx.drawPic(0, 147, &_menuLine);
		_vm->_gfx.writeCenter(160, 152, "#f171#b255" + str + "#b000");
		_vm->_gfx.setPage();
		_lastLine = str;
	}
}

void Logic_v3::updateMenu() {
	_vm->_gfx.setPage(2);
	_vm->_gfx.drawPic(0, 147, &_menu, false);
	for (int y = 0; y < 1; y++) {
		for (int x = 0; x < 2; x++) {
			int item = _itemInv + x + y * 3;
			if (item <= _numItems) {
				_vm->_gfx.drawPic(186 + x * 40, 159 + y * 20, &_itemPic[_item[item]]);
			}
		}
	}
	_vm->_gfx.setPage();
	updateLine();
}

void Logic_v3::drawMenuButton() {
	switch (_mouseMode) {
	case kMouseUse:
		_vm->_gfx.drawPic(5, 160, &_buttonPic[0]);
		break;
	case kMousePickUp:
		_vm->_gfx.drawPic(5, 172, &_buttonPic[1]);
		break;
	case kMousePush:
		_vm->_gfx.drawPic(5, 184, &_buttonPic[2]);
		break;
	case kMouseTalkTo:
		_vm->_gfx.drawPic(53, 160, &_buttonPic[3]);
		break;
	case kMouseGive:
		_vm->_gfx.drawPic(53, 172, &_buttonPic[4]);
		break;
	case kMouseDetach:
		_vm->_gfx.drawPic(53, 184, &_buttonPic[5]);
		break;
	case kMouseWalkTo:
		_vm->_gfx.drawPic(101, 160, &_buttonPic[6]);
		break;
	case kMouseLookAt:
		_vm->_gfx.drawPic(101, 172, &_buttonPic[7]);
		break;
	case kMouseOpen:
		_vm->_gfx.drawPic(101, 184, &_buttonPic[8]);
		break;
	}
}

void Logic_v3::drawPage() {
	_vm->_gfx.blockMove(2, 0, 0, _vm->_gfx.getPage(), 0, 0, 200, 200);
	_vm->_gfx.blockMove(2, 200, 0, _vm->_gfx.getPage(), 200, 0, 120, 200);
	drawMenuButton();
}

void Logic_v3::run() {
	_vm->_gfx.swapPage();
	drawPage();
}

} // namespace Raunes
