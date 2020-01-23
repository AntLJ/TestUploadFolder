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
#include <sindy/workspace.h>
#include "LayerManage.h"
#include "Reflect.h"
using namespace Reflect;
using namespace sindy::schema;

// コンストラクタ
CLayerManage::CLayerManage(void)
: m_IsInitial(false)
, m_MeshLayer(_T("Reference.BaseMesh"))
, m_FcIndex(-1)
, m_ExtFcIndex(-1)
, m_OtherIndex(-1)
{
}

// デストラクタ
CLayerManage::~CLayerManage(void)
{
}

// 「今回は」使わない部分はコメントアウト
// 但し、有効にした時は調整が必要になるかも
// 処理の順番が重要になるので、相変わらずテーブル形式

///< フィーチャクラス管理テーブル
EnvTable CLayerManage::m_FcEnvTable[] = {
	//EnvTable(_T("駐車場"), parking_shape::kTableName),
	//EnvTable(_T("ボトルネック"), _T("BOTTLENECK_RAILCROSSING")),
	EnvTable(_T("道路リンク"), road_link::kTableName),
	EnvTable(_T("道路ノード"), road_node::kTableName),
#ifndef STOP_POINT
	EnvTable(_T("リンク内属性"), segment_attr::kTableName),
	EnvTable(_T("傾斜"), grad::kTableName),
	EnvTable(_T("背景ポリゴン"), base_site::kTableName),
	//編集することはないからいらない　EnvTable(_T("背景ライン"), base_line::kTableName),
#endif
	EnvTable(_T(""), _T(""))
};

///< フィーチャクラス(一時停止)管理テーブル
EnvTable CLayerManage::m_ExtFcEnvTable[] = {
	EnvTable(_T("一時停止"), stop_point::kTableName),
	EnvTable(_T(""), _T(""))
};

///< リンク列用テーブル管理テーブル
LqEnvTable CLayerManage::m_LqEnvTable[] = {
	LqEnvTable(_T("BYWAY"), inf_byway::kTableName, lq_byway::kTableName),
	LqEnvTable(_T("DIRGUIDE"), inf_dirguide::kTableName, lq_dirguide::kTableName),
	LqEnvTable(_T("GUIDE"), inf_guide::kTableName, lq_guide::kTableName),
	LqEnvTable(_T("INTERNAVI"), inf_internavi::kTableName, lq_internavi::kTableName),
	LqEnvTable(_T("INTERSECTION"), inf_intersection::kTableName, lq_intersection::kTableName),
	LqEnvTable(_T("LANE"), inf_lane::kTableName, lq_lane::kTableName),
	LqEnvTable(_T("ROUTE"), inf_route::kTableName, lq_route::kTableName),
	LqEnvTable(_T("TURNREG"), inf_turnreg::kTableName, lq_turnreg::kTableName),
	LqEnvTable(_T("UTURN"), inf_uturn::kTableName, lq_uturn::kTableName),
	LqEnvTable(_T("VICS"), inf_vics::kTableName, lq_vics::kTableName),
	LqEnvTable(_T(""), _T(""), _T(""))
};

///< その他テーブル管理テーブル
EnvTable CLayerManage::m_OtherTable[] = {
	EnvTable(_T("傾斜"), _T("SIMPLE_GRAD")),
	EnvTable(_T("一通"), time_oneway::kTableName),
	EnvTable(_T("通行禁止"), time_nopassage::kTableName),
//	EnvTable(_T("ボトルネック"), _T("SEGMENT_BOTTLENECK")),
	EnvTable(_T(""), _T(""))
};

// 初期化
bool CLayerManage::init(const CString cConnectStr[2])
{
	if(!m_IsInitial){
		fnGetEnv();
		m_IsInitial = true;
	}
#ifdef STOP_POINT
	for(int i = 0; i < 2; ++i){
		m_ipWorkspace[i] = connectSDE( cConnectStr[i] );
		if(!m_ipWorkspace[i]){
			return false;
		}
	}
#else
		m_ipWorkspace[0] = connectSDE( cConnectStr[0] );
		if(!m_ipWorkspace[0]){
			return false;
		}
#endif

	return fnOpenSDELayers(m_IsInitial);
}

// SDE接続関数
IWorkspacePtr CLayerManage::connectSDE(LPCTSTR lpszConnectStr)
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

