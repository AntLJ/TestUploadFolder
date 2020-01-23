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

// ErrorHandle.cpp: CErrorHandle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorHandle.h"


const LPCTSTR CErrorHandle::m_lpszErrMsg[] = 
{
	_T("\t\t"),	// 不明なエラーコード用
	_T("WARNING\tW1\t11桁コード不一致(未整備)"),
	_T("WARNING\tW2\t11桁コード不一致(整備済)"),
	_T("WARNING\tW3\t8桁コード不一致"),
	_T("WARNING\tW4\t5桁コード不一致"),
	_T("NOTICE\tN5\t保留である"),
	_T("ERROR\tE6\t調査結果コード属性付与漏れ"),
	_T("ERROR\tE7\t不明の調査コード"),
	_T("ERROR\tE8\t行政界の境界上にあるまたは行政界上にない"),
	_T("WARNING\tW9\t電話番号がフリーダイヤル"),
	_T("ERROR\tE10\t位置不正（都市地図背景 : %s）"),
	_T("ERROR\tE11\t位置不正（中縮背景 : %s）"),
	_T("ERROR\tE12\t都市地図背景の境界上にあるのでチェックできません"),
	_T("ERROR\tE13\t中縮背景の境界上にあるのでチェックできません"),
	_T("ERROR\tE14\tポイント郡で調査結果コードが違うポイントがある"),
	_T("ERROR\tE15\t使用不可POI付近に整備"),							// Bug 5701対応 07/11/30追加
	_T("ERROR\tE16\t同一シーケンス複数有り")							// Bug 9295対応 12/09/20追加
};

// ログ出力
void CErrorHandle::printLog( const CTelTP& ob, int nErrorCode, const std::string* lpstrBGDomain /* = NULL */ )
{
	// 出力開始  「住所名」「20桁住所コード」「方書名称」「掲載名称」「調査結果」「情報ソース」「電話番号」「座標付与種別」
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< getErrorMsg( nErrorCode, lpstrBGDomain ) << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t"
		<< ob.m_strTelNum << "\t" << ob.getAddXYDomain() << "\t"
		<< ob.m_lSequence << std::endl;
}

// ログ出力(距離追加版 Bug 5701対応 07/11/30追加)
void CErrorHandle::printLog( const CTelTP& ob, int nErrorCode, const double dist )
{
	// 出力開始  「住所名」「20桁住所コード」「方書名称」「掲載名称」「調査結果」「情報ソース」「電話番号」「座標付与種別」「TelTPとPOIとの距離」
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< m_lpszErrMsg[nErrorCode] << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" << ob.m_strTelNum  << "\t"
		<< ob.getAddXYDomain() << "\t" << ob.m_lSequence << "\t"
		<< dist << std::endl;
}

// エラーコードからエラーメッセージ取得
const std::string CErrorHandle::getErrorMsg( int nErrorCode, const std::string* lpstrBGDomain /* = NULL */ ) const
{
	switch( nErrorCode )
	{
	case E_ON_NGCITYSITE:
	case E_ON_NGBASESITE:
		{
			char szTmp[150] = {0};
			sprintf_s( szTmp, sizeof(szTmp), m_lpszErrMsg[nErrorCode], lpstrBGDomain->c_str() );

			return szTmp;
		}
		break;

	default:
		return m_lpszErrMsg[nErrorCode];
	}
}

