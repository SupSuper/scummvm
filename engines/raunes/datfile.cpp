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
#include "raunes/datfile.h"

namespace Raunes {

const DatFile *DatArchive::findFile(const Common::Path &name) const {
	Common::String str = name.toString();
	str.toUppercase();
	for (Common::Array<DatFile>::const_iterator i = _files.begin(); i != _files.end(); ++i) {
		if (i->filename == str) {
			return i;
		}
	}
	return nullptr;
}

DatArchive::DatArchive() : _stream(nullptr) {}

DatArchive::~DatArchive() {
	delete _stream;
}

bool DatArchive::open(Common::SeekableReadStream *stream) {
	if (stream == nullptr) {
		return false;
	} else {
		_stream = stream;
	}

	// Validate header
	const char kHeader[] = "Udoiana Raunes - Special Edition Datafile ";
	Common::String header = stream->readString(0, ARRAYSIZE(kHeader) - 1);
	if (!header.equals(kHeader)) {
		warning("DatArchive: No header found");
		return false;
	}
	// Skip data chunk
	int dataBytes = stream->readUint32LE();
	stream->skip(dataBytes);

	int numFiles = stream->readUint32LE();
	for (int i = 0; i < numFiles; i++) {
		DatFile file;
		stream->skip(1); // length
		file.filename = stream->readString(0, 12);
		file.position = stream->readUint32LE();
		file.width = stream->readUint16LE();
		file.height = stream->readUint16LE();
		_files.push_back(file);
		// Precalculate size
		if (i > 0)
			_files[i-1].size = _files[i].position - _files[i-1].position;
	}
	_files[numFiles - 1].size = kHeaderSize + dataBytes - _files[numFiles - 1].position;

	return true;
}

bool DatArchive::hasFile(const Common::Path &name) const {
	return (findFile(name) != nullptr);
}

int DatArchive::listMembers(Common::ArchiveMemberList &list) const {
	for (Common::Array<DatFile>::const_iterator i = _files.begin(); i != _files.end(); ++i) {
		list.push_back(Common::ArchiveMemberPtr(new Common::GenericArchiveMember(i->filename, this)));
	}
	return _files.size();
}

const Common::ArchiveMemberPtr DatArchive::getMember(const Common::Path &name) const {
	const DatFile *file = findFile(name);
	if (file == nullptr) {
		return Common::ArchiveMemberPtr();
	} else {
		return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(file->filename, this));
	}
}

Common::SeekableReadStream *DatArchive::createReadStreamForMember(const Common::Path &name) const {
	const DatFile *file = findFile(name);
	if (file == nullptr) {
		return nullptr;
	} else {
		_stream->seek(kHeaderSize + file->position);
		return _stream->readStream(file->size);
	}
}

} // End of namespace Raunes
