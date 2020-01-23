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
#include "Import.h"
#include "ImportData.h"
#include <chrono>

using namespace sindy::schema;
using namespace Reflect;
namespace {

	// UPDATE_Cに設定される値の定義
	const long NO_CHG = 0x00;
	const long SHAPE_CHG = 0x01;	///< 形状に変更があった（ビットフラグ）
	const long ATTR_CHG = 0x02;		///< 属性に変更があった（ビットフラグ）
	const long BOTH_CHG = SHAPE_CHG | ATTR_CHG;	///< 形状と属性に変更があった
	// 以下２つはDiffImportPGDBのみで使用する(SiNDY-e,cでは使用しない)
	const long DELETE_CHG = 0x04;	///< 削除された
	const long NEW_CHG = 0x08;		///< 新規追加された

	LPCTSTR CHG_STATUS_NAME =_T("UPDATE_C");	///< 更新ステータスのフィールド名
	LPCTSTR ORG_OBJID_NAME = _T("ORG_OBJID");	///< オリジナルのオブジェクトIDフィールド名
	LPCTSTR ORG_MODIFYDATE_NAME = _T("ORG_MODIFYDATE");	///< オリジナルの最終更新日付フィールド名
	LPCTSTR GEOSPACE_ID_NAME = _T("GEOSPACE_ID"); //!< GEOSPACEIDフィールド名
}

// コンストラクタ
CImport::CImport(void)
: m_OrgIdIndex(-1)
, m_ChgStatusIndex(-1)
, m_OrgModifyDateIndex(-1)
, m_Command(NONE)
, m_tdcMode(false)
, m_geospaceMode(false)
, m_emptyDBMode(false)
{
	m_ExecInfo = _T("差分インポート");

	m_CommandString[DIFF_UPDATE] = _T("差分更新");
	m_CommandString[ONLY_DELETE] = _T("削除のみ");
	m_CommandString[ONLY_ADD]    = _T("追加のみ");
	m_CommandString[DELETE_ADD]  = _T("削除＋追加");
}

// デストラクタ
CImport::~CImport(void)
{
}


// 初期化関数
bool CImport::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
				   const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	// trueしか返してないが、CSDEBaseのinit()をオーバーライドしているため
	// bool値を返さないといけないためこうなっている。
	m_OldPath = args.m_oldPath.c_str();
	m_NewPath = args.m_newPath.c_str();
	m_testMode = args.m_testMode;
	m_tdcMode = args.m_tdcMode;
	m_geospaceMode = args.m_geospaceMode;
	m_layerList = args.m_layerList;
	m_logFile = args.m_logFile;
	m_emptyDBMode = args.m_emptyDBMode;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;

	COleDateTime oleTime;
	oleTime.ParseDateTime( args.m_availDate.c_str() );
	m_availDate = oleTime;

	return true;
}

