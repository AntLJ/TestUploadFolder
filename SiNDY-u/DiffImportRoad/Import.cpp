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
#include <ArcHelperEx/GlobalFunctions.h>
#include "Import.h"

using namespace Reflect;
using namespace sindy::schema;

namespace {

	const long SHAPE_CHG = 0x01; //!< 形状に変更があった（ビットフラグ）
	const long ATTR_CHG  = 0x02; //!< 属性に変更があった（ビットフラグ）
	const long BOTH_CHG  = SHAPE_CHG | ATTR_CHG; //!< 形状と属性に変更があった（新規も含む）

	LPCTSTR CHG_STATUS_NAME     = _T("UPDATE_C");       //!< 更新ステータスのフィールド名
	LPCTSTR ORG_OBJID_NAME      = _T("ORG_OBJID");      //!< オリジナルのオブジェクトIDフィールド名
	LPCTSTR ORG_MODIFYDATE_NAME = _T("ORG_MODIFYDATE"); //!< オリジナルの最終更新日付フィールド名
	LPCTSTR ORG_OPERATOR_NAME   = _T("ORG_OPERATOR");   //!< オリジナルの編集者名

//	const double BUF_SIZE = 0.000001;	///< 同一とみなす時のバッファサイズ

	// 他のレイヤのIDを指すフィールド群
	LPCTSTR REL_ID_TABLE[] = {
		category::link_relational_table::kLinkID,
		stop_point::kNodeID,
		category::from_to_node_relational_table::kToNodeID,
		category::from_to_node_relational_table::kFromNodeID,
		NULL
	};
}

// コンストラクタ
CImport::CImport(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("差分インポート");
}

// デストラクタ
CImport::~CImport(void)
{
}


// 初期化関数
bool CImport::init(LPCTSTR cOldPath, LPCTSTR cNewPath)
{
	m_OldPath = cOldPath;
	m_NewPath = cNewPath;
	return true;
}

// メッシュ単位での差分インポート（反映）関数
bool CImport::fnExecMesh(void)
{
	m_IDSets.clear();
	m_IDMaps.clear();
	m_OldTableInfo.clear();


	_ftprintf(stderr, _T("#%d\n"), g_CurrentMesh);
	IWorkspacePtr ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
	if(ipOldWorkspace == NULL){
		return false;
	}
	IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
	if(ipNewWorkspace == NULL){
		return false;
	}
#ifdef STOP_POINT
	//道路一般フィーチャクラス(編集はしない→IDの関係取得だけ)
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, true)){
			return false;
		}
	}
	//その他（現状一時停止のみ）
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::EXT_FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::EXT_FC_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){
			return false;
		}
	}
#else
	if(!fnGetMesh()){
		return false;
	}
	//道路一般フィーチャクラス（編集あり）
	//先にノードのIDの関係を取得してから（リンクを先に処理したいから）
	EnvTable *aEnvNode = m_LayerManage->findEnvTable(road_node::kTableName);
	if(aEnvNode){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnvNode, true)){
			return false;
		}
	}
	EnvTable *aEnvLink = m_LayerManage->findEnvTable(road_link::kTableName);
	if(aEnvLink){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnvLink, true)){
			return false;
		}
	}
#ifdef _DEBUG
//#define LQTEST　//リンク列のみのテスト時には定義する
#endif
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
#ifdef LQTEST
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, true)){
#else
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){

#endif			return false;
		}
	}

	//リンク列
	for(LqEnvTable *aEnv = m_LayerManage->getFitstLQEnv(); ! aEnv->m_OrgTables[0].IsEmpty(); aEnv = m_LayerManage->getNextLQEnv()){
		if(!fnReflectLQ(ipOldWorkspace, ipNewWorkspace,  *aEnv)){
			return false;
		}
		// リンク列関連のフラグのケアが必要
		fnLinkFlagCare(*aEnv);
	}

	//リンク列以外の各種テーブル類
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::OTHER_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::OTHER_TABLE)){
		if(!fnReflectGeneral(ipOldWorkspace, ipNewWorkspace,  *aEnv, false)){
			return false;
		}
	}

#endif
	return true;
}

// レイヤ単位で変更を反映する関数
// 「cGetIDOnly」が「true」の時はIDの対応を取得するだけで反映はしない
bool CImport::fnReflectGeneral(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, EnvTable& cEnv, bool cGetIDOnly)
{
	//作業用フィーチャクラス「m_ipTable」にセット（毎回探すのも面倒なので）
	m_ipTable[0] =  m_LayerManage->findTable(cEnv.m_OrgTable);
	m_AppendID.clear();
	ITablePtr ipOldTable = fnOpenLocalTable(ipOldWorkspace, cEnv.m_OrgTable);
	if(!ipOldTable){
		return false;
	}

	ITablePtr ipNewTable = fnOpenLocalTable(ipNewWorkspace, cEnv.m_OrgTable);
	if(!ipNewTable){
		return false;
	}
	CComBSTR aLayerName;
	((IDatasetPtr)ipOldTable)->get_Name(&aLayerName);
	LayerInfo aLayerInfo;
	aLayerInfo.m_LayerName = aLayerName;
	aLayerInfo.m_ipTable = ipOldTable;
	m_OldTableInfo[cEnv.m_OrgTable] = aLayerInfo;

	//フィールドインデックスの取得
	CLayerManage::getFieldInfo(ipNewTable, cEnv.m_PGDBIndexMap);

	//通常差分
	if(cGetIDOnly){
		_tprintf(_T("%s,IDの対応を取得しています\n"), cEnv.m_OrgTable);
	}else{
		_tprintf(_T("%s,比較と反映をしています（変更・削除）\n"), cEnv.m_OrgTable);
	}
	if(!fnCompare(ipOldTable, ipNewTable, cEnv, cGetIDOnly)){	//比較と変更・削除（ポリゴンとそれ以外の処理は内部で分けている）
		return false;
	}

	if(!cGetIDOnly){
		_tprintf(_T("%s,新規分を追加してます\n"), cEnv.m_OrgTable);
		fnAddNew(ipNewTable, cEnv);	//新規追加
	}

	//念のため明示的に破棄
	ipNewTable = NULL;
	return true;
}

// PGDBオープン関数
IWorkspacePtr CImport::fnOpenLocalDatabase(LPCTSTR cBasePath) const
{
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBasePath,  g_CurrentMesh/100,  g_CurrentMesh); 

	// パーソナルジオDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// オープンしてみる
	ipWorkspaceFactory->OpenFromFile( CComBSTR( aDatabase ), 0, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		_ftprintf( stderr, _T("#ERROR,%s が開けませんでした\n"), aDatabase);
		return NULL;
	}
	return ipWorkspace;
}

// PGDB内の指定レイヤ名のテーブル（フィーチャクラス）オープン関数
ITablePtr CImport::fnOpenLocalTable(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName) const
{
	ITablePtr ipTable;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenTable(CComBSTR(cLayerName), &ipTable) != S_OK){
		_ftprintf(stderr, _T("#ERROR,テーブルが開けない,%s\n"), cLayerName);
		return NULL;
	}
	return ipTable;
}

