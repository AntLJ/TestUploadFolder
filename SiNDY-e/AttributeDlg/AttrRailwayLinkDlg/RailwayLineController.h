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

#include <WinLib/grid_util.h>

#include "RelTableData.h"
#include "RelationTableHelper.h"


/// 路線リストグリッド列定義
namespace railwayline_grid {
	enum grid_cols{
		RailwayLineOid,		// RAILWAY_LINE_OID
		RailwayName,		// 路線名 
		CorpName,			// 会社名 
		col_num				// カラム数
	};
}

/// 路線検索グリッド列定義
namespace search_railway_grid {
	enum grid_cols{
		Check,				// チェックボックス
		RailwayLineOid,		// RAILWAY_LINE_OID
		RailwayName,		// 路線名 
		CorpName,			// 会社名 
		col_num				// カラム数
	};

	enum search_mode{
		kAroundSearch,		// 周辺駅の路線で検索
		kStarionNameSearch	// 駅名で検索
	};
}


/**
 * @brief 関連テーブルと鉄道ノードダイアログの2つのグリッドを制御するクラス
 * @note  汎用的な作りではなく、本ダイアログのグリッド制御に特化
 * @ntoe  関連テーブルへの制御も本クラスから行う
 */
class CRailwayLineController
{
public:
	// 駅POIの検索範囲(メートル)
	static const int SEARCH_METER = 100;

public:
	CRailwayLineController(void): 
				m_bIsChange(false), 
				m_SearchMode(search_railway_grid::kAroundSearch) 
			{};
	~CRailwayLineController(void) {};
	
	/**
	* @brief 初期化
	* @param hwnd1      [in] 路線リストグリッドコントロールのハンドル
	* @param hwnd2      [in] 周辺駅リストグリッドコントロールのハンドル
	* @param ipRow      [in] 鉄道リンクテーブルのレコード
	* @param cArcHelper [in] ArcHelper
	* @retval true  初期化成功
	* @retval false 初期化失敗
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper);

	/**
	* @brief 路線リストグリッドの表示を更新する
	*/
	void updateRailwayLineGrid();

	/**
	* @brief 路線検索リストグリッドの表示を更新する
	*/
	void updateSearchRailwayGrid();

	/**
	* @brief 周辺駅リストグリッドのチェックの入ったレコードを路線リストへ追加 
	*/
	void addCheckedData();
	
	/**
	 * @brief  指定フィーチャから一定範囲の駅ポイント情報を読み込む
	 * @param  ipFeature [in] 駅ポイント情報を検索する基準となるフィーチャ
	 * @param  ipLayer   [in] 駅ポイントレイヤ
	 */
	void searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer);

	/**
	 * @brief	指定した駅を走っている路線リストを読み込む
	 * @note    駅名は中間一致で検索
	 * @param   路線検索対象の駅名
	 */
	void searchStaName(CString strStaName);

	/**
	 * @brief	グリッドの変更点を関連テーブルに反映 
	 * @retval true   テーブルの更新成功
	 * @retval false  テーブルの更新失敗
	 */
	bool updateTable();

	/**
	 * @brief	グリットの変更箇所を色付けする 
	 */
	void drawColor();

	/**
	 * @brief	路線リストグリッドクリック時の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 * @param   bEnable   チェックボックスの編集を可能にするかどうか
	 */
	void OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable);

	/**
	 * @brief	路線リストグリッド編集前の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 * @note    クリックされた行の路線名のフィーチャをハイライト向けに格納しておく
	 */
	void OnRailwayLineGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	路線検索グリッド編集前の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 * @note    クリックされた行の路線名のフィーチャをハイライト向けに格納しておく
	 * @param   bCheckEnable   チェックボックスの編集を可能にするかどうか
	 */
	void OnSearchRailwayBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable);

	/**
	 * @brief	選択中の行を削除する
	 */
	void deleteSelectedRow();

	/**
	 *  @brief	 路線リストグリッドに変更があったか否か
	 *  @retval  true   変更あり 
	 *  @retval  false  変更なし
	 */
	bool isChange() const { return m_bIsChange; }

	/**
	 *  @brief	路線リストデータが空かどうか 
	 *  @retval true   空である
	 *  @retval false  空ではない
	 */
	bool isEmptyRelData() const { return m_RailwayLineData.empty(); };

	/**
	 *  @brief	検索モードのセット
	 *	@param	mode [in] 検索モード(周辺駅検索 or 駅名検索)
	 */
	void setSearchMode(search_railway_grid::search_mode mode){ m_SearchMode = mode; };

	/**
	 *  @brief	フィーチャーをハイライトさせる
	 */
	void highlightFeatures();
	
	/**
	 *  @brief  路線リストデータを同期する
	 *  @param  cRailwayLineCtlr   [in]  同期先のオブジェクト
	 */
	void syncRailwayLineData( CRailwayLineController& cRailwayLineCtlr );

private:

	/**
	 *  @brief	 路線検索リストグリッドからチェックの入っているレコードのID群を取得
	 *  @return  チェックが入っているレコードのID群
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 路線検索グリッドの指定行のチェック有無を取得
	 *  @param	  index  [in]  取得対象行のインデックス 
	 *  @return  チェックが入っているか否か
	 */
	bool isCheckedSearchRailwayGrid(int index) const;

	/**
	 *  @brief	 路線検索グリッドの指定行のRAILWAY_LINE_IDを取得
	 *  @param	 index  [in]  取得対象行のインデックス 
	 *  @return  RAILWAY_LINE_ID
	 */
	long getIdFromSearchRailwayGrid(int index) const;

	/**
	 *  @brief	 路線リストグリッドの指定行のRAILWAY_LINE_IDを取得
	 *  @param	 index  [in]  取得対象行のインデックス 
	 *  @return  RAILWAY_LINE_ID
	 */
	long getIdFromRailwayLineGrid(int index) const;

	/**
	 *  @brief	路線リストグリッドを変更したか否かのフラグを更新	
	 */
	void updateChangeStatus();

	/**
	 *  @brief  路線検索グリッドへ値を格納する
	 *  @param  cData [in]  値格納元の路線データ群
	 */
	void putIntoSearchGrid(const CRelTableData& cData);

	/**
	 *  @brief  指定コンテナの指定鉄道路線IDのレコードを路線リストデータコンテナへ格納する
	 *  @param  lId   [in]  鉄道路線ID
	 *  @param  cData [in]  格納元のコンテナ
	 */
	void addCheckedRecord(long lId, CRelTableData& cData);


private:

	IVSFlexGridPtr m_spRailwayLineGrid;				//!< 路線リストグリッドコントロール 
	IVSFlexGridPtr m_spSearchRailwayGrid;			//!< 路線検索グリッドコントロール 

	CRelTableData m_RailwayLineData;				//!< 路線リストデータ
	CRelTableData m_SearchFromAroundStaData;		//!< 路線検索リストデータ
	CRelTableData m_RailwayLineData_Org;			//!< 変更前の路線リストデータ
	CRelTableData m_SearchFromStationNameData;		//!< 駅名検索リストデータ

	CRelationTableHelper m_RelTableMgr;				//!< 関連テーブルの操作クラス

	CApplication  m_cArcHelper;

	search_railway_grid::search_mode m_SearchMode;	//!< 検索モード
	bool m_bIsChange;								//!< 値の変更を行ったか否か

	std::vector<CAdapt<IFeaturePtr>> m_HighlightFeatures;	//!< ハイライトさせるフィーチャー群
};
