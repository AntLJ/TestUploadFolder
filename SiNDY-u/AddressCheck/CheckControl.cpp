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

#include "stdafx.h"
#include "CheckControl.h"
#include <boost/timer.hpp>
#include <psapi.h>

// 初期化
bool CheckControl::init( Argument& argument )
{
	m_logDir = argument.getValue(argumentType::kLogDir);
	if( !PathIsDirectory(m_logDir) )
	{
		printProgress( getMsg( iLogPathError ));
		return false;
	}

	printProgress( getMsg( iStartInit ));


	// CTableFinder の初期化
	if(!m_tableFinder.addWorkspace( DB_ADDRESS, argument.getValue( argumentType::kAddressDBPath )))
	{
		printProgress( getMsg( eFailAddressDBOpen ), argument.getValue(argumentType::kAddressDBPath));
		return false;
	}

	if(!m_tableFinder.addWorkspace( DB_MESH, argument.getValue( argumentType::kMeshDBPath )))
	{
		printProgress( getMsg( eFailMeshDBOpen ), argument.getValue(argumentType::kMeshDBPath));
		return false;
	}

	if(!m_tableFinder.addWorkspace( DB_MAP, argument.getValue( argumentType::kMapDBPath )))
	{
		printProgress( getMsg( eFailMapDBOpen ), argument.getValue(argumentType::kMapDBPath));
		return false;
	}

	// チェックレイヤインスタンス作成
	vector<CString> layerList = uh::str_util::split(argument.getValue(argumentType::kLayer), _T(","));

	// 設定ファイル読み込み
	if( !m_settingFile.load( argument.getValue(argumentType::kIniFilePath), layerList ) )
	{
		printProgress( getMsg( eFailLoadSettingFile ), argument.getValue(argumentType::kIniFilePath) );
		return false;
	}

	// テキスト住所マスタパース初期化
	m_textAddressMasterPath = argument.getValue(argumentType::kMasterPath);
	// 住所マスタのパスチェック
	if( !m_textAddressMasterPath.IsEmpty() && !PathIsDirectory( m_textAddressMasterPath ) )
	{
		printProgress( getMsg( iTxtAddressMasterPathError ));
		return false;
	}

	for( const auto layerName: layerList )
	{
		if( !setLayerInstance(layerName) )
		{
			printProgress( message::getMsg( eFailCreateInstance ), layerName );
			return false;
		}

		// 引数layerの値がADDR_POLYGONとADDR_POINTのいずれかで指定して、city_mesh_listを指定しない場合、実行ログにエラーメッセージを出力する
		if( (layerName == addr_polygon::kTableName || layerName == addr_point::kTableName) && argument.getValue(argumentType::kCityMeshListPath).IsEmpty() )
		{
			printProgress( message::getMsg( eFaiCityMeshListSetting ), layerName );
			return false;		
		}

		// 引数layerの値がCITY_POLYGON、PLACENAME_REP_POINTのいずれかで指定して、base_mesh_listを指定しない場合、実行ログにエラーメッセージを出力する
		if( (layerName == city_polygon::kTableName || layerName == placename_rep_point::kTableName) && argument.getValue(argumentType::kBaseMeshListPath).IsEmpty() )
		{
			printProgress( message::getMsg( eFaiBaseMeshListSetting ), layerName );
			return false;		
		}
	}

	// 都市メッシュリスト読み込み
	if( !argument.getValue(argumentType::kCityMeshListPath).IsEmpty() && !m_cityMeshCodeList.load( argument.getValue(argumentType::kCityMeshListPath) ) )
	{
		printProgress( getMsg( eFailLoadMeshList ), argument.getValue(argumentType::kCityMeshListPath));
		return false;
	}
	// ベースメッシュリスト読み込み
	if( !argument.getValue(argumentType::kBaseMeshListPath).IsEmpty() && !m_baseMeshCodeList.load( argument.getValue(argumentType::kBaseMeshListPath) ) )
	{
		printProgress( getMsg( eFailLoadMeshList ), argument.getValue(argumentType::kBaseMeshListPath));
		return false;
	}

	printProgress( message::getMsg( iFinishCreateInstance ));

	return true;
}


//////////////////////////////////////////////////
//	内部関数
//////////////////////////////////////////////////

// メッシュ切り替え時の初期化
bool CheckControl::initMesh( unsigned long ulNextMesh, const CString& meshFeaturClassName, IGeometryPtr& ipMeshGeo, long &meshID )
{
	// メッシュフィーチャ取得
	IFeaturePtr ipMeshFeature = getMeshFeature( ulNextMesh, meshFeaturClassName );
	if( !ipMeshFeature )
	{
		printProgress( message::getMsg( eFailGetMeshFeature ), ulNextMesh );
		return false;
	}

	if( S_OK != ipMeshFeature->get_Shape( &ipMeshGeo ) || !ipMeshGeo )
	{
		printProgress( message::getMsg( eFailGetMeshGeometry ), ulNextMesh );
		return false;
	}

	ipMeshFeature->get_OID( &meshID );
	return true;
}