// 比較と変更・削除関数
bool CImport::fnCompare(const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, EnvTable& cEnv, bool cGetIDOnly)
{
	//ポリゴンの時だけ少し処理を変えるため調べておく
	esriGeometryType aType = esriGeometryNull;
	IFeatureClassPtr ipFeatureClass = m_ipTable[0];
	if(ipFeatureClass){
		ipFeatureClass->get_ShapeType(&aType);
	}
	_ICursorPtr ipCursor;
#ifdef STOP_POINT
	//1メッシュしか出していないので、ファイル内全部が対象
	if(FAILED(ipOldTable->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
#else
	//暫定対策で隣接9メッシュ出しているので、自メッシュだけを対象とする
	//厳密には外から接するのも検索されてしまうが、運用では境界部はいじらないことになっているので、
	//多少多く扱うだけで実害はないはず
	IQueryFilterPtr ipFilter;
	if(aType != esriGeometryNull){
		//フィーチャクラスなら空間検索
		CComBSTR          bstrFieldName;
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), m_ipMeshPoly, bstrFieldName );
	}else{
		//メッシュコードを持ってたら自メッシュ、それ以外は全部
		if(cEnv.m_PGDBIndexMap.find(category::mesh_code_table::kMeshCode) != cEnv.m_PGDBIndexMap.end()){
			ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::mesh_code_table::kMeshCode, g_CurrentMesh);
		}
	}

	if(FAILED(ipOldTable->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
#endif
	ID_MAP aIDMap;	///< PGDBとSDEのOIDの関係（後で参照しているIDの振り替えで使用）
	CHG_ID aChgID;	///< 反映可・不可のID（自プログラム内で変更したもの、反映してはいけないものの判定で使用）
	ID_SET_MAP::iterator it = m_IDSets.find(cEnv.m_OrgTable);
	if(it != m_IDSets.end()){
		aChgID = it->second;
	}
	_IRowPtr ipOldRow;
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);
#ifdef FOR_BUG 
		m_ExistID.insert(aObjID);	//ORG_OBJIDが継承される問題用
#endif

		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[ORG_OBJID_NAME].m_Index, &aOrgID);
		aIDMap[aObjID] = aOrgID.lVal;	//PGDBのIDに対応するSDEのIDを保持
		if(cGetIDOnly){
			continue;
		}
		//処理済みのオブジェクトを元のIDで管理（途中でマルチパートになった時のためなので、ポリゴンの時しか意味ないはず）
		if(aChgID.m_OK.find(aObjID) != aChgID.m_OK.end()){
			continue;
		}

		//元データ単位で未処理だったら（メッシュ内）
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(ipNewRow){
			//変更をチェック
			CComVariant aUpdate;
			ipNewRow->get_Value(cEnv.m_PGDBIndexMap[CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
				{
					//他の関連オブジェクトの影響で変更不可になっているものはメッセージを表示して反映しない(実質リンクの影響でノードが変更できない場合だけのはず)
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("関連オブジェクトに処理対象外が含まれている（変更）"), road_link::kTableName, PGDB_LAYER);
					continue;
				}
				if(aType != esriGeometryPolygon && (fnMeshRelObject(ipNewRow) || fnMeshRelObject(ipOldRow))){
					//メッシュ境界に関係するものはメッセージを表示して反映しない(ポリゴンは切り貼りするから許可)
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("メッシュ境界又は範囲外のオブジェクトなので反映しない"), NULL, PGDB_LAYER);
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					continue;
				}
				//違いがあったら更新
				if(!fnSiNDYChange(cEnv, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("変更"), m_ipTable )){
					//SiNDY上で削除されているものは、この先は無駄
					//国内優先の時も次のオブジェクトへ
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					continue;
				}
				if(aType != esriGeometryPolygon || !fnOverMesh(cEnv, aOrgID.lVal)){
					//更新前オブジェクト（SiNDY上でチェック）がメッシュをまたいでいなかったら
					//境界上でも内側から接しているだけならOK
					//ポリゴンでなければOK
					if(fnReflectToSiNDY(ipNewRow, cEnv, aOrgID.lVal, aUpdate.lVal)){
						aChgID.m_OK.insert(aObjID);
					}else{
						aChgID.m_NG.insert(aObjID);
						fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					}
				}else{
					//元が複数メッシュにまたがるオブジェクトだったら切り貼り
					if(fnClipAndApend(ipNewTable, cEnv, aOrgID.lVal, ipNewRow, aUpdate.lVal)){
						aChgID.m_OK.insert(aObjID);
					}else{
						aChgID.m_NG.insert(aObjID);
						fnProhibitRelIDAll(ipNewRow, cEnv.m_PGDBIndexMap);
					}
				}
			}
		}else{
			// 【Bug9997】通常時は削除できないよう修正
#ifndef STOP_POINT
			// 通常時の削除は、削除失敗扱いとする（ただし、形状もちのみ）
			if( aType != esriGeometryNull ){
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				continue;
			}
#endif
			//削除された
			if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
			{
				//他の関連オブジェクトの影響で変更不可になっているものはメッセージを表示して反映しない(実質リンクの影響でノードが変更できない場合だけのはず)
				fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("関連オブジェクトに処理対象外が含まれている（削除）"), road_link::kTableName, TGT_LAYER);
				continue;
			}
			if(aType != esriGeometryPolygon && fnMeshRelObject(ipOldRow)){
				//メッシュ境界に関係するものはメッセージを表示して反映しない(ポリゴンは切り貼りするから許可だが、通常はありえない)
				fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("メッシュ境界又は範囲外のオブジェクトなので削除しない"), NULL, TGT_LAYER);
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				continue;
			}
			_IRowPtr ipRow;
			m_ipTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				if(fnSiNDYChange(cEnv, aOrgID.lVal, ipRow, ipOldRow, _T("削除"), m_ipTable )){
					if(aType != esriGeometryPolygon || !fnOverMesh(cEnv, aOrgID.lVal, ipRow)){
						//更新前オブジェクト（SiNDY上でチェック）がメッシュをまたいでいなかったら
						//境界上でも内側から接しているだけならOK
						//ポリゴンでなければOK
						if(!(g_Mode & NO_CHG_MODE)){
							if(FAILED(ipRow->Delete())){
								aChgID.m_NG.insert(aObjID);
								fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
								fnPrintComError();
								fnPrintLogStyre(2, cEnv.m_OrgTable, aOrgID.lVal, _T("削除失敗"), NULL, TGT_LAYER);
							}else{
								aChgID.m_OK.insert(aObjID);
								fnPrintLogStyre(0, cEnv.m_OrgTable, aOrgID.lVal, _T("削除"), NULL, FIX_LAYER);
							}
						}else{
							aChgID.m_OK.insert(aObjID);
							fnPrintLogStyre(0, cEnv.m_OrgTable, aOrgID.lVal, _T("削除される"), NULL, TGT_LAYER);
						}
					}else{
						//元が複数メッシュにまたがるオブジェクトだったら切り貼り
						if(fnClipAndApend(ipNewTable, cEnv, aOrgID.lVal, NULL, 0)){
							aChgID.m_OK.insert(aObjID);
						}else{
							aChgID.m_NG.insert(aObjID);
							fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
						}
					}
				}else{
					//削除してはいけない
					aChgID.m_NG.insert(aObjID);
					fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
				}
			}else{
				fnPrintLogStyre(1, cEnv.m_OrgTable, aOrgID.lVal, _T("既に削除されている"), NULL, FIX_LAYER);
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipOldRow, cEnv.m_PGDBIndexMap);
			}
		}
	}

	//ファイル内全部のID関係を取得する（必要がありそう？）
	if(FAILED(ipOldTable->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);
		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[ORG_OBJID_NAME].m_Index, &aOrgID);
		aIDMap[aObjID] = aOrgID.lVal;	//PGDBのIDに対応するSDEのIDを保持
	}

	m_IDMaps[cEnv.m_OrgTable] = aIDMap;
	m_IDSets[cEnv.m_OrgTable] = aChgID;
	return true;
}

// 新規追加分を一括追加関数
bool CImport::fnAddNew(const ITablePtr& ipTable, EnvTable& cEnv)
{
	//ポリゴンの時だけ少し処理を変えるため調べておく
	esriGeometryType aType = esriGeometryNull;
	IFeatureClassPtr ipFeatureClass = m_ipTable[0];
	if(ipFeatureClass){
		ipFeatureClass->get_ShapeType(&aType);
	}

	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s IS NULL OR %s = 0"), ORG_OBJID_NAME, ORG_OBJID_NAME);

	_ICursorPtr ipCursor;
	if(FAILED(ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	IRowBufferPtr ipBuffer;
	_ICursorPtr ipInsCursor;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[0]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[0]->CreateRowBuffer(&ipBuffer);
	}

	ID_MAP& aIDMap = m_IDMaps[cEnv.m_OrgTable];
	CHG_ID& aChgID = m_IDSets[cEnv.m_OrgTable];
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);

		// 【Bug9997】通常時は追加できないよう修正
#ifndef STOP_POINT
		// 追加失敗扱いとする（ただし、形状もちのみ）
		if( aType != esriGeometryNull ){	
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}
#endif

		//エラーメッセージにポイントを使うから、早めに
		IGeometryPtr ipGeom;
		IFeaturePtr ipFeature = ipRow;
		if(ipFeature){
			ipFeature->get_ShapeCopy(&ipGeom);
		}
#ifdef STOP_POINT
		//現状一時停止でのみ使用。同じノード・リンクの組があったら追加しない
		if(fnExistSamePoint(ipRow, cEnv)){
			continue;
		}
		//現状一時停止でのみ使用。同じノード・リンクの組があったら追加しない ここまで
#else
		// リンク内属性・規制系も、同一と見なせるものがあったら追加しない
		if(fnExistSameObj(ipRow, cEnv)){
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("同一とみなせるものが既に存在します"), NULL, PGDB_LAYER, ipGeom);
			continue;
		}
#endif		

		if(aChgID.m_NG.find(aObjID) != aChgID.m_NG.end())
		{
			//他の関連オブジェクトの影響で変更不可になっているものはメッセージを表示して反映しない(実質リンクの影響でノードが変更できない場合だけのはず)
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("関連オブジェクトに処理対象外が含まれている（追加）"), road_link::kTableName, PGDB_LAYER, ipGeom);
			continue;
		}

		if(fnMeshRelObject(ipRow)){
			//メッシュ境界に関係するものはメッセージを表示して反映しない
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("メッシュ境界又は範囲外のオブジェクトなので追加しない"), NULL, PGDB_LAYER, ipGeom);
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}
		//関連するリンクやノードの削除・形状変化も見る
		CString aRelLayer;
		if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap, cEnv.m_OrgTable, ipGeom, aRelLayer)){
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("関連オブジェクトに処理対象外が含まれている（追加）"), aRelLayer, PGDB_LAYER, ipGeom);
			aChgID.m_NG.insert(aObjID);
			fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
			continue;
		}

		if(!( g_Mode & NO_CHG_MODE)){
			if(ipFeature){
				((IFeatureBufferPtr)ipBuffer)->putref_Shape(ipGeom);
			}
			CString aDummy;
			fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, aDummy);
			fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
			CComVariant aID = 0;
			if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
				aIDMap[aObjID] = aID.lVal;
				aChgID.m_OK.insert(aObjID);
				fnPrintLogStyre(0, cEnv.m_OrgTable, aID.lVal, _T("追加"), NULL, TGT_LAYER, ipGeom);
			}else{
				aChgID.m_NG.insert(aObjID);
				fnProhibitRelIDAll(ipRow, cEnv.m_PGDBIndexMap);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("追加失敗"), NULL, PGDB_LAYER, ipGeom);
			}
		}else{
			//変更判定用にダミーIDをセット
			aIDMap[aObjID] = -1;
			aChgID.m_OK.insert(aObjID);
			fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("追加される"), NULL, PGDB_LAYER, ipGeom);
		}
	}

	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipInsCursor->Flush())){
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数（引数調整用）
// 関数名と戻り値が逆っぽい気もするが、更新できない・更新してはいけない時にfalseを返す
bool CImport::fnSiNDYChange(LqEnvTable& cEnv, long cTblType, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2])
{
	return fnSiNDYChange(cEnv.m_PGDBIndexMap[cTblType], cEnv.m_SDEIndexMap[cTblType], cEnv.m_OrgTables[cTblType], cObjID, ipOrgRow, ipPgdbRow, cChgType, ipTable[cTblType] );
}

// Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数（本体）
bool CImport::fnSiNDYChange(INDEX_MAP& cPgdbIndexMap, INDEX_MAP& cSdeIndexMap, const CString& cTableName, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr& ipTable )
{
	long aObjID = 0;
	ipPgdbRow->get_OID(&aObjID);
	//Export後に変更があったかチェック
	if(!ipOrgRow){
		_IRowPtr ipRow;
		ipTable->GetRow(cObjID, &ipRow);
		if(!ipRow){
			fnPrintLogStyre(2, cTableName, aObjID, _T("変更対象がSiNDY上で削除されている"), NULL, PGDB_LAYER);
			return false;
		}
		ipOrgRow = ipRow;
	}
	IGeometryPtr ipGeom;
	IFeaturePtr ipFeature = ipOrgRow;
	if(ipFeature){
		//フィーチャなら形状取得（エラーメッセージ用）
		ipFeature->get_Shape(&ipGeom);
	}
	CComVariant aSindyModDate;
	ipOrgRow->get_Value(cSdeIndexMap[ipc_table::kModifyDate].m_Index, &aSindyModDate);
	CComVariant aPgdbModDate;
	ipPgdbRow->get_Value(cPgdbIndexMap[ORG_MODIFYDATE_NAME].m_Index, &aPgdbModDate);
	//PGDBの更新日付がNULLじゃなかったら日付だけ比較、NULLだったら全属性比較（主にLQ対策）
	if((aPgdbModDate.vt != VT_NULL && aSindyModDate.date != aPgdbModDate.date) || (aPgdbModDate.vt == VT_NULL && fnIsAttrChange(ipOrgRow, cSdeIndexMap, ipPgdbRow, cPgdbIndexMap))){
		if( g_Mode & TDC_MODE){
			//国内優先だったら、次に行っちゃダメ
			CString aMsg = CString(_T("変更対象がSiNDY上で変更されている(")) + CString(cChgType) + _T(")");
			fnPrintLogStyre(2, cTableName, aObjID, aMsg, NULL, PGDB_LAYER);
			return false;
		}else{
			//通常はメッセージだけ
			fnPrintLogStyre(2, cTableName, aObjID, _T("変更対象がSiNDY上で変更されている"), NULL, PGDB_LAYER);
		}
	}

	//関連するリンクやノードの削除・形状変化も見る
	CString aRelLayer;
	if(!fnRelObjsChange(ipPgdbRow, cPgdbIndexMap, cTableName, ipGeom, aRelLayer)){
		fnPrintLogStyre(2, cTableName, aObjID, _T("関連オブジェクトに処理対象外が含まれている（変更）"), aRelLayer, PGDB_LAYER);
		return false;
	}

	// 道路リンクの場合、関連リンク列が更新可能か見る（リンクを更新した後、リンク列が更新できないケースの回避）
	if( cTableName == road_link::kTableName ){

		// PGDBのワークスペース取得
		IWorkspacePtr ipOldWorkspace = fnOpenLocalDatabase(m_OldPath);
		if(!ipOldWorkspace){
			return false;
		}
		IWorkspacePtr ipNewWorkspace = fnOpenLocalDatabase(m_NewPath);
		if(!ipNewWorkspace){
			return false;
		}

		// 全てのリンク列で実施
		for(LqEnvTable *env = m_LayerManage->getFitstLQEnv(); ! env->m_OrgTables[0].IsEmpty(); env = m_LayerManage->getNextLQEnv()){

			ITablePtr ipSdeTable[2];
			ITablePtr ipOldTable[2];
			ITablePtr ipNewTable[2];

			// Inf/Lqのテーブルを新旧それぞれで取得
			for(int i = 0; i < 2; ++i){
				ipSdeTable[i] = m_LayerManage->findTable(env->m_OrgTables[i]);
				ipOldTable[i] = fnOpenLocalTable(ipOldWorkspace, env->m_OrgTables[i]);
				if(!ipOldTable[i]){
					return false;
				}

				ipNewTable[i] = fnOpenLocalTable(ipNewWorkspace, env->m_OrgTables[i]);
				if(!ipNewTable[i]){
					return false;
				}

				//フィールドインデックスの取得
				CLayerManage::getFieldInfo(ipNewTable[i], env->m_PGDBIndexMap[i]);
			}

			// リンクのIDから、自身の関連Inf/Lqが更新可能か確認
			if( !fnCanRelInfLqUpdate( ipOldTable, ipNewTable, ipSdeTable, *env, aObjID )){
				fnPrintLogStyre(2, cTableName, aObjID, _T("関連リンク列が更新不可"), NULL, PGDB_LAYER);
				return false;
			}
		}
	}

	return true;
}

// Export後にSiNDY上に変更か加えられたか、更新日付2種の比較で判断する関数（引数調整用）
bool CImport::fnSiNDYChange(EnvTable& cEnv, long cObjID, _IRowPtr& ipOrgRow, const _IRowPtr& ipPgdbRow, LPCTSTR cChgType, const ITablePtr ipTable[2] )
{
	return fnSiNDYChange(cEnv.m_PGDBIndexMap, cEnv.m_SDEIndexMap, cEnv.m_OrgTable, cObjID, ipOrgRow, ipPgdbRow, cChgType, ipTable[0] );
}

// メッシュをまたいだオブジェクトかを判定する関数（ポリゴン用）
bool CImport::fnOverMesh(EnvTable& cEnv, long cObjID, IFeaturePtr ipFeature) const
{
	if(ipFeature == NULL){
		((IFeatureClassPtr)m_ipTable[0])->GetFeature(cObjID, &ipFeature);
		if(ipFeature == NULL){
			//fnSiNDYChange()で弾くからここには来ないはず
			fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("変更対象がSiNDY上で削除されている（出てはいけないメッセージ）"), NULL, FIX_LAYER);
			return false;
		}
	}
	IGeometryPtr ipGeom = fnGetClipShape(ipFeature, cEnv, cObjID);
	if(ipGeom == NULL){
		return false;
	}
	IPointCollectionPtr ipPoints(ipGeom);
	long aCount;
	ipPoints->get_PointCount(&aCount);

	if(aCount > 3){ //ポリゴンとして成立するためには4点以上必要
		return true;
	}
	return false;
}

// メッシュをまたぐオブジェクトを切り貼りして更新する関数
bool CImport::fnClipAndApend(const IFeatureClassPtr& ipNewClass, EnvTable& cEnv, long cObjID, const IFeaturePtr& ipNewFeature, long cStatus)
{
	long aPgdbID;	///< PGDBのID（ログ用）
	ipNewFeature->get_OID(&aPgdbID);

	//ここに来るのはポリゴンの時だけ
	IFeaturePtr ipFeature;
	if(FAILED(((IFeatureClassPtr)m_ipTable[0])->GetFeature(cObjID, &ipFeature))){
		fnPrintComError();
		return false;
	}
	if(!ipFeature){
		fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("フィーチャーが見つからない(出てはいけない)"), NULL, TGT_LAYER);
		return false;
	}

	//切り貼り処理
	IGeometryPtr ipGeom = fnGetRemakeShape(ipNewClass, ipFeature, cEnv, cObjID);
	if(ipGeom == NULL){
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("メッシュ境界切り貼り失敗"), NULL, TGT_LAYER);
		return false;
	}
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount;
	if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
		fnPrintComError();
		return false;
	}
	if(aCount > 1){
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			aCount = aPolUtil.Get_Part_Num();
			if(aCount > 1){
				cStatus = 0;	//属性の変更は反映させないため
				fnPrintLogStyre(1, cEnv.m_OrgTable, cObjID, _T("再構成後複数に分かれた"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("再構成後ポリゴンの分析失敗"), NULL, PGDB_LAYER);
			return false;
		}
	}else if(aCount < 1){
		if(ipNewFeature == NULL || cStatus & ATTR_CHG){
			//単純削除で可
			if(FAILED(ipFeature->Delete())){
				fnPrintComError();
				fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("メッシュ境界削除失敗"), NULL, PGDB_LAYER);
				return false;
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("メッシュ境界で削除"), NULL, TGT_LAYER);
				return true;
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("切り貼り後の形状が不正"), NULL, PGDB_LAYER);
			return false;
		}
	}
	ipFeature->putref_Shape(ipGeom);
	fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, ipNewFeature);
	if(!(g_Mode & NO_CHG_MODE)){
		if(FAILED(ipFeature->Store())){
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTable, aPgdbID, _T("メッシュ境界変更失敗"), NULL, PGDB_LAYER);
			return false;
		}
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("メッシュ境界変更"), NULL, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("メッシュ境界変更される"), NULL, TGT_LAYER);
	}
	return true;
}

// メッシュをまたいでいないものの単純更新関数
bool CImport::fnReflectToSiNDY(const _IRowPtr& ipNewRow, EnvTable& cEnv, long cObjID, long cStatus)
{
	_IRowPtr ipRow;
	if(FAILED(m_ipTable[0]->GetRow(cObjID, &ipRow))){
		fnPrintComError();
		return false;
	}
	if(!ipRow){
		fnPrintLogStyre(3, cEnv.m_OrgTable, cObjID, _T("フィーチャーが見つからない(出てはいけない)"), NULL, TGT_LAYER);
		return false;
	}
	
	CString aMsg;

	if(cStatus & SHAPE_CHG){
		// 【Bug9997】通常時は形状変更できないよう修正
#ifdef STOP_POINT
		//形状変更あり(バグってなければフィーチャのはず→と思ったら、新規の場合はここに来る)
		IFeaturePtr ipFeature = ipNewRow;
		if(ipFeature){
			IGeometryPtr ipGeom;
			ipFeature->get_ShapeCopy(&ipGeom);
			((IFeaturePtr)ipRow)->putref_Shape(ipGeom);
			aMsg = _T("【形状】");
		}
#else
		// 属性だけ反映したいところだけど、フラグがおかしくなったりするので、それも却下
		return false;
#endif
	}

	if(cStatus & ATTR_CHG){
		//属性変更あり
		fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, aMsg);
	}
	if(aMsg.IsEmpty()){
		aMsg = _T("【履歴系のみ】");
	}
	fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, ipNewRow);
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipRow->Store())){
			fnPrintComError();
			long aObjID;
			ipNewRow->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("変更失敗"), NULL, PGDB_LAYER);
			return false;		
		}
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("変更"), aMsg, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTable, cObjID, _T("変更される"), aMsg, TGT_LAYER);
	}
	return true;
}