// メッシュ単位での差分インポート（反映）関数
bool CImport::fnExecMesh(COMMAND cCommand)
{
	m_Command = cCommand;
	_ftprintf(stderr, _T("#%d,%s\n"), g_CurrentMesh, m_CommandString[m_Command]);
	// 編集前PGDB取得
	IWorkspacePtr ipOldWorkspace;
	if( m_Command == DIFF_UPDATE ) // Uコマンド以外なら編集前はいらない
	{
		ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
		if( !ipOldWorkspace )
		{
			return false;
		}
	}
	// 編集後PGDB取得
	IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
	if( !ipNewWorkspace )
	{
		return false;
	}
	// メッシュポリゴン取得
	if( !fnGetMesh(false) )
	{
		return false;
	}
	
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		const auto start_time = chrono::system_clock::now();

		m_AppendID.clear();
		m_TargetID.clear();
		// 編集前PGDBのフィーチャクラス
		IFeatureClassPtr ipOldClass;
		if( m_Command == DIFF_UPDATE )
		{
			ipOldClass = fnOpenLocalFeatureClass(ipOldWorkspace, i);
			if( !ipOldClass )
			{
				return false;
			}
			ipOldClass->FindField( CComBSTR(ORG_OBJID_NAME), &m_OrgIdIndex );
			if( m_OrgIdIndex < 0 )
			{
				_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。,%s\n"), ORG_OBJID_NAME);
				return false;
			}
		}
		// 編集後PGDBのフィーチャクラス
		IFeatureClassPtr ipNewClass = fnOpenLocalFeatureClass(ipNewWorkspace, i);
		if( !ipNewClass )
		{
			return false;
		}
		// UPDATE_Cフィールドインデックス取得
		ipNewClass->FindField( CComBSTR(CHG_STATUS_NAME), &m_ChgStatusIndex );
		if(m_ChgStatusIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。,%s\n"), CHG_STATUS_NAME);
			return false;
		}
		// ORG_MODIFYDATEフィールドインデックス取得
		ipNewClass->FindField(CComBSTR(ORG_MODIFYDATE_NAME), &m_OrgModifyDateIndex);
		if(m_OrgModifyDateIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。,%s\n"), ORG_MODIFYDATE_NAME);
			return false;
		}

		for(int j = 0; gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName != NULL; ++j)
		{
			// 編集前フィーチャのフィールドインデックス取得
			if(m_Command == DIFF_UPDATE)
			{
				ipOldClass->FindField(CComBSTR(gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_OldPgdbIndex);
				if( gEnvTable[g_DataIndex].m_Fields[i][j].m_OldPgdbIndex < 0 )
				{
					_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。,%s\n"), gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName);
					return false;
				}
			}
			// 編集後フィーチャのフィールドインデックス取得
			ipNewClass->FindField(CComBSTR(gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_NewPgdbIndex);
			if( gEnvTable[g_DataIndex].m_Fields[i][j].m_NewPgdbIndex < 0 )
			{
				_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。,%s\n"), gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName);
				return false;
			}
		}

		// GEOSPACEインポート時はSiNDY側を一旦削除する
		if( m_geospaceMode )
		{
			// 差し替え対象レイヤか？
			CString targetTable = ((CString)gEnvTable[g_DataIndex].m_OrgTableNames[i]).MakeUpper();
			auto& itr = std::find( m_layerList.begin(), m_layerList.end(), targetTable );
			if( itr != m_layerList.end() )
			{
				_tprintf(_T("差し替え対応を行います。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				_tprintf(_T("削除しています。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				// 対象なら全削除
				if( !fnSiNDYDelete( i ) )
				{
					_ftprintf(stderr, _T("#ERROR,オブジェクト削除に失敗。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
					return false;
				}
			}
			else
			{
				// 対象でなければ通常差分インポート
				_tprintf(_T("差し替えリストにありません。通常インポート。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				_tprintf(_T("比較と反映をしています（変更・削除）。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
				switch( gEnvTable[g_DataIndex].m_Type )
				{
					case CHANGE_TYPE_NORMAL:
						fnCompare(ipOldClass, ipNewClass, i);	//比較と変更・削除
						break;
					case CHANGE_TYPE_POINTS:
						fnComparePoint(ipOldClass, ipNewClass, i);	//メッシュ境界を意識しなくていい比較と変更・削除
						break;
					default:
						break;
				}
			}
			// どちらの場合も新規分のインポートは必要
			_tprintf(_T("新規分を追加してます。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i]);
			fnAddNew( ipNewClass, i ); // 新規追加
		}
		else
		{
			if( m_Command == DIFF_UPDATE )
			{
				//通常差分
				_tprintf( _T("比較と反映をしています（変更・削除）。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				switch( gEnvTable[g_DataIndex].m_Type )
				{
				case CHANGE_TYPE_NORMAL:
					fnCompare(ipOldClass, ipNewClass, i);	//比較と変更・削除
					break;
				case CHANGE_TYPE_POINTS:
					fnComparePoint(ipOldClass, ipNewClass, i);	//メッシュ境界を意識しなくていい比較と変更・削除
					break;
				case CHANGE_TYPE_POI:
					fnComparePoi(ipOldClass, ipNewClass, i); // 比較と変更 削除はしない
					break;
				default:
					break;
				}
			}
			else if( m_Command == ONLY_DELETE || m_Command == DELETE_ADD )
			{
				//特殊モード（削除あり）の時
				_tprintf( _T("既存部を切り取っています。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				fnClipData(i);
			}

			// POIの新規追加はない
			if( m_Command != ONLY_DELETE && gEnvTable[g_DataIndex].m_Type != CHANGE_TYPE_POI )
			{
				_tprintf( _T("新規分を追加してます。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[i] );
				fnAddNew(ipNewClass, i);	//新規追加
			}
		}
		//念のため明示的に破棄
		ipNewClass = nullptr;
		ipOldClass = nullptr;

		// レイヤごとの処理時間
		const auto timeSpan = chrono::system_clock::now() - start_time;
		cout << CStringA(gEnvTable[g_DataIndex].m_OrgTableNames[i]) << ":"
			<< chrono::duration_cast<chrono::milliseconds>(timeSpan).count() << "[ms]" << endl;
	}
	// 1メッシュ分処理が終わるごとにクリア
	m_crossMeshOldOid.clear();
	m_crossMeshNewOid.clear();
	m_onlyNewOid.clear();
	return true;
}

// PGDBオープン関数
IWorkspacePtr CImport::fnOpenLocalDatabase(LPCTSTR cBasePath)
{
	// 指定パスが～.mdb, ～.gdbなら、それを直接開く
	CString aExt = CString( cBasePath ).Right(4);
	if( 0 == aExt.CompareNoCase(_T(".mdb")) || 0 == aExt.CompareNoCase(_T(".gdb")) )
	{
		return fnOpenWorkspaceFromFile(cBasePath);
	}

	// 指定パスがディレクトリを指しているなら、その下にメッシュごとのPGDBがあると仮定して開く
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBasePath, g_CurrentMesh/10000, g_CurrentMesh); 
	return fnOpenWorkspaceFromFile(aDatabase);
}

// PGDB内の指定インデックスのフィーチャクラスオープン関数
IFeatureClassPtr CImport::fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace, long cIndex)
{
	IFeatureClassPtr ipFeatureClass;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(
		CComBSTR(gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]), &ipFeatureClass) != S_OK){
		_ftprintf(stderr, _T("#ERROR,フィーチャークラスが開けない。,%s\n"), gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]);
		return NULL;
	}
	return ipFeatureClass;
}

// メッシュ境界を考慮しての比較と変更・削除
bool CImport::fnCompare(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex)
{
	CString targetTable = gEnvTable[g_DataIndex].m_OrgTableNames[cIndex];

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	CComBSTR shapeField;
	ipOldClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );

	IFeatureCursorPtr ipOldCursor;
	if( FAILED( ipOldClass->Search( ipFilter, VARIANT_FALSE, &ipOldCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	ipNewClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );
	IFeatureCursorPtr ipNewCursor;
	if( FAILED( ipNewClass->Search( ipFilter, VARIANT_FALSE, &ipNewCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	// 旧PGDBデータをORG_OBJIDごとにまとめる
	map<long, list<CAdapt<IFeaturePtr>>> mapOrgIDOldFeature;
	vector<long> oldOrgObjId;
	IFeaturePtr ipOldFeature;
	while( ipOldCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		long oid = -1;
		ipOldFeature->get_OID( &oid );

		CComVariant orgObjID;
		ipOldFeature->get_Value( m_OrgIdIndex, &orgObjID );
		if( orgObjID.lVal == 0 )
		{
			_ftprintf(
			stderr, _T("WARNING,旧PGDBフィーチャのORG_OBJIDが0,フィーチャクラス:%s,OBJECTID:%ld\n"),
			targetTable, oid);
		}

		CComVariant updateStatus;
		ipOldFeature->get_Value( m_ChgStatusIndex, &updateStatus );
		if( updateStatus.lVal != NO_CHG )
		{
			_ftprintf(
			stderr, _T("WARNING,旧PGDBフィーチャのUPDATE_Cが0でない,フィーチャクラス:%s,OBJECTID:%ld,UPDATE_C:%ld\n"),
			targetTable, oid, updateStatus.lVal);
		}

		mapOrgIDOldFeature[orgObjID.lVal].push_back(ipOldFeature);
		oldOrgObjId.push_back( orgObjID.lVal );

		// 元々メッシュまたいでいるもののORG_OBJIDを保持
		// Pointは別関数に行くのでCrossesで判定
		IGeometryPtr ipGeometry;
		ipOldFeature->get_Shape( &ipGeometry );
		VARIANT_BOOL cross = VARIANT_FALSE, disjoint = VARIANT_FALSE;
		((IRelationalOperatorPtr)ipGeometry)->Crosses( m_ipMeshGeom, &cross );
		if( cross )
		{
			m_crossMeshOldOid[targetTable].push_back( orgObjID.lVal );
		}
	}

	// 新PGDBデータをORG_OBJIDごとにまとめる
	map<long, list<CAdapt<IFeaturePtr>>> mapOrgIDNewFeature;
	IFeaturePtr ipNewFeature;
	while( ipNewCursor->NextFeature( &ipNewFeature ) == S_OK && ipNewFeature )
	{
		CComVariant orgObjID;
		ipNewFeature->get_Value( m_OrgIdIndex, &orgObjID );
		if( orgObjID.lVal == 0 )
		{
			// この時点でORG_OBJID=0のオブジェクトは、新規分追加の時に対応するのでここでは弾く
			continue;
		}

		mapOrgIDNewFeature[orgObjID.lVal].push_back(ipNewFeature);

		// メッシュまたいでいるもののORG_OBJIDを保持
		// Pointは別関数に行くのでCrossesで判定
		IGeometryPtr ipGeometry;
		ipNewFeature->get_Shape( &ipGeometry );
		VARIANT_BOOL cross = VARIANT_FALSE;
		((IRelationalOperatorPtr)ipGeometry)->Crosses( m_ipMeshGeom, &cross );
		if( cross )
		{
			m_crossMeshNewOid[targetTable].push_back( orgObjID.lVal );
		}

		// NEW側にしかいないORG_OBJIDを保持(メッシュ境界はみ出すようになったようなオブジェクト)
		if( oldOrgObjId.end() == std::find( oldOrgObjId.begin(), oldOrgObjId.end(), orgObjID.lVal ) )
			m_onlyNewOid[targetTable].push_back( orgObjID.lVal );
	}

	// ORG_OBJIDをキーにしながら更新対象を探し更新する
	for( const auto& oldTarget : mapOrgIDOldFeature )
	{
		const auto& newTarget = mapOrgIDNewFeature.find( oldTarget.first );
		if( newTarget == mapOrgIDNewFeature.end() )
		{
			// new側で見つからないなら削除されたということ
			for( const auto& oldFeature : oldTarget.second )
			{
				if( m_emptyDBMode )
				{
					// 処理済みORG_OBJIDが既にいたら何もしない。
					const auto& update = std::find( m_updateOid[targetTable].begin(), m_updateOid[targetTable].end(), oldTarget.first );
					if( update != m_updateOid[targetTable].end() )
						continue;

					const auto& deleted = std::find( m_deleteOid[targetTable].begin(), m_deleteOid[targetTable].end(), oldTarget.first );
					if( deleted != m_deleteOid[targetTable].end() )
						continue;

					// UPDATE_Cを削除扱いにして更新
					// 背景だとよくあるが、対象メッシュの外に接してるパターンはここでcontinueになる。
					if( !fnInsertIntoEmptyDB( oldFeature, cIndex, DELETE_CHG ) )
						continue;

					m_deleteOid[targetTable].push_back( oldTarget.first );
					continue;
				}

				// SiNDYにあるか？
				IFeaturePtr ipFeature;
				m_ipFeatureClasses[cIndex]->GetFeature( oldTarget.first, &ipFeature );
				if( !ipFeature )
				{
					// SiNDYでも削除されている
					fnPrintLogStyle( warning, cIndex, oldTarget.first, _T("既に削除されている。") );
					continue;
				}

				// SiNDYに存在しているなら削除
				if( !fnSiNDYChange( cIndex, oldTarget.first, ipFeature, oldFeature, _T("削除") ) )
					continue;

				if( !fnOverMesh( cIndex, oldTarget.first, ipFeature ) )
				{
					// 更新前オブジェクト（SiNDY上でチェック）がメッシュをまたいでいなかったら
					// 境界上でも内側から接しているだけならOK
					fnSiNDYDeleteCore(cIndex, ipFeature, oldTarget.first, _T("削除"));
				}
				else
				{
					// 元が複数メッシュにまたがるオブジェクトだったら切り貼り
					fnClipAndApend( ipNewClass, cIndex, oldTarget.first, NULL, NO_CHG );
				}
			}
			continue;
		}

		std::vector<CImportData> importDatas;
		for( const auto& newFeature : newTarget->second )
		{
			CImportData importData;

			// ORG_OBJIDが同一で「形状のみ変更」と「属性のみ変更」の両方がいた場合、どちらかを新規扱いにしないと更新できない
			// また、「属性のみ変更」がどの属性が変更されているかも取ってみないとわからないので、一旦全部取得
			for( int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i )
			{
				CString fieldName = gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName;
				long index = gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_NewPgdbIndex;
				CComVariant val;
				newFeature->get_Value( index, &val );

				importData.m_mapFieldValue[fieldName] = val;
			}

			CComVariant status;
			newFeature->get_Value( m_ChgStatusIndex, &status );
			if( importDatas.empty() )
			{
				// 初回
				importData.m_feature = newFeature;
				importData.m_updateStatus |= status.lVal;
				importDatas.push_back( importData );
				continue;
			}

			// 属性が同じ物同士で形状をマージしていく。属性が違うやつがいたら、新規扱いにする。
			auto& itrDatas = find_if( importDatas.begin(), importDatas.end(),
				[&importData](CImportData& data){ return data.IsSameAttribute( importData ); } );

			if( itrDatas != importDatas.end() )
			{
				itrDatas->m_updateStatus |= status.lVal;
				if( !itrDatas->UnionGeometry( newFeature ) )
					fnPrintLogStyle( warning, cIndex, newTarget->first, _T("インポート前マージ失敗。") );
			}
			else
			{
				importData.m_feature = newFeature;
				importData.m_feature->put_Value( m_OrgIdIndex, CComVariant(0) ); // ORG_OBJID=0にすることで新規物扱いに
				importData.m_updateStatus |= status.lVal;
				importDatas.push_back( importData );
			}
		}

		// 更新
		for( const auto& data : importDatas )
		{
			IFeaturePtr ipNewFeature = data.m_feature;

			// ORG_OBJID=0 (新規扱い)は何より優先
			CComVariant orgObjId;
			ipNewFeature->get_Value( m_OrgIdIndex, &orgObjId );
			if( orgObjId.lVal == 0 )
			{
				// 新規扱い
				if( m_emptyDBMode )
				{
					fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
					continue;
				}

				// 空DBじゃ無いとき用の関数？必要？
			}

			// UPDATE_Cが0なら何もしない
			if( NO_CHG == data.m_updateStatus )
			{
				if( m_emptyDBMode )
				{
					m_updateOid[targetTable].push_back( orgObjId.lVal );
				}
				continue;
			}

			// 上の方の処理で変わっているかもしれないので、セットし直す
			ipNewFeature->put_Value( m_ChgStatusIndex, CComVariant(data.m_updateStatus) );

			// 以下、差分の更新
			if( m_emptyDBMode )
			{
				// NEW側がメッシュまたいでいるならまず更新
				const auto& crossNew = std::find( m_crossMeshNewOid[targetTable].begin(), m_crossMeshNewOid[targetTable].end(), orgObjId.lVal );
				if( crossNew == m_crossMeshNewOid[targetTable].end() )
				{
					// 削除扱いになっているなら、片割れは新規扱い
					const auto& deleted = std::find( m_deleteOid[targetTable].begin(), m_deleteOid[targetTable].end(), orgObjId.lVal );
					if( deleted != m_deleteOid[targetTable].end() )
						fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
					else
						fnInsertIntoEmptyDB( ipNewFeature, cIndex );

					m_updateOid[targetTable].push_back( orgObjId.lVal );
					continue;
				}

				// NEW側がメッシュをまたいでいないときは処理済みORG_OBJIDを確認(別メッシュの処理で削除扱いになっているかも)
				// 処理済みかつ、OLD側がメッシュをまたいでいないなら新規扱い
				const auto& update = std::find( m_updateOid[targetTable].begin(), m_updateOid[targetTable].end(), orgObjId.lVal );
				const auto& cross = std::find( m_crossMeshOldOid[targetTable].begin(), m_crossMeshOldOid[targetTable].end(), orgObjId.lVal );
				if( update != m_updateOid[targetTable].end() && cross == m_crossMeshOldOid[targetTable].end() )
				{
					fnInsertIntoEmptyDB( ipNewFeature, cIndex, NEW_CHG );
				}
				else
				{
					// 未処理のものは単に更新するだけ
					fnInsertIntoEmptyDB( ipNewFeature, cIndex );
				}
				m_updateOid[targetTable].push_back( orgObjId.lVal );
				continue;
			}

			if( !fnSiNDYChange( cIndex, oldTarget.first, NULL, ipNewFeature, _T("変更") ) )
			{
				// SiNDY上で削除されているものは、この先は無駄
				// 国内優先の時も次のオブジェクトへ
				continue;
			}

			if( !fnOverMesh( cIndex, oldTarget.first ) )
			{
				// 更新前オブジェクト（SiNDY上でチェック）がメッシュをまたいでいなかったら
				// 境界上でも内側から接しているだけならOK
				fnReflectToSiNDY( ipNewFeature, cIndex, oldTarget.first, data.m_updateStatus );
			}
			else
			{
				// 元が複数メッシュにまたがるオブジェクトだったら切り貼り
				fnClipAndApend( ipNewClass, cIndex, oldTarget.first, ipNewFeature, data.m_updateStatus );
			}
		}
	}
	return true;
}

// メッシュ境界を考慮しなくていいものの比較と変更・削除
// （注記とポイントが該当するが、注記がなくなったので、実質ポイントのみ）
bool CImport::fnComparePoint(IFeatureClassPtr ipOldClass, IFeatureClassPtr ipNewClass, long cIndex)
{
	IFeatureCursorPtr ipCursor;
	if( FAILED( ipOldClass->Search( NULL, VARIANT_FALSE, &ipCursor ) ) )
	{
		fnPrintComError();
		return false;
	}
	IFeaturePtr ipOldFeature;
	while( ipCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		long aObjID;
		ipOldFeature->get_OID( &aObjID );
		CComVariant aOrgID;
		ipOldFeature->get_Value( m_OrgIdIndex, &aOrgID );
		IFeaturePtr ipNewFeature;
		ipNewClass->GetFeature( aObjID, &ipNewFeature );
		if( ipNewFeature )
		{
			//変更をチェック
			CComVariant aUpdate;
			ipNewFeature->get_Value( m_ChgStatusIndex, &aUpdate );
			if( aUpdate.vt != VT_NULL && aUpdate.lVal != NO_CHG )
			{
				if( m_emptyDBMode )
				{
					fnInsertIntoEmptyDB(ipNewFeature, cIndex);
					continue;
				}

				if( !fnSiNDYChange( cIndex, aOrgID.lVal, NULL, ipOldFeature, _T("変更") ) )
				{
					//SiNDY上で削除されているから、この先は無駄
					//国内優先モードの時も
					continue;
				}
				//違いがあったら更新
				fnReflectToSiNDY( ipNewFeature, cIndex, aOrgID.lVal, aUpdate.lVal );
			}
		}
		else
		{
			// PGDB間で削除されているとき
			if( m_emptyDBMode )
			{
				fnInsertIntoEmptyDB( ipOldFeature, cIndex, DELETE_CHG );
				continue;
			}

			// インポート先にフィーチャがいれば削除
			IFeaturePtr ipFeature;
			m_ipFeatureClasses[cIndex]->GetFeature( aOrgID.lVal, &ipFeature );
			if( ipFeature )
			{
				if( fnSiNDYChange( cIndex, aOrgID.lVal, ipFeature, ipOldFeature, _T("削除") ) )
				{
					fnSiNDYDeleteCore(cIndex, ipFeature, aOrgID.lVal, _T("削除"));
				}
			}
			else
			{
				fnPrintLogStyle( warning, cIndex, aOrgID.lVal, _T("既に削除されている。") );
			}
		}
	}
	return true;
}

bool CImport::fnComparePoi( const IFeatureClassPtr& ipOldClass, const IFeatureClassPtr& ipNewClass, long cIndex )
{
	IFeatureCursorPtr ipCursor;
	if( FAILED( ipOldClass->Search( NULL, VARIANT_FALSE, &ipCursor ) ) )
	{
		fnPrintComError();
		return false;
	}

	// インポート条件に使用するので削除コードのインデックス取得
	long pgdbUpdDateIdx = -1, sdeDeleteIdx = -1, sdeUpdDateIdx = -1;
	long sdeWorkerIdx = -1, sdeWorkDateIdx = -1, sdeVerifierIdx = -1, sdeVerifyDateIdx = -1;
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kDelete), &sdeDeleteIdx );
	ipOldClass->FindField( CComBSTR(sindyk::poi_point::kUPDAvailDate), &pgdbUpdDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kUPDAvailDate), &sdeUpdDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kWorker), &sdeWorkerIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kWorkDate), &sdeWorkDateIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kVerifier), &sdeVerifierIdx );
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(sindyk::poi_point::kVerifyDate), &sdeVerifyDateIdx );
	if( pgdbUpdDateIdx < 0 || sdeDeleteIdx < 0 || sdeUpdDateIdx < 0 ||
		sdeWorkerIdx < 0 || sdeWorkDateIdx < 0 || sdeVerifierIdx < 0 || sdeVerifyDateIdx < 0 )
	{
		fnPrintLogStyle( warning, cIndex, NULL, _T("インデックス取得に失敗。") );
		return false;
	}

	// 比較開始
	IFeaturePtr ipOldFeature;
	while( ipCursor->NextFeature( &ipOldFeature ) == S_OK && ipOldFeature )
	{
		// OLD_PGDBのOID
		long oldOid = 0;
		ipOldFeature->get_OID( &oldOid );
		// SDEのOID(ORG_OBJID)
		CComVariant orgOid;
		ipOldFeature->get_Value( m_OrgIdIndex, &orgOid );
		// NEW_PGDBからOLD_PGDBのOIDで取得
		IFeaturePtr ipNewFeature;
		ipNewClass->GetFeature( oldOid, &ipNewFeature );
		if( !ipNewFeature )
		{
			// ないなら削除されたということ
			fnPrintLogStyle( error, cIndex, orgOid.lVal, _T("PGDB上で削除されている（変更なし）。") );
			continue;
		}

		//変更をチェック
		CComVariant aUpdate;
		ipNewFeature->get_Value( m_ChgStatusIndex, &aUpdate );
		if( aUpdate.vt == VT_NULL || aUpdate.lVal == NO_CHG )
			continue;

		if( m_emptyDBMode )
		{
			fnInsertIntoEmptyDB( ipNewFeature, cIndex );
			continue;
		}

		IFeaturePtr ipSdeFeature;
		m_ipFeatureClasses[cIndex]->GetFeature( orgOid.lVal, &ipSdeFeature );
		if( !ipSdeFeature )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE側オブジェクト取得失敗。") );
			continue;
		}

		// OLD_PGDBとSDEで位置が変わってる？
		IGeometryPtr oldGeom, sdeGeom;
		ipOldFeature->get_Shape( &oldGeom );
		ipSdeFeature->get_Shape( &sdeGeom );
		oldGeom->putref_SpatialReference( m_ipSpRef );
		sdeGeom->putref_SpatialReference( m_ipSpRef );
		double oldX = 0, oldY =0, sdeX = 0, sdeY = 0;
		((IPointPtr)oldGeom)->QueryCoords( &oldX, &oldY );
		((IPointPtr)sdeGeom)->QueryCoords( &sdeX, &sdeY );
		if( oldX != sdeX || oldY != sdeY )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("既にSDE上で移動されている。") );
			continue;
		}

		// SDEで削除済み？
		CComVariant deleteCode;
		ipSdeFeature->get_Value( sdeDeleteIdx, &deleteCode );
		if( deleteCode.lVal == sindyk::poi_point::delete_code::kDead )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE上で「削除」になっている。") );
			continue;
		}

		// ここまできたらポイント移動確定
		CString message;
		// 形状変更あり
		IGeometryPtr newGeom;
		ipNewFeature->get_ShapeCopy( &newGeom );
		newGeom->putref_SpatialReference( m_ipSpRef );
		ipSdeFeature->putref_Shape( newGeom );
		message = _T("【形状】");

		// 編集者・編集完了日
		SYSTEMTIME stNow = {};
		GetLocalTime( &stNow );
		DATE today;
		SystemTimeToVariantTime( &stNow, &today );

		// 未検証か？
		CComVariant sdeWorker, sdeWorkDate;
		CComVariant sdeVerifier, sdeVerifyDate;
		ipSdeFeature->get_Value( sdeWorkerIdx, &sdeWorker );
		ipSdeFeature->get_Value( sdeWorkDateIdx, &sdeWorkDate );
		ipSdeFeature->get_Value( sdeVerifierIdx, &sdeVerifier );
		ipSdeFeature->get_Value( sdeVerifyDateIdx, &sdeVerifyDate );
		if( !( ( sdeWorker.vt != VT_NULL && sdeWorkDate.vt != VT_NULL ) 
			&& ( sdeVerifier.vt == VT_NULL && sdeVerifyDate.vt == VT_NULL ) ) )
		{
			// 未検証でなければ検証者・検証完了日更新
			message += _T("【VERIFIER】【VERIFY_DATE】");
			ipSdeFeature->put_Value( sdeVerifierIdx,CComVariant(PROG_NAME));
			ipSdeFeature->put_Value( sdeVerifyDateIdx, CComVariant(today) );
		}

		// どちらにしろ編集者・編集完了日は更新
		message += _T("【WORKER】【WORK_DATE】");
		ipSdeFeature->put_Value( sdeWorkerIdx, CComVariant(PROG_NAME) );
		ipSdeFeature->put_Value( sdeWorkDateIdx, CComVariant(today) );

		// 有効日付取得
		CComVariant oldUpdDate, sdeUpdDate;
		ipOldFeature->get_Value( pgdbUpdDateIdx, &oldUpdDate );
		ipSdeFeature->get_Value( sdeUpdDateIdx, &sdeUpdDate );
				
		// 比較のためにCOleDateTimeにしたいので一旦SYSTEMTIMEで取得
		SYSTEMTIME sdeSysTime = {}, oldSysTime = {};
		VariantTimeToSystemTime( sdeUpdDate.dblVal, &sdeSysTime );
		VariantTimeToSystemTime( oldUpdDate.dblVal, &oldSysTime );
		COleDateTime sdeCTime( sdeSysTime ), oldCTime( oldSysTime );

		bool changeDate = true; // 有効日付を変更するかどうか
		// SDEオブジェクトの有効日付は指定された有効日付より先の日付か？
		if( m_availDate <= sdeCTime )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE側の有効日付が指定日付より先のため有効日付は変更しません（ポイントは移動する）。") );
			changeDate = false;
		}

		// OLD_PGDBとSDEで有効日付が変わってる？
		if( changeDate && sdeCTime != oldCTime )
		{
			fnPrintLogStyle( warning, cIndex, orgOid.lVal, _T("SDE上で有効日付が変更されているため有効日付は変更しません（ポイントは移動する）。") );
			changeDate = false;
		}

		// 有効日付更新の必要がなければ形状変更して次のフィーチャへ
		if( !changeDate )
		{
			fnSiNDYPoiChange( ipSdeFeature, ipNewFeature, cIndex, orgOid.lVal, message );
			continue;
		}

		// ここまできたら有効日付も更新 CTimeからDATEに変換
		message += _T("【UPD_AVAIL_DATE】");
		SYSTEMTIME st;
		if( !m_availDate.GetAsSystemTime(st) )
		{
			_ASSERT( false );
			continue;
		}
		DATE availDate;
		SystemTimeToVariantTime( &st, &availDate );
		ipSdeFeature->put_Value( sdeUpdDateIdx, CComVariant(availDate) );
		fnSiNDYPoiChange( ipSdeFeature, ipNewFeature, cIndex, orgOid.lVal, message );
	}
	return true;
}

bool CImport::fnSiNDYPoiChange( IFeaturePtr& sindyFeature, const IFeaturePtr& pgdbFeature, long cIndex, long oid, CString& message )
{
	fnSetCommonAttr( sindyFeature, cIndex, pgdbFeature );
	message += _T("変更");
	return fnStoreFeature(sindyFeature, cIndex, oid, message);
}

// メッシュポリゴン内削除、メッシュ境界は切り取って更新（削除・削除+更新でしか使われない）
bool CImport::fnClipData(long cIndex)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR bstrName;
	m_ipFeatureClasses[cIndex]->get_ShapeFieldName( &bstrName );
	if(ipFilter->put_GeometryField(bstrName) != S_OK){
		return false;
	}
	if(ipFilter->putref_Geometry(m_ipMeshGeom) != S_OK){
		return false;
	}
	if(ipFilter->put_SpatialRel(esriSpatialRelContains) != S_OK){
		return false;
	}

	bool aResult = false;
	//完全内包のものを先に削除
	long aCount;
	m_ipFeatureClasses[cIndex]->FeatureCount(ipFilter, &aCount);
	_ftprintf(stderr, _T("#完全内包されたオブジェクト数 = %d\n"), aCount);
	if(aCount > 0){
		if(g_ShapeType[cIndex] == esriGeometryPoint){
			if(_tcscmp(gEnvTable[g_DataIndex].m_Suffix, _T("gou")) == 0){
				CString aWhere;
				aWhere.Format(_T("%s = 1"), GOUTYPE_NAME);
				ipFilter->put_WhereClause(CComBSTR(aWhere));
			}else{
				return false;
			}
		}
		if( !m_testMode ){
			_tprintf(_T("完全内包を削除しています。\n"));
			_ftprintf(stderr, _T("#完全内包を削除しています。\n"));
			HRESULT hr = ((ITablePtr)m_ipFeatureClasses[cIndex].m_T)->DeleteSearchedRows(ipFilter);
			if(SUCCEEDED(hr)){
				aResult = true;
				_tprintf(_T("完全内包を削除しました。(%d)\n"), hr);
				_ftprintf(stderr, _T("#完全内包を削除しました。(%d)\n"), hr);
			}else{
				fnPrintComError();
				_tprintf(_T("完全内包を削除失敗。(%d)\n"), hr);
				_ftprintf(stderr, _T("#完全内包を削除失敗。(%d)\n"), hr);
			}
	//		ipFilter->put_WhereClause(CComBSTR("")); //ポイント用のWhere削除
		}
	}else{
		_tprintf(_T("完全内包のオブジェクトがありません。境界の処理をします\n"));
		_ftprintf(stderr, _T("#完全内包のオブジェクトがありません。境界の処理をします\n"));
	}
	//完全内包のものを先に削除　ここまで

	if(ipFilter->put_SpatialRel(esriSpatialRelIntersects) != S_OK){
		return false;
	}

	IFeatureCursorPtr ipFeatureCursor;
	m_ipFeatureClasses[cIndex]->FeatureCount(ipFilter, &aCount);
	_ftprintf(stderr, _T("#完全内包削除後に検索された対象オブジェクト数 = %d\n"), aCount);

	if(m_ipFeatureClasses[cIndex]->Update(ipFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK){
		fnPrintComError();
		return false;
	}
	if(g_ShapeType[cIndex] == esriGeometryPolygon || g_ShapeType[cIndex] == esriGeometryPolyline){
		aResult |= fnCutObject(ipFeatureCursor, cIndex);
	}else if(g_ShapeType[cIndex] == esriGeometryPoint){
		aResult |= fnDeletePoint(ipFeatureCursor, cIndex);
	}else{
		_ftprintf(stderr, _T("#ERROR,サポートしていない形式です。\n"));
		aResult = false;
	}
	return aResult;
}

// 新規追加分を一括追加関数
bool CImport::fnAddNew(IFeatureClassPtr ipFeatureClass, long cIndex)
{
	using namespace sindy::schema::category;
	long orgObjIdIndex = -1;
	ipFeatureClass->FindField( CComBSTR(sindyc_table::kOrgOBJID), &orgObjIdIndex );

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	CComBSTR shapeField;
	ipFeatureClass->get_ShapeFieldName( &shapeField );
	ipFilter->put_GeometryField( shapeField );

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor)))
	{
		fnPrintComError();
		return false;
	}

	IFeatureCursorPtr ipInsCursor;
	if( !m_testMode )
	{
		if(FAILED(m_ipFeatureClasses[cIndex]->Insert(VARIANT_TRUE, &ipInsCursor)))
		{
			fnPrintComError();
			return false;
		}
	}

	// 単純に新規追加なものの処理
	IFeatureBufferPtr ipBuffer;
	m_ipFeatureClasses[cIndex]->CreateFeatureBuffer(&ipBuffer);
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature)
	{
#ifdef _DEBUG
		// TODO:マルチパートだとInsertでこけるっぽい。要調査。(bug11114)
		long oid = 0;
		ipFeature->get_OID( &oid );
		if( oid == 143 )
			int stop = 0;
#endif

		if(m_Command == DIFF_UPDATE)
		{
			CComVariant orgObjId;
			ipFeature->get_Value( orgObjIdIndex, &orgObjId );
			if( !m_emptyDBMode )
			{
				if( orgObjId.vt != VT_NULL && orgObjId.lVal != 0 )
				{
					continue;
				}
			}
			else
			{
				// 空DBモードはちょっとフローが変わっているので、NEW側のUPDATE_Cを見ながら更新する。
				if( orgObjId.vt != VT_NULL && orgObjId.lVal != 0 )
				{
					CString table = g_LayerName[cIndex];
					CComVariant updateC;
					ipFeature->get_Value( g_UpdateCIndex[cIndex], &updateC );
					switch(updateC.lVal)
					{
						case SHAPE_CHG:
						case ATTR_CHG:
						case BOTH_CHG:
							{
								// 処理中のメッシュ内にOLD側がいないことがある。(NEW側がメッシュ超えて形状変更したとき)
								// そのときはここで更新してやる。
								const auto& onlyNew = std::find( m_onlyNewOid[table].begin(), m_onlyNewOid[table].end(), orgObjId.lVal );
								if( onlyNew == m_onlyNewOid[table].end() )
								{
									// OLD側がいるならfnCompareで更新されているはずなのでとばす
									continue;
								}
								break;
							}
						case NO_CHG:
						case DELETE_CHG:
							{
								// 変化なしならなにもしない
								// 削除ならfnCompareで対応済みのはず
								continue;
								break;
							}
						case NEW_CHG:
							{
								// 新規は特に条件なく処理を行う
								break;
							}
						default:
							break;
					}
				}
				// 切り貼りはfnReflectShapeまかせ
			}
		}

		if( !fnReflectShape( ipBuffer, ipFeature, cIndex, 0 ) )
			continue;

		CString aDummy;
		if( !fnReflectAttr(ipBuffer, ipFeature, cIndex, aDummy) )
			continue;

		if( !fnSetCommonAttr(ipBuffer, cIndex, ipFeature) )
			continue;

		if( m_sindyCImport )
		{
			if( FAILED(ipBuffer->put_Value( g_UpdateCIndex[cIndex], CComVariant(NEW_CHG) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value失敗。") );
				continue;
			}

			if( FAILED(ipBuffer->put_Value( g_OrgObjIdIndex[cIndex], CComVariant(0) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value失敗。") );
				continue;
			}
		}
		if(g_GouTypeIndex > -1 && cIndex == 0)
		{
			//ちょっといやらしいけど、CS対策
			//号タイプの設定
			if( FAILED(ipBuffer->put_Value(g_GouTypeIndex, CComVariant(1))) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value失敗。") );
				continue;
			}
		}
		CComVariant aID = 0;
#ifndef _DEBUG
		CString msg = _T("追加");
#else
		CString msg;
		msg.Format(_T("%ld\t追加"), oid);
#endif
		fnInsertFeature(ipInsCursor, ipBuffer, aID, cIndex, msg);
	}

	// メッシュ境界の切り貼りで境界跨いで属性違いになり、新規扱いになったものの処理
	if( m_Command == DIFF_UPDATE )
	{
		for( auto& aInsertID : m_AppendID )
		{
			ipFeatureClass->GetFeature(aInsertID, &ipFeature);
			if( !ipFeature )
			{
				fnPrintLogStyle(error, cIndex, aInsertID, _T("追加するフィーチャが見つからない。"));
				continue;
			}
			CComVariant aOrgID;
			ipFeature->get_Value(m_OrgIdIndex, &aOrgID);
			IFeaturePtr ipOrgFeature;
			m_ipFeatureClasses[cIndex]->GetFeature(aOrgID.lVal, &ipOrgFeature);
			IFeatureBufferPtr ipBuffer;
			m_ipFeatureClasses[cIndex]->CreateFeatureBuffer(&ipBuffer);
			if( !fnReflectShape( ipBuffer, ipFeature, cIndex, aOrgID.lVal ) )
				continue;

			CString aDummy;
			if( !fnReflectAttr(ipBuffer, ipFeature, cIndex, aDummy) )
				continue;

			if( !fnSetCommonAttr(ipBuffer, cIndex, ipOrgFeature) )
				continue;

			if( m_sindyCImport )
				ipBuffer->put_Value( g_UpdateCIndex[cIndex], CComVariant(NEW_CHG) );

			CComVariant aID = 0;
			CString msg = _T("追加(新規扱い)");
			fnInsertFeature(ipInsCursor, ipBuffer, aID, cIndex, msg);
		}
	}

	if( !m_testMode )
	{
		if( FAILED( ipInsCursor->Flush() ) )
		{
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数
// 関数名と戻り値が逆っぽい気もするが、更新できない・更新してはいけない時にfalseを返す
bool CImport::fnSiNDYChange(long cIndex, long cObjID, IFeaturePtr ipOrgFeature, IFeaturePtr ipPgdbFeature, LPCTSTR cChgType)
{
	//Export後に変更があったかチェック
	if( !ipOrgFeature )
	{
		m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipOrgFeature );
		if( !ipOrgFeature )
		{
			long aObjID;
			ipPgdbFeature->get_OID( &aObjID );
			fnPrintLogStyle( error, cIndex, cObjID, _T("変更対象がSiNDY上で削除されている。") );
			return false;
		}
	}

	CComVariant aSindyModDate;
	ipOrgFeature->get_Value( g_ModifyDateIndex[cIndex], &aSindyModDate );
	CComVariant aPgdbModDate;
	ipPgdbFeature->get_Value( m_OrgModifyDateIndex, &aPgdbModDate );
	if( aSindyModDate.date != aPgdbModDate.date )
	{
		if( m_tdcMode )
		{
			//国内優先モードだったら、次に行っちゃダメ
			CString aMsg = CString(_T("変更対象がSiNDY上で変更されているため反映しません(")) + CString(cChgType) + _T(")");
			fnPrintLogStyle( warning, cIndex, cObjID, aMsg );
			return false;
		}
		else
		{
			//通常はメッセージだけ
			fnPrintLogStyle( warning, cIndex, cObjID, _T("変更対象がSiNDY上で変更されている。") );
		}
	}
	return true;
}

// メッシュをまたいだオブジェクトかを判定する関数
bool CImport::fnOverMesh(long cIndex, long cObjID, IFeaturePtr ipFeature)
{
	if( !ipFeature )
	{
		m_ipFeatureClasses[cIndex]->GetFeature(cObjID, &ipFeature);
		if( !ipFeature )
		{
			//fnSiNDYChange()で弾くからここには来ないはず
			fnPrintLogStyle(critical, cIndex, cObjID, _T("変更対象がSiNDY上で削除されている（出てはいけないメッセージ）。"));
			return false;
		}
	}

	IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, cObjID);
	if( !ipGeom )
	{
		return false;
	}

	IPointCollectionPtr ipPoints(ipGeom);
	long aCount = 0;
	ipPoints->get_PointCount(&aCount);

	//要チェック！
	if( ( g_ShapeType[cIndex] == esriGeometryPolygon && aCount > 3 )
		|| ( g_ShapeType[cIndex] == esriGeometryPolyline && aCount > 1 ) )
	{
		return true;
	}
	return false;
}

// メッシュをまたぐオブジェクトを切り貼りして更新する関数
bool CImport::fnClipAndApend(IFeatureClassPtr ipNewClass, long cIndex, long cObjID, IFeaturePtr ipNewFeature, long cStatus)
{
	IFeaturePtr ipFeature;
	if( FAILED( m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipFeature ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( !ipFeature )
	{
		fnPrintLogStyle(error, cIndex, cObjID, _T("フィーチャーが見つからない。"));
		return false;
	}

	//切り貼り処理
	IGeometryPtr ipGeom = fnGetRemakeShape(ipNewClass, ipFeature, cIndex, cObjID);
	if( !ipGeom )
	{
		fnPrintLogStyle(error, cIndex, cObjID, _T("メッシュ境界切り貼り失敗。"));
		return false;
	}

	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount = 0;
	if( FAILED( ipGeoCol->get_GeometryCount( &aCount ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( aCount > 1 )
	{
		esriGeometryType aShapeType;
		if( FAILED( m_ipFeatureClasses[cIndex]->get_ShapeType( &aShapeType ) ) )
		{
			fnPrintLogStyle( error, cIndex, cObjID, _T("ShapeType取得失敗。") );
			return false;
		}
		switch(aShapeType)
		{
		case esriGeometryPolygon:
			{
				msi_get_parts_info aPolUtil;
				if( aPolUtil.Init( ipGeom, cObjID, (string)CT2A(g_LayerName[cIndex]).m_psz, 0, stderr ) )
				{
					aCount = aPolUtil.Get_Part_Num();
					if( aCount > 1 )
					{
						cStatus = NO_CHG;	//属性の変更は反映させないため
						fnPrintLogStyle( warning, cIndex, cObjID, _T("再構成後複数に分かれた。") );
					}
				}
				else
				{
					fnPrintLogStyle( error, cIndex, cObjID, _T("再構成後ポリゴンの分析失敗。") );
					return false;
				}
			}
			break;
		case esriGeometryPolyline:
			{
				cStatus = NO_CHG;	//属性の変更は反映させないため
				fnPrintLogStyle( warning, cIndex, cObjID, _T("再構成後複数に分かれた(ライン)。") );
			}
			break;
		default:
			{
				fnPrintLogStyle( critical, cIndex, cObjID, _T("サポート外（絶対バグ）。") );
				return false;
			}
			break;
		}
	}
	else if(aCount < 1)
	{
		if( !ipNewFeature || cStatus & ATTR_CHG )
		{
			//単純削除で可
			return fnSiNDYDeleteCore(cIndex, ipFeature, cObjID, _T("メッシュ境界削除"));
		}
		else
		{
			fnPrintLogStyle( error, cIndex, cObjID, _T("切り貼り後の形状が不正。") );
			return false;
		}
	}

	ipFeature->putref_Shape(ipGeom);
	fnSetCommonAttr(ipFeature, cIndex, ipFeature);
	CString logMessage = _T("メッシュ境界変更");
	return fnStoreFeature(ipFeature, cIndex, cObjID, logMessage);
}

// メッシュをまたいでいないものの単純更新関数
bool CImport::fnReflectToSiNDY(IFeaturePtr ipNewFeature, long cIndex, long cObjID, long cStatus)
{
	IFeaturePtr ipFeature;
	if( FAILED( m_ipFeatureClasses[cIndex]->GetFeature( cObjID, &ipFeature ) ) )
	{
		fnPrintComError();
		return false;
	}
	if( !ipFeature )
	{
		fnPrintLogStyle( critical, cIndex, cObjID, _T("フィーチャーが見つからない。") );
		return false;
	}
		
	CString aMsg;
	if( ( cStatus & SHAPE_CHG ) )
	{
		//形状変更あり
		if( !fnReflectShape( ipFeature, ipNewFeature, cIndex, cObjID ) )
			return false;
		aMsg = _T("【形状】");
	}
	if( ( cStatus & ATTR_CHG ) )
	{
		//属性変更あり
		if( !fnReflectAttr( ipFeature, ipNewFeature, cIndex, aMsg ) )
			return false;
	}

	if( !fnSetCommonAttr( ipFeature, cIndex, ipFeature ) )
		return false;

	aMsg += _T("変更");
	if( !fnStoreFeature( ipFeature, cIndex, cObjID, aMsg ) )
		return false;

	return true;
}

// メッシュポリゴンでクリップして更新または削除する関数
bool CImport::fnCutObject(IFeatureCursorPtr ipCursor, long cIndex)
{
	IRelationalOperatorPtr ipRel = m_ipMeshGeom;
	IFeaturePtr ipFeature;
	bool aResult = false;
	HRESULT aHResult;
	int aCount = 0;

	while( ( aHResult = ipCursor->NextFeature(&ipFeature) ) == S_OK && ipFeature )
	{
		aCount++;
		long aObjID = 0;
		ipFeature->get_OID( &aObjID );
		IGeometryPtr ipGeometry;
		if( FAILED( ipFeature->get_Shape(&ipGeometry) ) ){
			fnPrintComError();
			return false;
		}
		VARIANT_BOOL aIsTouch = VARIANT_FALSE;
		if( FAILED( ipRel->Touches(ipGeometry, &aIsTouch) ) ){
			fnPrintLogStyle(error, cIndex, aObjID, _T("判定不能。"));
			continue;
		}

		//外側で接するのは対象外
		if( aIsTouch ){
			fnPrintLogStyle(info, cIndex, aObjID, _T("対象外。"));
			aResult = true;
		}

		VARIANT_BOOL aIsCross = VARIANT_FALSE;
		if( g_ShapeType[cIndex] == esriGeometryPolygon ){
			ipRel->Overlaps(ipGeometry, &aIsCross);
		}else{
			ipRel->Crosses(ipGeometry, &aIsCross);
		}

		if( aIsCross ){
			IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, aObjID, true);
			if( ipGeom ){
				ipFeature->putref_Shape(ipGeom);
				CString logMessage = _T("変形");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}else{
				//そのまま削除
				CString logMessage = _T("削除");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}
		}else{
			VARIANT_BOOL aIsContains = VARIANT_FALSE;
			if(FAILED(ipRel->Contains(ipGeometry, &aIsContains))){
				fnPrintComError();
				fnPrintLogStyle(error, cIndex, aObjID, _T("包含関係判定不能。"));
				continue;
			}

			if(aIsContains){
				//完全内包（先に消えてるはずだけど、念のため）
				//そのまま削除
				CString logMessage = _T("削除");
				aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
			}else{
				//メッシュを完全に含む（行政界・海等ではありえる）
				IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cIndex, aObjID, true);
				if( ipGeom ){
					ipFeature->putref_Shape(ipGeom);
					CString logMessage = _T("変形(繰り抜き)");
					aResult = fnStoreFeature( ipFeature, cIndex, aObjID, logMessage );
				}else{
					//そのまま削除
					aResult = fnSiNDYDeleteCore(cIndex, ipFeature, aObjID, _T("削除(要確認)"));
				}
			}
		}
	}
	_ftprintf(stderr, _T("#HRESULT = %d\n"), aHResult);
	_ftprintf(stderr, _T("#処理したCount = %d\n"), aCount);
	if( FAILED(aHResult) ){
		fnPrintComError();
	}
	return aResult;
}

// 検索されたオブジェクトを無条件で削除する関数（主にポイント用）
bool CImport::fnDeletePoint(IFeatureCursorPtr ipCursor, long cIndex)
{
	IFeaturePtr ipFeature;
	HRESULT aHResult;
	int aCount = 0;
	while((aHResult = ipCursor->NextFeature(&ipFeature)) == S_OK && ipFeature){
		aCount++;
		//そのまま削除
		long aObjID = 0;
		ipFeature->get_OID(&aObjID);
		fnSiNDYDeleteCore(cIndex, ipFeature, aObjID, _T("削除"));
	}
	_ftprintf(stderr, _T("#HRESULT = %d\n"), aHResult);
	_ftprintf(stderr, _T("#処理したCount = %d\n"), aCount);
	if(FAILED(aHResult)){
		fnPrintComError();
	}
	return true;
}

// 反映対象属性のコピー関数
bool CImport::fnReflectAttr(IFeaturePtr ipTgtFeature, IFeaturePtr ipSrcFeature, long cIndex, CString& cField)
{
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; i++){
		CComVariant aSrcVal, aTgtVal;
		ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_NewPgdbIndex, &aSrcVal);
		ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &aTgtVal);
		if(aSrcVal != aTgtVal){
			ipTgtFeature->put_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, aSrcVal);
			cField += _T("【");
			cField += gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName;
			cField += _T("】");
		}
	}
	return true;
}

// メッシュポリゴンでクリップした形状を求める
// fnGetRemakeShapeと似ているように見えるが、こちらは、メッシュポリゴンで切り取った残りのジオメトリを求める
IGeometryPtr CImport::fnGetClipShape(IFeaturePtr ipFeature, long cIndex, long cObjID, bool cForceMode)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	if(!cForceMode){
		//前のメッシュの影響でマルチでも切り取りが必要になることがあるから、
		//安易に省略できない　2008/02/01
		if(g_ShapeType[cIndex] == esriGeometryPolygon){
			msi_get_parts_info aPolUtil;
			if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(g_LayerName[cIndex]).m_psz, 0, stderr)){
				if(aPolUtil.Get_Part_Num() > 1){
					//強引だけど、マルチパートなら間違いなくメッシュ境界のはず
					//元が問題なく、切り貼りの途中でのみマルチパートになる前提
					return ipGeom;
				}
			}else{
				//どうしようもない
				fnPrintLogStyle(error, cIndex, cObjID, _T("判定時ポリゴンの分析失敗。"));
				return ipGeom;
			}
		}else{
			IGeometryCollectionPtr ipGeoCol = ipGeom;
			long aCount;
			if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
				fnPrintComError();
				return ipGeom;
			}
			if(aCount > 1){
				//強引だけど、マルチパートなら間違いなくメッシュ境界のはず
				//元が問題なく、切り貼りの途中でのみマルチパートになる前提
				return ipGeom;
			}
		}
	}
	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//元データをメッシュで切り取り
	if(FAILED(ipTopo->Difference(m_ipMeshGeom, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyle(error, cIndex, cObjID, _T("切り取り失敗。"));
		return ipGeom;
	}
	ipTgtGeom->putref_SpatialReference( m_ipSpRef );
	return ipTgtGeom;
}

// メッシュ境界のオブジェクトに対して、当該メッシュ外部分と当該メッシュ内の切り貼りを行う
// マルチパートになることもあり、オリジナルIDが同じものを1つにまとめようとするが、属性が変更になっているものがあれば新規追加扱いとする
IGeometryPtr CImport::fnGetRemakeShape(IFeatureClassPtr ipNewClass, IFeaturePtr ipFeature, long cIndex, long cObjID)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//元データをメッシュで切り取り
	if(FAILED(ipTopo->Difference(m_ipMeshGeom, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyle(error, cIndex, cObjID, _T("切り取り失敗。"));
		return NULL;
	}

	ipTgtGeom->putref_SpatialReference( m_ipSpRef );

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format(_T("%s = %d"), ORG_OBJID_NAME, cObjID);
	ipFilter->put_WhereClause(CComBSTR(aWhere));

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipNewClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return NULL;
	}
	IFeaturePtr ipSrcFeature;
	while(ipCursor->NextFeature(&ipSrcFeature) == S_OK && ipSrcFeature){
		ipTopo = ipTgtGeom;
		CComVariant aUpdate;
		ipSrcFeature->get_Value(m_ChgStatusIndex, &aUpdate);
		if(aUpdate.lVal & ATTR_CHG){
			//属性変更があったやつは新規扱い
			long aObjID = 0;
			ipSrcFeature->get_OID(&aObjID);
			m_AppendID.insert(aObjID);
			continue;
		}

		IGeometryPtr ipSrcGeom, ipTgtGeom2;
		ipSrcFeature->get_Shape(&ipSrcGeom);
		if( FAILED(ipTopo->Union(ipSrcGeom, &ipTgtGeom2)) )
		{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, cObjID, _T("切り取り後のマージ失敗。"));
			return NULL;
		}
		ipTgtGeom = ipTgtGeom2;
	}

	return ipTgtGeom;
}

