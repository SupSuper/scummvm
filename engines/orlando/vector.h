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

#ifndef ORLANDO_VECTOR_H
#define ORLANDO_VECTOR_H

#include "common/scummsys.h"
#include "common/rect.h"

namespace Orlando {

/**
 * Simple class for handling floating-point positions.
 */
struct Vector2 {
	float x;	///< The horizontal part of the point
	float y;	///< The vertical part of the point

	Vector2() : x(0), y(0) {}
	Vector2(float x1, float y1) : x(x1), y(y1) {}
	Vector2(Common::Point p) : x(p.x), y(p.y) {}
	bool    operator==(const Vector2 &v)    const { return x == v.x && y == v.y; }
	bool    operator!=(const Vector2 &v)    const { return x != v.x || y != v.y; }
	Vector2 operator+(const Vector2 &delta) const { return Vector2(x + delta.x, y + delta.y); }
	Vector2 operator-(const Vector2 &delta) const { return Vector2(x - delta.x, y - delta.y); }

	void operator+=(const Vector2 &delta) {
		x += delta.x;
		y += delta.y;
	}

	void operator-=(const Vector2 &delta) {
		x -= delta.x;
		y -= delta.y;
	}

	operator Common::Point() const { return Common::Point(x, y); }
};

} // End of namespace Orlando

#endif
