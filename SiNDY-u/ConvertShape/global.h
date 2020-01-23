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

#include <sindy/workspace.h>
typedef std::set<int> MESHLIST;
typedef CString ORG_COLNAME;
typedef CString DEST_COLNAME;
typedef std::pair<ORG_COLNAME, DEST_COLNAME> PAIR_COL;
typedef std::pair<DEST_COLNAME, CAdapt<IFieldPtr>> PAIR_FIELD;
typedef std::map<long, long> TRANS_VAL;				// key値をVal値に置き換える
typedef std::map<CComVariant,CComVariant> TRANS_VALUE;				// key値をVal値に置き換える

const static CString FIELD_MARK_NEW          = _T("@");
const static CString FIELD_MARK_ROUND        = _T(":");
const static CString FIELD_MARK_DESIGNATION  = _T("=");
const static CString FIELD_MARK_FIELDINFO    = _T("/");
const static CString FIELD_MARK_DEFAULTVALUE = _T("!");
const static CString FIELD_MARK_ATTR         = _T("[");

namespace field_attr_mark {
	enum ECode {
		kNull         = 0,
		kNew          = 1,
		kRound        = 2,
		kDesignation  = 3,
		kFiledInfo    = 4,
		kDefaultValue = 5,
		kAttr         = 6,
	};
}

//const static CString FIELDTYPE_SHORT  = _T("SHORT");
const static CString FIELDTYPE_LONG   = _T("LONG");
//const static CString FIELDTYPE_FLOAT  = _T("FLOAT");
const static CString FIELDTYPE_DOUBLE = _T("DOUBLE");
const static CString FIELDTYPE_STRING   = _T("STRING");
//const static CString FIELDTYPE_DATE   = _T("DATE");

namespace field_type {
	enum ECode {
		kNull      = 0,
		kShort     = 2,
		kLong      = 3,
		kFloat     = 4,
		kDouble    = 5,
		kText      = 6,
		kDate      = 7,
	};
}

const static CString FIELDINFO_TYPE      = _T("TYPE");
const static CString FIELDINFO_NAME      = _T("NAME");
const static CString FIELDINFO_SCALE     = _T("SCALE");
const static CString FIELDINFO_PRECISION = _T("PRECISION");
const static CString FIELDINFO_LENGTH    = _T("LENGTH");
const static CString FIELDINFO_NULLABLE  = _T("NULLABLE");
const static CString FIELDINFO_EDITABLE  = _T("EDITABLE");

const static CComVariant SHAPE_NUMERIC_NULL = CComVariant(-1);
const static CComVariant SHAPE_STRING_NULL = CComVariant(_T(" "));


// シェープファイルの属性と形状
struct SHAPE_INFO
{
	CString m_strSortKey;		// m_mapValue内にあるキーでソートする
	std::map<DEST_COLNAME, CString> m_mapValue;
	CAdapt<IGeometryPtr> m_ipGeom;

	// キーの値で小さい順に並べる
	bool operator < (const SHAPE_INFO& cInfo) const
	{
		std::map<DEST_COLNAME, CString>::const_iterator it1 = m_mapValue.find( m_strSortKey );
		std::map<DEST_COLNAME, CString>::const_iterator it2 = cInfo.m_mapValue.find( m_strSortKey );
		
		long lComp1 = _ttol( it1->second );
		long lComp2 = _ttol( it2->second );
		// ソートキーは、数値であることを前提とする
		if( lComp1 < lComp2 )
			return true;

		return false;
	}
};

// 変換情報
struct CONVERT_INFO
{
	bool m_bBatch;				// 一括出力かどうか（true:一括, false:メッシュ分割）
	long m_lTotalSizeDBF;		// Total dbfファイルサイズ計算用
	long m_lTotalSizeShp;		// Total shpファイルサイズ計算用
	long m_lDivideNum;			// ファイル分割数
	bool m_bIsNew;				// Newファイル(未Create)か？

	CONVERT_INFO(){
		m_bBatch = true;
		m_lDivideNum = 0;
		m_lTotalSizeDBF = 0;
		m_lTotalSizeShp = 0;
		m_bIsNew = true;
	}
};

