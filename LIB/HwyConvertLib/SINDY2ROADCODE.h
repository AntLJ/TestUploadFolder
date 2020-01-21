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

// SINDY2ROADCODE.h: SINDY2ROADCODE �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_)
#define AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "INHwySchema.h"

// road.code.doc�n�t�@�C���̂P���R�[�h��\���\����
struct RoadCodeDoc
{
	long		eClass;				// �Ǘ��c��
	long		eClassForSiRiUS;	// �Ǘ��c�́iSiRiUS�p�j[Bug6456] SiRiUS�����t�@�C���ł́u��ʗL�����H�v���u�X�����v�Ɠ��ꎋ���Ȃ��̂�eClass�Ƃ͕ʂɒǉ�
	long		eRoadCode;			// �H���R�[�h
	std::string	eName;				// ��������
	std::string	eYomi;				// �ǂݖ���
	std::string eName2;				// ��������2�iroad.code.doc.highway�p�j
	std::string	eYomi2;				// �ǂݖ���2�iroad.code.doc.highway�p�j
	std::string	eName3;				// ��������3�iroad.code.doc.display�p�j
	std::string	eYomi3;				// �ǂݖ���3�iroad.code.doc.display�p�j

	RoadCodeDoc() : eClass(0), eClassForSiRiUS(0), eRoadCode(0) {}
	virtual ~RoadCodeDoc() {}
};

// RoadCodeDoc���Ǘ��c�̖��ƊǗ��c�̖��{�H���R�[�h�Ń\�[�g���邽�߂̃t�@���N�^
struct lessRoadCodeDoc : public std::binary_function<RoadCodeDoc, RoadCodeDoc, bool>
{
	bool operator ()(const RoadCodeDoc &lhs, const RoadCodeDoc &rhs) const
	{
		return
			lhs.eClass != rhs.eClass ? lhs.eClass < rhs.eClass :
									   lhs.eRoadCode < rhs.eRoadCode;
	}
};


class SINDY2ROADCODE : public SINDY2INHWY  
{
public:
	SINDY2ROADCODE(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2ROADCODE();

	virtual HRESULT Convert();
	virtual HRESULT Write			(std::string cFileName, long cType = in::schema::road_code_doc::file_type::road_code);
	virtual HRESULT WriteForSirius	(std::string cFileName, long cFormat);

	std::string		LineClass2OrgName(long cLineClass);
	std::string		LineClass2OrgYomi(long cLineClass);
	std::string		shortenName(std::string cName, std::string cBefore, std::string cAfter);
	std::pair<std::string, std::string> divideByCaseArc(std::string cStr);

protected:
	typedef std::multiset<RoadCodeDoc,lessRoadCodeDoc> RoadCodeDocArray;
	RoadCodeDocArray mRoadCodeDocArray;

public:
	typedef RoadCodeDocArray::const_iterator Iter;

	/// �f�[�^��̐擪���w��const_iterator��Ԃ�
	Iter begin() const { return mRoadCodeDocArray.begin(); }

	/// �f�[�^��̖������w��const_iterator��Ԃ�
	Iter end() const { return mRoadCodeDocArray.end(); }

	/// �f�[�^��̃T�C�Y��Ԃ�
	std::size_t size() const { return mRoadCodeDocArray.size(); }
};

#endif // !defined(AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_)
