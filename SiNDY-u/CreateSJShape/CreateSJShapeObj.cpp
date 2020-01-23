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
#include "CreateSJShapeObj.h"
#include "Workspace.h"
#include "AheLayerFunctions.h"


using namespace std;

FILE* g_pFILE = NULL;

void OpenLogFile(const CString& strFile)
{
	CString strLogFile;
	if( strFile.IsEmpty() )
	{
		TCHAR cBuf[ _MAX_PATH+1 ];
		if( !GetModuleFileName( NULL, cBuf, _MAX_PATH ) )
			return;

		strLogFile.Format( _T("%s"), cBuf );
		long lidx = strLogFile.ReverseFind( '\\' );
		if( lidx == -1 )
			return;

		strLogFile = strLogFile.Left( lidx );
	}
	else
		strLogFile = strFile;

	CString cFileName;
	CTime cTime = CTime::GetCurrentTime();
	long lYear = cTime.GetYear();
	long lMonth = cTime.GetMonth();
	long lDay = cTime.GetDay();
	long lHour = cTime.GetHour();
	long lMinute = cTime.GetMinute();
	long lSecond = cTime.GetSecond();

	cFileName.Format( _T("\\CreateSJShape%04d%02d%02d%02d%02d%02d.log"), lYear, lMonth, lDay, lHour, lMinute, lSecond );
	strLogFile += cFileName;

	fopen_s(&g_pFILE, CT2CA( strLogFile ), "a" );
}

void WriteLog(const CString& msg, bool bDiplayLog = true )
{
	USES_CONVERSION;
	if( bDiplayLog )
		cerr << T2A( msg ) << endl;

	if( !g_pFILE )
		return;

	if( msg.IsEmpty() )
		return;

	fprintf( g_pFILE, CT2CA( msg ) );
	fprintf( g_pFILE, "\n" );
	fflush( g_pFILE );
}

void ReleaseLogFile()
{
	if( g_pFILE )
	{
		fclose( g_pFILE );
		g_pFILE = NULL;
	}
}


CCreateSJShapeObj::CCreateSJShapeObj(void)
{
}

CCreateSJShapeObj::~CCreateSJShapeObj(void)
{
}

bool Usage()
{
	std::cerr << "SJ用リリースデータ作成ツール" << std::endl;
	std::cerr << "Usage  : CreateSJShape -s <SJデータサーバ名> -m <メッシュサーバ名> -l <メッシュリスト> -r <リリースデータ出力フォルダ> -c <都市エリアPGDB> -o <ログ出力フォルダ>" << std::endl;
	return false;
}

// 初期化関数
bool CCreateSJShapeObj::Init(int argc, _TCHAR **argv)
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
			case 's':
				{
					CString strConnectServer = *++argv;
					ConnectServer( strConnectServer, false );
					--argc;
				}
				break;
			case 'm':
				{
					CString strConnectServer = *++argv;
					ConnectServer( strConnectServer, true );
					--argc;
				}
				break;
			case 'l':
				{
					ifstream file( *++argv );
					if (! file.is_open())
					{
						WriteLog( _T("メッシュリストが開けませんでした") );
						return false;
					}
					while (file.good())
					{
						unsigned int m = 0;
						file >> m;
						if (! m) continue;
						m_listMesh.push_back(m);
					}
					--argc;
				}
				break;
			case 'o':
				{
					m_strLogFileName = *++argv;
					--argc;
				}
				break;
			case 'r':
				{
					m_strOutputDir = *++argv;
					--argc;
				}
				break;
			case 'c':
				{
					CString strCityAreaPGDB = *++argv;
					sindy::CWorkspace cWorkspace;
					cWorkspace.Connect( strCityAreaPGDB );
					((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureClass(_bstr_t(_T("CITYAREA")), &m_ipCityArea);
					--argc;
					break;
				}
			default:
				return Usage();
			}
			break;
		default:
			break;
		}
	}

	return true;
}

