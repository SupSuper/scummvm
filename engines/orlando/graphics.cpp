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

#include "common/rect.h"
#include "common/stream.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/surface.h"
#include "graphics/managed_surface.h"

#include "orlando/graphics.h"
#include "orlando/orlando.h"

namespace Orlando {

const Graphics::PixelFormat GraphicsManager::kScreenFormat(2, 5, 6, 5, 0, 11, 5, 0, 0); // RGB565

GraphicsManager::GraphicsManager(OrlandoEngine *vm) : _vm(vm), _screenBuffer(nullptr) {
}

GraphicsManager::~GraphicsManager() {
	delete _screenBuffer;
}

bool GraphicsManager::setupScreen() {
	initGraphics(kScreenWidth, kScreenHeight, &kScreenFormat);
	if (_vm->_system->getScreenFormat() != kScreenFormat) {
		return false;
	}

	_screenBuffer = new Graphics::ManagedSurface(kScreenWidth, kScreenHeight, kScreenFormat);
	return true;
}

void GraphicsManager::updateScreen() {
	OSystem *system = _vm->_system;

	Graphics::Surface *screen = system->lockScreen();
	screen->copyRectToSurface(*_screenBuffer, 0, 0, Common::Rect(0, 0, _screenBuffer->w, _screenBuffer->h));
	system->unlockScreen();

	system->updateScreen();
}

void GraphicsManager::drawSurface(const Graphics::Surface &surface, const Common::Point &pos) {
	_screenBuffer->blitFrom(surface, pos);
}

void GraphicsManager::drawSprite(const Graphics::Surface &surface, const Common::Point &pos) {
	_screenBuffer->transBlitFrom(surface, pos);
}

Graphics::Surface *GraphicsManager::loadRawBitmap(Common::SeekableReadStream *stream) {
	int width = stream->readSint16LE();
	int height = stream->readSint16LE();

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(width, height, kScreenFormat);
	stream->read(surface->getPixels(), width * height * kScreenFormat.bytesPerPixel);

	delete stream;
	return surface;
}

Graphics::Surface *GraphicsManager::loadPaletteBitmap(Common::SeekableReadStream *stream) {
	int width = stream->readSint16LE();
	int height = stream->readSint16LE();
	uint16 palette[128];
	stream->read(&palette, 128 * sizeof(uint16));

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(width, height, kScreenFormat);

	uint16 *pixel = (uint16*)surface->getBasePtr(0, 0);
	do {
		int color = stream->readByte();
		if (stream->eos())
			break;

		int repeat = 1;
		// RLE byte
		if (color & 1) {
			repeat = stream->readByte();
		}

		color = color >> 1;
		for (int i = 0; i < repeat; i++) {
			*pixel = palette[color];
			pixel++;
		}
	} while (!stream->eos());

	delete stream;
	return surface;
}

} // End of namespace Orlando
