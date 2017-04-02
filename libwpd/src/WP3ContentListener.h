/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3CONTENTLISTENER_H
#define WP3CONTENTLISTENER_H

#include "WP3Listener.h"
#include "WPXContentListener.h"
#include "WP3SubDocument.h"

struct WP3ContentParsingState
{
	WP3ContentParsingState();
	~WP3ContentParsingState();
	unsigned short m_colSpan;
	unsigned short m_rowSpan;
	librevenge::RVNGString m_textBuffer;
	RGBSColor *m_cellFillColor;
	librevenge::RVNGString m_noteReference;

	WPXTableList m_tableList;
private:
	WP3ContentParsingState(const WP3ContentParsingState &);
	WP3ContentParsingState &operator=(const WP3ContentParsingState &);
};

class WP3ContentListener : public WP3Listener, protected WPXContentListener
{
public:
	WP3ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP3SubDocument *> &subDocuments, librevenge::RVNGTextInterface *documentInterface);
	~WP3ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void insertCharacter(unsigned character);
	void insertTab();
	void insertTab(unsigned char tabType, double tabPosition);
	void insertBreak(unsigned char breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void insertEOL();
	void attributeChange(bool isOn, unsigned char attribute);
	void lineSpacingChange(double lineSpacing)
	{
		WPXContentListener::lineSpacingChange(lineSpacing);
	}
	void pageMarginChange(unsigned char /* side */, unsigned short /* margin */) {}
	void pageFormChange(unsigned short /* length */, unsigned short /* width */, WPXFormOrientation /* orientation */) {}
	void marginChange(unsigned char side, unsigned short margin);
	void indentFirstLineChange(double offset);
	void setTabs(bool isRelative, const std::vector<WPXTabStop> tabStops);
	void columnChange(WPXTextColumnType columnType, unsigned char numColumns, const std::vector<double> &columnWidth,
	                  const std::vector<bool> &isFixedWidth);
	void endDocument()
	{
		WPXContentListener::endDocument();
	}
	void endSubDocument()
	{
		WPXContentListener::endSubDocument();
	}

	void defineTable(unsigned char position, unsigned short leftOffset);
	void addTableColumnDefinition(unsigned width, unsigned leftGutter, unsigned rightGutter,
	                              unsigned attributes, unsigned char alignment);
	void startTable();
	void insertRow();
	void insertCell();
	void closeCell();
	void closeRow();
	void setTableCellSpan(unsigned short colSpan, unsigned short rowSpan);
	void setTableCellFillColor(const RGBSColor *cellFillColor);
	void endTable();
	void undoChange(unsigned char undoType, unsigned short undoLevel);
	void justificationChange(unsigned char justification);
	void setTextColor(const RGBSColor *fontColor);
	void setTextFont(const librevenge::RVNGString &fontName);
	void setFontSize(unsigned short fontSize);
	void insertPageNumber(const librevenge::RVNGString &pageNumber);
	void insertNoteReference(const librevenge::RVNGString &noteReference);
	void insertNote(WPXNoteType noteType, const WP3SubDocument *subDocument);
	void headerFooterGroup(unsigned char headerFooterType, unsigned char occurrenceBits, WP3SubDocument *subDocument);
	void suppressPage(unsigned short /* suppressCode */) {}
	void backTab();
	void leftIndent();
	void leftIndent(double offset);
	void leftRightIndent();
	void leftRightIndent(double offset);
	void insertPicture(double height, double width, double verticalOffset, double horizontalOffset, unsigned char leftColumn, unsigned char rightColumn,
	                   unsigned short figureFlags, const librevenge::RVNGBinaryData &binaryData);
	void insertTextBox(double height, double width, double verticalOffset, double horizontalOffset, unsigned char leftColumn, unsigned char rightColumn,
	                   unsigned short figureFlags, const WP3SubDocument *subDocument, const WP3SubDocument *caption);
	void insertWP51Table(double height, double width, double verticalOffset, double horizontalOffset, unsigned char leftColumn, unsigned char rightColumn,
	                     unsigned short figureFlags, const WP3SubDocument *subDocument, const WP3SubDocument *caption);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);
	void _openParagraph();

	void _flushText();
	void _changeList() {}

	void _handleFrameParameters(librevenge::RVNGPropertyList &propList, double height, double width, double verticalOffset, double horizontalOffset, unsigned char leftColumn, unsigned char rightColumn,
	                            unsigned short figureFlags);

private:
	WP3ContentListener(const WP3ContentListener &);
	WP3ContentListener &operator=(const WP3ContentListener &);
	WP3ContentParsingState *m_parseState;
	std::vector<WP3SubDocument *> &m_subDocuments;
};

#endif /* WP3CONTENTLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