// サーバに接続して必要なフィーチャクラスを取得する
void CCreateSJShapeObj::ConnectServer( const CString& strConnectServer, bool bIsMesh )
{
	sindy::CWorkspace cWorkspace;
	cWorkspace.Connect( strConnectServer );

	// サーバ接続
	IFeatureDatasetPtr ipFeatureDataset;
	if( bIsMesh )
		((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_REF_MESH")), &ipFeatureDataset);
	else
		((IFeatureWorkspacePtr)(IWorkspace*)cWorkspace)->OpenFeatureDataset(_bstr_t(_T("WRD_CITY")), &ipFeatureDataset);

	if( ipFeatureDataset )
	{
		IEnumDatasetPtr ipSubSetsDataset;
		ipFeatureDataset->get_Subsets( &ipSubSetsDataset );
		ipSubSetsDataset->Reset();
		IDatasetPtr ipSubDataset;
		while( ipSubSetsDataset->Next( &ipSubDataset ) == S_OK )
		{
			CString strDatasetName = AheGetFeatureClassName( ipSubDataset );
			if( bIsMesh )
			{
				if( strDatasetName == _T("TERTIARYMESH") )
				{
					m_ipTertiaryMesh = ipSubDataset;
					break;
				}
			}
			else
			{
				m_listSJFeatureClass.push_back( (IFeatureClassPtr)ipSubDataset );
			}
		}
	}
}

// メイン関数
bool CCreateSJShapeObj::Run()
{
	OpenLogFile( m_strLogFileName );

	if( !m_ipCityArea )
	{
		WriteLog( _T("CITYAREAを取得できませんでした\n") );
		ReleaseLogFile();
		return false;
	}

	for( list< unsigned long >::iterator it = m_listMesh.begin(); it != m_listMesh.end(); ++it )
	{
		// INでつないで最初に全部もってきちゃったほうが早いので、あとで要調整
		CString strMeshQuery;
		strMeshQuery.Format( _T("MESHCODE = %u"), *it );
		IFeatureCursorPtr ipTertiaryMeshCursor;
		m_ipTertiaryMesh->Search( AheInitQueryFilter( NULL, NULL, strMeshQuery ), VARIANT_FALSE, &ipTertiaryMeshCursor );
		
		if( !ipTertiaryMeshCursor )
		{
			CString strLog;
			strLog.Format( _T("%u:メッシュを取得できませんでした"), *it );
			WriteLog(strLog);
			ReleaseLogFile();
			return false;
		}
		int nMeshCount = 0;
		IFeaturePtr ipTertiaryMeshFeature;
		// 基本1個しかないはず
		while( ipTertiaryMeshCursor->NextFeature( &ipTertiaryMeshFeature ) == S_OK )
		{
			nMeshCount++;
			IGeometryPtr ipTertiaryMeshGeom;
			ipTertiaryMeshFeature->get_ShapeCopy( &ipTertiaryMeshGeom );
			CreateSJShape( *it, ipTertiaryMeshGeom );
		}
		if( nMeshCount != 1 )
		{
			CString strLog;
			if( nMeshCount == 0 )
				strLog.Format( _T("%u:メッシュを取得できませんでした"), *it );
			else
				strLog.Format( _T("%u:メッシュが複数見つかりました"), *it );
			WriteLog(strLog);
			ReleaseLogFile();
			return false;
		}
	}

	ReleaseLogFile();
	return true;
}

void CCreateSJShapeObj::CreateSJShape( unsigned long lMeshCode, IGeometryPtr ipTertiaryMeshGeom )
{
	for( list< CAdapt< IFeatureClassPtr > >::iterator it = m_listSJFeatureClass.begin(); it != m_listSJFeatureClass.end(); ++it )
	{
		ISpatialReferencePtr ipSpRef;
		((IGeoDatasetPtr)it->m_T)->get_SpatialReference( &ipSpRef );
		CString strFeatureClassName = AheGetFeatureClassName( it->m_T );
		IFeatureClassPtr ipShapeFeatureClass = CreateShapeFeatureClass( lMeshCode, strFeatureClassName, ipSpRef );
		if( ipShapeFeatureClass )
		{
			// メッシュ形状でSDEフィーチャクラス空間検索
			IFeatureCursorPtr ipFeatureCursor;
			it->m_T->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipTertiaryMeshGeom ), NULL, NULL ), VARIANT_FALSE, &ipFeatureCursor );

			IFeatureCursorPtr	ipShapeInsertCursor;
			ipShapeFeatureClass->Insert(VARIANT_TRUE, &ipShapeInsertCursor);

			IFieldsPtr ipShapeFields, ipFields;
			ipShapeFeatureClass->get_Fields( &ipShapeFields );
			it->m_T->get_Fields( &ipFields );
			map< long, long > mapCorrespondFieldIndex = GetCorrespondFieldIndex( strFeatureClassName, ipShapeFields, ipFields );

			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
			{
				IGeometryPtr ipGeom;
				ipFeature->get_ShapeCopy( &ipGeom );
				IGeometryPtr ipIntersectGeom;
				if( strFeatureClassName == _T("HIG_A") || strFeatureClassName == _T("FRM_A") )
					((ITopologicalOperatorPtr)ipGeom)->Intersect( ipTertiaryMeshGeom, esriGeometry1Dimension, &ipIntersectGeom );
				else
					((ITopologicalOperatorPtr)ipGeom)->Intersect( ipTertiaryMeshGeom, esriGeometry2Dimension, &ipIntersectGeom );

				long lOID = 0;
				ipFeature->get_OID( &lOID );

#ifdef _DEBUG
				if( strFeatureClassName == _T("DEC_P") && lOID == 5754 )
				{
					IPointCollectionPtr ipPointCol = ipIntersectGeom;
					long lPointCol = 0;
					ipPointCol->get_PointCount( &lPointCol );
					for( int i = 0; i < lPointCol; ++i )
					{
						IPointPtr ipPoint;
						ipPointCol->get_Point( i, &ipPoint );
						double dX, dY;
						ipPoint->QueryCoords( &dX, &dY );
						CString str;
						str.Format( _T("%ld X=%.20lf Y=%.20lf\n"), i, dX, dY );
						OutputDebugString( str );
					}
				}
#endif
				// このスナップをするべきかするべきじゃないかはよくわからない
				// とはいえ、うえでIntersectした時点ですでに、空間参照あわされちゃっている？感じで
				// このスナップに意味はないかもしれない。なぜなら、このスナップ前後で座標がずれないので
				ipIntersectGeom->putref_SpatialReference( ipSpRef );
				ipIntersectGeom->SnapToSpatialReference();
				// メッシュ境界で切れているような形状（このプロジェクトにはないはずだが）だと、形状NULLになっちゃうので
				// 一応チェックしてはじいておく
				VARIANT_BOOL vaEmpty = VARIANT_FALSE;
				ipIntersectGeom->get_IsEmpty( &vaEmpty );
				if( vaEmpty )
					continue;

				IFeatureBufferPtr	ipFeatureBuffer;
				ipShapeFeatureClass->CreateFeatureBuffer(&ipFeatureBuffer);
				ipFeatureBuffer->putref_Shape( ipIntersectGeom );
				for( map< long, long >::iterator itField = mapCorrespondFieldIndex.begin(); itField != mapCorrespondFieldIndex.end(); ++itField )
				{
					CComVariant vaValue;
					ipFeature->get_Value( itField->first, &vaValue );
					ipFeatureBuffer->put_Value( itField->second, vaValue );

					// 雲ポリゴンのチェック
					// 雲ポリゴンとは、CITYAREA内にある不完全エリアのこと
					// 属性値60（不完全エリア）ハードコーディングだが許して
					if( strFeatureClassName == _T("FRM_P") && vaValue.lVal == 60 )
					{
						IFeatureCursorPtr ipCityAreaCursor;
						m_ipCityArea->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipGeom, _T("Shape"), esriSearchOrderSpatial, esriSpatialRelWithin ), NULL, NULL ), VARIANT_FALSE, &ipCityAreaCursor );
						IFeaturePtr ipCityArea;
						while( ipCityAreaCursor->NextFeature( &ipCityArea ) == S_OK )
						{
							IGeometryPtr ipCityAreaGeom;
							ipCityArea->get_ShapeCopy( &ipCityAreaGeom );
							IRelationalOperatorPtr ipRelOp = ipGeom;
							VARIANT_BOOL vaWithin = VARIANT_FALSE;
							ipRelOp->Within( ipCityAreaGeom, &vaWithin );
							if( vaWithin )
							{
								IFieldsPtr ipShapeFields;
								ipShapeFeatureClass->get_Fields( &ipShapeFields );
								long lSourceIndex = -1;
								ipShapeFields->FindField( _bstr_t( _T("Source") ), &lSourceIndex );
								ipFeatureBuffer->put_Value( lSourceIndex, _variant_t(_bstr_t(_T("cloud"))) );
							}
						}
					}
				}

				CComVariant va;
				HRESULT hr = ipShapeInsertCursor->InsertFeature(ipFeatureBuffer, &va);
				if( !( hr == S_OK && va.lVal >= 0 ) )
				{
					CString strLog;
					strLog.Format( _T("%u\t%s\t処理途中でInsertFeatureに失敗しました"), lMeshCode, strFeatureClassName );
					WriteLog( strLog );
				}
			}
			HRESULT hr = ipShapeInsertCursor->Flush();
			
			if( hr == S_OK )
			{
				CString strLog;
				strLog.Format( _T("%u\t%s\t処理正常終了"), lMeshCode, strFeatureClassName );
				WriteLog( strLog );
			}
			else
			{
				CString strLog;
				strLog.Format( _T("%u\t%s\t処理不正（Flushに失敗）"), lMeshCode, strFeatureClassName );
				WriteLog( strLog );
			}
		}
	}
}

