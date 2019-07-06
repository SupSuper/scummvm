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
#include "audio/decoders/adpcm_intern.h"
#include "audio/decoders/raw.h"
#include "common/file.h"
#include "common/substream.h"

#include "orlando/sound.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/util.h"

// TODO: Integrate into ScummVM
#include "orlando/libgsm.h"

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
	int _rate;

	libgsm::gsm _music;
	libgsm::gsm_frame _frame;
	libgsm::gsm_signal _decodedSamples[kSamplesPerFrame];
	uint8 _decodedSampleCount;

public:
	GSMStream(Common::SeekableReadStream *stream, DisposeAfterUse::Flag disposeAfterUse, GSMType type, int rate = 8000) :
		_stream(stream, disposeAfterUse), _rate(rate), _decodedSampleCount(0) {
		_music = libgsm::gsm_create();
		if (type == kGSM610MS) {
			int flag = 1;
			libgsm::gsm_option(_music, libgsm::GSM_OPT_WAV49, &flag);
		}
	}

	~GSMStream() {
		libgsm::gsm_destroy(_music);
	}

	virtual int readBuffer(int16 *buffer, const int numSamples) override {
		int samples;

		for (samples = 0; samples < numSamples && !endOfData(); samples++) {
			if (_decodedSampleCount == 0) {
				int frameIndex = gsm_option(_music, libgsm::GSM_OPT_FRAME_INDEX, nullptr);
				int frameSize = sizeof(libgsm::gsm_frame) - frameIndex;
				_stream->read(&_frame, frameSize);
				gsm_decode(_music, _frame, _decodedSamples);
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
	virtual bool endOfData() const override { return _stream->eos(); }
	// TODO: ?
	virtual bool seek(const Audio::Timestamp &where) override { return false; }
	virtual Audio::Timestamp getLength() const override { return Audio::Timestamp(); }
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
		return ADPCMStream::endOfData() && (_decodedSampleCount == 0);
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

SoundManager::SoundManager(OrlandoEngine *vm) : _vm(vm) {
}

Common::SeekableReadStream *SoundManager::makeHeaderless(Common::SeekableReadStream *stream, int headerSize) const {
	return new Common::SeekableSubReadStream(stream, headerSize, stream->size() - headerSize, DisposeAfterUse::YES);
}

Audio::SeekableAudioStream *SoundManager::loadRawAudio(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type) const {
	/* uint32 size = stream->readUint32LE(); */

	byte flags = Audio::FLAG_UNSIGNED | Audio::FLAG_LITTLE_ENDIAN;
	int rate = 16000;
	if (type == Audio::Mixer::kMusicSoundType) {
		flags |= Audio::FLAG_STEREO;
	} else if (_vm->isVersionHP()) {
		rate = 22050;
		flags |= Audio::FLAG_16BITS;
	}
	
	Audio::SeekableAudioStream *audio = Audio::makeRawStream(makeHeaderless(stream, 4), rate, flags);
	return audio;
}

Audio::SeekableAudioStream *SoundManager::loadHeaderAudio(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type) const {	
	enum AudioFormat {
		kAudioAdpcmMono = 50,
		kAudioAdpcmStereo = 51,
		kAudioGSM16 = 52,
		kAudioGSM22 = 53
	};
	AudioFormat format;

	format = (AudioFormat)stream->readUint32LE();
	uint32 size = stream->readUint32LE();

	// Guess format parameters
	int rate = 0;
	int channels = 1;
	switch (format) {
	case kAudioAdpcmStereo:
		channels = 2;
		// fall through
	case kAudioAdpcmMono:
		if (_vm->isVersionDC()) {
			rate = 44100;
		} else {
			rate = 22050;
		}
		break;
	case kAudioGSM16:
		rate = 16000;
		break;
	case kAudioGSM22:
		rate = 22050;
		break;
	}

	// Create appropriate stream
	Audio::SeekableAudioStream *audio = nullptr;
	switch (format) {
	case kAudioAdpcmMono:
	case kAudioAdpcmStereo:
		audio = new Orlando_ADPCMStream(makeHeaderless(stream, 24), DisposeAfterUse::YES, size, rate, channels);
		break;
	case kAudioGSM16:
	case kAudioGSM22:
		audio = new GSMStream(makeHeaderless(stream, 36), DisposeAfterUse::YES, kGSM610MS, rate);
		break;
	default:
		audio = loadRawAudio(stream, type);
		break;
	}
	return audio;
}

void SoundManager::playFile(Common::SeekableReadStream *stream, Audio::Mixer::SoundType type, bool loop) {
	Audio::SeekableAudioStream *audio = loadHeaderAudio(stream, type);
	if (audio == nullptr)
		return;

	// Only one music at a time
	if (type == Audio::Mixer::kMusicSoundType) {
		_vm->_mixer->stopHandle(_music);
	}

	Audio::SoundHandle handle;
	if (loop) {
		_vm->_mixer->playStream(type, &handle, Audio::makeLoopingAudioStream(audio, 0));
	} else {
		_vm->_mixer->playStream(type, &handle, audio);
	}

	if (type == Audio::Mixer::kMusicSoundType) {
		_music = handle;
	}
}

void SoundManager::playMusic(const Common::String &filename) {
	ResourceManager *resources = _vm->getResourceManager();
	const int kExt = 5;

	Common::File *audio = nullptr;
	if (_vm->isVersionSP()) {
		// Format is xxx-8.MUS
		Common::String pak = replaceEnd(filename, "8.PAK", kExt);
		audio = resources->loadPakFile(pak, filename);
	}
	else if (_vm->isVersionHP()) {
		// Format is xxx-22.PMS
		Common::String pak = replaceEnd(filename, "22.PAK", kExt);
		Common::String file = replaceEnd(filename, "22.PMS", kExt);
		audio = resources->loadPakFile(pak, file);
	}
	else if (_vm->isVersionDC()) {
		// Format is xxx-22.PMS
		Common::String file = replaceEnd(filename, "22.PMS", kExt);
		audio = resources->loadRawFile(file);
	}

	_vm->getSoundManager()->playFile(audio, Audio::Mixer::kMusicSoundType, true);
}

Common::String SoundManager::getSfx(const Common::String &filename) {
	if (_vm->isVersionHP()) {
		// Format is xxx.S16
		return replaceEnd(filename, "S16");
	} else if (_vm->isVersionDC()) {
		// Format is xxx.PSD
		return replaceEnd(filename, "PSD");
	} else {
		// Format is xxx.SND
		return filename;
	}
}

Common::String SoundManager::getSpeech(const Common::String &filename) {
	if (_vm->isVersionSP()) {
		// Format is xxx.SND
		return filename;
	} else {
		// Format is xxx.S22
		return replaceEnd(filename, "S22");
	}
}

} // End of namespace Orlando
