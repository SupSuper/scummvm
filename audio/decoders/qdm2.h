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


#ifndef AUDIO_QDM2_H
#define AUDIO_QDM2_H

#include "common/scummsys.h"

#ifdef USE_QDM2

#include "common/types.h"

namespace Common {
class SeekableReadStream;
}

namespace Audio {

class Codec;

/**
 * Create a new Codec from the QDM2 data in the given stream.
 *
 * @param extraData           the QuickTime extra data stream
 * @param disposeExtraData    the QuickTime extra data stream
 * @return   a new Codec, or NULL, if an error occurred
 */
Codec *makeQDM2Decoder(Common::SeekableReadStream *extraData,
					   DisposeAfterUse::Flag disposeExtraData = DisposeAfterUse::NO);

} // End of namespace Audio

#endif // USE_QDM2
#endif // AUDIO_QDM2_H
