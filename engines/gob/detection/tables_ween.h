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

/* Detection tables for Ween: The Prophecy. */

#ifndef GOB_DETECTION_TABLES_WEEN_H
#define GOB_DETECTION_TABLES_WEEN_H

// -- DOS VGA Floppy --

{
	{
		"ween",
		"",
		AD_ENTRY1("intro.stk", "2bb8878a8042244dd2b96ff682381baa"),
		EN_GRB,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "de92e5c6a8c163007ffceebef6e67f7d", 7117568),
		EN_USA,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{ // Supplied by cybot_tmin in bug report #3084
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "6d60f9205ecfbd8735da2ee7823a70dc", 7014426),
		ES_ESP,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "4b10525a3782aa7ecd9d833b5c1d308b", 7029591),
		FR_FRA,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{ // Bugreport #11524
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "cae57980940b919305e33a65d0f1dcc3", 7017982),
		FR_FRA,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{ // Supplied by cartman_ on #scummvm
	{
		"ween",
		"",
		AD_ENTRY1("intro.stk", "63170e71f04faba88673b3f510f9c4c8"),
		DE_DEU,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{ // Supplied by glorfindel in bugreport #3193
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "8b57cd510da8a3bbd99e3a0297a8ebd1", 7018771),
		IT_ITA,
		kPlatformDOS,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},

// -- Amiga --

{ // Supplied by vampir_raziel in bug report #3055
	{
		"ween",
		"",
		AD_ENTRY2s("intro.stk", "bfd9d02faf3d8d60a2cf744f95eb48dd", 456570,
				   "ween.ins",  "d2cb24292c9ddafcad07e23382027218", 87800),
		EN_GRB,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},
{ // Supplied by vampir_raziel in bug report #3055
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "257fe669705ac4971efdfd5656eef16a", 457719),
		FR_FRA,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},
{ // Supplied by vampir_raziel in bug report #3055
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "dffd1ab98fe76150d6933329ca6f4cc4", 459458),
		FR_FRA,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},
{ // Supplied by vampir_raziel in bug report #3055
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "af83debf2cbea21faa591c7b4608fe92", 458192),
		DE_DEU,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},
{ // Supplied by goodoldgeorg in bug report #4139
	{
		"ween",
		"",
		AD_ENTRY2s("intro.stk", "dffd1ab98fe76150d6933329ca6f4cc4", 459458,
				   "ween.ins",  "d2cb24292c9ddafcad07e23382027218", 87800),
		IT_ITA,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},

{ // Supplied by CaptainHIT in bug report #11591
	{
		"ween",
		"",
		AD_ENTRY1s("intro.stk", "53c57051c69c641fcc5270a41d35e7d5", 458536),
		ES_ESP,
		kPlatformAmiga,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},

// -- Atari ST --

{ // Supplied by pwigren in bug report #3355
	{
		"ween",
		"",
		AD_ENTRY2s("intro.stk",		"bfd9d02faf3d8d60a2cf744f95eb48dd", 456570,
				   "music__5.snd",	"7d1819b9981ecddd53d3aacbc75f1cc8", 13446),
		EN_GRB,
		kPlatformAtariST,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},
{
	{
		"ween",
		"",
		AD_ENTRY1("intro.stk", "e6d13fb3b858cb4f78a8780d184d5b2c"),
		FR_FRA,
		kPlatformAtariST,
		ADGF_NO_FLAGS,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesNone,
	0, 0, 0
},

// -- Demos --

{
	{
		"ween",
		"Demo",
		AD_ENTRY1("intro.stk", "2e9c2898f6bf206ede801e3b2e7ee428"),
		EN_USA,
		kPlatformDOS,
		ADGF_DEMO,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, "show.tot", 0
},
{
	{
		"ween",
		"Demo (v2.0)", // dated 8/1/93
		AD_ENTRY1s("intro.stk", "15fb91a1b9b09684b28ac75edf66e504", 2340230),
		EN_USA,
		kPlatformDOS,
		ADGF_DEMO,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, "show.tot", 0
},
{
	{
		"ween",
		"Demo",
		AD_ENTRY1("intro.stk", "aca10b973c03ba8b8b2804f4e7029ece"),
		DE_DEU,
		kPlatformDOS,
		ADGF_DEMO,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{
	{
		"ween",
		"Demo",
		AD_ENTRY1("intro.stk", "aca10b973c03ba8b8b2804f4e7029ece"),
		EN_GRB,
		kPlatformDOS,
		ADGF_DEMO,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},
{
	{
		"ween",
		"v1.2 Demo",
		AD_ENTRY1s("intro.stk", "dcff8f3a7dd1f4c33fd94aa7659b7578", 2425477),
		EN_GRB,
		kPlatformDOS,
		ADGF_DEMO,
		GUIO2(GUIO_NOSUBTITLES, GUIO_NOSPEECH)
	},
	kGameTypeWeen,
	kFeaturesAdLib,
	0, 0, 0
},

#endif // GOB_DETECTION_TABLES_WEEN_H
