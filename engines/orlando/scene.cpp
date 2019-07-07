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
#include "graphics/surface.h"

#include "orlando/scene.h"
#include "orlando/orlando.h"
#include "orlando/util.h"
#include "orlando/resource.h"
#include "orlando/text_parser.h"
#include "orlando/graphics.h"
#include "orlando/element.h"
#include "orlando/dialog.h"
#include "orlando/face.h"
#include "orlando/person.h"
#include "orlando/area.h"
#include "orlando/animation.h"
#include "orlando/insertion.h"
#include "orlando/film.h"
#include "orlando/macro.h"
#include "orlando/jack.h"

namespace Orlando {

/** Deletes all pointers in a container. */
template <typename K, typename V>
inline void deleteAll(Common::HashMap<K, V*> &container) {
	for (typename Common::HashMap<K, V*>::const_iterator i = container.begin(); i != container.end(); ++i) {
		delete i->_value;
	}
}

Scene::Scene(OrlandoEngine *vm, const Common::String &id) : _vm(vm), _id(id), _pak(nullptr), _pakEx(nullptr), _background(nullptr), _scrollX(0) {
}

Scene::~Scene() {
	deleteAll(_insertions);
	deleteAll(_areas);
	deleteAll(_persons);
	deleteAll(_faces);
	deleteAll(_dialogs);
	deleteAll(_elements);

	if (_background != nullptr) {
		_background->free();
	}
	delete _background;

	delete _pakEx;
	delete _pak;
}

GraphicsManager *Scene::getGraphicsManager() const {
	return _vm->getGraphicsManager();
}

Person *Scene::getPerson(const Common::String &id) {
	if (id == "JACK")
		return _vm->getJack();
	return _persons[id];
}

Common::File *Scene::loadFile(const Common::String &filename, bool optional) {
	Common::File *file = nullptr;
	if (_pak->hasFile(filename)) {
		file = _vm->getResourceManager()->loadPakFile(*_pak, filename, optional);
	} else if (_pakEx != nullptr && _pakEx->hasFile(filename)) {
		file = _vm->getResourceManager()->loadPakFile(*_pakEx, filename, optional);
	} else if (!optional) {
		warning("ResourceManager: File not found in PAK: %s", filename.c_str());
	}
	return file;
}

Graphics::Surface *Scene::loadSurface(const Common::String &filename, int bpp) {
	Common::File *file = loadFile(filename);
	if (file == nullptr || file->size() <= 1)
		return nullptr;

	if (bpp == 16) {
		return _vm->getGraphicsManager()->loadRawBitmap(file);
	} else if (bpp == 8) {
		return _vm->getGraphicsManager()->loadPaletteBitmap(file);
	} else {
		warning("GraphicsManager: Unknown bpp '%d'", bpp);
		return nullptr;
	}
}

bool Scene::initialize() {
	Common::String actionExt = ".PHK";
	Common::String dataExt = ".PH2";
	if (_vm->isVersionSP()) {
		actionExt = ".PAK";
		dataExt = ".PA2";
	}
	_pak = _vm->getResourceManager()->loadPakArchive(_id + actionExt);
	if (Common::File::exists(_id + dataExt))
		_pakEx = _vm->getResourceManager()->loadPakArchive(_id + dataExt);

	if (_pak == nullptr)
		return false;

	if (!loadCcg() ||
		!loadDcn() ||
		!loadFcc() ||
		!loadPcs() ||
		!loadAce() ||
		!loadAci() ||
		!loadIcs() ||
		!loadFcm() ||
		!loadMcc())
		return false;

	Macro *preMacro = _macros["PRE"];
	preMacro->start();
	while (preMacro->isEnabled())
		preMacro->execute(_vm->getScriptInterpreter());
	_macros["INIT"]->start();

	return true;
}

bool Scene::run() {
	GraphicsManager *graphics = _vm->getGraphicsManager();
	uint32 time = _vm->getTotalPlayTime();

	// Run commands
	for (Common::HashMap<Common::String, Macro*>::const_iterator i = _macros.begin(); i != _macros.end(); ++i) {
		i->_value->execute(_vm->getScriptInterpreter());
	}

	// Draw scene elements
	graphics->draw(*_background);
	for (Common::HashMap<Common::String, Element*>::const_iterator i = _elements.begin(); i != _elements.end(); ++i) {
		i->_value->draw(graphics, time);
	}
	for (Common::HashMap<Common::String, Person*>::const_iterator i = _persons.begin(); i != _persons.end(); ++i) {
		i->_value->draw(graphics, time);
	}
	_vm->getJack()->draw(graphics, time);
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

		if (section == "GRAFIKI") {
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

				Element *element = new Element(id);
				if (!element->load(parser, this)) {
					delete element;
					return false;
				}
				_elements[id] = element;
			}
		} else if (section == "PERSPEKTYWA") {
			// perspective
		} else if (section == "OBSZAR_CHODU") {
			// walk areas
			int n = parser.readInt();
			for (int j = 0; j < n; j++) {
				Triangle area;
				for (int i = 0; i < area.kPoints; i++) {
					area.p[i].x = parser.readInt();
					area.p[i].y = parser.readInt();
				}
				_walkRegions.push_back(area);
			}
		} else if (section == "KOLOR_LITER") {
			// text color
		} else if (section == "SWIATLO") {
			// light
		} else if (section == "ELEMENTY") {
			// items
			while (!parser.eof()) {
				Common::String id = parser.readString();
				if (id.empty() || id.firstChar() == '[') {
					parser.rewind();
					break;
				}

				Element *element = new Element(id);
				if (!element->load(parser, this)) {
					delete element;
					return false;
				}
				_elements[id] = element;
			}
		} else {
			break;
		}
	}

	return true;
}

