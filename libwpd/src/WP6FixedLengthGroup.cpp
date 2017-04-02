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

#include "WP6FixedLengthGroup.h"
#include "WP6ExtendedCharacterGroup.h"
#include "WP6UndoGroup.h"
#include "WP6AttributeGroup.h"
#include "WP6HighlightGroup.h"
#include "WP6UnsupportedFixedLengthGroup.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

WP6FixedLengthGroup::WP6FixedLengthGroup(unsigned char groupID)
	: m_group(groupID)
{
}

WP6FixedLengthGroup *WP6FixedLengthGroup::constructFixedLengthGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned char groupID)
{
	switch (groupID)
	{
	case WP6_TOP_EXTENDED_CHARACTER:
		return new WP6ExtendedCharacterGroup(input, encryption, groupID);

	case WP6_TOP_UNDO_GROUP:
		return new WP6UndoGroup(input, encryption, groupID);

	case WP6_TOP_ATTRIBUTE_ON:
		return new WP6AttributeOnGroup(input, encryption, groupID);

	case WP6_TOP_ATTRIBUTE_OFF:
		return new WP6AttributeOffGroup(input, encryption, groupID);

	case WP6_TOP_HIGHLIGHT_ON:
		return new WP6HighlightOnGroup(input, encryption, groupID);

	case WP6_TOP_HIGHLIGHT_OFF:
		return new WP6HighlightOffGroup(input, encryption, groupID);

	// Add the remaining cases here
	default:
		return new WP6UnsupportedFixedLengthGroup(input, encryption, groupID);
	}
}

bool WP6FixedLengthGroup::isGroupConsistent(librevenge::RVNGInputStream *input, WPXEncryption *encryption, const unsigned char groupID)
{
	if (groupID == (unsigned char)0xFF)
		return false;

	long startPosition = input->tell();

	try
	{
		unsigned size = WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[(unsigned char)groupID-0xF0];
		if (input->seek((startPosition + size - 2), librevenge::RVNG_SEEK_SET) || input->isEnd())
		{
			input->seek(startPosition, librevenge::RVNG_SEEK_SET);
			return false;
		}
		if (input->isEnd() || groupID != readU8(input, encryption))
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

void WP6FixedLengthGroup::_read(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();
	_readContents(input, encryption);

	if (m_group >= 0xF0 && m_group < 0xFF) // just an extra safety check
	{
		int size = WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[m_group-0xF0];
		input->seek((startPosition + size - 2), librevenge::RVNG_SEEK_SET);
		if (m_group != readU8(input, encryption))
		{
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected: bailing out!\n"));
			throw FileException();
		}
	}
	else
		throw FileException();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
