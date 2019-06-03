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
#include "common\textconsole.h"
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
				*ptr = _repeatVal;
				ptr++;
				size++;
				_repeatLen--;
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

FlxAnimation::FlxAnimation(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format, DisposeAfterUse::Flag disposeAfterUse)
	: _stream(stream), _disposeAfterUse(disposeAfterUse), _surface(nullptr), _surface8Bpp(nullptr), _16bit(true), _frameCurrent(0), _frameTotal(0) {
	if (stream == nullptr)
		return;

	// Validate FLX header tag
	const uint32 kFlxTag = MKTAG('F', 'L', 'X', '\0');
	uint32 tag = _stream->readUint32BE();
	if (tag != kFlxTag) {
		warning("FlxAnimation: Missing FLX tag");
		return;
	}

	_frameTotal = stream->readUint16LE();
	int width = stream->readUint16LE();
	int height = stream->readUint16LE();
	_surface = new Graphics::Surface();
	_surface->create(width, height, format);
	_surface8Bpp = (byte*)calloc(width * height, sizeof(byte));

	stream->skip(4);
	_16bit = stream->readByte() ? true : false;
	stream->skip(5);
	nextFrame();
}

FlxAnimation::~FlxAnimation() {
	free(_surface8Bpp);
	if (_surface) {
		_surface->free();
	}
	delete _surface;
	if (_disposeAfterUse) {
		delete _stream;
	}
}

bool FlxAnimation::nextFrame() {
	enum FlxChunk {
		kFlxBlank = 0,
		kFlxFrame = 1,
		kFlxRleFrame = 2,
		kFlxUnknown = 3,
		kFlxPalette = 4
	};

	if (_frameCurrent == _frameTotal)
		return false;

	bool readFrame = false;
	while (!readFrame) {
		FlxChunk chunkType = (FlxChunk)_stream->readUint16LE();
		if (_stream->eos())
			break;

		if (chunkType > 4) {
			_stream->skip(2);
			chunkType = (FlxChunk)_stream->readUint16LE();
		}

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
			if (_16bit) {
				_stream->read(_palette, dataSize);
			} else {
				int colors = dataSize / 3;
				for (int i = 0; i < colors; i++) {
					uint8 r = _stream->readByte();
					uint8 g = _stream->readByte();
					uint8 b = _stream->readByte();
					_palette[i] = _surface->format.RGBToColor(r, g, b);
				}
			}
			uint16 *pixel = (uint16 *)_surface->getPixels();
			byte *pixel8 = _surface8Bpp;
			for (int i = 0; i < _surface->w * _surface->h; i++) {
				*pixel = _palette[*pixel8];
				pixel++;
				pixel8++;
			}
			break;
		}
	}

	_frameCurrent++;
	return true;
}

void FlxAnimation::decodeFrame(Common::ReadStream *frame) {
	uint16 *pixel = (uint16*)_surface->getPixels();
	byte *pixel8 = _surface8Bpp;
	int bigSkip = frame->readUint32LE();
	pixel += bigSkip;
	pixel8 += bigSkip;
	
	while (!frame->eos()) {
		int skip = frame->readByte();
		if (frame->eos())
			break;
		int write = frame->readByte();
		pixel += skip;
		pixel8 += skip;
        for (int j = 0; j < write; j++) {
			byte color = frame->readByte();
			if (frame->eos())
				break;
            *pixel = _palette[color];
			*pixel8 = color;
			pixel++;
			pixel8++;
        }
	}

	delete frame;
}

} // End of namespace Orlando
