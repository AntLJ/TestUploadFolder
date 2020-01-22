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

#include <TDC/useful_headers/str_util.h>

/**
 * @brief フィールドインデックスマップ
 */
typedef std::map< CString, long > FieldIndexes;

/**
 * @brief 区切り文字（文字列内）
 */
const CString SEPARATOR_FULL_PIPE = _T("｜");
const CString SEPARATOR_HALF_SLASH = _T("/");

/**
 * @brief   建物ビル名称ポイントコンテナ
 */
struct BldNamePointRec
{
public:
	/**
	 * @brief コンストラクタ
	 */
	BldNamePointRec() : bldNamePointOID( 0 ), bldgOID( 0 ), contentsCode( 0 ), meshcode( 0 ), x( 0 ), y( 0 ) {}

	/**
	 * @brief デストラクタ
	 */
	virtual ~BldNamePointRec() {}

	long bldNamePointOID;     //!< 建物ビル名称ポイントOID
	long bldgOID;             //!< 建物ポリゴンOID
	CString name;             //!< 名称
	CString nameSep;          //!< 名称（分割）
	CString yomiSep;          //!< 読み（分割）
	CString bldgClass1;       //!< 建物種別コード1
	CString floors;           //!< 階数
	long contentsCode;        //!< 注記POIコンテンツコード
	CString suitable;         //!< ビル名称妥当フラグ
	CString addrcode;         //!< 住所コード
	int meshcode;             //!< 2次メッシュコード
	int x;                    //!< X座標
	int y;                    //!< Y座標

	/**
	 * @brief   読み（分割文字除去）取得
	 * @return  読み（分割文字除去後）
	 */
	const CString getRemovedSepStrYomi() { return uh::str_util::replace(yomiSep, SEPARATOR_FULL_PIPE, _T("") ); };

	/**
	 * @brief   名称（分割文字置換）取得
	 * @return  名称（分割文字置換後）
	 */
	const CString getReplacedSepName() { return uh::str_util::replace(nameSep, SEPARATOR_FULL_PIPE, SEPARATOR_HALF_SLASH ); };

	/**
	 * @brief   読み（分割文字置換）取得
	 * @return  読み（分割文字置換後）
	 */
	const CString getReplacedSepYomi() { return uh::str_util::replace(yomiSep, SEPARATOR_FULL_PIPE, SEPARATOR_HALF_SLASH ); };

	/**
	 * @brief   注記POIデータ分類コード取得
	 * @note    コンテンツコードの上位桁（下から6桁目以上）を文字列で返す
	 * @note    コンテンツコードが 0 の場合は空文字列を返す
	 * @return  データ分類コード
	 */
	const CString getGrpCode() { return ( 0 != contentsCode ) ? uh::str_util::ToString( static_cast<long>( contentsCode / 100000 ) ) : _T(""); }

	/**
	 * @brief   注記POI店舗コード取得
	 * @note    コンテンツコードの下位桁（下から5桁目まで）を文字列で返す
	 * @note    コンテンツコードが 0 の場合は空文字列を返す
	 * @return  店舗コード
	 */
	const CString getChainCode() { return ( 0 != contentsCode ) ? uh::str_util::ToString( contentsCode % 100000 ) : _T(""); }
};

namespace common_proc
{
	/**
	 * @brief ワークスペースの取得
	 * @param strConnect [in] ワークスペース名（DB名）
	 * @param ipWorkspace [out] ワークスペース
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getWorkspace( const CString& strConnect, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief フィーチャクラスの取得
	 * @param strName [in] テーブル名
	 * @param ipWorkspace [in] ワークスペース
	 * @param ipFeatureClass [out] フィーチャクラス
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getFeatureClass( const CString& strName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass );

	/**
	 * @brief テーブルの取得
	 * @param strName [in] テーブル名
	 * @param ipWorkspace [in] ワークスペース
	 * @param ipTable [out] テーブル
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @breif フィールドインデックスマップの取得
	 * @param ipTable [in] 対象テーブル
	 * @param mapIndexes [out] フィールドインデックスマップ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getFieldIndexes( const ITablePtr& ipTable, FieldIndexes& mapIndexes );

	/**
	 * @brief フィールドの取得
	 * @param ipTable [in] 対象テーブル
	 * @param strFieldName [in] 対象フィールド名
	 * @param ipField [out] フィールド
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getField( const ITablePtr& ipTable, const CString& strFieldName, IFieldPtr& ipField );

	/**
	 * @brief フィールド値の取得
	 * @param ipRow [in] 対象レコード
	 * @param strFieldName [in] 対象フィールド名
	 * @param mapIndexes [in] フィールドインデックスマップ
	 * @param varValue [out] 取得した値
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getValue( const _IRowPtr& ipRow, const CString& strFieldName, FieldIndexes& mapIndexes, CComVariant& varValue );

	/**
	 * @breif コード値ドメインで定義されているコード値のセットを取得
	 * @note ここでのコード値ドメインは整数型のみを対象とする
	 * @param ipField [in] 対象フィールド
	 * @param mapValues [out] コード値ドメインで定義されているコード値と説明のマップ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getCodedValues( const IFieldPtr& ipField, std::map< long, CString >& mapValues );

	/**
	 * @breif コード値ドメインで定義されているコード値のセットを取得
	 * @note ここでのコード値ドメインは整数型のみを対象とする
	 * @param ipTable [in] 対象テーブル
	 * @param strFieldName [in] 対象フィールド名
	 * @param mapValues [out] コード値ドメインで定義されているコード値と説明のマップ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::map< long, CString >& mapValues );

	/**
	 * @brief Where句による検索
	 * @param ipFeatureClass [in] 検索対象フィーチャクラス
	 * @param ipFeatureCursor [out] フィーチャカーソル（検索結果）
	 * @param lCount [out] 検索フィーチャ数
	 * @param strWhere [in] Where句
	 * @param strPostfix [in] 接尾句（「ORDER BY 句」など）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool searchByWhereClause( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const CString& strWhere = _T(""), const CString& strPostfix = _T("") );

	/**
	 * @brief 形状による空間検索
	 * @param ipFeatureClass [in] 検索対象フィーチャクラス
	 * @param ipFeatureCursor [out] フィーチャカーソル（検索結果）
	 * @param lCount [out] 検索フィーチャ数
	 * @param ipGeometry [in] 検索元形状
	 * @param spRel [in] 検索対象と検索元形状の空間的関係
	 * @param strPostfix [in] 接尾句（「ORDER BY 句」など）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool searchByGeometry( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& strPostfix = _T("") );

	/**
	 * @brief   文字コード変換（EUC→SJIS）
	 * @param   str [in] 変換対象文字列(EUC)
	 * @return  変換済み文字列(SJIS)
	 */
	std::string convEUC2SJIS( const std::string& str );

	/**
	 * @brief   文字コード変換（SJIS→EUC）
	 * @param   str [in] 変換対象文字列(SJIS)
	 * @return  変換済み文字列(EUC)
	 */
	std::string convSJIS2EUC( const std::string& str );

} // namespace common_proc
