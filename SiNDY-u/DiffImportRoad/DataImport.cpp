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
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h>
#include <libarchelper.h>
#define _USE_SINDYLIB_STATIC
#include <libsindylibcore.h>
#include <libsindylibbase.h>
#include <coord_converter.h>
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/workspace.h>
#include <schema.h>
#include "global.h"
#include "DataImport.h"

#include "Workspace.h"
#include "VersionInfo.h"
#include <TDC/useful_headers/str_util.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace sindy;
using namespace sindy::schema;

const long g_lReconcileCount = 20;		//!< リコンサイル回数

bool CImport::run()
{
	// インポート用のテーブル初期化
	setTableType();

	if(!m_cInputInfo.strExceptList.IsEmpty())
	{
		if(!readExceptFieldList())
			return false;
	}

	// 作業用子バージョン名作成
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
#ifdef _DEBUG
	m_strWorkVersionName.Format(_T("DiffImportRoad_%04d%02d%02d_%02d%02d%02d_for_post_debug"), 
#else
	m_strWorkVersionName.Format(_T("DiffImportRoad_%04d%02d%02d_%02d%02d%02d_for_post"), 
#endif
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	// ツール更新日作成
	m_strProgModifyDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), 
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// ログファイル名設定
	CString strLogFileName;
	strLogFileName.Format(_T("%s\\log_%04d%02d%02d_%02d%02d%02d.txt"), 
		m_cInputInfo.strLogDir,
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// エラーログファイル名設定
	CString strErrLogFileName;
	strErrLogFileName.Format(_T("%s\\err_%04d%02d%02d_%02d%02d%02d.txt"), 
		m_cInputInfo.strLogDir,
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	// ログファイル作成
	m_ofs.open(strLogFileName);
	if( m_ofs.fail() )
	{
		writeErrorLog( ERR, strLogFileName, _T("ログファイルオープン失敗"), _T(""), 0 );
		return false;
	}
	m_ofs << "# FREESTYLELOG" << endl;
	m_ofs << "FLAG\tCOMMENT{処理メッシュ}\tCOMMENT{接続先}\tLAYER\tOBJECTID\tCOMMENT{インポート可否}\tCOMMENT{状況}\t" << endl;

	// エラーログファイル作成
	m_ofsErr.open(strErrLogFileName);
	if( m_ofsErr.fail() )
	{
		writeErrorLog( ERR, strErrLogFileName, _T("エラーログファイルオープン失敗"), _T(""), 0 );
		return false;
	}

	// ツール名、ツールバージョン取得
	CVersion cToolVersion;
	cout << "#" << CT2A(cToolVersion.GetOriginalFilename()) << "\tFILEVERSION : " << CT2A(cToolVersion.GetFileVersion()) << "\tPRODUCTVERSION : " << CT2A(cToolVersion.GetProductVersion()) << endl;
	m_strToolName = cToolVersion.GetOriginalFilename();

	// メッシュ単位で処理を行う
	long lCount = 0, lSize = m_cInputInfo.setMesh.size();
	for( auto& mesh : m_cInputInfo.setMesh )
	{
		// 初期化
		m_mapData2Update.clear();
		m_mapUpdate2Data.clear();
		m_mapPGDBOldID2SDENewID.clear();
		m_vecImportLog.clear();

		{
#ifdef _DEBUG
			CFuncTraceWrapper ft( g_timetrace, _T("CImport::run") );
#endif

			lCount++;
			cout << "■ " << lCount << " / " << lSize << " 件 ( " << mesh << " ) \t" << CT2A(getNowTime()) << endl;
			cout << "　処理中断は「P」、処理終了は「Q」、強制終了は「Ctrl+C」" << endl;

			m_lMeshCode = mesh;
			CString strBeforePath, strAfterPath;
			strBeforePath.Format( _T("%s\\%ld\\%ld.mdb"), m_cInputInfo.strBeforeDir, long(mesh/100), mesh );
			strAfterPath.Format( _T("%s\\%ld\\%ld.mdb"), m_cInputInfo.strAfterDir, long(mesh/100), mesh );

			IWorkspacePtr ipCurrentSDEWork = create_workspace( m_cInputInfo.strSDEConnect );
			IWorkspacePtr ipBeforeWork = create_workspace( strBeforePath );
			IWorkspacePtr ipAfterWork = create_workspace( strAfterPath );

			if( !ipBeforeWork || !ipAfterWork || !ipCurrentSDEWork )
			{
				// ワークスペースがNULLってたらダメでしょう、、
				writeErrorLog( ERR, _T(""), _T("ワークスペース取得失敗"), _T(""), 0 );
				return false;
			}
			// 作業用子バージョン作成＋接続
			IWorkspacePtr ipSDEWork;
			if( !createWorkVersion( ipCurrentSDEWork, ipSDEWork ) )
			{
				// エラーメッセージは関数内で出力済み
				writeErrorLog( ERR, _T(""), _T("作業用子バージョン接続失敗"), _T(""), 0 );
				return false;
			}

			// SDEのデータ取得
			CNWData cSDEData;
			if( !cSDEData.Init_SDE( m_mapTableMap, ipSDEWork, m_exceptFieldMap ) ) // sde用はフィーチャクラス、テーブルオープンのみ
			{
				writeErrorLog( ERR, _T(""), _T("SDEデータ取得失敗"), _T(""), 0 );
				return false;
			}

			// 空間参照設定
			m_ipSpRef = cSDEData.GetSpatialReference();

			// メッシュ形状取得
			IGeometryPtr ipMeshGeom;
			if( !getMeshGeom( mesh, ipMeshGeom ) )
			{
				writeErrorLog( ERR, _T(""), _T("メッシュ形状取得失敗"), _T(""), 0 );
				return false;
			}

			// 新旧PGDB＋SDEのデータ取得
			CNWData cBeforeData, cAfterData;
			if( !cBeforeData.Init( m_mapTableMap, ipBeforeWork, mesh, ipMeshGeom, m_exceptFieldMap ) ||
				!cAfterData.Init( m_mapTableMap, ipAfterWork, mesh, ipMeshGeom, m_exceptFieldMap ) )
			{
				writeErrorLog( ERR, _T(""), _T("編集前PGDB、編集後PGDB"), _T(""), 0 );
				continue;
			}

			// 更新情報セット
			if( !setUpdateData( cBeforeData, cAfterData ) )
			{
				writeErrorLog( ERR, _T(""), _T("更新情報取得失敗"), _T(""), 0 );
				continue;
			}

			// SDEのデータと比較し、コンフリクトを起こしていないかチェック
			if( !setConflictData( cBeforeData, cAfterData, cSDEData ) )
			{
				writeErrorLog( ERR, _T(""), _T("コンフリクトチェック失敗"), _T(""), 0 );
				continue;
			}

			// ここでようやく更新情報をSDEに反映する
			if( !reflectData( cBeforeData, cAfterData, cSDEData ) )
			{
				writeErrorLog( ERR, _T(""), _T("更新情報反映失敗"), _T(""), 0 );
				continue;
			}
			// 最後の最後でログ出力
			for( const auto& importlog : m_vecImportLog )
			{
				m_ofs << CT2A(importlog) << endl;
			}
		}

		g_timetrace.Trace();
		g_timetrace.Report();
		g_timetrace.Clear();

		// 途中中断するか
		if( IsSuspension() )
			break;
	}

	return true;
}

bool CImport::IsSuspension()
{
	// 途中中断など
	if( _kbhit() )
	{
		int iKey = _getch();
		if( iKey == 'Q' )
		{
			// 終了
			cout << "...処理終了！！" << endl;
			writeErrorLog( INFO, _T(""), _T("処理終了"), _T(""), 0 );
			return true;
		}
		else if( iKey == 'P' )
		{
			//中断
			cout << "...処理中断！！" << endl;
			writeErrorLog( INFO, _T(""), _T("処理中断"), _T(""), 0 );
			while( 1 )
			{
				//大文字の「Q」又は「S」以外は受け付けない（CTRL+Cは効いてしまう）
				cout << "「S」キーで再開します。「Q」キーで終了します。" << endl;
				int iContinueKey = _getch();
				if( iContinueKey == 'Q' )
				{
					cout << "...処理終了" << endl;
					writeErrorLog( INFO, _T(""), _T("処理終了"), _T(""), 0 );
					return true;
				}
				else if( iContinueKey == 'S' )
				{
					cout << "...処理再開" << endl;
					writeErrorLog( INFO, _T(""), _T("処理再開"), _T(""), 0 );
					return false;
				}
			}
		}
	}
	return false;
}

void CImport::setTableType()
{
	// インポート用のテーブルリスト
	m_mapTableMap[road_link::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[road_node::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[segment_attr::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[grad::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[base_site::kTableName] = DATATYPE::FEATURECLASS;
	m_mapTableMap[time_oneway::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[time_nopassage::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[simple_grad::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[vehicle_reg::kTableName] = DATATYPE::REL_LINKTABLE;
	m_mapTableMap[vehicle_type::kTableName] = DATATYPE::REL_OTHERTABLE;
	m_mapTableMap[inf_byway::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_dirguide::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_guide::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_internavi::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_intersection::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_lane::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_route::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_turnreg::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_uturn::kTableName] = DATATYPE::INF;
	m_mapTableMap[inf_vics::kTableName] = DATATYPE::INF;
	m_mapTableMap[lq_byway::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_dirguide::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_guide::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_internavi::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_intersection::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_lane::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_route::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_turnreg::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_uturn::kTableName] = DATATYPE::LQ;
	m_mapTableMap[lq_vics::kTableName] = DATATYPE::LQ;

	// コンフリクト検知処理をする順番を設定（順番は重要）
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( road_link::kTableName, REL_LINK ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( road_node::kTableName, REL_NODE ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( segment_attr::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( grad::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( time_oneway::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( time_nopassage::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( simple_grad::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( vehicle_reg::kTableName, REL_AGOPS ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( vehicle_type::kTableName, REL_VEHICLETYPE ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_byway::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_dirguide::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_guide::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_internavi::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_intersection::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_lane::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_route::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_turnreg::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_uturn::kTableName, REL_INF ) );
	m_vecConflictTable.push_back( pair<CString,DATASTATUS>( inf_vics::kTableName, REL_INF ) );

	// SDEにデータ反映処理をする順番を設定（順番は重要）
	m_vecReflectTable.push_back( road_node::kTableName );
	m_vecReflectTable.push_back( road_link::kTableName );
	m_vecReflectTable.push_back( segment_attr::kTableName );
	m_vecReflectTable.push_back( grad::kTableName );
	m_vecReflectTable.push_back( time_oneway::kTableName );
	m_vecReflectTable.push_back( time_nopassage::kTableName );
	m_vecReflectTable.push_back( simple_grad::kTableName );
	m_vecReflectTable.push_back( vehicle_reg::kTableName );
	m_vecReflectTable.push_back( vehicle_type::kTableName );
	m_vecReflectTable.push_back( inf_byway::kTableName );
	m_vecReflectTable.push_back( inf_dirguide::kTableName );
	m_vecReflectTable.push_back( inf_guide::kTableName );
	m_vecReflectTable.push_back( inf_internavi::kTableName );
	m_vecReflectTable.push_back( inf_intersection::kTableName );
	m_vecReflectTable.push_back( inf_lane::kTableName );
	m_vecReflectTable.push_back( inf_route::kTableName );
	m_vecReflectTable.push_back( inf_turnreg::kTableName );
	m_vecReflectTable.push_back( inf_uturn::kTableName );
	m_vecReflectTable.push_back( inf_vics::kTableName );
}

bool CImport::getMeshGeom( long lMeshCode, IGeometryPtr& ipMeshGeom )
{
	// 指定メッシュのメッシュ形状取得
	double dMinX = 0.0, dMinY = 0.0;
	double dMaxX = 0.0, dMaxY = 0.0;
	g_cnv.MeshtoLL( lMeshCode, 0, 0, &dMinX, &dMinY );
	g_cnv.MeshtoLL( lMeshCode, 1000000, 1000000, &dMaxX, &dMaxY );

	IEnvelopePtr ipEnv(CLSID_Envelope);
	ipEnv->PutCoords( dMinX, dMinY, dMaxX, dMaxY );
	ipEnv->putref_SpatialReference( m_ipSpRef );
	ipMeshGeom = ipEnv;

	if( !ipMeshGeom )
		return false;
	else
		return true;
}

bool CImport::setUpdateData( const CNWData& cBeforeData, const CNWData& cAfterData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setUpdateData") );
#endif

	bool bRet = false;

	// 更新、削除データ取得
	for( const auto& tablename : m_mapTableMap )
	{
		map<CString,TableData>::const_iterator itBefore = cBeforeData.m_mapTableData.find( tablename.first );
		map<CString,TableData>::const_iterator itAfter = cAfterData.m_mapTableData.find( tablename.first );
		if( cBeforeData.m_mapTableData.end() == itBefore || cAfterData.m_mapTableData.end() == itAfter )
		{
			_ASSERT(0);
			bRet = false;
			break;
		}

		bool bSetData = true;
		switch( tablename.second )
		{
		case FEATURECLASS:
		case TABLE:
		case REL_LINKTABLE:
		case REL_OTHERTABLE:
		case INF:
			bSetData = setUpdateTableData( tablename.first, itBefore->second, itAfter->second, tablename.second );
			break;
		case LQ:
		default:
			break;
		}

		if( !bSetData )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::setUpdateTableData( const CString& strTableName, const TableData& beforeTableData, const TableData& afterTableData, DATATYPE dataType )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setUpdateTableData") );
#endif
	set<CString> exceptFields = getExceptFieldSet(strTableName);

	// 編集前→編集後で検索
	for( const auto& beforeRow : beforeTableData.mapRow )
	{
		long lBeforeOID = beforeRow.first; // 編集前データのオブジェクトID
		map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lBeforeOID );
		if( afterTableData.mapRow.end() == itAfterRow )
		{
			// 編集前に存在し、編集後に存在しないものは削除データ
			m_mapData2Update[strTableName][lBeforeOID].updateType = UPDATETYPE::DEL;
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = DATASTATUS::UPDATE;
			m_mapUpdate2Data[strTableName][UPDATETYPE::DEL].insert( lBeforeOID );
		}
		else
		{
			bool bUpdateAttr = false;
			bool bUpdateShape = false;
			// 属性が更新されているかチェック
			if(!IsEqualAttributeBySubstance(beforeTableData.cFieldMap, afterTableData.cFieldMap, beforeRow.second, itAfterRow->second, exceptFields, false ))
			{
				bUpdateAttr = true;
			}
			// 形状更新されているかチェック
			switch( dataType )
			{
			case INF:
				{
					// リンク列の場合はLQを比較
					map<long,set<LqData>>::const_iterator itBeforeInf = beforeTableData.mapInfData.find( lBeforeOID );
					map<long,set<LqData>>::const_iterator itAfterInf = afterTableData.mapInfData.find( lBeforeOID );
					if( beforeTableData.mapInfData.end() == itBeforeInf || afterTableData.mapInfData.end() == itAfterInf )
					{
						_ASSERT(0);
						writeErrorLog( ERR, __func__, _T("INFに紐付くLQデータ取得失敗"), strTableName, lBeforeOID );
						return false;
					}
					if( itBeforeInf->second != itAfterInf->second )
						bUpdateShape = true;
					break;
				}
			case FEATURECLASS:
				{
					// フィーチャクラスの場合
					long lShapeIndex = beforeTableData.cFieldMap.GetShapeIndex();
					if( lShapeIndex >= 0 )
					{
						// 形状一致しているかチェック
						IGeometryPtr ipBeforeGeom;
						IGeometryPtr ipAfterGeom;
						IFeaturePtr(beforeRow.second.m_T)->get_ShapeCopy( &ipBeforeGeom );
						IFeaturePtr(itAfterRow->second.m_T)->get_ShapeCopy( &ipAfterGeom );
						if( !ipBeforeGeom || !ipAfterGeom )
						{
							_ASSERT(0);
							writeErrorLog( ERR, __func__, _T("形状取得"), strTableName, lBeforeOID );
							return false;
						}
						if( !IsSameGeom( ipBeforeGeom, ipAfterGeom ) )
							bUpdateShape = true;
					}
				}
				break;
			default:
				break;
			}

			// 更新情報の設定
			UPDATETYPE updateType = UPDATETYPE::SAME;
			DATASTATUS dataStatus = DATASTATUS::UPDATE;
			if( bUpdateAttr && bUpdateShape )        // 属性＋形状更新
				updateType = UPDATETYPE::SHAPE_ATTR;
			else if( !bUpdateAttr && bUpdateShape )  // 形状更新
				updateType = UPDATETYPE::SHAPE;
			else if( bUpdateAttr && !bUpdateShape )  // 属性更新
				updateType = UPDATETYPE::ATTR;
			else                                     // 変更なし
				dataStatus = DATASTATUS::NONE;

			m_mapData2Update[strTableName][lBeforeOID].updateType = updateType;
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = dataStatus;
			m_mapUpdate2Data[strTableName][updateType].insert( lBeforeOID );
		}

		// メッシュ外データだった場合の設定
		if( !beforeTableData.mapInMeshData.find( lBeforeOID )->second )
			m_mapData2Update[strTableName][lBeforeOID].dataStatus = DATASTATUS(m_mapData2Update[strTableName][lBeforeOID].dataStatus | DATASTATUS::OUTMESH);
	}

	// 編集後→編集前で検索
	for( const auto& afterRow : afterTableData.mapRow )
	{
		long lAfterOID = afterRow.first; // 編集後データのオブジェクト
		map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lAfterOID );
		if( beforeTableData.mapRow.end() == itBeforeRow )
		{
			// 編集後に存在し、編集前に存在しないものは新規データ
			m_mapData2Update[strTableName][lAfterOID].updateType = UPDATETYPE::NEW;
			m_mapData2Update[strTableName][lAfterOID].dataStatus = DATASTATUS::UPDATE;
			m_mapUpdate2Data[strTableName][UPDATETYPE::NEW].insert( lAfterOID );

			// メッシュ外データだった場合の設定
			if( !afterTableData.mapInMeshData.find( lAfterOID )->second )
				m_mapData2Update[strTableName][lAfterOID].dataStatus = DATASTATUS(m_mapData2Update[strTableName][lAfterOID].dataStatus | DATASTATUS::OUTMESH);
		}
	}

	return true;
}

bool CImport::IsConflict( const CFieldMap& cSDEFieldMap, const CFieldMap& cPGDBFieldMap, const _IRowPtr& ipSDERow, const _IRowPtr& ipBeforeRow, const _IRowPtr& ipAfterRow, const std::set<CString>& exceptFieldSet )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::IsConflict") );
#endif

	// OPERATORとMODIFYDATEがSDEとPGDBで異なればSDE側のデータは更新されたことになる
	// MODIFYPROGNAMEとPROGMODIFYDATEもコンフリクト条件として追加(bug10363)

	// フィールドインデックス取得
	long lSDEOperatorIndex = cSDEFieldMap.FindField( ipc_table::kOperator );
	long lSDEModifyDateIndex = cSDEFieldMap.FindField( ipc_table::kModifyDate );
	long lSDEModifyProgNameIndex = cSDEFieldMap.FindField( ipc_table::kModifyProgName );	// bug10363
	long lSDEProgModifyDateIndex = cSDEFieldMap.FindField( ipc_table::kProgModifyDate );	// bug10363

	long lPGDBOperatorIndex = cPGDBFieldMap.FindField( category::sindyc_table::kOrgOperator );
	long lPGDBModifyDateIndex = cPGDBFieldMap.FindField( category::sindyc_table::kOrgModifyDate );
	long lPGDBModifyProgNameIndex = cPGDBFieldMap.FindField( ipc_table::kModifyProgName );	// bug10363
	long lPGDBProgModifyDateIndex = cPGDBFieldMap.FindField( ipc_table::kProgModifyDate );	// bug10363

	// ここで失敗することは無いと思うけど、、
	if( lSDEOperatorIndex < 0  || lSDEModifyDateIndex < 0  || lSDEModifyProgNameIndex < 0  || lSDEProgModifyDateIndex < 0
	 || lPGDBOperatorIndex < 0 || lPGDBModifyDateIndex < 0 || lPGDBModifyProgNameIndex < 0 || lPGDBProgModifyDateIndex < 0)
	{
		// TODO:チェックするつもりなら、ほんとに return true でいいの？
		_ASSERT(0);
		writeErrorLog( ERR, __func__, _T("フィールドインデックスがNG"), _T(""), 0 );
		return true;
	}

	CComVariant vaSDEOperator , vaSDEModifyDate , vaSDEModifyProgName , vaSDEProgModifyDate;
	CComVariant vaPGDBOperator, vaPGDBModifyDate, vaPGDBModifyProgName, vaPGDBProgModifyDate;

	ipSDERow->get_Value( lSDEOperatorIndex, &vaSDEOperator );
	ipSDERow->get_Value( lSDEModifyDateIndex, &vaSDEModifyDate );
	ipSDERow->get_Value( lSDEModifyProgNameIndex, &vaSDEModifyProgName );
	ipSDERow->get_Value( lSDEProgModifyDateIndex, &vaSDEProgModifyDate );

	ipBeforeRow->get_Value( lPGDBOperatorIndex, &vaPGDBOperator );
	ipBeforeRow->get_Value( lPGDBModifyDateIndex, &vaPGDBModifyDate );
	ipBeforeRow->get_Value( lPGDBModifyProgNameIndex, &vaPGDBModifyProgName );
	ipBeforeRow->get_Value( lPGDBProgModifyDateIndex, &vaPGDBProgModifyDate );

	// 一致していればコンフリクトしていない
	if( vaSDEOperator == vaPGDBOperator && vaSDEModifyDate == vaPGDBModifyDate
	 && vaSDEModifyProgName == vaPGDBModifyProgName && vaSDEProgModifyDate == vaPGDBProgModifyDate )
		return false;

	// 除外フィールドが空でないなら、beforeの属性とSDEの属性を見る
	if(!exceptFieldSet.empty())
	{
		if(!IsEqualAttributeBySubstance(cSDEFieldMap, cPGDBFieldMap, ipSDERow, ipBeforeRow, exceptFieldSet))
			return true;

		// 形状持ちだった場合は、形状も調べる
		if(!(cSDEFieldMap.GetShapeIndex()<0))
		{
			IRelationalOperatorPtr ipRelOp(((IFeaturePtr)ipSDERow)->GetShape());
			IGeometryPtr ipPGDBGeom = ((IFeaturePtr)ipBeforeRow)->GetShape();
			VARIANT_BOOL vbEqual = ipRelOp->_Equals(ipPGDBGeom);
			if(vbEqual != VARIANT_TRUE)
				return true;
		}
		return false;
	}

	// コンフリクトしている
	return true;
}

bool CImport::IsEqualAttributeBySubstance(
	const CFieldMap& srcFieldMap,
	const CFieldMap& dstFieldMap,
	const _IRowPtr& ipSrcRow,
	const _IRowPtr& ipDstRow,
	const std::set<CString>& exceptFieldSet,
	bool  sdeCheck /*= true*/ )
{
	int fieldNum = srcFieldMap.GetFieldCount();

	for(long i=0; i<fieldNum; ++i)
	{
		// 編集不可のもの、形状は無視する
		if(!srcFieldMap.IsEditable(i) || srcFieldMap.IsShapeField(i))
			continue;

		CString fieldName = srcFieldMap.GetName(i);

		// PGDB同士の比較では、OPERATORとMODIFYDATEは別のところで見ているし、
		// SDEとPGDBの比較では、これは元々インポート対象外なので除外してOK
		if( fieldName.CompareNoCase(ipc_table::kModifyDate) == 0
		 || fieldName.CompareNoCase(ipc_table::kOperator) == 0)
			continue;

		// PGDB同士でなく、SDEとPGDBの比較の場合
		if(sdeCheck)
		{
			// PGDBとSDEで合致しない属性(関連テーブルのID)、あるいは元々インポートされない属性は無視する
			if(fieldName.CompareNoCase(road_link::kFromNodeID) == 0
			|| fieldName.CompareNoCase(road_link::kToNodeID) == 0
			|| fieldName.CompareNoCase(link_queue::kInfID) == 0
			|| fieldName.CompareNoCase(link_queue::kLinkID) == 0
			|| fieldName.CompareNoCase(ipc_table::kPurpose) == 0
			|| fieldName.CompareNoCase(ipc_table::kProgModifyDate) == 0
			|| fieldName.CompareNoCase(ipc_table::kModifyProgName) == 0)
				continue;
		}

		// 属性インポートの除外フィールドは無視する
		if(exceptFieldSet.find(fieldName) != exceptFieldSet.end())
		{
			continue;
		}
		// 対応の取れない属性は無視する
		long dstField = dstFieldMap.FindField(fieldName);
		if(dstField<0)
		{
			continue;
		}

		// 上記はインポートに関係ない属性群。それ以外で差異があれば等価ではない
		CComVariant vaSrcVal = ipSrcRow->GetValue(i);
		CComVariant vaDstVal = ipDstRow->GetValue(dstField);
		if(vaSrcVal != vaDstVal)
		{
			return false;
		}
	}
	return true;
}

bool CImport::setConflictData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictData") );
#endif

	for( const auto& conflictTable : m_vecConflictTable )
	{
		if( !setConflictTableData( conflictTable.second, conflictTable.first, cBeforeData, cAfterData, cSDEData ) )
		{
			return false;
		}
	}

	// 2次メッシュノードに紐付く更新予定のデータは更新不可にする
	if( !setNGDataForEdgeNode( DATASTATUS::REL_EDGENODE, cBeforeData, cAfterData, cSDEData ) )
	{
		writeErrorLog( ERR, __func__, _T("2次メッシュ境界に紐付くデータのチェック失敗"), _T(""), 0 );
		return false;
	}
	
	// ノードIDが0のFromToノードIDを持つリンクに紐付く更新予定のデータは更新不可にする
	if( !setNGDataForIDZeroNodeLink( DATASTATUS::REL_ZERONODE, cBeforeData, cAfterData, cSDEData ) )
	{
		writeErrorLog( ERR, __func__, _T("FromToノードIDが0のリンクのチェック失敗"), _T(""), 0 );
		return false;
	}

	return true;
}

bool CImport::setConflictTableData( DATASTATUS dataStatus, const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictTableData") );
#endif

	set<long> setNGLink; //!< 更新不可にしたリンク
	set<long> setSearchedLink; //!< 探索済みリンク

	// 削除データのコンフリクト状態セット
	if( !setConflictTableData2( strTableName, UPDATETYPE::DEL, cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) )
	{
		writeErrorLog( ERR, __func__, _T("削除データのコンフリクトチェック失敗"), strTableName, 0 );
		return false;
	}
	// 関連する道路ノード、リンク内属性、傾斜、通行禁止、一方通行、簡易傾斜、リンク列が更新予定場合は更新不可にする
	if( !setUpdateNGData( dataStatus, setNGLink, cBeforeData ) )
	{
		writeErrorLog( ERR, __func__, _T("属性、形状更新データに関連するデータの更新ステータス変更失敗"), strTableName, 0 );
		return false;
	}

	// NGリンククリア
	setNGLink.clear();

	// 形状更新、属性更新、形状＋属性更新データのコンフリクト状態セット
	if( !setConflictTableData2( strTableName, UPDATETYPE::SHAPE     , cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) ||
		!setConflictTableData2( strTableName, UPDATETYPE::ATTR      , cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ) ||
		!setConflictTableData2( strTableName, UPDATETYPE::SHAPE_ATTR, cBeforeData, cAfterData, cSDEData, setSearchedLink, setNGLink ))
	{
		writeErrorLog( ERR, __func__, _T("属性、形状更新データのコンフリクトチェック失敗"), strTableName, 0 );
		return false;
	}
	// 関連する道路ノード、リンク内属性、傾斜、通行禁止、一方通行、簡易傾斜、リンク列が更新予定場合は更新不可にする
	if( !setUpdateNGData( dataStatus, setNGLink, cAfterData ) )
	{
		writeErrorLog( ERR, __func__, _T("属性、形状更新データに関連するデータの更新ステータス変更失敗"), strTableName, 0 );
		return false;
	}

	// 新規分は↑の処理で更新不可対象にならなければOK

	return true;
}

bool CImport::setNGDataForEdgeNode( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setNGDataForEdgeNode") );
#endif

	set<long> setBeforeNGLink, setAfterNGLink; //!< 更新不可にしたリンク
	set<long> setBeforeSearchedLink, setAfterSearchedLink; //!< 探索済みリンク

	for( const auto& edgenode : cBeforeData.m_setMeshNode )
	{
		// メッシュノードに紐付く更新対象リンクは更新させない
		if( !searchNoUpdateLink( dataStatus, edgenode, cBeforeData.m_mapLink2FromToNode, cBeforeData.m_mapNode2Link, setBeforeSearchedLink, setBeforeNGLink ) )
		{
			return false;
		}
	}
	for( const auto& edgenode : cAfterData.m_setMeshNode )
	{
		// メッシュノードに紐付く更新対象リンクは更新させない
		if( !searchNoUpdateLink( dataStatus, edgenode, cAfterData.m_mapLink2FromToNode, cAfterData.m_mapNode2Link, setAfterSearchedLink, setAfterNGLink ) )
		{
			return false;
		}
	}

	// 関連する道路ノード、リンク内属性、傾斜、通行禁止、一方通行、簡易傾斜、リンク列が更新予定場合は更新不可にする
	if( !setUpdateNGData( dataStatus, setBeforeNGLink, cBeforeData )
	 || !setUpdateNGData( dataStatus, setAfterNGLink, cAfterData ) )
	{
		// エラー
		return false;
	}

	return true;
}

bool CImport::setNGDataForIDZeroNodeLink( DATASTATUS dataStatus, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setNGDataForIDZeroNodeLink") );
#endif

	set<long> setBeforeNGLink, setAfterNGLink; //!< 更新不可にしたリンク
	set<long> setBeforeSearchedLink, setAfterSearchedLink; //!< 探索済みリンク

	if( cBeforeData.m_mapNode2Link.end() != cBeforeData.m_mapNode2Link.find( 0 ) )
	{
		if( !searchNoUpdateLink( dataStatus, 0, cBeforeData.m_mapLink2FromToNode, cBeforeData.m_mapNode2Link, setBeforeSearchedLink, setBeforeNGLink ) )
		{
			return false;
		}
	}
	if( cAfterData.m_mapNode2Link.end() != cAfterData.m_mapNode2Link.find( 0 ) )
	{
		if( !searchNoUpdateLink( dataStatus, 0, cAfterData.m_mapLink2FromToNode, cAfterData.m_mapNode2Link, setAfterSearchedLink, setAfterNGLink ) )
		{
			return false;
		}
	}

	// 関連する道路ノード、リンク内属性、傾斜、通行禁止、一方通行、簡易傾斜、リンク列が更新予定場合は更新不可にする
	if( !setUpdateNGData( dataStatus, setBeforeNGLink, cBeforeData )
	 || !setUpdateNGData( dataStatus, setAfterNGLink, cAfterData ) )
	{
		// エラー
		return false;
	}

	return true;
}
bool CImport::setUpdateNGData( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	// リンク内属性、傾斜、通行禁止、一方通行、簡易傾斜、リンク列が更新予定場合は更新不可にする
	for( const auto& tableName : m_mapTableMap )
	{
		if( 0 == _tcsicmp( tableName.first, road_node::kTableName ) )
		{
			if( !setUpdateNGNode( dataStatus, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
		else if( IsAGOPSTable( tableName.first ) )
		{
			if( !setUpdateNGAGOPS( dataStatus, tableName.first, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
		else if( IsInfTable( tableName.first ) )
		{
			if( !setUpdateNGInf( dataStatus, tableName.first, setNGLink, cPGDBData ) )
			{
				return false;
			}
		}
	}
	// 車種タイプのNGデータ設定は最後に(一通/通禁車種別規制が終わってから)
	if(!setUpdateNGVehicleType(dataStatus, cPGDBData))
	{
		return false;
	}

	return true;
}

bool CImport::setConflictTableData2( const CString& strTableName, UPDATETYPE updateType, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData, set<long>& setSearchedLink, set<long>& setNGLink )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setConflictTableData2") );
#endif
	const CNWData& cPGDBData = (updateType == UPDATETYPE::DEL?cBeforeData:cAfterData);

	set<CString> exceptFields = getExceptFieldSet(strTableName);

	map<CString,map<long,UpdateData>>::iterator itData2Update = m_mapData2Update.find( strTableName );
	map<CString,map<UPDATETYPE,set<long>>>::iterator itUpdate2Data = m_mapUpdate2Data.find( strTableName );
	if( m_mapData2Update.end() == itData2Update || m_mapUpdate2Data.end() == itUpdate2Data )
		return true;

	CFieldMap cPGDBDataField = cPGDBData.GetFieldMap( strTableName );
	CFieldMap cSDEDataField = cSDEData.GetFieldMap( strTableName );
	ITablePtr ipSDEDataTable = cSDEData.GetTable( strTableName );
	map<UPDATETYPE,set<long>>::const_iterator itUpdateTypeData = itUpdate2Data->second.find( updateType );

	if( itUpdate2Data->second.end() != itUpdateTypeData )
	{
		for( const auto& updata : itUpdateTypeData->second )
		{
			long lOrgID = cPGDBData.GetOrgOIDFromPGDBOID( strTableName, updata );
			if( lOrgID < 0 )
			{
				// エラー
				_ASSERT(0);
				return false;
			}
			bool bConflict = false;
			_IRowPtr ipSDERow;
			if( S_OK == ipSDEDataTable->GetRow( lOrgID, &ipSDERow ) && ipSDERow )
			{
				// 更新対象データがSDE側に存在する場合はコンフリクトチェックを実行
				_IRowPtr ipAfterRow  = cPGDBData.GetRow( strTableName, updata ); // PGDBにはあるはずなんだけどねー
				_IRowPtr ipBeforeRow = cBeforeData.GetRow( strTableName, updata );
				if( !ipAfterRow || !ipBeforeRow )
				{
					// エラー
					_ASSERT(0);
					return false;
				}
				// コンフリクトしているかチェック
				
				if( IsConflict( cSDEDataField, cPGDBDataField, ipSDERow, ipBeforeRow, ipAfterRow, exceptFields ) )
				{
					bConflict = true;
				}
			}
			else
			{
				// SDE側に対象データが無い場合は削除済みということでコンフリクトしていることになる
				bConflict = true;
			}

			// コンフリクトしている場合は関連する更新予定データ全てを更新不可にする
			//   -SDEに存在するがSDE側で更新されている場合（コンフリクト）と
			//   -SDEに存在しない（既に削除済み＝コンフリクト）の場合は更新不可とする
			if( bConflict )
			{
				// コンフリクトしている場合は自分自身にコンフリクトフラグを立てる（更新不可とする）
				itData2Update->second[updata].dataStatus = DATASTATUS(itData2Update->second[updata].dataStatus | DATASTATUS::CONFLICT);

				if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
				{
					// リンク向けの処理
					// このリンクに紐付く更新対象リンクも更新させない
					map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( updata );
					if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
					{
						setSearchedLink.insert( updata );
						if( !searchNoUpdateLink( DATASTATUS::REL_LINK, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
							return false;
						if( !searchNoUpdateLink( DATASTATUS::REL_LINK, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
							return false;
					}
					else
					{
						_ASSERT(0);
						return false;
					}
				}
				else if( 0 == _tcsicmp( strTableName, road_node::kTableName ) )
				{
					// ノード向けの処理
					// このノードに紐付く更新対象リンクも更新させない
					if( !searchNoUpdateLink( DATASTATUS::REL_NODE, updata, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
						return false;
				}
				else if( IsAGOPSTable( strTableName ) )
				{
					// [リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]向けの処理
					// この[リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]に紐付く更新対象リンクも更新させない
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,long>::const_iterator itAGOPS2Link = itPGDBData->second.mapAGOPS2Link.find( updata );
					if( itPGDBData->second.mapAGOPS2Link.end() == itAGOPS2Link )
					{
						_ASSERT(0);
						return false;
					}
					// 更新予定リンクなら紐付くリンクを更に探索
					if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_AGOPS, itAGOPS2Link->second ) )
					{
						map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( itAGOPS2Link->second );
						if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
						{
							// 紐付いたリンクは探索済みに
							setSearchedLink.insert( itAGOPS2Link->second );
							if( !searchNoUpdateLink( DATASTATUS::REL_AGOPS, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
							if( !searchNoUpdateLink( DATASTATUS::REL_AGOPS, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
						}
						else
						{
							_ASSERT(0);
							return false;
						}
					}
				}
				else if( IsVehicleTypeTable( strTableName ) )
				{
					// [リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]向けの処理
					// この[リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]に紐付く更新対象リンクも更新させない
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,long>::const_iterator itType2VehicleReg = itPGDBData->second.mapType2VehicleReg.find( updata );
					if( itPGDBData->second.mapType2VehicleReg.end() == itType2VehicleReg )
					{
						_ASSERT(0);
						return false;
					}
					// 紐付く一通/通禁車種別規制の情報を更新する
					IsUpdateRow( vehicle_reg::kTableName, DATASTATUS::REL_VEHICLETYPE, itType2VehicleReg->second );

					map<CString,TableData>::const_iterator itPGDBData2 = cPGDBData.m_mapTableData.find( vehicle_reg::kTableName );
					map<long,long>::const_iterator itVehicleReg2Link = itPGDBData2->second.mapAGOPS2Link.find( itType2VehicleReg->second );
					if( itPGDBData2->second.mapAGOPS2Link.end() == itVehicleReg2Link )
					{
						_ASSERT(0);
						return false;
					}
					// 更新予定リンクなら紐付くリンクを更に探索
					if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_VEHICLETYPE, itVehicleReg2Link->second ) )
					{
						map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( itVehicleReg2Link->second );
						if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
						{
							// 紐付いたリンクは探索済みに
							setSearchedLink.insert( itVehicleReg2Link->second );
							if( !searchNoUpdateLink( DATASTATUS::REL_VEHICLETYPE, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
							if( !searchNoUpdateLink( DATASTATUS::REL_VEHICLETYPE, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
								return false;
						}
						else
						{
							_ASSERT(0);
							return false;
						}
					}
				}
				else if( IsInfTable( strTableName ) )
				{
					// リンク列向けの処理
					// リンク列に紐付く更新対象リンクも更新させない
					map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
					if( cPGDBData.m_mapTableData.end() == itPGDBData )
					{
						_ASSERT(0);
						return false;
					}
					map<long,set<long>>::const_iterator itInf2Link = itPGDBData->second.mapInf2Link.find( updata );
					if( itPGDBData->second.mapInf2Link.end() == itInf2Link )
					{
						_ASSERT(0);
						return false;
					}
					for( const auto& inflink : itInf2Link->second )
					{
						// 更新予定リンクなら紐付くリンクを更に探索
						if( IsUpdateRow( road_link::kTableName, DATASTATUS::REL_INF, inflink ) )
						{
							map<long,pair<long,long>>::const_iterator itPGDBLink2FromToNode = cPGDBData.m_mapLink2FromToNode.find( inflink );
							if( cPGDBData.m_mapLink2FromToNode.end() != itPGDBLink2FromToNode )
							{
								// 紐付いたリンクは探索済みに
								setSearchedLink.insert( inflink );
								if( !searchNoUpdateLink( DATASTATUS::REL_INF, itPGDBLink2FromToNode->second.first, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
									return false;
								if( !searchNoUpdateLink( DATASTATUS::REL_INF, itPGDBLink2FromToNode->second.second, cPGDBData.m_mapLink2FromToNode, cPGDBData.m_mapNode2Link, setSearchedLink, setNGLink ) )
									return false;
							}
							else
							{
								// メッシュをまたいだリンク列が存在するためスルー
								//_ASSERT(0);
								//return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool CImport::IsUpdateRow( LPCTSTR tableName, DATASTATUS dataStatus, long lPGDBLinkID )
{
	map<CString,map<long,UpdateData>>::iterator itLinkData2Update = m_mapData2Update.find( tableName );
	if( m_mapData2Update.end() == itLinkData2Update )
		return false;

	map<long,UpdateData>::iterator itLinkUpdate = itLinkData2Update->second.find( lPGDBLinkID );
	if( itLinkData2Update->second.end() == itLinkUpdate )
		return false;

	if( itLinkUpdate->second.updateType == SAME )
		return false;

	// 更新予定リンクをステータス変更（更新不可）にする
	itLinkUpdate->second.dataStatus = DATASTATUS( itLinkUpdate->second.dataStatus | dataStatus );

	return true;
}

bool CImport::setUpdateNGNode( DATASTATUS dataStatus, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	for( const auto& nglink : setNGLink )
	{
		map<long,pair<long,long>>::const_iterator itNGLinkNode = cPGDBData.m_mapLink2FromToNode.find( nglink );
		if( cPGDBData.m_mapLink2FromToNode.end() == itNGLinkNode )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		// リンクに紐づFromToノードを取得
		map<long,UpdateData>::iterator itUpdateNode1 = m_mapData2Update[road_node::kTableName].find( itNGLinkNode->second.first );
		map<long,UpdateData>::iterator itUpdateNode2 = m_mapData2Update[road_node::kTableName].find( itNGLinkNode->second.second );
		if( m_mapData2Update[road_node::kTableName].end() == itUpdateNode1 || m_mapData2Update[road_node::kTableName].end() == itUpdateNode2 )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		if( itUpdateNode1->second.updateType != SAME )
			itUpdateNode1->second.dataStatus = DATASTATUS( itUpdateNode1->second.dataStatus | dataStatus );
		if( itUpdateNode2->second.updateType != SAME )
			itUpdateNode2->second.dataStatus = DATASTATUS( itUpdateNode2->second.dataStatus | dataStatus );
	}
	return true;
}

bool CImport::setUpdateNGAGOPS( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateAGOPS = m_mapData2Update.find(strTableName);
	if( m_mapData2Update.end() == itTableUpdateAGOPS )
	{
		// [リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]が無ければスルー
		return true;
	}
	for( const auto& nglink : setNGLink )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find( strTableName );
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itAGOPS = itPGDBData->second.mapLink2AGOPS.find( nglink );
		if( itPGDBData->second.mapLink2AGOPS.end() == itAGOPS )
		{
			// NGリンクに紐付く[リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]がなければスルー
			continue;
		}

		// NG道路リンクに紐付く更新予定の[リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]は更新不可とする
		for( const auto& ngagops : itAGOPS->second )
		{
			map<long,UpdateData>::iterator itUpdateAGOPS = itTableUpdateAGOPS->second.find( ngagops );
			if( itTableUpdateAGOPS->second.end() == itUpdateAGOPS )
			{
				// ここにはこないはず。。
				_ASSERT(0);
				return false;
			}
			if( itUpdateAGOPS->second.updateType != SAME )
				itUpdateAGOPS->second.dataStatus = DATASTATUS( itUpdateAGOPS->second.dataStatus | dataStatus );
		}
	}
	return true;
}

bool CImport::setUpdateNGVehicleType( DATASTATUS dataStatus, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateVehicleType = m_mapData2Update.find(vehicle_type::kTableName);
	if( m_mapData2Update.end() == itTableUpdateVehicleType )
	{
		// 車種タイプが無ければスルー
		return true;
	}
	map<CString,map<long,UpdateData>>::iterator itTableUpdateVehicleReg = m_mapData2Update.find(vehicle_reg::kTableName);
	if( m_mapData2Update.end() == itTableUpdateVehicleReg )
	{
		// ここにはこないはず。。
		_ASSERT(0);
		return false;
	}
	set<long> ngVehicleRegSet;
	for( const auto& element : itTableUpdateVehicleReg->second )
	{
		// 車種別規制に「更新あり」「更新なし」以外のビットが立っていたらNG
		if( ~(NONE|UPDATE) & element.second.dataStatus )
		{
			ngVehicleRegSet.insert(element.first);
		}
	}
	for( const auto& ngVehicleReg : ngVehicleRegSet )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find(vehicle_type::kTableName);
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itVehicleType = itPGDBData->second.mapVehicleReg2Type.find( ngVehicleReg );
		if( itPGDBData->second.mapVehicleReg2Type.end() == itVehicleType )
		{
			// NG車種別規制に紐付く車種タイプがなければスルー
			continue;
		}

		// NG道路リンクに紐付く更新予定の[リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制]は更新不可とする
		for( const auto& ngVehicleType : itVehicleType->second )
		{
			map<long,UpdateData>::iterator itUpdateVehicleType = itTableUpdateVehicleType->second.find( ngVehicleType );
			if( itTableUpdateVehicleType->second.end() == itUpdateVehicleType )
			{
				// ここにはこないはず。。
				_ASSERT(0);
				return false;
			}
			if( itUpdateVehicleType->second.updateType != SAME )
				itUpdateVehicleType->second.dataStatus = DATASTATUS( itUpdateVehicleType->second.dataStatus | dataStatus );
		}
	}
	return true;
}

bool CImport::setUpdateNGInf( DATASTATUS dataStatus, const CString& strTableName, const set<long>& setNGLink, const CNWData& cPGDBData )
{
	map<CString,map<long,UpdateData>>::iterator itTableUpdateInf = m_mapData2Update.find(strTableName);
	if( m_mapData2Update.end() == itTableUpdateInf )
	{
		// リンク列が無ければスルー
		return true;
	}
	for( const auto& nglink : setNGLink )
	{
		map<CString,TableData>::const_iterator itPGDBData = cPGDBData.m_mapTableData.find(strTableName);
		if( cPGDBData.m_mapTableData.end() == itPGDBData )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		map<long,set<long>>::const_iterator itInf = itPGDBData->second.mapLink2Inf.find( nglink );
		if( itPGDBData->second.mapLink2Inf.end() == itInf )
		{
			// NGリンクに紐付かないリンク列の場合はスルー
			continue;
		}

		// NG道路リンクに紐付く更新予定のリンク列は更新不可とする
		for( const auto& nginf : itInf->second )
		{
			map<long,UpdateData>::iterator itUpdateInf = itTableUpdateInf->second.find( nginf );
			if( itTableUpdateInf->second.end() == itUpdateInf )
			{
				// ここにはこないはず。。
				_ASSERT(0);
				return false;
			}
			if( itUpdateInf->second.updateType != UPDATETYPE::SAME )
				itUpdateInf->second.dataStatus = DATASTATUS( itUpdateInf->second.dataStatus | dataStatus );
		}
	}
	return true;
}


bool CImport::searchNoUpdateLink( DATASTATUS dataStatus, long lSearchNodeID, const map<long,pair<long,long>>& mapPGDBLink2FromToNode, const map<long,set<long>>& mapPGDBNode2Link, set<long>& setSearchedLink, set<long>& setNGLink )
{
	map<long,set<long>>::const_iterator itNode2Link = mapPGDBNode2Link.find( lSearchNodeID );
	if( mapPGDBNode2Link.end() == itNode2Link )
	{
		// エラー
		_ASSERT(0);
		return false;
	}

	for( const auto& link : itNode2Link->second )
	{
		// 探索済みリンクはスルー
		if( setSearchedLink.end() != setSearchedLink.find( link ) )
			continue;

		map<long,UpdateData>::iterator itUpdateLink = m_mapData2Update[road_link::kTableName].find( link );
		if( m_mapData2Update[road_link::kTableName].end() == itUpdateLink )
		{
			// ここにはこないはず。。
			_ASSERT(0);
			return false;
		}
		// 探索済みリンクに追加
		setSearchedLink.insert( link );

		// 更新されていないリンクはスルー
		if( UPDATETYPE::SAME == itUpdateLink->second.updateType )
			continue;

		// 更新されているリンクは更新不可とする
		itUpdateLink->second.dataStatus = DATASTATUS( itUpdateLink->second.dataStatus | dataStatus );
		setNGLink.insert( link );
		
		// 次を探索
		map<long,pair<long,long>>::const_iterator itLink2FromToNode = mapPGDBLink2FromToNode.find( link );
		if( mapPGDBLink2FromToNode.end() == itLink2FromToNode )
		{
			// エラー
			_ASSERT(0);
			return false;
		} 
		long lNextSearchNodeID = ( itLink2FromToNode->second.first == lSearchNodeID ? itLink2FromToNode->second.second : itLink2FromToNode->second.first );
		if( !searchNoUpdateLink( dataStatus, lNextSearchNodeID, mapPGDBLink2FromToNode, mapPGDBNode2Link, setSearchedLink, setNGLink ) )
			return false;
	}

	return true;
}

bool CImport::reflectData( const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::reflect_Data") );
#endif

	// 編集保存するか
	bool bSave = true;

	// 編集開始
	if( !StartEdit( cSDEData.GetWorkspace() ) )
	{
		return false;
	}

	// 更新情報割り当て
	// ノード→リンク→リンク内属性orリンク列の順で（実行順番は重要！）
	for( const auto& reflectTable : m_vecReflectTable )
	{
		if( !reflectTableData( reflectTable, cBeforeData, cAfterData, cSDEData ) )
		{
			writeErrorLog( ERR, __func__, _T("反映失敗"), _T(""), 0 );
			bSave = false;
			break;
		}
	}

	// 編集終了
	if( m_cInputInfo.bTestMode ) // テストモードのときは編集破棄
		bSave = false;
	if( !StopEdit( bSave, cSDEData.GetWorkspace() ) )
	{
		return false;
	}

	return true;
}

bool CImport::reflectNewData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cAfterData, const TableData& sdeTableData, const TableData& afterTableData )
{
	// 新規作成
	map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lPGDBOID );
	if( afterTableData.mapRow.end() == itAfterRow )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}

	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->CreateRow( &ipSDERow ) )
	{
		// 失敗しないと思うけど。。
		_ASSERT(0);
		return false;
	}

	// 新規作成したデータに編集後データの情報をぶち込む
	if( !setRowData( ipSDERow, itAfterRow->second.m_T, sdeTableData.cFieldMap, afterTableData.cFieldMap ) )
	{
		_ASSERT(0);
		return false;
	}
	// 編集後データと新規作成データのIDマップを確保
	long lOID = 0;
	if( S_OK != ipSDERow->get_OID( &lOID ) )
	{
		_ASSERT(0);
		return false;
	}
	m_mapPGDBOldID2SDENewID[strTableName][lPGDBOID] = lOID;

	// 道路リンクの場合
	if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
	{
		// FromToノードIDの付け替え
		map<long,pair<long,long>>::const_iterator itAfterLink2FromToNode = cAfterData.m_mapLink2FromToNode.find( lPGDBOID );
		if( cAfterData.m_mapLink2FromToNode.end() == itAfterLink2FromToNode )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;	
		}
		// FromToノードIDの付け替え処理
		if( !replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.first, sdeTableData.cFieldMap.FindField( road_link::kFromNodeID ) ) || 
			!replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.second, sdeTableData.cFieldMap.FindField( road_link::kToNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制の場合
	else if( IsAGOPSTable( strTableName ) )
	{
		// 対応リンクの付け替え
		map<long,long>::const_iterator itAGOPS2Link = afterTableData.mapAGOPS2Link.find( lPGDBOID );
		if( afterTableData.mapAGOPS2Link.end() == itAGOPS2Link )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, itAGOPS2Link->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( category::link_relational_table::kLinkID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	else if( IsVehicleTypeTable( strTableName ) )
	{
		// 対応車種別規制の付け替え
		map<long,long>::const_iterator itType2VehicleReg = afterTableData.mapType2VehicleReg.find( lPGDBOID );
		if( afterTableData.mapType2VehicleReg.end() == itType2VehicleReg )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( vehicle_reg::kTableName, cAfterData, itType2VehicleReg->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( vehicle_type::kRegID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// リンク列の場合
	else if( IsInfTable( strTableName ) )
	{
		CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
		map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
		map<long,set<LqData>>::const_iterator itAfterLqData = afterTableData.mapInfData.find( lPGDBOID );
		ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
		if( !createLqData( lOID, ipSDELqTable, itSDELqTableData->second.cFieldMap, cAfterData, itAfterLqData->second ) )
		{
			_ASSERT(0);
			return false;
		}
	}

	// 最後にストアする
	if( S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	// ログ出力
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOID, true, _T("新規作成") ) );

	return  true;
}
bool CImport::replaceFromToNodeID( const CString& strTableName, const CNWData& cAfterData, _IRowPtr& ipSDERow, long lPGDBNodeID, long lNodeIndex )
{
	bool bNew = false;
	if( m_mapPGDBOldID2SDENewID.end() != m_mapPGDBOldID2SDENewID.find( road_node::kTableName ) )
	{
		if( m_mapPGDBOldID2SDENewID[road_node::kTableName].end() != m_mapPGDBOldID2SDENewID[road_node::kTableName].find( lPGDBNodeID ) )
		{
			if( S_OK != ipSDERow->put_Value( lNodeIndex, CComVariant(m_mapPGDBOldID2SDENewID[road_node::kTableName][lPGDBNodeID]) ) )
			{
				_ASSERT(0);
				return false;
			}
			bNew = true;
		}
	}
	if( !bNew )
	{
		long lOrgNodeID = cAfterData.GetOrgOIDFromPGDBOID( road_node::kTableName, lPGDBNodeID );
		if( lOrgNodeID < 0 )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( lNodeIndex, CComVariant( lOrgNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::reflectDeleteData( const CString& strTableName, long lPGDBOID, const CNWData& cSDEData, const CNWData& cBeforeData, const TableData& sdeTableData, const TableData& beforeTableData )
{
	// 削除
	map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lPGDBOID );
	if( beforeTableData.mapRow.end() == itBeforeRow )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	long lOrgOID = cBeforeData.GetOrgOIDFromPGDBOID( strTableName, itBeforeRow->first );
	if( lOrgOID < 0 )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	// SDEから削除フィーチャを取得
	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->GetRow( lOrgOID, &ipSDERow ) || !ipSDERow )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	// 削除して更新
	if( S_OK != ipSDERow->Delete() ||
		S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	// リンク列の場合
	if( IsInfTable( strTableName ) )
	{
		// INFに紐付くLQを削除する
		CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
		map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
		ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
		if( !deleteLqData( lOrgOID, ipSDELqTable ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// ログ出力
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOrgOID, true, _T("削除") ) );
	
	return true;
}

bool CImport::reflectUpdateData(
	const CString& strTableName,
	long lPGDBOID,
	UPDATETYPE updateType,
	const CNWData& cSDEData,
	const CNWData& cAfterData,
	const TableData& sdeTableData,
	const TableData& beforeTableData,
	const TableData& afterTableData )
{
	// 属性、形状、形状+属性更新
	map<long,CAdapt<_IRowPtr>>::const_iterator itAfterRow = afterTableData.mapRow.find( lPGDBOID );
	if( afterTableData.mapRow.end() == itAfterRow )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	// インポート除外フィールドの変更有無を確認する
	CString exceptFields;
	if(!afterTableData.exceptFieldIndexSet.empty())
	{
		map<long,CAdapt<_IRowPtr>>::const_iterator itBeforeRow = beforeTableData.mapRow.find( lPGDBOID );
		if( beforeTableData.mapRow.end() == itBeforeRow )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		exceptFields = getExceptFieldString(beforeTableData, afterTableData, itBeforeRow->second, itAfterRow->second, afterTableData.exceptFieldIndexSet);
	}

	long lOrgOID = cAfterData.GetOrgOIDFromPGDBOID( strTableName, itAfterRow->first );
	if( lOrgOID < 0 )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	// SDEから更新フィーチャを取得
	_IRowPtr ipSDERow;
	if( S_OK != sdeTableData.ipTable->GetRow( lOrgOID, &ipSDERow ) || !ipSDERow )
	{
		// ないはずはない。。
		_ASSERT(0);
		return false;
	}
	long lOID = 0;
	if( S_OK != ipSDERow->get_OID( &lOID ) )
	{
		_ASSERT(0);
		return false;
	}
	// データ更新
	set<CString> exceptFieldSet = getExceptFieldSet(strTableName);
	if( !setRowData( ipSDERow, itAfterRow->second.m_T, sdeTableData.cFieldMap, afterTableData.cFieldMap, exceptFieldSet ) )
	{
		_ASSERT(0);
		return false;
	}
	// 道路リンクの場合
	if( 0 == _tcsicmp( strTableName, road_link::kTableName ) )
	{
		// FromToノードIDの付け替え
		map<long,pair<long,long>>::const_iterator itAfterLink2FromToNode = cAfterData.m_mapLink2FromToNode.find( lPGDBOID );
		if( cAfterData.m_mapLink2FromToNode.end() == itAfterLink2FromToNode )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		// FromToノードIDの付け替え
		if( !replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.first, sdeTableData.cFieldMap.FindField( road_link::kFromNodeID ) ) || 
			!replaceFromToNodeID( strTableName, cAfterData, ipSDERow, itAfterLink2FromToNode->second.second, sdeTableData.cFieldMap.FindField( road_link::kToNodeID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// リンク内属性、傾斜、一方通行、通行禁止、簡易傾斜、一通/通禁車種別規制の場合
	else if( IsAGOPSTable( strTableName ) )
	{
		// 対応リンクの付け替え
		map<long,long>::const_iterator itAGOPS2Link = afterTableData.mapAGOPS2Link.find( lPGDBOID );
		if( afterTableData.mapAGOPS2Link.end() == itAGOPS2Link )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		long lOrgLinkID = -1;
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, itAGOPS2Link->second, lOrgLinkID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( category::link_relational_table::kLinkID ), CComVariant( lOrgLinkID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	else if( IsVehicleTypeTable( strTableName ) )
	{
		// 対応車種別規制の付け替え
		map<long,long>::const_iterator itType2VehicleReg = afterTableData.mapType2VehicleReg.find( lPGDBOID );
		if( afterTableData.mapType2VehicleReg.end() == itType2VehicleReg )
		{
			// ないはずはない。。
			_ASSERT(0);
			return false;
		}
		long lOrgVehicleRegID = -1;
		if( !getOrgIDFromPgdbID( vehicle_reg::kTableName, cAfterData, itType2VehicleReg->second, lOrgVehicleRegID ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipSDERow->put_Value( sdeTableData.cFieldMap.FindField( vehicle_type::kRegID ), CComVariant( lOrgVehicleRegID ) ) )
		{
			_ASSERT(0);
			return false;
		}
	}
	// リンク列の場合
	else if( IsInfTable( strTableName ) )
	{
		// 形状、形状＋属性更新の場合、LQの付け替えを行う
		if( updateType == UPDATETYPE::SHAPE ||
			updateType == UPDATETYPE::SHAPE_ATTR )
		{
			CString strLqTableName = _T("LQ_") + strTableName.Right( strTableName.GetLength() - 4 );
			map<CString,TableData>::const_iterator itSDELqTableData = cSDEData.m_mapTableData.find( strLqTableName );
			map<long,set<LqData>>::const_iterator itAfterLqData = afterTableData.mapInfData.find( lPGDBOID );
			ITablePtr ipSDELqTable = itSDELqTableData->second.ipTable;
			// 一旦LQを削除してから新規作成する
			if( !deleteLqData( lOrgOID, ipSDELqTable ) ||
				!createLqData( lOID, ipSDELqTable, itSDELqTableData->second.cFieldMap, cAfterData, itAfterLqData->second ) )
			{
				_ASSERT(0);
				return false;
			}
		}
	}

	// 最後にストアする
	if( S_OK != ipSDERow->Store() )
	{
		_ASSERT(0);
		return false;
	}
	CString strStatus;
	if( updateType == UPDATETYPE::SHAPE )
		strStatus = _T("形状更新");
	else if( updateType == UPDATETYPE::SHAPE_ATTR )
		strStatus = _T("形状＋属性更新");
	else
		strStatus = _T("属性更新");

	if(!exceptFields.IsEmpty())
		strStatus.Append(exceptFields);

	// ログ出力
	m_vecImportLog.push_back( getImportLog( SDE, strTableName, lOrgOID, true, strStatus ) );

	return true;
}

CString CImport::getExceptFieldString(
	const TableData& beforeTableData,
	const TableData& afterTableData,
	const _IRowPtr& ipBeforeRow,
	const _IRowPtr& ipAfterRow,
	const std::set<long>& exceptFieldIndexSet
	)
{
	CString result;
	// 除外フィールド
	for(long afterIndex : afterTableData.exceptFieldIndexSet)
	{
		CString fieldName = afterTableData.cFieldMap.GetName(afterIndex);
		long beforeIndex = beforeTableData.cFieldMap.FindField(fieldName);
		CComVariant vaAfterVal  = ipAfterRow->GetValue(afterIndex);
		CComVariant vaBeforeVal = ipBeforeRow->GetValue(beforeIndex);
		if(vaAfterVal != vaBeforeVal)
		{
			result.AppendFormat(_T(",%s"), fieldName);
		}
	}
	if(!result.IsEmpty())
	{
		result.TrimLeft(_T(","));
		result = _T("(除外:")+result+_T(")");
	}
	return result;
}

std::set<CString> CImport::getExceptFieldSet(LPCTSTR fieldName)
{
	const auto& exceptElement = m_exceptFieldMap.find(CString(fieldName).MakeUpper());
	if(exceptElement != m_exceptFieldMap.end())
		return exceptElement->second;

	return set<CString>();
}

bool CImport::reflectTableData( const CString& strTableName, const CNWData& cBeforeData, const CNWData& cAfterData, const CNWData& cSDEData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::reflectTableData") );
#endif

	map<CString,map<long,UpdateData>>::const_iterator itData2Update = m_mapData2Update.find( strTableName );
	map<CString,TableData>::const_iterator itBeforeTableData = cBeforeData.m_mapTableData.find( strTableName );
	map<CString,TableData>::const_iterator itAfterTableData = cAfterData.m_mapTableData.find( strTableName );
	map<CString,TableData>::const_iterator itSDETableData = cSDEData.m_mapTableData.find( strTableName );
	if( m_mapData2Update.end() != itData2Update )
	{
		for( const auto& updatedata : itData2Update->second )
		{
			if( updatedata.second.dataStatus == DATASTATUS::UPDATE ) 
			{
				// 更新有データ
				if( updatedata.second.updateType == UPDATETYPE::NEW )
				{
					// 新規作成
					if( !reflectNewData( strTableName, updatedata.first, cSDEData, cAfterData, itSDETableData->second, itAfterTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("新規データ反映失敗"), strTableName, updatedata.first );
						return false;
					}
				}
				else if( updatedata.second.updateType == UPDATETYPE::DEL )
				{
					// 削除
					if( !reflectDeleteData( strTableName, updatedata.first, cSDEData, cBeforeData, itSDETableData->second, itBeforeTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("削除データ反映失敗"), strTableName, updatedata.first );
						return false;
					}
				}
				else if( updatedata.second.updateType == UPDATETYPE::SHAPE ||
						 updatedata.second.updateType == UPDATETYPE::ATTR ||
						 updatedata.second.updateType == UPDATETYPE::SHAPE_ATTR )
				{
					// 属性、形状、形状+属性更新
					if( !reflectUpdateData( strTableName, updatedata.first, updatedata.second.updateType, cSDEData, cAfterData, itSDETableData->second, itBeforeTableData->second, itAfterTableData->second ) )
					{
						writeErrorLog( ERR, __func__, _T("属性,形状データ反映失敗"), strTableName, updatedata.first );
						return false;
					}
				} 
			}
			else if( updatedata.second.dataStatus == DATASTATUS::NONE || updatedata.second.dataStatus == DATASTATUS::OUTMESH )
			{
				// 更新無データ、指定メッシュ外だけのものはスルー
			}
			else
			{
				CString strNGComment;
				// 更新有データだったけど更新させないもの
				if( updatedata.second.dataStatus & DATASTATUS::CONFLICT )
					strNGComment += _T("コンフリクト,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_LINK )
					strNGComment += _T("リンク,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_NODE )
					strNGComment += _T("ノード,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_AGOPS )
					strNGComment += _T("リンク内属性,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_INF )
					strNGComment += _T("リンク列,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_EDGENODE )
					strNGComment += _T("2次メッシュ境界,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_ZERONODE )
					strNGComment += _T("リンクFromToIDが0,");
				if( updatedata.second.dataStatus & DATASTATUS::REL_VEHICLETYPE )
					strNGComment += _T("車種タイプ,");
				if( updatedata.second.dataStatus & DATASTATUS::OUTMESH )
					strNGComment += _T("指定メッシュ外,");
				strNGComment.TrimRight( _T(",") );

				DBTYPE dbType;
				if( updatedata.second.updateType == UPDATETYPE::DEL )
					dbType = BEFORE;
				else
					dbType = AFTER;

				m_vecImportLog.push_back( getImportLog( dbType, strTableName, updatedata.first, false, strNGComment ) );
			}
		}
	}
	return true;
}

bool CImport::createWorkVersion( IWorkspacePtr& ipWorkspace, IWorkspacePtr& ipSDEWork )
{
	IVersionPtr ipCurrentVersion( ipWorkspace );
	if( !ipCurrentVersion )
	{
		_ASSERT(0);
		return false;
	}
	// 同一バージョンが見つかった場合はそれを利用する
	IVersionPtr ipChildVersion;
	((IVersionedWorkspacePtr)ipCurrentVersion)->FindVersion( CComBSTR(m_strWorkVersionName), &ipChildVersion );
	if( !ipChildVersion )
	{
		HRESULT hr = ipCurrentVersion->CreateVersion( CComBSTR(m_strWorkVersionName), &ipChildVersion );
		if( ipChildVersion == NULL )
		{
			_ASSERT(0);
			return false;
		}
	}
	CString strWorkVersionName = sindy::workspace_address((IWorkspacePtr)ipChildVersion).c_str();

	ipSDEWork = create_workspace( strWorkVersionName );
	if( !ipSDEWork )
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

bool CImport::setRowData( _IRowPtr& ipSDERow, const _IRowPtr& ipPGDBRow, const CFieldMap& cSDEField, const CFieldMap& cPGDBField, const std::set<CString>& exceptFieldSet /*= std::set<CString>()*/)
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::setRowData") );
#endif
	for( long i = 0; i < cSDEField.GetFieldCount(); ++i )
	{
		if( !cSDEField.IsEditable( i ) )
			continue;

		CComVariant vaPGDBValue;
		CString strFieldName = cSDEField.GetName( i );
		// MODIFYDATEのときは更新しない(bug10009)
		if(strFieldName.CompareNoCase(ipc_table::kModifyDate) == 0)
			continue;
		
		// インポート除外フィールドは無視(bug10363)
		else if( exceptFieldSet.find(strFieldName) != exceptFieldSet.end() )
		{
			continue;
		}

		// OPERATORのときは引数で指定されている値を入れる
		else if(strFieldName.CompareNoCase(ipc_table::kOperator) == 0)
		{
			if(m_cInputInfo.strOperator.IsEmpty())
				continue;
			vaPGDBValue = m_cInputInfo.strOperator;
		}

		// PURPOSE_Cのときは引数で指定されている値を入れる
		else if(strFieldName.CompareNoCase(ipc_table::kPurpose) == 0)
		{
			if(m_cInputInfo.lPurpose == -1)
				continue;
			vaPGDBValue = m_cInputInfo.lPurpose;
		}
		// TODO:SDEに新しいフィールドが追加されたりした際に、対応の取れないフィールドが出てくるため、警告文が大量に出力される
		// 最初に、SDEとPGDBのフィールドの整合性を確認し、差異のあるフィールドを格納し、そのフィールドは除くように修正するべき
		else if( S_OK != ipPGDBRow->get_Value( cPGDBField.FindField( cSDEField.GetName( i ) ), &vaPGDBValue ) )
		{
			_ASSERT(0);
			return false;
		}

		if( S_OK != ipSDERow->put_Value( i, vaPGDBValue ) )
		{
			_ASSERT(0);
			return false;
		}


		if( cSDEField.IsShapeField( i ) )
		{
			IGeometryPtr ipGeom;
			if( S_OK != IFeaturePtr(ipPGDBRow)->get_ShapeCopy( &ipGeom ) || !ipGeom )
			{
				_ASSERT(0);
				return false;
			}
			if( S_OK != IFeaturePtr(ipSDERow)->putref_Shape( ipGeom ) )
			{
				_ASSERT(0);
				return false;
			}
		}
	}
	// ツール名＋ツール更新日を追記
	if( S_OK != ipSDERow->put_Value( cSDEField.FindField( ipc_table::kProgModifyDate ), CComVariant( m_strProgModifyDate ) ) ||
		S_OK != ipSDERow->put_Value( cSDEField.FindField( ipc_table::kModifyProgName ), CComVariant( m_strToolName ) ) )
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

bool CImport::getOrgIDFromPgdbID( LPCTSTR tableName, const CNWData& cNWData, long lPgdbID, long& lOrgID )
{
	lOrgID = -1; // 一応初期化

	// 対象オブジェクトが新規作成の場合は付け替え
	if( m_mapPGDBOldID2SDENewID.end() != m_mapPGDBOldID2SDENewID.find( tableName ) )
	{
		if( m_mapPGDBOldID2SDENewID[tableName].end() != m_mapPGDBOldID2SDENewID[tableName].find( lPgdbID ) )
		{
			lOrgID = m_mapPGDBOldID2SDENewID[tableName][lPgdbID];
			return true;
		}
	}
	// 新規オブジェクトでない場合は既存オブジェクトで紐付
	lOrgID = cNWData.GetOrgOIDFromPGDBOID( tableName, lPgdbID );
	if( lOrgID < 0 )
	{
		_ASSERT(0);
		return false;
	}
	return true;
}

bool CImport::createLqData( long lSDEInfID, ITablePtr& ipSDELqTable, const CFieldMap& cSDELqField, const CNWData& cAfterData, const set<LqData>& setAfterLqData )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::create_LqData") );
#endif

	// 編集後PGDBのLQデータでループ
	for( const auto& afterlq : setAfterLqData )
	{
		long lLinkID = -1;

		// リンクIDの付け替え
		if( !getOrgIDFromPgdbID( road_link::kTableName, cAfterData, afterlq.lLinkID, lLinkID ) )
		{
			_ASSERT(0);
			return false;
		}

		_IRowPtr ipLqRow;
		if( S_OK != ipSDELqTable->CreateRow( &ipLqRow ) || !ipLqRow )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kMeshCode ), CComVariant(afterlq.lMeshCode) ) || 
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLinkID ), CComVariant(lLinkID) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLinkDir ), CComVariant(afterlq.lLinkDir) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kSequence ), CComVariant(afterlq.lSeq) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kLastLink ), CComVariant(afterlq.lLast) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( link_queue::kInfID ), CComVariant(lSDEInfID) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( ipc_table::kProgModifyDate ), CComVariant(m_strProgModifyDate) ) ||
			S_OK != ipLqRow->put_Value( cSDELqField.FindField( ipc_table::kModifyProgName ), CComVariant(m_strToolName) ) )
		{
			_ASSERT(0);
			return false;
		}
		if( S_OK != ipLqRow->Store() )
		{
			_ASSERT(0);
			return false;
		}
	}

	return true;
}

bool CImport::deleteLqData( long lSDEInfID, ITablePtr& ipSDELqTable )
{
#ifdef _DEBUG
	CFuncTraceWrapper ft( g_timetrace, _T("CImport::delete_LqData") );
#endif

	// INFのIDに紐付くLQを取得
	CString strWhere;
	strWhere.Format( _T("%s=%ld"), link_queue::kInfID, lSDEInfID );
	IQueryFilterPtr ipQFil(CLSID_QueryFilter);
	ipQFil->put_WhereClause( CComBSTR(strWhere) );
	_ICursorPtr ipCursor;
	if( S_OK == ipSDELqTable->Search( ipQFil, VARIANT_FALSE, &ipCursor ) && ipCursor )
	{
		_IRowPtr ipRow;
		while( S_OK == ipCursor->NextRow( &ipRow ) && ipRow )
		{
			// ヒットしたものを削除
			if( S_OK != ipRow->Delete() ||
				S_OK != ipRow->Store() )
			{
				_ASSERT(0);
				return false;
			}
		}
	}
	else
	{
		_ASSERT(0);
		return false;
	}

	return true;
}

CString CImport::getImportLog( DBTYPE dbType, const CString& strTableName, long lOID, bool bOK, const CString& strStatus )
{
	CString strConnect;
	switch( dbType )
	{
		case BEFORE:
			strConnect = _T("編集前PGDB");
			break;
		case AFTER:
			strConnect = _T("編集後PGDB");
			break;
		case SDE:
			strConnect = _T("SDE");
			break;
		default:
			break;
	}
	CString strRet, strOKNG;
	strOKNG = bOK ? _T("OK") : _T("NG");
	strRet.Format( _T("0\t%ld\t%s\t%s\t%ld\t%s\t%s\t"), m_lMeshCode, strConnect, strTableName, lOID, strOKNG, strStatus );

	return strRet;
}

void CImport::writeErrorLog( ERRTYPE errType, const CString& strFuncName, const CString& strComment, const CString& strTableName, long lOID, bool bOutTime/*=false*/)
{
	CString strErrType;
	switch( errType )
	{
	case 0:
		strErrType = _T("INFO");
		break;
	case 1:
		strErrType = _T("NOTICE");
		break;
	case 2:
		strErrType = _T("WARNING");
		break;
	case 3:
		strErrType = _T("ERROR");
		break;
	case 4:
		strErrType = _T("CRITICAL");
		break;
	default:
		break;
	}
	
	CString strTableOID;
	if( !strTableName.IsEmpty() )
	{
		if( lOID > 0 )
			strTableOID.Format( _T("%s:%ld"), strTableName, lOID );
		else
			strTableOID = strTableName;
	}

	CString strErrMessage;
	strErrMessage.Format( _T("#%s\tMESHCODE：%ld\t%s\t%s\t%s\t"), strErrType, m_lMeshCode, strFuncName, strComment, strTableOID );
	
	if(bOutTime)
	{
		SYSTEMTIME aTime;
		GetLocalTime(&aTime);
		strErrMessage += uh::str_util::ToString(&aTime);
	}
	if( m_ofsErr.fail() )
		cerr << CT2A(strErrMessage) << endl;
	else
		m_ofsErr << CT2A(strErrMessage) << endl;
}

bool CImport::StartEdit( IWorkspacePtr& ipWorkspace )
{
	IWorkspaceEditPtr ipWorkspaceEdit(ipWorkspace);
	if( !ipWorkspaceEdit )
		return false;
	
	if( S_OK != ipWorkspaceEdit->StartEditing( VARIANT_TRUE ) )
	{
		writeErrorLog( ERR, _T(""), _T("StartEditing()失敗"), _T(""), 0 );
		return false;
	}

	if( S_OK != ipWorkspaceEdit->StartEditOperation() )
	{
		writeErrorLog( ERR, _T(""), _T("StartEditOperation()失敗"), _T(""), 0 );
		return false;
	}

	return true;
}

bool CImport::StopEdit( bool bSave, IWorkspacePtr& ipWorkspace )
{
	IWorkspaceEditPtr ipWorkspaceEdit( ipWorkspace );
	if( !ipWorkspaceEdit )
		return false;
	
	if( bSave )
	{
		if( S_OK != ipWorkspaceEdit->StopEditOperation() )
		{
			writeErrorLog( ERR, _T(""), _T("StopEditOperation()失敗"), _T(""), 0 );
			if( S_OK == ipWorkspaceEdit->AbortEditOperation() )
				writeErrorLog( INFO, _T(""), _T("AbortEditOperation()成功"), _T(""), 0 );
			else
				writeErrorLog( ERR,_T(""), _T("AbortEditOperation()失敗"), _T(""), 0 );
			if( S_OK == ipWorkspaceEdit->StopEditing( VARIANT_FALSE ) )
				writeErrorLog( INFO, _T(""), _T("StopEditing()成功"), _T(""), 0 );
			else
				writeErrorLog( ERR, _T(""), _T("StopEditing()失敗"), _T(""), 0 );
			return false;
		}
	}
	else
	{
		if( S_OK == ipWorkspaceEdit->AbortEditOperation() )
			writeErrorLog( INFO, _T(""), _T("AbortEditOperation()成功"), _T(""), 0 );
		else
			writeErrorLog( ERR, _T(""), _T("AbortEditOperation()失敗"), _T(""), 0 );
	}

	if( bSave )
	{
		bool bReconcile = true;
		int lCount = 0;
		// 編集終了処理
		while( !DoReconcile( ipWorkspaceEdit ) )
		{
			lCount++;
			if( lCount > g_lReconcileCount )
			{
				CString strReconcile;
				strReconcile.Format( _T("リコンサイル%d回失敗"), g_lReconcileCount );
				writeErrorLog( ERR, __func__, strReconcile, _T(""), 0 );
				bReconcile = false;
				break;
			}
		}
		return bReconcile;
	}
	else
	{
		if( S_OK != ipWorkspaceEdit->StopEditing( VARIANT_FALSE ) )
		{
			writeErrorLog( ERR, _T(""), _T("StopEditing()失敗"), _T(""), 0, true );
			return false;
		}
		else
			writeErrorLog( INFO, _T(""), _T("StopEditing()成功"), _T(""), 0, true ); 
	}

	return true;
}

bool CImport::DoReconcile( IWorkspaceEditPtr& ipWorkspaceEdit )
{
	// StopEditing()に失敗したらリコンサイル
	HRESULT hr = ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
	bool bRet = false;
	switch( hr )
	{
		case FDO_E_VERSION_REDEFINED:
		{
			std::cout << "◆リコンサイル中...\n";
			writeErrorLog( INFO, _T(""), _T("リコンサイル実施"), _T(""), 0 );
			IVersionEditPtr ipVersionEdit( ipWorkspaceEdit );
			if( ipVersionEdit )
			{
				IVersionInfoPtr ipVersionInfo, ipParentVersionInfo;
				IVersionPtr ipVersion( ipVersionEdit );
				CComBSTR bstrVersionName, bstrParentVersionName;
				if( ipVersion )
				{
					if( SUCCEEDED( ipVersion->get_VersionInfo( &ipVersionInfo ) ) )
						ipVersionInfo->get_VersionName( &bstrVersionName );
				}
				if( !CString(bstrVersionName).IsEmpty() )
				{
					VARIANT_BOOL vbConflicts = VARIANT_FALSE;
					HRESULT hrReconcile = hr = ipVersionEdit->Reconcile( bstrParentVersionName, &vbConflicts );
					switch( hrReconcile )
					{
						case FDO_E_RECONCILE_VERSION_NOT_AVAILABLE:
							writeErrorLog( ERR, _T(""), _T("リコンサイル失敗：FDO_E_RECONCILE_VERSION_NOT_AVAILABLE"), _T(""), 0 );
							break;
						case FDO_E_VERSION_BEING_EDITED:
							writeErrorLog( ERR, _T(""), _T("リコンサイル失敗：FDO_E_VERSION_BEING_EDITED"), _T(""), 0 );
							break;
						case FDO_E_VERSION_NOT_FOUND:
							writeErrorLog( ERR, _T(""), _T("リコンサイル失敗：FDO_E_VERSION_NOT_FOUND"), _T(""), 0 );
							break;
						case S_OK:
							// コンフリクトした場合はエラーとせず、そのまま抜ける
							// コンフリクト時の処理は呼び出し先に任せる
							if( vbConflicts )
								writeErrorLog( WARNING, _T(""), _T("リコンサイル成功：コンフリクト発生"), _T(""), 0 );
							else {
								// ヘルプには、Reconcileが終了したらStopEditingが自動的に呼ばれるようなことが
								// 書いてあったので、何もしなくてもいいのかな？？
								writeErrorLog( INFO, _T(""), _T("リコンサイル成功：編集終了"), _T(""), 0 );
							}
							bRet = true;
						break;
						default:
							writeErrorLog( ERR, _T(""), _T("リコンサイル中に原因不明のエラーが発生"), _T(""), 0 );
							break;
					}
				}
				else
					writeErrorLog( ERR, _T(""), _T("IVersionEditからバージョン名の取得失敗"), _T(""), 0 );
			}
			else
				writeErrorLog( ERR, _T(""), _T("IVersionEditの取得失敗"), _T(""), 0 );
		}
			break;
		case S_OK:
			writeErrorLog( INFO, _T(""), _T("編集保存成功"), _T(""), 0, true );
			bRet = true;
			break;
		default:
			writeErrorLog( ERR, _T(""), _T("編集保存失敗"), _T(""), 0, true );
			break;
	}
	return bRet;
}

bool CImport::readExceptFieldList()
{
	using namespace uh::str_util;

	// TSVファイルの読み込み
	ifstream ifs(CT2A(m_cInputInfo.strExceptList));
	if(!ifs)
	{
		CString message;
		cerr << "除外フィールドファイルの読み込みに失敗しました" << endl;
		cerr << "ERROR\texcept_fieldlist : " << CT2A(m_cInputInfo.strExceptList) << " が正しいか確認してください" << endl;
		return false;
	}

	bool succeed_f = true;
	string line;
	int lineNum = 0;
	while(getline(ifs, line))
	{
		CString curLine(CA2T(line.c_str()));
		curLine = curLine.MakeUpper();

		std::vector<CString> cols = split(curLine, _T("\t"));
		if(cols.size() != 2)
		{
			cerr << "ERROR\texcept_fieldlist : 列の数が不正です\t" << line << endl;
			succeed_f = false;
			continue;
		}
		CString layerName = cols[0];
		if(m_exceptFieldMap.find(layerName) != m_exceptFieldMap.end())
		{
			cerr << "ERROR\texcept_fieldlist : レイヤの指定が重複しています\t" << CT2A(layerName) << endl;
			succeed_f = false;
			continue;
		}
		std::vector<CString> cols2 = split(cols[1], _T(","));
		for(const auto& col : cols2)
		{
			if(!checkExceptField(col))
			{
				cerr << "ERROR\texcept_fieldlist : インポート除外フィールドには設定できない値です\t"
				     << CT2A(layerName) << "\t" << CT2A(col) << endl;
				succeed_f = false;
				continue;
			}
			m_exceptFieldMap[cols[0]].insert(col);
		}
	}
	return succeed_f;
}

bool CImport::checkExceptField(const CString& fieldName)
{
	using namespace sindy::schema;

	return ((fieldName.CompareNoCase(_T("OBJECTID")) != 0 )
	     && (fieldName.CompareNoCase(_T("SHAPE")) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kPurpose) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kProgModifyDate) != 0 )
	     && (fieldName.CompareNoCase(ipc_table::kModifyProgName) != 0 )
		 && (fieldName.CompareNoCase(ipc_table::kModifyDate) != 0 )
		 && (fieldName.CompareNoCase(ipc_table::kOperator) != 0 )
	     && (fieldName.Right(3).CompareNoCase(_T("_ID")) != 0 )
	     && (fieldName.Right(3).CompareNoCase(_T("_RF")) != 0 )
	     && (fieldName.Right(5).CompareNoCase(_T("_LQRF")) != 0 )
		 );
}
