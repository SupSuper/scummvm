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
#include "common/rect.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/cursorman.h"
#include "graphics/palette.h"
#include "graphics/surface.h"
#include "image/pcx.h"

#include "raunes/graphics.h"
#include "raunes/datfile.h"
#include "raunes/raunes.h"

namespace Raunes {

GraphicsManager::GraphicsManager(RaunesEngine *vm) : _vm(vm), _page(0), _drawPage(nullptr), _pages{} {
}

GraphicsManager::~GraphicsManager() {
	for (int i = 0; i < kPages; i++) {
		_pages[i].free();
	}
}

bool GraphicsManager::load() {
	// Load DAT archive
	Common::File *datFile = new Common::File();
	if (datFile->open("RAUNES.DAT")) {
		if (!_data.open(datFile)) {
			return false;
		}
	} else {
		warning("GraphicsManager: RAUNES.DAT not found");
		delete datFile;
		return false;
	}

	// Load font
	Common::File fontFile;
	if (fontFile.open("SELF.SF")) {
		if (!_font.open(&fontFile)) {
			return false;
		}
	} else {
		warning("GraphicsManager: SELF.SF not found");
		return false;
	}

	// Load graphics
	_cursor = loadGrf("ITEM0A.GRF");

	return true;
}

void GraphicsManager::init() {
	// Initialize screen
	initGraphics(kScreenW, kScreenH);
	for (int i = 0; i < kPages; i++) {
		_pages[i].create(kScreenW, kScreenH, Graphics::PixelFormat::createFormatCLUT8());
	}
	_page = 0;
	setPage(_page);
	showPage(1 - _page);

	// Initialize cursor
	CursorMan.replaceCursor(_cursor->getPixels(), _cursor->w, _cursor->h, 0, 0, kTransparent, false);
}

void GraphicsManager::setPage(int page) {
	if (page < 0)
		page += kPages;
	_drawPage = &_pages[page];
}

void GraphicsManager::showPage(int page) {
	if (page < 0)
		page += kPages;
	Graphics::Surface *surface = &_pages[page];
	g_system->copyRectToScreen(surface->getPixels(), surface->pitch, 0, 0, surface->w, surface->h);
}

void GraphicsManager::swapPage() {
	_page = 1 - _page;
	setPage(_page);
	showPage(1 - _page);
}

void GraphicsManager::clearScreen() {
	drawBlock(0, 0, kScreenW, kScreenH, 0);
	swapPage();
	drawBlock(0, 0, kScreenW, kScreenH, 0);
	swapPage();
}

void GraphicsManager::blockMove(int page1, int x1, int y1, int page2, int x2, int y2, int width, int height) {
	Graphics::Surface *surface1 = &_pages[page1];
	Graphics::Surface *surface2 = &_pages[page2];
	surface2->copyRectToSurface(*surface1, x2, y2, Common::Rect(x1, y1, x1 + width, y1 + height));
}

void GraphicsManager::drawBlock(int x, int y, int width, int height, int color) {
	_drawPage->fillRect(Common::Rect(x, y, x + width, y + height), color);
}

bool GraphicsManager::loadPcx(const Common::String &filename) {
	const DatFile *file = _data.findFile(filename);
	Common::SeekableReadStream *stream = _data.readFile(file);
	if (stream == nullptr) {
		warning("GraphicsManager: %s not found", filename.c_str());
		return false;
	}
	if (!_pcx.loadStream(*stream)) {
		warning("GraphicsManager: %s is corrupted", filename.c_str());
		delete stream;
		return false;
	}
	delete stream;
	return true;
}

Graphics::Surface *GraphicsManager::loadGrf(const Common::String &filename) {
	const DatFile *file = _data.findFile(filename);
	Common::SeekableReadStream *stream = _data.readFile(file);
	if (stream == nullptr) {
		warning("GraphicsManager: %s not found", filename.c_str());
		return nullptr;
	}

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(file->width, file->height, Graphics::PixelFormat::createFormatCLUT8());
	stream->read(surface->getPixels(), surface->w * surface->h);
	delete stream;
	return surface;
}

void GraphicsManager::showPcx(const Common::String &filename) {
	loadPcx(filename);
	g_system->getPaletteManager()->setPalette(_pcx.getPalette(), _pcx.getPaletteStartIndex(), _pcx.getPaletteColorCount());

	const Graphics::Surface *surface = _pcx.getSurface();
	_drawPage->copyRectToSurface(surface->getPixels(), surface->pitch, 0, 0, surface->w, surface->h);
}

void GraphicsManager::write(int x, int y, const Common::String &str) {
	_font.write(_drawPage, x, y, str);
}

void GraphicsManager::writeCenter(int x, int y, const Common::String &str) {
	_font.writeCenter(_drawPage, x, y, str);
}

} // End of namespace Raunes
