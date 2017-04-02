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

#include "WP1StylesListener.h"
#include "WP1FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

WP1StylesListener::WP1StylesListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments) :
	WP1Listener(),
	WPXStylesListener(pageList),
	m_currentPage(),
	m_nextPage(),
	m_subDocuments(subDocuments),
	m_tempMarginLeft(1.0),
	m_tempMarginRight(1.0),
	m_currentPageHasContent(false),
	m_isSubDocument(false),
	m_pageListHardPageMark(m_pageList.end())
{
}

void WP1StylesListener::endDocument()
{
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP1StylesListener::endSubDocument()
{
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP1StylesListener::insertBreak(unsigned char breakType)
{
	if (m_isSubDocument)
		return;

	if (!isUndoOn())
	{
		WPXTableList tableList;
		switch (breakType)
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
		{
			if ((m_pageList.size() > 0) && (m_currentPage==m_pageList.back())
			        && (m_pageListHardPageMark != m_pageList.end()))
			{
				m_pageList.back().setPageSpan(m_pageList.back().getPageSpan() + 1);
			}
			else
			{
				m_pageList.push_back(WPXPageSpan(m_currentPage));
				if (m_pageListHardPageMark == m_pageList.end())
				{
					--m_pageListHardPageMark;
				}
			}
			m_currentPage = WPXPageSpan(m_pageList.back(), 0.0, 0.0);
			m_currentPage.setPageSpan(1);

			for (std::vector<WPXHeaderFooter>::const_iterator HFiter = (m_nextPage.getHeaderFooterList()).begin();
			        HFiter != (m_nextPage.getHeaderFooterList()).end(); ++HFiter)
			{
				if ((*HFiter).getOccurrence() != NEVER)
				{
					m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
					                              (*HFiter).getOccurrence(), (*HFiter).getSubDocument(), (*HFiter).getTableList());
					_handleSubDocument((*HFiter).getSubDocument(), WPX_SUBDOCUMENT_HEADER_FOOTER, (*HFiter).getTableList());
				}
				else
				{
					m_currentPage.setHeaderFooter((*HFiter).getType(), (*HFiter).getInternalType(),
					                              (*HFiter).getOccurrence(), 0, (*HFiter).getTableList());
				}
			}
			m_nextPage = WPXPageSpan();
			m_currentPageHasContent = false;
		}
		break;
		default:
			break;
		}
		if (breakType == WPX_PAGE_BREAK)
		{
			m_pageListHardPageMark = m_pageList.end();
			m_currentPage.setMarginLeft(m_tempMarginLeft);
			m_currentPage.setMarginRight(m_tempMarginRight);
		}
	}
}

void WP1StylesListener::marginReset(unsigned short leftMargin, unsigned short rightMargin)
{
	if (!isUndoOn())
	{
		if (m_isSubDocument)
			return; // do not collect L/R margin information in sub documents

		std::list<WPXPageSpan>::iterator Iter;
		double marginInch = 0.0;

		if (leftMargin)
		{
			marginInch = (double)((double)leftMargin / 72.0);
			WPD_DEBUG_MSG(("Left Margin: %ipts = %.4finch\n", leftMargin, marginInch));
			if (!m_currentPageHasContent && (m_pageListHardPageMark == m_pageList.end()))
				m_currentPage.setMarginLeft(marginInch);
			else if (m_currentPage.getMarginLeft() > marginInch)
			{
				// Change the margin for the current page and for all pages in the list since the last Hard Break
				m_currentPage.setMarginLeft(marginInch);
				for (Iter = m_pageListHardPageMark; Iter != m_pageList.end(); ++Iter)
				{
					(*Iter).setMarginLeft(marginInch);
				}
			}
			m_tempMarginLeft = marginInch;
		}

		if (rightMargin)
		{
			marginInch = (double)((double)rightMargin / 72.0);
			WPD_DEBUG_MSG(("Right Margin: %ipts = %.4finch\n", rightMargin, marginInch));
			if (!m_currentPageHasContent && (m_pageListHardPageMark == m_pageList.end()))
				m_currentPage.setMarginRight(marginInch);
			else if (m_currentPage.getMarginRight() > marginInch)
			{
				// Change the margin for the current page and for all pages in the list since the last Hard Break
				m_currentPage.setMarginRight(marginInch);
				for (Iter = m_pageListHardPageMark; Iter != m_pageList.end(); ++Iter)
				{
					(*Iter).setMarginRight(marginInch);
				}
			}
			m_tempMarginRight = marginInch;
		}
	}
}

