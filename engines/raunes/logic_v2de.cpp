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

namespace Raunes {

void Logic_v2de::logo() {
	_vm->_gfx.setPage(0);
	_vm->_gfx.showPage(1);
	_vm->_gfx.clearScreen();
	_vm->_gfx.showPcx("LOGO.PCX");
	_vm->_gfx.swapPage();
	_vm->delay(15000);
	_vm->_gfx.showPcx("INTRO1.PCX"); // TODO: Skip in English version
	_vm->_gfx.swapPage();
	_vm->delay(35000);
	_vm->_gfx.showPcx("INTRO2.PCX");
	_vm->_gfx.swapPage();
	_vm->delay(35000);
}

void Logic_v2de::intro() {
	_vm->_snd.play("RAUNES.SND", 11110);
	_vm->_gfx.clearScreen();

	_vm->_gfx.setPage(2);
	_vm->_gfx.showPcx("INTRO.PCX");
	_vm->_gfx.write(0, 0, "#b255#f031");
	int y = 15;
	while (!_vm->shouldQuit()) {
		_vm->_gfx.swapPage();
		_vm->_gfx.blockMove(2, 0, 0, _vm->_gfx.getPage(), 0, 0, 200, 200);
		_vm->_gfx.blockMove(2, 200, 0, _vm->_gfx.getPage(), 200, 0, 120, 200);

		introWriteCenter(190, 210 - y, "1. April 1999");
		introWriteCenter(190, 230 - y, "Sch\x81ler brachten das Wilhelm-Hausenstein-Gymnasium");
		introWriteCenter(190, 240 - y, "in ihre Gewalt.");
		introWriteCenter(190, 250 - y, "Ursache war das Vorherrschen von B\x81rokratie,");
		introWriteCenter(190, 260 - y, "das mit der Attestpflicht f\x81r f\x81nf sehr gute Sch\x81ler");
		introWriteCenter(190, 270 - y, "am 30. M\x84rz seinen H\x94hepunkt erreichte.");
		introWriteCenter(190, 290 - y, "Um 8:45 Uhr des 1. April");
		introWriteCenter(190, 300 - y, "st\x81rmte eine auserw\x84hlte Sch\x81lergruppe das Sekretariat.");
		introWriteCenter(190, 310 - y, "Alle Sekret\x84rinnen wurden sofort hingerichtet.");
		introWriteCenter(190, 320 - y, "Direktorin und Stellvertreter");
		introWriteCenter(190, 330 - y, "- Karin Plodeck und Heinz Schaffer -");
		introWriteCenter(190, 340 - y, "konnten fliehen.");
		introWriteCenter(190, 360 - y, "Nachdem die Sch\x81ler die Macht an sich gerissen hatten,");
		introWriteCenter(190, 370 - y, "wurden die Lehrer nach folgenden Kriterien selektiert:");
		introWriteCenter(190, 380 - y, "1. Popul\x84r und Intelligent");
		introWriteCenter(190, 390 - y, "2. Popul\x84r und Dumm");
		introWriteCenter(190, 400 - y, "3. Unpopul\x84r und Intelligent");
		introWriteCenter(190, 410 - y, "4. Unpopul\x84r und Dumm");
		introWriteCenter(190, 430 - y, "Die letzte Klasse wurde umgehend");
		introWriteCenter(190, 440 - y, "einem Exekutionskommando \x81""berstellt.");
		introWriteCenter(190, 450 - y, "Klasse Zwei und Drei wurden");
		introWriteCenter(190, 460 - y, "Disziplinierungsma\xE1nahmen unterzogen.");
		introWriteCenter(190, 480 - y, "Zuletzt wurden Elternbeirat,");
		introWriteCenter(190, 490 - y, "Freundeskreis und Verbindungslehrer");
		introWriteCenter(190, 500 - y, "durch Zwangsr\x81""cktritt oder");
		introWriteCenter(190, 510 - y, "Eliminierungsma\xE1nahmen entmachtet.");
		introWriteCenter(190, 530 - y, "Nachdem das Schulsystem ausgeschaltet worden war,");
		introWriteCenter(190, 540 - y, "standen die Sch\x81ler jedoch vor dem postrevolt\x84ren Nichts.");
		introWriteCenter(190, 560 - y, "Die Sch\x81lereinheit hatte sich aufgespalten.");
		introWriteCenter(190, 570 - y, "Es entstanden Parteien:");
		introWriteCenter(190, 580 - y, "Die VSP");
		introWriteCenter(190, 590 - y, "(Vergn\x81gungsS\x81""chtige Proletengruppe)");
		introWriteCenter(190, 600 - y, "und die BVAJ");
		introWriteCenter(190, 610 - y, "(Bessere Verh\x84ltnisse Anstrebende Intellektuellengruppe).");
		introWriteCenter(190, 630 - y, "An einer Neuordnung der Verh\x84ltnisse wird gerade");
		introWriteCenter(190, 640 - y, "seitens der BVAJ gearbeitet.");
		introWriteCenter(190, 660 - y, "Das System ist jedoch zerst\x94rt - es herrscht Anarchie.");
		introWriteCenter(190, 690 - y, "Es gibt keine Hoffnung... oder?");

		_vm->delay(100);
		y++;
	}
}

void Logic_v2de::introWriteCenter(int x, int y, const Common::String &str) {
	Common::String s = "#f000";
	if (y > 130 && y < 190) {
		_vm->_gfx.writeCenter(x, y, s + str);
	}
}

} // namespace Raunes
