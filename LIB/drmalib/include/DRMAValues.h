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
 * @brief DRMA���K�����W
 */
struct DRMAPoint {
	int eX; ///< X���W
	int eY; ///< Y���W
};

/**
 * @brief DRMA��{���H�����NID
 */
struct DRMABaseLinkId {
	DRMABaseLinkId();
	DRMABaseLinkId(int nNode1Id, int nNode2Id);

	int eNode1Id; ///< �m�[�h�P�ԍ�
	int eNode2Id; ///< �m�[�h�Q�ԍ�
};

/**
 * @brief DRMA�S���H�����NID
 */
typedef DRMABaseLinkId DRMAAllLinkId;

/**
 * @brief DRMA�Z�ߖ���
 */
struct DRMAShortName {
	int eKanjiLength; ///< ����������
	char eKanji[21]; ///< SJIS����������
	int eKanaLength; ///< �J�i������
	char eKana[21]; ///< ASCII�J�i������
};

/**
 * @brief DRMA���ߖ���
 */
struct DRMALongName {
	int eKanjiLength; ///< ����������
	char eKanji[61]; ///< SJIS����������
	int eKanaLength; ///< �J�i������
	char eKana[61];///< ASCII�J�i������
};

bool operator < (const DRMABaseLinkId& rId1, const DRMABaseLinkId& rId2);

#endif
