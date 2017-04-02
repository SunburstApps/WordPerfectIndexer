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

#ifndef WP3PARSER_H
#define WP3PARSER_H

#include <librevenge/librevenge.h>
#include "WPXParser.h"

class WP3Listener;
class WP3ResourceFork;

class WP3Parser : public WPXParser
{
public:
	WP3Parser(librevenge::RVNGInputStream *input, WPXHeader *header, WPXEncryption *encryption);
	~WP3Parser();

	void parse(librevenge::RVNGTextInterface *documentInterface);
	void parseSubDocument(librevenge::RVNGTextInterface *documentInterface);

	static void parseDocument(librevenge::RVNGInputStream *input, WPXEncryption *encryption, WP3Listener *listener);

private:
	WP3ResourceFork *getResourceFork(librevenge::RVNGInputStream *input, WPXEncryption *encryption);

	void parse(librevenge::RVNGInputStream *input, WPXEncryption *encryption, WP3Listener *listener);
};

#endif /* WP3PARSER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
