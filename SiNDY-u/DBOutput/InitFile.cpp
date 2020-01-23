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

#include "StdAfx.h"
#include "InitFile.h"

// 初期化
bool CInitFile::init( LPCTSTR lpcszFile, const ITablePtr& ipTable, const bool& bShape, const bool& bSkip )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( !ifs )
	{
		std::cerr << "#ERROR 設定ファイルの読み込みに失敗 : " << CT2CA(lpcszFile) << std::endl;
		return false;
	}

	std::string strBuff;
	while( !ifs.eof() )
	{
		std::getline( ifs, strBuff );

		// コメントまたは、改行だけの時無視
		if( strBuff.empty() || strBuff.find("#") == 0 )
			continue;

		// フィールドＩＤ取得できたら、フィールドIDマップ格納
		CString strField( strBuff.c_str() );
		strField.MakeUpper();	// 大文字にする

		LONG lFID = -1;

		// 位置情報系フィールドかチェック
		if( isShapeField(strField) )
		{
			if( bShape )
			{
				++m_nShapeFieldCount;
				// メッシュコードならメッシュレベル取得する
				if( FN_CITYMESH == strField )		m_nMeshLevel = 64;
				else if( FN_BASEMESH == strField )	m_nMeshLevel = 2;
				else if( FN_MIDDLEMESH == strField )	m_nMeshLevel = 1;

				m_listField.push_back( CFieldInfo(strField, -1, false, true) ); 
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR 図形なしのテーブルを指定した場合、設定ファイルに位置情報関連は指定できません : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
		else if( strField.Right(7) == _T("_DOMAIN") )	// ドメイン指定しているかチェック(_DOMAIN)
		{
			if( SUCCEEDED(ipTable->FindField(CComBSTR(strField.Left(strField.GetLength()-7)), &lFID)) && -1 != lFID )
			{
				m_listField.push_back( CFieldInfo(strField, lFID, true, false) );
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR 設定ファイルに指定した項目がテーブルに存在しません : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
		else if( strField.Left((int)_tcslen(FN_DUMMY)) == FN_DUMMY )	// Ver0.8 よりダミーフィールド追加
		{
			CString strDummyVal;
			if( strField.Find(':') != -1 )
				strDummyVal = strField.Mid( strField.Find(':') + 1 );
			m_listField.push_back( CFieldInfo(strField, lFID, false, false, true, strDummyVal) );
		}
		else
		{
			if( SUCCEEDED(ipTable->FindField(CComBSTR(strField), &lFID)) && -1 != lFID )
			{
				m_listField.push_back( CFieldInfo(strField, lFID, false, false) );
			}
			else if( !bSkip )
			{
				std::cerr << "#ERROR 設定ファイルに指定した項目がテーブルに存在しません : " << CT2CA(strField) << std::endl;
				return false;
			}
		}
	}

	return true;
}
