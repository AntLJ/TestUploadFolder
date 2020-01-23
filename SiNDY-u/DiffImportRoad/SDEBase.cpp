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
#include "SDEBase.h"

using namespace Reflect;
using namespace sindy::schema;
namespace{
	LPCTSTR PROG_NAME = _T("DiffImportRoad");
}

// コンストラクタ
CSDEBase::CSDEBase(void)
: m_ExecInfo(_T(""))
, m_MaxX(0)
, m_MaxY(0)
, m_MinX(0)
, m_MinY(0)
, m_LayerManage(NULL)
{
}

// デストラクタ
CSDEBase::~CSDEBase(void)
{
}

// 初期化関数
bool CSDEBase::init(void)
{
	return true;
}

// 編集開始関数
bool CSDEBase::fnStartEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->StartEditing( VARIANT_FALSE ))){	// Undo Redo がいらないなら VARIANT_FALSE
			_ftprintf(stderr, _T("#ERROR 編集開始できませんでした(StartEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR 編集開始できませんでした(StartEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(FAILED(ipWorkspaceEdit->StartEditOperation())){
			_ftprintf(stderr, _T("#ERROR 編集開始できませんでした(StartEditOperation),%d,%s\n"), g_CurrentMesh,  cMsg);
			_tprintf(_T("ERROR 編集開始できませんでした(StartEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}

// 編集終了・保存関数
bool CSDEBase::fnStopEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->StopEditOperation())){
			_ftprintf(stderr, _T("#ERROR,編集終了失敗(StopEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,編集終了失敗(StopEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){;	// 編集を保存するなら VARIANT_TRUE にすること
			_ftprintf(stderr, _T("#保存成功,%d,%s\n"), g_CurrentMesh, cMsg);
		}else{
			_ftprintf(stderr, _T("#ERROR,保存失敗(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,保存失敗(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}

// 編集終了・破棄関数
bool CSDEBase::fnAbortEdit(LPCTSTR cMsg) const
{
	if(!(g_Mode & NO_CHG_MODE)){
		IWorkspaceEditPtr ipWorkspaceEdit = m_LayerManage->getEditWorkspace();
		if(FAILED(ipWorkspaceEdit->AbortEditOperation())){
			_ftprintf(stderr, _T("#ERROR,編集終了失敗(AbortEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,編集終了失敗(AbortEditOperation),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_FALSE ))){;	// 編集を保存するなら VARIANT_TRUE にすること
			_ftprintf(stderr, _T("#破棄成功,%d,%s\n"), g_CurrentMesh, cMsg);
		}else{
			_ftprintf(stderr, _T("#ERROR,破棄失敗(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			_tprintf(_T("ERROR,保存失敗(StopEditing),%d,%s\n"), g_CurrentMesh, cMsg);
			return false;
		}
	}
	return true;
}


// 中間保存関数
bool CSDEBase::fnMidSave(LPCTSTR cMsg) const
{
	if(fnStopEdit(cMsg)){
		return fnStartEdit(cMsg);
	}
	return false;
}

// 処理開始のメッシュ取得（中断したらその次から）関数
MESH_SET::iterator CSDEBase::fnGetFirstMesh(void) const
{
	MESH_SET::iterator aMesh = g_MeshSet.find(g_CurrentMesh);
	if(aMesh == g_MeshSet.end()){
		aMesh = g_MeshSet.begin();
	}else{
		++aMesh;
	}
	return aMesh;
}

/*
	全メッシュ処理関数
	引数：中間保存までの処理メッシュ数
	　　　-1は保存なし（オーバーフローするくらい回せばす保存されるけど）
*/
bool CSDEBase::executeALL(int cMidSave)
{
	CString aMsg =m_ExecInfo + _T("開始");
	g_IsContinue =false;
	if(!fnStartEdit(aMsg)){
		return false;
	}
	int aCount = 0;
	aMsg = _T("");
	MESH_SET::iterator aMesh;
	for(aMesh = fnGetFirstMesh(); aMesh  != g_MeshSet.end(); ++aMesh){
		g_CurrentMesh = *aMesh;
		_tprintf(_T("Now %s,%d\n"), m_ExecInfo,  g_CurrentMesh);

		if(!fnExecMesh()){
			//メッシュ毎の処理で処理続行不可能なエラーがあったら、破棄して終了
			aMsg = m_ExecInfo + _T("エラー");
			fnAbortEdit(aMsg);
			return false;
		}
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				_tprintf(_T("処理は中止されました\n"));
				_ftprintf(stderr, _T("#処理は中止されました,%d\n"), g_CurrentMesh);
				aMsg = _T("手動中止");
				break;
			}else if(aKey == 'P'){
				//中断
				_tprintf(_T("処理は一時中断されました\n"));
				_ftprintf(stderr, _T("#処理は一時中断されました,%d\n"), g_CurrentMesh);
				aMsg = _T("一時停止");
				g_IsContinue = true;
				break;
			}
		}
		if(++aCount == cMidSave){
			aMsg = m_ExecInfo + _T("中間");
			if(!fnMidSave(aMsg)){
				return false;
			}
			aCount = 0;
		}
	}
	if(aMesh == g_MeshSet.end()){
		//最後まで行ったら
		g_CurrentMesh = -1;
		g_NowProccess++;
		aMsg = m_ExecInfo + _T("終了");
	}
	if(!fnStopEdit((aMsg))){	//保存して終了
		return false;
	}
	return true;
}

// メッシュポリゴン又はメッシュ境界ライン取得関数
// 境界上の厳密な比較をするための最大最小値も取得
bool CSDEBase::fnGetMesh(void)
{
	IFeatureClassPtr ipFeatureClass = m_LayerManage->findTable(basemesh::kTableName);
	if(!ipFeatureClass){
		//バグのはずだけど
		fnPrintComError();
		return false;
	}
	CString aWhere;
	aWhere.Format(_T("%s = %d"), basemesh::kMeshCode, g_CurrentMesh);
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, NULL, aWhere);

	IFeatureCursorPtr ipCursor;
	if(FAILED(ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		return false;
	}
	IFeaturePtr ipFeature;
	ipCursor->NextFeature(&ipFeature);
	if(!ipFeature){
		fnPrintComError();
		return false;
	}
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		fnPrintComError();
		return false;
	}
	m_ipMeshPoly = ipGeom;
	//境界の「枠」も取得
	ITopologicalOperatorPtr ipTopo(m_ipMeshPoly);
	ipTopo->get_Boundary(&m_ipMeshBoundary);
	m_ipMeshBoundary->putref_SpatialReference( m_LayerManage->get_SpRef() );
	m_ipMeshBoundary->SnapToSpatialReference();

	IPointCollectionPtr ipPointCollection(m_ipMeshPoly);
	ATLASSERT( ipPointCollection != NULL );

	long aPointCount = 0;
	ipPointCollection->get_PointCount( &aPointCount );
	std::vector<WKSPoint> pPt( aPointCount );
	ipPointCollection->QueryWKSPoints( 0, aPointCount, &pPt.at(0) );


	double aMinX, aMinY, aMaxX, aMaxY;
	aMinX = aMinY = 1000000.0;
	aMaxX = aMaxY = -1000000.0;
	for( int j = 0; j < aPointCount; j++ ) {
		if(aMinX > pPt[j].X)
			aMinX = pPt[j].X;
		if(aMinY > pPt[j].Y)
			aMinY = pPt[j].Y;
		if(aMaxX < pPt[j].X)
			aMaxX = pPt[j].X;
		if(aMaxY < pPt[j].Y)
			aMaxY = pPt[j].Y;
	}

	m_MaxX = aMaxX;
	m_MaxY = aMaxY;
	m_MinX = aMinX;
	m_MinY = aMinY;
	return true;
}

// COMのエラー出力関数
void CSDEBase::fnPrintComError(void) const
{
	IErrorInfoPtr	ipErrorInfo;
	::GetErrorInfo(0, &ipErrorInfo);
	if(ipErrorInfo != NULL){
		CComBSTR	aMsg;
		ipErrorInfo->GetDescription(&aMsg);
		_ftprintf(stderr, _T("#ERROR %s,%d\n"), CString(aMsg), g_CurrentMesh);
		_tprintf(_T("ERROR %s,%d\n"), aMsg, g_CurrentMesh);
	}else{
		_ftprintf(stderr, _T("#UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
		_tprintf(_T("UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
	}
}

// 共通属性部分のコピーand/orセット関数
bool CSDEBase::fnSetCommonAttr(_IRowPtr& ipFeature, INDEX_MAP& cSDEIndexMap, INDEX_MAP& cPGDBIndexMap, const _IRowPtr& ipSrcFeature) const
{
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kOperator].m_Index, CComVariant(g_Operator)))){
		fprintf(stderr, "#最終更新者セット失敗\n");
		return false;
	}
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kUpdateType].m_Index, CComVariant(5)))){ //上海インポート
		fprintf(stderr, "#最終更新内容セット失敗\n");
		return false;
	}

	if(cSDEIndexMap.find(ipc_table::kUserClaim) != cSDEIndexMap.end() && cPGDBIndexMap.find(ipc_table::kUserClaim) != cPGDBIndexMap.end()){
		if(ipSrcFeature != NULL){
			CComVariant aClaim, aOrgClaim;
			ipSrcFeature->get_Value(cPGDBIndexMap[ipc_table::kUserClaim].m_Index, &aClaim);
			ipFeature->get_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, &aOrgClaim);
			if(aClaim != aOrgClaim){
				ipFeature->put_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, aClaim);
			}
			//情報ソースは出力時に出していないので反映しない
			//CComVariant aSource, aOrgSource;
			//ipSrcFeature->get_Value(g_SourceIndex[cIndex], &aSource);
			//ipFeature->get_Value(g_SourceIndex[cIndex], &aOrgSource);
			//if(aSource != aOrgSource){
			//	ipFeature->put_Value(cSDEIndexMap[cIndex], aSource);
			//}
		}else {
			ipFeature->put_Value(cSDEIndexMap[ipc_table::kUserClaim].m_Index, CComVariant(0));
			ipFeature->put_Value(cSDEIndexMap[ipc_table::kPurpose].m_Index, CComVariant(0));
		}
	}
	//嶋野さんのパクリ
	CString aStr = _T("");
	SYSTEMTIME stSystemTime;
	//嶋野さんのパクリ　ここまで

	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kModifyProgName].m_Index, CComVariant(PROG_NAME)))){
		_ftprintf(stderr, _T("#ERROR,プログラム名セット失敗\n"));
		return false;
	}
	//嶋野さんのパクリ
	::GetLocalTime( &stSystemTime);
	aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	//嶋野さんのパクリ　ここまで
	if(FAILED(ipFeature->put_Value(cSDEIndexMap[ipc_table::kProgModifyDate].m_Index, CComVariant(aStr)))){
		fprintf(stderr, "#更新日時セット失敗\n");
		return false;
	}
	return true;
}

