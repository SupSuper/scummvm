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
#include "common/textconsole.h"
#include "common/util.h"
#include "raunes/datfile.h"

namespace Raunes {

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
	const int kHeaderSize = 46;
	const char kHeader[] = "Udoiana Raunes - Special Edition Datafile ";
	Common::String header = stream->readString(0, ARRAYSIZE(kHeader) - 1);
	if (!header.equals(kHeader)) {
		warning("DatArchive: No header found");
		return false;
	}

	// Skip data chunk
	int dataBytes = stream->readUint32LE();
	stream->skip(dataBytes);

	// Read file table
	int numFiles = stream->readUint32LE();
	_files.resize(numFiles);
	for (int i = 0; i < numFiles; i++) {
		DatFile *file = &_files[i];
		stream->skip(1); // length
		file->filename = stream->readString(0, 12);
		file->position = kHeaderSize + stream->readUint32LE();
		file->width = stream->readUint16LE();
		file->height = stream->readUint16LE();
	}

	return true;
}

const DatFile *DatArchive::findFile(Common::String name) const {
	name.toUppercase();
	for (Common::Array<DatFile>::const_iterator i = _files.begin(); i != _files.end(); ++i) {
		if (i->filename == name) {
			return i;
		}
	}
	return nullptr;
}

Common::SeekableReadStream *DatArchive::readFile(const DatFile *file) {
	if (file == nullptr) {
		return nullptr;
	}
	uint32 size = file->width * file->height;
	if (size == 0) {
		const DatFile *next = file + 1;
		size = next->position - file->position;
	}
	_stream->seek(file->position);
	Common::SeekableReadStream *stream = _stream->readStream(size);
	return stream;
}

} // End of namespace Raunes
