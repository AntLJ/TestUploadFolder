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

#include "LayerCheckInclude.h"

/**
 * @class	CheckControl
 * @brief	制御クラス
 */
class CheckControl
{
public:
	typedef vector< shared_ptr<LayerCheckBase> > CHECKLIST;    //!< チェッククラスリスト

	/**
	 * @brief  コンストラクタ
	 */
	CheckControl(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~CheckControl(void){}

	/**
	* @brief	初期化
	* @param	[in] argument	        	コマンドライン引数
	* @retval   true:成功  false:失敗
	*/
	bool init( Argument& argument );

	/**
	* @brief	チェック
	* @retval   true:成功  false:失敗
	*/
	bool doCheck();

	/**
	* @brief	チェック
	* @param	[in]  meshCodeList           メッシュコードリスト
	* @param	[in]  meshFeatureClassName   メッシュフィチャクラース名
	* @param	[in]  currentCheckLayerList  対応するチエックリスト
	* @retval   true:成功  false:失敗
	*/
	bool check( const MeshCodeList& meshCodeList, const CString& meshFeatureClassName, CHECKLIST& currentCheckLayerList );

private:
	/**
	 * @brief	メッシュ切り替え時の初期化
	 * @param	[in]  ulNextMesh    次のメッシュコード
	 * @param	[in] meshFeaturClassName メッシュフィチャクラース名
	 * @param	[out] ipMeshGeo		メッシュジオメトリ
	 * @param	[out] ulMeshID		メッシュオブジェクトID
	 * @note	メッシュ切り替え時に行うこと
	 * @retval	true:成功  false:失敗
	 */
	bool initMesh( unsigned long ulNextMesh, const CString& meshFeaturClassName, IGeometryPtr& ipMeshGeo, long &meshID );

	/**
	 * @brief	メッシュフィーチャ取得
	 * @param	[in] ulMeshCode		メッシュコード
	 * @param	[in] meshFeaturClassName メッシュフィチャクラース名
	 * @return	メッシュフィーチャ
	 */
	IFeaturePtr getMeshFeature( unsigned long ulMeshCode, const CString&meshFeaturClassName );

	/**
	 * @brief	チェックレイヤインスタンスセット
	 * @param	[in] layer		レイヤ名
	 * @retval	true:成功  false:失敗
	 */
	bool setLayerInstance( const CString& layer );

private:
	CHECKLIST                      m_cityListLayerCheckInstance;   //!< 都市地図チェックレイヤリスト
	CHECKLIST                      m_baseListLayerCheckInstance;   //!< ベースメッシュチェックレイヤリスト

	MeshCodeList                   m_cityMeshCodeList;         //!< 都市メッシュコードリスト
	MeshCodeList                   m_baseMeshCodeList;         //!< ベースメッシュコードリスト
	TableFinder                    m_tableFinder;              //!< テーブル管理
	SettingFile                    m_settingFile;              //!< 設定ファイル
	map< CString, shared_ptr<ErrLogger> >     m_errorLogList;  //!< エラーログリスト
	CString                        m_logDir;                   //!< ログフォルダ
	CString                        m_textAddressMasterPath;    //!< マスタフォルダ
};
