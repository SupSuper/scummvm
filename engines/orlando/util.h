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

#ifndef ORLANDO_UTIL_H
#define ORLANDO_UTIL_H

#include "common/scummsys.h"
#include "common/str.h"

namespace Orlando {

/**
 * Replaces the end of a string with another string.
 * @param str Source string.
 * @param replace Replacement string.
 * @param count Amount of characters to replace (npos = same as replacement).
 * @return String with replacement applied.
 */
inline Common::String replaceEnd(Common::String str, const Common::String &replace, uint32 count = Common::String::npos) {
	if (count == Common::String::npos)
		count = replace.size();
	str.replace(str.size() - count, count, replace);
	return str;
}

/**
 * Deletes the first and last character of a string.
 * @param str Source string.
 */
inline void deleteFirstLast(Common::String &str) {
	str.deleteChar(0);
	str.deleteLastChar();
}

/**
 * Replaces every character in a string.
 * @param str Source string.
 * @param find Character to find.
 * @param replace Character to replace with.
 */
inline void replaceChar(Common::String &str, char find, char replace) {
	for (char *c = str.begin(); c != str.end(); c++) {
		if (*c == find)
			*c = replace;
	}
}

inline int toInt(const Common::String &str) {
	return atoi(str.c_str());
}

inline float toFloat(const Common::String &str) {
	return atof(str.c_str());
}

} // End of namespace Orlando

#endif
