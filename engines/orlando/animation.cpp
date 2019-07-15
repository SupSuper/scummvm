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
#include "orlando/util.h"

namespace Orlando {

Animation::Animation(const Common::String &id) : _id(id), _flx(nullptr), _curFrame(0), _curRecord(0), _dir(1),
	_time(0), _delay(0), _begin(0), _end(0), _playing(false), _loop(false), _rec(false) {
}

Animation::~Animation() {
	if (_flx == nullptr) {
		for (Common::Array<AFrame>::const_iterator i = _frames.begin(); i != _frames.end(); ++i) {
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

	parser.readInt();
	while (!parser.eof()) {
		Common::String rec = parser.readString();
		if (rec == "REC") {
			Record record;
			while (!parser.eof()) {
				Common::String frame = parser.readString();
				if (frame == "END") {
					_records.push_back(record);
					break;
				}
				record.push_back(toInt(frame));
			}
		} else {
			parser.rewind();
			break;
		}
	}

	if (!_id.hasPrefix("FLX")) {
		AFrame frame;
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
		AFrame frame;
		if (filename.hasSuffix(".FLX")) {
			Common::File *flx = scene->loadFile(filename);
			if (flx == nullptr)
				return false;
			_flx = new FlxAnimation(flx, scene->getGraphicsManager()->kScreenFormat);
			frame.surface = _flx->getSurface();
		} else {
			frame.surface = element->loadSurface(filename, scene);
			if (!frame.surface)
				return false;
		}
		frame.offset.x = parser.readInt();
		frame.offset.y = parser.readInt();
		_frames.push_back(frame);
	}

	element->setAnimation(this);
	return true;
}

void Animation::loadFlx(Common::SeekableReadStream *flx, Scene *scene, uint32 time) {
	_flx = new FlxAnimation(flx, scene->getGraphicsManager()->kScreenFormat);
	AFrame frame = { _flx->getSurface() };
	_frames.push_back(frame);
	play(false, 1000 / _flx->getFps(), kPlayFlxLoop, 0, time);
}

void Animation::play(bool reverse, int delay, PlayMode mode, int rec, uint32 time) {
	_dir = (reverse) ? -1 : 1;
	_delay = delay;
	_curRecord = rec;
	_time = time;
	_loop = (mode == kPlayLoop || mode == kPlayRecLoop || mode == kPlayFlxLoop);
	_rec = (mode == kPlayRecOnce || mode == kPlayRecLoop);

	_begin = 0;
	_end = _frames.size();
	if (_rec)
		_end = _records[_curRecord].size();
	_end--;

	_curFrame = (reverse) ? _end : _begin;
	_playing = true;
}

const AFrame *Animation::nextFrame(uint32 time) {
	while (_playing && time >= _time + _delay) {
		_time += _delay;
		if (_flx) {
			_playing = _flx->nextFrame(_loop);
		} else {
			_curFrame += _dir;
			if (_loop) {
				if (_curFrame > _end) {
					_curFrame = _begin;
				}
				else if (_curFrame < _begin) {
					_curFrame = _end;
				}
			} else {
				if (_curFrame < _begin || _curFrame > _end) {
					_curFrame -= _dir;
					_playing = false;
				}
			}
		}
	}
	int nextFrame = _curFrame;
	if (_rec)
		nextFrame = ABS(_records[_curRecord][_curFrame]) - 1;
	return &_frames[nextFrame];
}

} // End of namespace Orlando
