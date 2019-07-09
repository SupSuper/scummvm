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
#include "common/file.h"
#include "common/stream.h"
#include "common/system.h"
#include "common/config-manager.h"
#include "graphics/surface.h"

#include "orlando/avx_video.h"
#include "orlando/flx_anim.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/graphics.h"
#include "orlando/sound.h"
#include "orlando/mouse.h"
#include "orlando/main_menu.h"

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

AvxSubtitles::AvxSubtitles(Common::SeekableReadStream *stream, const Graphics::PixelFormat &format, bool bpp16) : _stream(stream), _format(format), _bpp16(bpp16),
	_surface(nullptr) {
	nextFrame();
}

AvxSubtitles::~AvxSubtitles() {
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
	delete _stream;
}

bool AvxSubtitles::nextFrame() {
	if (_bpp16)
		return decodeFrame16();
	else
		return decodeFrame24();
}

bool AvxSubtitles::decodeFrame16() {
	_frameStart = _stream->readUint16LE();
	if (_stream->eos())
		return false;
	_frameEnd = _stream->readUint16LE();
	_pos.x = _stream->readUint16LE();
	_pos.y = _stream->readUint16LE();
	_stream->skip(2);

	int width = _stream->readUint16LE();
	int height = _stream->readUint16LE();
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
	_surface = new Graphics::Surface();
	_surface->create(width, height, _format);
	_stream->read(_surface->getPixels(), width * height * _format.bytesPerPixel);

	return true;
}

bool AvxSubtitles::decodeFrame24() {
	int width = _stream->readUint16LE();
	if (_stream->eos())
		return false;
	int height = _stream->readUint16LE();
	if (_surface != nullptr) {
		_surface->free();
	}
	delete _surface;
	_surface = new Graphics::Surface();
	_surface->create(width, height, _format);

	_pos.x = _stream->readUint16LE();
	_pos.y = _stream->readUint16LE();
	_frameStart = _stream->readUint16LE();
	_frameEnd = _stream->readUint16LE();
	_flx = _stream->readUint16LE();

	uint8 r = _stream->readByte();
	uint8 g = _stream->readByte();
	uint8 b = _stream->readByte();
	uint16 color = _format.RGBToColor(r, g, b);
	_stream->skip(7);

	uint16 *pixel = (uint16 *)_surface->getPixels();
	for (int i = 0; i < width * height; i++) {
		byte data = _stream->readByte();
		if (data == 0x00) {
			*pixel = 1;
		} else if (data == 0xFF) {
			*pixel = color;
		}
		pixel++;
	}
	return true;
}

void AvxSubtitles::draw(GraphicsManager *graphics, int frame, int flx) {
	int flxSub = _bpp16 ? flx : _flx;
	if (flxSub == flx && frame >= _frameStart && frame < _frameEnd) {
		graphics->drawTransparent(*_surface, _pos);
	}
	if ((flxSub == flx && frame == _frameEnd) || (flx > flxSub)) {
		nextFrame();
	}
}

AvxVideo::AvxVideo(OrlandoEngine *vm, const Common::String &id) : Scene(vm, id), _audio(nullptr), _audioBuffer(new byte[kAudioStart]),
	_flx(nullptr), _flxTotal(0), _flxCurrent(0), _time(0), _subtitles(nullptr), _outro(false) {
}

AvxVideo::~AvxVideo() {
	delete _subtitles;
	delete _flx;
	delete[] _audioBuffer;
}

bool AvxVideo::initialize() {
	ResourceManager *resources = _vm->getResourceManager();
	GraphicsManager *graphics = _vm->getGraphicsManager();

	_stream = resources->loadRawFile(_id + ".AVX");
	if (_stream == nullptr) {
		return false;
	}

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
	const int kRingBuffer = 10 * 1024 * 1024;
	_audio = new MemoryAudioStream(kRingBuffer, audioSize, DisposeAfterUse::YES);
	_audio->write(_audioBuffer, _stream->read(_audioBuffer, kAudioStart));

	_stream->skip(8);
	_flx = new FlxAnimation(_stream, graphics->kScreenFormat, DisposeAfterUse::NO);
	_flx->nextFrame();
	_flxCurrent++;

	if (ConfMan.getBool("subtitles")) {
		Common::File *dlg = resources->loadRawFile(_id + ".DLG");
		if (dlg != nullptr) {
			_subtitles = new AvxSubtitles(dlg, graphics->kScreenFormat, _flx->is16Bpp());
		}
	}

	if (_id == "OUTRO16") {
		_outro = true;
		_surfaceOutro = graphics->loadRawBitmap(resources->loadRawFile("OUTRO16.BM"));
		_surfaceCredits = graphics->loadPaletteBitmap(resources->loadRawFile("CREDITS.PBM"));
		_surfaceCykl[0] = graphics->loadPaletteBitmap(resources->loadRawFile("CYKL1.BM"));
		_surfaceCykl[1] = graphics->loadPaletteBitmap(resources->loadRawFile("CYKL2.BM"));
		_surfaceCykl[2] = graphics->loadPaletteBitmap(resources->loadRawFile("CYKL3.BM"));
	}

	_vm->getMouse()->show(false);
	_vm->_mixer->stopAll();
	_vm->getSoundManager()->playFile(_audio, Audio::Mixer::kMusicSoundType);

	_time = _vm->getTotalPlayTime();
	_delay = kMsPerFrame / _flx->getFps();

	return true;
}

bool AvxVideo::run() {
	GraphicsManager *graphics = _vm->getGraphicsManager();
	uint32 time = _vm->getTotalPlayTime();

	while (time >= _time + _delay) {
		_time += _delay;

		int flx = _flxCurrent - 1;
		int frame = _flx->getFrame() - 1;
		if (_outro) {
			if (flx >= 0 && flx < 4) {
				graphics->draw(*_surfaceOutro);
			} else if (flx >= 5 && flx < 19) {
				graphics->draw(*_surfaceCredits);
			}
			if (flx >= 7 && flx < 19) {
				int step = frame % 3;
				graphics->draw(*_surfaceCykl[step], Common::Point(35, 0));
			}
			graphics->drawTransparent(*_flx->getSurface());
		} else {
			graphics->draw(*_flx->getSurface());
		}

		if (_subtitles != nullptr) {
			_subtitles->draw(graphics, frame, flx);
		}

		uint32 chunk = _stream->readUint32LE();
		if (!_stream->eos()) {
			_audio->write(_audioBuffer, _stream->read(_audioBuffer, chunk - 4));
			_stream->skip(4);
			if (!_flx->nextFrame() && _flxCurrent < _flxTotal) {
				delete _flx;
				_flx = new FlxAnimation(_stream, graphics->kScreenFormat, DisposeAfterUse::NO);
				_flx->nextFrame();
				_flxCurrent++;
				_delay = kMsPerFrame / _flx->getFps();
			}
		}
	}

	if (_stream->eos() || _vm->getMouse()->getLeftButton() == kButtonReleased) {
		_vm->gotoScene(new MainMenu(_vm));
	}

	return true;
}

} // End of namespace Orlando
