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
#include "Reflect.h"
#include "Import.h"
#include "MeshMerge.h"
#include "SeparatePolygon.h"
#include "DelSinglePoint.h"

using namespace sindy::schema;

namespace Reflect
{
	esriGeometryType g_ShapeType[fc_max_size] = {esriGeometryNull};
	CString g_LayerName[fc_max_size];
	long g_OperatorIndex[fc_max_size] = {-1};
	long g_UpdateIndex[fc_max_size] = {-1};
	long g_PurposeIndex[fc_max_size] = {-1};
	long g_ClaimIndex[fc_max_size] = {-1};
	long g_ProgNameIndex[fc_max_size] = {-1};
	long g_ModifyDateIndex[fc_max_size] = {-1};
	long g_ProgModifyDateIndex[fc_max_size] = {-1};
	long g_SourceIndex[fc_max_size] = {-1};
	long g_OrgOperatorIndex[fc_max_size];
	long g_OrgModifyIndex[fc_max_size];
	long g_OrgObjIdIndex[fc_max_size];
	long g_UpdateCIndex[fc_max_size];
	long g_CheckCIndex[fc_max_size];
	long g_MeshcodeIndex[fc_max_size];
	long g_GouTypeIndex = -1;
	long g_CurrentMesh = -1;
	bool g_IsContinue = false;
	CString g_Suffix = _T("");
	MESH_MAP g_MeshMap;
	MESH_POLY_MAP g_MeshPolyMap;
	long g_NowProccess = -1;
	int g_DataIndex = -1;
	CString g_Operator = _T("");
	
	CString gMeshPoly = _T("CityMesh");

	LPCTSTR GOUTYPE_NAME = _T("GOUTYPE");
	LPCTSTR PROG_NAME = _T("DiffImportPGDB");
}

namespace {
	
	// GS対応で追加 "City_Area", "City_Bridge_Line", "City_Tunnel", "City_Road", "Building_Roof"
	// [2015.01] GS対応で次のレイヤを追加 "City_Divider" 不要になったので次のレイヤを削除 "City_Bridge_Line"
	LPCTSTR gFrmTables[] = {
		city_site::kTableName, city_line::kTableName, city_area::kTableName,
		city_tunnel::kTableName, city_road::kTableName, city_divider::kTableName, NULL};
	LPCTSTR gHigTables[] = {
		building::kTableName, building_line::kTableName, building_step::kTableName,
		building_roof::kTableName, NULL};
	LPCTSTR gRalTables[] = {city_station::kTableName, city_railway::kTableName, NULL};
	LPCTSTR gAdmTables[] = {city_admin::kTableName, NULL};
	LPCTSTR gGouTables[] = {gou_point::kTableName, cs_addr_point::kTableName, NULL};
//	LPCTSTR gAnoTables[] = {_T("City_Annotation"), _T("City_Disp_Line"), NULL};
	LPCTSTR gRefTables[] = {_T("RefS_Point"), NULL};
	LPCTSTR gPoiTables[] = {_T("Export_Poi_Point"), NULL};

	//サーバ上の別のフィーチャクラスにロードする場合のために分けた
	//環境変数で変更可能
	CString gTgtFrmTables[] = {
		city_site::kTableName, city_line::kTableName, city_area::kTableName,
		city_tunnel::kTableName, city_road::kTableName, city_divider::kTableName, NULL};
	CString gTgtHigTables[] = {
		building::kTableName, building_line::kTableName, building_step::kTableName,
		building_roof::kTableName, NULL};
	CString gTgtRalTables[] = {city_station::kTableName, city_railway::kTableName, NULL};
	CString gTgtAdmTables[] = {city_admin::kTableName, NULL};
	CString gTgtGouTables[] = {gou_point::kTableName, cs_addr_point::kTableName, NULL};
	CString gTgtAnoTables[] = {city_annotation::kTableName, city_disp_line::kTableName, NULL};
	CString gTgtRefTables[] = {_T("RefS_Point"), NULL};
	CString gTgtPoiTables[] = {sindyk::poi_point::kTableName, NULL};


