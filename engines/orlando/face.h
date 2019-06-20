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

#ifndef ORLANDO_FACE_H
#define ORLANDO_FACE_H

#include "common/str.h"
#include "common/rect.h"

namespace Graphics {
	struct Surface;
}

namespace Orlando {

class TextParser;
class Scene;

/**
 * Represents a conversation portrait.
 */
class Face {
	Common::String _id;
	Graphics::Surface *_head;
	Common::Point _eyesPos;
	Graphics::Surface *_eyes[3];
	Common::Point _mouthPos;
	Graphics::Surface *_mouth[5];

public:
	Face(const Common::String &id);
	~Face();
	bool load(TextParser &parser, Scene *scene);
};

} // End of namespace Orlando

#endif
