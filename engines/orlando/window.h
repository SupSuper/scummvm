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

#ifndef ORLANDO_WINDOW_H
#define ORLANDO_WINDOW_H

#include "common/rect.h"

namespace Graphics {
	class ManagedSurface;
	struct Surface;
}

namespace Orlando {

class GraphicsManager;

/**
 * Represents a surface window for a scene element.
 * Elements draw to this buffer instead of directly to the screen.
 */
class Window {
	Graphics::ManagedSurface *_surface;
	Common::Point _pos;
	bool _visible;

public:
	Window(const Common::Rect &bounds);
	~Window();
	void setVisible(bool visible) { _visible = visible; }

	void change(const Common::Rect &bounds);
	void drawFrom(const Graphics::Surface *surface, const Common::Point &pos = Common::Point(), bool flipped = false, float scale = 1.0f) const;
	void drawTo(GraphicsManager *graphics) const;
};

} // End of namespace Orlando

#endif
