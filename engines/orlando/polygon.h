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

#ifndef ORLANDO_POLYGON_H
#define ORLANDO_POLYGON_H

#include "common/scummsys.h"
#include "common/rect.h"

namespace Orlando {

/** Wrapper for common polygon maths. */
template <uint N>
struct Polygon {
	static const uint kPoints = N;
	Common::Point p[N];
	int16 &x(unsigned int i) { return p[i].x; }
	int16 &y(unsigned int i) { return p[i].y; }
};
struct Triangle : Polygon<3> {};
struct Quad : Polygon<4> {};

} // End of namespace Orlando

#endif
