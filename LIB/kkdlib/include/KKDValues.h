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

// KKDValues.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __KKD_VALUES_H__
#define __KKD_VALUES_H__

/**
 * @brief KKD名称
 */
struct KKDName {
	int eKanjiLength; ///< 漢字文字列長
	char eKanji[31]; ///< SJIS漢字文字列
	int eKanaLength; ///< カナ文字列長
	char eKana[31]; ///< ASCIIカナ文字列
};

/**
 * @brief KKD交差点番号
 */
struct KKDIntersectionId {
	int eMeshCode; ///< ２次メッシュコード
	int ePrefectureCode; ///< 都道府県コード
	int eStationCode; ///< 署コード
	int eCrossId; ///< 署内交差点番号
};

struct KKDIntersectionIdPair {
	KKDIntersectionId eIntersection1Id;
	KKDIntersectionId eIntersection2Id;
};

/**
 * @brief KKDリンク番号
 */
struct KKDLinkId {
	int eNode1Id; ///< ノード１番号
	int eNode2Id; ///< ノード２番号
};

/**
 * @brief KKD交差点通行規制
 */
struct KKDIntersectionRegulation {
	int eOppositeNodeMeshDirection; ///< 接続ノード該当メッシュコード
	int eOppositeNodeId; ///< 経由先接続ノード番号
	KKDIntersectionId eOppositeIntersectionId; ///< 接続交差点番号
	int eRegulationLane; ///< 交通規制レーンコード
	int ePassage; ///< 通行コード
	int eCarType; ///< 車種指定
	int eWeekday; ///< 曜日指定
	int eTime; ///< 時間指定（始、終時刻）
	int eDate; ///< 月日指定（始、終日時）
};

bool operator < (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator > (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator == (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator < (const KKDIntersectionIdPair& rPair1, const KKDIntersectionIdPair& rPair2);

#endif
