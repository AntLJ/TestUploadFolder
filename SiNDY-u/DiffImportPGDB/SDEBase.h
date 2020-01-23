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
#include "Arguments.h"

typedef std::set<long> ID_SET;	///< ID用のセット（主にユニーク処理に使用）

/**
 * @brief 差分更新処理の基底クラス
 */
class CSDEBase
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CSDEBase(void);

	/**
	 * @brief デストラクタ。
	 */
	~CSDEBase(void);

	/**
	* @brief 全メッシュ分の処理（各処理のメイン部分）。中断に対応。
	* @param cMidSave [in] 中間保存までの処理メッシュ数。-1は保存なし（オーバーフローするくらい回せばす保存されるけど）
	* @retval true 処理成功
	* @retval false 処理失敗
	*/
	bool executeALL(int cMidSave = -1);

	/**
	 * @brief 各種初期化（クラス毎に一回限りでいいもの）。
	 * @param args [in] 実行時オプション
	 * @param ipWorkspace [in] インポート先ワークスペース
	 * @param ipSpRef [in] インポート先空間参照
	 * @param ipFeatureClasses [in] 処理対象フィーチャクラス群
	 * @retval true 成功
	 * @retval false 失敗
	 */
	virtual bool init( const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );

protected:
	CString m_ExecInfo;                //!< 処理内容（状態出力用）
	double m_MaxX;                     //!< メッシュ境界（最東経度）
	double m_MaxY;                     //!< メッシュ境界（最北緯度）
	double m_MinX;                     //!< メッシュ境界（最西経度）
	double m_MinY;                     //!< メッシュ境界（再南緯度）
	IFeatureClassPtr m_ipFeatureClass; //!< 現在処理中のフィーチャクラス
	bool m_testMode;                   //!< テストモード
	mutable CLogFile m_logFile;        //!< ログ管理クラス
	IGeometryPtr m_ipMeshGeom;         //!< 処理中のメッシュジオメトリ
	ISpatialReferencePtr m_ipSpRef;    //!< インポート先空間参照
	IWorkspaceEditPtr m_ipWorkspaceEdit; //!< インポート先ワークスペース
	FCArray m_ipFeatureClasses;          //!< 処理対象フィーチャクラス群
	bool m_sindyCImport;                 //!< インポート先がSiNDY-cスキーマかどうか

	/**
	 * @brief 編集開始関数。
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnStartEdit(LPCTSTR cMsg) const;

	/**
	 * @brief 編集終了・保存関数。
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnStopEdit(LPCTSTR cMsg) const;

	/**
	 * @brief 中間保存関数。
	 *
	 * @param cMsg	[in]	ログ出力用文字列
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnMidSave(LPCTSTR cMsg) const;

	/**
	 * @brief 処理開始のメッシュ取得（中断したらその次から）関数。
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	MESH_MAP::iterator fnGetFirstMesh(void) const;

	/**
	 * @brief メッシュ毎の各種処理関数（クラス毎の機能の違いのほとんどはここ）。
	 *
	 * @param cCommand	[in]	メッシュ毎のコマンド（追加・更新・削除：Importでしか使わない）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	virtual bool fnExecMesh(COMMAND cCommand) = 0;

	/**
	 * @brief メッシュポリゴン又はメッシュ境界ライン取得関数。
	 *　　　　境界上の厳密な比較をするための最大最小値も取得
	 *
	 * @param cGetBoundary	[in]	true:境界 false：ポリゴン
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnGetMesh(bool cGetBoundary);

	/**
	 * @brief COMのエラー出力関数。
	 *
	 */
	void fnPrintComError(void) const;

	/**
	 * @brief 共通属性部分のコピーand/orセット関数。
	 *
	 * @param ipFeature		[out]	コピー・セット対象フィーチャ
	 * @param cIndex		[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param ipSrcFeature	[in]	コピー元フィーチャ（NULLの時は初期値（固定値）をセット）
	 *
	 * @retval true		成功
	 * @retval false	失敗、エラー
	 */
	bool fnSetCommonAttr(IFeaturePtr ipFeature, long cIndex, const IFeaturePtr& ipSrcFeature) const;
	
	/**
	 * @brief 別のフィーチャから指定した属性をセットする
	 *
	 * @param ipFeature    [out] コピー・セット対象フィーチャ
	 * @param ipSrcFeature [in]  コピー元フィーチャ
	 * @param index        [in]  テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param fieldName    [in]  フィールド名
	 *
	 * @retval true  成功
	 * @retval false 失敗、エラー
	 */
	bool fnSetAttrFromFeature( IFeaturePtr ipFeature, const IFeaturePtr& ipSrcFeature, long index, const CString& fieldName ) const;

	/**
	 * @brief ログハンドル（標準タイプ）用のメッセージ出力関数。
	 *
	 * @param cType		[in]	メッセージタイプ（エラーレベル）
	 * @param cIndex	[in]	テーブルの何番目のフィーチャクラスかを示すインデックス
	 * @param cObjID	[in]	オブジェクトID
	 * @param cMsg		[in]	（エラー）メッセージ
	 *
	 */
	void fnPrintLogStyle(ErrorCode cType, int cIndex, int cObjID, LPCTSTR cMsg) const;

	/**
	 * @brief 空間検索関数。
	 *
	 * @param ipGeom		[in]	検索元のジオメトリ
	 * @param SpatialRel	[in]	空間検索のタイプ（条件）
	 * @param subFields		[in]	検索条件（SubFields）
	 *
	 * @retval NULL以外	検索結果のフィーチャカーソル
	 * @retval NULL		失敗、エラー
	 */
	IFeatureCursorPtr fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects, const CString& subFields = _T("")) const;
	
	// レコード数取得
	long fnGetSelectCountByShape( ID_SET& cIDList, const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel = esriSpatialRelIntersects );
};
