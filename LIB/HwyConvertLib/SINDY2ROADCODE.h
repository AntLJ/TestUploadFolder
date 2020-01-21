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

// SINDY2ROADCODE.h: SINDY2ROADCODE クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_)
#define AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "INHwySchema.h"

// road.code.doc系ファイルの１レコードを表す構造体
struct RoadCodeDoc
{
	long		eClass;				// 管理団体
	long		eClassForSiRiUS;	// 管理団体（SiRiUS用）[Bug6456] SiRiUS向けファイルでは「一般有料道路」を「街道名」と同一視しないのでeClassとは別に追加
	long		eRoadCode;			// 路線コード
	std::string	eName;				// 漢字名称
	std::string	eYomi;				// 読み名称
	std::string eName2;				// 漢字名称2（road.code.doc.highway用）
	std::string	eYomi2;				// 読み名称2（road.code.doc.highway用）
	std::string	eName3;				// 漢字名称3（road.code.doc.display用）
	std::string	eYomi3;				// 読み名称3（road.code.doc.display用）

	RoadCodeDoc() : eClass(0), eClassForSiRiUS(0), eRoadCode(0) {}
	virtual ~RoadCodeDoc() {}
};

// RoadCodeDocを管理団体名と管理団体名＋路線コードでソートするためのファンクタ
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

	/// データ列の先頭を指すconst_iteratorを返す
	Iter begin() const { return mRoadCodeDocArray.begin(); }

	/// データ列の末尾を指すconst_iteratorを返す
	Iter end() const { return mRoadCodeDocArray.end(); }

	/// データ列のサイズを返す
	std::size_t size() const { return mRoadCodeDocArray.size(); }
};

#endif // !defined(AFX_SINDY2ROADCODE_H__9A02F887_3209_479F_A528_1E7F3CB66B5F__INCLUDED_)
