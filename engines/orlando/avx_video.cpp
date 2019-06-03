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
#include "common/system.h"

#include "orlando/avx_video.h"
#include "orlando/flx_anim.h"
#include "orlando/orlando.h"
#include "orlando/graphics.h"
#include "orlando/sound.h"

namespace Orlando {

/**
 * Ring buffer based on MemoryReadyWriteStream. Used for reading video audio chunks.
 */
class MemoryAudioStream : public Common::SeekableReadStream, public Common::SeekableWriteStream {
private:
	uint32 _capacity, _size;
	byte *_data;
	uint32 _writePos, _readPos, _writeTotal, _readTotal;
	DisposeAfterUse::Flag _disposeMemory;
	bool _eos;

public:
	MemoryAudioStream(uint32 capacity, uint32 size, DisposeAfterUse::Flag disposeMemory)
		: _capacity(capacity), _size(size), _writePos(0), _readPos(0), _writeTotal(0), _readTotal(0), _disposeMemory(disposeMemory), _eos(false) {
		_data = new byte[capacity];
	}

	~MemoryAudioStream() {
		if (_disposeMemory)
			delete[] _data;
	}

	virtual uint32 write(const void *dataPtr, uint32 dataSize) override {
		if (_writePos + dataSize < _capacity) {
			memcpy(_data + _writePos, dataPtr, dataSize);
		} else {
			memcpy(_data + _writePos, dataPtr, _capacity - _writePos);
			const byte *shiftedPtr = (const byte *)dataPtr + _capacity - _writePos;
			memcpy(_data, shiftedPtr, dataSize - (_capacity - _writePos));
		}
		_writePos = (_writePos + dataSize) % _capacity;
		_writeTotal += dataSize;
		return dataSize;
	}

	virtual uint32 read(void *dataPtr, uint32 dataSize) override {
		if (_readTotal + dataSize > _size) {
			dataSize = _size - _readTotal;
			_eos = true;
		}
		if (dataSize == 0 || _capacity == 0) return 0;
		if (_readTotal + dataSize > _writeTotal) {
			warning("MemoryAudioBuffer: Ran out of buffer");
			return 0;
		}
		if (_readPos + dataSize < _capacity) {
			memcpy(dataPtr, _data + _readPos, dataSize);
		} else {
			memcpy(dataPtr, _data + _readPos, _capacity - _readPos);
			byte *shiftedPtr = (byte *)dataPtr + _capacity - _readPos;
			memcpy(shiftedPtr, _data, dataSize - (_capacity - _readPos));
		}
		_readPos = (_readPos + dataSize) % _capacity;
		_readTotal += dataSize;
		return dataSize;
	}

	virtual int32 pos() const override { return _readTotal; }
	virtual int32 size() const override { return _size; }
	virtual bool seek(int32, int) override { return false; }
	virtual bool eos() const override { return _eos; }
	virtual void clearErr() override { _eos = false; }
};

AvxVideo::AvxVideo(OrlandoEngine *vm, Common::SeekableReadStream *stream) : Scene(vm, "MOVIE"), _stream(stream), _audio(nullptr),
	_flx(nullptr), _flxTotal(0), _flxCurrent(0) {
	_audioBuffer = new byte[kAudioStart];
}

AvxVideo::~AvxVideo() {
	if (_flx != nullptr)
		delete _flx;
	delete[] _audioBuffer;
}

bool AvxVideo::initialize() {
	_flxTotal = _stream->readUint16LE();
	_stream->readByte();
	byte tag = _stream->readByte();
	if (tag != 16) {
		warning("AvxVideo: Missing tag");
		return false;
	}

	_stream->skip(4);
	int audioSize = _stream->readUint32LE();

	_stream->seek(4);
	const int kRingBuffer = 1 * 1024 * 1024;
	_audio = new MemoryAudioStream(kRingBuffer, audioSize, DisposeAfterUse::YES);
	_audio->write(_audioBuffer, _stream->read(_audioBuffer, kAudioStart));
	_vm->getSoundManager()->playFile(_audio, Audio::Mixer::kMusicSoundType);

	_stream->skip(8);
	_flx = new FlxAnimation(_stream, _vm->getGraphicsManager()->kScreenFormat, DisposeAfterUse::NO);
	_flxCurrent++;

	return true;
}

bool AvxVideo::run() {
	_vm->getGraphicsManager()->draw(*_flx->getSurface());

	uint32 chunk = _stream->readUint32LE();
	if (!_stream->eos()) {
		_audio->write(_audioBuffer, _stream->read(_audioBuffer, chunk - 4));
		_stream->skip(4);
		if (!_flx->nextFrame() && _flxCurrent < _flxTotal) {
			delete _flx;
			_flx = new FlxAnimation(_stream, _vm->getGraphicsManager()->kScreenFormat, DisposeAfterUse::NO);
			_flxCurrent++;
		}
	}

	_vm->_system->delayMillis(25);

	return true;
}

} // End of namespace Orlando
