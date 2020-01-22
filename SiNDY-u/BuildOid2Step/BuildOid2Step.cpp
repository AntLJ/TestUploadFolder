#include "stdafx.h"
#include <sindy/libschema.h>
#include <sindy/workspace.h>
#define _USE_SINDYLIB_STATIC	// sindylib_baseをスタティックリンクするために必要
#include <TDC/sindylib_base/libsindylibbase.h>
#include <crd_cnv/coord_converter.h>
#include <WinLib/libwinlib.h>
#include <ArcHelperEx/libarchelper.h>

#include "BuildOid2Step.h"
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/scope_guard.h>

using namespace std;
using namespace sindy::schema;
using namespace uh;
using namespace uh::str_util;

// 初期化
bool CBuildOid2Step::init()
{
	if( !ERRLOGGER.Initialize( m_args.m_err_log.c_str() ) )
	{
		cerr << "エラーログファイルのオープンに失敗" << endl;
		return false;
	}
	if( !RUNLOGGER.Initialize( m_args.m_run_log.c_str() ) )
	{
		cerr << "実行ログファイルのオープンに失敗" << endl;
		return false;
	}
	RUNLOGGER.writeOption( m_args );
	RUNLOGGER.writeInfo( _T( "開始時刻" ) );

	// ワークスペース取得
	if( !openWorkspace( m_args.m_db.c_str(), m_ipWorkspace ) ||
		!openWorkspace( m_args.m_mesh_db.c_str(), m_ipMeshWorkspace ) )
		return false;

	switch( m_ipWorkspace->GetType() )
	{
		case esriRemoteDatabaseWorkspace:
		{
			// デフォルトバージョンは処理中止
			if( isDefaultVersion( m_ipWorkspace ) )
				return false;
			break;
		}
		case esriLocalDatabaseWorkspace:
			break;
		default:
			return false;
	}

	// フィーチャクラス取得
	if( !openFeatureClass( m_ipWorkspace, building::kTableName, m_ipBuilding, m_BuildingFieldMap ) ||
		!openFeatureClass( m_ipWorkspace, building_step::kTableName, m_ipBuildingStep, m_BuildingStepFieldMap ) )
		return false;

	// BUILDING_ID有無確認
	if( 0 > m_BuildingStepFieldMap.FindField( global::building_step::kBuildingID ) )
	{
		RUNLOGGER.writeInfo( format( _T( "%sが存在しない" ), global::building_step::kBuildingID ) );
		return false;
	}

	// メッシュリスト取得
	if( !createMeshList( m_args.m_mesh_list.c_str(), m_meshList ) )
	{
		RUNLOGGER.writeInfo( _T( "メッシュリスト取得失敗" ) );
		return false;
	}

	// メッシュフィーチャクラス取得
	auto itr = m_meshList.begin();
	long length = static_cast<long>(log10(*itr)+1);
	switch( length )
	{
		case 8: // CITYMESH
		{
			if( !openFeatureClass( m_ipWorkspace, citymesh::kTableName, m_ipMeshFc, m_meshFieldMap ) )
				return false;
			break;
		}
		case 10: // TERTIARYMESH
		{
			if( !openFeatureClass( m_ipWorkspace, global::citymesh::kTableName, m_ipMeshFc, m_meshFieldMap ) )
				return false;
			break;
		}
		default:
		{
			RUNLOGGER.writeInfo( _T( "メッシュ桁数が不正" ) );
			return false;
		}
	}

	// 空間参照取得
	IGeoDatasetPtr( m_ipBuilding )->get_SpatialReference( &m_ipSpatialRef );

	// バッファ距離指定
	m_bufDistance = m_args.m_buf_distance;

	return true;
}

// 実行
bool CBuildOid2Step::execute()
{
	// 初期化
	if( !init() )
		return false;

	// ここのスコープを抜けたら強制的にabort
	uh::scope_guard scope( bind( abortEdit, m_ipWorkspace ) );

	// メッシュリストから形状を取得
	if( !getMeshPolygon( m_meshList, m_meshGeoMap ) )
		return false;

	// メッシュ分ループ
	long count = 0;
	for( const auto& mesh : m_meshGeoMap )
	{
		cerr << "メッシュ処理中..."+to_string(mesh.first)+" : "+to_string(++count)+" / "+to_string(m_meshGeoMap.size()) << endl;

		// メッシュにバッファ付与
		IGeometryPtr ipBufferedMesh = createBuffer( mesh.second, m_bufDistance );
		ipBufferedMesh->putref_SpatialReference( m_ipSpatialRef );

		BGRTree rTree;
		FeatureMap mapBldIdFeature;

		// 編集開始
		if( !startEdit( m_ipWorkspace ) )
			return false;

		// 検索・更新
		if( !searchBuild( ipBufferedMesh, rTree, mapBldIdFeature ) ||
			!updateStep( mesh, ipBufferedMesh, rTree, mapBldIdFeature ) )
		{
			stopEdit( m_ipWorkspace );
			return false;
		}

		// 編集終了
		if( !stopEdit( m_ipWorkspace ) )
			return false;
	}

	return true;
}

