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

#ifndef M4_RELOC_H
#define M4_RELOC_H

#include "common/str.h"
#include "m4/m4_types.h"

namespace M4 {

typedef void **MemHandle;
typedef int32 Size;

inline void HLock(Handle h) {}
inline void HUnLock(Handle h) {}
inline void HPurge(Handle h) {}
inline void HNoPurge(Handle h) {}

inline bool MakeMem(size_t FreeBlockNeeded, const char *) {
	return true;
}

MemHandle MakeNewHandle(size_t size, const Common::String &);
bool mem_ReallocateHandle(MemHandle h, size_t size, const Common::String &name);
MemHandle NewHandle(size_t size, const Common::String &str);
void DisposeHandle(MemHandle handle);

} // namespace M4

#endif
