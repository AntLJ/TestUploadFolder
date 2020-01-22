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
#include "OutputFormat.h"
#include "ConvertTable.h"
#include "FieldDef.h"
#include "CheckString.h"
#include "BaseRow.h"

/**
 * @brief DBレコードデータを文字列に変換するクラス
 */
class COutputString : CBaseRow
{
public:
	COutputString(){}
	COutputString(const crd_cnv& cnv, const COutputFormat& cOutputFormat, bool isZ, bool isM, esriGeometryType geomType){
		m_cOutputFormat = cOutputFormat;
		m_isZ = isZ;
		m_isM = isM;
		m_geomType = geomType;
		m_cnv = cnv;
	}
	~COutputString(){}

public:
	/**
	 * @brief DBレコードデータを文字列に変換する
	 * @param cConvTableInfo [in] 変換対象テーブル情報
	 * @param ipRow [in] 変換元レコード
	 * @param strOutput [out] 変換元データを文字列変換したもの
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool createString(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow, CString& strOutput);
	
	/**
	 * @brief 出力ファイルのヘッダー作成
	 * @param cConvTableInfo [in] 変換対象テーブル情報
	 * @param isZ [in] 形状のZ値有無
	 * @param isM [in] 形状のM値有無
	 * @param strHeader [out] 出力データのヘッダー
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool createHeader(const CConvTableInfo& cConvTableInfo, bool isZ, bool isM, CString& strHeader);

private:
	/**
	 * @brief カラムデータを文字列に変換する
	 * @param ipRow [in] 変換元レコード（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDefInfo [in] 変換対象テーブルのフィールド対応表
	 * @param vaValue [in] 変換元カラムデータ
	 * @param strColumnString [out] 変換元データを文字列変換したもの
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool value2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, const CComVariant& vaValue, CString& strColumnString);

	/**
	 * @brief 形状データの文字列変換
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param vaValue [in] 変換元形状データ
	 * @param vecShapeString [out] 文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool shape2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief ポイント形状データの文字列変換
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param vaValue [in] 形状データ
	 * @param vecShapeString [out] 文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool point2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief ポリライン形状データの文字列変換
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param vaValue [in] 形状データ
	 * @param vecShapeString [out] 文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool polyline2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	/**
	 * @brief ポリゴン形状データの文字列変換
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param vaValue [in] 形状データ
	 * @param vecShapeString [out] 文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool polygon2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString);

	bool ring2vector(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IRingPtr& ipRing, std::vector<std::vector<CString>>& vecRing);

	/**
	 * @brief ポイントデータの文字列変換（緯度経度＋ZM値）
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param ipPoint [in] 形状データ
	 * @param vecCoordStr [out] 文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool point2CoordString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IPointPtr& ipPoint, std::vector<CString>& vecCoordStr);

	/**
	 * @brief 通常フォーマットのポイントデータをWKT文字列変換（緯度経度＋ZM値）
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param vecCoordStr [in] 形状データ
	 * @param strWKTPoint [out] WKT文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	bool normal2WKTString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const std::vector<CString>& vecCoordStr, CString& strWKTPoint);

	/**
	 * @brief WKTの形状定義
	 * @param strWKTGeomType [in] WKT文字列変換後データ
	 * @retval true 処理成功
	 * @retval true 処理失敗
	 */
	CString setGeomTypeWKT(const CString& strWKTGeomType);

	/**
	 * @brief 丸め込み
	 * @param mapRoundValue [in] 丸め込み条件
	 * @param strValue [in,out] 対象データ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool roundValue(const std::map<CString,CString>& mapRoundValue, CString& strValue);

	/**
	 * @brief NULL値変換
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDef [in] フィールド対応表の情報
	 * @param strValue [in] 対象データ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool null2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, CString& strValue);

	/**
	 * @brief 整数型データ→文字列変換関数
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDefInfo [in] フィールド対応表の情報
	 * @param lValue [in] 変換対象データ
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param strColumnString [out] 出力文字列
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool numVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, long lValue, const _IRowPtr& ipRow, CString& strColumnString);
	
	/**
	 * @brief 浮動小数点型データ→文字列変換関数
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDefInfo [in] フィールド対応表の情報
	 * @param dValue [in] 変換対象データ
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param strColumnString [out] 出力文字列
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool doubleVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, double dValue, const _IRowPtr& ipRow, CString& strColumnString);

	/**
	 * @brief 文字列型データ→文字列変換関数
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDefInfo [in] フィールド対応表の情報
	 * @param strValue [in] 変換対象データ
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param strColumnString [out] 出力文字列
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool str2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, CStringW& strValue, const _IRowPtr& ipRow, CString& strColumnString);

private:
	COutputFormat m_cOutputFormat;	//!< 出力フォーマット
	bool m_isZ;						//!< Z値使用フラグ
	bool m_isM;						//!< M値使用フラグ
	esriGeometryType m_geomType;	//!< 形状タイプ
	crd_cnv m_cnv;
};

