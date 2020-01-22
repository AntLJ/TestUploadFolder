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


// ツール名
const LPCTSTR TOOLNAME = _T("BldNameBaseDBCreator");

// 引数
const LPCSTR INI		= "ini";
const LPCSTR PROCLOG	= "proc_log";
const LPCSTR ERRLOG		= "err_log";
const LPCSTR UPDATE		= "u";
const LPCSTR SURVEY		= "s";

// 設定ファイル項目
const LPCSTR SRC_DB				= "SRC_DB";
const LPCSTR MAP_DB				= "MAP_DB";
const LPCSTR ADDR_DB			= "ADDR_DB";
const LPCSTR TWN_DB				= "TWN_DB";
const LPCSTR POI_DB				= "POI_DB";
const LPCSTR BASE_DB			= "BASE_DB";
const LPCSTR MESH_DB			= "MESH_DB";
const LPCSTR AM_DB				= "AM_DB";
const LPCSTR SURVEY_LIST		= "SURVEY_LIST";
const LPCSTR TGT_ADDR_LIST		= "TGT_ADDR_LIST";
const LPCSTR SRC_PRI_LIST_PRIOR	= "SRC_PRI_LIST_PRIOR";
const LPCSTR SRC_PRI_LIST_MATCH	= "SRC_PRI_LIST_MATCH";
const LPCSTR POI_PRI_LIST		= "POI_PRI_LIST";
const LPCSTR SUP_POI_PRI_LIST	= "SUP_POI_PRI_LIST";
const LPCSTR BLD_EXC_LIST		= "BLD_EXC_LIST";
const LPCSTR REPLACE_LIST		= "REPLACE_LIST";
const LPCSTR COND_LIST			= "COND_LIST";
const LPCSTR TWN_TBL_NAME		= "TWN_TBL_NAME";
const LPCSTR SOURCENAME			= "SOURCENAME";
const LPCSTR SOURCEDATE			= "SOURCEDATE";

// インポート処理単位
const long FETCHNUM = 1000;

// 閉鎖理由コード「休止中」
const LPCSTR CLOSE_SUSPENDED	= "b";

/**
 * @brief 建物名称ポイントレコード登録情報
 */
struct BldNamePtInfo
{
	CString e_name;			//!< 名称
	long e_bldOID;			//!< 建物ポリゴンID
	long e_srcType;			//!< 出典コード
	CString e_srcName;		//!< 出典元（付加情報との紐付けに使用）
	CString e_srcID;		//!< 出典資料ID（付加情報との紐付けに使用）
	long e_srcOID;			//!< 参照素材OID
	CString e_surveyDate;	//!< 調査日（YYYY/MM/DD HH:MI:SS 形式？） … 適当な日付型にしたほうがいいかも
	CString e_anyPlaceID;	//!< AnyPlace ID
	long e_priority;		//!< 信頼度コード
	double e_matchPer;		//!< マッチング率（DB格納時に整数に変換）
	CAdapt<IGeometryPtr> e_ipGeometry;	//!< 形状

	BldNamePtInfo() : e_bldOID(-1), e_srcType(-1), e_srcOID(-1), e_priority(-1), e_matchPer(-1) {}

	/**
	 * @brief リセット
	 */
	void Reset() {
		e_name.Empty();
		e_bldOID		= -1;
		e_srcType		= -1;
		e_srcOID		= -1;
		e_surveyDate.Empty();
		e_anyPlaceID.Empty();
		e_priority		= -1;
		e_matchPer		= -1;
		e_ipGeometry	= nullptr;
	}

	/**
	 * @brief  出典資料IDからコンテンツコードを取得
	 * @note   (例)「2335-1137-543」 → 「233501137」
	 * @return コンテンツコード（注記POI由来でない場合や取得できない場合は「-1」）
	 */
	long GetContentsCode() const;

	/**
	 * @brief  出典資料IDからコンテンツ内シーケンスを取得
	 * @note   (例)「2335-1137-543」 → 「543」
	 * @return コンテンツ内シーケンス（注記POI由来でない場合や取得できない場合は「-1」）
	 */
	long GetContentsSeq() const;

private:
	/**
	 * @brief  出典資料IDを「-」でトークンに分割
	 * @note   (例)「2335-1137-543」 → 「2355」「1137」「543」
	 * @param  token		[out] トークン
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool TokenizeSrcID( std::vector< long > &tokens ) const;
};

/**
 * @brief 建物名称付加情報
 */
struct BldNameAddInfo
{
	CString	e_name;			//!< 名称
	CString	e_nameSep;		//!< 名称（分割）
	CString	e_yomiSep;		//!< ヨミ（分割）
	long	e_bldgClass1;	//!< 建物種別コード1
	long	e_bldgClass2;	//!< 建物種別コード2
	long	e_bldgClass3;	//!< 建物種別コード3
	long	e_floors;		//!< 階数
	long	e_suitable;		//!< ビル名称妥当フラグ
	long	e_fixclass;		//!< 種別確定コード

