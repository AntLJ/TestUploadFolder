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

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const LPCTSTR CErrorHandle::m_lpszErrMsg[] = 
{
	_T("\t\t"),	// 不明なエラーコード用
	_T("NOTICE\tN1\tCSポイントで取得(未整備)"),
	_T("WARNING\tW2\t11桁コード不一致(整備済)"),
	_T("ERROR\tE3\t街区符号が異なる(整備済)"),
	_T("ERROR\tE4\tGOU拡張整備なのに未整備(整備済)"),
	_T("WARNING\tW5\t11桁コード不一致(整備済)"),
	_T("ERROR\tE6\t調査結果コード選択誤り(TPG整備)"),
	_T("NOTICE\tN7\t11桁コード不一致(未整備)"),
	_T("WARNING\tW8\t11桁コード不一致(整備済)"),
	_T("NOTICE\tN9\t街区符号が異なる(整備済)"),
	_T("NOTICE\tN10\t11桁コード不一致(未整備)"),
	_T("WARNING\tW11\t11桁コード不一致(整備済)"),
	_T("NOTICE\tN12\t街区符号が異なる(整備済)"),
	_T("WARNING\tW13\t行政界不一致なのにデータと一致(未整備)"),
	_T("WARNING\tW14\t行政界不一致なのにデータと一致(整備済)"),
	_T("ERROR\tE15\t調査結果コード属性付与漏れ"),
	_T("WARNING\tW16\t5桁コード不一致"),
	_T("ERROR\tE17\t街区符号拡張の上にポイント"),
	_T("ERROR\tE18\t不明の調査コード"),
	_T("ERROR\tE19\t行政界の境界上にあるまたは行政界上にない"),
	_T("ERROR\tE20\t位置不正(都市地図背景 : %s)"),
	_T("ERROR\tE21\t位置不正(中縮背景 : %s)"),
	_T("ERROR\tE22\t都市地図背景の境界上にあるのでチェックできません"),
	_T("ERROR\tE23\t中縮背景の境界上にあるのでチェックできません"),
	_T("ERROR\tE24\t街区符号（親地番）一致（整備済み）"),
	_T("WARNING\tW25\t8桁コード不一致（GOU整備）"),
	_T("WARNING\tW26\t8桁コード不一致（GOU拡張整備）"),
	_T("WARNING\tW27\t5桁コード不一致[アルプス]"),
	_T("WARNING\tW28\t8桁コード不一致[アルプス]"),
	_T("NOTICE\tN29\t11桁コード不一致（整備済）[アルプス]"),
	_T("NOTICE\tN30\t11桁コード不一致（未整備）[アルプス]"),
	_T("NOTICE\tN31\t街区符号が異なる（整備済）[アルプス]"),
	_T("ERROR\tE32\t位置不正（都市地図背景 : %s）[アルプス]"),
	_T("ERROR\tE33\t位置不正（中縮背景 : %s）[アルプス]"),
	_T("ERROR\tE34\t都市地図背景の境界上にあるのでチェックできません[アルプス]"),
	_T("ERROR\tE35\t中縮背景の境界上にあるのでチェックできません[アルプス]"),
	_T("ERROR\tE36\t同一20桁複数有り")		// Bug 9296対応
};

// ログ出力
void CErrorHandle::printLog( const CExTP& ob, int nErrorCode, const std::string& strBGDomain /* = "" */ )
{
	// 出力開始  「住所名」「20桁住所コード」「方書名称」「掲載名称」「調査結果」「情報ソース」「住所検索ポイント化フラグ」
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< getErrorMsg( getErrorCode(nErrorCode, ob.m_ePointStatC), strBGDomain ) << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" << ob.m_lAddPointF << std::endl;
}

// ログ出力（同一20桁コード保持相手のオブジェクトID追加版（Bug 9296対応）
void CErrorHandle::printLog( const CExTP& ob, int nErrorCode, const long lOID )
{
	// 出力開始  「住所名」「20桁住所コード」「方書名称」「掲載名称」「調査結果」「情報ソース」「住所検索ポイント化フラグ」「同一20桁コード保持相手のオブジェクトID」
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< m_lpszErrMsg[nErrorCode] << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" 
		<< ob.m_lAddPointF <<"\t" << lOID << std::endl;
}

/**
 * 内部関数
 */

// エラー大分類から、エラーコードを取得
int CErrorHandle::getErrorCode( int nErrorState, POINTSTAT_C ePointStat )
{
	// 大分類エラーコードなら、分解する
	switch( nErrorState )
	{
	case E_MISMATCH_ADDRCODE11_NOTSEIBI:
		
		switch( ePointStat )
		{
		case GOU_SEIBI:		return N_GOU_CSPOINT_NOTSEIBI;
		case REFNUM_DIFF:	return N_REF_ADDRCODE_NOTSEIBI;
		case NUM_NOTKEISAI:	return N_NUM_ADDRCODE_NOTSEIBI;
		}
		break;

	case E_MISMATCH_ADDRCODE11_SEIBI:
		
		switch( ePointStat )
		{
		case GOU_SEIBI:		return W_GOU_ADDRCODE_SEIBI;
		case GOU_EXSEIBI:	return W_EXGOU_ADDRCODE_SEIBI;
		case REFNUM_DIFF:	return W_REF_ADDRCODE_SEIBI;
		case NUM_NOTKEISAI:	return W_NUM_ADDRCODE_SEIBI;
		}
		break;
	
	case E_MISMATCH_GAIKU:

		switch( ePointStat )
		{
		case GOU_SEIBI:		return E_GOU_GAIKU_SEIBI;
		case REFNUM_DIFF:	return N_REF_GAIKU_NOTSEIBI;
		case NUM_NOTKEISAI:	return N_NUM_GAIKU_NOTSEIBI;
		}
		break;

	case E_MATCH_GAIKU:

		switch( ePointStat )
		{
		case GOU_EXSEIBI:	return E_EXGOU_GAIKU_SEIBI;
		}
		break;

	default:
		return nErrorState;
	}

	return nErrorState;
}

// エラーコードからエラーメッセージ取得
const std::string CErrorHandle::getErrorMsg( int nErrorCode, const std::string& strBGDomain /* = "" */ ) const
{
	switch( nErrorCode )
	{
	case E_ON_NGCITYSITE:
	case E_ON_NGBASESITE:
	case E_ALPS_ON_NGCITYSITE:
	case E_ALPS_ON_NGBASESITE:
		{
			char szTmp[150] = {0};
			sprintf_s( szTmp, sizeof(szTmp), m_lpszErrMsg[nErrorCode], strBGDomain.c_str() );

			return szTmp;
		}
		break;

	default:
		return m_lpszErrMsg[nErrorCode];
	}
}