// SDEのテーブル・フィーチャクラス（リンク列関連以外の全部）のオープンと各種準備
bool CLayerManage::fnOpenSDELayers(bool cIsEdit)
{
	// 基本フィーチャクラスの準備
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); i++){
#ifdef STOP_POINT
		//一時停止では、リンクやノードのバージョン対応の状態を見る必要がない
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_ROAD], m_FcEnvTable[i], false)){
			return false;
		}
#else
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_ROAD], m_FcEnvTable[i], cIsEdit)){
			return false;
		}
#endif
	}

#ifdef STOP_POINT
	// 拡張フィーチャクラスの準備（現状一時停止のみ）
	for(int i = 0; !m_ExtFcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		if(!fnOpenSDEFeatureClass(m_ipWorkspace[DB_STOP], m_ExtFcEnvTable[i], cIsEdit)){
			return false;
		}
	}
#else
	// 一時停止の代わりに、リンク列や各種テーブルの準備
	for(int i = 0; !m_LqEnvTable[i].m_OrgTables[0].IsEmpty(); i++){
		if(!fnOpenSDETables(m_ipWorkspace[DB_ROAD], m_LqEnvTable[i], cIsEdit)){
			return false;
		}
	}
	
	for(int i = 0; !m_OtherTable[i].m_OrgTable.IsEmpty(); i++){
		if(!fnOpenSDETable(m_ipWorkspace[DB_ROAD], m_OtherTable[i].m_SdeTable, m_OtherTable[i].m_SDEIndexMap, cIsEdit)){
			return false;
		}
	}
	// メッシュポリゴンはちょっと別管理（フィールド情報がいらない程度だけど）
	if(!fnOpenMeshPoly()){
		return false;
	}

#endif
	////最初だけでいいかもしれないけど、怖いから毎接続毎に取得（ノードは絶対ないといけないと思うから）
	if(m_TableInfo.find(road_node::kTableName) != m_TableInfo.end()){
		IGeoDatasetPtr ipDataset = m_TableInfo[road_node::kTableName].m_ipTable;
		if(FAILED(ipDataset->get_SpatialReference(&m_ipSpRef))){
			_ftprintf(stderr, _T("#ERROR,SpatialReference取得失敗,ROAD_NODE\n"));
			return false;
		}
		return true;
	}
	return false;
}

// 環境変数から設定を反映
void CLayerManage::fnGetEnv(void)
{
	//各フィーチャクラス名
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		CString aTmp = _T("FC_") + CString(m_FcEnvTable[i].m_OrgTable);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			m_FcEnvTable[i].m_SdeTable = aEnv;
		}
		_ftprintf(stderr, _T("#変更後設定：%s\n"), m_FcEnvTable[i].m_SdeTable);
	}
	for(int i = 0; !m_ExtFcEnvTable[i].m_OrgTable.IsEmpty(); ++i){
		CString aTmp = _T("FC_") + CString(m_ExtFcEnvTable[i].m_OrgTable);
		LPCTSTR aEnv = _tgetenv(aTmp);
		if(aEnv){
			m_ExtFcEnvTable[i].m_SdeTable = aEnv;
		}
		_ftprintf(stderr, _T("#変更後設定：%s\n"), m_ExtFcEnvTable[i].m_SdeTable);
	}
	LPCTSTR aEnv = _tgetenv(_T("FC_BASEMESH"));
	if(aEnv != NULL){
		m_MeshLayer = aEnv;
	}
}

// SDEのフィーチャクラスのオープンと各種準備（1個分）
bool CLayerManage::fnOpenSDEFeatureClass(const IFeatureWorkspacePtr& ipFeatureWorkspace, EnvTable& cEnv, bool cIsEdit)
{
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(ipFeatureWorkspace->OpenFeatureClass(CComBSTR(cEnv.m_SdeTable), &ipFeatureClass))){
		_ftprintf(stderr, _T("#ERROR フィーチャークラスが開けません,%s\n"), cEnv.m_SdeTable);
		_tprintf(_T("ERROR フィーチャークラスが開けません,%s\n"), cEnv.m_SdeTable);
		return false;
	}
	//反映対象ならバージョン対応であることが必要
	if(cIsEdit && !(g_Mode & NO_CHG_MODE)){
		//バージョン対応かチェック
	    IVersionedObjectPtr ipVersionedObject =ipFeatureClass;
		if(ipVersionedObject){
			VARIANT_BOOL aIsVersioned;
			ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
			if(!aIsVersioned){
				_tprintf(_T("ERROR,バージョン対応ではないため、更新できません\n"));
				_ftprintf(stderr, _T("#ERROR,バージョン対応ではないため、更新できません\n"));
				return false;
			}
		}
	}
	fnRegistTable(ipFeatureClass, cEnv.m_OrgTable);	// 識別にはタグとしてm_OrgTableを使う
	return getFieldInfo(ipFeatureClass, cEnv.m_SDEIndexMap);
}