// ログハンドル（標準タイプ）用のメッセージ出力関数
void CSDEBase::fnPrintLogStyre(int cType, LPCTSTR cLayerName, int cObjID, LPCTSTR cMsg, LPCTSTR cMsg2, REF_LAYER cRefLayer, IGeometryPtr ipGeom) const
{
	static LPCTSTR aErrType[] = {
		_T("INFO"),
		_T("WARNING"),
		_T("ERROR"),
		_T("CRITICAL")
	};
	CString aLayerName;
	switch(cRefLayer){
		case TGT_LAYER:
			aLayerName = m_LayerManage->findTableName(cLayerName);	// サーバから取得した名前（ユーザ名付き）
			break;
		case FIX_LAYER:
			aLayerName = g_FixUser + _T(".") + cLayerName;		// 指定したFIXユーザ名+タグ的に使用している名称
			break;
		case PGDB_LAYER:
			aLayerName = cLayerName;	// タグ的に使用している名称（ユーザ名なし）
			break;
		default:
			break;
	}
	if(ipGeom){
		double aX = -1.0, aY = -1.0;
		esriGeometryType aType = esriGeometryNull;
		ipGeom->get_GeometryType(&aType);
		switch(aType){
			case esriGeometryPolygon:
				{
					IPointPtr ipPoint;
					((IAreaPtr)ipGeom)->get_LabelPoint(&ipPoint);
					ipPoint->QueryCoords(&aX, &aY);
				}
				break;
			case esriGeometryPolyline:
				{
					IPointPtr ipPoint;
					((IPolylinePtr)ipGeom)->get_FromPoint(&ipPoint);
					ipPoint->QueryCoords(&aX, &aY);
				}
				break;
			case esriGeometryPoint:
				((IPointPtr)ipGeom)->QueryCoords(&aX, &aY);
				break;
			default:
				//ここには来ないはずだけど
				break;
		}
		if(cMsg2){
			_ftprintf(stderr, _T("0\t%s\t%d\t%.12f\t%.12f\t%s\t0\t%s\t%d\t%s\n"), aLayerName, cObjID, aX, aY, aErrType[cType], cMsg, g_CurrentMesh, cMsg2);
		}else{
			_ftprintf(stderr, _T("0\t%s\t%d\t%.12f\t%.12f\t%s\t0\t%s\t%d\n"), aLayerName, cObjID, aX, aY, aErrType[cType], cMsg, g_CurrentMesh);
		}
	}else{
		if(cMsg2){
			_ftprintf(stderr, _T("0\t%s\t%d\t\t%\t%s\t0\t%s\t%d\t%s\n"), aLayerName, cObjID, aErrType[cType], cMsg, g_CurrentMesh, cMsg2);
		}else{
			_ftprintf(stderr, _T("0\t%s\t%d\t\t%\t%s\t0\t%s\t%d\n"), aLayerName, cObjID, aErrType[cType], cMsg, g_CurrentMesh);
		}
	}
	if(cType > 2){
		//画面にも出力
		_tprintf(_T("%s\t%d\t\t\t%s\t0\t%s\t%d\n"), aLayerName, cObjID,aErrType[cType], cMsg, g_CurrentMesh);
	}
#ifdef _DEBUG
		fflush(stderr);
#endif
}

// 空間検索関数
IFeatureCursorPtr CSDEBase::fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum cSpatialRel) const
{
	if(!m_ipTable[0]){
		_ftprintf(stderr, _T("#ERROR,検索対象フィーチャクラスがNULLです,%d\n"), g_CurrentMesh);
		return NULL;
	}
	IFeatureCursorPtr ipCursor;
	CComBSTR          bstrFieldName;
	((IFeatureClassPtr)m_ipTable[0])->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, cSpatialRel );
	
	if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		_ftprintf(stderr, _T("#ERROR,関連するフィーチャー取得失敗,%d\n"), g_CurrentMesh);
		return NULL;
	}
	return ipCursor;
}
