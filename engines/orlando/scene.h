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

#ifndef ORLANDO_SCENE_H
#define ORLANDO_SCENE_H

#include "common/str.h"

namespace Common {
	class Archive;
	class File;
}

namespace Orlando {

class OrlandoEngine;

/**
  * Represents a game room, screen, etc.
  */
class Scene {
protected:
	OrlandoEngine *_vm;
	Common::String _id;
	Common::Archive *_pak, *_pakEx;

	Common::File *loadFile(const Common::String &filename);

public:
	Scene(OrlandoEngine *vm, const Common::String &id);
	~Scene();

	virtual bool initialize();
	virtual bool run();

	void playMusic(const Common::String &filename);
	void playSfx(const Common::String &filename);
	void playSpeech(const Common::String &filename);
};

} // End of namespace Orlando

#endif
