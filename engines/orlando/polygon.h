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
	bool contains(Common::Point i) const {
		int yflag0;
		int yflag1;
		bool inside_flag = false;
		unsigned int pt;

		const Common::Point *vtx0 = &p[N - 1];
		const Common::Point *vtx1 = &p[0];

		yflag0 = (vtx0->y >= i.y);
		for (pt = 0; pt < N; pt++, vtx1++) {
			yflag1 = (vtx1->y >= i.y);
			if (yflag0 != yflag1) {
				if (((vtx1->y - i.y) * (vtx0->x - vtx1->x) >=
				     (vtx1->x - i.x) * (vtx0->y - vtx1->y)) == yflag1) {
					inside_flag = !inside_flag;
				}
			}
			yflag0 = yflag1;
			vtx0 = vtx1;
		}

		return inside_flag;
	}
};
struct Triangle : Polygon<3> {};
struct Quad : Polygon<4> {};

} // End of namespace Orlando

#endif
