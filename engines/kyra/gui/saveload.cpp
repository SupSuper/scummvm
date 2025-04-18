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

#include "kyra/kyra_v1.h"
#include "kyra/engine/util.h"

#include "common/savefile.h"
#include "common/system.h"

#include "graphics/thumbnail.h"
#include "graphics/surface.h"

#define CURRENT_SAVE_VERSION 24

#define GF_FLOPPY  (1 <<  0)
#define GF_TALKIE  (1 <<  1)
#define GF_FMTOWNS (1 <<  2)

namespace Kyra {

WARN_UNUSED_RESULT KyraEngine_v1::ReadSaveHeaderError KyraEngine_v1::readSaveHeader(Common::SeekableReadStream *in, SaveHeader &header, bool skipThumbnail) {
	uint32 type = in->readUint32BE();
	header.originalSave = false;
	header.oldHeader = false;
	header.flags = 0;

	if (type == MKTAG('K', 'Y', 'R', 'A') || type == MKTAG('A', 'R', 'Y', 'K')) { // old Kyra1 header ID
		header.gameID = GI_KYRA1;
		header.oldHeader = true;
	} else if (type == MKTAG('H', 'O', 'F', 'S')) { // old Kyra2 header ID
		header.gameID = GI_KYRA2;
		header.oldHeader = true;
	} else if (type == MKTAG('W', 'W', 'S', 'V')) {
		header.gameID = in->readByte();
	} else {
		// try checking for original save header
		const int descriptionSize[3] = { 30, 80, 60 };
		char descriptionBuffer[81];

		bool saveOk = false;

		for (uint i = 0; i < ARRAYSIZE(descriptionSize) && !saveOk; ++i) {
			if (in->size() < descriptionSize[i] + 6)
				continue;

			in->seek(0, SEEK_SET);
			in->read(descriptionBuffer, descriptionSize[i]);
			descriptionBuffer[descriptionSize[i]] = 0;

			Util::convertString_KYRAtoGUI(descriptionBuffer, 81);

			type = in->readUint32BE();
			header.version = in->readUint16LE();
			if (type == MKTAG('M', 'B', 'L', '3') && header.version == 100) {
				saveOk = true;
				header.description = descriptionBuffer;
				header.gameID = GI_KYRA2;
				break;
			} else if (type == MKTAG('M', 'B', 'L', '4') && header.version == 102) {
				saveOk = true;
				header.description = descriptionBuffer;
				header.gameID = GI_KYRA3;
				break;
			} else if (type == MKTAG('C','D','0','4')) {
				header.version = in->readUint32BE();
				// We don't check the minor version, since the original doesn't do that either and it isn't required.
				if (header.version != MKTAG(' ','C','D','1'))
					continue;
				saveOk = true;
				header.description = descriptionBuffer;
				header.gameID = GI_LOL;
				in->seek(6, SEEK_CUR);
				break;
			}
		}

		if (saveOk) {
			header.originalSave = true;
			header.description = descriptionBuffer;
			return kRSHENoError;
		} else {
			return kRSHEInvalidType;
		}
	}

	header.version = in->readUint32BE();
	if (header.version > CURRENT_SAVE_VERSION || (header.oldHeader && header.version > 8) || (type == MKTAG('A', 'R', 'Y', 'K') && header.version > 3))
		return kRSHEInvalidVersion;

	// Versions prior to 9 are using a fixed length description field
	if (header.version <= 8) {
		char buffer[31];
		in->read(buffer, 31);
		// WORKAROUND: Old savegames could contain a missing termination 0 at the
		// end so we manually add it.
		buffer[30] = 0;
		header.description = buffer;
	} else {
		header.description = "";
		for (char c = 0; (c = in->readByte()) != 0;)
			header.description += c;
	}

	if (header.version < 20)
		header.description = Util::convertISOToUTF8(header.description);

	if (header.version >= 2)
		header.flags = in->readUint32BE();

	if (header.version >= 14) {
		if (!Graphics::loadThumbnail(*in, header.thumbnail, skipThumbnail)) {
			if (!skipThumbnail)
				return kRSHEIoError;
		}
	} else {
		header.thumbnail = nullptr;
	}

	if (header.version >= 21) {
		header.timeDate.tm_sec = in->readSint32BE();
		header.timeDate.tm_min = in->readSint32BE();
		header.timeDate.tm_hour = in->readSint32BE();
		header.timeDate.tm_mday = in->readSint32BE();
		header.timeDate.tm_mon = in->readSint32BE();
		header.timeDate.tm_year = in->readSint32BE();
		header.timeDate.tm_wday = in->readSint32BE();
		header.totalPlaySecs = in->readUint32BE();
	} else {
		header.totalPlaySecs = 0;
		memset(&header.timeDate, 0, sizeof(TimeDate));
	}

	return ((in->err() || in->eos()) ? kRSHEIoError : kRSHENoError);
}

Common::SeekableReadStream *KyraEngine_v1::openSaveForReading(const char *filename, SaveHeader &header, bool checkID) {
	Common::SeekableReadStream *in = nullptr;
	if (!(in = _saveFileMan->openForLoading(filename)))
		return nullptr;

	ReadSaveHeaderError errorCode = KyraEngine_v1::readSaveHeader(in, header);
	if (errorCode != kRSHENoError) {
		if (errorCode == kRSHEInvalidType)
			warning("No ScummVM Kyra engine savefile header");
		else if (errorCode == kRSHEInvalidVersion)
			warning("Savegame is not the right version (%u, '%s')", header.version, header.oldHeader ? "true" : "false");
		else if (errorCode == kRSHEIoError)
			warning("Load failed '%s'", filename);

		delete in;
		return nullptr;
	}

	if (!header.originalSave) {
		if (!header.oldHeader) {
			if (header.gameID != _flags.gameID && checkID) {
				warning("Trying to load saved game from other game (saved game: %u, running game: %u)", header.gameID, _flags.gameID);
				delete in;
				return nullptr;
			}
		}

		if (header.version < 2) {
			warning("Make sure your savefile was from this version! (too old savefile version to detect that)");
		} else if (checkID) {
			if ((header.flags & GF_FLOPPY) && (_flags.isTalkie || _flags.platform == Common::kPlatformFMTowns || _flags.platform == Common::kPlatformPC98)) {
				warning("Can not load DOS Floppy savefile for this (non DOS Floppy) gameversion");
				delete in;
				return nullptr;
			} else if ((header.flags & GF_TALKIE) && !(_flags.isTalkie)) {
				warning("Can not load DOS CD-ROM savefile for this (non DOS CD-ROM) gameversion");
				delete in;
				return nullptr;
			} else if ((header.flags & GF_FMTOWNS) && !(_flags.platform == Common::kPlatformFMTowns || _flags.platform == Common::kPlatformPC98)) {
				warning("Can not load FM-TOWNS/PC98 savefile for this (non FM-TOWNS/PC98) gameversion");
				delete in;
				return nullptr;
			}
		}
	}

	return in;
}

Common::OutSaveFile *KyraEngine_v1::openSaveForWriting(const char *filename, const char *saveName, const Graphics::Surface *thumbnail) const {
	if (shouldQuit())
		return nullptr;

	Common::WriteStream *out = nullptr;
	if (!(out = _saveFileMan->openForSaving(filename))) {
		warning("Can't create file '%s', game not saved", filename);
		return nullptr;
	}

	// Savegame version
	out->writeUint32BE(MKTAG('W', 'W', 'S', 'V'));
	out->writeByte(_flags.gameID);
	out->writeUint32BE(CURRENT_SAVE_VERSION);
	out->write(saveName, strlen(saveName) + 1);
	if (_flags.isTalkie)
		out->writeUint32BE(GF_TALKIE);
	else if (_flags.platform == Common::kPlatformFMTowns || _flags.platform == Common::kPlatformPC98)
		out->writeUint32BE(GF_FMTOWNS);
	else
		out->writeUint32BE(GF_FLOPPY);

	if (out->err()) {
		warning("Can't write file '%s'. (Disk full?)", filename);
		delete out;
		return nullptr;
	}

	Graphics::Surface *genThumbnail = nullptr;
	if (!thumbnail)
		thumbnail = genThumbnail = generateSaveThumbnail();

	if (thumbnail)
		Graphics::saveThumbnail(*out, *thumbnail);
	else
		Graphics::saveThumbnail(*out);

	if (genThumbnail) {
		genThumbnail->free();
		delete genThumbnail;
	}

	TimeDate td;
	_system->getTimeAndDate(td);

	out->writeSint32BE(td.tm_sec);
	out->writeSint32BE(td.tm_min);
	out->writeSint32BE(td.tm_hour);
	out->writeSint32BE(td.tm_mday);
	out->writeSint32BE(td.tm_mon);
	out->writeSint32BE(td.tm_year);
	out->writeSint32BE(td.tm_wday);

	out->writeUint32BE(_totalPlaySecs);

	return new Common::OutSaveFile(out);
}

const char *KyraEngine_v1::getSavegameFilename(int num) {
	_savegameFilename = getSavegameFilename(_targetName, num);
	return _savegameFilename.c_str();
}

Common::String KyraEngine_v1::getSavegameFilename(const Common::String &target, int num) {
	assert(num >= 0 && num <= 999);
	return target + Common::String::format(".%03d", num);
}

bool KyraEngine_v1::saveFileLoadable(int slot) {
	if (slot < 0 || slot > 999)
		return false;

	SaveHeader header;
	Common::SeekableReadStream *in = openSaveForReading(getSavegameFilename(slot), header);

	if (in) {
		delete in;
		return true;
	}

	return false;
}

void KyraEngine_v1::loadGameStateCheck(int slot) {
	// FIXME: Instead of throwing away the error returned by
	// loadGameState, we should use it / augment it.
	if (loadGameState(slot).getCode() != Common::kNoError) {
		const char *filename = getSavegameFilename(slot);
		Common::String errorMessage = "Could not load savegame: '";
		errorMessage += filename;
		errorMessage += "'";

		GUIErrorMessage(errorMessage);
		error("%s", errorMessage.c_str());
	}
}

} // End of namespace Kyra
