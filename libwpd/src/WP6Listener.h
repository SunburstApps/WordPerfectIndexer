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
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6LISTENER_H
#define WP6LISTENER_H

#include <librevenge/librevenge.h>
#include "libwpd_internal.h"
#include <vector>

class WP6DefaultInitialFontPacket;
class WP6PrefixData;
class WP6PrefixDataPacket;
class WP6SubDocument;

class WP6Listener
{
public:
	WP6Listener();
	virtual ~WP6Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void setDate(const unsigned short type, const unsigned short year,
	                     const unsigned char month, const unsigned char day,
	                     const unsigned char hour, const unsigned char minute,
	                     const unsigned char second, const unsigned char dayOfWeek,
	                     const unsigned char timeZone, const unsigned char unused) = 0;
	virtual void setExtendedInformation(const unsigned short type, const librevenge::RVNGString &data) = 0;
	virtual void setAlignmentCharacter(const unsigned character) = 0;
	virtual void setLeaderCharacter(const unsigned character, const unsigned char numSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops,
	                            const std::vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(unsigned character) = 0;
	virtual void insertTab(const unsigned char tabType, double tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void insertBreak(const unsigned char breakType) = 0;
	virtual void lineSpacingChange(const double lineSpacing) = 0;
	virtual void justificationChange(const unsigned char justification) = 0;
	virtual void characterColorChange(const unsigned char red, const unsigned char green, const unsigned char blue) = 0;
	virtual void characterShadingChange(const unsigned char shading) = 0;
	virtual void highlightChange(const bool isOn, const RGBSColor &color) = 0;
	virtual void fontChange(const unsigned short matchedFontPointSize, const unsigned short fontPID, const librevenge::RVNGString &fontName) = 0;
	virtual void attributeChange(const bool isOn, const unsigned char attribute) = 0;
	virtual void spacingAfterParagraphChange(const double spacingRelative, const double spacingAbsolute) = 0;
	virtual void pageMarginChange(const unsigned char side, const unsigned short margin) = 0;
	virtual void pageFormChange(const unsigned short length, const unsigned short width, const WPXFormOrientation orientation) = 0;
	virtual void pageNumberingChange(const WPXPageNumberPosition pageNumberPosition, const unsigned short pageNumberFontPointSize, const unsigned short pageNumberFontPID) = 0;
	virtual void marginChange(const unsigned char side, const unsigned short margin) = 0;
	virtual void paragraphMarginChange(const unsigned char side, const signed short margin) = 0;
	virtual void indentFirstLineChange(const signed short offset) = 0;
	virtual void columnChange(const WPXTextColumnType columnType, const unsigned char numColumns,
	                          const std::vector<double> &columnWidth, const std::vector<bool> &isFixedWidth) = 0;
	virtual void updateOutlineDefinition(const unsigned short outlineHash, const unsigned char *numberingMethods, const unsigned char tabBehaviourFlag) = 0;
	virtual void paragraphNumberOn(const unsigned short outlineHash, const unsigned char level, const unsigned char flag) = 0;
	virtual void paragraphNumberOff() = 0;
	virtual void displayNumberReferenceGroupOn(const unsigned char subGroup, const unsigned char level) = 0;
	virtual void displayNumberReferenceGroupOff(const unsigned char subGroup) = 0;
	virtual void styleGroupOn(const unsigned char subGroup) = 0;
	virtual void styleGroupOff(const unsigned char subGroup) = 0;
	virtual void globalOn(const unsigned char systemStyle) = 0;
	virtual void globalOff() = 0;
	virtual void noteOn(const unsigned short textPID) = 0;
	virtual void noteOff(const WPXNoteType noteType) = 0;
	virtual void headerFooterGroup(const unsigned char headerFooterType, const unsigned char occurrenceBits, const unsigned short textPID) = 0;
	virtual void suppressPageCharacteristics(const unsigned char suppressCode) = 0;
	virtual void setPageNumber(const unsigned short pageNumber) = 0;
	virtual void setPageNumberingType(const WPXNumberingType pageNumberingType) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;
	virtual void defineTable(const unsigned char position, const unsigned short leftOffset) = 0;
	virtual void addTableColumnDefinition(const unsigned width, const unsigned leftGutter, const unsigned rightGutter,
	                                      const unsigned attributes, const unsigned char alignment) = 0;
	virtual void startTable() = 0;
	virtual void insertRow(const unsigned short rowHeight, const bool isMinimumHeight, const bool isHeaderRow) = 0;
	virtual void insertCell(const unsigned char colSpan, const unsigned char rowSpan, const unsigned char borderBits,
	                        const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                        const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment,
	                        const bool useCellAttributes, const unsigned cellAttributes) = 0;
	virtual void endTable() = 0;

	virtual void undoChange(const unsigned char undoType, const unsigned short undoLevel) = 0;
	virtual void boxOn(const unsigned char anchoringType, const unsigned char generalPositioningFlags, const unsigned char horizontalPositioningFlags,
	                   const signed short horizontalOffset, const unsigned char leftColumn, const unsigned char rightColumn,
	                   const unsigned char verticalPositioningFlags, const signed short verticalOffset, const unsigned char widthFlags, const unsigned short width,
	                   const unsigned char heightFlags, const unsigned short height, const unsigned char boxContentType, const unsigned short nativeWidth,
	                   const unsigned short nativeHeight,
	                   const librevenge::RVNGString &linkTarget) = 0;
	virtual void boxOff() = 0;
	virtual void insertGraphicsData(const unsigned short packetId) = 0;
	virtual void insertTextBox(const WP6SubDocument *subDocument) = 0;
	virtual void commentAnnotation(const unsigned short textPID) = 0;

	void setPrefixData(WP6PrefixData *prefixData)
	{
		m_prefixData = prefixData;
	}
	const WP6PrefixDataPacket *getPrefixDataPacket(const int prefixID) const;
	librevenge::RVNGString getFontNameForPID(const int prefixID) const;

private:
	WP6Listener(const WP6Listener &);
	WP6Listener &operator=(const WP6Listener &);
	WP6PrefixData *m_prefixData;
};

#endif /* WP6LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
