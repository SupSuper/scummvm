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

#ifndef RAUNES_LOGIC_H
#define RAUNES_LOGIC_H

#include "common/str.h"
#include "graphics/surface.h"
#include "raunes/struct.h"

namespace Raunes {

class RaunesEngine;

enum MouseMode {
	kMouseWalkTo,
	kMouseUse,
	kMousePickUp,
	kMousePush,
	kMouseTalkTo,
	kMouseGive,
	kMouseDetach,
	kMouseLookAt,
	kMouseOpen
};

class LogicManager {
protected:
	RaunesEngine *_vm;

	Graphics::Surface _itemPic[50];
	Graphics::Surface _buttonPic[50];
	Player _rau;
	int _item[50];
	Common::String _itemName[150];
	Common::String _commandName[9];
	Point _itemGo;
	int _numItems, _itemInv;
	Graphics::Surface _menu, _convMenu, _menuLine;

	int _mouseBtn, _mouseX, _mouseY;
	MouseMode _mouseMode;
	Common::String _lastLine;

	int _room;
	Graphics::Surface _moveMap, _deckMap, _clickMap;
	int _numSprites;
	Sprite _sprite[10];
	bool _gameFlag[100];
	int _numWay, _numWP;
	Point _way[12], _wp[12];

public:
	LogicManager(RaunesEngine *vm) : _vm(vm) {}
	virtual ~LogicManager() = default;
	virtual void load() = 0;
	virtual void start() = 0;
	virtual void setRoom(int room) = 0;
	virtual void run() = 0;
	virtual void end() = 0;
};

// Udoiana Raunes - Special Edition (German)
class Logic_v2de : public LogicManager {
protected:
	void logo();
	void intro();
	void introWriteCenter(int x, int y, const Common::String &str);

public:
	Logic_v2de(RaunesEngine *vm) : LogicManager(vm) {}
	void load() override {}
	void start() override;
	void setRoom(int room) override;
	void run() override {}
	void end() override {}
};

// Udoiana Raunes - Special Edition (English)
class Logic_v2en : public Logic_v2de {
protected:
	void logo();

public:
	Logic_v2en(RaunesEngine *vm) : Logic_v2de(vm) {}
};

// Udoiana Raunes in search for Indiana Jones 4
class Logic_v3 : public LogicManager {
protected:
	void initVars();
	void initGfx();
	void logo();
	void intro();
	void introWriteCenter(int x, int y, const Common::String &str);
	void loadSprites();
	void updateLine();
	void writeLine(const Common::String &str);
	void updateMenu();
	void drawMenuButton();
	void drawPage();

public:
	Logic_v3(RaunesEngine *vm) : LogicManager(vm) {}
	void load() override;
	void start() override;
	void setRoom(int room) override;
	void run() override;
	void end() override {}
};

} // End of namespace Raunes

#endif