// INF,LQ用更新関数
bool CImport::fnInfReflectToSiNDY(const _IRowPtr& ipNewRow, LqEnvTable& cEnv, long cObjID)
{
	long PgdbID;
	ipNewRow->get_OID(&PgdbID);
	//INF更新
	_IRowPtr ipRow;
	if(FAILED(m_ipTable[0]->GetRow(cObjID, &ipRow))){
		fnPrintComError();
		return false;
	}
	if(!ipRow){
		fnPrintLogStyre(3, cEnv.m_OrgTables[0], PgdbID, _T("ロウが見つからない"), NULL, PGDB_LAYER);
		return false;
	}
	
	CString aMsg;
	//属性変更あり
	fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], aMsg);

	fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], ipNewRow);
	if(!(g_Mode & NO_CHG_MODE)){
		if(FAILED(ipRow->Store())){
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTables[0], PgdbID, _T("変更失敗"), NULL, PGDB_LAYER);
			return false;		
		}
		fnPrintLogStyre(0, cEnv.m_OrgTables[0], cObjID, _T("変更"), aMsg, TGT_LAYER);
	}else{
		fnPrintLogStyre(0, cEnv.m_OrgTables[0], cObjID,  _T("変更される"), aMsg, TGT_LAYER);
	}
	return true;
}

// 反映対象属性のコピー(PGDB->SDE)関数
bool CImport::fnReflectAttr(_IRowPtr& ipTgtRow, const _IRowPtr& ipSrcRow, INDEX_MAP& cSDEIndex, INDEX_MAP& cPGDBIndex, CString& cField)
{
	bool aIsChange = false;	//実際に変更したか否か
	//個別属性
	for(INDEX_MAP::const_iterator it = cSDEIndex.begin(); it != cSDEIndex.end(); ++it){
		//最終更新日付は反映対象外、最終更新者は後で指定値で統一するので除外、INF_IDは別途更新するので除外
		//作業目的、更新内容（？）も対象外（制作要望）
		//文字列比較は、コンテナに入れる際に大文字にしているのと、スキーマが大文字統一のはずなので「==」でいいはず
		if(it->first == ipc_table::kOperator || it->first == ipc_table::kModifyDate || it->first == link_queue::kInfID
			|| it->first == ipc_table::kPurpose || it->first == ipc_table::kUpdateType){
			continue;
		}
		CComVariant aVal;
		ipSrcRow->get_Value(cPGDBIndex[it->first].m_Index, &aVal);
		if(it->first == category::link_relational_table::kLinkID){
			aVal.lVal = m_IDMaps[road_link::kTableName][aVal.lVal];
		}else if(it->first == category::from_to_node_relational_table::kToNodeID || it->first == category::from_to_node_relational_table::kFromNodeID || it->first == stop_point::kNodeID){
			aVal.lVal = m_IDMaps[road_node::kTableName][aVal.lVal];
		}
		CComVariant aTgtVal;
		ipTgtRow->get_Value(it->second.m_Index, &aTgtVal);
		if(aVal != aTgtVal){
			ipTgtRow->put_Value(it->second.m_Index, aVal);
			cField += _T("【");
			cField += it->second.m_Alias;
			cField += _T("】");
			aIsChange = true;
		}
	}
	return aIsChange;
}

// メッシュポリゴンでクリップした形状を求める関数
// fnGetRemakeShapeと似ているように見えるが、こちらは、メッシュポリゴンで切り取った残りのジオメトリを求める
IGeometryPtr CImport::fnGetClipShape(const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID, bool cForceMode) const
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	if(!cForceMode){
		//前のメッシュの影響でマルチでも切り取りが必要になることがあるから、
		//安易に省略できない　2008/02/01
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, cObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			if(aPolUtil.Get_Part_Num() > 1){
				//強引だけど、マルチパートなら間違いなくメッシュ境界のはず
				//元が問題なく、切り貼りの途中でのみマルチパートになる前提
				return ipGeom;
			}
		}else{
			//どうしようもない
			fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("判定時ポリゴンの分析失敗"), NULL, FIX_LAYER);
			return ipGeom;
		}
	}
	ITopologicalOperatorPtr ipTopo = ipGeom;
	IGeometryPtr ipTgtGeom;
	//元データをメッシュで切り取り
	if(FAILED(ipTopo->Difference(m_ipMeshPoly, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("切り取り失敗"), NULL, FIX_LAYER);
		return ipGeom;
	}
	ipTgtGeom->putref_SpatialReference( m_LayerManage->get_SpRef() );
	return ipTgtGeom;
}

// メッシュ境界のオブジェクトに対して、当該メッシュ外部分と当該メッシュ内の切り貼りを行う関数
// マルチパートになることもあり、オリジナルIDが同じものを1つにまとめようとするが、属性が変更になっているものがあれば新規追加扱いとする
IGeometryPtr CImport::fnGetRemakeShape(const IFeatureClassPtr& ipNewClass, const IFeaturePtr& ipFeature, EnvTable& cEnv, long cObjID)
{
	IGeometryPtr ipGeom;
	ipFeature->get_ShapeCopy(&ipGeom);

	ITopologicalOperatorPtr ipTopo = ipGeom;
	//対象データをメッシュで切り取り（隣接9メッシュ出しているので、自分も切らないといけない）
	IGeometryPtr ipTgtGeom;
	if(FAILED(ipTopo->Intersect(m_ipMeshPoly, esriGeometry2Dimension, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("切り取り失敗"), NULL, TGT_LAYER);
		return NULL;
	}
	ipTopo = ipTgtGeom;

	//元データをメッシュで切り取り
	if(FAILED(ipTopo->Difference(m_ipMeshPoly, &ipTgtGeom))){
		fnPrintComError();
		fnPrintLogStyre(2, cEnv.m_OrgTable, cObjID, _T("切り取り失敗"), NULL, TGT_LAYER);
		return NULL;
	}
	ipTgtGeom->putref_SpatialReference( m_LayerManage->get_SpRef() );

	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s = %d"), ORG_OBJID_NAME, cObjID);

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipNewClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return NULL;
	}
	bool aNeedWarning = false;
	IFeaturePtr ipSrcFeature;
	while(ipCursor->NextFeature(&ipSrcFeature) == S_OK && ipSrcFeature){
		ipTopo = ipTgtGeom;
		CComVariant aUpdate;
		ipSrcFeature->get_Value(cEnv.m_PGDBIndexMap[CHG_STATUS_NAME].m_Index, &aUpdate);
		if(aUpdate.lVal & ATTR_CHG){
			//属性変更があったやつは新規扱い（道路作業ではあってなならないけど）
			long aObjID;
			ipSrcFeature->get_OID(&aObjID);
			m_AppendID.insert(aObjID);
			continue;
		}

		IGeometryPtr ipSrcGeom, ipTgtGeom2;
		ipSrcFeature->get_Shape(&ipSrcGeom);
		ipTopo->Union(ipSrcGeom, &ipTgtGeom2);
		ipTgtGeom = ipTgtGeom2;
	}
	return ipTgtGeom;
}

// 同じ位置又は同じリンクID,ノードIDを持つものが既に存在するかチェックする関数
bool CImport::fnExistSamePoint(const IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	{
		//位置的に同じにあるかチェック
		CComBSTR          bstrFieldName;
		((IFeatureClassPtr)m_ipTable[0])->get_ShapeFieldName( &bstrFieldName );
		ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName );
		IFeatureCursorPtr ipCursor;
		if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fnPrintComError();
			return false;
		}
		IFeaturePtr ipTgtFeature;
		ipCursor->NextFeature(&ipTgtFeature);
		if(ipTgtFeature){
			long aObjID;
			ipTgtFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("同じ位置にポイントが存在"), NULL, TGT_LAYER, ipGeom);
			return true;
		}
	}
	{
		//同じリンクID,ノードIDを持つものをチェック
		CComVariant aVal;
		ipFeature->get_Value(cEnv.m_PGDBIndexMap[category::link_relational_table::kLinkID].m_Index, &aVal);
		long aOrgLinkID = m_IDMaps[road_link::kTableName][aVal.lVal];

		ipFeature->get_Value(cEnv.m_PGDBIndexMap[stop_point::kNodeID].m_Index, &aVal);
		long aOrgNodeID = m_IDMaps[road_node::kTableName][aVal.lVal];

		IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("LINK_ID = %ld AND NODE_ID = %ld"), aOrgLinkID, aOrgNodeID);

		IFeatureCursorPtr ipCursor;
		if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fnPrintComError();
			return false;
		}
		IFeaturePtr ipTgtFeature;
		ipCursor->NextFeature(&ipTgtFeature);
		if(ipTgtFeature){
			CString aMsg;
			aMsg.Format(_T("同じリンクID(%ld)・ノードID(%ld)を持つポイントが存在"), aOrgLinkID, aOrgNodeID);
			long aObjID;
			ipTgtFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, aMsg, NULL, TGT_LAYER, ipGeom);
			return true;
		}
	}
	return false;
}

// 他レイヤのIDを示すフィールドについて、その示しているオブジェクトの形状の変化を調べる関数(一時停止)。
// 他レイヤのIDを示すフィールドについて、その示しているオブジェクトの変化を調べる関数(一時停止以外)。
bool CImport::fnRelObjChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const CString& cFieldName, const IGeometryPtr& ipGeom, CString& cRelLayer)
{
	INDEX_MAP::const_iterator it = cPgdbIndexMap.find(cFieldName);
	if(it == cPgdbIndexMap.end()){
		//指定フィールドは持っていないから関係ない
		return true;
	}
	//ローカルでのIDを取得
	CComVariant aVal;
	ipRow->get_Value(it->second.m_Index, &aVal);
	
	if(cFieldName == category::link_relational_table::kLinkID){
		cRelLayer = road_link::kTableName;
	}else if(cFieldName == category::from_to_node_relational_table::kToNodeID || cFieldName == category::from_to_node_relational_table::kFromNodeID || cFieldName == stop_point::kNodeID){
		cRelLayer = road_node::kTableName;
	}
	if(cRelLayer.IsEmpty()){
		//呼ぶ側がおかしいはずだけど
		fnPrintLogStyre(3, cTableName, -1, _T("バグのはず（出てはいけない）"), NULL, TGT_LAYER);
		return false;
	}
	CHG_ID& aChgID = m_IDSets[cRelLayer];

#ifndef STOP_POINT
	//このプログラム内での更新によるものなら許可しないと整合が取れない
	if(aChgID.m_OK.find(aVal.lVal) != aChgID.m_OK.end()){
		return true;
	}
	//既に反映不可のものを指していたら、NG
	if(aChgID.m_NG.find(aVal.lVal) != aChgID.m_NG.end()){
		return false;
	}
#endif
	long aPgdbID;
	ipRow->get_OID(&aPgdbID);
	//SDE上でのIDに変換
	long aOrgID = m_IDMaps[cRelLayer][aVal.lVal];

	IFeaturePtr ipSdeFeature;
	IFeatureClassPtr ipSdeClass =  m_LayerManage->findTable(cRelLayer);
	ipSdeClass->GetFeature(aOrgID, &ipSdeFeature);
	if(!ipSdeFeature){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("参照ノード・リンクが見つからない"), NULL, PGDB_LAYER, ipGeom);
		return false;
	}

	IFeaturePtr ipPgdbFeature;
	IFeatureClassPtr ipPgdbClass = m_OldTableInfo[cRelLayer].m_ipTable;
	ipPgdbClass->GetFeature(aVal.lVal, &ipPgdbFeature);
	if(!ipPgdbFeature){
		fnPrintLogStyre(3, cTableName, aPgdbID, _T("バグのはず（出てはいけない）"), NULL, PGDB_LAYER);
		return false;
	}

	if(fnMeshRelObject(ipSdeFeature) || fnMeshRelObject(ipPgdbFeature)){
		//メッシュ境界に関係するものはメッセージを表示して反映しない
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("関連オブジェクトがメッシュ境界又は範囲外のオブジェクトなので反映しない"), NULL, PGDB_LAYER);
		return false;
	}

