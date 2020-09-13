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

#ifndef ORLANDO_ANIMATION_H
#define ORLANDO_ANIMATION_H

#include "common/str.h"
#include "common/rect.h"
#include "common/array.h"

namespace Graphics {
	struct Surface;
}
namespace Common {
	class SeekableReadStream;
}

namespace Orlando {

class TextParser;
class FlxAnimation;
class Scene;
class Element;

struct AFrame {
	Graphics::Surface *surface;
	Common::Point offset;
};

enum PlayMode
{
	kPlayNone = 0,
	kPlayOnce,
	kPlayLoop,
	kPlayRecOnce,
	kPlayRecLoop,
	kPlayFlxOnce,
	kPlayFlxLoop,
};

/** Sequence of frames */
typedef Common::Array<int> Record;

/**
 * Represents an animation for a graphical element.
 */
class Animation {
	Common::String _id;
	Common::Array<Record> _records;
	Common::Array<AFrame> _frames;
	FlxAnimation *_flx;

	int _curFrame, _curRecord, _dir;
	uint32 _time, _delay, _begin, _end;
	bool _playing, _loop, _rec;
public:
	Animation(const Common::String &id);
	~Animation();
	void addFrame(const AFrame &frame) { _frames.push_back(frame); }
	bool isPlaying() { return _playing; }
	void setPlaying(bool playing) { _playing = playing; }

	bool load(TextParser &parser, Scene *scene);
	void loadFlx(Common::SeekableReadStream *flx, Scene *scene, uint32 time);
	void play(bool reverse, int delay, PlayMode mode, int rec, uint32 time);
	void nextFrame(uint32 time, const Element *element);
	const AFrame *getFrame() const;
};

} // End of namespace Orlando

#endif
