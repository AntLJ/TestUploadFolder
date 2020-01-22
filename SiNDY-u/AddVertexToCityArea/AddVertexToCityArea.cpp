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

// AddVertexToCityArea.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include  <io.h>
#include  <stdio.h>
#include "AheGeometryOP.h"
#include "Workspace.h"

using namespace std;

CString m_strCityAreaWorkspace;
CString m_strMeshServerName;

bool Init(int argc, _TCHAR **argv);
int run();
IFeatureClassPtr GetTertiaryMesh( CString strConnectServer );


int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	int retval = 0;

	try {
		arctl::coinitializer initializer;
		{
			if( Init( argc, argv) )
				retval = run();
			else
				return retval;
		}
	} catch (...) {
		retval = 2;
	}

	switch (retval) {
	case 0: std::cerr << "正常に終了しました" << std::endl; break;
	case 1: std::cerr << "異常終了しました" << std::endl; break;
	case 2: std::cerr << "例外が発生しました" << std::endl; break;
	default: break;
	}
	return retval;
}

// 使用方法出力関数
bool Usage()
{
	std::cerr << "都市形状にメッシュとの交点に構成点を作成するツール説明" << std::endl;
	std::cerr << "Usage  : AddVertexToCityArea -c <都市形状が入っているPGDBのファイルパス>" << std::endl;
	std::cerr << "                             -m <メッシュ形状を取得する接続先>" << std::endl;
	std::cerr << std::endl;
	std::cerr << "接続先の形式:ユーザ名/パスワード/バージョン/サービス/サーバ名" << std::endl;
	std::cerr << "例：REFERENCE/REFERENCE/SDE.DEFAULT/5151/SPINEL2.MR.IPC.PIONEER.CO.JP" << std::endl;
	return false;
}

// 初期化関数
bool Init(int argc, _TCHAR **argv)
{
	if (argc <= 1)
		return Usage();
	while (--argc)
	{
		_TCHAR *s = *++argv;
		switch (*s)
		{
		case '-':
			switch (*++s)
			{
			case 'c':
				m_strCityAreaWorkspace = *++argv;
				--argc;
				break;
			case 'm':
				m_strMeshServerName = *++argv;
				--argc;
				break;
			default:
				return Usage();
			}
			break;
		default:
			break;
		}
	}

	// 値は全部埋まっていないといけない
	if( m_strCityAreaWorkspace.IsEmpty() || m_strMeshServerName.IsEmpty()  )
	{
		std::cerr << "引数が不足しています" << std::endl;
		return false;
	}

	return true;
}

// サーバにつなぎ、JIBANフィーチャクラスを取得する関数
IFeatureClassPtr GetTertiaryMesh( CString strConnectServer )
{
	USES_CONVERSION;
	sindy::CWorkspace cWorkspace;
	if( !cWorkspace.Connect( strConnectServer ) )
	{
		std::cerr << T2A( strConnectServer ) << "に接続できませんでした" << std::endl;
		return false;
	}
	// サーバ接続
	IFeatureDatasetPtr ipFeatureDataset;
	if(((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_REF_MESH")), &ipFeatureDataset) == S_OK)
	{
		// すでにある場合には、対象のフィーチャクラス取得して終了
		IEnumDatasetPtr ipSubSetsDataset;
		ipFeatureDataset->get_Subsets( &ipSubSetsDataset );
		ipSubSetsDataset->Reset();
		IDatasetPtr ipSubDataset;
		while( ipSubSetsDataset->Next( &ipSubDataset ) == S_OK )
		{
			CString strDatasetName = AheGetFeatureClassName( ipSubDataset );
			if( lstrcmpi( strDatasetName, _T("TERTIARYMESH")) == 0 )
			{
				return ipSubDataset;
			}
		}
	}

	std::cerr << "TERTIARYMESHを取得できませんでした" << std::endl;
	return NULL;
}

// 実際の実行関数
int run()
{
	IFeatureClassPtr ipMeshClass = GetTertiaryMesh( m_strMeshServerName );
	if( !ipMeshClass )
		return false;

	USES_CONVERSION;
	// PGDBオープン
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile( _bstr_t( m_strCityAreaWorkspace ), 0, &ipWorkspace );

	IEnumDatasetPtr ipEnumDataset;
	ipWorkspace->get_Datasets( esriDTFeatureClass, &ipEnumDataset );
	IDatasetPtr ipDataset;
	while( ipEnumDataset->Next( &ipDataset ) == S_OK )
	{
		IFeatureClassPtr ipFeatureClass = ipDataset;
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		ipQuery->put_WhereClause( _bstr_t( _T("OBJECTID>0") ) );
		IFeatureCursorPtr ipFeatureCursor;
		ipFeatureClass->Update(ipQuery, VARIANT_TRUE, &ipFeatureCursor);
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
		{
			IGeometryPtr ipCityGeom;
			ipFeature->get_ShapeCopy( &ipCityGeom );

			IFeatureCursorPtr ipCursor;
			ipMeshClass->Search( AheInitSpatialFilter( NULL, ipCityGeom ), VARIANT_FALSE, &ipCursor );
			IFeaturePtr ipMeshFeature;
			while( ipCursor->NextFeature( &ipMeshFeature ) == S_OK )
			{
				IGeometryPtr ipMeshGeom;
				ipMeshFeature->get_ShapeCopy( &ipMeshGeom );
				// メッシュと交わる点を取得
				IPointCollectionPtr ipAddPointCol = AheGetCrossOrTouchPoints( ipCityGeom, ipMeshGeom  );
				if( ipAddPointCol )
				{
					// 構成点をうまいこと追加してくれる関数
					AheAddVertexesIfOnGeometry( ipCityGeom, (IGeometryPtr)ipAddPointCol );
				}
			}
			ipFeature->putref_Shape( ipCityGeom );
			HRESULT hr = ipFeature->Store();
			CComVariant vaValue;
			ipFeature->get_Value( 2, &vaValue );
			CString strName = vaValue.bstrVal;
			std::cerr << T2A(strName) << "に構成点追加終了" << std::endl;
		}
	}

	return 0;
}

