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

#include "MeshClipProcessor.h"

/**
 * @brief クリッピング実行クラス
 */
template <typename T>
class LotClipProcessor : public MeshClipProcessor<T>
{
public:

	/// コンストラクタ
	LotClipProcessor(const sindy::CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef, const std::vector<uh::tstring>& meshs);

	/// デストラクタ
	virtual ~LotClipProcessor(){};

	/// クリップ処理を実施する
	bool doClip( sindy::CTableContainer& con, IEnvelopePtr mesh ) override;
private:
	/**
	* @brief ロットポリゴンを初期化する
	* @param spref [in] 空間参照
	*/
	void initLotPolygon( ISpatialReferencePtr spref );

	IGeometryPtr m_ipLotPolygon; //!< ロットポリゴン
	std::vector<uh::tstring> m_vecMesh; //!< メッシュリスト
};
