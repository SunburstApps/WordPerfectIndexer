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
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP42LISTENER_H
#define WP42LISTENER_H


class WP42SubDocument;

class WP42Listener
{
public:
	WP42Listener();
	virtual ~WP42Listener() {}

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void insertCharacter(unsigned character) = 0;
	virtual void insertTab(unsigned char tabType, double tabPosition) = 0;
	virtual void insertBreak(unsigned char breakType) = 0;
	virtual void insertEOL() = 0;
	virtual void attributeChange(bool isOn, unsigned char attribute) = 0;
	virtual void marginReset(unsigned char leftMargin, unsigned char rightMargin) = 0;
	virtual void headerFooterGroup(unsigned char headerFooterDefinition, WP42SubDocument *subDocument) = 0;
	virtual void suppressPageCharacteristics(unsigned char suppressCode) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;
};

#endif /* WP42LISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
