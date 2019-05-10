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
#include "orlando/area.h"
#include "orlando/text_parser.h"

namespace Orlando {

Area::Area(const Common::String &id) : _id(id) {
}

Area::~Area() {
}

void Area::load(TextParser &parser) {
	int num = parser.readInt();
	for (int j = 0; j < num; j++) {
		Triangle area;
		for (int i = 0; i < area.kPoints; i++) {
			area.p[i].x = parser.readInt();
			area.p[i].y = parser.readInt();
		}
		_regions.push_back(area);
	}
}

} // End of namespace Orlando
