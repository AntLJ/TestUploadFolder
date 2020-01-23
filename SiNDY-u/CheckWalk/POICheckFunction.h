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

#pragma once
#include "OutputLog.h"

// TODO: 他のばらして基底作れそうなら作る

class POICheckFunction
{
public:
	POICheckFunction(COutPut& out, double refer_id_threshold):
		m_output(out)
		, m_refer_id_threshold(refer_id_threshold)
	{}

	/**
	 * @brief	POIポイントのチェック
	 * @param	meshcode	[in]	チェック対象メッシュコード
	 * @param	ipMeshGeo	[in]	チェック対象のジオメトリ
	 * @param	poiPoint   	[in]	チェック対象POIポイント群クラス
	 * @param	walkNode   	[in]	チェック参照用歩行者ノード群クラス
	 * @param	walkNodeRTree	[in]	チェック参照用歩行者ノードRTree
	 */
	void CheckPoiPoint( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo, 
		const FeaturesPack& poiPoint,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
		);

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	double m_refer_id_threshold; //!< 関連ID距離閾値(m)
};
