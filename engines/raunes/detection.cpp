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
#include "raunes/raunes.h"

namespace Raunes {
static const PlainGameDescriptor raunesGames[] = {
	{ "raunes1", "Udoiana Raunes" },
	{ "raunes2", "Udoiana Raunes - Special Edition" },
	{ "raunes3", "Udoiana Raunes in search for Indiana Jones 4" },
	{ nullptr, nullptr }
};

static const ADGameDescription gameDescriptions[] = {
	{
		"raunes1",
		nullptr,
		AD_ENTRY1s("raunes.dat", "699ff024dbd82978401654ae03ede945", 1220677),
		Common::DE_DEU,
		Common::kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"raunes2",
		nullptr,
		AD_ENTRY1s("raunes.dat", "b2e0328fe2a090291ce7e6a59fdd063b", 1785818),
		Common::EN_ANY,
		Common::kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"raunes2",
		nullptr,
		AD_ENTRY1s("raunes.dat", "f7b6b09d0b8bcd03d1921c57faf0a4ff", 1794600),
		Common::DE_DEU,
		Common::kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	{
		"raunes3",
		nullptr,
		AD_ENTRY1s("raunes.dat", "eb46ccf33c37dea3a5b6ef5fdb9dbd70", 1851778),
		Common::EN_ANY,
		Common::kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO0()
	},
	AD_TABLE_END_MARKER
};
} // End of namespace Raunes

class RaunesMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	RaunesMetaEngineDetection() : AdvancedMetaEngineDetection(Raunes::gameDescriptions, sizeof(ADGameDescription), Raunes::raunesGames) {
		_guiOptions = GUIO2(GUIO_NOMIDI, GUIO_NOSPEECH);
	}

	const char *getName() const override {
		return "raunes";
	}

	const char *getEngineName() const override {
		return "Udoiana Raunes";
	}

	const char *getOriginalCopyright() const override {
		return "(C) 1997-2005 Stefan Zwanzger & Thomas Wagner";
	}
};

REGISTER_PLUGIN_STATIC(RAUNES_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, RaunesMetaEngineDetection);