// SDEのリンク列関連テーブル全部のオープンと各種準備
bool CLayerManage::fnOpenSDETables(const IFeatureWorkspacePtr& ipFeatureWorkspace, LqEnvTable& cEnv, bool cIsEdit)
{
	for(int i = 0; i < 2; ++i){
		if(!fnOpenSDETable(ipFeatureWorkspace, cEnv.m_SdeTables[i], cEnv.m_SDEIndexMap[i], cIsEdit)){
			return false;
		}
	}
	return true;
}

// SDEのリンク列関連テーブルのオープンと各種準備（1個分）
bool CLayerManage::fnOpenSDETable(const IFeatureWorkspacePtr& ipFeatureWorkspace, LPCTSTR cTableName, INDEX_MAP& cIndexMap, bool cIsEdit)
{
	ITablePtr ipTable;
	if(FAILED(ipFeatureWorkspace->OpenTable(CComBSTR(cTableName), &ipTable))){
		_ftprintf(stderr, _T("#ERROR テーブルが開けません,%s\n"), cTableName);
		_tprintf(_T("ERROR テーブルが開けません,%s\n"), cTableName);
		return false;
	}
	//反映対象ならバージョン対応であることが必要
	if(cIsEdit && !(g_Mode & NO_CHG_MODE)){
		//バージョン対応かチェック
	    IVersionedObjectPtr ipVersionedObject =ipTable;
		if(ipVersionedObject){
			VARIANT_BOOL aIsVersioned;
			ipVersionedObject->get_IsRegisteredAsVersioned(&aIsVersioned);
			if(!aIsVersioned){
				_tprintf(_T("ERROR,バージョン対応ではないため、更新できません\n"));
				_ftprintf(stderr, _T("#ERROR,バージョン対応ではないため、更新できません\n"));
				return false;
			}
		}
	}

	fnRegistTable(ipTable, cTableName);
	return getFieldInfo(ipTable,cIndexMap);
}
// 属性コピー対象のフィールド名とSDEのフィールドインデックスを取得
bool CLayerManage::getFieldInfo(const ITablePtr& ipTable, INDEX_MAP& cIndexMap)
{
	cIndexMap.clear();
	//コピーに必要なSDEのフィールド名とフィールドインデックスを取得
	IFieldsPtr ipFields;
	ipTable->get_Fields(&ipFields);
	if(!ipFields){
		return false;
	}
	long aCount;
	ipFields->get_FieldCount(&aCount);
	for(int i = 0; i < aCount; ++i){
		IFieldPtr ipField;
		ipFields->get_Field(i, &ipField);
		esriFieldType aType;
		ipField->get_Type(&aType);
		if(aType == esriFieldTypeOID || aType == esriFieldTypeGeometry){
			//OIDと形状はいらない（コピー対象にしない→特別扱い）
			continue;
		}
		CComBSTR abstrName;
		ipField->get_Name(&abstrName);
		CString aName = abstrName;
		//sindy::schemaも全部大文字であることを前提としている。この前提が崩れたら正常に動かない
		aName.MakeUpper();	//後の検索の時用に、念のため、全部大文字にする。
		if(aName.Find(_T("SHAPE.")) == 0 || aName == ipc_table::kSource){
			//SHAPE.LEN、SHAPE.AREA、SOURCEも除く
			continue;
		}
		CComBSTR aAlias;
		ipField->get_AliasName(&aAlias);

		cIndexMap[aName] = FieldInfo(i, CString(aAlias));
	}
	return true;
}

// 後処理関数（全部破棄してSDEから切断する）
void CLayerManage::unInit(void)
{
	m_ipSpRef = NULL;
	for(TABLE_INFO::iterator aLayerInfo = m_TableInfo.begin(); aLayerInfo != m_TableInfo.end(); ++aLayerInfo){
		aLayerInfo->second.m_ipTable = NULL;
	}
	m_TableInfo.clear();
	m_ipWorkspace[DB_ROAD] = m_ipWorkspace[DB_STOP] = NULL;
}

