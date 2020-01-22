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
namespace lineinfo_grid {
	enum grid_cols{
		JoinLineInfoOid,	// JOIN_LINE_INFO_OID
		StationName,		// 駅名 
		RailwayName,		// 路線名 
		StationNumber,		// 駅番号 
		col_num				// カラム数
	};
}

/// 周辺駅リストグリッド列定義
namespace around_sta_grid {
	enum grid_cols{
		Check,				// チェックボックス
		JoinLineInfoOid,	// JOIN_LINE_INFO_OID
		StationName,		// 駅名 
		RailwayName,		// 路線名 
		col_num				// カラム数
	};
}


/**
 * @brief 鉄道ノードダイアログの2つのグリッドを制御するクラス
 * @note  本ダイアログのグリッド制御に特化
 * @ntoe  関連テーブルへの制御も本クラスから行う
 */
class CLineInfoController
{
public:
	// 駅POIの検索範囲(メートル)
	static const int SEARCH_METER = 100;


public:
	CLineInfoController(void): m_bIsChange(false) {};
	~CLineInfoController(void) {};
	
	/**
	* @brief 初期化
	* @param hwnd1      [in] 路線リストグリッドコントロールのハンドル
	* @param hwnd2      [in] 周辺駅リストグリッドコントロールのハンドル
	* @param ipRow      [in] 鉄道ノードテーブルのレコード
	* @param cArcHelper [in] ArcHelper
	* @retval true   初期化成功
	* @retval false  初期化失敗
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper);

	/**
	* @brief 路線リストグリッドの表示を更新する
	*/
	void updateLineInfoGrid();

	/**
	* @brief 周辺駅リストグリッドの表示を更新する
	*/
	void updateAroundStaGrid();

	/**
	* @brief 周辺駅リストグリッドのチェックの入ったレコードを路線リストへ追加 
	*/
	void addCheckedRecord();
	
	/**
	 * @brief	駅情報を検索しグリッドに表示 
	 */
	void searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer);

	/**
	 * @brief	グリッドの変更点を関連テーブルに反映 
	 */
	bool updateTable();

	/**
	 * @brief	グリットの変更箇所を色付けする 
	 */
	void drawColor();

	/**
	 * @brief	グリッドを編集不可にする 
	 */
	void disable();

	/**
	 * @brief	路線リストグリッドクリック時の処理
	 * @param   子ダイアログのイベント処理を委譲する
	 */
	void OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId);

	/**
	 * @brief	路線リストグリッド編集前の処理
	 * @param   子ダイアログのイベント処理を委譲する
	 */
	void OnLineInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	路線リストグリッド編集後の処理
	 * @param   子ダイアログのイベント処理を委譲する
	 */
	void OnLineInfoGridAfterEdit(long Row, long Col);

	/**
	 * @brief	周辺駅リストグリッド編集前の処理
	 * @param   子ダイアログのイベント処理を委譲する
	 */
	void OnAroundStaBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	選択中の行を削除する
	 * @note    現在選択されている行に紐付くCRowを削除します
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
	bool isEnptyRelData() const { return m_LineInfoData.empty(); };


private:

	/**
	 *  @brief	「周辺駅リスト」リストビューからチェックの入っているレコードのID群を取得
	 *  @return  チェックが入っているレコードのID群
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 周辺駅グリッドの指定行のチェック有無を取得
	 *  @param	 取得対象行のインデックス 
	 *  @return  チェックが入っているか否か
	 */
	bool isCheckedAroundStaGrid(int index) const;

	/**
	 *  @brief	 周辺駅グリッドの指定行のJOIN_INFO_IDを取得
	 *  @param	 取得対象行のインデックス 
	 *  @return  JOIN_INFO_ID
	 */
	long getIdFromAroundStaGrid(int index) const;

	/**
	 *  @brief	 路線リストグリッドの指定行のJOIN_INFO_IDを取得
	 *  @param	 取得対象行のインデックス 
	 *  @return  JOIN_INFO_ID
	 */
	long getIdFromLineInfoGrid(int index) const;

	/**
	 *  @brief	 路線リストグリッドの指定行の駅番号を取得
	 *  @param	 取得対象行のインデックス 
	 *  @return  駅番号
	 */
	CString getStaNumFromLineInfoGrid(int index) const;

	/**
	 *  @brief	路線リストグリッドを変更したか否かのフラグを更新	
	 */
	void updateChangeStatus();


private:

	IVSFlexGridPtr m_spLineInfoGrid;	//!< 路線リストグリッドコントロール 
	IVSFlexGridPtr m_spAroundStaGrid;	//!< 周辺駅グリッドコントロール 

	CRelTableData m_LineInfoData;		//!< 路線リストデータ
	CRelTableData m_AroundStaData;		//!< 周辺駅リストデータ
	CRelTableData m_LineInfoData_Org;	//!< 変更前の路線リストデータ

	CRelationTableHelper m_RelTableMgr;	//!< 関連テーブルの操作クラス

	CApplication  m_cArcHelper;

	bool m_bIsChange;					//!< 値の変更を行ったか否か

};

