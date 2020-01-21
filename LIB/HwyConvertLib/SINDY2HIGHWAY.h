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

// SINDY2HIGHWAY.h: SINDY2HIGHWAY �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2HIGHWAY_H__AB2F08CA_12AA_4AAB_85D1_504295FDEC5D__INCLUDED_)
#define AFX_SINDY2HIGHWAY_H__AB2F08CA_12AA_4AAB_85D1_504295FDEC5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"

class SINDY2HIGHWAY : public SINDY2INHWY  
{
public:
	struct FacilInfo
	{
		long		eRoadCode;					// �H���R�[�h
		long		eFacilClassAndInOutCode;	// �{�ݎ�ʁ{�o�����R�[�h
		long		eFacilClass;				// �{�ݎ�ʃR�[�h
		long		eInOut;						// �o�����R�[�h
		long		eLineDir;					// �����t���O
		long		eOrderSeq;					// �����ʘH�����V�[�P���X�ԍ�
		long		eBranchCode;					// ����R�[�h

		FacilInfo() : eRoadCode(0), eFacilClassAndInOutCode(0), eFacilClass(0), eInOut(0), eLineDir(0), eOrderSeq(0), eBranchCode(0) {}
		virtual ~FacilInfo() {}
	};

	// highway.txt�̂P���R�[�h��\���\����
	struct HighwayText
	{
		FacilInfo				eFacilInfo;		// ���Y�{�ݏ��
		long					eRoadSeq;		// �H�����V�[�P���X�ԍ�
		long					eNext;			// ���̎{�ݔԍ�
		long					eDistance;		// ���̎{�݂܂ł̋�ԋ���
		long					eTerm;			// �I�[�t���O
		std::vector<FacilInfo>	eTo;			// �O������{��
		std::vector<FacilInfo>	eFrom;			// �������{��

		HighwayText() : eFacilInfo(), eRoadSeq(0), eNext(0), eDistance(0), eTerm(0), eTo(), eFrom() {}
		virtual ~HighwayText() {}
	};

	// �H���R�[�h�A�����t���O�A�����ʘH�����V�[�P���X�ԍ���HighwayText���\�[�g���邽�߂̃t�@���N�^
	struct lessSequence : public std::binary_function<HighwayText, HighwayText, bool>
	{
		bool operator ()(const HighwayText &cHwyPnt1, const HighwayText &cHwyPnt2) const
		{
			return (cHwyPnt1.eFacilInfo.eRoadCode != cHwyPnt2.eFacilInfo.eRoadCode) ? cHwyPnt1.eFacilInfo.eRoadCode < cHwyPnt2.eFacilInfo.eRoadCode :
			       (cHwyPnt1.eFacilInfo.eLineDir != cHwyPnt2.eFacilInfo.eLineDir) ? cHwyPnt1.eFacilInfo.eLineDir < cHwyPnt2.eFacilInfo.eLineDir :
			       cHwyPnt1.eFacilInfo.eOrderSeq < cHwyPnt2.eFacilInfo.eOrderSeq;
		}
	};

public:
	SINDY2HIGHWAY(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2HIGHWAY();

	virtual HRESULT Convert();
	virtual HRESULT Write(std::string cFileName, long cFormat, bool bIsGuide = false);

public:
	static HRESULT isTarget(_IRowPtr ipHwyText, bool *cTarget);
	static HRESULT GetFacilInfo(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, FacilInfo *cFacilInfo);
	static HRESULT GetFacilClassAndInOutCode(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cClass);
	static HRESULT GetSequence(_IRowPtr ipHwyText, long *cSequence);
	static HRESULT GetName(_IRowPtr ipHwyText, std::string *cName);
	static HRESULT GetNextID(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cNextID);
	static HRESULT GetDistance(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cDistance);
	static HRESULT GetTermFlag(_IRowPtr ipHwyText, long *cTerm);
	static HRESULT GetToFacil(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, std::vector<FacilInfo> *cToFacil);
	static HRESULT GetFromFacil(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, std::vector<FacilInfo> *cFromFacil);

protected:
	typedef std::set<HighwayText, lessSequence> HighwayTextArray;
	HighwayTextArray	mHighwayTextArray;

public:
	typedef HighwayTextArray::const_iterator Iter;

	/// �f�[�^��̐擪���w��const_iterator��Ԃ�
	Iter begin() const { return mHighwayTextArray.begin(); }

	/// �f�[�^��̖������w��const_iterator��Ԃ�
	Iter end() const { return mHighwayTextArray.end(); }

	/// �f�[�^��̃T�C�Y��Ԃ�
	std::size_t size() const { return mHighwayTextArray.size(); }
};


long GetFacilClass(_IRow* cpRow, SINDY2INHWYINFO* cpHwyInfo);


#endif // !defined(AFX_SINDY2HIGHWAY_H__AB2F08CA_12AA_4AAB_85D1_504295FDEC5D__INCLUDED_)
