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

static const PlainGameDescriptor trecisionGames[] = {
	{"nl", "Nightlong: Union City Conspiracy"},
	{0, 0}
};

namespace Trecision {

static const ADGameDescription gameDescriptions[] = {
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 436228),
		Common::EN_ANY,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 457299),
		Common::DE_DEU,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 436697),
		Common::ES_ESP,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 456209),
		Common::FR_FRA,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 446634),
		Common::IT_ITA,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"nl",
		0,
		AD_ENTRY1s("data.nl", "7665db13ad2a1ceb576531be3e1efb30", 432900),
		Common::RU_RUS,
		Common::kPlatformWindows,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	AD_TABLE_END_MARKER
};

#define GAMEOPTION_ORIGINAL_SAVELOAD GUIO_GAMEOPTIONS1

static const ADExtraGuiOptionsMap optionsList[] = {

	{
		GAMEOPTION_ORIGINAL_SAVELOAD,
		{
			_s("Use original save/load screens"),
			_s("Use the original save/load screens instead of the ScummVM ones"),
			"originalsaveload",
			false
		}
	},
};

} // End of namespace Trecision

static const char *directoryGlobs[] = {
	"autorun",
	0
};

class TrecisionMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	TrecisionMetaEngineDetection() : AdvancedMetaEngineDetection(Trecision::gameDescriptions, sizeof(ADGameDescription), trecisionGames, Trecision::optionsList) {
		_maxScanDepth = 2;
		_directoryGlobs = directoryGlobs;
		_guiOptions = GUIO2(GUIO_NOMIDI, GAMEOPTION_ORIGINAL_SAVELOAD);
	}

	const char *getEngineId() const override {
		return "trecision";
	}

	const char *getName() const override {
		return "Trecision Adventure Engine";
	}

	const char *getOriginalCopyright() const override {
		return "(C) 1997-98 Trecision S.p.A.";
	}
};

REGISTER_PLUGIN_STATIC(TRECISION_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, TrecisionMetaEngineDetection);