// sindyのフィーチャを削除する (GS対応)
bool CImport::fnSiNDYDelete( const int cIndex )
{
	// Buildingの場合は削除する際GEOSPACE_IDを出す必要がある
	bool isBuilding = false;
	if( 0 == ((CString)gEnvTable[g_DataIndex].m_OrgTableNames[cIndex]).CompareNoCase( building::kTableName ) )
		isBuilding = true;

	// メッシュ矩形とのInterSects取得
	IFeatureCursorPtr ipCursor;
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR bstrFieldName;
	m_ipFeatureClasses[cIndex]->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( m_ipMeshGeom );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
	
	m_ipFeatureClasses[cIndex]->Search( ipFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
		return false;

	long geoIDIndex = -1;
	m_ipFeatureClasses[cIndex]->FindField( CComBSTR(GEOSPACE_ID_NAME), &geoIDIndex );

	// 削除
	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		long oid = 0;
		ipFeature->get_OID( &oid );
		// メッシュ矩形の外側にはみ出すようなやつは外側部分の形状を残す
		IGeometryPtr ipGeom, ipDiffGeom;
		ipFeature->get_ShapeCopy( &ipGeom );
		ITopologicalOperatorPtr ipTopo( ipGeom );
		if( FAILED(ipTopo->Difference( m_ipMeshGeom, &ipDiffGeom )) ) // メッシュの外側取得
		{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, oid, _T("メッシュ形状での切り取り失敗。"));
			continue;
		}
		ipDiffGeom->putref_SpatialReference( m_ipSpRef );
		esriGeometryType geomType = esriGeometryNull;
		ipGeom->get_GeometryType( &geomType );
		switch( geomType )
		{
		case esriGeometryPolyline:
			{
				// はみだし分の長さ
				// ラインはメッシュ境界で切れている気もする
				double length = 0;
				((IPolylinePtr)ipDiffGeom)->get_Length( &length );
				if( length == 0 )
				{
					// はみ出してないなら削除
					fnSiNDYDeleteCore( cIndex, ipFeature, oid, _T("削除。") );
				}
				else
				{
					// はみ出してるなら、はみ出し分の形状だけ残す
					fnSiNDYPutDifferenceGeom( cIndex, ipFeature, oid, ipDiffGeom );
				}
				break;
			}
		case esriGeometryPolygon:
			{
				// はみだし分の面積
				double area = 0;
				((IAreaPtr)ipDiffGeom)->get_Area( &area );
				if( area == 0 )
				{
					CString msg = _T("削除。");
					if( isBuilding )
					{
						// 家形削除する場合はGEOSPACEIDを取得
						CComVariant geoID;
						ipFeature->get_Value( geoIDIndex, &geoID );
						CString strGeoID( geoID.bstrVal );
						if( strGeoID.IsEmpty() )
							strGeoID = _T("なし");
						msg.AppendFormat( _T(" GEOSPACE_ID：%s"), strGeoID );
					}
					// はみ出してないなら削除
					fnSiNDYDeleteCore( cIndex, ipFeature, oid, msg );
				}
				else
				{
					// はみ出してるなら、はみ出し分の形状だけ残す
					fnSiNDYPutDifferenceGeom( cIndex, ipFeature, oid, ipDiffGeom );
				}
				break;
			}
		default:
			{
				// 来るとしたらポイント
				fnSiNDYDeleteCore( cIndex, ipFeature, oid, _T("削除。") );
				break;
			}
		}
	}

	return true;
}