//
//bool LoadMeshlist(CString aStrMeshlistPath, MESHLIST& r_MeshlistSet);
bool CreateMeshList(IFeatureClassPtr ipMesh, MESHLIST& setMeshlist);

//FILE* GetFile(bool bIsUTF8, CString strFile);

//CString GetTokenize(CString str, CString strToken, long& lPos);

// Shape作成Util
//bool SetField(CDBFHandle& cDBFHdl, long& lIndex, LPCTSTR lpszFieldName, DBFFieldType eFieldType, int length, int decimals);
//template <class X> bool SetFieldValue(CDBFHandle& incDBFHdl, int fid, int index, const X &xVal, LPCTSTR lpszFieldName)
//{
//	if( !incDBFHdl.write_Attribute(fid, index, xVal) )
//	{
//		CString strErrMsg;
//		strErrMsg.Format(_T("ERROR\t9001\t属性付与に失敗しました：%s"), lpszFieldName);
//		WriteLog( ETYPE_ERR, CT2CA( strErrMsg ).m_psz );
//		return false;
//	}
//	return true;
//};
//bool SetUTF8Value(CDBFHandle& cDBFHdl, int fid, int index, CString strVal, LPCTSTR lpszFieldName);

// MinMax取得
//bool GetMinMax(const IGeometryPtr ipGeom, double& dx_min, double& dy_min, double& dx_max, double& dy_max);

/**
// フィーチャクラスから、IDと形状のマップを取得する
// @param IFeatureClassPtr		ipFC		対象フィーチャクラス(in)
// @param double				dBuffer		形状のバッファ(in)
// @param std::map<long, CAdapt<IPolygonPtr>>&	mapPolygon		IDと形状のマップ(out)
//
**/
//bool GetMapGeom(IFeatureClassPtr ipFC, double dBuffer, std::map<long, CAdapt<IPolygonPtr>>& mapPolygon);

// 空間参照を作成する
//ISpatialReferencePtr CreateSpatialReference();

//bool IsTouchesEdge(const IPolylinePtr& ipBase, const IPolylinePtr& ipComp);

/**
 * @brief フィールドインデックスのマップ取得関数
 * @note フィールドインデックスとフィールド名称のマップを取得
 * @param ipTablePtr [in] フィールドインデックス取得対象テーブル
 * @param mapFieldIndex [out] フィールドインデックス
 * @retval true  処理成功
 * @retval false 処理失敗
 */
bool GetFieldIndex(ITablePtr ipTable, std::map<long,CString>& mapFieldIndex);

/**
 * @brief フィールドインデックス取得関数
 * @note フィールド名称とフィールドインデックスマップを取得
 * @param ipTablePtr [in] フィールドインデックス取得対象テーブル
 * @param mapFieldIndex [out] フィールドインデックス
 * @retval true  処理成功
 * @retval false 処理失敗
 */
bool GetFieldNameIndex(ITablePtr ipTable, std::map<CString,long>& mapFieldNameIndex);


/**
 * @brief フィールドタイプ取得関数
 * @param ipTablePtr [in] フィールドタイプ取得対象テーブル
 * @param mapFieldIndex [out] フィールドタイプ
 * @retval true  処理成功
 * @retval false 処理失敗
 */
bool GetFieldTypes(ITablePtr ipTable, std::map<CString,esriFieldType>& mapFieldType);

/**
 * @brief 文字列マッチング
 * @param strFilter [in] マッチング文字列
 * @param strCheck [in] 処理対象文字列
 * @return true マッチングOK
 * @return true マッチングNG
 */
bool IsMatch(const CString& strFilter, const CString& strCheck);

/**
 * @brief 数値かどうか（小数点もOK）
 * @param str [in] 処理対象文字列
 * @return true 数値である
 * @return true 数値ではない
 */
bool IsNumeric(const CString& str);

/**
 * @brief 整数値かどうか（小数点はNG）
 * @param str [in] 処理対象文字列
 * @return true 数値である
 * @return true 数値ではない
 */
bool IsInteger(const CString& str);
