/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

// 
//
//////////////////////////////////////////////////////////////////////

#ifndef __DRMA_VALUES_H__
#define __DRMA_VALUES_H__

/**
 * @brief DRMA³K»ΐW
 */
struct DRMAPoint {
	int eX; ///< XΐW
	int eY; ///< YΐW
};

/**
 * @brief DRMAξ{ΉHNID
 */
struct DRMABaseLinkId {
	DRMABaseLinkId();
	DRMABaseLinkId(int nNode1Id, int nNode2Id);

	int eNode1Id; ///< m[hPΤ
	int eNode2Id; ///< m[hQΤ
};

/**
 * @brief DRMASΉHNID
 */
typedef DRMABaseLinkId DRMAAllLinkId;

/**
 * @brief DRMAZίΌΜ
 */
struct DRMAShortName {
	int eKanjiLength; ///< ΏΆρ·
	char eKanji[21]; ///< SJISΏΆρ
	int eKanaLength; ///< JiΆρ·
	char eKana[21]; ///< ASCIIJiΆρ
};

/**
 * @brief DRMA·ίΌΜ
 */
struct DRMALongName {
	int eKanjiLength; ///< ΏΆρ·
	char eKanji[61]; ///< SJISΏΆρ
	int eKanaLength; ///< JiΆρ·
	char eKana[61];///< ASCIIJiΆρ
};

bool operator < (const DRMABaseLinkId& rId1, const DRMABaseLinkId& rId2);

#endif