bool CImport::fnSiNDYDeleteCore( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const CString& message )
{
	if( m_testMode )
	{
		fnPrintLogStyle( info, cIndex, oid, _T("削除される。") );
		return true;
	}

	if( FAILED( ipFeature->Delete() ) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, oid, _T("削除失敗。") );
		return false;
	}
	fnPrintLogStyle( info, cIndex, oid, message );
	return true;
}

bool CImport::fnSiNDYPutDifferenceGeom( const int cIndex, const IFeaturePtr& ipFeature, const long oid, const IGeometryPtr& ipDiffGeom )
{
	ipFeature->putref_Shape( ipDiffGeom );
	CString logMessage = _T("形状変更");
	return fnStoreFeature( ipFeature, cIndex, oid, logMessage );
}

template <typename T>
bool CImport::fnReflectShape( T& ipTgtFeature, const IFeaturePtr& ipSrcFeature, long cIndex, long cObjID )
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_ShapeCopy( &ipGeom );

	// GS協業以降は、NULL形状のオブジェクトがいる可能性がある
	// そういうのはここでリターンしてしまう
	if( !ipGeom )
		return false;

	// 処理対象メッシュからはみ出す分は切っておく
	IEnvelopePtr ipMeshEnvelope;
	m_ipMeshGeom->get_Envelope( &ipMeshEnvelope );
	if( FAILED(((ITopologicalOperatorPtr)ipGeom)->Clip( ipMeshEnvelope )) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, cObjID, _T("Clip失敗。") );
		return false;
	}

	// ここで扱うものはメッシュのintersectsで取っているオブジェクト。
	// GS新フローだと元データがメッシュ範囲内に収まってないので、
	// Clipした結果null形状ということがありうる。そういうときはreturn false。
	esriGeometryType type = esriGeometryNull;
	ipGeom->get_GeometryType( &type );
	switch( type )
	{
	case esriGeometryPolygon:
		{
			double area = 0;
			((IAreaPtr)ipGeom)->get_Area( &area );
			if( area == 0 )
			{
				return false;
			}
			break;
		}
	case esriGeometryPolyline:
		{
			double length = 0;
			((IPolylinePtr)ipGeom)->get_Length( &length );
			if( length == 0 )
			{
				return false;
			}
			break;
		}
	default:
		break;
	}

	if( FAILED( ipTgtFeature->putref_Shape( ipGeom ) ) )
	{
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, cObjID, _T("形状コピー失敗。") );
		return false;
	};
	return true;
}