bool CBuildOid2Step::searchBuild( const IGeometryPtr& ipBufferedMesh, BGRTree& rTree, FeatureMap& featureMap )
{
	// 空間検索：建物
	IFeatureCursorPtr ipFeatureCur;
	if( FAILED( m_ipBuilding->Search( AheInitSpatialFilter( nullptr, ipBufferedMesh ), VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), building::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
	{
		long oid = ipFeature->GetOID();
		IGeometryPtr ipGeometry = ipFeature->GetShape();

		// RTree作成
		rTree.insert( make_pair( getBox( ipGeometry ), static_cast<unsigned int>( oid ) ) );
		featureMap.emplace( oid, ipFeature );
	}
	return true;
}

bool CBuildOid2Step::updateStep( const MeshGeo& mesh, const IGeometryPtr& ipBufferedMesh, const BGRTree& rTree, const FeatureMap& featureMap )
{
	// 空間検索：階数
	IFeatureCursorPtr ipUpdateCur;
	if( FAILED( m_ipBuildingStep->Update( AheInitSpatialFilter( nullptr, ipBufferedMesh ), VARIANT_FALSE, &ipUpdateCur ) ) || !ipUpdateCur )
	{
		RUNLOGGER.writeInfo( format( _T( "%s検索失敗" ), building_step::kTableName ) );
		return false;
	}

	IFeaturePtr ipFeature;
	while( S_OK == ipUpdateCur->NextFeature( &ipFeature ) && ipFeature )
	{
		long oid = ipFeature->GetOID();
		IGeometryPtr ipGeometry = ipFeature->GetShape();

		// 当該メッシュと Disjoint or Touches なら無関係かもしれないのでスキップ
		if( AheIsDisjoint( mesh.second, ipGeometry ) || AheIsTouch( mesh.second, ipGeometry ) ) continue;

		// 実際の空間関係を分析
		long bldOid = -1;
		if( !analyzeRefByRTree( ipGeometry, rTree, featureMap, bldOid ) )
		{
			ERRLOGGER.error( building_step::kTableName, ToString( oid ),
				_T( "完全包括される建物ポリゴンが見つからない" ), ToString( mesh.first ) );
			continue;
		}

		// 紐づく建物のオブジェクトIDと、自身のBUILDING_IDが等しければ更新不要
		CComVariant var = ipFeature->GetValue( m_BuildingStepFieldMap.HasField( global::building_step::kBuildingID ) );
		if( ( VT_I4 == var.vt ) && ( bldOid == var.lVal ) ) continue;

		// 更新
		if( FAILED( ipFeature->put_Value( m_BuildingStepFieldMap.FindField( global::building_step::kBuildingID ), CComVariant( bldOid ) ) ) ||
			FAILED( ipUpdateCur->UpdateFeature( ipFeature ) ) )
		{
			ERRLOGGER.error( building_step::kTableName, ToString( oid ),
				_T( "更新失敗" ), ToString( mesh.first ) );
		}
	}
	return true;
}

bool CBuildOid2Step::analyzeRefByRTree( const IGeometryPtr& ipGeometry, const BGRTree& rTree, const FeatureMap& featureMap, long& id )
{
	// RTreeを検索して階数に紐づく建物を探す
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( getBox( ipGeometry ) ), back_inserter( results ) );

	// 引っかかった建物でループ
	for( const auto& result : results )
	{
		long bldOid = static_cast<long>( result.second );

		IFeaturePtr ipBldFeature = featureMap.at( bldOid );
		IGeometryPtr ipBldGeo = ipBldFeature->GetShape();
		if( !ipBldGeo )
			continue;

		// 建物に対し階数が Equals or Contains であればOK
		if( !AheIsContain2( ipBldGeo, ipGeometry ) )
			continue;

		id = bldOid;
		break; // 階数からみて建物は1つしか紐付かないのでbreak
	}

	return id > 0 ;
}

BGBox CBuildOid2Step::getBox( const IGeometryPtr& ipGeometry )
{
	IEnvelopePtr ipEnvelope;
	ipGeometry->get_Envelope( &ipEnvelope );

	double XMin = 0.0, YMin = 0.0, XMax = 0.0, YMax = 0.0;
	ipEnvelope->QueryCoords( &XMin, &YMin, &XMax, &YMax );

	return BGBox( BGPoint( XMin, YMin ), BGPoint( XMax, YMax ) ) ;
}

bool CBuildOid2Step::getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap )
{
	long count = 0; // 検索完了メッシュ数
	CString subFieleds = format( _T( "%s,%s" ), citymesh::kMeshCode, feature::kShape );

	// 1000メッシュ毎に検索
	for( const auto& oidString : enumerate_elements( meshList ) )
	{
		IFeatureCursorPtr ipFeatureCur;
		if( FAILED( m_ipMeshFc->Search(
			AheInitQueryFilter( nullptr, subFieleds, _T( "%s IN (%s)" ), citymesh::kMeshCode, CString( oidString.c_str() ) ),
			VARIANT_FALSE, &ipFeatureCur ) ) || !ipFeatureCur )
		{
			RUNLOGGER.writeInfo( _T( "メッシュ検索失敗" ) );
			return false;
		}

		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCur->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant var = ipFeature->GetValue( m_meshFieldMap.FindField( citymesh::kMeshCode ) );
			var.ChangeType( VT_UI4 );
			meshGeoMap.emplace( var.uintVal, ipFeature->GetShape() );
			++count;
		}
		system("cls");
		cerr << "メッシュ検索完了... "+to_string(count)+" / "+to_string(meshList.size()) << endl;
	}
	return true;
}

