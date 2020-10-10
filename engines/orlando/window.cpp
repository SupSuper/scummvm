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

#include "graphics/managed_surface.h"

#include "orlando/window.h"
#include "orlando/graphics.h"

namespace Orlando {

Window::Window(const Common::Rect &bounds, uint32 order) :
	_surface(new Graphics::ManagedSurface(bounds.width(), bounds.height(), GraphicsManager::kScreenFormat)),
	_pos(bounds.left, bounds.top), _visible(true), _order(order) {
}

Window::~Window() {
	delete _surface;
}

void Window::resize(const Common::Rect &bounds) {
	Common::Point oldPos = _pos;
	Graphics::ManagedSurface *oldSurface = _surface;

	_surface = new Graphics::ManagedSurface(bounds.width(), bounds.height(), GraphicsManager::kScreenFormat);
	_pos = Common::Point(bounds.left, bounds.top);

	// We need to copy the old contents to the new surface
	Common::Rect srcRect(oldSurface->w, oldSurface->h);
	_surface->blitFrom(*oldSurface, srcRect, oldPos - _pos);
	delete oldSurface;
}

void Window::drawFrom(const Graphics::Surface *surface, const Common::Point &pos, bool flipped, float scale) const {
	_surface->clear();
	if (surface != nullptr) {
		Common::Rect srcRect(surface->w, surface->h);
		Common::Rect destRect(surface->w * scale, surface->h * scale);
		destRect.moveTo(pos - _pos);
		_surface->transBlitFrom(*surface, srcRect, destRect, 0, flipped);
	}
}

void Window::drawTo(GraphicsManager *graphics) const {
	if (_visible)
		graphics->drawTransparent(*_surface, _pos);
}

bool Window::isOver(const Window *window) const {
	return _order > window->getOrder();
}

bool Window::isUnder(const Window *window) const {
	return _order < window->getOrder();
}

} // End of namespace Orlando
