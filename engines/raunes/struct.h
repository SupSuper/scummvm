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

#ifndef RAUNES_STRUCT_H
#define RAUNES_STRUCT_H

#define RAU_POS(x) (x * 1024)
#define RAU_POS_XY(x, y) {x * 1024, y * 1024}
#define RAU_DRAW(x) (x / 1024)
#define RAU_DRAW_XY(x, y) {x / 1024, y / 1024}

#include "graphics/surface.h"

namespace Raunes {

struct Point {
	int _x, _y;

	Point() : _x(0), _y(0) {}
	Point(int x, int y) : _x(x), _y(y) {}
};

struct Player {
	Graphics::Surface _walkPic[4][12];
	int _numPics[4];
	Graphics::Surface _specPic[8][6];
	int _specNum, _specMode;
	bool _moving, _anim, _freeMove, _allowMove;
	int _shrink;
	Point _pos, _oldPos, _step;
	int _dir;
};

struct Sprite {
	Point _curPos, _pos;
	int _numPics[5];
	Graphics::Surface _pics[5][25];
	bool _draw;

	void setNumPics(int n1 = 0, int n2 = 0, int n3 = 0, int n4 = 0, int n5 = 0) {
		_numPics[0] = n1;
		_numPics[1] = n2;
		_numPics[2] = n3;
		_numPics[3] = n4;
		_numPics[4] = n5;
	}
};

} // End of namespace Raunes

#endif
