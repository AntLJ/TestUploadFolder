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
#include "sdebase.h"

/**
 * @brief マルチパートポリゴンの解消をするクラス
 */
class CSeparatePolygon :
	public CSDEBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CSeparatePolygon(CLayerManage* cLayerManage);

	/**
	 * @brief デストラクタ
	 */
	virtual ~CSeparatePolygon(void);

	/**
	 * @brief 初期化関数
	 */
	bool init(void);

private:
	typedef std::list<CString> TGT_LAYER_LIST;	///< （処理対象）レイヤ管理用リスト

	TGT_LAYER_LIST m_TgtList;	///< 処理対象レイヤ管理用リスト

	/**
	 * @brief メッシュ単位でのマルチパートポリゴン解消関数
	 */
	bool fnExecMesh(void);

	/**
	 * @brief レイヤ毎のマルチパート解消関数
	 *
	 * @param cEnv		[in]	各種情報を保持した構造体
	 *
	 */
	bool fnSeparate(EnvTable& cEnv);

	/**
	 * @brief マルチパートであるかのチェックと分割関数
	 *
	 * @param ipFeature	[in/out]	チェック・分割するフィーチャ
	 * @param cEnv		[in]		各種情報を保持した構造体
	 *
	 */
	bool fnCheckAndSeparate(IFeaturePtr& ipFeature, EnvTable& cEnv);

	/**
	 * @brief 継承する属性のコピー関数
	 *
	 * @param ipBuffer	[in/out]	コピー先フィーチャ
	 * @param ipFeature	[in/out]	コピー元フィーチャ
	 * @param cSDEIndex		[in]		フィールドインデックスマップ
	 *
	 */
	bool fnCopyAttr(IFeatureBufferPtr& ipBuffer, const IFeaturePtr& ipFeature, INDEX_MAP& cSDEIndex);
};
