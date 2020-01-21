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

// SINDY2HWYNODE.h: SINDY2HWYNODE クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_)
#define AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "INHwySchema.h"
#include <boost/array.hpp>


// 料金所機能コードのフラグ列
typedef boost::array<
	bool,
	in::schema::hwynode::toll_func_flags::num_of_tollfunc_value
> TollFuncFlags;


// hwynode.txtの１レコードの情報を格納する構造体
struct HighwayNode
{
	long		eFileType;		// このデータを出力すべきファイルの種類

	long		eMesh;			// 2次メッシュコード
	long		eNode;			// ノードID
	long		eRoadCode;		// 路線コード
	long		eSequence;		// 路線内シーケンス番号
	long		eFacilPos;		// 施設位置コード
	long		eDirection;		// 上下線コード

	long		eFacilClass;	// 施設種別コード
	long		eTollClass;		// 料金所種別コード
	long		eInOut;			// 出入口コード

	bool			eNoEtcCarFlag;			// 一般車両対応フラグ
	TollFuncFlags	eTollFuncFlags;			// 料金所機能コード（一般）
	bool			eNoGuideTollFlag;		// 非誘導料金所フラグ（一般）

	long			eEtcFacilCode;			// ETC施設コード
	TollFuncFlags	eEtcTollFuncFlags;		// 料金所機能コード(ETC)の上5桁
	bool			eEtcNoGuideTollFlag;	// 非誘導料金所フラグ（ETC）

	long		eAppNode;		// 流入ノードID
	long		eFlowNode;		// 流出ノードID
	std::string	eName;			// 漢字名称
	std::string	eYomi;			// 読み名称

	HighwayNode();
	virtual ~HighwayNode() {}
};


typedef	long	RoadCode;
typedef	long	RoadSeq;
typedef std::multimap<RoadCode, RoadSeq>	FacilCodeArray;

// tollguide.txtに出力するデータを格納する構造体
struct TollGuide
{
	INMNode			eNode;				// メッシュコード＋ノードID
	TollFuncFlags	eTollFuncFlags;		// 対応料金所機能コード
	FacilCodeArray	eFacilCodeArray;	// 施設情報（路線コードと路線内シーケンスの組）の列

	TollGuide() { eTollFuncFlags.assign(false); }
	virtual ~TollGuide() {}
};

// tollguide.txtのレコードを、メッシュ＋ノードIDを基準として並べるためのファンクタ
struct lessMNode : public std::binary_function<INMNode,INMNode,bool>
{
	bool operator ()(const INMNode &cNode1, const INMNode &cNode2) const
	{
		return (cNode1.eMesh != cNode2.eMesh) ? cNode1.eMesh < cNode2.eMesh : cNode1.eNode < cNode2.eNode;
	}
};

// tollguide.txtのレコード列
typedef std::map<INMNode, TollGuide, lessMNode> TollGuideArray;


class SINDY2HWYNODE : public SINDY2INHWY
{
public:
	SINDY2HWYNODE(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2HWYNODE();

	virtual HRESULT Convert(long cFormat);
	virtual HRESULT Write(std::string cFileName, long cFileType, long cFormat);

	static HRESULT isTarget(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, bool *cTarget);
	static HRESULT isGuideNode(IFeaturePtr ipHwyNode, bool *cGuide);
	static HRESULT GetMeshCode(IFeaturePtr ipHwyNode, long *cMesh);
	static HRESULT GetNodeID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cNodeID);
	static HRESULT GetFacilPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cFacilPos);
	static HRESULT GetUpDown(IFeaturePtr ipHwyNode, long *cUpDown);
	static HRESULT GetInOutNode(IFeaturePtr ipHwyNode, long *cAppNode, long *cFlowNode);
	static HRESULT GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, std::string *cName, std::string *cYomi);

private:
	void	WriteHwyNode		(std::ofstream& fout, long cFormat);
	void	WriteHwyNodeInOut	(std::ofstream& fout, long cFormat);
	void	WriteTollNode		(std::ofstream& fout, long cFormat);
	void	WriteTollGuide		(std::ofstream& fout, long cFormat);

	std::auto_ptr<TollGuideArray> makeTollGuideArray(void);

private:
	typedef std::vector<HighwayNode> HighwayNodeArray;
	HighwayNodeArray mHighwayNodeArray;

public:
	typedef HighwayNodeArray::const_iterator Iter;

	/// データ列の先頭を指すconst_iteratorを返す
	Iter begin() const { return mHighwayNodeArray.begin(); }

	/// データ列の末尾を指すconst_iteratorを返す
	Iter end() const { return mHighwayNodeArray.end(); }

	/// データ列のサイズを返す
	std::size_t size() const { return mHighwayNodeArray.size(); }
};

#endif // !defined(AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_)
