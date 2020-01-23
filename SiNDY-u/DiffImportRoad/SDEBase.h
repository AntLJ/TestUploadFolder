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
#include "Reflect.h"

typedef std::set<long> ID_SET;	///< ID用のセット（主にユニーク処理に使用）

/**
 * @brief 差分更新処理の基底クラス
 */
class CSDEBase
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CSDEBase(void);

	/**
	 * @brief デストラクタ
	 */
	~CSDEBase(void);

	/**
	 * @brief 全メッシュ分の処理（各処理のメイン部分）。中断に対応
	 */
	bool executeALL(int cMidSave = -1);

	/**
	 * @brief 各種初期化（クラス毎に一回限りでいいもの）
	 */
	virtual bool init(void);

protected:
	enum REF_LAYER {
		TGT_LAYER = 0,
		FIX_LAYER = 1,
		PGDB_LAYER = 2
	};
	CLayerManage* m_LayerManage;
	CString m_ExecInfo;		///< 処理内容（状態出力用）
	double m_MaxX;			///< メッシュ境界（最東経度）
	double m_MaxY;			///< メッシュ境界（最北緯度）
	double m_MinX;			///< メッシュ境界（最西経度）
	double m_MinY;			///< メッシュ境界（再南緯度）
	ITablePtr m_ipTable[2];	///< 現在処理中のテーブル（フィーチャクラス）[1]がLQ用[0]はその他
	IPolygonPtr m_ipMeshPoly;	///< メッシュポリゴン
	IGeometryPtr m_ipMeshBoundary;	///< メッシュポリゴン（枠）

	/**
	 * @brief 編集開始関数
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnStartEdit(LPCTSTR cMsg) const;

	/**
	 * @brief 編集終了・保存関数
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnStopEdit(LPCTSTR cMsg) const;


	/**
	 * @brief 編集終了・破棄関数
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnAbortEdit(LPCTSTR cMsg) const;

	/**
	 * @brief 中間保存関数
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMidSave(LPCTSTR cMsg) const;

	/**
	 * @brief 処理開始のメッシュ取得（中断したらその次から）関数
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	MESH_SET::iterator fnGetFirstMesh(void) const;

	/**
	 * @brief メッシュ毎の各種処理関数（クラス毎の機能の違いのほとんどはここ）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	virtual bool fnExecMesh(void) = 0;

	/**
	 * @brief メッシュポリゴン及びメッシュ境界ライン取得関数
	 *　　　　境界上の厳密な比較をするための最大最小値も取得
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnGetMesh(void);

	/**
	 * @brief COMのエラー出力関数
	 *
	 */
	void fnPrintComError(void) const;

	/**
	 * @brief 共通属性部分のコピーand/orセット関数
	 *
	 * @param ipFeature			[out]	コピー・セット対象フィーチャ（DB側）
	 * @param cSDEIndexMap		[in]	SDEのフィールド名とフィールドインデックスの対応マップ
	 * @param cPGDBIndexMap		[in]	PGDBのフィールド名とフィールドインデックスの対応マップ
	 * @param ipSrcFeature		[in]	コピー元フィーチャ（PGDB側）（NULLの時は初期値（固定値）をセット）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnSetCommonAttr(_IRowPtr& ipRow, INDEX_MAP& cSDEIndexMap, INDEX_MAP& cPGDBIndexMap, const _IRowPtr& ipSrcRow) const;

	/**
	 * @brief ログハンドル（標準タイプ）用のメッセージ出力関数
	 *
	 * @param cType			[in]	メッセージタイプ（エラーレベル）
	 * @param cLayerName	[in]	レイヤ名
	 * @param cObjID		[in]	オブジェクトID
	 * @param cMsg			[in]	（エラー）メッセージ
	 * @param cRefLayer		[in]	対象レイヤを示すタイプ
	 * @param ipGeom		[in]	座標出力用ジオメトリ
	 *
	 */
	void fnPrintLogStyre(int cType, LPCTSTR cLayerName, int cObjID, LPCTSTR cMsg, LPCTSTR cMsg2, REF_LAYER cRefLayer, IGeometryPtr ipGeom=NULL) const;

	/**
	 * @brief 空間検索関数
	 *
	 * @param ipGeom		[in]	検索元のジオメトリ
	 * @param SpatialRel	[in]	空間検索のタイプ（条件）
	 *
	 * @retval NULL以外	検索結果のフィーチャカーソル
	 * @retval NULL		失敗、エラー
	 */
	IFeatureCursorPtr fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects) const;
};
