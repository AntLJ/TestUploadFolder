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

#include "Editor.h"
#include "Processor.h"
#include "ClippingDefinitionManager.h"

namespace sindy {
	class CTableContainer;
}

/**
 * @brief クリッピング実行クラス
 */
template <typename T>
class MeshClipProcessor : public Processor<T>
{
public:

	/// コンストラクタ
	MeshClipProcessor(const sindy::CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef);

	/// デストラクタ
	virtual ~MeshClipProcessor(){};

	/**
	 * @brief  処理実行
	 * @param  src  [in]  メッシュコード
	 * @retval true:成功
	 */
	virtual bool proc(const uh::tstring& src);

	/// @brief ループ処理前実装メソッド
	bool preprocess() override;

	/// @brief ループ処理後実行メソッド
	bool postprocess() override
	{
		return m_workspace.StopEditing(true);
	}

	/**
	 * @brief  処理対象のWorkspaceを返却
	 * @retval 処理対象のWorkspace
	 */
	const sindy::CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CEditor  m_workspace;			//!< 出力先のDB
	ClippingDefinitionManager m_mgr;	//!< マージ対象定義クラス
	uh::tstring m_internalName;		//!< プログラム名(更新用)

	/**
	* @brief クリップ処理を実施する
	* 
	* mesh矩形でクリップ処理を実施します。\n
	* 検索も実行するため、conは空の状態を想定しています。\n
	* 処理後は、conにクリップ処理されたフィーチャが格納されます。
	*
	* @param con [in] クリップ処理対象フィーチャクラス（空で渡すこと）
	* @param mesh [in] クリップ処理矩形
	* @retval true クリップ処理成功
	* @retval false クリップ処理失敗
	*/
	virtual bool doClip( sindy::CTableContainer& con, IEnvelopePtr mesh );
};
