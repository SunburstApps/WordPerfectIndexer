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

#include <math.h>
#include <algorithm>
#include "WPXPageSpan.h"
#include "libwpd_internal.h"

const double WPX_DEFAULT_PAGE_MARGIN_TOP = 1.0;
const double WPX_DEFAULT_PAGE_MARGIN_BOTTOM = 1.0;

const unsigned char DUMMY_INTERNAL_HEADER_FOOTER = 16;

// precondition: 0 <= headerFooterType <= 3 (i.e.: we don't handle watermarks here)
WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurrence occurrence,
                                 const unsigned char internalType, const WPXSubDocument *subDocument, WPXTableList tableList) :
	m_type(headerFooterType),
	m_occurrence(occurrence),
	m_internalType(internalType),
	m_subDocument(subDocument),
	m_tableList(tableList)
{
}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurrence occurrence,
                                 const unsigned char internalType, const WPXSubDocument *subDocument) :
	m_type(headerFooterType),
	m_occurrence(occurrence),
	m_internalType(internalType),
	m_subDocument(subDocument),
	m_tableList()
{
}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooter &headerFooter) :
	m_type(headerFooter.getType()),
	m_occurrence(headerFooter.getOccurrence()),
	m_internalType(headerFooter.getInternalType()),
	m_subDocument(headerFooter.getSubDocument()),
	m_tableList(headerFooter.getTableList())
{
}

WPXHeaderFooter &WPXHeaderFooter::operator=(const WPXHeaderFooter &headerFooter)
{
	if (this != &headerFooter)
	{
		m_type = headerFooter.getType();
		m_occurrence = headerFooter.getOccurrence();
		m_internalType = headerFooter.getInternalType();
		m_subDocument = headerFooter.getSubDocument();
		m_tableList = headerFooter.getTableList();
	}
	return *this;
}

WPXHeaderFooter::~WPXHeaderFooter()
{
//	delete m_subDocument;
}

