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

#ifndef ORLANDO_RESOURCE_H
#define ORLANDO_RESOURCE_H

namespace Common {
	class File;
	class Archive;
	class String;
}

namespace Graphics {
	class Font;
}

namespace Orlando {

class OrlandoEngine;

/**
  * Manages global resources and resource acquisition.
  */
class ResourceManager {
	OrlandoEngine *_vm;
	Common::Archive *_globalPak, *_resourcePak;
	Graphics::Font *_font;

public:
	ResourceManager(OrlandoEngine *vm);
	~ResourceManager();
	/**
	 * Loads and stores resources needed throughout the game.
	 * @return False if a file wasn't found.
	 */
	bool loadGlobalResources();
	/**
	 * Loads a specific PAK archive.
	 * @param filename Path relative to game folder.
	 * @return Loaded resource, or nullptr if an error occured.
	 */
	Common::Archive *loadPakArchive(const Common::String &filename) const;
	/**
	* Loads a file contained in a PAK archive.
	* @param archive Archive to search.
	* @param filename File to load.
	* @return Loaded resource, or nullptr if an error occured.
	*/
	Common::File *loadPakFile(Common::Archive &archive, const Common::String &filename) const;

	Graphics::Font *getFont() { return _font; }
};

} // End of namespace Orlando

#endif
