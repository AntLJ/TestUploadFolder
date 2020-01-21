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
 * @brief DRMA正規化座標
 */
struct DRMAPoint {
	int eX; ///< X座標
	int eY; ///< Y座標
};

/**
 * @brief DRMA基本道路リンクID
 */
struct DRMABaseLinkId {
	DRMABaseLinkId();
	DRMABaseLinkId(int nNode1Id, int nNode2Id);

	int eNode1Id; ///< ノード１番号
	int eNode2Id; ///< ノード２番号
};

/**
 * @brief DRMA全道路リンクID
 */
typedef DRMABaseLinkId DRMAAllLinkId;

/**
 * @brief DRMA短め名称
 */
struct DRMAShortName {
	int eKanjiLength; ///< 漢字文字列長
	char eKanji[21]; ///< SJIS漢字文字列
	int eKanaLength; ///< カナ文字列長
	char eKana[21]; ///< ASCIIカナ文字列
};

/**
 * @brief DRMA長め名称
 */
struct DRMALongName {
	int eKanjiLength; ///< 漢字文字列長
	char eKanji[61]; ///< SJIS漢字文字列
	int eKanaLength; ///< カナ文字列長
	char eKana[61];///< ASCIIカナ文字列
};

bool operator < (const DRMABaseLinkId& rId1, const DRMABaseLinkId& rId2);

#endif