bool CImport::fnStoreFeature( const IFeaturePtr& sindyFeature, long cIndex, long oid, CString &message )
{
	if( m_testMode )
	{
		message += _T("される。");
		fnPrintLogStyle( info, cIndex, oid, message );
		return true;
	}

	if( FAILED( sindyFeature->Store() ) )
	{
		message += _T("失敗。");
		fnPrintComError();
		fnPrintLogStyle( error, cIndex, oid, message );
		return false;
	}
	message += _T("。"); //他メッセージと合わせるための句点
	fnPrintLogStyle( info, cIndex, oid, message );
	return true;
}

bool CImport::fnInsertFeature(IFeatureCursorPtr& ipInsCursor, const IFeatureBufferPtr& ipBuffer, CComVariant &aID, long cIndex, CString& message)
{
	if( m_testMode )
	{
		message += _T("される。");
		fnPrintLogStyle(info, cIndex, aID.lVal, message);
		return true;
	}

	if( FAILED( ipInsCursor->InsertFeature( ipBuffer, &aID ) ) )
	{
		message += _T("失敗。");
		fnPrintLogStyle(error, cIndex, aID.lVal, message);
		return false;
	}

	message += _T("。");

	fnPrintLogStyle(info, cIndex, aID.lVal, message);
	return true;
}

