/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5BOXGROUP_H
#define WP5BOXGROUP_H

#include "WP5VariableLengthGroup.h"
#include <libwpd/libwpd.h>

class WP5BoxGroup : public WP5VariableLengthGroup
{
public:
	WP5BoxGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP5Listener *listener);

protected:
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

private:
	unsigned short m_boxNumber;
	unsigned char m_positionAndType, m_alignment;
	unsigned short m_width, m_height, m_x, m_y;
	unsigned char m_boxType;
	unsigned short m_graphicsOffset;
	const librevenge::RVNGBinaryData *m_data;
	// Unimplemented to prevent compiler from creating crasher ones
	WP5BoxGroup(const WP5BoxGroup &);
	WP5BoxGroup &operator=(const WP5BoxGroup &);
};

#endif /* WP5BOXGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
