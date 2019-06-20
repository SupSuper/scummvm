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
#include "common/array.h"
#include "common/hash-str.h"
#include "orlando/polygon.h"

namespace Common {
	class Archive;
	class File;
}
namespace Graphics {
	struct Surface;
}

namespace Orlando {

class OrlandoEngine;
class GraphicsManager;
class Element;
class Dialog;
class Face;
class Person;
class Area;

/**
 * Represents a game room, screen, etc.
 */
class Scene {
protected:
	OrlandoEngine *_vm;
	Common::String _id;
	Common::Archive *_pak, *_pakEx;

	Graphics::Surface *_background;
	int _scrollX;
	Common::Array<Triangle> _walkRegions;

	Common::HashMap<Common::String, Element*> _elements;
	Common::HashMap<int, Dialog*> _dialogs;
	Common::HashMap<Common::String, Face*> _faces;
	Common::HashMap<Common::String, Area*> _areas;

	/** Loads configurations and graphics. */
	bool loadCcg();
	/** Loads dialogs. */
	bool loadDcn();
	/** Loads faces. */
	bool loadFcc();
	/** Loads persons. */
	bool loadPcs();
	/** Loads areas. */
	bool loadAce();
	/** Loads animations. */
	bool loadAci();
	/** Loads sounds. */
	bool loadSsd();
	/** Loads insertions. */
	bool loadIcs();
	/** Loads films. */
	bool loadFcm();
	/** Loads script macros. */
	bool loadMcc();

public:
	Scene(OrlandoEngine *vm, const Common::String &id);
	virtual ~Scene();

	GraphicsManager *getGraphicsManager() const;
	Element *getElement(const Common::String &id) { return _elements[id]; }

	Common::File *loadFile(const Common::String &filename);
	Graphics::Surface *loadSurface(const Common::String &filename, int bpp);
	bool playMusic(const Common::String &filename);
	bool playSfx(const Common::String &filename);
	bool playSpeech(const Common::String &filename);

	/**
	 * Initializes the scene and its contents.
	 * @return False if a resource is missing.
	 */
	virtual bool initialize();
	/**
	 * Executes scene logic every frame.
	 * @return False if an error occurs.
	 */
	virtual bool run();
};

} // End of namespace Orlando

#endif
