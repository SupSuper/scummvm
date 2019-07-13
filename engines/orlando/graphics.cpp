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
#include "graphics/font.h"
#include "graphics/surface.h"
#include "graphics/managed_surface.h"

#include "orlando/graphics.h"
#include "orlando/orlando.h"
#include "orlando/resource.h"
#include "orlando/mouse.h"

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

	system->copyRectToScreen(_screenBuffer->getPixels(), _screenBuffer->pitch, 0, 0, _screenBuffer->w, _screenBuffer->h);
	/*
	Graphics::Surface *screen = system->lockScreen();
	screen->copyRectToSurface(*_screenBuffer, 0, 0, Common::Rect(0, 0, _screenBuffer->w, _screenBuffer->h));
	system->unlockScreen();
	*/

	system->updateScreen();
}

Graphics::Surface *GraphicsManager::loadRawBitmap(Common::SeekableReadStream *stream) const {
	int width = stream->readUint16LE();
	int height = stream->readUint16LE();

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(width, height, kScreenFormat);
	stream->read(surface->getPixels(), width * height * kScreenFormat.bytesPerPixel);

	delete stream;
	return surface;
}

Graphics::Surface *GraphicsManager::loadPaletteBitmap(Common::SeekableReadStream *stream) const {
	int width = stream->readUint16LE();
	int height = stream->readUint16LE();
	uint16 palette[128];
	stream->read(palette, 128 * sizeof(uint16));

	Graphics::Surface *surface = new Graphics::Surface();
	surface->create(width, height, kScreenFormat);

	uint16 *pixel = (uint16*)surface->getPixels();
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

void GraphicsManager::draw(const Graphics::Surface &surface, const Common::Point &pos) {
	_screenBuffer->blitFrom(surface, pos);
}

void GraphicsManager::drawTransparent(const Graphics::Surface &surface, const Common::Point &pos, const Common::Rect &window, bool flipped, float scale) {
	Common::Rect srcRect(surface.w, surface.h);
	Common::Rect destRect(surface.w * scale, surface.h * scale);
	destRect.translate(pos.x, pos.y);
	if (!window.isEmpty()) {
		destRect.clip(window);
		srcRect = destRect;
		srcRect.translate(-pos.x, -pos.y);
		srcRect.left /= scale;
		srcRect.top /= scale;
		srcRect.right /= scale;
		srcRect.bottom /= scale;
	}
	_screenBuffer->transBlitFrom(surface, srcRect, destRect, 0, flipped);
}

void GraphicsManager::drawText(const Common::String &text, const Common::Point &pos, int width, const TextColor &color, Graphics::TextAlign align) {
	_vm->getResourceManager()->getFont()->drawString(_screenBuffer, text, pos.x, pos.y, width, color.toUint(), align);
}

void GraphicsManager::drawShadowRect(const Common::Rect &rect, float shadow, int bevel) {
	// TODO: Figure out the original game blending formula
	Common::Rect fill(rect);
	fill.grow(-bevel);
	for (int y = rect.top; y < rect.bottom; ++y) {
		for (int x = rect.left; x < rect.right; ++x) {
			float alpha = shadow;
			if (!fill.contains(x, y)) {
				if ((x <= rect.left + bevel && y < rect.bottom - (x - rect.left)) ||
					(y <= rect.top + bevel && x < rect.right - (y - rect.top))) {
					// Top-left bevel (brighter)
					alpha += shadow / 2;
				} else {
					// Bottom-right bevel (darker)
					alpha -= shadow / 2;
				}
			}
			uint16 *pixel = (uint16*)_screenBuffer->getBasePtr(x, y);
			*pixel = colorAlpha(*pixel, alpha);
		}
	}
}

bool GraphicsManager::drawButton(const Common::String &text, const Common::Rect &rect, const TextColor &color) {
	Mouse *mouse = _vm->getMouse();
	bool isPressed = (mouse->getLeftButton() == kButtonPressed && mouse->isOver(rect));

	// Draw background
	int bevel = 5;
	if (isPressed) {
		bevel = 2;
	}
	drawShadowRect(rect, 1.5f, bevel);

	// Draw label
	int y = (rect.height() - _vm->getResourceManager()->getFont()->getFontHeight()) / 2 + 2;
	Common::Point pos = Common::Point(rect.left, rect.top + y);
	if (isPressed) {
		pos += Common::Point(1, 1);
	}
	drawText(text, pos, rect.width(), color, Graphics::kTextAlignCenter);

	return (mouse->getLeftButton() == kButtonReleased && mouse->isOver(rect));
}

void GraphicsManager::drawPolygon(const Common::Rect &rect, uint16 color) {
	_screenBuffer->frameRect(rect, color);
}

void GraphicsManager::drawPolygon(const Common::Point points[], int numPoints, uint16 color) {
	for (int i = 0; i < numPoints; i++) {
		int j = (i + 1) % numPoints;
		_screenBuffer->drawLine(points[i].x, points[i].y, points[j].x, points[j].y, color);
	}
}

} // End of namespace Orlando
