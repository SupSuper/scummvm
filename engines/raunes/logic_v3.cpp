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

#include "raunes/graphics.h"
#include "raunes/logic.h"
#include "raunes/raunes.h"
#include "raunes/sound.h"

namespace Raunes {

void Logic_v3::load() {
	for (int i = 0; i < 50; i++) {
		_vm->_gfx.loadGrf(Common::String::format("ITEM%d.GRF", i), &_itemPic[i]);
	}
	for (int i = 0; i < 9; i++) {
		_vm->_gfx.loadGrf(Common::String::format("BUT%d.GRF", i + 1), &_itemPic[i]);
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

void Logic_v3::run() {
}

} // namespace Raunes
