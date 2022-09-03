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

#include "raunes/sound.h"
#include "raunes/graphics.h"
#include "raunes/logic.h"
#include "raunes/raunes.h"

namespace Raunes {

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
	while (!_vm->shouldQuit()) {
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

		_vm->delay(100);
		y++;
	}
}

void Logic_v3::introWriteCenter(int x, int y, const Common::String &str) {
	Common::String s = "#f255";
	if (y > 140 && y < 200) {
		_vm->_gfx.writeCenter(x, y, s + str);
	}
}

} // namespace Raunes
