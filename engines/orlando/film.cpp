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
#include "orlando/film.h"
#include "orlando/text_parser.h"
#include "orlando/scene.h"

namespace Orlando {

Film::Film(const Common::String &id) : _id(id), _background(nullptr) {
}

Film::~Film() {
	for (Common::Array<Graphics::Surface*>::const_iterator i = _frames.begin(); i != _frames.end(); ++i) {
		if ((*i) != nullptr) {
			(*i)->free();
			delete (*i);
		}
	}
	if (_background != nullptr) {
		_background->free();
		delete _background;
	}
}

bool Film::load(TextParser &parser, Scene *scene) {
	parser.readInt();
	_background = scene->loadSurface(parser.readString(), 8);
	if (!_background)
		return false;
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
			Graphics::Surface *surface = scene->loadSurface(id, 8);
			if (!surface)
				return false;
			_frames.push_back(surface);
		}
	}
	return true;
}

} // End of namespace Orlando
