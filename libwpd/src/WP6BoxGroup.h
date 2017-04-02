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

#ifndef WP6BOXGROUP_H
#define WP6BOXGROUP_H

#include <librevenge/librevenge.h>
#include "WP6VariableLengthGroup.h"
#include "libwpd_internal.h"

class WP6Listener;

class WP6BoxGroup : public WP6VariableLengthGroup
{
public:
	WP6BoxGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	unsigned char m_generalPositioningFlagsMask, m_generalPositioningFlagsData;
	bool m_hasHorizontalPositioning;
	unsigned char m_horizontalPositioningFlags;
	signed short m_horizontalOffset;
	unsigned char m_leftColumn, m_rightColumn;
	bool m_hasVerticalPositioning;
	unsigned char m_verticalPositioningFlags;
	signed short m_verticalOffset;
	bool m_hasWidthInformation;
	unsigned char m_widthFlags;
	unsigned short m_width;
	bool m_hasHeightInformation;
	unsigned char m_heightFlags;
	unsigned short m_height;
	bool m_hasZOrderInformation;
	unsigned char m_zOrderFlags;
	bool m_hasBoxContentType;
	unsigned char m_boxContentType;
	unsigned short m_nativeWidth, m_nativeHeight;
};

#endif /* WP6BOXGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
