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
#include "common/file.h"
#include "graphics/surface.h"
#include "orlando/face.h"
#include "orlando/text_parser.h"
#include "orlando/scene.h"
#include "orlando/graphics.h"

namespace Orlando {

Face::Face(const Common::String &id) : _id(id), _head(nullptr) {
	memset(_eyes, 0, sizeof(_eyes));
	memset(_mouth, 0, sizeof(_mouth));
}

Face::~Face() {
	for (int i = 0; i < ARRAYSIZE(_mouth); i++)
		delete _mouth[i];
	for (int i = 0; i < ARRAYSIZE(_eyes); i++)
		delete _eyes[i];
	delete _head;
}

bool Face::load(TextParser &parser, Scene *scene) {
	_head = scene->loadSurface(parser.readString(), 8);
	if (!_head)
		return false;
	_eyesPos.x = parser.readInt();
	_eyesPos.y = parser.readInt();
	for (int i = 0; i < ARRAYSIZE(_eyes); i++) {
		_eyes[i] = scene->loadSurface(parser.readString(), 8);
		if (!_eyes[i])
			return false;
	}
	_mouthPos.x = parser.readInt();
	_mouthPos.y = parser.readInt();
	for (int i = 0; i < ARRAYSIZE(_mouth); i++) {
		_mouth[i] = scene->loadSurface(parser.readString(), 8);
		if (!_mouth[i])
			return false;
	}
	return true;
}

} // End of namespace Orlando
