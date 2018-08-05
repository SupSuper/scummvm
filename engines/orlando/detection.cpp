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

#include "base/plugins.h"

#include "engines/advancedDetector.h"

#include "orlando/orlando.h"

static const PlainGameDescriptor orlandoGames[] = {
	{"orlando", "Jack Orlando"},
	{0, 0}
};

namespace Orlando {

enum {
	GF_HIGHPERF = (1 << 0)
};

/**
 * Is this the Standard Performance version?
 */
bool OrlandoEngine::isStandardPerf() const {
	return _gameDescription->platform == Common::kPlatformDOS && (_gameDescription->flags & GF_HIGHPERF) == 0;
}
/**
 * Is this the High Performance version?
 */
bool OrlandoEngine::isHighPerf() const {
	return _gameDescription->platform == Common::kPlatformDOS  && (_gameDescription->flags & GF_HIGHPERF) != 0;
}
/**
 * Is this the Director's Cut version?
 */
bool OrlandoEngine::isDirectorCut() const {
	return _gameDescription->platform == Common::kPlatformWindows;
}

/**
 * Known game variants:
 * @li 1997 DOS version: Standard Performance
 * @li 1997 DOS version: High Performance (higher quality audio)
 * @li 2001 Windows version: Director's Cut (different UI and puzzles)
 * Digital rereleases are the same as the Windows version without CD checks.
 *
 * TODO: Support different language versions (assuming English for now).
 */
static const ADGameDescription gameDescriptions[] = {
	{
		"orlando",
		"Standard Performance",
		{
			{ "MENU.PAK", 0, "3002f66028d63a6f2bb81e68c033b6e6", 2279621 },
			{ "MUSIC.PAK", 0, "1eee80995fa625a8dbd2fecb15263737", 4077007 },
			AD_LISTEND
		},
		Common::EN_ANY,
		Common::kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"orlando",
		"High Performance",
		{
			{ "MENU.PAK", 0, "3002f66028d63a6f2bb81e68c033b6e6", 2279621 },
			{ "MUSIC16.PAK", 0, "37f1dbc6dec2a155d42fffaf39b083ff", 2809337 },
			AD_LISTEND
		},
		Common::EN_ANY,
		Common::kPlatformDOS,
		GF_HIGHPERF,
		GUIO0()
	},
	{
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "507574735545af15cc1e95ef58accb98", 4052566),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},

	AD_TABLE_END_MARKER
};

} // End of namespace Orlando

class OrlandoMetaEngine : public AdvancedMetaEngine {
public:
	OrlandoMetaEngine() : AdvancedMetaEngine(Orlando::gameDescriptions, sizeof(ADGameDescription), orlandoGames) {
		_singleId = "orlando";
		_guiOptions = GUIO1(GUIO_NOMIDI);
	}

	virtual const char *getName() const {
		return "Jack Orlando Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Jack Orlando (C) ToonTRAXX, TopWare Interactive";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
};

bool OrlandoMetaEngine::hasFeature(MetaEngineFeature f) const {
	return false;
}

bool Orlando::OrlandoEngine::hasFeature(EngineFeature f) const {
	switch (f) {
	case kSupportsRTL:
		return true;
	default:
		return false;
	}
}

bool OrlandoMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc)
		*engine = new Orlando::OrlandoEngine(syst, desc);

	return desc != nullptr;
}

#if PLUGIN_ENABLED_DYNAMIC(ORLANDO)
REGISTER_PLUGIN_DYNAMIC(ORLANDO, PLUGIN_TYPE_ENGINE, OrlandoMetaEngine);
#else
REGISTER_PLUGIN_STATIC(ORLANDO, PLUGIN_TYPE_ENGINE, OrlandoMetaEngine);
#endif
