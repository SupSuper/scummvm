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
#include "common/system.h"
#include "audio/decoders/raw.h"
#include "audio/audiostream.h"
#include "audio/mixer.h"

#include "raunes/sound.h"
#include "raunes/raunes.h"

namespace Raunes {

SoundManager::SoundManager(RaunesEngine *vm) : _vm(vm) {
}

SoundManager::~SoundManager() {
}

void SoundManager::play(const Common::String& filename, int rate) {
	Common::File *file = new Common::File();
	if (!file->open(filename)) {
		delete file;
		warning("SoundManager: %s not found", filename.c_str());
		return;
	}
	_vm->_mixer->stopAll();
	Audio::SeekableAudioStream *audio = Audio::makeRawStream(file, rate, Audio::FLAG_UNSIGNED | Audio::FLAG_LITTLE_ENDIAN);
	_vm->_mixer->playStream(Audio::Mixer::kMusicSoundType, nullptr, Audio::makeLoopingAudioStream(audio, 0));
}

} // End of namespace Raunes
