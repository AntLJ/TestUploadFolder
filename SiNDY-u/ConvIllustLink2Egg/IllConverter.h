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

#include "common.h"
#include "PgDataBaseEx.h"
#include "IllCooker.h"

#include <sindy/schema/illust.h>

/**
 *	@class  IllConverter
 *	@brief	イラストEGG変換クラス
 */
class IllConverter 
{
public:
	/**
	 *	@brief	コンストラクタ
	 *	@param	orgDataBase [in] 変換元イラストリンクPostgreSQLDBハンドリングクラス
	 *	@param	tgtDataBase [in] 変換先PostgreSQLDBハンドリングクラス
	 *	@param	orgSchema [in] 変換元イラストリンクテーブル格納スキーマ
	 *	@param	tgtSchema [in] 変換先テーブル格納スキーマ
	 */
	IllConverter(std::shared_ptr<PgDataBaseEx> orgDataBase,
				 std::shared_ptr<PgDataBaseEx> tgtDataBase,
				 LPCTSTR orgIlSchema, 
				 LPCTSTR tgtSchema);
	/**
	 *	@brief	デストラクタ
	 */
	~IllConverter(void);

	/**
	 *	@brief イラストEGG変換関数
	 *	@param	illSetInfo	[in] イラスト処理情報設定ファイル情報
	 *	@param	inputListInfo [in] イラストファイル名対応表情報
	 *	@return true 正常終了
	 *	@return false 異常終了
	 */
	bool Convert(const std::vector<IllSetRecord>& illSetInfo, const std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo);

private:
	/**
	 *	@brief	出力テーブル作成
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool CreateIllTables();
	/**
	 *	@brief	テーブル削除関数
	 *	@param	dataBase [in] 対象DBハンドリングクラス
	 *	@param	tableName [in] 削除対象テーブル
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool DeleteTable(std::shared_ptr<PgDataBaseEx> dataBase, LPCTSTR tableName);
	/**
	 *	@brief	変換対象イラストリンクレコード群取得関数
	 *	@param	illClass	[in] 対象イラストクラス
	 *	@param	dstCode		[in] 対象仕向け先コード
	 *	@param	rows		[out] レコード群
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool GetIllLinkRows(LPCTSTR illClass, LPCTSTR dstCode, ns::RowsPtr& rows);
	/**
	 *	@brief	レコード情報→テキスト関数
	 *	@note	レコード情報をPostgreSQLのコピー文用の文字列に変換する。
	 *	@param	infIllInfos [in] inf_illustレコード情報
	 *	@param	lqIllInfos [in] lq_illustレコード情報
	 *	@param	infCopyValues [out] inf_illustコピー文用文字列
	 *	@param	lqCopyValues [out] lq_illustコピー文用文字列
	 */
	void Info2Str(const std::map<OID, InfIllInfo>& infIllInfos,
				  const std::map<LqIllKey, LqIllInfo>& lqIllInfos,
				  std::string& infCopyValues,
				  std::string& lqCopyValues);
	/**
	 *	@brief	レコードインポート処理
	 *	@param	tableName [in] 対象テーブル名
	 *	@param	fileds [in] 対象フィールド列(,区切り)
	 *	@param	imporValues [in] 登録値
	 *	@return true 成功
	 *	@return false 失敗
	 */
	bool importValues(LPCSTR tableName, LPCSTR fields, const std::string& importValues);

private:
	
	std::shared_ptr<PgDataBaseEx> m_orgDataBase;		//!< PostgreSQLDBハンドリングクラス (input イラストリンク用)
	std::shared_ptr<PgDataBaseEx> m_tgtDataBase;		//!< PostgreSQLDBハンドリングクラス (output用)

	IllCooker m_illCooker;								//!< イラストリンクレコード変換クラス

	CString m_illustLink;								//!< 変換元イラストリンクテーブル名(スキーマ+テーブル名)
	CString m_roadLink;									//!< 変換元ロードリンクテーブル名
	CString m_infIllustLink;							//!< 変換先イラスト情報テーブル名(スキーマ+テーブル名)
	CString m_lqIllustLink;								//!< 変換先イラストリンク列テーブル名(スキーマ+テーブル名)
	CString m_infFields;								//!< 変換先イラスト情報フィールド文字列(インポート時に使用)
	CString m_lqFields;									//!< 変換先イラストリンク列フィールド文字列(インポート時に使用)

};

