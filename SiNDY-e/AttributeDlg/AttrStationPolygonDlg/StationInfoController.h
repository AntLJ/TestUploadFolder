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

#include "RelStationSiteData.h"
#include "RelationTableHelper.h"

// WinLib
#include <WinLib/grid_util.h>


// 駅情報グリッド
namespace station_grid
{
	// 列定義
	enum grid_cols{
		GroupId,			// GROUP_ID
		StationName,		// 駅名
		col_num				// カラム数
	};

	// ヘッダー
	static const _TCHAR* HEADER    = _T("GROUPID|駅名");
}

// 駅情報検索グリッド
namespace search_station_grid
{
	// 列定義
	enum grid_cols{
		Check,				// チェックボックス
		GroupId,		 	// GROUP_ID
		StationName,		// 駅名
		col_num				// カラム数
	};

	// ヘッダー
	static const _TCHAR* HEADER = _T(" |GROUPID|駅名");
}

// クリック
namespace clicked {
	enum click{
		kLeft = 1,
		kRight = 2
	};
}


/**
 * @brief 駅情報 / 駅情報検索 の2つのグリッドが持つ情報を管理するクラス
 * @note  汎用的な作りではなく、本ダイアログのグリッド制御に特化
 * @note  本来はコントロールとデータ処理は分けるべきなのかもしれないが、
 * @ntoe  関連テーブルへの制御も本クラスから行っている
 */
class CStationInfoController
{
public:
	/**
	 *   @brief  コンストラクタ
	 */
	CStationInfoController(void): m_bIsChange(false) {};

	/**
	 *   @brief  デストラクタ
	 */
	~CStationInfoController(void) {};
	
	/**
	* @brief 初期化
	* @param hwnd1      [in] 駅情報グリッドコントロールのハンドル
	* @param hwnd2      [in] 駅情報検索グリッドコントロールのハンドル
	* @param ipRow      [in] 駅ポリゴンのレコード
	* @retval true  初期化成功
	* @retval false 初期化失敗
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow );

	/**
	* @brief 駅情報リストグリッドの表示を更新する
	*/
	void updateStationInfoGrid();

	/**
	* @brief 駅情報検索リストグリッドの表示を更新する
	*/
	void updateSearchStationInfoGrid();

	/**
	 * @brief	グリッドの変更点を関連テーブルに反映 
	 * @retval true   テーブルの更新成功
	 * @retval false  テーブルの更新失敗
	 */
	bool updateTable();

	/**
	* @brief チェックの入った駅情報検索データを駅情報データへ追加
	*/
	void addCheckedData();
	
	/**
	 * @brief	選択中の行を削除する
	 */
	void deleteSelectedRow();

	/**
	 *  @brief	 駅情報グリッドに変更があったか否か
	 *  @retval  true   変更あり 
	 *  @retval  false  変更なし
	 */
	bool isChange() const { return m_bIsChange; }

	/**
	 * @brief	指定した駅名のレコードを読み込む
	 * @note    駅名は中間一致で検索
	 * @param   strStationName [in]  検索対象の駅名
	 */
	void searchStationName( const CString& strStationName );


	/// グリッドのイベント処理の引き受けるための関数

	/**
	 * @brief	駅情報グリッドクリック時の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 * @param   bEnable   チェックボックスの編集を可能にするかどうか
	 */
	void OnStationInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable);

	/**
	 * @brief	駅情報グリッド編集前の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 */
	void OnStationInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	駅情報検索グリッド編集前の処理
	 * @note    属性ダイアログクラスのイベント処理を委譲する
	 * @param   bCheckEnable   チェックボックスの編集を可能にするかどうか
	 */
	void OnSearchStationInfoBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable);


private:

	/**
	 *  @brief	 駅情報検索リストグリッドからチェックの入っているレコードのID群を取得
	 *  @return  チェックが入っているレコードのID群
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 駅情報検索グリッドの指定行のチェック有無を取得
	 *  @param	  index  [in]  取得対象行のインデックス 
	 *  @return  チェックが入っているか否か
	 */
	bool isCheckedSearchStationGrid(int index) const;

	/**
	 *  @brief	 駅情報検索グリッドの指定行のグループIDを取得
	 *  @param	 index  [in]  取得対象行のインデックス 
	 *  @return  グループID
	 */
	long getGroupIdFromSearchStationGrid(int index) const;

	/**
	 *  @brief	 駅情報グリッドの指定行のグループIDを取得
	 *  @param	 index  [in]  取得対象行のインデックス 
	 *  @return  グループID
	 */
	long getGroupIdFromStationGrid(int index) const;

	/**
	 *  @brief	駅情報グリッドを変更したか否かのフラグを更新	
	 */
	void updateChangeStatus();

	/**
	 * @brief	グリットの変更箇所を色付けする 
	 */
	void drawColor();


private:

	IVSFlexGridPtr m_spStationGrid;					//!< 駅情報グリッドコントロール 
	IVSFlexGridPtr m_spSearchStationGrid;			//!< 駅情報検索グリッドコントロール 

	CRelStationSiteData m_StationInfoData;			//!< 駅情報データ
	CRelStationSiteData m_StationInfoData_Org;		//!< 変更前の駅情報データ
	CRelStationSiteData m_SearchStationNameData;	//!< 駅名検索データ

	CRelationTableHelper m_RelTableMgr;				//!< 関連テーブルの操作クラス

	bool m_bIsChange;								//!< 値の変更を行ったか否か

};