WPXPageSpan::WPXPageSpan() :
	m_isPageNumberSuppressed(false),
	m_formLength(11.0),
	m_formWidth(8.5f),
	m_formOrientation(PORTRAIT),
	m_marginLeft(1.0),
	m_marginRight(1.0),
	m_marginTop(WPX_DEFAULT_PAGE_MARGIN_TOP),
	m_marginBottom(WPX_DEFAULT_PAGE_MARGIN_BOTTOM),
	m_pageNumberPosition(PAGENUMBER_POSITION_NONE),
	m_isPageNumberOverridden(false),
	m_pageNumberOverride(0),
	m_pageNumberingType(ARABIC),
	m_pageNumberingFontName(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman"), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_pageNumberingFontSize(12.0/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_headerFooterList(),
	m_pageSpan(1)
{
	for (int i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i]=false;
}

// NB: this is not a literal "clone" function: it is contingent on the side margins that are passed,
// and suppression and override variables are not copied
WPXPageSpan::WPXPageSpan(const WPXPageSpan &page, double paragraphMarginLeft, double paragraphMarginRight) :
	m_isPageNumberSuppressed(false),
	m_formLength(page.getFormLength()),
	m_formWidth(page.getFormWidth()),
	m_formOrientation(page.getFormOrientation()),
	m_marginLeft(page.getMarginLeft()+paragraphMarginLeft),
	m_marginRight(page.getMarginRight()+paragraphMarginRight),
	m_marginTop(page.getMarginTop()),
	m_marginBottom(page.getMarginBottom()),
	m_pageNumberPosition(page.getPageNumberPosition()),
	m_isPageNumberOverridden(false),
	m_pageNumberOverride(0),
	m_pageNumberingType(page.getPageNumberingType()),
	m_pageNumberingFontName(page.getPageNumberingFontName()),
	m_pageNumberingFontSize(page.getPageNumberingFontSize()),
	m_headerFooterList(page.getHeaderFooterList()),
	m_pageSpan(page.getPageSpan())
{
	for (int i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i] = false;
}

WPXPageSpan::~WPXPageSpan()
{
}


void WPXPageSpan::setHeaderFooter(const WPXHeaderFooterType type, const unsigned char headerFooterType, const WPXHeaderFooterOccurrence occurrence,
                                  const  WPXSubDocument *subDocument, WPXTableList tableList)
{
	WPXHeaderFooter headerFooter(type, occurrence, headerFooterType, subDocument, tableList);
	switch (occurrence)
	{
	case ALL:
	case NEVER:
		_removeHeaderFooter(type, ODD);
		_removeHeaderFooter(type, EVEN);
		_removeHeaderFooter(type, ALL);
		break;
	case ODD:
		_removeHeaderFooter(type, ODD);
		_removeHeaderFooter(type, ALL);
		break;
	case EVEN:
		_removeHeaderFooter(type, EVEN);
		_removeHeaderFooter(type, ALL);
		break;
	default:
		break;
	}

	if ((occurrence != NEVER) && (subDocument))
		m_headerFooterList.push_back(headerFooter);

	bool containsHFLeft = _containsHeaderFooter(type, ODD);
	bool containsHFRight = _containsHeaderFooter(type, EVEN);

	WPD_DEBUG_MSG(("Contains HFL: %i HFR: %i\n", containsHFLeft, containsHFRight));
	if (containsHFLeft && !containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header right\n"));
		WPXHeaderFooter dummyHeader(type, EVEN, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
	else if (!containsHFLeft && containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header left\n"));
		WPXHeaderFooter dummyHeader(type, ODD, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
}

void WPXPageSpan::_removeHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurrence occurrence)
{
	for (std::vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); ++iter)
	{
		if ((*iter).getType() == type && (*iter).getOccurrence() == occurrence)
		{
			WPD_DEBUG_MSG(("WordPerfect: Removing header/footer element of type: %i since it is identical to %i\n",(*iter).getType(), type));
			m_headerFooterList.erase(iter);
			return;
		}
	}
}

bool WPXPageSpan::_containsHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurrence occurrence)
{
	for (std::vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); ++iter)
	{
		if ((*iter).getType()==type && (*iter).getOccurrence()==occurrence)
			return true;
	}

	return false;
}

inline bool operator==(const WPXHeaderFooter &headerFooter1, const WPXHeaderFooter &headerFooter2)
{
	return ((headerFooter1.getType() == headerFooter2.getType()) &&
	        (headerFooter1.getSubDocument() == headerFooter2.getSubDocument()) &&
	        (headerFooter1.getOccurrence() == headerFooter2.getOccurrence()) &&
	        (headerFooter1.getInternalType() == headerFooter2.getInternalType()));
}

bool operator==(const WPXPageSpan &page1, const WPXPageSpan &page2)
{
	if ((page1.getMarginLeft() != page2.getMarginLeft()) || (page1.getMarginRight() != page2.getMarginRight()) ||
	        (page1.getMarginTop() != page2.getMarginTop())|| (page1.getMarginBottom() != page2.getMarginBottom()))
		return false;

	if (page1.getPageNumberPosition() != page2.getPageNumberPosition())
		return false;

	if (page1.getPageNumberSuppression() != page2.getPageNumberSuppression())
		return false;

	if (page1.getPageNumberOverriden() != page2.getPageNumberOverriden() ||
	        page1.getPageNumberOverride() != page2.getPageNumberOverride())
		return false;

	if (page1.getPageNumberingType() != page2.getPageNumberingType())
		return false;

	if (page1.getPageNumberingFontName() != page2.getPageNumberingFontName() ||
	        page1.getPageNumberingFontSize() != page2.getPageNumberingFontSize())
		return false;

	for (unsigned char i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++)
	{
		if (page1.getHeaderFooterSuppression(i) != page2.getHeaderFooterSuppression(i))
			return false;
	}

	// NOTE: yes this is O(n^2): so what? n=4 at most
	const std::vector<WPXHeaderFooter> headerFooterList1 = page1.getHeaderFooterList();
	const std::vector<WPXHeaderFooter> headerFooterList2 = page2.getHeaderFooterList();
	std::vector<WPXHeaderFooter>::const_iterator iter1;
	std::vector<WPXHeaderFooter>::const_iterator iter2;

	for (iter1 = headerFooterList1.begin(); iter1 != headerFooterList1.end(); ++iter1)
	{
		if (std::find(headerFooterList2.begin(), headerFooterList2.end(), (*iter1)) == headerFooterList2.end())
			return false;
	}

	// If we came here, we know that every header/footer that is found in the first page span is in the second too.
	// But this is not enought for us to know whether the page spans are equal. Now we have to check in addition
	// whether every header/footer that is in the second one is in the first too. If someone wants to optimize this,
	// (s)he is most welcome :-)

	for (iter2 = headerFooterList2.begin(); iter2 != headerFooterList2.end(); ++iter2)
	{
		if (std::find(headerFooterList1.begin(), headerFooterList1.end(), (*iter2)) == headerFooterList1.end())
			return false;
	}


	WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan == comparison finished, found no differences\n"));

	return true;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