void WP1StylesListener::topMarginSet(unsigned short topMargin)
{
	if (!isUndoOn())
	{
		if (!topMargin) return;
		double marginInch = (double)((double)topMargin / 72.0);
		m_currentPage.setMarginTop(marginInch);
	}
}

void WP1StylesListener::bottomMarginSet(unsigned short bottomMargin)
{
	if (!isUndoOn())
	{
		if (!bottomMargin) return;
		double marginInch = (double)((double)bottomMargin / 72.0);
		m_currentPage.setMarginBottom(marginInch);
	}
}

void WP1StylesListener::headerFooterGroup(unsigned char headerFooterDefinition, WP1SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);

	if (!isUndoOn())
	{
		bool tempCurrentPageHasContent = m_currentPageHasContent;

		unsigned char headerFooterType = (unsigned char)(headerFooterDefinition & 0x03);
		WPXHeaderFooterType wpxType = ((headerFooterType <= WPX_HEADER_B) ? HEADER : FOOTER);

		unsigned char occurrenceBits = (unsigned char)((headerFooterDefinition & 0x1C) >> 2);

		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurrenceBits: %i)\n",
		               headerFooterType, occurrenceBits));

		WPXHeaderFooterOccurrence wpxOccurrence;

		if (occurrenceBits & WP1_HEADER_FOOTER_GROUP_ALL_BIT)
			wpxOccurrence = ALL;
		else if (occurrenceBits & WP1_HEADER_FOOTER_GROUP_EVEN_BIT)
			wpxOccurrence = EVEN;
		else if (occurrenceBits & WP1_HEADER_FOOTER_GROUP_ODD_BIT)
			wpxOccurrence = ODD;
		else
			wpxOccurrence = NEVER;

		WPXTableList tableList;

		if ((wpxType == HEADER) && tempCurrentPageHasContent)
			m_nextPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurrence, subDocument, tableList);
		else /* FOOTER || !tempCurrentPageHasContent */
		{
			if (wpxOccurrence != NEVER)
			{
				m_currentPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurrence, subDocument, tableList);
				_handleSubDocument(subDocument, WPX_SUBDOCUMENT_HEADER_FOOTER, tableList);
			}
			else
				m_currentPage.setHeaderFooter(wpxType, headerFooterType, wpxOccurrence, 0, tableList);
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}

void WP1StylesListener::suppressPageCharacteristics(unsigned char suppressCode)
{
	if (!isUndoOn())
	{
		if (suppressCode & 0x01)
		{
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_A, true);
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_B, true);
		}
		if (suppressCode & 0x10)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_A, true);
		if (suppressCode & 0x20)
			m_currentPage.setHeadFooterSuppression(WPX_HEADER_B, true);
		if (suppressCode & 0x40)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_A, true);
		if (suppressCode & 0x80)
			m_currentPage.setHeadFooterSuppression(WPX_FOOTER_B, true);
	}
}

void WP1StylesListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType,
                                           WPXTableList /* tableList */, int /* nextTableIndice */)
{
	if (!isUndoOn())
	{
		bool oldIsSubDocument = m_isSubDocument;
		m_isSubDocument = true;
		if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER)
		{
			bool oldCurrentPageHasContent = m_currentPageHasContent;

			if (subDocument)
				static_cast<const WP1SubDocument *>(subDocument)->parse(this);

			m_currentPageHasContent = oldCurrentPageHasContent;
		}
		else
		{
			if (subDocument)
				static_cast<const WP1SubDocument *>(subDocument)->parse(this);
		}
		m_isSubDocument = oldIsSubDocument;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