#ifndef STOP_POINT
	//関連リンク、ノードの種別もチェック
	EnvTable *pEnv = m_LayerManage->findEnvTable(cRelLayer);
	if(!pEnv){
		fnPrintLogStyre(3, cTableName, aPgdbID, _T("バグのはず（出てはいけない）"), NULL, PGDB_LAYER);
		return false;
	}
	CComVariant aSindyModDate;
	ipSdeFeature->get_Value(pEnv->m_SDEIndexMap[ipc_table::kModifyDate].m_Index, &aSindyModDate);
	CComVariant aPgdbModDate;
	ipPgdbFeature->get_Value(pEnv->m_PGDBIndexMap[ORG_MODIFYDATE_NAME].m_Index, &aPgdbModDate);
	//PGDBの更新日付がNULLじゃなかったら日付だけ比較、NULLだったら全属性比較（主にLQ対策）
	if((aPgdbModDate.vt != VT_NULL && aSindyModDate.date != aPgdbModDate.date) || (aPgdbModDate.vt == VT_NULL && fnIsAttrChange(ipSdeFeature, pEnv->m_SDEIndexMap, ipPgdbFeature, pEnv->m_PGDBIndexMap))){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("参照ノード・リンク属性が変更されている"), NULL, PGDB_LAYER, ipGeom);
		return false;	
	}
#endif
	if(!fnShapeComp(ipSdeFeature, ipPgdbFeature)){
		fnPrintLogStyre(2, cRelLayer, aPgdbID, _T("参照ノード・リンク形状が変更されている"), NULL, PGDB_LAYER, ipGeom);
		return false;
	}
	return true;
}

// 他レイヤのIDを示すフィールドについて、その示しているオブジェクトの変化を調べる関数
bool CImport::fnRelObjsChange(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cTableName, const IGeometryPtr& ipGeom, CString& cRelLayer)
{
	for(int i = 0; REL_ID_TABLE[i] != NULL; ++i){
		if(!fnRelObjChange(ipRow, cPgdbIndexMap, cTableName, REL_ID_TABLE[i], ipGeom, cRelLayer)){
			return false;
		}
	}
	return true;
}

// ２つのフィーチャについて、形状の一致を調べる関数
bool CImport::fnShapeComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const
{
	IGeometryPtr ipGeom1;
	ipFeature1->get_Shape(&ipGeom1);

	IGeometryPtr ipGeom2;
	ipFeature2->get_Shape(&ipGeom2);

	if(!ipGeom1 || !ipGeom2){
		fnPrintLogStyre(3, _T("レイヤ名"), -1, _T("NULL Shape（出てはいけないはず）"), NULL, PGDB_LAYER);
		return false;
	}

	VARIANT_BOOL aEqual = false;
	/* 参考 
	点列の回転とか、許容値とか（？）も考慮して、形状としては同じと言うことにしたい場合は下の通り
	ここでは、編集されていないことを確認したいので、厳密な比較を選択
	if(((IRelationalOperatorPtr)ipGeom1)->Equals(ipGeom2, &aEqual) == S_OK && !aEqual) {	// 甘い比較
	*/
	//「IsEqual」は、形状が違う時に「S_FALSE」を返すようで、「==S_OK」とすると正しい結果にならない
	if(SUCCEEDED(((IClonePtr)ipGeom1)->IsEqual((IClonePtr)ipGeom2, &aEqual)) && !aEqual){	//厳密な比較
		//形状が違うなら（変更があったら）false
		return false;
	}
	return true;
}

// INF・LQのペア単位で変更を反映する関数
bool CImport::fnReflectLQ(const IWorkspacePtr& ipOldWorkspace, const IWorkspacePtr& ipNewWorkspace, LqEnvTable& cEnv)
{
	ITablePtr ipOldTable[2];
	ITablePtr ipNewTable[2];
	for(int i = 0; i < 2; ++i){
		m_ipTable[i] = m_LayerManage->findTable(cEnv.m_OrgTables[i]);
		ipOldTable[i] = fnOpenLocalTable(ipOldWorkspace, cEnv.m_OrgTables[i]);
		if(!ipOldTable[i]){
			return false;
		}

		ipNewTable[i] = fnOpenLocalTable(ipNewWorkspace, cEnv.m_OrgTables[i]);
		if(!ipNewTable[i]){
			return false;
		}

		//フィールドインデックスの取得
		CLayerManage::getFieldInfo(ipNewTable[i], cEnv.m_PGDBIndexMap[i]);
	}
	//INF単位で更新可能かを見ながら更新と削除
	_tprintf(_T("%s,更新と削除をしています\n"), cEnv.m_OrgTables[0]);
	if(!fnInfModAndDel(ipOldTable, ipNewTable, cEnv)){
		return false;
	}
	//INF単位で更新可能かを見ながら追加
	return fnInfAdd(ipOldTable, ipNewTable, cEnv);
}

// リンク列の新規追加関数
bool CImport::fnInfAdd(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv)
{
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, _T("%s IS NULL OR %s = 0"), ORG_OBJID_NAME, ORG_OBJID_NAME);

	_ICursorPtr ipCursor;
	if(FAILED(ipNewTable[0]->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	IRowBufferPtr ipBuffer;
	_ICursorPtr ipInsCursor;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[0]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[0]->CreateRowBuffer(&ipBuffer);
	}

	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		//関連するリンクやノードの削除・形状変化も見る
		CString aRelLayer;
		if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap[0], cEnv.m_OrgTables[0], NULL, aRelLayer)){
			fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("関連オブジェクトに処理対象外が含まれている（INF追加）"), aRelLayer, PGDB_LAYER);
			continue;
		}

		ID_SET aAdd, aMod, aDel;
		if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
			if(fnExistSameLQ(ipNewTable[1], ipRow, cEnv, m_ipTable[1] )){
				//同じと見なせるものがあったら追加しない（結構手が込んだ処理になるので、最終チェックの意味でこの位置）
				fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("同一とみなせるものが既に存在します"), NULL, PGDB_LAYER);
				continue;
			}
			CComVariant aID = 0;
			if(!( g_Mode & NO_CHG_MODE)){
				CString aDummy;
				fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], aDummy);
				fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap[0], cEnv.m_PGDBIndexMap[0], NULL);
				if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
					fnPrintLogStyre(0, cEnv.m_OrgTables[0], aID.lVal, _T("追加"), NULL, TGT_LAYER);
				}else{
					fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("追加失敗"), NULL, PGDB_LAYER);
					continue;	//エラーで戻った方がいいのかもしれない
				}
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTables[0], aObjID, _T("追加される"), NULL, PGDB_LAYER);
			}
			if(!fnAddLQs(aAdd, aID.lVal, ipNewTable[1], cEnv)){//更新しない時でもログが出るように、この位置
				return false;
			}
		}
	}
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(ipInsCursor->Flush())){
			fnPrintComError();
			return false;
		}
	}
	return true;
}

