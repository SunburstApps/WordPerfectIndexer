/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2015 David Tardon (dtardon@redhat.com)
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

#include "WP6HyperlinkPacket.h"

#include <string>

#include "libwpd_internal.h"

WP6HyperlinkPacket::WP6HyperlinkPacket(librevenge::RVNGInputStream *const input, WPXEncryption *const encryption, int, const unsigned dataOffset, const unsigned dataSize)
	: WP6PrefixDataPacket(input, encryption)
	, m_target()
{
	_read(input, encryption, dataOffset, dataSize);
}

void WP6HyperlinkPacket::_readContents(librevenge::RVNGInputStream *const input, WPXEncryption *const encryption)
{
	if (getDataSize() % 2 != 0)
	{
		WPD_DEBUG_MSG(("WP6HyperlinkPacket::_readContents: size of the UTF-16 string is odd"));
	}
	// read all chars except the trailing \0
	for (unsigned pos = 0; pos + 2 < getDataSize(); pos += 2)
	{
		const unsigned unichar = readU16(input, encryption);
		appendUCS4(m_target, unichar);
	}
}

const librevenge::RVNGString &WP6HyperlinkPacket::getTarget() const
{
	return m_target;
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