// メッシュフィーチャ取得
IFeaturePtr CheckControl::getMeshFeature( unsigned long ulMeshCode, const CString& meshFeaturClassName )
{
	CString strWhere;
	strWhere.Format( _T("%s=%u"), category::mesh_code_table::kMeshCode, ulMeshCode );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(strWhere) );

	CString strWorkspaceKey = DB_MESH;
	IFeatureClassPtr ipMeshFC = m_tableFinder.findTable( strWorkspaceKey, meshFeaturClassName );

	if( !ipMeshFC ) return nullptr;
	
	IFeatureCursorPtr ipFeatureCursor;
	ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );

	IFeaturePtr ipFeature;
	if( ipFeatureCursor && ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		return ipFeature;

	return nullptr;
}

// チェックレイヤインスタンスセット
bool CheckControl::setLayerInstance( const CString& layer )
{
	shared_ptr<LayerCheckBase> ipLayerCheck = setLayerCheckInstance( layer );
	if( !ipLayerCheck) return false;

	// ErrorLog の初期化
	CString path;
	path.Format(_T("%s\\%s"), m_logDir, m_settingFile.getLayerLogName(layer));
	m_errorLogList[layer] = shared_ptr<ErrLogger>( new ErrLogger() );

	if( !m_errorLogList[layer]->initialize( path ) )
	{
		printProgress( message::getMsg( eFailLoadLogFile ) );
		return false;
	}

	if( !ipLayerCheck->init( m_settingFile, m_tableFinder, *m_errorLogList[layer], m_textAddressMasterPath ) )
	{
		printProgress( message::getMsg( iLayerExcept ), layer);
		return true;
	}
	if( layer.Compare( addr_polygon::kTableName ) == 0 || layer.Compare( addr_point::kTableName ) == 0 )	m_cityListLayerCheckInstance.push_back( ipLayerCheck );
	if( layer.Compare(city_polygon::kTableName) == 0 || layer.Compare(placename_rep_point::kTableName) == 0 )	m_baseListLayerCheckInstance.push_back( ipLayerCheck );
	return true;	
}

// チェック
bool CheckControl::doCheck()
{
	boost::timer time; 

	// 都市メッシュデータを使って実行するチエック
	if( !check( m_cityMeshCodeList, citymesh::kTableName, m_cityListLayerCheckInstance ) )	return false;

	// ベースメッシュデータを使って実行するチエック
	if( !check( m_baseMeshCodeList, basemesh::kTableName, m_baseListLayerCheckInstance ) )	return false;
	
	m_tableFinder.reset();
	m_cityListLayerCheckInstance.clear();
	m_baseListLayerCheckInstance.clear();
	m_errorLogList.clear();

	HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	CString memory;
	memory.Format(_T("メモリ使用量の最大値: %dK  \n "), pmc.PeakWorkingSetSize/1024 );

	printProgress( memory );
	printProgress( getMsg( iProcessingTime ), time.elapsed() );
	printProgress( getMsg( iCompleteCheck ));

	return true;
}

// チェック
bool CheckControl::check( const MeshCodeList& meshCodeList, const CString& meshFeatureClassName, CHECKLIST& currentCheckLayerList )
{
	if( currentCheckLayerList.empty() )	return true;

	for(const auto& itrMesh : meshCodeList.getMeshCodeList() )
	{
		IGeometryPtr ipMeshGeo;
		long meshID(0L);
		if( initMesh(itrMesh, meshFeatureClassName, ipMeshGeo, meshID) )
		{
			printProgress( getMsg( iStartMesh ), itrMesh );

			// ここで更にチェック分のループを行う
			for( const auto itrCheck : currentCheckLayerList )
			{
				CString layerCheckName = itrCheck->getLayerCheckName();
				printProgress( getMsg( iStartCheck ), layerCheckName );
				DWORD dwStart = GetTickCount();

				// レイヤをチェックする
				itrCheck->checkLayer( itrMesh, ipMeshGeo, meshID );

				double check_time = ( (double)GetTickCount() - (double)dwStart ) / 1000.0;
				printProgress( getMsg( iFinishCheck ), layerCheckName, check_time );
			}

			printProgress( getMsg( iFinishMesh ), itrMesh );
		}
		else
		{
			printProgress( getMsg( iInterruptCheck ));
			return false;
		}
	}
	return true;
}