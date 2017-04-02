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

#ifndef WP5CONTENTLISTENER_H
#define WP5CONTENTLISTENER_H

#include "WP5Listener.h"
#include "WPXContentListener.h"
#include "WP5SubDocument.h"
#include "libwpd_internal.h"
#include <vector>

struct WP5ContentParsingState
{
	WP5ContentParsingState();
	~WP5ContentParsingState();
	librevenge::RVNGString m_textBuffer;
	librevenge::RVNGString m_noteReference;

	WPXTableList m_tableList;

	bool m_isFrameOpened;
};

class WP5ContentListener : public WP5Listener, protected WPXContentListener
{
public:
	WP5ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP5SubDocument *> &subDocuments, librevenge::RVNGTextInterface *documentInterface);
	~WP5ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void setFont(const librevenge::RVNGString &fontName, double fontSize);
	void setTabs(const std::vector<WPXTabStop> &tabStops, unsigned short tabOffset);
	void insertCharacter(unsigned character);
	void insertTab(unsigned char tabType, double tabPosition);
	void insertIndent(unsigned char indentType, double indentPosition);
	void insertEOL();
	void insertBreak(unsigned char breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void lineSpacingChange(double lineSpacing)
	{
		WPXContentListener::lineSpacingChange(lineSpacing);
	}
	void justificationChange(unsigned char justification)
	{
		WPXContentListener::justificationChange(justification);
	}
	void characterColorChange(unsigned char red, unsigned char green, unsigned char blue);
	void attributeChange(bool isOn, unsigned char attribute);
	void pageMarginChange(unsigned char /* side */, unsigned short /* margin */) {}
	void pageFormChange(unsigned short /* length */, unsigned short /* width */, WPXFormOrientation /* orientation */) {}
	void marginChange(unsigned char side, unsigned short margin);
	void paragraphMarginChange(unsigned char /* side */, signed short /* margin */) {}
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
	void insertRow(unsigned short rowHeight, bool isMinimumHeight, bool isHeaderRow);
	void insertCell(unsigned char colSpan, unsigned char rowSpan, unsigned char borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, WPXVerticalAlignment cellVerticalAlignment,
	                bool useCellAttributes, unsigned cellAttributes);
	void endTable();

	void insertNoteReference(const librevenge::RVNGString &noteReference);
	void insertNote(WPXNoteType noteType, const WP5SubDocument *subDocument);
	void headerFooterGroup(unsigned char headerFooterType, unsigned char occurrenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(unsigned char /* suppressCode */) {}

	void setDefaultFont(const librevenge::RVNGString &fontName, double fontSize);

	void boxOn(unsigned char positionAndType, unsigned char alignment, unsigned short width, unsigned short height, unsigned short x, unsigned short y);
	virtual void boxOff();
	virtual void insertGraphicsData(const librevenge::RVNGBinaryData *data);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);

	void _flushText();
	void _changeList() {}

private:
	WP5ContentListener(const WP5ContentListener &);
	WP5ContentListener &operator=(const WP5ContentListener &);
	WP5ContentParsingState *m_parseState;
	std::vector<WP5SubDocument *> &m_subDocuments;
	double m_defaultFontSize;
	librevenge::RVNGString m_defaultFontName;
};

#endif /* WP5CONTENTLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
