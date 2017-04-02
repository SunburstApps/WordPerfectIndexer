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

#ifndef WP1LISTENER_H
#define WP1LISTENER_H

#include <vector>
#include <librevenge/librevenge.h>
#include "libwpd_internal.h"

class WP1SubDocument;

class WP1Listener
{
public:
	WP1Listener();
	virtual ~WP1Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void insertCharacter(unsigned character) = 0;
	virtual void insertExtendedCharacter(unsigned char extendedCharacter) = 0;
	virtual void insertTab() = 0;
	virtual void insertBreak(unsigned char breakType) = 0;
	virtual void insertEOL() = 0;
	virtual void insertNote(WPXNoteType noteType, WP1SubDocument *subDocument) = 0;
	virtual void attributeChange(bool isOn, unsigned char attribute) = 0;
	virtual void fontPointSize(unsigned char pointSize) = 0;
	virtual void fontId(unsigned short id) = 0;
	virtual void marginReset(unsigned short leftMargin, unsigned short rightMargin) = 0;
	virtual void topMarginSet(unsigned short topMargin) = 0;
	virtual void bottomMarginSet(unsigned short bottomMargin) = 0;
	virtual void leftIndent(unsigned short leftMarginOffset) = 0;
	virtual void leftRightIndent(unsigned short leftRightMarginOffset) = 0;
	virtual void leftMarginRelease(unsigned short release) = 0;
	virtual void setTabs(const std::vector<WPXTabStop> &tabStops) = 0;
	virtual void headerFooterGroup(unsigned char headerFooterDefinition, WP1SubDocument *subDocument) = 0;
	virtual void suppressPageCharacteristics(unsigned char suppressCode) = 0;
	virtual void justificationChange(unsigned char justification) = 0;
	virtual void lineSpacingChange(unsigned char spacing) = 0;
	virtual void flushRightOn() = 0;
	virtual void flushRightOff() = 0;
	virtual void centerOn() = 0;
	virtual void centerOff() = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;
	virtual void insertPicture(unsigned short width, unsigned short height, const librevenge::RVNGBinaryData &binaryData) = 0;

};

#endif /* WP1LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
