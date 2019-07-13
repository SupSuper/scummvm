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

#ifndef ORLANDO_GRAPHICS_H
#define ORLANDO_GRAPHICS_H

#include "graphics/pixelformat.h"
#include "graphics/font.h"

namespace Common {
	struct Point;
	class SeekableReadStream;
}

namespace Graphics {
	struct Surface;
	class ManagedSurface;
}

namespace Orlando {

struct TextColor {
	uint16 border;
	uint16 fill;

	uint32 toUint() const {
		return (uint32)border << 16 | fill;
	}
};

class OrlandoEngine;

/**
 * Manages all graphics operations for the Orlando engine.
 * The game runs at 640x480x16 using RGB565 pixels. 0 is transparent.
 * All graphics have the same format, no conversions necessary.
 * Drawing is performed on a buffer surface which is later blit to the screen.
 */
class GraphicsManager {
	static const int kScreenWidth = 640;
	static const int kScreenHeight = 480;

	OrlandoEngine *_vm;
	Graphics::ManagedSurface *_screenBuffer;

	/** Alpha-blends a color. */
	inline uint16 colorAlpha(uint16 color, float alpha) {
		uint8 r, g, b;
		colorToRGB(color, r, g, b);
		r *= alpha;
		g *= alpha;
		b *= alpha;
		return RGBToColor(r, g, b);
	}
public:
	static const Graphics::PixelFormat kScreenFormat;

	GraphicsManager(OrlandoEngine *vm);
	~GraphicsManager();
	/**
	 * Initializes the system screen and buffer surface with the required resolution.
	 * @return False if the pixel format isn't supported.
	 */
	bool setupScreen();
	/**
	 * Refreshes the screen contents with our buffer surface.
	 */
	void updateScreen();
	/**
	 * Loads a raw bitmap (normally *.BM) into a surface. They are uncompressed
	 * images composed of:
	 * @li int16: Width in pixels.
	 * @li int16: Height in pixels.
	 * @li int16[]: Row-first colors of each pixel.
	 * @param stream Stream to load from. Deleted after loading.
	 * @return New image surface. Must be free()d manually.
	 */	 
	Graphics::Surface *loadRawBitmap(Common::SeekableReadStream *stream) const;
	/**
	 * Loads a palette bitmap (normally *.PBM) into a surface. They are RLE-compressed
	 * images composed of:
	 * @li int16: Width in pixels.
	 * @li int16: Height in pixels.
	 * @li int16[]: 128-color palette.
	 * @li byte[]: Row-first indices of each pixel. The first 7 bits are the index and
	 * the last bit is a RLE flag (if 1, repeat index by the value of the next byte).
	 * @param stream Stream to load from. Deleted after loading.
	 * @return New image surface. Must be free()d manually.
	 */
	Graphics::Surface *loadPaletteBitmap(Common::SeekableReadStream *stream) const;
	/**
	 * Converts a RGB value to a color in the screen format.
	 * @param r Red component (0-255).
	 * @param g Green component (0-255).
	 * @param b Blue component (0-255).
	 * @return 16-bit color.
	 */
	inline uint16 RGBToColor(uint8 r, uint8 g, uint8 b) const {
		return kScreenFormat.RGBToColor(r, g, b);
	}
	/**
	* Converts a color in the screen format to a RGB value.
	* @param color Source color.
	* @param r Red component (0-255).
	* @param g Green component (0-255).
	* @param b Blue component (0-255).
	*/
	inline void colorToRGB(uint16 color, uint8 &r, uint8 &g, uint8 &b) const {
		kScreenFormat.colorToRGB(color, r, g, b);
	}
	/**
	 * Draws a surface to the screen.
	 * @param surface Source surface.
	 * @param pos Position on screen to draw to.
	 */
	void draw(const Graphics::Surface &surface, const Common::Point &pos = Common::Point());
	/**
	 * Draws a transparent surface to the screen.
	 * @param surface Source surface.
	 * @param pos Position on screen to draw to.
	 * @param window Draw window used to clip the sprite.
	 * @param flipped Flip the surface horizontally?
	 * @param scale Scale factor (1.0 = original size)
	 */
	void drawTransparent(const Graphics::Surface &surface, const Common::Point &pos = Common::Point(), const Common::Rect &window = Common::Rect(), bool flipped = false, float scale = 1.0f);
	/**
	 * Draws a text string to the screen.
	 * @param text Source text.
	 * @param pos Position on screen to draw to.
	 * @param width Width of the text region, used for wrapping and alignment.
	 * @param color Color for the text.
	 * @param align Text alignment relative to the width.
	 */
	void drawText(const Common::String &text, const Common::Point &pos, int width, const TextColor &color, Graphics::TextAlign align = Graphics::kTextAlignLeft);
	/**
	 * Draws a filled rectangle with shadow color.
	 * @param rect Rectangle coordinates.
	 * @param shadow Amount to darken (0 black - 1 unchanged).
	 * @param bevel Amount of bevel in pixels.
	 */
	void drawShadowRect(const Common::Rect &rect, float shadow, int bevel = 0);
	/**
	 * Draws a clickable button on screen.
	 * @param text Button text.
	 * @param rect Button coordinates.
	 * @param color Text color.
	 * @return True if the button was clicked.
	 */
	bool drawButton(const Common::String &text, const Common::Rect &rect, const TextColor &color);
	/**
	 * Draws a polygon border on screen.
	 * @param rect Polygon coordinates.
	 * @param color Line color.
	 */
	void drawPolygon(const Common::Rect &rect, uint16 color);
	/**
	 * Draws a polygon border on screen.
	 * @param points Array of point coordinates.
	 * @param numPoints Total number of points.
	 * @param color Line color.
	 */
	void drawPolygon(const Common::Point points[], int numPoints, uint16 color);
};

} // End of namespace Orlando

#endif
