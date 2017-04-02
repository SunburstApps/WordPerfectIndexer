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

#ifndef WP1CONTENTLISTENER_H
#define WP1CONTENTLISTENER_H

#include "WP1Listener.h"
#include "WPXContentListener.h"

class WP1SubDocument;

struct WP1ContentParsingState
{
	WP1ContentParsingState();
	~WP1ContentParsingState();
	librevenge::RVNGString m_textBuffer;
	int m_numDeferredTabs;
	int m_footNoteNumber, m_endNoteNumber;
private:
	WP1ContentParsingState(const WP1ContentParsingState &);
	WP1ContentParsingState &operator=(const WP1ContentParsingState &);
};

class WP1ContentListener : public WP1Listener, protected WPXContentListener
{
public:
	WP1ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments, librevenge::RVNGTextInterface *documentInterface);
	~WP1ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void insertCharacter(unsigned character);
	void insertExtendedCharacter(unsigned char extendedCharacter);
	void insertTab();
	void insertBreak(unsigned char breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void insertEOL();
	void insertNote(WPXNoteType noteType, WP1SubDocument *subDocument);
	void attributeChange(bool isOn, unsigned char attribute);
	void fontPointSize(unsigned char pointSize);
	void fontId(unsigned short id);
	void marginReset(unsigned short leftMargin, unsigned short rightMargin);
	void topMarginSet(unsigned short /* topMargin */) {}
	void bottomMarginSet(unsigned short /* bottomMargin */) {}
	void leftIndent(unsigned short leftMarginOffset);
	void leftRightIndent(unsigned short leftRightMarginOffset);
	void leftMarginRelease(unsigned short release);
	void setTabs(const std::vector<WPXTabStop> &tabStops);
	void headerFooterGroup(unsigned char headerFooterDefinition, WP1SubDocument *subDocument);
	void suppressPageCharacteristics(unsigned char /* suppressCode */) {}
	void justificationChange(unsigned char justification);
	void lineSpacingChange(unsigned char spacing)
	{
		WPXContentListener::lineSpacingChange((double)((double)spacing/2.0));
	}
	void flushRightOn();
	void flushRightOff() {}
	void centerOn();
	void centerOff() {}
	void endDocument()
	{
		WPXContentListener::endDocument();
	}
	void endSubDocument()
	{
		WPXContentListener::endSubDocument();
	}
	void insertPicture(unsigned short width, unsigned short height, const librevenge::RVNGBinaryData &binaryData);

protected:
	using WPXContentListener::lineSpacingChange;
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);

	void _flushText();
	void _changeList() {}

private:
	WP1ContentParsingState *m_parseState;
	std::vector<WP1SubDocument *> &m_subDocuments;
	WP1ContentListener(const WP1ContentListener &);
	WP1ContentListener &operator=(WP1ContentListener &);
};

#endif /* WP1LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
