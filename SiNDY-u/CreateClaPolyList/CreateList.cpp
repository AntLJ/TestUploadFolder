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

// CreateList.cpp: CCreateList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateList.h"
#include <sindy/workspace.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CCreateList::CCreateList() : 
	m_lClaimNumberFID(-1),
	m_lUselessF_FID(-1)
{
}

CCreateList::~CCreateList()
{
}

// 使用法
void CCreateList::printUsage()
{
	cerr << "【クレームポリゴンリスト作成ツール使用法】\n"
		 << "(Usage)CrateClaPolyList.exe [LogFile]\n"
		 << "・環境変数について\n"
		 << "  DB_ANY        クレポリのあるサーバ"
		 << "  FC_CLAIMPOLY  クレポリフィーチャクラス名" << endl;	
}

// 初期化
bool CCreateList::init( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		cerr << "#Error 引数に必ずログファイルは指定してください" << endl;
		return false;
	}

#ifdef _DEBUG
	_tputenv( _T("DB_ANY=techmap/techmap/SDE.DEFAULT/5151/spinel") );
	_tputenv( _T("FC_CLAIMPOLY=ClaimPoly_Map") );
#endif

	// ＤＢ接続プロパティ取得
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ANY")) )
	{
		cerr << "#Error 環境変数 DB_ANYの取得に失敗" << endl;
		return false;
	}

	// クレームポリゴンフィーチャクラス名取得
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_CLAIMPOLY")) )
	{
		cerr << "#Error 環境変数 FC_CLAIMPOLYの取得に失敗" << endl;
		return false;
	}

	// 接続
	IWorkspacePtr ipWorkspace;
	INamePtr ipName( sindy::create_workspace_name(strDBProp) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		ipWorkspace = ipUnknown;
	}

	if( ! ipWorkspace )
	{
		cerr << "#Error SDE接続に失敗 : " << strDBProp << endl;
		return false;
	}
	cerr << "#SDE接続 : " << strDBProp << std::endl;

	// クレームポリゴンフィーチャクラス取得
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(strFCName), &m_ipClaimPolyClass );
	if( ! m_ipClaimPolyClass )
	{
		cerr << "#Error クレームポリゴンフィーチャクラスの取得に失敗 : " << strFCName << endl;
		return false;
	}

	// フィールドインデックス取得
	if( FAILED(m_ipClaimPolyClass->FindField(CComBSTR(_T("CLAIM_NO")), &m_lClaimNumberFID))
		|| FAILED(m_ipClaimPolyClass->FindField(CComBSTR(_T("USELESS_F")), &m_lUselessF_FID)) )
	{
		cerr << "#Error フィールドインデックスの取得に失敗" << std::endl;
		return false;
	}

	// ログファイルオープン
	m_fout.open( argv[1] );
	if( !m_fout )
	{
		cerr << "#Error ログファイルのオープンに失敗 : " << argv[1] << endl;
		return false;
	}

	return true;
}

// 実行
bool CCreateList::execute()
{
	m_fout << "#オブジェクトID,クレーム番号,不要フラグ" << endl;	// ログヘッダー出力

	// 全て取得する
	long lAllCount = 0;
	m_ipClaimPolyClass->FeatureCount( NULL, &lAllCount );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipClaimPolyClass->Search( NULL, VARIANT_FALSE, &ipFeatureCursor );

	if( ! ipFeatureCursor )
	{
		cerr << "#Error : クレームポリゴンが取得できませんでした" << endl;
		return false;
	}

	IFeaturePtr ipFeature;
	for( long i = 1; ipFeatureCursor->NextFeature(&ipFeature) == S_OK; ++i )
	{
		makeClaimPolyList( ipFeature );	// リスト作成

		cerr << i << " / " << lAllCount << " 件終了\r";
	}

	cerr << endl;

	return true;
}

////////////////////////////////////////////
//	内部関数
////////////////////////////////////////////

// クレポリリスト作成
void CCreateList::makeClaimPolyList( IFeature* ipFeature )
{
	// オブジェクトＩＤ取得
	long lOID = 0;
	ipFeature->get_OID( &lOID );

	CComVariant vaValue;

	// クレーム番号取得
	ipFeature->get_Value( m_lClaimNumberFID, &vaValue );
	CString strClaimNo = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();

	// 不要フラグ取得
	ipFeature->get_Value( m_lUselessF_FID, &vaValue );
	long lUselessF = vaValue.lVal;

	// クレーム番号分割して出力
	int nCurPos = 0;
	CString strToken = strClaimNo.Tokenize( _T(",\n"), nCurPos );
	while( strToken != _T("") )
	{
		printInfo( lOID, strToken, lUselessF );
		strToken = strClaimNo.Tokenize( _T(",\n"), nCurPos );
	}
}
