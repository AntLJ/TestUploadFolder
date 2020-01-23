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
#include "App.h"

#include <iomanip>
#include <sindy/workspace.h>
#include <crd_cnv/coord_converter.h>
static crd_cnv g_cCrdCnv;	/// crd_cnvインスタンス

// 初期化
void CApp::printUsage()
{
	std::cerr << "【DBダンパー使用法】\n"
			  << "(Usage)DBOutput.exe <接続DB> <テーブル名> <設定ファイル> <その他オプション> (> 出力ログ)\n"
			  << "・その他オプションについて\n"
			  << "  -w [Where句]         ... 属性で絞り込みたいときに使用して下さい\n"
			  << "  -ol [OBJECTIDリスト] ... 指定したOBJECITIDリストのもののみ出力対象(-w と -mlとの併用不可)\n"
			  << "  -ml [メッシュリスト] ... 指定したメッシュリスト内のデータのみ出力対象\n"
			  << "  -lp ... ポリゴンに対する座標値を、重心ではなくラベルポイントで取得\n"
			  << "  -s  ... 指定した項目がテーブルに無い場合エラーとしない(スキップして出力する)" << std::endl;
}

// 初期化
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( argc < 4 )
	{
		std::cerr << "#ERROR 引数の数が不正です" << std::endl;
		return false;
	}

	// 接続
	INamePtr ipName( sindy::create_workspace_name(argv[1]) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		m_ipWorkspace = ipUnknown;
	}

	if( ! m_ipWorkspace )
	{
		std::cerr << "#ERROR DB接続に失敗 : " << CT2CA(argv[1]) << std::endl;
		return false;
	}
	std::cerr << "#接続開始 : " << CT2CA(argv[1]) << std::endl;

#ifdef SiNDY
	// [bug11555]対応 ⇒ oraエラー回避用
	CComBSTR bstrProgID;
	if(S_OK != IWorkspaceNamePtr(ipName)->get_WorkspaceFactoryProgID(&bstrProgID)){
		std::cerr << "#ERROR ワークスペース情報取得に失敗" << std::endl;
		return false;
	}

	// SDE接続時のみSQL発行
	if(0 <= CString(bstrProgID).Find(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))){
		if (FAILED(m_ipWorkspace->ExecuteSQL(CComBSTR("alter session set cursor_sharing=exact")))) {
			std::cerr << "#ERROR SDEへのCURSOR_SHARING変更に失敗" << std::endl;
			return false;
		}
	}
