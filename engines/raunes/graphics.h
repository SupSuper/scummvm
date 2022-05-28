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

#ifndef RAUNES_GRAPHICS_H
#define RAUNES_GRAPHICS_H

#include "graphics/surface.h"
#include "image/pcx.h"
#include "raunes/datfile.h"
#include "raunes/font.h"

namespace Raunes {

class RaunesEngine;

class GraphicsManager {
	static const int kScreenWidth = 320;
	static const int kScreenHeight = 200;
	static const int kScreenPages = 4;

	RaunesEngine *_vm;
	DatArchive _data;
	SnagFont _font;
	Image::PCXDecoder _pcx;

	int _page;
	Graphics::Surface *_drawPage;
	Graphics::Surface _pages[kScreenPages];

	Graphics::Surface *_cursor;

public:
	GraphicsManager(RaunesEngine *vm);
	~GraphicsManager();
	bool load();
	void init();

	void setPage(int page);
	void showPage(int page);
	void swapPage();
	void clearScreen();

	bool loadPcx(const Common::String &filename);
	Graphics::Surface *loadGrf(const Common::String &filename);
	void showPcx(const Common::String &filename);
};

} // End of namespace Raunes

#endif
