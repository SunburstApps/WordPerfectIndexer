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
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5STYLESLISTENER_H
#define WP5STYLESLISTENER_H

#include "WP5Listener.h"
#include "WPXStylesListener.h"
#include <vector>
#include <list>
#include "WPXPageSpan.h"
#include "WPXTable.h"
#include "WP5SubDocument.h"

class WP5StylesListener : public WP5Listener, protected WPXStylesListener
{
public:
	WP5StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, std::vector<WP5SubDocument *> &subDocuments);

	void startDocument() {}
	void startSubDocument() {}
	void setFont(const librevenge::RVNGString & /* fontName */, double /* fontSize */) {}
	void setTabs(const std::vector<WPXTabStop> & /* tabStops */, unsigned short /* tabOffset */) {}
	void insertCharacter(unsigned /* character */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void insertTab(unsigned char /* tabType */, double /* tabPosition */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	virtual void insertIndent(unsigned char /* indentType */, double /* indentPosition */)
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void characterColorChange(unsigned char /* red */, unsigned char /* green */, unsigned char /* blue */) {}
	void insertEOL()
	{
		/*if (!isUndoOn())*/ m_currentPageHasContent = true;
	}
	void insertBreak(unsigned char breakType);
	void attributeChange(bool /* isOn */, unsigned char /* attribute */) {}
	void lineSpacingChange(double /* lineSpacing */) {}
	void justificationChange(unsigned char /* justification */) {}
	void pageMarginChange(unsigned char side, unsigned short margin);
	void pageFormChange(unsigned short length, unsigned short width, WPXFormOrientation orientation);
	void marginChange(unsigned char side, unsigned short margin);
	void endDocument();
	void endSubDocument();

	void defineTable(unsigned char /* position */, unsigned short /* leftOffset */) {}
	void addTableColumnDefinition(unsigned /* width */, unsigned /* leftGutter */, unsigned /* rightGutter */,
	                              unsigned /* attributes */, unsigned char /* alignment */) {}
	void startTable();
	void insertRow(unsigned short rowHeight, bool isMinimumHeight, bool isHeaderRow);
	void insertCell(unsigned char colSpan, unsigned char rowSpan, unsigned char borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, WPXVerticalAlignment cellVerticalAlignment,
	                bool useCellAttributes, unsigned cellAttributes);
	void endTable() {}

	void insertNoteReference(const librevenge::RVNGString & /* noteReference */) {}
	void insertNote(WPXNoteType /* noteType */, const WP5SubDocument * /* subDocument */) {}
	void headerFooterGroup(unsigned char headerFooterType, unsigned char occurrenceBits, WP5SubDocument *subDocument);
	void suppressPageCharacteristics(unsigned char suppressCode);

	void boxOn(unsigned char /* positionAndType */, unsigned char /* alignment */, unsigned short /* width */, unsigned short /* height */, unsigned short /* x */, unsigned short /* y */) {}
	virtual void boxOff() {}
	virtual void insertGraphicsData(const librevenge::RVNGBinaryData * /* data */) {}

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, int nextTableIndice = 0);

private:
	WP5StylesListener(const WP5StylesListener &);
	WP5StylesListener &operator=(const WP5StylesListener &);
	WPXPageSpan m_currentPage, m_nextPage;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	double m_tempMarginLeft, m_tempMarginRight;
	bool m_currentPageHasContent;
	bool m_isSubDocument;
	std::vector<WP5SubDocument *> &m_subDocuments;
	std::list<WPXPageSpan>::iterator m_pageListHardPageMark;
};

#endif /* WP5STYLESLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
