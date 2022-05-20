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
#include "graphics/palette.h"
#include "graphics/surface.h"
#include "image/pcx.h"

#include "raunes/graphics.h"
#include "raunes/datfile.h"

namespace Raunes {

GraphicsManager::GraphicsManager() {
	_data = new DatArchive();
}

GraphicsManager::~GraphicsManager() {
	delete _data;
}

bool GraphicsManager::loadDat() {
	Common::File *file = new Common::File();
	if (file->open("RAUNES.DAT")) {
		return _data->open(file);
	} else {
		warning("GraphicsManager: RAUNES.DAT not found");
		delete file;
		return false;
	}
}

Graphics::Surface *GraphicsManager::loadPcx(const Common::String &filename) {
	Common::SeekableReadStream *file = _data->createReadStreamForMember(filename);
	if (file == nullptr) {
		warning("GraphicsManager: %s not found", filename.c_str());
		return nullptr;
	} else {
		Image::PCXDecoder pcx = Image::PCXDecoder();
		if (!pcx.loadStream(*file)) {
			warning("GraphicsManager: %s is corrupted", filename.c_str());
			return nullptr;
		}
		g_system->getPaletteManager()->setPalette(pcx.getPalette(), pcx.getPaletteStartIndex(), pcx.getPaletteColorCount());
		Graphics::Surface *surf = new Graphics::Surface();
		surf->copyFrom(*pcx.getSurface());
		return surf;
	}
}

} // End of namespace Raunes
