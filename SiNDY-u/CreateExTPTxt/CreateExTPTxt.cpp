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

// CreateExTPTxt.cpp: CreateExTPTxt クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateExTPTxt.h"
#include <map>

#include <crd_cnv/coord_converter.h>
#include <sindy/workspace.h>

static crd_cnv	g_cCrdCnv;	// 座標変換クラスインスタンス

// 使用法
void CreateExTPTxt::Usage()
{
	std::cout << "[XY付与DB用ExTPポイントデータ出力ツール]\n"
	  		  << "(Usage)CreateExTPTxt.exe [SDE接続先] [フィーチャクラス名] [出力ファイル]" << std::endl;
}

// 初期化
bool CreateExTPTxt::Init( int argc, char** argv )
{
	if( argc != 4 )
	{
		std::cerr << "#Eror 引数の数が不正です" << std::endl;
		return false;
	}

	// 環境変数の値取得し、DB接続
	IWorkspacePtr ipWorkspace;
	INamePtr ipName = sindy::create_workspace_name( argv[1] );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		ipWorkspace = ipUnknown;
	}
	if( NULL == ipWorkspace )
	{
		std::cerr << "#Error SDE接続失敗 : " << argv[1] << std::endl;
		return false;
	}
	std::cout << "#SDE接続 : " << argv[1] << std::endl;

	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(argv[2]), &m_ipExTPClass );
	if( NULL == m_ipExTPClass )
	{
		std::cerr << "#Error ExTPポイントフィーチャクラスの取得に失敗 : " << argv[2] << std::endl;
		return false;
	}

	// ログファイルオープン
	m_ofs.open( static_cast<CT2CW>(argv[3]) );
	if( !m_ofs )
	{
		std::cerr << "#Error ログファイルオープンに失敗 : " << argv[3] << std::endl;
		return false;
	}

	return true;
}

// 実行
void CreateExTPTxt::Execute()
{
	// フィールドインデックス取得
	long lAddrCode_FIdx = -1, lInfoSrc_FIdx = -1;
	if( FAILED(m_ipExTPClass->FindField(_bstr_t("ADDRCODE"), &lAddrCode_FIdx)) 
		|| FAILED(m_ipExTPClass->FindField( _bstr_t("INFOSRC_C"), &lInfoSrc_FIdx)) )
	{
		std::cerr << "#Error フィールドインデックスの取得に失敗" << std::endl;
		return;
	}
	
	// フィーチャの検索 ... [Bug6577]精度4も対象に追加 [Bug8625]HLPも対象に追加
	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->PutWhereClause( _bstr_t("AddPoint_f = 0 and ((InfoSrc_c IN (1, 3, 4) and PointStat_c in (2, 3, 4, 5, 6, 7, 8, 9)) or InfoSrc_c = 2)") );

	IFeatureCursorPtr ipCursor;
	m_ipExTPClass->Search( ipQuery, VARIANT_FALSE, &ipCursor );
	if( ipCursor )
	{
		IFeaturePtr ipFeature;
		while( ipCursor->NextFeature(&ipFeature) == S_OK )
		{
			CExTPRec cExTP;

			ipFeature->get_OID( &cExTP.m_lOID );	// OID

			// 座標値の取得
			IGeometryPtr ipGeo;
			ipFeature->get_Shape( &ipGeo );
			((IPointPtr)ipGeo)->QueryCoords( &cExTP.m_point.X, &cExTP.m_point.Y );
			
			// 住所コード
			CComVariant	vaValue;
			ipFeature->get_Value( lAddrCode_FIdx, &vaValue );
			cExTP.m_strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
			vaValue.Clear();

			// 初期座標提供元コード
			ipFeature->get_Value( lInfoSrc_FIdx, &vaValue );
			cExTP.m_lInfoSrc = vaValue.lVal;

			AddExTP( cExTP );	// 追加
		}

		OutputExTP();	// 出力
	}
}

///////////////////////////////////////////////////////
// 内部関数
///////////////////////////////////////////////////////

// ExTP追加
void CreateExTPTxt::AddExTP( const CExTPRec& cExTP )
{
	EXTPREC_MAP::iterator itr = m_mapExTP.find( cExTP.m_strAddrCode );
	if( itr != m_mapExTP.end() )
	{
		// [Bug6577]既に存在する場合、以下の条件を満たしたらスワップ
		// (1) INFOSRC_Cが「2」以外を優先
		// (2) INFOSRC_Cが同じなら、オブジェクトIDが大きい方を優先
		if( itr->second.m_lInfoSrc == 2 )
		{
			if( cExTP.m_lInfoSrc != 2 || itr->second.m_lOID < cExTP.m_lOID )
				itr->second = cExTP;
		}
		else
		{
			if( cExTP.m_lInfoSrc != 2 && itr->second.m_lOID < cExTP.m_lOID )
				itr->second = cExTP;
		}
	}
	else
	{
		m_mapExTP.insert( std::pair<CString, CExTPRec>(cExTP.m_strAddrCode, cExTP) );	// 普通に追加
	}
}

// レコード出力
void CreateExTPTxt::OutputExTP()
{
	for( EXTPREC_MAP::const_iterator itr = m_mapExTP.begin(); itr != m_mapExTP.end(); ++itr )
	{
		// 2次メッシュXYの取得
		int nMeshCode = 0, nMeshX = 0, nMeshY = 0;
		g_cCrdCnv.LLtoMesh( itr->second.m_point.X, itr->second.m_point.Y, 2, &nMeshCode, &nMeshX, &nMeshY, 1 );

		// 初期座標提供元コードからIPコードに変換
		CString strIPCode;
		strIPCode = "07";	// [Bug9101] 12春よりExTPは全てIPCODE[76]

		// 住所コードから座標精度/代表点・地番止まりフラグの設定
		int nSeido = itr->second.m_strAddrCode.Right(4) == _T("0000")? 4 : 5;
		CString strChibanDomariF = (nSeido == 4)? _T("2") : _T("");
		
		// データ出力開始
		m_ofs << nMeshCode << "\t" << nMeshX << "\t" << nMeshY << "\t"  << strIPCode << "\t" << nSeido << "\t"
			  << itr->second.m_strAddrCode.Mid(0, 2) << "\t" << itr->second.m_strAddrCode.Mid(2, 3) << "\t" 
			  << itr->second.m_strAddrCode.Mid(5, 3) << "\t" << itr->second.m_strAddrCode.Mid(8, 3) << "\t"
			  << itr->second.m_strAddrCode.Mid(11,5) << "\t" << itr->second.m_strAddrCode.Mid(16,4) << "\t"
			  << strChibanDomariF << std::endl;
	}
}
