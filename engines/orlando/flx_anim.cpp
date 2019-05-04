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
#include "common/substream.h"
#include "common/rect.h"
#include "common/util.h"
#include "graphics/surface.h"
#include "orlando/flx_anim.h"

namespace Orlando {

/**
 * Wrapper for decoding RLE frames on the fly. Runs are made of 3 bytes:
 * @li uint8 - 0xFF flagging start of run.
 * @li uint8 - Value of run.
 * @li uint8 - Length of run.
 */
class SeekableRleReadStream : virtual public Common::SeekableReadStream {
protected:
	Common::DisposablePtr<Common::SeekableReadStream> _parentStream;
	uint32 _repeatVal, _repeatLen;
public:
	SeekableRleReadStream(SeekableReadStream *parentStream, DisposeAfterUse::Flag disposeParentStream = DisposeAfterUse::NO)
		: _parentStream(parentStream, disposeParentStream), _repeatVal(0), _repeatLen(0) {
		assert(parentStream);
	}

	virtual uint32 read(void *dataPtr, uint32 dataSize) {
		byte *ptr = (byte*)dataPtr;
		uint32 size = 0;
		while (size < dataSize) {
			if (_repeatLen > 0) {
				int read = MIN(dataSize - size, _repeatLen);
				memset(ptr, _repeatVal, read);
				size += read;
				_repeatLen -= read;
				ptr += read;
			} else {
				uint8 byte = _parentStream->readByte();
				if (eos())
					break;
				if (byte == 255) {
					_repeatVal = _parentStream->readByte();
					_repeatLen = _parentStream->readByte();
				} else {
					*ptr = byte;
					ptr++;
					size++;
				}
			}
		}

		return size;
	}
	virtual bool eos() const { return _repeatLen == 0 && _parentStream->eos(); }
	virtual bool err() const { return _parentStream->err(); }
	virtual void clearErr() { _parentStream->clearErr(); }
	virtual int32 pos() const { return _parentStream->pos(); }
	virtual int32 size() const { return _parentStream->size(); }
	virtual bool seek(int32 offset, int whence = SEEK_SET) { return _parentStream->seek(offset, whence); }
};

FlxAnimation::FlxAnimation(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format) : _stream(stream), _surface(nullptr) {
	if (stream == nullptr)
		return;

	// Validate FLX header tag
	const uint32 kFlxTag = MKTAG('F', 'L', 'X', '\0');
	uint32 tag = _stream->readUint32BE();
	if (tag != kFlxTag)
		return;

	int frames = stream->readUint16LE();
	int width = stream->readUint16LE();
	int height = stream->readUint16LE();
	_surface = new Graphics::Surface();
	_surface->create(width, height, format);

	stream->seek(kHeaderSize);
	nextFrame();
}

FlxAnimation::~FlxAnimation() {
	if (_surface) {
		_surface->free();
	}
	delete _surface;
	delete _stream;
}

void FlxAnimation::nextFrame() {
	enum FlxChunk {
		kFlxBlank = 0,
		kFlxFrame = 1,
		kFlxRleFrame = 2,
		kFlxUnknown = 3,
		kFlxPalette = 4
	};

	bool readFrame = false;
	while (!readFrame) {
		FlxChunk chunkType = (FlxChunk)_stream->readUint16LE();
		int chunkSize = _stream->readUint32LE();
		int dataSize = chunkSize - 6;

		switch (chunkType) {
		case kFlxBlank:
			_stream->skip(dataSize);
			readFrame = true;
			break;
		case kFlxFrame:
			decodeFrame(new Common::SeekableSubReadStream(_stream, _stream->pos(), _stream->pos() + dataSize));
			readFrame = true;
			break;
		case kFlxRleFrame:
			decodeFrame(new SeekableRleReadStream(new Common::SeekableSubReadStream(_stream, _stream->pos() + 4, _stream->pos() + dataSize), DisposeAfterUse::YES));
			readFrame = true;
			break;
		case kFlxUnknown:
			_stream->skip(dataSize); // Not used by game
			break;
		case kFlxPalette:
			_stream->read(_palette, dataSize);
			_surface->fillRect(Common::Rect(0, 0, _surface->w, _surface->h), 0); // Clear surface
			break;
		}		

		// Rewind if we've reached the end
		if (_stream->eos()) {
			_stream->seek(kHeaderSize);
		}
	}
}

void FlxAnimation::decodeFrame(Common::ReadStream *frame) {
	uint16 *pixel = (uint16*)_surface->getPixels();
	int bigSkip = frame->readUint32LE();
	pixel += bigSkip;
	
	while (!frame->eos()) {
		int skip = frame->readByte();
		if (frame->eos())
			break;
		int write = frame->readByte();
		pixel += skip;
        for (int j = 0; j < write; j++) {
			int color = frame->readByte();
			if (frame->eos())
				break;
            *pixel = _palette[color];
			pixel++;
        }
	}

	delete frame;
}

} // End of namespace Orlando
