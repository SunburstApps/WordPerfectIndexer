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
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
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

#ifndef WPXHEADER_H
#define WPXHEADER_H

#include <libwpd/libwpd.h>
#include <librevenge-stream/librevenge-stream.h>

class WPXEncryption;

class WPXHeader
{
public:
	WPXHeader(librevenge::RVNGInputStream *input, WPXEncryption *encryption, unsigned documentOffset, unsigned char productType,
	          unsigned char fileType, unsigned char majorVersion, unsigned char minorVersion, unsigned short documentEncryption);
	virtual ~WPXHeader();

	static WPXHeader *constructHeader(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

	unsigned getDocumentOffset() const
	{
		return m_documentOffset;
	}
	unsigned char getProductType() const
	{
		return m_productType;
	}
	unsigned char getFileType() const
	{
		return m_fileType;
	}
	unsigned char getMajorVersion() const
	{
		return m_majorVersion;
	}
	unsigned char getMinorVersion() const
	{
		return m_minorVersion;
	}
	unsigned short getDocumentEncryption() const
	{
		return m_documentEncryption;
	}

private:
	unsigned m_documentOffset;
	unsigned char m_productType;
	unsigned char m_fileType;
	unsigned char m_majorVersion;
	unsigned char m_minorVersion;
	unsigned short m_documentEncryption;
};

#endif /* WPXHEADER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
