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

#ifndef RAUNES_DATFILE_H
#define RAUNES_DATFILE_H

#include "common/archive.h"
#include "common/array.h"

namespace Raunes {

struct DatFile {
	Common::String filename;
	int position;
	int size;
	int width;
	int height;
};

class DatArchive : public Common::Archive {
	Common::Array<DatFile> _files;
	Common::SeekableReadStream *_stream;

	static const int kHeaderSize = 46;
	const DatFile *findFile(const Common::Path &name) const;

public:
	DatArchive();
	~DatArchive();
	bool open(Common::SeekableReadStream *stream);

	// Common::Archive API implementation
	bool hasFile(const Common::Path &name) const override;
	int listMembers(Common::ArchiveMemberList &list) const override;
	const Common::ArchiveMemberPtr getMember(const Common::Path &name) const override;
	Common::SeekableReadStream *createReadStreamForMember(const Common::Path &name) const override;
};

} // End of namespace Orlando

#endif
