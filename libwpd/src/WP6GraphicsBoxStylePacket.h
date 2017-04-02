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

#ifndef WP6GRAPHICSBOXSTYLEPACKET_H
#define WP6GRAPHICSBOXSTYLEPACKET_H

#include <libwpd/libwpd.h>
#include <librevenge-stream/librevenge-stream.h>
#include "WP6PrefixDataPacket.h"
#include "WP6Listener.h"

class WP6GraphicsBoxStylePacket : public WP6PrefixDataPacket
{
public:
	WP6GraphicsBoxStylePacket(librevenge::RVNGInputStream *input, WPXEncryption *encryption, int id, unsigned dataOffset, unsigned dataSize);
	~WP6GraphicsBoxStylePacket();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener * /*listener*/) const {}

	unsigned char getGeneralPositioningFlags() const
	{
		return m_generalPositioningFlags;
	}
	unsigned char getHorizontalPositioningFlags() const
	{
		return m_horizontalPositioningFlags;
	}
	signed short getHorizontalOffset() const
	{
		return m_horizontalOffset;
	}
	unsigned char getLeftColumn() const
	{
		return m_leftColumn;
	}
	unsigned char getRightColumn() const
	{
		return m_rightColumn;
	}
	unsigned char getVerticalPositioningFlags() const
	{
		return m_verticalPositioningFlags;
	}
	signed short getVerticalOffset() const
	{
		return m_verticalOffset;
	}
	unsigned char getWidthFlags() const
	{
		return m_widthFlags;
	}
	unsigned short getWidth() const
	{
		return m_width;
	}
	unsigned char getHeightFlags() const
	{
		return m_heightFlags;
	}
	unsigned short getHeight() const
	{
		return m_height;
	}
	unsigned char getContentType() const
	{
		return m_contentType;
	}
	unsigned short getNativeWidth() const
	{
		return m_nativeWidth;
	}
	unsigned short getNativeHeight() const
	{
		return m_nativeHeight;
	}

private:
	WP6GraphicsBoxStylePacket(const WP6GraphicsBoxStylePacket &);
	WP6GraphicsBoxStylePacket &operator=(const WP6GraphicsBoxStylePacket &);
	bool m_isLibraryStyle;
	librevenge::RVNGString m_boxStyleName;
	unsigned char m_generalPositioningFlags;
	unsigned char m_horizontalPositioningFlags;
	signed short m_horizontalOffset;
	unsigned char m_leftColumn, m_rightColumn;
	unsigned char m_verticalPositioningFlags;
	signed short m_verticalOffset;
	unsigned char m_widthFlags;
	unsigned short m_width;
	unsigned char m_heightFlags;
	unsigned short m_height;
	unsigned char m_contentType, m_contentHAlign, m_contentVAlign;
	bool m_contentPreserveAspectRatio;
	unsigned short m_nativeWidth, m_nativeHeight;
};
#endif /* WP6GRAPHICSBOXSTYLEPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