// リンク列の変更と削除関数
bool CImport::fnInfModAndDel(const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], LqEnvTable& cEnv)
{
	_ICursorPtr ipCursor;
	if(FAILED(ipOldTable[0]->Search(NULL, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}

	_IRowPtr ipOldRow;
	while(ipCursor->NextRow(&ipOldRow) == S_OK && ipOldRow){
		long aObjID;
		ipOldRow->get_OID(&aObjID);

		CComVariant aOrgID;
		ipOldRow->get_Value(cEnv.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow(aObjID, &ipNewRow);
		if(ipNewRow){
			//変更をチェック
			CComVariant aUpdate;
			ipNewRow->get_Value(cEnv.m_PGDBIndexMap[0][CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				//違いがあったら更新
				if(!fnSiNDYChange(cEnv, 0, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("変更"), m_ipTable )){
					//SiNDY上で削除されているものは、この先は無駄
					//国内優先の時も次のオブジェクトへ
					continue;
				}
				//Inf自体は更新可能だが、LQも見ないとわからない
				ID_SET aAdd, aMod, aDel;
				if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
					//fnPrintLogStyre(0, cEnv.m_TgtTables[0], aOrgID.lVal, _T("INF更新？"));
					fnInfReflectToSiNDY(ipNewRow, cEnv, aOrgID.lVal);
					if(!fnDeleteLQs(aDel, cEnv.m_OrgTables[1])){
						return false;
					}
					if(!fnModLQs(aMod, ipNewTable[1], cEnv)){
						return false;
					}
					if(!fnAddLQs(aAdd, aOrgID.lVal, ipNewTable[1], cEnv)){
						return false;
					}
				}
			}
		}else{
			//削除された
			_IRowPtr ipRow;
			m_ipTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				if(fnSiNDYChange(cEnv, 0, aOrgID.lVal, ipRow, ipOldRow, _T("削除"), m_ipTable )){
					//Inf自体は削除可能だが、LQも見ないとわからない
					ID_SET aAdd, aMod, aDel;
					if(fnCanRefrectLq(aObjID, ipOldTable[1], ipNewTable[1], m_ipTable, cEnv, aAdd, aMod, aDel)){
						//INF,LQ全削除
						if(!(g_Mode & NO_CHG_MODE)){
							if(FAILED(ipRow->Delete())){
								fnPrintComError();
								fnPrintLogStyre(2, cEnv.m_OrgTables[0], aOrgID.lVal, _T("削除失敗"), NULL, TGT_LAYER);
							}else{
								fnPrintLogStyre(0, cEnv.m_OrgTables[0], aOrgID.lVal, _T("削除"), NULL, FIX_LAYER);
							}
						}else{
							fnPrintLogStyre(0, cEnv.m_OrgTables[0], aOrgID.lVal, _T("削除される"), NULL, PGDB_LAYER);
						}
						if(!fnDeleteLQs(aDel, cEnv.m_OrgTables[1])){
							return false;
						}
					}
				}
			}else{
				fnPrintLogStyre(1, cEnv.m_OrgTables[0], aOrgID.lVal, _T("既に削除されている"), NULL, FIX_LAYER);
			}
		}
	}
	return true;
}

// LQの変更状態を見ながら変更可能かを調べる関数
bool CImport::fnCanRefrectLq(long cInfID, const ITablePtr& ipOldTable, const ITablePtr& ipNewTable, const ITablePtr ipSdeTable[2], LqEnvTable& cEnv, ID_SET& cAdd, ID_SET& cMod, ID_SET& cDel)
{
	//指定INF_IDを持つLQの新旧を比較して、追加・更新・削除を抽出
	//関連リンクの変化の有無もチェック
	//LQが削除されるケースについても、関連リンクはチェック対象（道路制作Gに確認した結果）

	//「新」にあるものからチェック（削除以外）
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), link_queue::kInfID, cInfID);

	_ICursorPtr ipCursor;
	ipNewTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		_IRowPtr ipOldRow;
		ipOldTable->GetRow(aObjID, &ipOldRow);
		if(ipOldRow){
			CComVariant aOrgID;
			ipOldRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
			//変更ありと変更なし（追加・削除以外）
			if(!fnSiNDYChange(cEnv, 1, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("LQ変更"), ipSdeTable )){
				return false;
			}
			if(fnIsAttrChange(ipRow, cEnv.m_PGDBIndexMap[1], ipOldRow, cEnv.m_PGDBIndexMap[1])){
				cMod.insert(aObjID);
			}
		}else{
			//追加
			//LQ自体は比較できないが、関連するリンクやノードの削除・形状変化を見る
			cAdd.insert(aObjID);
			CString aRelLayer;
			if(!fnRelObjsChange(ipRow, cEnv.m_PGDBIndexMap[1], cEnv.m_OrgTables[1], NULL, aRelLayer)){
				fnPrintLogStyre(2, cEnv.m_OrgTables[0], aObjID, _T("関連オブジェクトに処理対象外が含まれている（LQ追加）"), aRelLayer, PGDB_LAYER);
				return false;
			}
		}
	}

	//「旧」にしかないものをチェック
	ipOldTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		long aObjID;
		ipRow->get_OID(&aObjID);
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(!ipNewRow){
			//削除
			CComVariant aOrgID;
			ipRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
			cDel.insert(aOrgID.lVal);	//削除だけはSDE上のIDを保持
			if(!fnSiNDYChange(cEnv, 1, aOrgID.lVal, (_IRowPtr)NULL, ipRow, _T("LQ変更"), ipSdeTable )){
				return false;
			}
		}
	}
	return true;
}

// 属性の変更の有無を調べる関数
// 単純な比較ではなく、見なくて良い（見てはいけない）所を除くなど、処理に都合の良い比較をする
// フィールド数が違う時は、Src側に少ない方をセットした方がいい
bool CImport::fnIsAttrChange(const _IRowPtr& ipSrcRow, const INDEX_MAP& cSrcIndex, const _IRowPtr& ipTgtRow, const INDEX_MAP& cTgtIndex)
{
	//比較フィールド数が同じと言うことは、PGDB同士（呼んでないけどSDE同士）なので、ほぼ単純比較
	//基本はSDEとPGDBの比較
	bool aNormalComp = true;
	if(cSrcIndex.size() == cTgtIndex.size()){
		//PGDB同士の時
		aNormalComp = false;
	}
	for(INDEX_MAP::const_iterator aSrc = cSrcIndex.begin(); aSrc != cSrcIndex.end(); ++aSrc){
		if(aSrc->first == ipc_table::kModifyDate){
			//最終更新日付は別に比較するので、飛ばした方が便利
			continue;
		}

		if(aNormalComp && (aSrc->first.CompareNoCase(ipc_table::kOperator) == 0
			|| aSrc->first.CompareNoCase(link_queue::kInfID) == 0)){
			//最終更新者はSDEとPGDBで違うようにしているので、比較対象外
			//INF_IDは対応が取れていないのと、ここだけ変わることはないはずなので対象外
			continue;
		}
		INDEX_MAP::const_iterator aTgt = cTgtIndex.find(aSrc->first);
		if(aTgt != cTgtIndex.end()){
			CComVariant aSrcVal;
			ipSrcRow->get_Value(aSrc->second.m_Index, &aSrcVal);
			CComVariant aTgtVal;
			ipTgtRow->get_Value(aTgt->second.m_Index, &aTgtVal);
			if(aNormalComp){
				//SDE上のIDに変換して比較
				if(aSrc->first == category::link_relational_table::kLinkID){
					aTgtVal.lVal = m_IDMaps[road_link::kTableName][aTgtVal.lVal];
				}else if(aSrc->first == category::from_to_node_relational_table::kToNodeID || aSrc->first == category::from_to_node_relational_table::kFromNodeID || aSrc->first == stop_point::kNodeID){
					aTgtVal.lVal = m_IDMaps[road_node::kTableName][aTgtVal.lVal];
				}
			}
			if(aSrcVal != aTgtVal){
				return true;
			}
		}
	}
	return false;
}

// 対象IDのLQを削除する関数
// cDelIDsにはSDE上のものが入る
bool CImport::fnDeleteLQs(const ID_SET& cDelIDs, LPCTSTR cLayerName)
{
	foreach(const long aObjID, cDelIDs){
		_IRowPtr ipRow;
		m_ipTable[1]->GetRow(aObjID, &ipRow);
		if(ipRow){
			if(!(g_Mode & NO_CHG_MODE)){
				if(FAILED(ipRow->Delete())){
					fnPrintComError();
					fnPrintLogStyre(2, cLayerName, aObjID, _T("削除失敗"), NULL, TGT_LAYER);
					return false;
				}else{
					fnPrintLogStyre(0, cLayerName, aObjID, _T("削除"), NULL, FIX_LAYER);
				}
			}else{
				fnPrintLogStyre(0, cLayerName, aObjID, _T("削除される"), NULL, TGT_LAYER);
			}
		}
	}
	return true;
}

// 対象IDのLQを追加する関数
// cAddIDsにはPGDB上のものが入る
bool CImport::fnAddLQs(const ID_SET& cAddIDs, long cInfID, const ITablePtr& ipNewTable, LqEnvTable& cEnv)
{
	_ICursorPtr ipInsCursor;
	IRowBufferPtr ipBuffer;
	if(!( g_Mode & NO_CHG_MODE)){
		if(FAILED(m_ipTable[1]->Insert(VARIANT_TRUE, &ipInsCursor))){
			fnPrintComError();
			return false;
		}
		m_ipTable[1]->CreateRowBuffer(&ipBuffer);
	}

	foreach(const long aObjID, cAddIDs){
		_IRowPtr ipRow;
		ipNewTable->GetRow(aObjID, &ipRow);
		if(!ipRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("ロウが見つからない(絶対バグ)"), NULL, PGDB_LAYER);
			return false;
		}

		if(!( g_Mode & NO_CHG_MODE)){
			CString aDummy;
			fnReflectAttr((_IRowPtr)ipBuffer, ipRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], aDummy);
			fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], NULL);
			ipBuffer->put_Value(cEnv.m_SDEIndexMap[1][link_queue::kInfID].m_Index, CComVariant(cInfID));	//INF_IDは付け替え
			CComVariant aID = 0;
			if(SUCCEEDED(ipInsCursor->InsertRow(ipBuffer, &aID))){
				long aObjID;
				ipRow->get_OID(&aObjID);
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aID.lVal, _T("追加"), NULL, TGT_LAYER);
			}else{
				fnPrintLogStyre(2, cEnv.m_OrgTables[1], aObjID, _T("追加失敗"), NULL, PGDB_LAYER);
			}
		}else{
			fnPrintLogStyre(0, cEnv.m_OrgTables[1], aObjID, _T("追加される"), NULL, PGDB_LAYER);
		}
	}
	return true;
}

// 対象IDのLQを変更する関数
// cModIDsにはPGDB上のものが入る
bool CImport::fnModLQs(const ID_SET& cModIDs, const ITablePtr& ipNewTable, LqEnvTable& cEnv)
{
	foreach(const long aObjID, cModIDs){
		_IRowPtr ipNewRow;
		ipNewTable->GetRow(aObjID, &ipNewRow);
		if(!ipNewRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("ロウが見つからない(絶対バグ)"), NULL, PGDB_LAYER);
			return false;
		}

		CComVariant aOrgID;
		ipNewRow->get_Value(cEnv.m_PGDBIndexMap[1][ORG_OBJID_NAME].m_Index, &aOrgID);
		_IRowPtr ipRow;
		if(FAILED(m_ipTable[1]->GetRow(aOrgID.lVal, &ipRow))){
			fnPrintComError();
			return false;
		}
		if(!ipRow){
			fnPrintLogStyre(3, cEnv.m_OrgTables[1], aObjID, _T("ロウが見つからない"), NULL, PGDB_LAYER);
			return false;
		}
	
		CString aMsg;
		//属性変更あり
		if(fnReflectAttr(ipRow, ipNewRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], aMsg)){
			fnSetCommonAttr(ipRow, cEnv.m_SDEIndexMap[1], cEnv.m_PGDBIndexMap[1], ipNewRow);
			if(!(g_Mode & NO_CHG_MODE)){
				if(FAILED(ipRow->Store())){
					fnPrintComError();
					fnPrintLogStyre(2, cEnv.m_OrgTables[1], aObjID, _T("変更失敗"), NULL, PGDB_LAYER);
					return false;		
				}
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aOrgID.lVal, _T("変更"), aMsg, TGT_LAYER);
			}else{
				fnPrintLogStyre(0, cEnv.m_OrgTables[1], aOrgID.lVal, _T("変更される"), aMsg, TGT_LAYER);
			}
		}
	}
	return true;
}

