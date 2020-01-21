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

// DRMARoadLinkRelation.h: DRMARoadLinkRelation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_)
#define AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief ��{���H�����N�E�S���H�����N�Ή��t�H�[�}�b�g�N���X
 */
struct DRMARoadLinkRelationFormat
{
// ��`
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct AllLink {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

// �t�H�[�}�b�g
	_DRMADec eRecordId[2];
	LinkId eId;
	_DRMADec eSequence[2];

	_DRMADec eAllLinkCount[3];
	AllLink eAllLinks[24];

	_DRMAHex	eContinue[1];
};

/**
 * @brief ��{���H�����N�E�S���H�����N�Ή����R�[�h�N���X
 */
class DRMARoadLinkRelation : public DRMARecordT<DRMARoadLinkRelationFormat>
{
public:
// ��`
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct AllLink : public DRMAAllLinkId, public DRMAItemConverterTemplate<DRMARoadLinkRelationFormat::AllLink> {
		AllLink(const Format& cOriginal);
	};

	typedef std::vector<AllLink> AllLinks;

// �\�z/����
	DRMARoadLinkRelation(const Format& cOriginalRecord);

// �ǉ�
	void append(const Format& cOriginalRecord);

protected:
// �ǉ�
	void appendAllLinks(const Format& cOriginalRecord);

public:
// �v���p�e�B
//	int eRecordId; ///< ���R�[�hID
	LinkId eId; ///< ��{���H�����N�ԍ�
	int eSequence; ///< �A�C�e�������R�[�h�ԍ�

	int eAllLinkCount; ///< �S���H�����N����
	AllLinks eAllLinks; ///< �S���H�����N�ԍ�

	int eContinue; ///< �p���t���O

// �ÓI�v���p�e�B
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_)