	BldNameAddInfo() : e_bldgClass1(-1), e_bldgClass2(-1), e_bldgClass3(-1), e_floors(-1), e_suitable(0), e_fixclass(0) {}
	~BldNameAddInfo() {}
};

/**
 * @brief 置換文字列リストレコード
 */
struct StrPair
{
	CString e_orgStr;
	CString e_replaceStr;

	StrPair(CString orgStr, CString replaceStr) : e_orgStr(orgStr), e_replaceStr(replaceStr) {}
	StrPair() {}
};

/**
 * @brief IDをキーとしたフィーチャのマップ
 */
typedef std::map<long, CAdapt<IFeaturePtr>> IDFeatureMap;

/**
 * @brief 共通関数
 */
namespace common_proc {

	/**
	 * @brief  フィールド値取得関数
	 * @param  ipRow		[in]  IRowPtr
	 * @param  layerName	[in]  テーブル名(エラー出力用)
	 * @param  fieldName	[in]  フィールド名
	 * @param  indeces		[in]  フィールドインデックス情報
	 * @param  val			[out] 取得した値
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetValue(_IRowPtr ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, CComVariant* val);

	/**
	 * @brief  フィールド値セット関数
	 * @param  ipBuffer		[in]  IBufferPtr
	 * @param  tableName	[in]  テーブル名(エラー出力用)
	 * @param  fieldName	[in]  フィールド名
	 * @param  indeces		[in]  フィールドインデックス情報
	 * @param  val			[in]  セットする値
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SetValue(IRowBufferPtr ipBuffer, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, const CComVariant& val);

	/**
	 * @brief  優先フィーチャ取得関数
	 * @note　 名称文字列を持つ複数フィーチャのうち優先度の高いフィーチャを取得する関数
	 * @param  tgtFeatures		[in]  フィーチャ群
	 * @param  tableName		[in]  テーブル名
	 * @param  fieldName		[in]  フィールド名
	 * @param  indeces			[in]  フィールドインデックス情報
	 * @param  headID			[out] 選ばれたフィーチャのOBJECTID
	 * @param  ipHeadFeature	[out] 選ばれたフィーチャ
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SelectHeadNameFeature(const IDFeatureMap& tgtFeatures, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, long* headID, IFeaturePtr& ipHeadFeature);

	/**
	 * @brief  Where句による検索関数（フィーチャクラス）
	 * @param  ipFeatureClass	[in]  検索対象フィーチャクラス
	 * @param  ipFeatureCursor	[out] フィーチャカーソル（検索結果）
	 * @param  whereClause		[in]  Where句
	 * @param  bRecycling		[in]  リサイクリングするかどうか
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SearchByWhereClause(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const CString& whereClause = _T(""), bool bRecycling = false);

	/**
	 * @brief  Where句による検索関数（テーブル）
	 * @param  ipTable			[in]  検索対象テーブル
	 * @param  ipCursor			[out] フィーチャカーソル（検索結果）
	 * @param  whereClause		[in]  Where句
	 * @param  bRecycling		[in]  リサイクリングするかどうか
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SearchByWhereClause(ITablePtr ipTable, _ICursorPtr& ipCursor, const CString& whereClause = _T(""), bool bRecycling = false);

	/**
	 * @brief  Geometryによる空間検索関数
	 * @param  ipFeatureClass	[in]  検索対象フィーチャクラス
	 * @param  ipFeatureCursor	[out] フィーチャカーソル（検索結果）
	 * @param  ipGeometry		[in]  検索元Geometry
	 * @param  spRel			[in]  検索対象の検索元Geometryとの関係
	 * @param  bRecycling		[in]  リサイクリングするかどうか
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool SearchByGeometry(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, bool bRecycling = false);

	/**
	 * @brief  コード値ドメインの取得（整数値）
	 * @param  ipTable		[in]  対象テーブル
	 * @param  layerName	[in]  テーブル名(エラー出力用)
	 * @param  fieldName	[in]  フィールド名
	 * @param  indeces		[in]  フィールドインデックス情報
	 * @param  codedValues  [out] コード値のリスト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool GetCodedValues(const ITablePtr& ipTable, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, std::set<long>& codedValues);
};
