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
#include "orlando/resource.h"
#include "orlando/sound.h"
#include "orlando/text_parser.h"
#include "orlando/graphics.h"

/**
 * Utility function for replacing the ending of a string with another string.
 * @param str Source string.
 * @param replace Replacement string.
 * @param count Amount of characters to replace (npos = same as replacement).
 * @return String with replacement applied.
 */
Common::String replaceEnd(Common::String str, const Common::String &replace, uint32 count = Common::String::npos) {
	if (count == Common::String::npos)
		count = replace.size();
	str.replace(str.size() - count, count, replace);
	return str;
}

namespace Orlando {

Scene::Scene(OrlandoEngine *vm, const Common::String &id) : _vm(vm), _id(id), _pak(nullptr), _pakEx(nullptr) {
}

Scene::~Scene() {
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

	return true;
}

bool Scene::loadCcg() {
	GraphicsManager *graphics = _vm->getGraphicsManager();

	TextParser parser = TextParser(loadFile(_id + ".CCG"));

	Common::String id = parser.readString();
	while (id.firstChar() == '[') {
		id.deleteChar(0);
		id.deleteLastChar();

		if (id == "grafiki") {
			Common::String bg = parser.readString();
			if (bg.lastChar() == '+') {
				bg.deleteLastChar();
				int xOffset = parser.readInt();
			}
			Graphics::Surface *bgSurface = _vm->getGraphicsManager()->loadRawBitmap(loadFile(bg));
			graphics->draw(*bgSurface);

			while (true) {
				id = parser.readString();
				if (id.firstChar() == '[')
					break;
				Common::File *sprite = loadFile(id);
				int bpp = parser.readInt();
				int16 x = parser.readInt();
				int16 y = parser.readInt();

				if (sprite != nullptr) {
					Graphics::Surface *spriteSurface = nullptr;
					if (bpp == 16) {
						spriteSurface = _vm->getGraphicsManager()->loadRawBitmap(sprite);
					}
					else if (bpp == 8 || bpp == -8) {
						spriteSurface = _vm->getGraphicsManager()->loadPaletteBitmap(sprite);
					}
					graphics->drawTransparent(*spriteSurface, Common::Point(x, y));
					Common::Rect border = { x, y, x + spriteSurface->w, y + spriteSurface->h };
					graphics->drawPolygon(border, graphics->RGBToColor(255, 0, 0));
				}

				parser.readFloat();
				parser.readFloat();
				int16 x1 = parser.readInt();
				int16 y1 = parser.readInt();
				int16 x2 = parser.readInt();
				int16 y2 = parser.readInt();
				int16 x3 = parser.readInt();
				int16 y3 = parser.readInt();
				int16 x4 = parser.readInt();
				int16 y4 = parser.readInt();
				if (x1 == x2) x1++;
				if (x3 == x4) x3++;
				if (y1 == y2) y1++;
				if (y3 == y4) y3++;
				Common::Point poly[] = { {x + x1, y + y1}, {x + x2, y + y2}, {x + x3, y + y3}, {x + x4, y + y4} };
				graphics->drawPolygon(poly, 4, graphics->RGBToColor(0, 255, 0));
			}
		} else if (id == "perspektywa") {
			parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readFloat();
			id = parser.readString();
		} else if (id == "obszar_chodu") {
			int num = parser.readInt();
			for (int i = 0; i < num; i++) {
				int16 x1 = parser.readInt();
				int16 y1 = parser.readInt();
				int16 x2 = parser.readInt();
				int16 y2 = parser.readInt();
				int16 x3 = parser.readInt();
				int16 y3 = parser.readInt();
				Common::Point poly[] = { {x1, y1}, {x2, y2}, {x3, y3} };
				graphics->drawPolygon(poly, 3, graphics->RGBToColor(0, 0, 255));
			}
			id = parser.readString();
		} else if (id == "kolor_liter") {
			parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readInt();
			parser.readInt();
			id = parser.readString();
		} else if (id == "swiatlo") {
			parser.readInt();
			parser.readInt();
			id = parser.readString();
		} else if (id == "elementy") {
			id = parser.readString();
		} else {
			break;
		}
	}

	return true;
}

bool Scene::run() {
	return true;
}

} // End of namespace Orlando