	// 建物
	FieldTable gHigPFields[] = {
		{building::kBldClass,  -1, -1, -1, true},
		{building::kCorridor,  -1, -1, -1, true},
		{building::kNoWall,    -1, -1, -1, true}, // GEOSPACE新設
		{building::kGeospaceID, -1, -1, -1, true}, // GEOSPACE新設
		{NULL, -1, -1, -1, false}
	};

	// 建物ライン
	FieldTable gHigAFields[] = {
		{building_line::kBldClass, -1, -1, -1, true},
		{NULL, -1, -1, -1}
	};

	// 建物階数
	FieldTable gStpPFields[] = {
		{building_step::kFloors, -1, -1, -1, true},
		{NULL, -1, -1, false}
	};
	
	// GEOSPACE新設 建物屋上
	FieldTable gFroPFields[] = {
		{building_roof::kRfClass, -1, -1, -1, true}, // 屋上施設種別
		{NULL, -1, -1}
	};

	FieldTable *gHigFields[] = {
		gHigPFields,
		gHigAFields,
		gStpPFields,
		gFroPFields
	};

	// 背景
	FieldTable gFrmPFields[] = {
		{city_site::kBgClass,  -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	// 背景ライン
	FieldTable gFrmAFields[] = {
		{city_line::kBgClass, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};
	
	// GEOSPACE新設 都市地図エリア
	FieldTable gArePFields[] = {
		{city_area::kAreaClass, -1, -1, -1, true}, // エリア種別
		{NULL, -1, -1}
	};
	
	// GEOSPACE新設 都市地図トンネル
	FieldTable gTunPFields[] = {
		{city_tunnel::kTunnelClass, -1, -1, -1, true}, // トンネル種別
		{NULL, -1, -1}
	};

	// GEOSPACE新設 都市地図道路
	FieldTable gRodPFields[] = {
		{city_road::kRoadClass, -1, -1}, // 道路ポリゴン種別
		{NULL, -1, -1}
	};

	// GEOSPACE新設 都市地図料金所分離帯
	FieldTable gDivPFields[] = {
		{city_divider::kDividerClass, -1, -1, -1, true}, // 料金所分離帯種別
		{NULL, -1, -1}
	};

	FieldTable *gFrmFields[] = {
		gFrmPFields,
		gFrmAFields,
		gArePFields,
		gTunPFields,
		gRodPFields,
		gDivPFields
	};

	// 鉄道駅
	FieldTable gRalPFields[] = {
		{city_station::kStationClass, -1, -1, -1, true},
		{city_station::kUnderGround, -1, -1, -1, true},
		{NULL, -1, -1}
	};

	// 鉄道ライン
	FieldTable gRalAFields[] = {
		{city_railway::kRailwayClass, -1, -1, -1, true},
		{city_railway::kUnderGround, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gRalFields[] = {
		gRalPFields,
		gRalAFields
	};

	// 行政界
	FieldTable gAdmPFields[] = {
		{city_admin::kCityCode,  -1, -1, -1, true},
		{city_admin::kAddrCode, -1, -1, -1, true},
		{city_admin::kAddrCode2, -1, -1, -1, true},
		{city_admin::kGaikuFugo, -1, -1, -1, true},
		{city_admin::kExtGaikuFugo, -1, -1, -1, true},
		{city_admin::kAddrClass, -1, -1, -1, true},
		{city_admin::kAreaClass, -1, -1, -1, true},
		{city_admin::kManage, -1, -1, -1, false},
		{city_admin::kColorCode, -1, -1, -1, false},
		{city_admin::kConvType, -1, -1, -1, true},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gAdmFields[] = {
		gAdmPFields,
	};

	// 号種別
	FieldTable gGouPFields[] = {
		{gou_point::kGouNo,  -1, -1, -1, false},
		{gou_point::kExpGouNo, -1, -1, -1, false},
		{gou_point::kPriority, -1, -1, -1, false},
		{gou_point::kConfirm, -1, -1, -1, false},
		{NULL, -1, -1}
	};

	// CS種別
	FieldTable gCsPFields[] = {
		{cs_addr_point::kAddrCode,  -1, -1, -1, false},
		{cs_addr_point::kSource,  -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable *gGouFields[] = {
		gGouPFields,
		gCsPFields
	};

	// 注記種別
	FieldTable gAnoPFields[] = {
		{city_annotation::kNameString1,  -1, -1, -1, false},
		{city_annotation::kNameString2, -1, -1, -1, false},
		{city_annotation::kStringNum1, -1, -1, -1, false},
		{city_annotation::kStringNum2, -1, -1, -1, false},
		{city_annotation::kAnnoCode, -1, -1, -1, false},
		{city_annotation::kDispType, -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable gAnoAFields[] = {
		{city_disp_line::kFontSize, -1, -1, -1, false},
		{city_disp_line::kTargetStr, -1, -1, -1, false},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gAnoFields[] = {
		gAnoPFields,
		gAnoAFields
	};

	// 昭文社拡充作業用種別
	FieldTable gRefPFields[] = {
		{_T("ADDRCODE"), -1, -1, -1, false},
		{_T("GOU_NO"),  -1, -1, -1, false},
		{NULL, -1, -1, false}
	};

	FieldTable *gRefFields[] = {
		gRefPFields,
	};
	
	// POI_POINTインポート対象は緯度経度と有効日付のみ
	// ちなみに ContentsSeq, SerialID, Name はエクスポート対象になっている
	FieldTable gPoiPFields[] = {
		{sindyk::poi_point::kUPDAvailDate, -1, -1, -1, false},
		{NULL, -1, -1, -1, false}
	};

	FieldTable *gPoiFields[] = {
		gPoiPFields
	};
}
namespace Reflect{
	const EnvTable gEnvTable[] = {
		{_T("hig"), gHigTables, gTgtHigTables, gHigFields, CHANGE_TYPE_NORMAL},
		{_T("frm"), gFrmTables, gTgtFrmTables, gFrmFields, CHANGE_TYPE_NORMAL},
		{_T("ral"), gRalTables, gTgtRalTables, gRalFields, CHANGE_TYPE_NORMAL},
		{_T("adm"), gAdmTables, gTgtAdmTables, gAdmFields, CHANGE_TYPE_NORMAL},
		{_T("ref"), gRefTables, gTgtRefTables, gRefFields, CHANGE_TYPE_POINTS},
		{_T("gou"), gGouTables, gTgtGouTables, gGouFields, CHANGE_TYPE_POINTS},
		{_T("poi"), gPoiTables, gTgtPoiTables, gPoiFields, CHANGE_TYPE_POI},
		{NULL, NULL, NULL, NULL, -1}
	};

} //namespace

using namespace Reflect;

// コンストラクタ
CReflect::CReflect(void)
: m_sindyCImport(false)
{
}

// デストラクタ
CReflect::~CReflect(void)
{
	g_MeshPolyMap.clear();
}

// 初期化関数（中断しても、処理開始前に必ず呼ばれる）
bool CReflect::init(LPCTSTR cConnectStr, const CArguments& args)
{
	//接続
	m_ipWorkspace = CReflect::connectSDE( cConnectStr );
	if( !m_ipWorkspace )
		return false;

	if( 0 != g_Suffix.CompareNoCase( args.m_suffix.c_str() ) )
	{
		//簡易的に初回かどうかの判定
		//ここは初回のみ
		CString oldPath = args.m_oldPath.c_str();
		CString newPath = args.m_newPath.c_str();
		if(_taccess(oldPath, 0x04) == 0)
		{
			m_OldPath = oldPath;
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR 編集前データのパスが存在しないかアクセスできません。,%s\n"), oldPath);
			_tprintf( _T("ERROR 編集前データのパスが存在しないかアクセスできません。,%s\n"), oldPath);
			return false;
		}
		if(_taccess(newPath, 0x04) == 0)
		{
			m_NewPath = newPath;
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR 編集後データのパスが存在しないかアクセスできません。,%s\n"), newPath);
			_tprintf( _T("ERROR 編集後データのパスが存在しないかアクセスできません。,%s\n"), newPath);
			return false;
		}
		if( 0 == m_OldPath.CompareNoCase( m_NewPath ) )
		{
			_ftprintf(stderr, _T("#ERROR 編集前のデータパスと編集後データのパスが同じです。,%s,%s\n"), oldPath, newPath);
			_tprintf(_T("ERROR 編集前のデータパスと編集後データのパスが同じです。,%s,%s\n"), oldPath, newPath);
			return false;
		}
		if(!fnFirstOnlyInit(args))
		{
			fnUnInit();
			return false;
		}
	}
	//2回目以降（中断からの復帰）の場合は開き直し
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
		if( !m_ipFeatureClasses[i].m_T )
		{
			if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass(CComBSTR(table), &m_ipFeatureClasses[i].m_T)))
			{
				_ftprintf(stderr, _T("#ERROR フィーチャークラスが開けません。,%s\n"), table);
				_tprintf(_T("ERROR フィーチャークラスが開けません。,%s\n"), table);
				return false;
			}
		}
	}
	//最初だけでいいかもしれないけど、怖いから毎接続毎に取得
	IGeoDatasetPtr ipDataset = m_ipFeatureClasses[0].m_T;
	if(FAILED(ipDataset->get_SpatialReference(&m_ipSpRef)))
	{
		_ftprintf(stderr, _T("#ERROR,SpatialReference取得失敗。,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[0]);
		return false;
	}
	return true;
}

// SDE接続関数
IWorkspacePtr CReflect::connectSDE(LPCTSTR lpszConnectStr)
{
	// 接続
	_tprintf(_T("%sに接続中..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
		_tprintf(_T("[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		_ftprintf(stderr, _T("[%s]に接続できなかったＹｏ\n") , lpszConnectStr);
		return NULL;
	}
	_tprintf(_T("接続しました\n"));
	_ftprintf(stderr, _T("#%sに接続\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

// 環境変数取得関数
void CReflect::fnGetEnv(void)
{
	//各フィーチャクラス名
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		CString aTmp = _T("FC_") + CString(gEnvTable[g_DataIndex].m_OrgTableNames[i]);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			gEnvTable[g_DataIndex].m_TgtTableNames[i] = aEnv;
		}
		_ftprintf(stderr, _T("#変更後設定：%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
	}

	LPCTSTR aEnv = _tgetenv(_T("FC_CITYMESH"));
	if(aEnv != NULL){
		gMeshPoly = aEnv;
	}
}

// 最初の1回のみ実行すればよい初期化関数
bool CReflect::fnFirstOnlyInit(const CArguments& args)
{
	CString suffix = args.m_suffix.c_str();
	//有効な拡張子か判定
	for(int i = 0; gEnvTable[i].m_Suffix != NULL; ++i)
	{
		if( 0 == suffix.CompareNoCase( gEnvTable[i].m_Suffix ) )
		{
			g_DataIndex = i;
			break;
		}
	}
	if( g_DataIndex < 0 )
	{
		CString aSuffList = gEnvTable[0].m_Suffix;
		for(int i = 1; gEnvTable[i].m_Suffix != NULL; ++i)
		{
			aSuffList += _T(",");
			aSuffList += gEnvTable[i].m_Suffix;
		}
		_tprintf(_T("#ERROR,拡張子には %s のいずれかを指定して下さい。\n"), static_cast<LPCTSTR>(aSuffList));
		_ftprintf(stderr, _T("#ERROR,拡張子には %s のいずれかを指定して下さい。\n"), static_cast<LPCTSTR>(aSuffList));
		return false;
	}
#ifdef _DEBUG
	g_Operator = _T("for_debug");
#else
	LPCTSTR aOperator = _tgetenv(_T("OPERATOR"));
	if(aOperator != NULL){
		g_Operator = aOperator;
	}else{
		_ftprintf(stderr, _T("#環境変数「OPERATOR」がセットされていません。\n"));
		_tprintf(_T("#環境変数「OPERATOR」がセットされていません。\n"));
		return false;
	}
#endif

	g_Suffix = suffix;
	fnGetEnv();

	//メッシュリスト読み込み
	if(!fnMeshLoad(args)){
		return false;
	}

	IFeatureWorkspacePtr ipFeatureWorkspace(m_ipWorkspace);
	ATLASSERT( ipFeatureWorkspace != NULL );
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		if(FAILED(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(gEnvTable[g_DataIndex].m_TgtTableNames[i]),	&m_ipFeatureClasses[i].m_T)))
		{
			_ftprintf(stderr, _T("#ERROR フィーチャークラスが開けません。,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
			_tprintf(_T("ERROR フィーチャークラスが開けません。,%s\n"), gEnvTable[g_DataIndex].m_TgtTableNames[i]);
			return false;
		}
		//強制モードでない時はバージョン対応であることが必要
		if(!args.m_forceMode && !args.m_testMode)
		{
			//バージョン対応かチェック
			IVersionedObjectPtr ipVersionedObject = m_ipFeatureClasses[i].m_T;
			if(ipVersionedObject)
			{
				VARIANT_BOOL aIsVersioned = VARIANT_FALSE;
				ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
				if(!aIsVersioned)
				{
					_tprintf(_T("ERROR,バージョン対応ではないため、更新できません\n"));
					_ftprintf(stderr, _T("#ERROR,バージョン対応ではないため、更新できません\n"));
					return false;
				}
			}
		}

		CComBSTR aLayerName;
		((IDatasetPtr)m_ipFeatureClasses[i].m_T)->get_Name(&aLayerName);
		g_LayerName[i] = aLayerName;
		for(int j = 0; gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName != NULL; ++j)
		{
			CString fieldName = gEnvTable[g_DataIndex].m_Fields[i][j].m_fieldName;
			m_ipFeatureClasses[i]->FindField(CComBSTR(fieldName), &gEnvTable[g_DataIndex].m_Fields[i][j].m_SindyIndex);
			if(gEnvTable[g_DataIndex].m_Fields[i][j].m_SindyIndex < 0)
			{
				_ftprintf(stderr, _T("#ERROR,フィールドが見つかりません。,%s\n"), fieldName);
				return false;
			}
		}
		if(!fnGetCommonFieldIndex(i))
		{
			return false;
		}
		m_ipFeatureClasses[i]->get_ShapeType(&g_ShapeType[i]);	//メッシュ境界の判定,マージする・しない等で必要（ポリゴンorライン）
	}
	// 号タイプ用特殊処理
	if( 0 == suffix.CompareNoCase( _T("gou") ) )
	{
		m_ipFeatureClasses[0]->FindField(CComBSTR(GOUTYPE_NAME), &g_GouTypeIndex);
		if(g_GouTypeIndex < 0)
		{
			_ftprintf(stderr, _T("#ERROR,フィールドが見つかりません。,%s\n"), GOUTYPE_NAME);
			return false;
		}
	}
	else
	{
		g_GouTypeIndex = -1;
	}
	g_NowProccess = 0;
	return true;
}

// 共通で使用するフィールドインデックス取得関数
bool CReflect::fnGetCommonFieldIndex(int cIndex)
{
	using namespace sindy::schema::ipc_table;
	using namespace sindy::schema::category::sindyc_table;

	// SiNDY-c用かどうか確認するのに1個試しに取ってみる
	// TODO:取れなかったらエラー出るがSiNDY-c用かどうか確定するまでエラー出さないようにしたい
	if( fnFindFieldIndex(cIndex, kOrgOperator, g_OrgOperatorIndex[cIndex]) )
	{
		// 取れたならFeatureClassはSiNDY-c用のはずなので、以下の4つも取っておく
		if( !fnFindFieldIndex(cIndex, kOrgModifyDate, g_OrgModifyIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kOrgOBJID, g_OrgObjIdIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kUpdateC, g_UpdateCIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kCheckC, g_CheckCIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, _T("MESHCODE"), g_MeshcodeIndex[cIndex]) )
			return false;

		m_sindyCImport = true;
	}
	else
	{
		// この2つはSiNDY-c用のFeatureClassにはいない
		if( !fnFindFieldIndex(cIndex, kModifyProgName, g_ProgNameIndex[cIndex]) )
			return false;

		if( !fnFindFieldIndex(cIndex, kProgModifyDate, g_ProgModifyDateIndex[cIndex]) )
			return false;

		m_sindyCImport = false;
	}

	// 以下はSiNDY用・SiNDY-c用どちらにもいるスキーマ
	if( !fnFindFieldIndex(cIndex, kOperator, g_OperatorIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kUpdateType, g_UpdateIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kPurpose, g_PurposeIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kUserClaim, g_ClaimIndex[cIndex]) )
		return false;
	
	if( !fnFindFieldIndex(cIndex, kModifyDate, g_ModifyDateIndex[cIndex]) )
		return false;

	if( !fnFindFieldIndex(cIndex, kSource, g_SourceIndex[cIndex]) )
		return false;

	return true;
}

/*
	編集差分反映作業メイン関数。次の（以前は個別だった）機能が組み込まれている。
	・差分取り込み
	・不要構成点削除
	・メッシュ境界マージ
	・マルチパートポリゴン分割
	・不要構成点削除
*/
bool CReflect::execReflect( const CArguments& args )
{
	//差分インポート
	if(g_NowProccess == 0){
		CImport aImport;
		aImport.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport);
		if(!aImport.executeALL(300)){
			return false;
		}
	}

	if( args.m_onlyImport )
	{
		// TODO:必要ならマルチパートポリゴン解消
		// UPDATE_C=4の扱いとかは要検討
		//CSeparatePolygon aSeparatePolygon;
		//if(aSeparatePolygon.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
		//	//ポリゴンがある時だけ実行
		//	if(!aSeparatePolygon.executeALL()){
		//		return false;
		//	}
		//}
		fnUnInit(); //全部破棄
		return g_IsContinue;
	}

	//不要構成点削除+メッシュ境界マージ
	if(g_NowProccess == 1){
		CMeshMerge aMeshMerge;
		if(aMeshMerge.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//ポリゴン、ポリラインがある時だけ実行
			if(!aMeshMerge.executeALL()){
				return false;
			}
		}
	}
	
	//マルチパートポリゴン解消
	if(g_NowProccess == 2){
		CSeparatePolygon aSeparatePolygon;
		if(aSeparatePolygon.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//ポリゴンがある時だけ実行
			if(!aSeparatePolygon.executeALL()){
				return false;
			}
		}
	}
	//不要構成点削除その２（境界不整合解消）
	if(g_NowProccess == 3){
		CDelSinglePoint aDelSinglePoint;
		if(aDelSinglePoint.init(args, m_ipWorkspace, m_ipSpRef, m_ipFeatureClasses, m_sindyCImport)){
			//ポリゴンがある時だけ実行
			if(!aDelSinglePoint.executeALL()){
				return false;
			}
		}
	}
	
	fnUnInit();	//全部破棄
	return g_IsContinue;
}

// コマンド付きメッシュリストの読み込みとメッシュポリゴン取得関数
bool CReflect::fnMeshLoad(const CArguments& args)
{
	CString meshList = args.m_meshPath.c_str();
	if(g_MeshMap.empty())
	{
		_tprintf( _T("メッシュリスト解析開始\n") );
		FILE *pFile;
		if((pFile = _tfopen(meshList, _T("rt"))) != NULL)
		{
			//メッシュポリゴン
			IFeatureClassPtr ipMesh;
			if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace)->OpenFeatureClass(CComBSTR(gMeshPoly), &ipMesh)))
			{
				_ftprintf(stderr, _T("#ERROR フィーチャークラスが開けません。,%s\n"), static_cast<LPCTSTR>(gMeshPoly));
				_tprintf(_T("ERROR フィーチャークラスが開けません。,%s\n"), static_cast<LPCTSTR>(gMeshPoly));
				fclose(pFile);
				return false;
			}
			char aBuf[16] = "";
			while(fgets(aBuf, 16, pFile) != NULL)
			{
				if(aBuf[0] == '#')
				{
					continue;
				}
				long aMesh = atol(aBuf);
				// メッシュごとにログファイル作成 (bug 10509)
				args.m_logFile.InitLogFile( aMesh );

				COMMAND aCommand = NONE;
				char *p = strchr(aBuf, ',');
				if(p != NULL)
				{
					p++;
					switch(*p)
					{
						case 'u':
						case 'U':
							//差分取り込み（当初想定していた通り）
							aCommand = DIFF_UPDATE;
							break;
						case 'd':
						case 'D':
							//削除のみ
							aCommand = ONLY_DELETE;
							break;
						case 'a':
						case 'A':
							//追加のみ
							aCommand = ONLY_ADD;
							break;
						case 'n':
						case 'N':
							//新規追加（削除も含む）
							aCommand = DELETE_ADD;
							break;
						default:
							fprintf(stderr, "#ERROR,不正なコマンド[%c],処理しません。%d\n", *p, aMesh);
							printf("ERROR,不正なコマンド[%c],処理しません。%d\n", *p, aMesh);
							break;
					}
				}
				else
				{
					_ftprintf(stderr, _T("#ERROR,コマンドがないため処理しません。,%d\n"), aMesh);
				}
				if(aCommand == NONE)
				{
					continue;
				}
				//メッシュポリゴンキープ
				IQueryFilterPtr ipFilter(CLSID_QueryFilter);
				CString aWhere;
				aWhere.Format(_T("MESHCODE = %d"), aMesh);
				ipFilter->put_WhereClause(CComBSTR(aWhere));
				IFeatureCursorPtr ipCursor;
				if(FAILED(ipMesh->Search(ipFilter, VARIANT_FALSE, &ipCursor)))
				{
					_ftprintf(stderr, _T("#ERROR,メッシュポリゴンの検索失敗。,%s\n"), static_cast<LPCTSTR>(aWhere));
					return false;
				}
				IFeaturePtr ipFeature;
				if(ipCursor->NextFeature(&ipFeature) != S_OK)
				{
					_ftprintf(stderr, _T("#ERROR,メッシュポリゴンが見つからない。,%d\n"), aMesh);
					return false;		
				}
				IGeometryPtr ipGeom;
				ipFeature->get_ShapeCopy(&ipGeom);
				if(ipGeom)
				{
					g_MeshPolyMap[aMesh] = ipGeom;
					g_MeshMap[aMesh] = aCommand;
				}
				else
				{
					_ftprintf(stderr, _T("#ERROR,メッシュポリゴンが取得できない。,%d\n"), aMesh);
				}
			}
			ipMesh = NULL;
			fclose(pFile);
		}
		else
		{
			_ftprintf(stderr, _T("#ERROR,リストファイルが開けません。,%s\n"), meshList);
			_tprintf(_T("ERROR,リストファイルが開けません。,%s\n"), meshList);
			return false;
		}
	}
	return true;
}

// 後処理関数（全部破棄してSDEから切断する）
void CReflect::fnUnInit(void)
{
	m_ipSpRef = nullptr;
	for( auto& fc : m_ipFeatureClasses )
	{
		// m_ipFeatureClasses.fill(nullptr); ってやりたいけどCAdaptあるから？できない
		fc = nullptr;
	}
	m_ipWorkspace = nullptr;
}

bool CReflect::fnFindFieldIndex(int cIndex, const CString& fieldName, long& fieldIndex)
{
	if(m_ipFeatureClasses[cIndex]->FindField(CComBSTR(fieldName), &fieldIndex) != S_OK){
		_ftprintf(stderr, _T("#ERROR,インデックス検索失敗。[%s]\n"), fieldName);
			return false;
	}
	if(fieldIndex < 0){
		_ftprintf(stderr, _T("#ERROR,フィールドが見つからない。[%s]\n"), fieldName);
			return false;
	}
	return true;
}
