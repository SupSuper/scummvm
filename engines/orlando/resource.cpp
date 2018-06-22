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

namespace Orlando {

ResourceManager::ResourceManager(OrlandoEngine *vm) : _vm(vm), _globalPak(nullptr), _resourcePak(nullptr) {
}

ResourceManager::~ResourceManager() {
	delete _resourcePak;
	delete _globalPak;
}

bool ResourceManager::loadGlobalResources() {
	if (!(_globalPak = loadPakArchive("global.pak")))
		return false;

	if (!(_resourcePak = loadPakArchive("resource/resource.pak")))
		return false;

	return true;
}

Common::Archive *ResourceManager::loadPakArchive(const Common::String &filename) const {
	Common::File *file = new Common::File();
	if (!file->open(filename)) {
		error("ResourceManager: Failed to open PAK file: %s", filename.c_str());
		delete file;
		return nullptr;
	}

	Common::Archive *archive = new PakArchive(file);
	// Empty PAKs shouldn't exist, assume an error occurred
	Common::ArchiveMemberList list;
	if (archive->listMembers(list) == 0) {
		error("ResourceManager: Invalid PAK file: %s", filename.c_str());
		delete archive;
		return nullptr;
	}
	return archive;
}

Common::File *ResourceManager::loadPakFile(Common::Archive &archive, const Common::String &filename) const {
	Common::File *file = new Common::File();
	if (!file->open(filename, archive)) {
		error("ResourceManager: File not found in PAK: %s", filename.c_str());
		delete file;
		return nullptr;
	}

	return file;
}

} // End of namespace Orlando