bool Scene::loadDcn() {
	Common::File *dcn = loadFile(_id + ".DCN", true);
	if (dcn == nullptr)
		return true;
	TextParser parser = TextParser(dcn);

	while (!parser.eof()) {
		Common::String type = parser.readString();
		if (type.empty())
			break;
		bool multiple;
		if (type == "Q" || type == "R") {
			multiple = true;
		} else {
			multiple = false;
			parser.rewind();
		}

		int id = parser.readInt();

		Dialog *dialog = new Dialog(id);
		dialog->load(parser, multiple);
		_dialogs[id] = dialog;
	}

	return true;
}

bool Scene::loadFcc() {
	Common::File *fcc = loadFile(_id + ".FCC", true);
	if (fcc == nullptr)
		return true;
	TextParser parser = TextParser(fcc);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Face *face = new Face(id);
		if (!face->load(parser, this)) {
			delete face;
			return false;
		}
		_faces[id] = face;
	}

	return true;
}

bool Scene::loadPcs() {
	Common::File *pcs = loadFile(_id + ".PCS", true);
	if (pcs == nullptr)
		return true;
	TextParser parser = TextParser(pcs);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Person *person = new Person(id);
		if (!person->load(parser, this)) {
			delete person;
			return false;
		}
		_persons[id] = person;
	}

	return true;
}

bool Scene::loadAce() {
	Common::File *ace = loadFile(_id + ".ACE", true);
	if (ace == nullptr)
		return true;
	TextParser parser = TextParser(ace);

	int n = parser.readInt();
	for (int i = 0; i < n; i++) {
		Common::String id = parser.readString();
		deleteFirstLast(id);

		Area *area = new Area(id);
		area->load(parser);
		_areas[id] = area;
	}

	return true;
}

bool Scene::loadAci() {
	Common::File *aci = loadFile(_id + ".ACI", true);
	if (aci == nullptr)
		return true;
	TextParser parser = TextParser(aci);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Animation *anim = new Animation(id);
		if (!anim->load(parser, this)) {
			delete anim;
			return false;
		}

	}

	return true;
}

bool Scene::loadIcs() {
	Common::File *ics = loadFile(_id + ".ICS", true);
	if (ics == nullptr)
		return true;
	TextParser parser = TextParser(ics);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Insertion *insertion = new Insertion(id);
		if (!insertion->load(parser, this)) {
			delete insertion;
			return false;
		}
		_insertions[id] = insertion;
	}

	return true;
}

bool Scene::loadFcm() {
	Common::File *fcm = loadFile(_id + ".FCM", true);
	if (fcm == nullptr)
		return true;
	TextParser parser = TextParser(fcm);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Film *film = new Film(id);
		if (!film->load(parser, this)) {
			delete film;
			return false;
		}
		_films[id] = film;
	}

	return true;
}

bool Scene::loadMcc() {
	Common::File *mcc = loadFile(_id + ".MCC", false);
	if (mcc == nullptr)
		return false;
	TextParser parser = TextParser(mcc);

	while (!parser.eof()) {
		Common::String id = parser.readString();
		if (id.empty())
			break;
		deleteFirstLast(id);
		Macro *macro = new Macro(id);
		macro->load(parser);
		_macros[id] = macro;
	}

	return true;
}

} // End of namespace Orlando
