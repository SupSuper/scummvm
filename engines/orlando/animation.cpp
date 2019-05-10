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
#include "orlando/element.h"
#include "orlando/flx_anim.h"

namespace Orlando {

Animation::Animation(const Common::String &id) : _id(id), _flx(nullptr), _curFrame(0), _time(0) {
}

Animation::~Animation() {
	if (_flx == nullptr) {
		for (Common::Array<Frame>::const_iterator i = _frames.begin(); i != _frames.end(); ++i) {
			if (i->surface != nullptr) {
				i->surface->free();
				delete i->surface;
			}
		}
	} else {
		delete _flx;
	}
}

bool Animation::load(TextParser &parser, Scene *scene) {
	Element *element = scene->getElement(_id);
	if (element == nullptr) {
		warning("Animation: Missing element '%s'", _id.c_str());
		return false;
	}

	parser.readInt(); // unused
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
		frame.surface = element->loadSurface(_id, scene);
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
			frame.surface = _flx->getSurface();
		} else {
			frame.surface = element->loadSurface(filename, scene);
		}
		frame.offset.x = parser.readInt();
		frame.offset.y = parser.readInt();
		_frames.push_back(frame);
	}

	if (_timeline.empty()) {
		_timeline.push_back(1);
	}

	element->setAnimation(this);
	return true;
}

const Frame &Animation::nextFrame(uint32 time) {
	const int kDelay = 50;
	if (time >= _time + kDelay) {
		if (_flx == nullptr) {
			_curFrame = (_curFrame + 1) % _timeline.size();
		} else {
			_flx->nextFrame();
		}
		_time = time;
	}
	int nextFrame = ABS(_timeline[_curFrame]) - 1;
	return _frames[nextFrame];
}

} // End of namespace Orlando
