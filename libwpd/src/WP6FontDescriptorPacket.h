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

#ifndef WP6FONTDESCRIPTORPACKET_H
#define WP6FONTDESCRIPTORPACKET_H

#include <librevenge/librevenge.h>
#include <libwpd/libwpd.h>
#include "WP6PrefixDataPacket.h"

class WP6FontDescriptorPacket : public WP6PrefixDataPacket
{
public:
	WP6FontDescriptorPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int id, unsigned dataOffset, unsigned dataSize);
	~WP6FontDescriptorPacket();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	const librevenge::RVNGString &getFontName() const
	{
		return m_fontName;
	}

private:
	void _readFontName(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

	WP6FontDescriptorPacket(const WP6FontDescriptorPacket &);
	WP6FontDescriptorPacket &operator=(const WP6FontDescriptorPacket &);
	unsigned short m_characterWidth;
	unsigned short m_ascenderHeight;
	unsigned short m_xHeight;
	unsigned short m_descenderHeight;
	unsigned short m_italicsAdjust;
	unsigned char m_primaryFamilyId; // family id's are supposed to be one unified element, but I split them up to ease parsing
	unsigned char m_primaryFamilyMemberId;

	unsigned char m_scriptingSystem;
	unsigned char m_primaryCharacterSet;
	unsigned char m_width;
	unsigned char m_weight;
	unsigned char m_attributes;
	unsigned char m_generalCharacteristics;
	unsigned char m_classification;
	unsigned char m_fill; // fill byte
	unsigned char m_fontType;
	unsigned char m_fontSourceFileType;

	unsigned short m_fontNameLength;

	librevenge::RVNGString m_fontName;
};
#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
