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
#include "common/translation.h"
#include "engines/advancedDetector.h"

#include "orlando/orlando.h"

namespace Orlando {

static const PlainGameDescriptor orlandoGames[] = {
	{"orlando", "Jack Orlando"},
	{0, 0}
};

#define GAMEOPTION_EASY_VERSION GUIO_GAMEOPTIONS1

static const ADExtraGuiOptionsMap optionsList[] = {
	{
		GAMEOPTION_EASY_VERSION,
		{
			_s("Easy version"),
			_s("Director's Cut easy difficulty (only applies to new games)"),
			"easy",
			false
		}
	},

	AD_EXTRA_GUI_OPTIONS_TERMINATOR
};

enum {
	GF_HIGHPERF = (1 << 0)
};

/** Is this the Standard Performance version? */
bool OrlandoEngine::isVersionSP() const {
	return _gameDescription->platform == Common::kPlatformDOS && (_gameDescription->flags & GF_HIGHPERF) == 0;
}
/** Is this the High Performance version? */
bool OrlandoEngine::isVersionHP() const {
	return _gameDescription->platform == Common::kPlatformDOS  && (_gameDescription->flags & GF_HIGHPERF) != 0;
}
/** Is this the Director's Cut version? */
bool OrlandoEngine::isVersionDC() const {
	return _gameDescription->platform == Common::kPlatformWindows;
}

/**
 * Known game variants:
 * @li 1997 DOS version: Standard Performance
 * @li 1997 DOS version: High Performance (higher quality audio)
 * @li 2001 Windows version: Director's Cut (different UI and puzzles)
 * Digital releases are the same as the Windows version without CD checks.
 *
 * TODO: Find localized versions of original DOS release.
 */
static const ADGameDescription gameDescriptions[] = {
	{ // Standard English
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
	{ // High Performance English
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
	{ // Director's Cut English
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "507574735545af15cc1e95ef58accb98", 4052566),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GAMEOPTION_EASY_VERSION)
	},
	{ // Director's Cut German
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "25a8205e025f8a2fc38b7109feed5443", 4052566),
		Common::DE_DEU,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GAMEOPTION_EASY_VERSION)
	},
	{ // Director's Cut Polish
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "894181dc0d749a8a4df62077dc65403e", 4052566),
		Common::PL_POL,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GAMEOPTION_EASY_VERSION)
	},
	{ // Director's Cut Russian
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "4c8013d298f69b35fe45a493e881d73a", 4052566),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GAMEOPTION_EASY_VERSION)
	},
	{ // Director's Cut Chinese
		"orlando",
		"Director's Cut",
		AD_ENTRY1s("MENU.PAK", "c2fedd5778691a38a6544b4ff71064a9", 4052566),
		Common::ZH_CNA,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO1(GAMEOPTION_EASY_VERSION)
	},

	AD_TABLE_END_MARKER
};

class OrlandoMetaEngine : public AdvancedMetaEngine {
public:
	OrlandoMetaEngine() : AdvancedMetaEngine(gameDescriptions, sizeof(ADGameDescription), orlandoGames, optionsList) {
		_guiOptions = GUIO1(GUIO_NOMIDI);
	}

	virtual const char *getEngineId() const {
		return "orlando";
	}
	
	virtual const char *getName() const {
		return "Jack Orlando";
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
	return (f == kSupportsRTL ||
			f == kSupportsSubtitleOptions);
}

bool OrlandoMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc)
		*engine = new Orlando::OrlandoEngine(syst, desc);

	return desc != nullptr;
}

} // End of namespace Orlando

#if PLUGIN_ENABLED_DYNAMIC(ORLANDO)
REGISTER_PLUGIN_DYNAMIC(ORLANDO, PLUGIN_TYPE_ENGINE, Orlando::OrlandoMetaEngine);
#else
REGISTER_PLUGIN_STATIC(ORLANDO, PLUGIN_TYPE_ENGINE, Orlando::OrlandoMetaEngine);
#endif
