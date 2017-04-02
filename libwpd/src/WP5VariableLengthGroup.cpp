/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5PageFormatGroup.h"
#include "WP5VariableLengthGroup.h"
#include "WP5UnsupportedVariableLengthGroup.h"
#include "libwpd_internal.h"
#include "WP5HeaderFooterGroup.h"
#include "WP5FontGroup.h"
#include "WP5BoxGroup.h"
#include "WP5FootnoteEndnoteGroup.h"
#include "WP5DefinitionGroup.h"
#include "WP5TableEOLGroup.h"
#include "WP5TableEOPGroup.h"

WP5VariableLengthGroup::WP5VariableLengthGroup() :
	m_subGroup(0),
	m_size(0)
{
}

WP5VariableLengthGroup *WP5VariableLengthGroup::constructVariableLengthGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char group)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group Ox%x\n", group));
	switch (group)
	{
	case WP5_TOP_DEFINITION_GROUP:
		return new WP5DefinitionGroup(input, encryption);
	case WP5_TOP_PAGE_FORMAT_GROUP:
		return new WP5PageFormatGroup(input, encryption);
	case WP5_TOP_HEADER_FOOTER_GROUP:
		return new WP5HeaderFooterGroup(input, encryption);
	case WP5_TOP_FONT_GROUP:
		return new WP5FontGroup(input, encryption);
	case WP5_TOP_FOOTNOTE_ENDNOTE_GROUP:
		return new WP5FootnoteEndnoteGroup(input, encryption);
	case WP5_TOP_BOX_GROUP:
		return new WP5BoxGroup(input, encryption);
	case WP5_TOP_TABLE_EOL_GROUP:
		return new WP5TableEOLGroup(input, encryption);
	case WP5_TOP_TABLE_EOP_GROUP:
		return new WP5TableEOPGroup(input, encryption);
	default:
		// this is an unhandled group, just skip it
		return new WP5UnsupportedVariableLengthGroup(input, encryption);
	}
}

bool WP5VariableLengthGroup::isGroupConsistent(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char group)
{
	long startPosition = input->tell();

	try
	{
		unsigned char subGroup = readU8(input, encryption);
		unsigned short size = readU16(input, encryption);

		if (input->seek((startPosition + size - 1), librevenge::RVNG_SEEK_SET) || input->isEnd())
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}
		if (size != readU16(input, encryption))
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

void WP5VariableLengthGroup::_read(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();

	m_subGroup = readU8(input, encryption);
	m_size = (unsigned short)(readU16(input, encryption) + 4); // the length is the number of data bytes minus 4 (ie. the function codes)

	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %li, sub_group: 0x%2x, size: %u)\n", startPosition, m_subGroup, m_size));

	_readContents(input, encryption);

	input->seek((startPosition + m_size - 5), librevenge::RVNG_SEEK_SET);

	if (m_size != (readU16(input, encryption) + 4))
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
