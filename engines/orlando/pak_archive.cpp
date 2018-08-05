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
#include "common/stream.h"
#include "orlando/pak_archive.h"

namespace Orlando {

struct PakFile {
	Common::String filename;
	int offset;
	int size;
};

void PakArchive::loadDosPak() {
	const int kFileHeaderSize = 21;
	int headerSize = _stream->readSint32LE();
	_numMembers = (headerSize - sizeof(int32)) / kFileHeaderSize;

	_members = new PakFile[_numMembers];
	char name[13];
	for (int i = 0; i < _numMembers; i++) {
		_members[i].offset = _stream->readSint32LE();
		_members[i].size = _stream->readSint32LE();
		_stream->read(name, 13);
		_members[i].filename = name;
	}
}

void PakArchive::loadWindowsPak() {
	_numMembers = _stream->readSint32LE();
	_stream->skip(8);

	_members = new PakFile[_numMembers];
	char name[13];
	for (int i = 0; i < _numMembers; i++) {
		_members[i].offset = _stream->readSint32LE();
		_members[i].size = _stream->readSint32LE();
		int nameSize = _stream->readSint32LE();
		_stream->read(name, nameSize);
		_members[i].filename = name;
	}
}

const PakFile *PakArchive::findFile(const Common::String &name) const {
	for (int i = 0; i < _numMembers; i++) {
		if (_members[i].filename.compareToIgnoreCase(name) == 0) {
			return &_members[i];
		}
	}
	return nullptr;
}

PakArchive::PakArchive(Common::SeekableReadStream *stream) : _stream(stream), _members(nullptr), _numMembers(0) {
	if (stream == nullptr)
		return;
	// Determine PAK type	
	const uint32 kWindowsPak = MKTAG('P', 'A', 'K', '\0');
	uint32 tag = _stream->readUint32BE();
	if (tag == kWindowsPak) {
		loadWindowsPak();
	} else {
		_stream->seek(0);
		loadDosPak();
	}
}

PakArchive::~PakArchive() {
	delete[] _members;
	delete _stream;
}

bool PakArchive::hasFile(const Common::String &name) const {
	return (findFile(name) != nullptr);
}

int PakArchive::listMembers(Common::ArchiveMemberList &list) const {
	for (int i = 0; i < _numMembers; i++) {
		list.push_back(Common::ArchiveMemberPtr(new Common::GenericArchiveMember(_members[i].filename, this)));
	}
	return _numMembers;
}

const Common::ArchiveMemberPtr PakArchive::getMember(const Common::String &name) const {
	const PakFile *file = findFile(name);
	if (file == nullptr) {
		return Common::ArchiveMemberPtr();
	} else {
		return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(file->filename, this));
	}
}

Common::SeekableReadStream *PakArchive::createReadStreamForMember(const Common::String &name) const {
	const PakFile *file = findFile(name);
	if (file == nullptr) {
		return nullptr;
	} else {
		_stream->seek(file->offset);
		return _stream->readStream(file->size);
	}
}

} // End of namespace Orlando
