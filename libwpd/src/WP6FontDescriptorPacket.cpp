/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */
#include <string.h>
#include <limits>
#include "WP6FontDescriptorPacket.h"
#include "libwpd_internal.h"
#include <string>

const char *FONT_WEIGHT_STRINGS [] = {	"Bold", "bold", "Demi", "demi", "Extended", "extended",
                                        "Extra", "extra", "Headline", "headline", "Light", "light",
                                        "Medium", "medium", "Normal", "normal", "Regular", "regular",
                                        "Standaard", "standaard", "Standard", "standard"
                                     };
const char *USELESS_WP_POSTFIX = "-WP";
#define countElements(a) ((sizeof(a) / sizeof(a[0])))

WP6FontDescriptorPacket::WP6FontDescriptorPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int /* id */, unsigned dataOffset, unsigned dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_characterWidth(0),
	m_ascenderHeight(0),
	m_xHeight(0),
	m_descenderHeight(0),
	m_italicsAdjust(0),
	m_primaryFamilyId(0),
	m_primaryFamilyMemberId(0),
	m_scriptingSystem(0),
	m_primaryCharacterSet(0),
	m_width(0),
	m_weight(0),
	m_attributes(0),
	m_generalCharacteristics(0),
	m_classification(0),
	m_fill(0),
	m_fontType(0),
	m_fontSourceFileType(0),
	m_fontNameLength(0),
	m_fontName()
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6FontDescriptorPacket::~WP6FontDescriptorPacket()
{
}

void WP6FontDescriptorPacket::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	// short sized characteristics
	m_characterWidth = readU16(input, encryption);
	m_ascenderHeight = readU16(input, encryption);
	m_xHeight = readU16(input, encryption);
	m_descenderHeight = readU16(input, encryption);
	m_italicsAdjust = readU16(input, encryption);
	// byte sized characteristics
	m_primaryFamilyMemberId = readU8(input, encryption);
	m_primaryFamilyId = readU8(input, encryption);
	m_scriptingSystem = readU8(input, encryption);
	m_primaryCharacterSet = readU8(input, encryption);
	m_width = readU8(input, encryption);
	m_weight = readU8(input, encryption);
	m_attributes = readU8(input, encryption);
	m_generalCharacteristics = readU8(input, encryption);
	m_classification = readU8(input, encryption);
	m_fill = readU8(input, encryption);
	m_fontType = readU8(input, encryption);
	m_fontSourceFileType = readU8(input, encryption);

	m_fontNameLength = readU16(input, encryption);

	_readFontName(input, encryption);
}

void WP6FontDescriptorPacket::_readFontName(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	if (m_fontNameLength > ((std::numeric_limits<unsigned short>::max)() / 2))
		m_fontNameLength = ((std::numeric_limits<unsigned short>::max)() / 2);
	if (m_fontNameLength)
	{
		for (unsigned short i=0; i<(m_fontNameLength/2); i++)
		{
			unsigned short charWord = readU16(input, encryption);
			unsigned char characterSet = (unsigned char)((charWord >> 8) & 0x00FF);
			unsigned char character = (unsigned char)(charWord & 0xFF);

			if (character == 0x00 && characterSet == 0x00)
				break;

			const unsigned *chars;
			int len = extendedCharacterWP6ToUCS4(character, characterSet, &chars);

			for (int j = 0; j < len; j++)
				appendUCS4(m_fontName, chars[j]);
		}

		WPD_DEBUG_MSG(("WordPerfect: stripping font name (original: %s)\n", m_fontName.cstr()));
		std::string stringValue(m_fontName.cstr());
		std::string::size_type pos;
		for (unsigned k = 0; k < countElements(FONT_WEIGHT_STRINGS); k++)
		{
			if (!stringValue.empty())
				while ((pos = stringValue.find(FONT_WEIGHT_STRINGS[k])) != std::string::npos)
					stringValue.replace(pos, strlen(FONT_WEIGHT_STRINGS[k]),"");
		}
		// SPECIAL CASE: eliminate the -WP postfix (if it's there), which isn't spaced out from
		// the rest of the font
		if (!stringValue.empty())
			while ((pos = stringValue.find(USELESS_WP_POSTFIX)) != std::string::npos)
				stringValue.replace(pos, strlen(USELESS_WP_POSTFIX), "");
		// also consume any whitespace at the end of the font.
		if (!stringValue.empty())
			while ((pos = stringValue.find("  ")) != std::string::npos)
				stringValue.replace(pos, strlen("  "), " ");
		if (!stringValue.empty())
			while ((pos = stringValue.find(" ", stringValue.size() - 1)) != std::string::npos)
				stringValue.replace(pos, strlen(" "), "");
		if (!stringValue.empty())
			while ((pos = stringValue.find("-", stringValue.size() - 1)) != std::string::npos)
				stringValue.replace(pos, strlen("-"), "");

		m_fontName = librevenge::RVNGString(stringValue.c_str());
		WPD_DEBUG_MSG(("WordPerfect: stripping font name (final: %s)\n", m_fontName.cstr()));
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