IGeometryPtr CBuildOid2Step::createBuffer( const IGeometryPtr& ipGeometry, double buf )
{
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( buf, esriMeters, esriDecimalDegrees, &dConvRange );
	ITopologicalOperatorPtr ipTopo( ipGeometry );

	IGeometryPtr ipBufferedGeo;
	ipTopo->Buffer( dConvRange, &ipBufferedGeo );

	return ipBufferedGeo;
}

bool CBuildOid2Step::openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace )
{
	ipWorkspace = sindy::create_workspace( _bstr_t( db ) );
	if( !ipWorkspace )
	{
		RUNLOGGER.writeInfo( format( _T( "%s接続失敗" ), db ) );
		return false;
	}

	return true;
}

bool CBuildOid2Step::isDefaultVersion( const IWorkspacePtr& ipWorkspace )
{
	IVersionedWorkspacePtr ipVerWorkspace( ipWorkspace );
	if( !ipVerWorkspace ) return false;

	IVersionPtr ipVersion( ipWorkspace );
	CComBSTR bstr;
	ipVersion->get_VersionName( &bstr );
	CString strVerName = COLE2T( bstr );
	if( strVerName.Find( _T( "DEFAULT" ) ) != -1 )
	{
		RUNLOGGER.writeInfo( _T( "接続先がDEFAULTバージョン" ) );
		return true;
	}

	return false;
}

bool CBuildOid2Step::createMeshList( const CString& fileName, MeshList& meshList )
{
	if( fileName.IsEmpty() ) return false;

	ifstream ifs( fileName );
	if( !ifs ) return false;

	string line;
	while( getline( ifs, line ) )
	{
		// 空の行や行頭「#」は無視
		if( line.size() == 0 || line.at( 0 ) == '#' ) continue;

		meshList.emplace( stoul(line) );
	}
	if( ifs.fail() && !ifs.eof() ) return false;

	return true;
}

bool CBuildOid2Step::openFeatureClass( const IWorkspacePtr& ipWorkspace, const CString& layerName, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap )
{
	ipFeatureClass = ( IFeatureWorkspacePtr( ipWorkspace )->_OpenFeatureClass( _bstr_t( layerName ) ) );
	if( !ipFeatureClass )
	{
		RUNLOGGER.writeInfo( format( _T( "%s取得失敗" ), layerName ) );
		return false;
	}
	fieldMap.CreateFieldMap( ipFeatureClass->GetFields() );

	return true;
}

bool CBuildOid2Step::isVersioned( const IFeatureClassPtr& ipFeatureClass )
{
	VARIANT_BOOL vbIsVersioned = VARIANT_FALSE;
	IVersionedObjectPtr( ipFeatureClass )->get_IsRegisteredAsVersioned( &vbIsVersioned );

	return vbIsVersioned ? true : false;
}

bool CBuildOid2Step::startEdit( IWorkspacePtr& ipWorkspace )
{
	IMultiuserWorkspaceEditPtr ipMultiuserWorkspace( ipWorkspace );
	if( ipMultiuserWorkspace )
	{
		if( FAILED( ipMultiuserWorkspace->StartMultiuserEditing( isVersioned( m_ipBuildingStep ) ? esriMESMVersioned : esriMESMNonVersioned ) ) )
		{
			RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
			return false;
		}
	}
	else
	{
		if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditing( VARIANT_FALSE ) ) )
		{
			RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
			return false;
		}
	}

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditOperation() ) )
	{
		RUNLOGGER.writeInfo( _T( "編集開始失敗" ) );
		return false;
	}

	return true;
}

bool CBuildOid2Step::stopEdit( IWorkspacePtr& ipWorkspace )
{
	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->StartEditOperation() ) ||
		FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_TRUE ) ) )
	{
		RUNLOGGER.writeInfo( _T( "編集終了失敗" ) );
		abortEdit( ipWorkspace );
		return false;
	}

	return true;
}

void CBuildOid2Step::abortEdit( IWorkspacePtr& ipWorkspace )
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	IWorkspaceEditPtr(ipWorkspace)->IsBeingEdited( &vb );
	if( !vb ) return;

	if( FAILED( IWorkspaceEditPtr( ipWorkspace )->AbortEditOperation() ) ||
		FAILED( IWorkspaceEditPtr( ipWorkspace )->StopEditing( VARIANT_FALSE ) ) )
		RUNLOGGER.writeInfo( _T( "編集破棄失敗" ) );
}