IFeatureClassPtr CCreateSJShapeObj::CreateShapeFeatureClass( unsigned long lMeshCode, CString strFeatureClassName, ISpatialReferencePtr ipSpRef )
{
	USES_CONVERSION;
	CString strMeshCode, strPrimary, strSecondary;
	strMeshCode.Format( _T("%lu"), lMeshCode );
	strPrimary.Format( _T("%.6s"), strMeshCode );
	strSecondary.Format( _T("%.8s"), strMeshCode );

	CString strPrimaryDir, strSecondaryDir;
	strPrimaryDir = m_strOutputDir + _T("\\") + strPrimary;
	strSecondaryDir = m_strOutputDir + _T("\\") + strPrimary + _T("\\") + strSecondary;
	bool bExist = true;
	if( !PathFileExists( m_strOutputDir ) )
	{
		if( !CreateDirectory( m_strOutputDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s を作成できませんでした"), m_strOutputDir );
		WriteLog( strLog );
		return false;
	}
	if( !PathFileExists( strPrimaryDir ) )
	{
		if( !CreateDirectory( strPrimaryDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s を作成できませんでした"), strPrimaryDir );
		WriteLog( strLog );
		return false;
	}
	if( !PathFileExists( strSecondaryDir ) )
	{
		if( !CreateDirectory( strSecondaryDir, NULL ) )
			bExist = false;
	}
	if( !bExist )
	{
		CString strLog;
		strLog.Format( _T("%s を作成できませんでした"), strSecondaryDir );
		WriteLog( strLog );
		return false;
	}

	// shapeファイル名作成
	CString strShapeFileName;
	strShapeFileName.Format( _T("%u%s"), lMeshCode, strFeatureClassName );
	// 小文字に統一
	strShapeFileName.MakeLower();

	//-------------------------------------------------------------------------------
	// Shapeファイルのオープン処理	
	//-------------------------------------------------------------------------------
	// ワークスペースをオープン
	IPropertySetPtr			ipPropertySet(CLSID_PropertySet);
	ipPropertySet->SetProperty(_bstr_t(_T("DATABASE")), _variant_t( _bstr_t(strSecondaryDir) ) );
	// ワークスペースのオープン
	IWorkspaceFactoryPtr	ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IWorkspacePtr			ipWorkspace;
	ipWSFactory->Open(ipPropertySet, 0, &ipWorkspace);
	if( !ipWorkspace) {
		CString strLog;
		strLog.Format( _T("%s のワークスペース（ディレクトリ）のオープンに失敗しました"), strPrimaryDir );
		WriteLog( strLog );
		return	NULL;
	}

	// フィーチャクラスのオープン
	IFeatureWorkspacePtr	ipFeatureWorkspace(ipWorkspace);
	IFeatureClassPtr		ipFeatureClass;
	ipFeatureWorkspace->OpenFeatureClass(_bstr_t(strShapeFileName), &ipFeatureClass);

	if(ipFeatureClass != NULL) {
		CString strLog;
		strLog.Format( _T("%s.shp:すでに同名のshapeファイルが存在しています"), strShapeFileName );
		WriteLog( strLog );
		return	NULL;
	}


	// ジオメトリ(Shape)・フィールド追加
	//-------------------------------------------------------------
	IFieldsPtr		ipFields(CLSID_Fields);
	IFieldsEditPtr	ipFieldsEdit(ipFields);

	IFieldPtr		ipFieldObj(CLSID_Field);
	IFieldEditPtr	ipFieldEditObj(ipFieldObj);
	ipFieldEditObj->put_Name(T2BSTR(_T("ObjectID")));
	ipFieldEditObj->put_Type(esriFieldTypeInteger);
	ipFieldEditObj->put_IsNullable(VARIANT_FALSE);

	if(ipFieldsEdit->AddField(ipFieldEditObj) != S_OK) 
		return NULL;

	if( strFeatureClassName == _T("FRM_P") || strFeatureClassName == _T("FRM_A") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("BgCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		if( strFeatureClassName == _T("FRM_P") )
		{
			IFieldPtr		ipField2(CLSID_Field);
			IFieldEditPtr	ipFieldEdit2(ipField2);
			ipFieldEdit2->put_Name(T2BSTR(_T("Source")));
			ipFieldEdit2->put_Type(esriFieldTypeString);
			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
				return NULL;
		}
	}
	else if( strFeatureClassName == _T("HIG_P") || strFeatureClassName == _T("HIG_A") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("BldCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		if( strFeatureClassName == _T("HIG_P") )
		{
			IFieldPtr		ipField2(CLSID_Field);
			IFieldEditPtr	ipFieldEdit2(ipField2);
			ipFieldEdit2->put_Name(T2BSTR(_T("CorridorF")));
			ipFieldEdit2->put_Type(esriFieldTypeInteger);
			ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
				return NULL;
		}
	}
	else if( strFeatureClassName == _T("DEC_P") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("DecCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;

		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);
		ipFieldEdit2->put_Name(T2BSTR(_T("RooftopF")));
		ipFieldEdit2->put_Type(esriFieldTypeInteger);
		ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) 
			return NULL;
	}
	else if( strFeatureClassName == _T("ZLV_P") )
	{
		IFieldPtr		ipField(CLSID_Field);
		IFieldEditPtr	ipFieldEdit(ipField);
		ipFieldEdit->put_Name(T2BSTR(_T("ZLevelCls")));
		ipFieldEdit->put_Type(esriFieldTypeInteger);
		ipFieldEdit->put_IsNullable(VARIANT_FALSE);

		if(ipFieldsEdit->AddField(ipFieldEdit) != S_OK) 
			return NULL;
	}


	IFieldPtr		ipField(CLSID_Field);
	IFieldEditPtr	ipFieldEditShape(ipField);
	if( strFeatureClassName == _T("HIG_A") || strFeatureClassName == _T("FRM_A") )
	{
		ipFieldEditShape->put_Name(T2BSTR(_T("Shape")));
		ipFieldEditShape->put_Type(esriFieldTypeGeometry);
		ipFieldEditShape->put_IsNullable(VARIANT_FALSE);

		IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
		IGeometryDefEditPtr		ipGDefEdit(ipGDef);
		// 格納するShapeのジオメトリタイプを設定
		ipGDefEdit->put_GeometryType(esriGeometryPolyline);

		// Shapeにも空間参照充てる
//		ISpatialReferenceTolerancePtr ipTole = ipSpRef;
//		double dTolerance = 0;
		// きっちり座標を出力するためのおまじない
//		ipTole->put_XYTolerance( 0.00000003125 );
//		ipTole->get_XYTolerance( &dTolerance );
//		ISpatialReferenceResolutionPtr ipResol = ipSpRef;
		// きっちり座標を出力するためのおまじない
//		ipResol->put_XYResolution( VARIANT_FALSE, 0.000000015625 );
		ipGDefEdit->putref_SpatialReference( ipSpRef );
		ipFieldEditShape->putref_GeometryDef(ipGDefEdit);
	}
	else
	{
		ipFieldEditShape->put_Name(T2BSTR(_T("Shape")));
		ipFieldEditShape->put_Type(esriFieldTypeGeometry);
		ipFieldEditShape->put_IsNullable(VARIANT_FALSE);

		IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
		IGeometryDefEditPtr		ipGDefEdit(ipGDef);
		// 格納するShapeのジオメトリタイプを設定
		ipGDefEdit->put_GeometryType(esriGeometryPolygon);

		// Shapeにも空間参照充てる
		ISpatialReferenceTolerancePtr ipTole = ipSpRef;
		// 許容値と精度を与えても与えなくても、結局ずれるので、与えないことにする
		// 許容値0.0000003125,精度0.000000015625を与えるとメッシュ境界で正数値を
		// 出すことができるが、それが重要ではない
		// 上記許容値を与えると、SDEのデータと小数点9桁目くらいから出力される座標がずれる
		// おそらく許容値範囲内くらいなのだろうと思われる
		// 与えない場合には、SDEの値から小数点15桁目くらいでずれる
		// これはおそらくdouble値の値のずれだろうと思われる
		// 今回SJプロジェクトでは、メッシュ境界付近で正数値が出せないということは
		// 特に問題にはならないはず（SJにサンプルで2メッシュ出した時と同じ状態）
//		double dTolerance = 0;
//		ipTole->get_XYTolerance( &dTolerance );
		// きっちり座標を出力するためのおまじない
//		ipTole->put_XYTolerance( 0.00000003125 );
//		ISpatialReferenceResolutionPtr ipResol = ipSpRef;
//		double dReso = 0;
//		ipResol->get_XYResolution( VARIANT_FALSE, &dReso );
		// きっちり座標を出力するためのおまじない
//		ipResol->put_XYResolution( VARIANT_TRUE, dTolerance / 2.0 );
//		ipResol->put_XYResolution( VARIANT_FALSE, 0.000000015625 );
		ipGDefEdit->putref_SpatialReference( ipSpRef );
		ipFieldEditShape->putref_GeometryDef(ipGDefEdit);
	}

	if(ipFieldsEdit->AddField(ipFieldEditShape) != S_OK) 
		return NULL;

	//-------------------------------------------------------------


	ipFeatureWorkspace->CreateFeatureClass(_bstr_t( strShapeFileName ), ipFields,
		NULL, NULL, esriFTSimple, _bstr_t(_T("Shape")), CComBSTR(""), &ipFeatureClass);

	return ipFeatureClass;
}

map< long, long > CCreateSJShapeObj::GetCorrespondFieldIndex( CString strFeatureClassName, IFieldsPtr ipShapeFields, IFieldsPtr ipFields )
{
	map< long, long > mapFieldIndex;
	long lShapeIndex = -1, lIndex = -1;
	ipFields->FindField( _T("ObjectID"), &lIndex );
	ipShapeFields->FindField( _T("ObjectID"),  &lShapeIndex );
	mapFieldIndex[ lIndex ] = lShapeIndex;

	if( strFeatureClassName == _T("FRM_P") || strFeatureClassName == _T("FRM_A") )
	{
		ipFields->FindField( _T("BgCls"), &lIndex );
		ipShapeFields->FindField( _T("BgCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
	}
	else if(  strFeatureClassName == _T("HIG_P") || strFeatureClassName == _T("HIG_A") )
	{
		ipFields->FindField( _T("BldCls"), &lIndex );
		ipShapeFields->FindField( _T("BldCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
		if( strFeatureClassName == _T("HIG_P") )
		{
			ipFields->FindField( _T("CorFlag"), &lIndex );
			ipShapeFields->FindField( _T("CorridorF"),  &lShapeIndex );
			mapFieldIndex[ lIndex ] = lShapeIndex;
		}
	}
	else if(  strFeatureClassName == _T("DEC_P") )
	{
		ipFields->FindField( _T("DecCls"), &lIndex );
		ipShapeFields->FindField( _T("DecCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
		ipFields->FindField( _T("RooftopFlag"), &lIndex );
		ipShapeFields->FindField( _T("RooftopF"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;

	}
	else if(  strFeatureClassName == _T("ZLV_P") )
	{
		ipFields->FindField( _T("ZLevelCls"), &lIndex );
		ipShapeFields->FindField( _T("ZLevelCls"),  &lShapeIndex );
		mapFieldIndex[ lIndex ] = lShapeIndex;
	}

	return mapFieldIndex;
}
