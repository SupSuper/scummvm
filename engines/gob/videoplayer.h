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
 *
 * This file is dual-licensed.
 * In addition to the GPLv3 license mentioned above, this code is also
 * licensed under LGPL 2.1. See LICENSES/COPYING.LGPL file for the
 * full text of the license.
 *
 */

#ifndef GOB_VIDEOPLAYER_H
#define GOB_VIDEOPLAYER_H

#include "common/array.h"
#include "common/list.h"
#include "common/rect.h"
#include "common/str.h"

#include "graphics/surface.h"

#include "gob/util.h"
#include "gob/draw.h"

namespace Video {
class CoktelDecoder;
}

namespace Gob {

class GobEngine;
class DataStream;

class VideoPlayer {
public:
	enum Flags {
		kFlagNone                  = 0x000000,
		kFlagUseBackSurfaceContent = 0x000040, ///< Use the back surface as a video "base".
		kFlagFrontSurface          = 0x000080, ///< Draw directly into the front surface.
		kFlagNoVideo               = 0x000100, ///< Only sound.
		kFlagOtherSurface          = 0x000800, ///< Draw into a specific sprite.
		kFlagScreenSurface         = 0x400000  ///< Draw into a newly created sprite of screen dimensions.
	};

	/** Video format. */
	enum Type {
		kVideoTypeTry    = -1, ///< Try any format.
		kVideoTypeIMD    =  0,
		kVideoTypePreIMD =  1, ///< Early IMD format found in Fascination.
		kVideoTypeVMD    =  2,
		kVideoTypeRMD    =  3  ///< VMD containing "reversed" video.
	};

	struct Properties {
		Type type; ///< Type of the video to open.

		int sprite; ///< The sprite onto which to draw the video.

		int32 x;      ///< X coordinate of the video.
		int32 y;      ///< Y coordinate of the video.
		int32 width;  ///< Width of the video.
		int32 height; ///< Height of the video.

		uint32 flags; ///< Video flags.

		bool switchColorMode; ///< Switch between paletted / true color modes?

		int32 startFrame; ///< Frame to start playback from.
		int32 lastFrame;  ///< Frame to stop playback at.
		int32 endFrame;   ///< Last frame of this playback cycle.

		bool forceSeek; ///< Force the seeking to the start frame.

		int16 breakKey; ///< Keycode of the break/abort key.

		uint16 palCmd;      ///< Palette command.
		int16 palStart;    ///< Palette entry to start with.
		int16 palEnd;      ///< Palette entry to end at.
		int32 palFrame;    ///< Frame to apply the palette command at.

		bool noBlock; ///< Non-blocking "live" video?

		bool loop; ///< Loop the video?
		bool fade; ///< Fade in?

		bool waitEndFrame; ///< Wait for the frame's time to run out?

		bool hasSound; ///< Does the video have sound?
		bool canceled; ///< Was the video canceled?

		int slot; ///< Explicit slot index (-1 = auto).
		bool reuseSlotWitSameFilename;

		bool noWaitSound;

		Properties();
	};

	VideoPlayer(GobEngine *vm);
	~VideoPlayer();

	void evaluateFlags(Properties &properties);

	int  openVideo(bool primary, const Common::String &file, Properties &properties);
	bool closeVideo(int slot = 0);

	void closeLiveVideos();
	void closeAll();

	bool reopenVideo(int slot = 0);
	bool reopenAll();

	void pauseVideo(int slot, bool pause);
	void pauseAll(bool pause);

	void finishVideoSound(int slot);
	void waitSoundEnd(int slot);

	bool play(int slot, Properties &properties);
	void waitEndFrame(int slot, bool onlySound = false);
	int32 getExpectedFrameFromCurrentTime(int slot);

	bool isPlayingLive() const;
	bool isSoundPlaying() const;

	void updateLive(bool force = false, int exceptSlot = -1);

	bool slotIsOpen(int slot = 0) const;

	Common::String getFileName(int slot = 0) const;

	uint32 getFrameCount     (int slot = 0) const;
	uint32 getCurrentFrame   (int slot = 0) const;
	uint16 getWidth          (int slot = 0) const;
	uint16 getHeight         (int slot = 0) const;
	uint16 getDefaultX       (int slot = 0) const;
	uint16 getDefaultY       (int slot = 0) const;
	uint32 getFlags          (int slot = 0) const;
	uint16 getSoundFlags     (int slot = 0) const;
	uint32 getVideoBufferSize(int slot = 0) const;
	bool   hasVideo          (int slot = 0) const;


	const Common::List<Common::Rect> *getDirtyRects(int slot = 0) const;

	bool                      hasEmbeddedFile(const Common::String &fileName, int slot = 0) const;
	Common::SeekableReadStream *getEmbeddedFile(const Common::String &fileName, int slot = 0);

	int32 getSubtitleIndex(int slot = 0) const;

	void writeVideoInfo(const Common::String &file, uint16 varX, uint16 varY,
						uint16 varFrames, uint16 varWidth, uint16 varHeight);

	bool copyFrame(int slot, Surface &dest,
			uint16 left, uint16 top, uint16 width, uint16 height, uint16 x, uint16 y,
			int32 transp = -1, bool yAxisReflexion = false) const;

private:
	struct Video {
		::Video::CoktelDecoder *decoder;
		Common::String fileName;

		SurfacePtr surface;
		Common::SharedPtr<Graphics::Surface> tmpSurfBppConversion;
		uint32 *highColorMap;

		Properties properties;

		bool live;

		Video();

		bool isEmpty() const;
		void close();

		void reopen();
	};

	static const int kVideoSlotCount = 32;
	static const int kPrimaryVideoSlot = 0;
	static const int kLiveVideoSlotCount = 6;
	static const int kVideoSlotWithCurFrameVarCount = 4;

	static const char *const _extensions[];

	GobEngine *_vm;

	// _videoSlots[0] is reserved for the "primary" video
	Video _videoSlots[kVideoSlotCount];

	bool _needBlit;

	bool _noCursorSwitch;
	bool _woodruffCohCottWorkaround;

	const Video *getVideoBySlot(int slot) const;
	Video *getVideoBySlot(int slot);

	int getNextFreeSlot();

	Common::String findFile(const Common::String &file, Properties &properties);

	::Video::CoktelDecoder *openVideo(const Common::String &file, Properties &properties);

	bool reopenVideo(Video &video);

	bool lastFrameReached(Video &video, Properties &properties);
	bool playFrame(int slot, Properties &properties);

	void checkAbort(Video &video, Properties &properties);
	void evalBgShading(Video &video);

	void copyPalette(const Video &video, int16 palStart, int16 palEnd);

	void updateLive(int slot, bool force = false);
};

} // End of namespace Gob

#endif // GOB_VIDEOPLAYER_H