// 識別用テーブル名称から対応するテーブルを取得
ITablePtr CLayerManage::findTable(LPCTSTR cLayerName) const
{
	//const関数にするため、[]演算子を使わないようにしてみた
	TABLE_INFO::const_iterator it = m_TableInfo.find(cLayerName);
	if(it != m_TableInfo.end()){
		return it->second.m_ipTable;
	}else{
		return NULL;
	}
}

// 識別用テーブル名称から対応する実際の（ユーザ名付きとかの）テーブル名称を取得
LPCTSTR CLayerManage::findTableName(LPCTSTR cLayerName) const
{
	//const関数にするため、[]演算子を使わないようにしてみた
	TABLE_INFO::const_iterator it = m_TableInfo.find(cLayerName);
	if(it != m_TableInfo.end()){
		return it->second.m_LayerName;
	}else{
		return _T("不明（バグ）");
	}
}

// 編集（反映）対象のワークスペースを取得
IWorkspacePtr CLayerManage::getEditWorkspace(void) const
{
#ifdef STOP_POINT
	return m_ipWorkspace[DB_STOP];
#else
	return m_ipWorkspace[DB_ROAD];
#endif
}

// テーブルの種類毎に最初の処理対象の情報を取得
EnvTable* CLayerManage::getFitstEnv(TABLE_TYPE cType)
{
	switch(cType){
		case FC_TABLE:
			m_FcIndex = 0;
			return &m_FcEnvTable[m_FcIndex];
			break;
		case EXT_FC_TABLE:
			m_ExtFcIndex = 0;
			return &m_ExtFcEnvTable[m_ExtFcIndex];
			break;
		case OTHER_TABLE:
			m_OtherIndex = 0;
			return &m_OtherTable[m_OtherIndex];
			break;
		default:
			break;
	}
	//バグってなければここに来ないけどどうしよう
	return NULL;
}

// テーブルの種類毎に次の処理対象の情報を取得
EnvTable* CLayerManage::getNextEnv(TABLE_TYPE cType)
{
	switch(cType){
		case FC_TABLE:
			++m_FcIndex;
			return &m_FcEnvTable[m_FcIndex];
			break;
		case EXT_FC_TABLE:
			++m_ExtFcIndex;
			return &m_ExtFcEnvTable[m_ExtFcIndex];
			break;
		case OTHER_TABLE:
			++m_OtherIndex;
			return &m_OtherTable[m_OtherIndex];
			break;
		default:
			break;
	}
	//バグってなければここに来ないけどどうしよう
	return NULL;
}

// 識別用テーブル名称から対応する情報を取得
EnvTable* CLayerManage::findEnvTable(LPCTSTR cLayerName) const
{
	for(int i = 0; !m_FcEnvTable[i].m_OrgTable.IsEmpty(); i++){
		if(m_FcEnvTable[i].m_OrgTable == cLayerName){
			return &m_FcEnvTable[i];
		}
	}
	return NULL;
}

// リンク列で最初の処理対象の情報を取得
LqEnvTable* CLayerManage::getFitstLQEnv(void)
{
	m_LqIndex = 0;
	return &m_LqEnvTable[m_LqIndex];
}

// リンク列で次の処理対象の情報を取得
LqEnvTable* CLayerManage::getNextLQEnv(void)
{
	++m_LqIndex;
	return &m_LqEnvTable[m_LqIndex];
}

// メッシュポリゴンオープン
bool CLayerManage::fnOpenMeshPoly(void)
{
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(((IFeatureWorkspacePtr)m_ipWorkspace[DB_ROAD])->OpenFeatureClass(CComBSTR(m_MeshLayer), &ipFeatureClass))){
		_ftprintf(stderr, _T("#ERROR メッシュフィーチャークラスが開けません,%s\n"), m_MeshLayer);
		_tprintf(_T("ERROR メッシュフィーチャークラスが開けません,%s\n"), m_MeshLayer);
		return false;
	}
	fnRegistTable(ipFeatureClass, basemesh::kTableName);
	return true;
}

// 識別用テーブル名称とテーブルを関連づけて管理（登録）
bool CLayerManage::fnRegistTable(const ITablePtr& ipTable, LPCTSTR cTableName)
{
	CComBSTR aLayerName;
	((IDatasetPtr)ipTable)->get_Name(&aLayerName);
	LayerInfo aLayerInfo;
	aLayerInfo.m_LayerName = aLayerName;
	aLayerInfo.m_ipTable = ipTable;
	m_TableInfo[cTableName] = aLayerInfo;
	return true;
}


