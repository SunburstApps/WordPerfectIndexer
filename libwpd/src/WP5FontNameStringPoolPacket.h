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

#ifndef WP5FONTNAMESTRINGPACKET_H
#define WP5FONTNAMESTRINGPACKET_H

#include <map>
#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>
#include <libwpd/libwpd.h>
#include "WP5GeneralPacketData.h"

class WP5FontNameStringPoolPacket : public WP5GeneralPacketData
{
public:
	WP5FontNameStringPoolPacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int id, unsigned dataOffset, unsigned dataSize);
	~WP5FontNameStringPoolPacket();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned dataSize);
	librevenge::RVNGString getFontName(const unsigned int offset) const;

private:
	std::map<unsigned int, librevenge::RVNGString> m_fontNameString;
};
#endif /* WP5FONTNAMESTRINGPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
