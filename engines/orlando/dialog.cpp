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

#include "orlando/dialog.h"
#include "orlando/text_parser.h"
#include "orlando/util.h"
#include "orlando/orlando.h"
#include "orlando/sound.h"
#include "orlando/scene.h"

namespace Orlando {

Dialog::Dialog(int id) : _id(id), _answer(0) {
}

void Dialog::load(TextParser &parser, bool multiple) {
	int num = 1;
	if (multiple) {
		num = parser.readInt();
	}
	for (int i = 0; i < num; i++) {
		DialogOption option;
		option.text = parser.readString(false);
		replaceChar(option.text, '_', ' ');
		option.width = parser.readInt();
		option.sound = parser.readString();
		option.enabled = true;
		_options.push_back(option);
	}
}

void Dialog::talk(OrlandoEngine *vm) {
	const DialogOption &dialog = _options.front();
	Common::File *file = vm->getScene()->loadFile(vm->getSoundManager()->getSpeech(dialog.sound));
	_handle = vm->getSoundManager()->playFile(file, Audio::Mixer::kSpeechSoundType);
}

void Dialog::skip(OrlandoEngine *vm) {
	vm->_mixer->stopHandle(_handle);
}

bool Dialog::isTalking(OrlandoEngine *vm) const {
	return vm->_mixer->isSoundHandleActive(_handle);
}

} // End of namespace Orlando
