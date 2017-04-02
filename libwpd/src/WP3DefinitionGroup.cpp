/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <math.h>
#include "WP3DefinitionGroup.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"
#include "WP3Listener.h"

WP3DefinitionGroup::WP3DefinitionGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_colType(0),
	m_numColumns(0),
	m_isFixedWidth(),
	m_columnWidth()
{
	_read(input, encryption);
}

WP3DefinitionGroup::~WP3DefinitionGroup()
{
	// fixme delete the font name
}

void WP3DefinitionGroup::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_DEFINITION_GROUP_SET_COLUMNS:
	{
		unsigned char tmpColType = readU8(input, encryption);
		if (tmpColType)
		{
			unsigned char tmpNumColumns = readU8(input, encryption);
			if (tmpNumColumns)
				input->seek(((2*tmpNumColumns) - 1), librevenge::RVNG_SEEK_CUR);
		}

		m_colType = readU8(input, encryption);
		if (!m_colType)
		{
			m_numColumns = 1;
			m_isFixedWidth.clear();
			m_columnWidth.clear();
		}
		else
		{
			m_numColumns = readU8(input, encryption);
			if (m_numColumns > 1)
			{
				for (int i=0; i<((2*m_numColumns)-1); i++)
				{
					if (i%2)
					{
						unsigned tmpSpaceBetweenColumns = readU32(input, encryption, true);
						m_isFixedWidth.push_back(true);
						m_columnWidth.push_back((double)((double)fixedPointToWPUs(tmpSpaceBetweenColumns)/(double)WPX_NUM_WPUS_PER_INCH));
					}
					else
					{
						unsigned short tmpSizeOfColumn = readU16(input, encryption, true);
						m_isFixedWidth.push_back(false);
						m_columnWidth.push_back((double)((double)tmpSizeOfColumn/(double)0x10000));
					}
				}
			}
		}
	}
	break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3DefinitionGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Definition group\n"));

	switch (getSubGroup())
	{
	case WP3_DEFINITION_GROUP_SET_COLUMNS:
		// number of columns = {0,1} means columns off
		if ((m_numColumns <= 1))
		{
			listener->columnChange(NEWSPAPER, 1, m_columnWidth, m_isFixedWidth); // the value "1" is bugus, the false bool gives you all the information you need here
		}
		else
		{
			switch (m_colType)
			{
			case WP3_COLUMN_TYPE_NEWSPAPER:
				listener->columnChange(NEWSPAPER, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			case WP3_COLUMN_TYPE_PARALLEL:
				listener->columnChange(PARALLEL, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			case WP3_COLUMN_TYPE_EXTENDED:
				listener->columnChange(PARALLEL_PROTECT, m_numColumns, m_columnWidth, m_isFixedWidth);
				break;
			default: // something else we don't support, since it isn't in the docs
				break;
			}
		}
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
