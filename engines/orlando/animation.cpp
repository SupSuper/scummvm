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
#include "orlando/animation.h"
#include "orlando/text_parser.h"
#include "orlando/scene.h"
#include "orlando/graphics.h"
#include "orlando/sprite.h"
#include "orlando/flx_anim.h"

namespace Orlando {

Animation::Animation(const Common::String &id) : _id(id), _delay(0), _current(0), _flx(nullptr) {
}

Animation::~Animation() {
	delete _flx;
	for (Common::Array<Frame>::const_iterator i = _frames.begin(); i != _frames.end(); ++i) {
		if (i->surface != nullptr) {
			i->surface->free();
			delete i->surface;
		}
	}
}

bool Animation::load(TextParser &parser, Scene *scene) {
	Sprite *object = scene->getObject(_id);
	if (object == nullptr) {
		warning("Animation: Missing object '%s'", _id.c_str());
		return false;
	}

	_delay = parser.readInt();
	while (!parser.eof()) {
		Common::String rec = parser.readString();
		if (rec == "REC") {
			while (!parser.eof()) {
				Common::String frame = parser.readString();
				if (frame == "END") {
					break;
				}
				_timeline.push_back(atoi(frame.c_str()));
			}
		} else {
			parser.rewind();
			break;
		}
	}

	if (!_id.hasPrefix("FLX")) {
		Frame frame;
		frame.surface = object->loadSurface(_id, scene);
		frame.offset = Common::Point(0, 0);
		_frames.push_back(frame);
	}

	while (!parser.eof()) {
		Common::String filename = parser.readString();
		if (filename.empty() || filename.firstChar() == '[') {
			parser.rewind();
			break;
		}
		Frame frame;
		if (filename.hasSuffix(".FLX")) {
			Common::File *flx = scene->loadFile(filename);
			if (flx == nullptr)
				return false;
			_flx = new FlxAnimation(flx, scene->getGraphicsManager()->kScreenFormat);
			frame.surface = nullptr;
		} else {
			frame.surface = object->loadSurface(filename, scene);
		}
		frame.offset.x = parser.readInt();
		frame.offset.y = parser.readInt();
		_frames.push_back(frame);
	}

	object->setAnimation(this);
	return true;
}

Frame Animation::nextFrame() {
	if (_flx == nullptr) {
		if (_timeline.empty()) {
			return _frames.back();
		} else {
			_current = (_current + 1) % _timeline.size();
			int nextFrame = ABS(_timeline[_current]) - 1;
			return _frames[nextFrame];
		}
	} else {
		Frame frame = { _flx->nextFrame(), Common::Point(0, 0) };
		return frame;
	}
}

} // End of namespace Orlando
