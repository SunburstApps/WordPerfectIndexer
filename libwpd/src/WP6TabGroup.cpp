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
 * Copyright (C) 2004-2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP6TabGroup.h"
#include "libwpd_internal.h"
#include "WPXFileStructure.h"
#include "WP6Listener.h"

WP6TabGroup::WP6TabGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_position(0.0),
	m_ignoreFunction(false)
{
	_read(input, encryption);
}

void WP6TabGroup::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	unsigned short tempPosition = 0xFFFF;
	if ((getFlags() & 0x40) == 0x40) // 0x40 is "ignore function" flag
	{
		m_ignoreFunction = true;
	}

	/* What follows is a result of reverse-engineering of undocumented information contained in the functions of the
	 * Tab Group. It is result of adjustment by trials and errors and can be wrong in certain cases */

	if ((getSubGroup() & 0xC0) == 0x00)
		/* Left aligned tabs contain  the position of the tab as a word (unsigned short) in WPUs
		 * from left edge of the paper just after the size of "non-deletable" */
	{
		tempPosition = readU16(input, encryption);
	}
	else if ((getSize() >= 12) && (getSize() <= 18)) // Minimum size of the function if the position information is present
		/* This case might be fully included in the previous condition, but I am not sure;
		 * so leaving it in for the while */
	{
		input->seek((getSize() - 12), librevenge::RVNG_SEEK_CUR);
		tempPosition = readU16(input, encryption);
	}
	else if (getSize() > 18)
	{
		input->seek(6, librevenge::RVNG_SEEK_CUR);
		tempPosition = readU16(input, encryption);
	}
	// If we got a tempPosition of 0, it means, the information in WPUs is not there (WP6 for DOS??).
	// We will have to dig a bit more to see whether we can get the information from the screen units ???
	if (!tempPosition)
		tempPosition = 0xFFFF;
	m_position = (double)((double)tempPosition/(double)WPX_NUM_WPUS_PER_INCH);
}

void WP6TabGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Tab group (Tab type: %i, Tab position: %.4finch, Ignore function: %s)\n",
	               getSubGroup(), m_position, (m_ignoreFunction?"true":"false")));
	if (!m_ignoreFunction)
	{
		WPD_DEBUG_MSG(("WordPerfect: Parsing a Tab group\n"));
		listener->insertTab(getSubGroup(), m_position);
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
