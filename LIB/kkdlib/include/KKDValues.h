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
 * @brief KKD����
 */
struct KKDName {
	int eKanjiLength; ///< ����������
	char eKanji[31]; ///< SJIS����������
	int eKanaLength; ///< �J�i������
	char eKana[31]; ///< ASCII�J�i������
};

/**
 * @brief KKD�����_�ԍ�
 */
struct KKDIntersectionId {
	int eMeshCode; ///< �Q�����b�V���R�[�h
	int ePrefectureCode; ///< �s���{���R�[�h
	int eStationCode; ///< ���R�[�h
	int eCrossId; ///< ���������_�ԍ�
};

struct KKDIntersectionIdPair {
	KKDIntersectionId eIntersection1Id;
	KKDIntersectionId eIntersection2Id;
};

/**
 * @brief KKD�����N�ԍ�
 */
struct KKDLinkId {
	int eNode1Id; ///< �m�[�h�P�ԍ�
	int eNode2Id; ///< �m�[�h�Q�ԍ�
};

/**
 * @brief KKD�����_�ʍs�K��
 */
struct KKDIntersectionRegulation {
	int eOppositeNodeMeshDirection; ///< �ڑ��m�[�h�Y�����b�V���R�[�h
	int eOppositeNodeId; ///< �o�R��ڑ��m�[�h�ԍ�
	KKDIntersectionId eOppositeIntersectionId; ///< �ڑ������_�ԍ�
	int eRegulationLane; ///< ��ʋK�����[���R�[�h
	int ePassage; ///< �ʍs�R�[�h
	int eCarType; ///< �Ԏ�w��
	int eWeekday; ///< �j���w��
	int eTime; ///< ���Ԏw��i�n�A�I�����j
	int eDate; ///< �����w��i�n�A�I�����j
};

bool operator < (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator > (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator == (const KKDIntersectionId& rId1, const KKDIntersectionId& rId2);
bool operator < (const KKDIntersectionIdPair& rPair1, const KKDIntersectionIdPair& rPair2);

#endif
