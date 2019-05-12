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

#ifndef ORLANDO_PAK_ARCHIVE_H
#define ORLANDO_PAK_ARCHIVE_H

#include "common/archive.h"

namespace Orlando {

struct PakFile;

/**
 * Represents a Jack Orlando PAK file. PAK files are just a collection
 * of uncompressed files with a header defining their names and locations.
 * For some reason the header is slightly different between game versions.
 */
class PakArchive : public Common::Archive {
	PakFile *_members;
	int _numMembers;
	Common::SeekableReadStream *_stream;

	/**
	 * Loads a PAK file from the DOS version. Header format:
	 * @li uint32: Size of header in bytes.
	 * Followed by each file definition:
	 * @li uint32: Starting position of file in PAK.
	 * @li uint32: Size of file.
	 * @li char[]: Filename zero-padded to 13 chars (8.3).
	 */
	void loadDosPak();
	/**
	 * Loads a PAK file from the Windows version. Header format:
	 * @li char[]: "PAK\0" code.
	 * @li uint32: Number of files stored.
	 * @li uint32: Size of header in bytes (unused).
	 * @li uint32: File handle (unused).
	 * Followed by each file definition:
	 * @li uint32: Starting position of file in PAK.
	 * @li uint32: Size of file.
	 * @li uint32: Length of filename.
	 * @li char[]: Zero-terminated filename.
	 */
	void loadWindowsPak();
	/**
	 * Finds a given filename in the PAK, case-insensitive.
	 * PAKs may contain duplicate files, if so the first found is returned.
	 * @param name Filename to search.
	 * @return File entry or NULL if not found.
	 */
	const PakFile *findFile(Common::String name) const;

public:
	/**
	 * Loads a PAK archive from a stream. If an error occurs, the archive will be empty.
	 * @param stream Pointer to file stream. The archive takes ownership of the stream.
	 */
	PakArchive(Common::SeekableReadStream *stream);
	~PakArchive();
	bool hasFile(const Common::String &name) const override;
	int listMembers(Common::ArchiveMemberList &list) const override;
	const Common::ArchiveMemberPtr getMember(const Common::String &name) const override;
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const override;
};

} // End of namespace Orlando

#endif
