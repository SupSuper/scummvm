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

#include "audio/mixer.h"
#include "audio/audiostream.h"
#include "audio/decoders/adpcm.h"
#include "audio/decoders/adpcm_intern.h"
#include "common/file.h"

// TODO: Integrate into ScummVM
#include "orlando/libgsm.h"

#include "orlando/sound.h"
#include "orlando/orlando.h"

namespace Orlando {

enum GSMType {
	kGSM610,	// libgsm format
	kGSM610MS	// Microsoft format (WAV #49)
};

/**
 * GSM 06.10
 * https://wiki.multimedia.cx/index.php/GSM_06.10
 */
class GSMStream : public Audio::SeekableAudioStream {
	static const int kSamplesPerFrame = 160;
	Common::DisposablePtr<Common::SeekableReadStream> _stream;
	int32 _startpos;
	const int32 _endpos;
	int _rate;

	libgsm::gsm _handle;
	libgsm::gsm_frame _frame;
	libgsm::gsm_signal _decodedSamples[kSamplesPerFrame];
	uint8 _decodedSampleCount;

public:
	GSMStream(Common::SeekableReadStream *stream, DisposeAfterUse::Flag disposeAfterUse, GSMType type, uint32 size, int rate = 8000) :
		_stream(stream, disposeAfterUse), _startpos(stream->pos()),	_endpos(_startpos + size), _rate(rate), _decodedSampleCount(0) {
		_handle = libgsm::gsm_create();
		if (type == kGSM610MS) {
			int flag = 1;
			libgsm::gsm_option(_handle, libgsm::GSM_OPT_WAV49, &flag);
		}
	}

	~GSMStream() {
		libgsm::gsm_destroy(_handle);
	}

	virtual int readBuffer(int16 *buffer, const int numSamples) override {
		int samples;

		for (samples = 0; samples < numSamples && !endOfData(); samples++) {
			if (_decodedSampleCount == 0) {
				int frameIndex = gsm_option(_handle, libgsm::GSM_OPT_FRAME_INDEX, nullptr);
				int frameSize = sizeof(libgsm::gsm_frame) - frameIndex;
				_stream->read(&_frame, frameSize);
				gsm_decode(_handle, _frame, _decodedSamples);
				_decodedSampleCount = kSamplesPerFrame;
			}

			// (n - 1) - (count - 1) ensures that _decodedSamples acts as a FIFO of depth n
			int i = (kSamplesPerFrame - 1) - (_decodedSampleCount - 1);
			buffer[samples] = _decodedSamples[i];
			_decodedSampleCount--;
		}

		return samples;
	}

	virtual bool isStereo() const override { return false; }
	virtual int getRate() const override { return _rate; }
	virtual bool endOfData() const override { return (_stream->eos() || _stream->pos() >= _endpos); }
	// TODO: ?
	virtual bool seek(const Audio::Timestamp &where) override { return false; }
	virtual Audio::Timestamp getLength() const override { return -1; }
};

/**
 * This IMA ADPCM variant is processed 32-bit at a time, rather than 8-bit.
 * In case of stereo data, each 32-bit alternates between left and right channel data.
 * So we need to do some manipulation to get the samples in the format ScummVM expects.
 */
class Orlando_ADPCMStream : public Audio::Ima_ADPCMStream {
	static const int kSamplesPerData = 8; // nibbles per uint32
	const uint8 kDecodedSampleMax;
	uint8 _decodedSampleCount;
	int16 _decodedSamples[kSamplesPerData * 2]; // make room for two channels even if we only need one

public:
	Orlando_ADPCMStream(Common::SeekableReadStream *stream, DisposeAfterUse::Flag disposeAfterUse, uint32 size, int rate, int channels)
		: Ima_ADPCMStream(stream, disposeAfterUse, size, rate, channels, 0), kDecodedSampleMax(kSamplesPerData * _channels), _decodedSampleCount(0)  {
	}

	virtual bool endOfData() const override {
		return (_stream->eos() || _stream->pos() >= _endpos) && (_decodedSampleCount == 0);
	}

	virtual int readBuffer(int16 *buffer, const int numSamples) override {
		int samples;

		for (samples = 0; samples < numSamples && !endOfData(); samples++) {
			if (_decodedSampleCount == 0) {
				for (int channel = 0; channel < _channels; channel++) {
					uint32 data = _stream->readUint32LE();
					for (int i = 0; i < kSamplesPerData; i++) {
						_decodedSamples[i + channel * kSamplesPerData] = decodeIMA((data >> i * 4) & 0x0f, channel);
					}
				}
				_decodedSampleCount = kDecodedSampleMax;
			}

			// (n - 1) - (count - 1) ensures that _decodedSamples acts as a FIFO of depth n
			int i = (kDecodedSampleMax - 1) - (_decodedSampleCount - 1);
			// need to interleave samples from each channel
			int channel = i % _channels;
			buffer[samples] = _decodedSamples[i / _channels + channel * kSamplesPerData];
			_decodedSampleCount--;
		}

		return samples;
	}
};

SoundManager::SoundManager(OrlandoEngine *vm) : _vm(vm), _handle(nullptr) {
}

SoundManager::~SoundManager() {
	delete _handle;
}

void SoundManager::playFile(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type) {
	enum AudioFormat {
		kAudioAdpcmMono = 50,	// sound fx
		kAudioAdpcmStereo = 51,	// music
		kAudioGSM52 = 52,		// unused?
		kAudioGSM53 = 53		// voice
	};
	AudioFormat format;

	format = (AudioFormat)stream->readUint32LE();
	uint32 size = stream->readUint32LE();

	Audio::SeekableAudioStream *audio = nullptr;
	switch (format) {
	case kAudioAdpcmMono:
		stream->seek(24); // skip header
		audio = new Orlando_ADPCMStream(stream, DisposeAfterUse::YES, size, 44100, 1);
		break;
	case kAudioAdpcmStereo:
		stream->seek(24);
		audio = new Orlando_ADPCMStream(stream, DisposeAfterUse::YES, size, 44100, 2);
		break;
	case kAudioGSM52:
	case kAudioGSM53:
		stream->seek(36);
		audio = new GSMStream(stream, DisposeAfterUse::YES, kGSM610MS, size, 22050);
		break;
	default:
		error("SoundManager: Unrecognized audio format: %d", format);
		delete stream;
		return;
	}

	delete _handle;
	_handle = new Audio::SoundHandle();
	_vm->_mixer->playStream(type, _handle, audio);
}

} // End of namespace Orlando