// メッシュ境界に関わる又はメッシュ範囲外かを判定する関数
bool CImport::fnMeshRelObject(const _IRowPtr& ipRow) const
{
	IFeaturePtr ipFeature = ipRow;
	if(ipFeature){
		IGeometryPtr ipGeom;
		ipFeature->get_Shape(&ipGeom);
#if 1
		IRelationalOperatorPtr ipRel(m_ipMeshBoundary);
		VARIANT_BOOL aDisjoint;
		//メッシュ枠と関わっていないこと
		if(SUCCEEDED(ipRel->Disjoint(ipGeom, &aDisjoint)) && aDisjoint){
			ipRel = m_ipMeshPoly;
			//メッシュポリゴンの中に納まっていること
			VARIANT_BOOL aContain;
			if(SUCCEEDED(ipRel->Contains(ipGeom, &aContain)) && aContain){
				return false;
			}
		}
		return true;
#else
		//メッシュポリゴンに完全内包してること
		IRelationalOperatorPtr ipRel(m_ipMeshPoly);
		VARIANT_BOOL aContain;
		if(SUCCEEDED(ipRel->Contains(ipGeom, &aContain)) && aContain){
			return false;
		}
		return true;
#endif
	}
	return false;
}

// 参照している他のレイヤを変更禁止リストに追加する関数（リンクID,ノードID等全部）
bool CImport::fnProhibitRelIDAll(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap)
{
	for(int i = 0; REL_ID_TABLE[i] != NULL; ++i){
		fnProhibitRelID(ipRow, cPgdbIndexMap, REL_ID_TABLE[i]);
	}
	return true;
}

// 参照している他のレイヤを変更禁止リストに追加する関数（フィールド名指定）
bool CImport::fnProhibitRelID(const _IRowPtr& ipRow, INDEX_MAP& cPgdbIndexMap, const CString& cFieldName)
{
	INDEX_MAP::const_iterator it = cPgdbIndexMap.find(cFieldName);
	if(it == cPgdbIndexMap.end()){
		//指定フィールドは持っていないから関係ない
		return true;
	}
	//ローカルでのIDを取得
	CComVariant aVal;
	ipRow->get_Value(it->second.m_Index, &aVal);
	
	CString aRelLayer;
	long aOrgID = -1;
	if(cFieldName == category::link_relational_table::kLinkID){
		aRelLayer = road_link::kTableName;
	}else if(cFieldName == category::from_to_node_relational_table::kToNodeID || cFieldName == category::from_to_node_relational_table::kFromNodeID || cFieldName == stop_point::kNodeID){
		aRelLayer = road_node::kTableName;
	}
	if(aRelLayer.IsEmpty()){
		//呼ぶ側がおかしいはずだけど
		return false;
	}
	m_IDSets[aRelLayer].m_NG.insert(aVal.lVal);
	return true;
}

// リンク列と関連フラグの整合を取る関数
bool CImport::fnLinkFlagCare(LqEnvTable& cEnv)
{
	CString aFlagField = cEnv.m_Tag + _T("_LQRF");	///< 関連フラグのフィールド名（こんなやり方でいいのか？）

	//道路リンクの情報取得
	CHG_ID& aChgID = m_IDSets[road_link::kTableName];	///< 道路リンクの変更可・不可情報
	IFeatureClassPtr ipClass = m_LayerManage->findTable(road_link::kTableName);	///< 道路リンクフィーチャクラス
	EnvTable *aLinkEnv = m_LayerManage->findEnvTable(road_link::kTableName);	///< 道路リンクの情報テーブル
	long aIndex = aLinkEnv->m_SDEIndexMap[aFlagField].m_Index;	///< 関連フラグのフィールドインデックス

	// 変更した、しようとしたものが対象でいいはず
	fnLinkFlagCareByID(ipClass, aIndex, aChgID.m_OK);	// こっちだけでいいかもしれないけど
	fnLinkFlagCareByID(ipClass, aIndex, aChgID.m_NG);	// 念のためこっちも
	return true;
}

// ID列指定でリンク列と関連フラグの整合を取る関数
bool CImport::fnLinkFlagCareByID(const IFeatureClassPtr& ipClass, long cFlagIndex, const ID_SET& cIDSet)
{
	foreach(const long aObjID, cIDSet){
		// SDE上のIDに変換
		long aSdeID = m_IDMaps[road_link::kTableName][aObjID];

		IFeaturePtr ipFeature;
		ipClass->GetFeature(aSdeID, &ipFeature);
		if(!ipFeature){
			//削除されたものもあるからスキップ
			continue;
		}
		CComVariant aVal;
		ipFeature->get_Value(cFlagIndex, &aVal);
		int aFlag = fnIsExistLQ(aSdeID);
		if(aVal.lVal != aFlag){
			if(!( g_Mode & NO_CHG_MODE)){
				ipFeature->put_Value(cFlagIndex, CComVariant(aFlag));
				ipFeature->Store();
				//_ftprintf(stderr, _T("INFO\tLINKのフラグ変更\t%d\n"), aObjID);
				fnPrintLogStyre(0, road_link::kTableName, aSdeID, _T("LINKのフラグ変更（後始末）"), NULL, TGT_LAYER);
			}else{
				// ここに来ることはないはず
				fnPrintLogStyre(0, road_link::kTableName, aSdeID, _T("LINKのフラグ要変更（後始末）"), NULL, TGT_LAYER);
			}
		}
	}
	return true;
}

// リンクのIDを元にリンク列で参照されているかを判定する関数
// 戻り値をそのまま関連フラグの値とするために数値で0/1を返す
int CImport::fnIsExistLQ(long cObjID) const
{
	//リンクIDで検索して、方向で振り分け
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::link_relational_table::kLinkID, cObjID);

	//フラグに向きは関係ない（有無だけ）
	long aCount;
	m_ipTable[1]->RowCount(ipFilter, &aCount);
	if(aCount > 0){
		return 1;
	}
	return 0;
}

// 同じとみなせるものが既に存在するかを調べる関数
bool CImport::fnExistSameObj(const _IRowPtr& ipRow, EnvTable& cEnv)
{
	//テーブル・フィーチャクラスによって、「同一」とみなす条件が違う
	//もうちょっと系統立ててやりたかったが、後付けになってしまったので・・
	if(cEnv.m_OrgTable != segment_attr::kTableName && cEnv.m_OrgTable != time_oneway::kTableName && cEnv.m_OrgTable != time_nopassage::kTableName){
		//リンク内属性・一通・通行禁止 以外ならスルー
		return false;
	}

	//リンクIDが同じで、他の属性（履歴等の共通属性を除いた、特有の属性）も一致するものがあるか
	CComVariant aLinkID;
	ipRow->get_Value(cEnv.m_PGDBIndexMap[category::link_relational_table::kLinkID].m_Index, &aLinkID);
	// SDE上のIDに変換
	aLinkID.lVal = m_IDMaps[road_link::kTableName][aLinkID.lVal];
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), category::link_relational_table::kLinkID, aLinkID.lVal);
	
	_ICursorPtr ipCursor;
	m_ipTable[0]->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		// ここに来てはいけないはずだが、反映禁止にするためにtrue（存在する）を返す
		return true;
	}
	_IRowPtr ipTgtRow;
	while(ipCursor->NextRow(&ipTgtRow) == S_OK && ipTgtRow){
		if(cEnv.m_OrgTable == segment_attr::kTableName){
			//リンク内属性なら、形状も比較（部分一致？接触判定でも）
			if(fnShapePartComp(ipRow, ipTgtRow)){
				return true;
			}
		}else{
			if(fnSameAttr(ipRow, ipTgtRow, cEnv)){
				return true;
			}
		}
	}
	return false;
}

// 固有属性（共通属性以外で）の差異を調べる関数
bool CImport::fnSameAttr(const _IRowPtr& ipPgdbRow, const _IRowPtr& ipSdeRow, EnvTable& cEnv)
{
	//うまく調整すれば「fnIsAttrChange(LQを意識して作成したもの)」と共通でも良さそう
	for(INDEX_MAP::const_iterator aSrc = cEnv.m_SDEIndexMap.begin(); aSrc != cEnv.m_SDEIndexMap.end(); ++aSrc){
		if(aSrc->first.CompareNoCase(ipc_table::kModifyDate) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kOperator) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kProgModifyDate) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kPurpose) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kSource) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kUpdateType) == 0
			|| aSrc->first.CompareNoCase(ipc_table::kModifyProgName) == 0){
			// 共通属性は対象外
			continue;
		}
		INDEX_MAP::const_iterator aTgt = cEnv.m_PGDBIndexMap.find(aSrc->first);
		if(aTgt != cEnv.m_PGDBIndexMap.end()){
			CComVariant aSrcVal;
			ipPgdbRow->get_Value(aSrc->second.m_Index, &aSrcVal);
			CComVariant aTgtVal;
			ipSdeRow->get_Value(aTgt->second.m_Index, &aTgtVal);
			//SDE上のIDに変換して比較
			if(aSrc->first == category::link_relational_table::kLinkID){
				aTgtVal.lVal = m_IDMaps[road_link::kTableName][aTgtVal.lVal];
			}else if(aSrc->first == category::from_to_node_relational_table::kToNodeID || aSrc->first == category::from_to_node_relational_table::kFromNodeID || aSrc->first == stop_point::kNodeID){
				aTgtVal.lVal = m_IDMaps[road_node::kTableName][aTgtVal.lVal];
			}
			if(aSrcVal != aTgtVal){
				return true;
			}
		}
	}
	return false;
}

