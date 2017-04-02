/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5LISTENER_H
#define WP5LISTENER_H

#include <vector>
#include <librevenge/librevenge.h>
#include "libwpd_internal.h"

class WP5SubDocument;
class WP5PrefixData;
class WP5GeneralPacketData;

class WP5Listener
{
public:
	WP5Listener();
	virtual ~WP5Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void setFont(const librevenge::RVNGString &fontName, double fontSize) = 0;
	virtual void setTabs(const std::vector<WPXTabStop> &tabStops, unsigned short tabOffset) = 0;
	virtual void insertCharacter(unsigned character) = 0;
	virtual void insertTab(unsigned char tabType, double tabPosition) = 0;
	virtual void insertIndent(unsigned char indentType, double indentPosition) = 0;
	virtual void insertEOL() = 0;
	virtual void insertBreak(unsigned char breakType) = 0;
	virtual void lineSpacingChange(double lineSpacing) = 0;
	virtual void justificationChange(unsigned char justification) = 0;
	virtual void characterColorChange(unsigned char red, unsigned char green, unsigned char blue) = 0;
	virtual void attributeChange(bool isOn, unsigned char attribute) = 0;
	virtual void pageMarginChange(unsigned char side, unsigned short margin) = 0;
	virtual void pageFormChange(unsigned short length, unsigned short width, WPXFormOrientation orientation) = 0;
	virtual void marginChange(unsigned char side, unsigned short margin) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;

	virtual void defineTable(unsigned char position, unsigned short leftOffset) = 0;
	virtual void addTableColumnDefinition(unsigned width, unsigned leftGutter, unsigned rightGutter,
	                                      unsigned attributes, unsigned char alignment) = 0;
	virtual void startTable() = 0;
	virtual void insertRow(unsigned short rowHeight, bool isMinimumHeight, bool isHeaderRow) = 0;
	virtual void insertCell(unsigned char colSpan, unsigned char rowSpan, unsigned char borderBits,
	                        const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                        const RGBSColor *cellBorderColor, WPXVerticalAlignment cellVerticalAlignment,
	                        bool useCellAttributes, unsigned cellAttributes) = 0;
	virtual void endTable() = 0;

	virtual void insertNoteReference(const librevenge::RVNGString &noteReference) = 0;
	virtual void insertNote(WPXNoteType noteType, const WP5SubDocument *subDocument) = 0;
	virtual void headerFooterGroup(unsigned char headerFooterType, unsigned char occurrenceBits, WP5SubDocument *subDocument) = 0;
	virtual void suppressPageCharacteristics(unsigned char suppressCode) = 0;

	virtual void boxOn(unsigned char positionAndType, unsigned char alignment, unsigned short width, unsigned short height, unsigned short x, unsigned short y) = 0;
	virtual void boxOff() = 0;
	virtual void insertGraphicsData(const librevenge::RVNGBinaryData *data) = 0;

	void setPrefixData(WP5PrefixData *prefixData)
	{
		m_prefixData = prefixData;
	}
	const WP5GeneralPacketData *getGeneralPacketData(const int type) const;

private:
	WP5Listener(const WP5Listener &);
	WP5Listener &operator=(const WP5Listener &);
	WP5PrefixData *m_prefixData;
};

#endif /* WP5LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