#endif

	// テーブル取得
	IFeatureWorkspacePtr(m_ipWorkspace)->OpenTable( CComBSTR(argv[2]), &m_ipTable );
	if( NULL == m_ipTable )
	{
		std::cerr << "#ERROR テーブルの取得に失敗 : " << CT2CA(argv[2]) << std::endl;
		return false;
	}
	std::cerr << "#出力対象テーブル : " << CT2CA(argv[2]) << std::endl;

	// テーブルに図形を含むかどうか？
	bool bShape = false;
	if( IFeatureClassPtr(m_ipTable) )
		bShape = true;

	// オプション
	for( int i = 4; i < argc; ++ i )
	{
		if( _tcscmp(argv[i], _T("-w")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:	m_emQueryMode = kWhere;		break;
			case kMesh:m_emQueryMode = kMeshWhere;	break;
			case kOID:
				std::cerr << "#ERROR -w と -ol オプションは同時使用できません" << std::endl;
				return false;			
			default:
				break;
			}
			m_strWhereClause = argv[++i];
			if( m_strWhereClause.IsEmpty() )
			{
				std::cerr << "#ERROR -w オプションに where 句が指定されていません" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-ol")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:	m_emQueryMode = kOID;	break;
			default:
				std::cerr << "#ERROR -ol オプションは、他のオプションと同時使用できません" << std::endl;
				return false;
			}
			m_strOIDList = argv[++i];
			if( m_strOIDList.IsEmpty() )
			{
				std::cerr << "#ERROR -ol オプションにOIDリストファイルが指定されていません" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-ml")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:		m_emQueryMode = kMesh;		break;
			case kWhere:	m_emQueryMode = kMeshWhere;	break;
			case kOID:
				std::cerr << "#ERROR -ml と -ol オプションは同時使用できません" << std::endl;
				return false;
			default:
				break;
			}
			m_strMeshList = argv[++i];
			if( m_strMeshList.IsEmpty() )
			{
				std::cerr << "#ERROR -ml オプションにメッシュリストファイルが指定されていません" << std::endl;
				return false;
			}
			if( !bShape )
			{
				std::cerr << "#ERROR 図形なしのテーブルを指定した場合 -ml オプションは使用できません" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-lp")) == 0 )
		{
			m_bLabelPoint = true;
		}
		else if( _tcscmp(argv[i], _T("-s")) == 0 )
		{
			m_bSkip = true;
		}
		else
		{
			std::cerr << "#ERROR 不正なその他オプションです : " << argv[i] << std::endl;
			return false;
		}
	}

	// 設定ファイル読み込み
	if( !m_cInit.init(argv[3], m_ipTable, bShape, m_bSkip) )
		return false;

	std::cout.precision(12);

	return true;
}

// 実行
bool CApp::execute()
{
	bool bResult = true;

	// 属性絞りモードに応じて処理
	if( m_emQueryMode != kOID )	// OIDリスト以外
	{
		std::list<long> listMesh;
		switch( m_emQueryMode )
		{
		case kMesh:
		case kMeshWhere:
			if( !loadList(m_strMeshList, listMesh) ) {
				std::cerr << "#ERROR メッシュリストの読み込みに失敗" << std::endl;
				return false;
			}
			break;
		default:
			listMesh.push_back( 0 );	// ループで1回のみ回すように追加
			break;
		}

		for( std::list<long>::const_iterator itr = listMesh.begin(); itr != listMesh.end(); ++itr )
		{
			CString strPostErrMsg;
			if( *itr != 0 )
				strPostErrMsg.Format("メッシュコード : %ld", *itr);

			ISpatialFilterPtr ipQuery( CLSID_SpatialFilter );
			if( kWhere == m_emQueryMode || kMeshWhere == m_emQueryMode )
				ipQuery->put_WhereClause( CComBSTR(m_strWhereClause) );

			// メッシュリスト指定時は、メッシュ矩形検索
			if( kMesh == m_emQueryMode || kMeshWhere == m_emQueryMode )
			{
				IGeometryPtr ipMeshGeo( GetMeshShape(*itr) );
				CComBSTR bstr;
				((IFeatureClassPtr)m_ipTable)->get_ShapeFieldName( &bstr );
				if( ipMeshGeo && bstr )
				{
					ipQuery->put_GeometryField( bstr );
					ipQuery->putref_Geometry( ipMeshGeo );
					ipQuery->put_SpatialRel( esriSpatialRelIntersects );
					ipQuery->put_SearchOrder( esriSearchOrderSpatial );
				}
				else
				{
					std::cerr << "#ERROR メッシュ矩形が取得できません。" << CT2CA(strPostErrMsg) << std::endl;
					bResult = false;
				}
			}

			_ICursorPtr ipCursor;
			m_ipTable->Search( ipQuery, VARIANT_FALSE, &ipCursor );
			if( !ipCursor )
			{
				std::cerr << "#ERROR レコードの検索に失敗しました。" << CT2CA(strPostErrMsg) << std::endl;
				bResult = false;
				continue;
			}

			int iCount = 0;
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				if( !output( ipRow, strPostErrMsg ) )
				{
					bResult = false;
				}
				iCount++;
			}
			if( iCount == 0 )
				std::cerr << "#INFO レコードがありませんでした。" << CT2CA(strPostErrMsg) << std::endl;
		}
		
	}
	else	// OIDリスト
	{
		// OIDリスト読み込み
		std::list<long> listOID;
		if( ! loadList(m_strOIDList, listOID) )
		{
			std::cerr << "#ERROR OIDリストの読み込みに失敗" << std::endl;
			return false;
		}

		for( std::list<long>::const_iterator itr = listOID.begin(); itr != listOID.end(); ++itr )
		{
			CString strPostErrMsg;
			strPostErrMsg.Format("OID : %ld", *itr);

			_IRowPtr ipRow;
			m_ipTable->GetRow( *itr, &ipRow );
			if( ipRow )
			{
				if( !output( ipRow, strPostErrMsg ) )
				{
					bResult = false;
				}
			}
			else
			{
				std::cerr << "#ERROR 該当ＩＤのオブジェクトは存在しません。" << CT2CA(strPostErrMsg) << std::endl;
				bResult = false;
			}
		}
	}
	return bResult;
}

//////////////////////////////////
//	非公開関数
//////////////////////////////////

// OID・メッシュリスト読み込み(兼用)
bool CApp::loadList( LPCTSTR lpcszFile, std::list<long>& rList )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		std::copy( std::istream_iterator<long>(ifs), std::istream_iterator<long>(), std::back_inserter(rList) );
		return ifs.eof();
	}
	else
		return false;
}

// 指定メッシュコードのメッシュ形状取得
IGeometryPtr CApp::GetMeshShape( long lMeshCode )
{
	if( !m_ipMeshFC )
	{
		CString strMeshFName;
		if( 1000 < lMeshCode && lMeshCode < 10000 )				strMeshFName = _T("REFERENCE.MiddleMesh");
		else if( 100000 < lMeshCode && lMeshCode < 1000000 )	strMeshFName = _T("REFERENCE.BaseMesh");
		else if( 10000000 < lMeshCode )	strMeshFName =			strMeshFName = _T("REFERENCE.CityMesh");
		else
			return NULL;

		IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( _bstr_t(strMeshFName), &m_ipMeshFC );
		if( !m_ipMeshFC )
			return NULL;
	}

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	CString strWhere;
	strWhere.Format( _T("MESHCODE = %ld"), lMeshCode );
	ipQuery->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipCursor );
	IFeaturePtr ipFeature;
	if( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
	{
		IGeometryPtr ipGeo;
		ipFeature->get_Shape( &ipGeo );
		return ipGeo;
	}
	else
		return NULL;
}

