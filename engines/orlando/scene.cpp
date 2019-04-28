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
#include "graphics/surface.h"

#include "orlando/scene.h"
#include "orlando/orlando.h"
#include "orlando/util.h"
#include "orlando/resource.h"
#include "orlando/sound.h"
#include "orlando/text_parser.h"
#include "orlando/graphics.h"
#include "orlando/sprite.h"
#include "orlando/animation.h"
#include "orlando/hotspot.h"

namespace Orlando {

Scene::Scene(OrlandoEngine *vm, const Common::String &id) : _vm(vm), _id(id), _pak(nullptr), _pakEx(nullptr), _background(nullptr), _scrollX(0) {
}

Scene::~Scene() {
	deleteArray(_hotspots);
	deleteArray(_anims);
	deleteArray(_items);
	deleteArray(_objects);

	if (_background != nullptr) {
		_background->free();
	}
	delete _background;

	delete _pakEx;
	delete _pak;
}

Common::File *Scene::loadFile(const Common::String &filename) {
	Common::File *file = _vm->getResourceManager()->loadPakFile(*_pak, filename);
	if (file == nullptr && _pakEx != nullptr) {
		file = _vm->getResourceManager()->loadPakFile(*_pakEx, filename);
	}
	return file;
}

void Scene::playMusic(const Common::String &filename) {
	ResourceManager *resources = _vm->getResourceManager();
	const int kExt = 5;

	Common::File *audio = nullptr;
	if (_vm->isVersionSP()) {
		// Format is xxx-8.MUS
		Common::String pak = replaceEnd(filename, "8.PAK", kExt);
		audio = resources->loadPakFile(pak, filename);
	} else if (_vm->isVersionHP()) {
		// Format is xxx-22.PMS
		Common::String pak = replaceEnd(filename, "22.PAK", kExt);
		Common::String file = replaceEnd(filename, "22.PMS", kExt);
		audio = resources->loadPakFile(pak, file);
	} else if (_vm->isVersionDC()) {
		// Format is xxx-22.PMS
		Common::String file = replaceEnd(filename, "22.PMS", kExt);
		audio = resources->loadRawFile(file);
	}

	if (audio != nullptr) {
		_vm->getSoundManager()->playFile(audio, Audio::Mixer::kMusicSoundType);
	}
}

void Scene::playSfx(const Common::String &filename) {
	Common::File *audio = nullptr;
	if (_vm->isVersionSP()) {
		// Format is xxx.SND
		audio = loadFile(filename);
	} else if (_vm->isVersionHP()) {
		// Format is xxx.S16
		audio = loadFile(replaceEnd(filename, "S16"));
	} else if (_vm->isVersionDC()) {
		// Format is xxx.PSD
		audio = loadFile(replaceEnd(filename, "PSD"));
	}

	if (audio != nullptr) {
		_vm->getSoundManager()->playFile(audio, Audio::Mixer::kSFXSoundType);
	}
}

void Scene::playSpeech(const Common::String &filename) {
	Common::File *audio = nullptr;
	if (_vm->isVersionSP()) {
		// Format is xxx.SND
		audio = loadFile(filename);
	} else {
		// Format is xxx.S22
		audio = loadFile(replaceEnd(filename, "S22"));
	}

	if (audio != nullptr) {
		_vm->getSoundManager()->playFile(audio, Audio::Mixer::kSpeechSoundType);
	}
}

bool Scene::initialize() {
	if (_vm->isVersionSP()) {
		_pak = _vm->getResourceManager()->loadPakArchive(_id + ".PAK");
		_pakEx = _vm->getResourceManager()->loadPakArchive(_id + ".PA2");
	} else {
		_pak = _vm->getResourceManager()->loadPakArchive(_id + ".PHK");
		_pakEx = _vm->getResourceManager()->loadPakArchive(_id + ".PH2");
	}

	if (_pak == nullptr)
		return false;

	if (!loadCcg())
		return false;
	if (!loadAci())
		return false;
	if (!loadAce())
		return false;

	return true;
}

bool Scene::loadCcg() {
	Common::File *ccg = loadFile(_id + ".CCG");
	if (ccg == nullptr)
		return false;
	TextParser parser = TextParser(ccg);

	while (!parser.eof()) {
		Common::String section = parser.readString();
		if (section.firstChar() != '[')
			continue;
		deleteFirstLast(section);

		if (section == "grafiki") {
			// graphics
			Common::String bg = parser.readString();

			if (bg.lastChar() == '+') {
				bg.deleteLastChar();
				_scrollX = parser.readInt();
			}

			Common::File *bgFile = loadFile(bg);
			if (bgFile == nullptr)
				return false;
			_background = _vm->getGraphicsManager()->loadRawBitmap(bgFile);

			while (!parser.eof()) {
				Common::String id = parser.readString();
				if (id.empty() || id.firstChar() == '[') {
					parser.rewind();
					break;
				}
				Sprite *sprite = new Sprite(id);
				sprite->load(parser);

				if (!id.hasPrefix("FLX")) {
					Common::File *file = loadFile(id);
					if (file == nullptr)
						return false;
					sprite->loadSurface(file, _vm->getGraphicsManager());
				}

				_objects.push_back(sprite);
			}
		} else if (section == "perspektywa") {
			// perspective
		} else if (section == "obszar_chodu") {
			// walk areas
			int num = parser.readInt();
			for (int j = 0; j < num; j++) {
				Triangle area;
				for (int i = 0; i < area.kPoints; i++) {
					area.p[i].x = parser.readInt();
					area.p[i].y = parser.readInt();
				}
				_walkAreas.push_back(area);
			}
		} else if (section == "kolor_liter") {
			// text color
		} else if (section == "swiatlo") {
			// light
		} else if (section == "elementy") {
			// items
			while (!parser.eof()) {
				Common::String id = parser.readString();
				if (id.empty() || id.firstChar() == '[') {
					parser.rewind();
					break;
				}
				Sprite *sprite = new Sprite(id);
				sprite->load(parser);

				Common::File *file = loadFile(id);
				if (file == nullptr)
					return false;
				sprite->loadSurface(file, _vm->getGraphicsManager());

				_items.push_back(sprite);
			}
		} else {
			break;
		}
	}

	return true;
}

bool Scene::loadAci() {
	Common::File *aci = loadFile(_id + ".ACI");
	if (aci == nullptr)
		return true;
	TextParser parser = TextParser(aci);

	return true;
}

bool Scene::loadAce() {
	Common::File *ace = loadFile(_id + ".ACE");
	if (ace == nullptr)
		return true;
	TextParser parser = TextParser(ace);

	int hotspots = parser.readInt();
	for (int i = 0; i < hotspots; i++) {
		Common::String id = parser.readString();
		deleteFirstLast(id);

		Hotspot *hotspot = new Hotspot(id);
		hotspot->load(parser);
		_hotspots.push_back(hotspot);
	}

	return true;
}

bool Scene::run() {
	return true;
}

} // End of namespace Orlando
