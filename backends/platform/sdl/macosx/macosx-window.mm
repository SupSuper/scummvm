/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Disable symbol overrides so that we can use system headers.
#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include "backends/platform/sdl/macosx/macosx-window.h"
#include "backends/platform/sdl/macosx/macosx-compat.h"
#include <AppKit/NSWindow.h>

float SdlWindow_MacOSX::getDpiScalingFactor() const {
#if SDL_VERSION_ATLEAST(2, 0, 0) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
#if SDL_VERSION_ATLEAST(3, 0, 0)
	if (_window) {
		NSWindow *nswindow = (__bridge NSWindow *)SDL_GetPointerProperty(SDL_GetWindowProperties(_window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#else
	SDL_SysWMinfo wmInfo;
	if (getSDLWMInformation(&wmInfo)) {
		NSWindow *nswindow = wmInfo.info.cocoa.window;
#endif
		if ([nswindow respondsToSelector:@selector(backingScaleFactor)]) {
			debug(4, "Reported DPI ratio: %g", [nswindow backingScaleFactor]);
			return [nswindow backingScaleFactor];
		}
	}
#endif

	return SdlWindow::getDpiScalingFactor();
}
