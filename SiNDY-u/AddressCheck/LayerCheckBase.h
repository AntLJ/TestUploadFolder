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

#include "GlobalFunc.h"
#include "TableFinder.h"
#include "SettingFile.h"
#include "Logger.h"
#include "CommonCheck\RelationCheck.h"

/**
 * @class	LayerCheckBase
 * @brief	チェック基底クラス
 */
class LayerCheckBase
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	LayerCheckBase(void):m_curMeshCode(0L){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~LayerCheckBase(void){}

	/**
	 * @brief  デストラクタ
	 */
	friend class RelationCheck;
	/**
	 * @brief	レイヤ名取得
	 * @return	レイヤ名
	 */
	LPCTSTR getLayerCheckName(){ return getName(); }

	/**
	 * @brief	初期化
	 * @param	[in] settingFile     設定ファイル
	 * @param	[in] tableFinder     テーブル管理
	 * @param	[in] errorLog        エラーファイル管理
	 * @retval	true:成功
	 * @retval	false:失敗
	 */
	bool init( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster ){ return initialize( settingFile, tableFinder, errorLog, textAddressMaster ); }

	/**
	 * @brief	チェック
	 * @param	[in] ulMeshCode			メッシュコード
	 * @param	[in] ipMeshGeometry		メッシュジオメトリ
	 */
	void checkLayer( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID ){ check( ulMeshCode, ipMeshGeometry, meshID ); }

private:

	/**
	 * @brief	レイヤ名取得
	 * @return	レイヤ名
	 */
	virtual LPCTSTR getName() = 0;

	/**
	 * @brief	初期化
	 * @param	[in] settingFile     設定ファイル
	 * @param	[in] tableFinder     テーブル管理
	 * @param	[in] errorLog        エラーファイル管理
	 * @param	[in] textAddressMaster  住所マスタデータパース
	 * @retval	true:成功
	 * @retval	false:失敗
	 */
	virtual bool initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster ) = 0;

	/**
	 * @brief	チェック
	 * @param	[in] ulMeshCode			メッシュコード
	 * @param	[in] ipMeshGeometry		メッシュジオメトリ
	 */
	virtual void check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID ) = 0;

public:

	ErrLogger		   *m_errorLog;		    //!< エラーログ
	unsigned long	   m_curMeshCode;       //!< 現在チェックしているメッシュコード
	std::set<ErrorIdInfo>   m_errorIdSet;   //!< エラー情報セット
};
