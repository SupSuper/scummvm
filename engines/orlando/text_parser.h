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

#ifndef ORLANDO_TEXT_PARSER_H
#define ORLANDO_TEXT_PARSER_H

#include "common/str.h"

namespace Common {
	class SeekableReadStream;
}

namespace Orlando {

/**
 * Parses text files used to define game data, scripts, etc.
 * Contents are whitespace-separated tokens.
 */
class TextParser {
	Common::SeekableReadStream *_stream;

	/**
	 * Reads the next token in the text file.
	 */
	Common::String nextToken();
public:
	/**
	 * Loads the text from a specified stream.
	 * @param stream Pointer to file stream. The parser takes ownership of the stream.
	 * @param encrypted Do the contents need to be decrpyted?
	 */
	TextParser(Common::SeekableReadStream *stream, bool encrypted = true);
	~TextParser();
	/**
	 * Returns the next string value in the file.
	 */
	Common::String readString();
	/**
	 * Returns the next integer value in the file.
	 */
	int readInt();
	/**
	 * Returns the next float value in the file.
	 */
	float readFloat();
};

} // End of namespace Orlando

#endif
