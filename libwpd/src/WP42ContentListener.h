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

#ifndef WP42CONTENTLISTENER_H
#define WP42CONTENTLISTENER_H

#include "WP42Listener.h"
#include "WP42SubDocument.h"
#include "WPXContentListener.h"
#include <libwpd/libwpd.h>

struct WP42ContentParsingState
{
	WP42ContentParsingState();
	~WP42ContentParsingState();
	librevenge::RVNGString m_textBuffer;
};

class WP42ContentListener : public WP42Listener, protected WPXContentListener
{
public:
	WP42ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP42SubDocument *> &subDocuments, librevenge::RVNGTextInterface *documentInterface);
	~WP42ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void insertCharacter(unsigned character);
	void insertTab(unsigned char tabType, double tabPosition);
	void insertBreak(unsigned char breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void insertEOL();
	void attributeChange(bool isOn, unsigned char attribute);
	void marginReset(unsigned char leftMargin, unsigned char rightMargin);
	void headerFooterGroup(unsigned char headerFooterDefinition, WP42SubDocument *subDocument);
	void suppressPageCharacteristics(unsigned char /* suppressCode */) {}
	void endDocument()
	{
		WPXContentListener::endDocument();
	}
	void endSubDocument()
	{
		WPXContentListener::endSubDocument();
	}

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);

	void _flushText();
	void _changeList() {}

private:
	WP42ContentListener(const WP42ContentListener &);
	WP42ContentListener &operator=(const WP42ContentListener &);
	WP42ContentParsingState *m_parseState;
	std::vector<WP42SubDocument *> &m_subDocuments;
};

#endif /* WP42LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
