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

#ifndef WP6PAGEGROUP_H
#define WP6PAGEGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6PageGroup : public WP6VariableLengthGroup
{
public:
	WP6PageGroup(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	~WP6PageGroup();
	void _readContents(librevenge::RVNGInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	// variables needed for subgroup 0 and 1 (Left/Right Margin Set)
	unsigned short m_margin;
	// variables needed for subgroup 2 (suppress page characteristics)
	unsigned char m_suppressedCode;
	// variables needed for subgroup 3 (page number position)
	unsigned short m_pageNumberTypefaceDesc;
	unsigned char m_pageNumberUseFlag;
	unsigned short m_pageNumberingFontPIDCopy;
	unsigned short m_pageNumberPointSize;
	unsigned char m_pageNumberPosition;
	unsigned short m_pageNumberMatchedFontIndex;
	unsigned short m_pageNumberMatchedFontPointSize;
	unsigned short m_pageNumberAttributes1;
	unsigned short m_pageNumberAttributes2;
	RGBSColor m_pageNumberColor;
	unsigned short m_pageNumberHeight;
	unsigned char m_pageNumberNewPagePosition;

	// variables needed for subgroup 0x11 (Form)
	unsigned short m_formLength;
	unsigned short m_formWidth;
	unsigned char m_formType;
	WPXFormOrientation m_formOrientation;

};

#endif /* WP6PAGEGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