bool CImport::fnInsertIntoEmptyDB( const IFeaturePtr& ipNewFeature, long cIndex, long updateStatus /* = -1 */ )
{
	IFeatureCursorPtr ipInsertCursor;
	if( !m_testMode )
	{
		if( FAILED( m_ipFeatureClasses[cIndex]->Insert( VARIANT_TRUE, &ipInsertCursor ) ) )
		{
			fnPrintComError();
			return false;
		}
	}
	IFeatureBufferPtr ipFeature;
	m_ipFeatureClasses[cIndex]->CreateFeatureBuffer( &ipFeature );
	if( !fnReflectShape( ipFeature, ipNewFeature, cIndex, 0 ) )
		return false;

	CString dummy;
	if( !fnReflectAttr( ipFeature, ipNewFeature, cIndex, dummy ) )
		return false;

	if( !fnSetCommonAttr( ipFeature, cIndex, ipNewFeature ) )
		return false;

	// updateStatus == -1 なら fnSetCommonAttr() でセットされた値のままで良い
	if( m_sindyCImport && updateStatus != -1 )
	{
		if( FAILED(ipFeature->put_Value( g_UpdateCIndex[cIndex], CComVariant(updateStatus) ) ) )
		{
			fnPrintComError();
			fnPrintLogStyle( error, cIndex, 0, _T("put_Value失敗。") );
			return false;
		}

		// 新規扱いのものはORG_OBJIDが必ず0になるようにする。
		if( updateStatus == NEW_CHG )
		{
			if( FAILED(ipFeature->put_Value( g_OrgObjIdIndex[cIndex], CComVariant(0) ) ) )
			{
				fnPrintComError();
				fnPrintLogStyle( error, cIndex, 0, _T("put_Value失敗。") );
				return false;
			}
		}
	}

	CComVariant id;
	CString msg = _T("追加（空DBインポート）");
	if( !fnInsertFeature( ipInsertCursor, ipFeature, id, cIndex, msg ) )
		return false;

	return true;
}

IWorkspacePtr CImport::fnOpenWorkspaceFromFile(LPCTSTR filePath)
{
	IWorkspacePtr ipWorkspace = sindy::create_workspace(filePath);
	if( !ipWorkspace )
	{
		_ftprintf( stderr, _T("#ERROR,%s が開けませんでした\n"), filePath );
		return nullptr;
	}
	return ipWorkspace;
}