bool CImport::fnShapePartComp(const IFeaturePtr& ipFeature1, const IFeaturePtr& ipFeature2) const
{
	IGeometryPtr ipGeom1;
	ipFeature1->get_Shape(&ipGeom1);

	IGeometryPtr ipGeom2;
	ipFeature2->get_Shape(&ipGeom2);

	if(!ipGeom1 || !ipGeom2){
		fnPrintLogStyre(3, _T("レイヤ名"), -1, _T("NULL Shape（出てはいけないはず）"), NULL, PGDB_LAYER);
		return false;
	}
#if 1
	// 2点以上同じ点があったら（本当は連続していることも見た方がいいんだろうけど）
	IPointCollectionPtr ipPointCol1 = ipGeom1;
	long aPointCount1 = 0;
	ipPointCol1->get_PointCount( &aPointCount1 );
	std::vector<WKSPoint> pPt1( aPointCount1 );
	ipPointCol1->QueryWKSPoints( 0, aPointCount1, &pPt1.at(0) );

	IPointCollectionPtr ipPointCol2 = ipGeom2;
	long aPointCount2 = 0;
	ipPointCol2->get_PointCount( &aPointCount2 );
	std::vector<WKSPoint> pPt2( aPointCount2 );
	ipPointCol2->QueryWKSPoints( 0, aPointCount2, &pPt2.at(0) );
	int aMachCount = 0;
	for(int i = 0; i < aPointCount1; ++i){
		for(int j = 0; j < aPointCount2; ++j){
			if(pPt1[i].X == pPt2[j].X && pPt1[i].Y == pPt2[j].Y){
				++aMachCount;
			}
			if(aMachCount > 1){
				return true;
			}
		}
	}
#else
	// どちらかに内包されていることの判定を期待したが、ダメっぽい
	VARIANT_BOOL aContain = VARIANT_FALSE;
	if(SUCCEEDED(((IRelationalOperatorPtr)ipGeom1)->Contains(ipGeom2, &aContain)) && aContain){
		return true;
	}else if(SUCCEEDED(((IRelationalOperatorPtr)ipGeom2)->Contains(ipGeom1, &aContain)) && aContain){
		return true;
	}
#endif
	return false;
}

bool CImport::fnExistSameLQ(const ITablePtr& ipLqTable, const _IRowPtr& ipNewRow, LqEnvTable &cEnv, const ITablePtr& ipSdeLqTable )
{
	// 自分のリンク列情報を取得
	long aObjID;
	ipNewRow->get_OID(&aObjID);
	LQ_MAP aLqMap;
	fnGetLqInfo(ipLqTable, aObjID, cEnv.m_PGDBIndexMap[1], true, aLqMap);

	// 始点が同じになるリンク列を列挙
	long aStartLinkID = aLqMap[1];
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d AND %s = 1"), link_queue::kLinkID, aStartLinkID, link_queue::kSequence);
	_ICursorPtr ipCursor;
	ipSdeLqTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}

	// 開始リンクが同じリンク列の「LQ」が列挙される
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		CComVariant aInfID;
		ipRow->get_Value(cEnv.m_SDEIndexMap[1][link_queue::kInfID].m_Index, &aInfID);

		LQ_MAP aSdeLqMap;
		fnGetLqInfo(ipSdeLqTable, aInfID.lVal, cEnv.m_SDEIndexMap[1], false, aSdeLqMap);
		// （これから追加しようとしている）自分と同じセット（並びとリンクID）の有無を調べる
		if(aLqMap == aSdeLqMap){
			return true;
		}
	}

	return false;
}

// INFのIDを元にLQが示すリンクIDを順番になるよう取得する関数
bool CImport::fnGetLqInfo(const ITablePtr& ipLqTable, long cInfID, INDEX_MAP& cLqIndexMap, bool cIsPGDB, LQ_MAP& cLqMap)
{
	IQueryFilterPtr ipFilter = AheInitQueryFilter(NULL, NULL, _T("%s = %d"), link_queue::kInfID, cInfID);

	_ICursorPtr ipCursor;
	ipLqTable->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(!ipCursor){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK && ipRow){
		CComVariant aSeq;
		ipRow->get_Value(cLqIndexMap[link_queue::kSequence].m_Index, &aSeq);
		CComVariant aLinkID;
		ipRow->get_Value(cLqIndexMap[link_queue::kLinkID].m_Index, &aLinkID);
		if(cIsPGDB){
			// SDE上のリンクIDに変換して格納
			cLqMap[aSeq.lVal] = m_IDMaps[road_link::kTableName][aLinkID.lVal];
		}else{
			// SDEだからそのまま格納
			cLqMap[aSeq.lVal] = aLinkID.lVal;
		}
	}
	return true;
}

// リンクのIDから、自身の関連Inf/Lqが更新可能か確認
bool CImport::fnCanRelInfLqUpdate( const ITablePtr ipOldTable[2], const ITablePtr ipNewTable[2], const ITablePtr ipSdeTable[2], LqEnvTable& env, long linkPgdbOID )
{
	// 「リンクのObjectID → LQ_*にそのIDを持つLQが指すINF_ID → そのINF_IDを持つINF_*のObjectID」 という形で関連INFのIDを取得する

	ID_SET    infModOIDs, infAddOIDs; //!< リンク自身の関連Inf格納用（変更・追加）

	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString strWhere;

	// LQ_*の自身のLINK_IDを持つもの
	strWhere.Format( _T("%s = %d"), category::link_relational_table::kLinkID, linkPgdbOID );
	ipFilter->put_WhereClause( _bstr_t(strWhere) );
		
	// 検索
	_IRowPtr    ipOldLqRow,    ipNewLqRow;
	_ICursorPtr ipOldLqCursor, ipNewLqCursor;
	ipOldTable[1]->Search( ipFilter, VARIANT_FALSE, &ipOldLqCursor );
	ipNewTable[1]->Search( ipFilter, VARIANT_FALSE, &ipNewLqCursor );

	// 更新前PGDBのLQから、関連InfのOIDリストを作成する（変更or削除対象確認用）
	while( S_OK == ipOldLqCursor->NextRow(&ipOldLqRow) && ipOldLqRow ){
		
		CComVariant vaValue;
		ipOldLqRow->get_Value( env.m_PGDBIndexMap[1][link_queue::kInfID].m_Index, &vaValue );

		// InfIDを変更リストに格納
		infModOIDs.insert( vaValue.lVal );
	}

	// 更新後PGDBのLQから、関連InfのOIDリストを作成する（追加対象確認用）
	while( S_OK == ipNewLqCursor->NextRow(&ipNewLqRow) && ipNewLqRow ){
		
		CComVariant vaValue;
		ipNewLqRow->get_Value( env.m_PGDBIndexMap[1][link_queue::kInfID].m_Index, &vaValue );

		// 新規追加分のみ、リストに格納
		if( infModOIDs.end() == infModOIDs.find( vaValue.lVal ) ){
			infAddOIDs.insert( vaValue.lVal );
		}
	}

	// 作成したInfのObjectIDリストから、infを検索し、更新可能か判断（変更or削除対象確認用）
	for( auto& infOID : infModOIDs ){
			
		_IRowPtr ipOldRow;
		ipOldTable[0]->GetRow( infOID, &ipOldRow );

		if( !ipOldRow ){
			continue;
		}

		CComVariant aOrgID;
		ipOldRow->get_Value(env.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);

		// 変更後PGDBから該当Infを取得
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow(infOID, &ipNewRow);
		if(ipNewRow){
			//変更をチェック
			CComVariant aUpdate;
			ipNewRow->get_Value(env.m_PGDBIndexMap[0][CHG_STATUS_NAME].m_Index, &aUpdate);
			if(aUpdate.vt != VT_NULL && aUpdate.lVal != 0){
				//違いがある場合
				if(!fnSiNDYChange(env, 0, aOrgID.lVal, (_IRowPtr)NULL, ipOldRow, _T("変更"), ipSdeTable )){
					//Infが変更不可の場合は、該当Linkも変更不可
					return false;
				}
				//Inf自体は更新可能だが、LQも見ないとわからない
				ID_SET aAdd, aMod, aDel;
				if( !fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel )){
					// Lqが変更不可の場合は、該当Linkも変更不可
					return false;
				}
			}
		}
		else{
			//削除された
			_IRowPtr ipRow;
			ipSdeTable[0]->GetRow(aOrgID.lVal, &ipRow);
			if(ipRow){
				// 削除対象のInfの状態を確認
				if(!fnSiNDYChange(env, 0, aOrgID.lVal, ipRow, ipOldRow, _T("削除"), ipSdeTable )){
					// Infが削除不可の場合は、該当Linkも変更不可
					return false;
				}
				//Inf自体は削除可能だが、LQも見ないとわからない
				ID_SET aAdd, aMod, aDel;
				if( !fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel )){
					// Lqが削除不可の場合は、該当Linkも変更不可
					return false;
				}
			}
		}
	}

	// 作成したInfのObjectIDリストから、infを検索し、更新可能か判断（追加対象確認用）
	for( auto& infOID : infAddOIDs ){
			
		_IRowPtr ipNewRow;
		ipNewTable[0]->GetRow( infOID, &ipNewRow );

		if( !ipNewRow ){
			continue;
		}

		CComVariant aOrgID;
		ipNewRow->get_Value(env.m_PGDBIndexMap[0][ORG_OBJID_NAME].m_Index, &aOrgID);
		
		// ORG_OBJIDが空以外は除外（新規オブジェクトじゃない）
		if( aOrgID.vt != VT_NULL && aOrgID.vt != VT_EMPTY && aOrgID.lVal != 0 ){
			continue;
		}

		//関連するリンクの削除・形状変化も見る
		CString aRelLayer;
		if(!fnRelObjsChange(ipNewRow, env.m_PGDBIndexMap[0], env.m_OrgTables[0], NULL, aRelLayer)){
			// 自身のInfの関連リンクが変更不可の場合は、該当Linkも変更不可
			return false;
		}

		ID_SET aAdd, aMod, aDel;
		if(!fnCanRefrectLq(infOID, ipOldTable[1], ipNewTable[1], ipSdeTable, env, aAdd, aMod, aDel)){
			// Lqも含め追加できない場合は、該当Linkも変更不可
			return false;
		}
			
		if(fnExistSameLQ(ipNewTable[1], ipNewRow, env, ipSdeTable[1] )){
			//同じと見なせるものがあったら追加しない扱いとなるため、該当Linkも変更不可
			return false;
		}
	}

	return true;
}
