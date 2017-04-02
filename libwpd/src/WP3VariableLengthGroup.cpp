/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3FileStructure.h"
#include "WP3VariableLengthGroup.h"
#include "WP3UnsupportedVariableLengthGroup.h"
#include "WP3EndOfLinePageGroup.h"
#include "WP3MiscellaneousGroup.h"
#include "WP3PageFormatGroup.h"
#include "WP3FontGroup.h"
#include "WP3DefinitionGroup.h"
#include "WP3DisplayGroup.h"
#include "WP3HeaderFooterGroup.h"
#include "WP3FootnoteEndnoteGroup.h"
#include "WP3TablesGroup.h"
#include "WP3WindowGroup.h"
#include "libwpd_internal.h"
#include <limits>

WP3VariableLengthGroup::WP3VariableLengthGroup() :
	m_subGroup(0),
	m_size(0)
{
}

WP3VariableLengthGroup *WP3VariableLengthGroup::constructVariableLengthGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char group)
{
	switch (group)
	{
	case WP3_PAGE_FORMAT_GROUP:
		return new WP3PageFormatGroup(input, encryption);
	case WP3_END_OF_LINE_PAGE_GROUP:
		return new WP3EndOfLinePageGroup(input, encryption);
	case WP3_MISCELLANEOUS_GROUP:
		return new WP3MiscellaneousGroup(input, encryption);
	case WP3_TABLES_GROUP:
		return new WP3TablesGroup(input, encryption);
	case WP3_FONT_GROUP:
		return new WP3FontGroup(input, encryption);
	case WP3_DEFINITION_GROUP:
		return new WP3DefinitionGroup(input, encryption);
	case WP3_HEADER_FOOTER_GROUP:
		return new WP3HeaderFooterGroup(input, encryption);
	case WP3_FOOTNOTE_ENDNOTE_GROUP:
		return new WP3FootnoteEndnoteGroup(input, encryption);
	case WP3_DISPLAY_GROUP:
		return new WP3DisplayGroup(input, encryption);
	case WP3_WINDOW_GROUP:
		return new WP3WindowGroup(input, encryption);
	default:
		// this is an unhandled group, just skip it
		return new WP3UnsupportedVariableLengthGroup(input, encryption);
	}
}

bool WP3VariableLengthGroup::isGroupConsistent(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char group)
{
	long startPosition = input->tell();
	if (0 > startPosition  || startPosition > ((std::numeric_limits<long>::max)()))
		return false;

	try
	{
		unsigned char subGroup = readU8(input, encryption);
		unsigned short size = readU16(input, encryption, true);
		if (startPosition + size < startPosition)
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}

		if (input->seek((startPosition + size - 1), librevenge::RVNG_SEEK_SET) || input->isEnd())
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}
		if (size != readU16(input, encryption, true))
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}
		if (subGroup != readU8(input, encryption))
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}
		if (group != readU8(input, encryption))
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}

		input->seek(startPosition, librevenge::RVNG_SEEK_SET);
		return true;
	}
	catch (...)
	{
		input->seek(startPosition, librevenge::RVNG_SEEK_SET);
		return false;
	}
}

void WP3VariableLengthGroup::_read(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));

	m_subGroup = readU8(input, encryption);
	m_size = (unsigned short)(readU16(input, encryption, true) + 4); // the length is the number of data bytes minus 4 (ie. the function codes)

	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %li, sub_group: 0x%x, size: %u)\n", startPosition, m_subGroup, m_size));

	_readContents(input, encryption);

	input->seek((startPosition + m_size - 5), librevenge::RVNG_SEEK_SET);

	if (m_size != (readU16(input, encryption, true) + 4))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	if (m_subGroup != readU8(input, encryption))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}

	input->seek((startPosition + m_size - 1), librevenge::RVNG_SEEK_SET);

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
