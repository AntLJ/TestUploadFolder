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

#ifndef	_SINDY2TOLLGATELANE_H_
#define	_SINDY2TOLLGATELANE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"


// tollgate_laneinfo.txtの1レコードを表す構造体
struct TollGateLaneInfo
{
	long		m_HwyNodeOID;		// 対応するハイウェイノードのObjectID（デバッグ用の情報）
	long		m_MeshCode;			// 2次メッシュコード
	long		m_TollGateNodeID;	// 料金所ノードID
	long		m_ApproachNodeID;	// 流入ノードID
	long		m_LaneNum;			// レーン数
	std::string	m_LaneStr;			// レーン状態
};
// tollgate_laneinfo.txtのレコードのソート基準とするファンクタ
struct lessTollGateLaneInfo : public std::binary_function<TollGateLaneInfo,TollGateLaneInfo,bool>
{
	bool operator ()(const TollGateLaneInfo& lhs, const TollGateLaneInfo& rhs) const
	{
		return
			lhs.m_MeshCode != rhs.m_MeshCode				? lhs.m_MeshCode < rhs.m_MeshCode :
			lhs.m_TollGateNodeID != rhs.m_TollGateNodeID	? lhs.m_TollGateNodeID < rhs.m_TollGateNodeID :
			lhs.m_ApproachNodeID != rhs.m_ApproachNodeID	? lhs.m_ApproachNodeID < rhs.m_ApproachNodeID :
			lhs.m_LaneNum != rhs.m_LaneNum					? lhs.m_LaneNum < rhs.m_LaneNum :
															  lhs.m_LaneStr < rhs.m_LaneStr;
	}
};
// tollgate_laneinfo.txtのレコード群を格納するコンテナ
typedef std::set<TollGateLaneInfo, lessTollGateLaneInfo> TollGateLaneInfoArray;

// tollgate_laneinfo.txtを作成するためのSiNDYからのデータの取得、データの保持、ファイルへの出力を担当するクラス
class SINDY2TOLLGATELANE : public SINDY2INHWY
{
public:
	SINDY2TOLLGATELANE(SINDY2INHWYINFO *cHwyInfo);
	~SINDY2TOLLGATELANE();

	HRESULT Convert(void);
	HRESULT Write(std::string cFileName);

	HRESULT GetMeshCode(IFeaturePtr cpHwyNode, long& crMesh);
	HRESULT GetTollGateNodeID(IFeaturePtr cpHwyNode, long& crNodeID);
	HRESULT GetApproachNodeID(IFeaturePtr cpHwyNode, long& crNodeID);
	HRESULT GetLaneStr(IFeaturePtr cpHwyNode, std::string& crLaneStr);
private:
	TollGateLaneInfoArray m_TollGateLaneInfoArray;

public:
	typedef TollGateLaneInfoArray::const_iterator Iter;

	/// データ列の先頭を指すconst_iteratorを返す
	Iter begin() const { return m_TollGateLaneInfoArray.begin(); }

	/// データ列の末尾を指すconst_iteratorを返す
	Iter end() const { return m_TollGateLaneInfoArray.end(); }

	/// データ列のサイズを返す
	std::size_t size() const { return m_TollGateLaneInfoArray.size(); }
};


#endif	// _SINDY2TOLLGATELANE_H_
