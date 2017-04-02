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

#include "WP6Listener.h"
#include "WP6DefaultInitialFontPacket.h"
#include "libwpd_internal.h"

WP6DefaultInitialFontPacket::WP6DefaultInitialFontPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int /* id */, unsigned dataOffset, unsigned dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_numPrefixIDs(0),
	m_initialFontDescriptorPID(0),
	m_pointSize(0)
{
	_read(input, encryption, dataOffset, dataSize);
}

void WP6DefaultInitialFontPacket::parse(WP6Listener *listener) const
{
	listener->fontChange(getPointSize(), getInitialFontDescriptorPID(), librevenge::RVNGString());
}

void WP6DefaultInitialFontPacket::_readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption)
{
	m_numPrefixIDs = readU16(input, encryption);
	m_initialFontDescriptorPID = readU16(input, encryption);
	m_pointSize = readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Read default initial font packet (initial font descriptor pid: %i, point size: %i)\n",
	               (int) m_initialFontDescriptorPID, (int) m_pointSize));
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
