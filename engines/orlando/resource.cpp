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

#include "orlando/resource.h"
#include "orlando/pak_archive.h"
#include "orlando/font.h"

namespace Orlando {

ResourceManager::ResourceManager(OrlandoEngine *vm) : _vm(vm), _globalPak(nullptr), _resourcePak(nullptr), _font(nullptr) {
}

ResourceManager::~ResourceManager() {
	delete _font;
	delete _resourcePak;
	delete _globalPak;
}

bool ResourceManager::loadCommonResources() {
	// If this is a game folder, there should be a GLOBAL.PAK in the root
	// If this is an install disc, the GLOBAL.PAK should be inside a DATA.PAK
	if (!(_globalPak = loadPakArchive("global.pak"))) {
		if (!(_globalPak = loadPakArchive(loadPakFile("data.pak", "global.pak")))) {
			return false;
		}
	}

	if (!(_resourcePak = loadPakArchive("resource.pak")))
		return false;

	if (Common::File *file = loadResourceFile("jack.fnt")) {
		_font = new Font(file);
	} else {
		return false;
	}

	return true;
}

Common::Archive *ResourceManager::loadPakArchive(const Common::String &filename) const {
	Common::File *file = new Common::File();
	if (!file->open(filename)) {
		warning("ResourceManager: Failed to open PAK file: %s", filename.c_str());
		delete file;
		return nullptr;
	}

	return loadPakArchive(file);
}

Common::Archive *ResourceManager::loadPakArchive(Common::File *file) const {
	Common::Archive *archive = new PakArchive(file);
	// Empty PAKs shouldn't exist, assume an error occurred
	Common::ArchiveMemberList list;
	if (archive->listMembers(list) == 0) {
		warning("ResourceManager: Invalid PAK file: %s", file->getName());
		delete archive;
		return nullptr;
	}
	return archive;
}

Common::File *ResourceManager::loadPakFile(Common::Archive &archive, const Common::String &filename) const {
	Common::File *file = new Common::File();
	if (!file->open(filename, archive)) {
		warning("ResourceManager: File not found in PAK: %s", filename.c_str());
		delete file;
		return nullptr;
	}

	return file;
}

Common::File *ResourceManager::loadPakFile(const Common::String &pakName, const Common::String &fileName) const {
	Common::Archive *archive = loadPakArchive(pakName);
	if (archive == nullptr) {
		return nullptr;
	}
	Common::File *file = loadPakFile(*archive, fileName);
	return file;
}

Common::File *ResourceManager::loadRawFile(const Common::String &filename) const {
	Common::File *file = new Common::File();
	if (!file->open(filename)) {
		warning("ResourceManager: Failed to open file: %s", filename.c_str());
		delete file;
		return nullptr;
	}

	return file;
}

Common::File *ResourceManager::loadGlobalFile(const Common::String &filename) const {
	return loadPakFile(*_globalPak, filename);
}

Common::File *ResourceManager::loadResourceFile(const Common::String &filename) const {
	return loadPakFile(*_resourcePak, filename);
}

} // End of namespace Orlando
