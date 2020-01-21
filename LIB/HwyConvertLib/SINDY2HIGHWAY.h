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

// SINDY2HIGHWAY.h: SINDY2HIGHWAY クラスのインターフェイス
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
		long		eRoadCode;					// 路線コード
		long		eFacilClassAndInOutCode;	// 施設種別＋出入口コード
		long		eFacilClass;				// 施設種別コード
		long		eInOut;						// 出入口コード
		long		eLineDir;					// 方向フラグ
		long		eOrderSeq;					// 方向別路線内シーケンス番号
		long		eBranchCode;					// 分岐コード

		FacilInfo() : eRoadCode(0), eFacilClassAndInOutCode(0), eFacilClass(0), eInOut(0), eLineDir(0), eOrderSeq(0), eBranchCode(0) {}
		virtual ~FacilInfo() {}
	};

	// highway.txtの１レコードを表す構造体
	struct HighwayText
	{
		FacilInfo				eFacilInfo;		// 当該施設情報
		long					eRoadSeq;		// 路線内シーケンス番号
		long					eNext;			// 次の施設番号
		long					eDistance;		// 次の施設までの区間距離
		long					eTerm;			// 終端フラグ
		std::vector<FacilInfo>	eTo;			// 前方分岐施設
		std::vector<FacilInfo>	eFrom;			// 後方分岐施設

		HighwayText() : eFacilInfo(), eRoadSeq(0), eNext(0), eDistance(0), eTerm(0), eTo(), eFrom() {}
		virtual ~HighwayText() {}
	};

	// 路線コード、方向フラグ、方向別路線内シーケンス番号でHighwayTextをソートするためのファンクタ
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

	/// データ列の先頭を指すconst_iteratorを返す
	Iter begin() const { return mHighwayTextArray.begin(); }

	/// データ列の末尾を指すconst_iteratorを返す
	Iter end() const { return mHighwayTextArray.end(); }

	/// データ列のサイズを返す
	std::size_t size() const { return mHighwayTextArray.size(); }
};


long GetFacilClass(_IRow* cpRow, SINDY2INHWYINFO* cpHwyInfo);


#endif // !defined(AFX_SINDY2HIGHWAY_H__AB2F08CA_12AA_4AAB_85D1_504295FDEC5D__INCLUDED_)
