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
#include "common/ptr.h"
#include "common/util.h"
#include "orlando/text_parser.h"

namespace Orlando {

/**
 * Wrapper for reading encrpyted text files.
 */
class SeekableCryptReadStream : virtual public Common::SeekableReadStream {
protected:
	Common::DisposablePtr<Common::SeekableReadStream> _parentStream;
public:
	SeekableCryptReadStream(SeekableReadStream *parentStream, DisposeAfterUse::Flag disposeParentStream = DisposeAfterUse::NO)
		: _parentStream(parentStream, disposeParentStream) {
		assert(parentStream);
	}

	virtual uint32 read(void *dataPtr, uint32 dataSize) {
		uint32 bytes = _parentStream->read(dataPtr, dataSize);
		uint8 *data = (uint8 *)dataPtr;
		for (uint32 i = 0; i < bytes; i++) {
			*data = ~(*data - 7); // top secret
			data++;
		}
		return bytes;
	}
	virtual bool eos() const { return _parentStream->eos(); }
	virtual bool err() const { return _parentStream->err(); }
	virtual void clearErr() { _parentStream->clearErr(); }
	virtual int32 pos() const { return _parentStream->pos(); }
	virtual int32 size() const { return _parentStream->size(); }
	virtual bool seek(int32 offset, int whence = SEEK_SET) { return _parentStream->seek(offset, whence); }
};

TextParser::TextParser(Common::SeekableReadStream *stream, bool encrypted) : _stream(stream) {
	if (encrypted) {
		_stream = new SeekableCryptReadStream(stream, DisposeAfterUse::YES);
	}
	_stream->seek(0);
}

TextParser::~TextParser() {
	delete _stream;
}

Common::String TextParser::nextToken() {
	Common::String token;
	bool comment = false;
	bool tokenRead = false;
	bool end = false;
	while (!end) {
		char c = _stream->readByte();
		if (_stream->eos() || _stream->err()) {
			end = true;
		} else {
			if (c == '/') {
				// Ignore comments / like this /
				comment = !comment;
			} else if (comment || Common::isSpace(c)) {
				// Skip whitespace
				end = tokenRead;
			} else {
				token += c;
				tokenRead = true;
			}
		}
	}
	return token;
}

Common::String TextParser::readString() {
	Common::String token = nextToken();
	return token;
}

int TextParser::readInt() {
	Common::String token = nextToken();
	return atoi(token.c_str());
}

float TextParser::readFloat() {
	Common::String token = nextToken();
	return atof(token.c_str());
}

} // End of namespace Orlando
