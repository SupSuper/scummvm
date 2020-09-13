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

#include "orlando/insertion.h"
#include "orlando/text_parser.h"
#include "orlando/scene.h"
#include "orlando/person.h"

namespace Orlando {

Insertion::Insertion(const Common::String &id) : _id(id), _curFrame(0), _playing(false) {
}

Insertion::~Insertion() {
	for (Common::Array<PFrame>::const_iterator i = _frames.begin(); i != _frames.end(); ++i) {
		if (i->surface != nullptr) {
			i->surface->free();
			delete i->surface;
		}
	}
}

bool Insertion::load(TextParser &parser, Scene *scene) {
	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty() || id.firstChar() == '[') {
			parser.rewind();
			break;
		}
		if (id.equalsIgnoreCase("EFFECT")) {
			parser.readString();
			parser.readInt();
			parser.readInt();
			parser.readInt();
		} else {
			PFrame frame;
			frame.surface = scene->loadSurface(id, 8);
			if (!frame.surface)
				return false;
			frame.offsetX = parser.readInt();
			frame.offsetXFlip = parser.readInt();
			frame.offsetY = parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readInt();
			_frames.push_back(frame);
		}
	}
	return true;
}

void Insertion::init(bool play, uint32 time) {
	_playing = play;
	_curFrame = 0;
	_time = time;
}

void Insertion::nextFrame(uint32 time, Person *person) {
	while (_playing && time >= _time + person->getDelay()) {
		_time += person->getDelay();
		_curFrame++;
		if (_curFrame >= _frames.size()) {
			_playing = false;
			_curFrame = _frames.size() - 1;
		}
		person->draw();
	}
}

} // End of namespace Orlando
