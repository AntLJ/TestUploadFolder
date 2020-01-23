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

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

/**
 * @brief  駅ポリゴン関連チェッカークラス
 * @detail 実際のチェックロジックを持つクラス
 */
class StationChecker
{
public:
	StationChecker(void){}
	~StationChecker(void){}

	/**
	 * @brief  初期化
	 * @param  db_info   [in]  DB接続情報
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool init( const sindy::CWorkspace& workspace );

	/**
	 * @brief  駅情報紐付きチェック
	 * @param  row   [in]  チェック対象オブジェクト
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkLinkedStation( const sindy::CRowContainer& row, const CString& table_name ) const;

	/**
	 * @brief  ポリゴン種別チェック
	 * @param  row   [in]  チェック対象オブジェクト
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkPolygonC( const sindy::CRowContainer& row ) const;

	/**
	 * @brief  ポリゴン包括チェック
	 * @param  row            [in]  チェック対象オブジェクト
	 * @param  outer_FcName [in]  包括する側(外側)のフィーチャクラス名
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkContain( const sindy::CRowContainer& row, const CString& outer_fc_name ) const;

	/**
	 * @brief  ポリゴン包括チェック(駅ID考慮)
	 * @param  row            [in]  チェック対象オブジェクト
	 * @param  outer_FcName [in]  包括する側(外側)のフィーチャクラス名
	 * @return エラーコード
	 */
	bool checkContainedBySameStationPolygon( const sindy::CRowContainer& row, const CString& outer_fc_name ) const;

	/**
	 * @brief  駅ポリゴンとの紐付き正当性チェック
	 * @param  row            [in]  チェック対象オブジェクト
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkLinkedNotExistPolygon( const sindy::CRowContainer& row );

	/**
	 * @brief  駅との紐付き正当性チェック
	 * @param  row            [in]  チェック対象オブジェクト
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkLinkedNotExistStation( const sindy::CRowContainer& row );

private:

	/**
	 * @brief  テーブルファインダの初期化
	 * @param  workspace  [in]  ワークスペース
	 * @retval true  初期化成功
	 * @retval false 初期化失敗
	 */
	bool initTableFinder( const sindy::CWorkspace& workspace );

	/**
	 * @brief  SJ駅ポリゴン種別コードのコード値ドメイン読み込み
	 * @retval true  読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool readCodeValueDomain();

	/**
	 * @brief  SJ駅ポリゴンのOID群を読み込み
	 * @retval true  読み込み成功
	 * @retval false 読み込み失敗
	 */
	void readStationPolygonOid();

	/**
	 * @brief  駅IDの読み込み
	 * @retval true  読み込み成功
	 * @retval false 読み込み失敗
	 */
	void readStationIds();

	/**
	 * @brief  指定のフィーチャ(row)が指定のフィーチャクラス(outer_fc_name)のポリゴンに包括されているかをチェックする
	 * @detail OIDリスト(oids) が指定されている場合は、指定OIDのポリゴンに指定フィーチャ(row)包括するものがあるかをチェックする。
	 * @detail OIDリスト(oids) が未指定(空)であれば、OIDを限定せずに指定フィーチャ(row)包括するものがあるかをチェックする。
	 * @param row   [in] チェック対象フィーチャ
	 * @param table [in] 包括する側のフィーチャクラス名
	 * @param oids  [in] 包括する側のOID
	 * @retval true  問題なし
	 * @retval false 問題あり
	 */
	bool checkContainedByFeatureClass( const sindy::CRowContainer& row, const CString& outer_fc_name, const std::set<long>& oids ) const;

private:

	sindy::CTableFinder m_TableFinder;	//!< テーブルファインダ

	std::set<long>  m_CVDomains;		//!< ポリゴン種別のコード値ドメイン

	std::unordered_multimap<long, long> m_OidList;	//!< 駅ポリゴンのOIDのリスト ( Key: レイヤ番号, Value: OID )
	std::set<long>  m_StationIdList;				//!< 駅IDのリスト
};

typedef std::shared_ptr<StationChecker> SPStationChecker;