// 出力
bool CApp::output( _IRow* ipRow, const CString& strPostErrMsg )
{
	try
	{
		// 最初に位置情報出力する必要あるか調べる。必要なら左記に位置情報取得(緯度経度)
		WKSPoint point;
		point.X = 0.0; point.Y = 0.0;
		int nMeshcode = 0, nMeshX = 0, nMeshY = 0;
		IGeometryPtr ipGeo = NULL;
		if( m_cInit.getShapeFieldCount() > 0 )
		{
			if( FAILED( IFeaturePtr(ipRow)->get_Shape( &ipGeo ) ) || !ipGeo )
			{
				std::cerr << "#ERROR 図形の取得に失敗 " << CT2CA(strPostErrMsg) << std::endl;
				return false;
			}
			if( !gf::getLonLat( ipGeo, point, m_bLabelPoint ) )
			{
				std::cerr << "#ERROR 緯度経度の取得に失敗 " << CT2CA(strPostErrMsg) << std::endl;
				return false;
			}
		}

		CComVariant vaValue;
		const FIELDLIST& listField = m_cInit.getFieldList();
		for( FIELDLIST::const_iterator itr = listField.begin(); itr != listField.end(); ++itr )
		{
			if( itr != listField.begin() )		std::cout << "\t";

			// SHAPEフィールドでないかで場合わけ
			if( !itr->m_bShapeField )
			{
				// ダミーフィールドか否かで場合分け
				if( !itr->m_bDummy )
				{
					if( FAILED( ipRow->get_Value( itr->m_lFID, &vaValue ) ) )
					{
						std::cerr << "#ERROR 値の取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}

					// 通常 or _DOMAINじゃないかで場合分け
					if( !itr->m_bDomain )
						std::cout << vaValue;
					else
					{
						CString strDomain;
						if( !gf::getDomain( ipRow, itr->m_lFID, vaValue.lVal, strDomain ) )	// _DOMAINならば、ドメインを取得する
						{
							std::cerr << "#ERROR ドメインの取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
							std::cout << std::endl;
							return false;
						}
						std::cout << strDomain;
					}

					vaValue.Clear();
				}
				else
					std::cout << itr->m_strDummyVal;
			}
			else
			{
				// SHAPE
				if( FN_LONGITUDE == itr->m_strName )		std::cout << point.X;
				else if( FN_LATITUDE == itr->m_strName )	std::cout << point.Y;
				else if( FN_POINTS == itr->m_strName )		// [Bug 5691]
				{
					LONG lCount = 0;
					if( !gf::getPointCount( ipGeo, lCount ) )
					{
						std::cerr << "#ERROR 値の取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << lCount;
				}
				else if( FN_ISLINEAR == itr->m_strName )	// [Bug 5691]
				{
					bool bResult = false;
					if( !gf::isLinearObject(ipGeo, bResult) )
					{
						std::cerr << "#ERROR 値の取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << bResult? 1 : 0;
				}
				else if( FN_LENGTH == itr->m_strName )		// [Bug 7086]
				{
					double dLen = -1.0;
					if( !gf::getLength(ipGeo, dLen) )
					{
						std::cerr << "#ERROR 値の取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << dLen;
				}
				else if( FN_AREA == itr->m_strName )		// [Bug 7086]
				{
					double dArea = -1.0;
					if( !gf::getArea(ipGeo, dArea) )
					{
						std::cerr << "#ERROR 値の取得に失敗(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << dArea;
				}
				else
				{
					if( nMeshcode == 0 )
						g_cCrdCnv.LLtoMesh(  point.X, point.Y, m_cInit.getMeshLevel(), &nMeshcode, &nMeshX, &nMeshY, 1 );

					if( FN_MESHX == itr->m_strName )		std::cout << nMeshX;
					else if( FN_MESHY == itr->m_strName )	std::cout << nMeshY;
					else									std::cout << nMeshcode;				
				}
			}
		}
		std::cout << std::endl;
		return true;
	}
	catch( _com_error& e )
	{
		std::cerr << "#COM ERROR 発生 : " << e.ErrorMessage() << " " << CT2CA(strPostErrMsg) << std::endl;
	}
	catch( std::exception& e )
	{
		std::cerr << "#例外発生 : " << e.what() << " " << CT2CA(strPostErrMsg) << std::endl;
	}
	return false;
}
