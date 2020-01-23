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
#include "SDEBase.h"

using namespace Reflect;

// コンストラクタ
CSDEBase::CSDEBase(void)
: m_ExecInfo(_T(""))
, m_MaxX(0)
, m_MaxY(0)
, m_MinX(0)
, m_MinY(0)
, m_testMode(false)
{
}

// デストラクタ
CSDEBase::~CSDEBase(void)
{
}



// 初期化関数
bool CSDEBase::init( const CArguments& args, const IWorkspacePtr& ipWorkspace,
					 const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	m_testMode = args.m_testMode;
	m_logFile = args.m_logFile;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;
	return true;
}

// 編集開始関数
bool CSDEBase::fnStartEdit(LPCTSTR cMsg) const
{
	if(!m_testMode){
		if(FAILED(m_ipWorkspaceEdit->StartEditing( VARIANT_FALSE ))){	// Undo Redo がいらないなら VARIANT_FALSE
			_ftprintf(stderr, _T("#ERROR 編集開始できませんでした(StartEditing)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			_tprintf(_T("ERROR 編集開始できませんでした(StartEditing)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			return false;
		}
		if(FAILED(m_ipWorkspaceEdit->StartEditOperation())){
			_ftprintf(stderr, _T("#ERROR 編集開始できませんでした(StartEditOperation)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			_tprintf(_T("ERROR 編集開始できませんでした(StartEditOperation)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			return false;
		}
	}
	return true;
}


// 編集終了・保存関数
bool CSDEBase::fnStopEdit(LPCTSTR cMsg) const
{
	if(!m_testMode){
		if(FAILED(m_ipWorkspaceEdit->StopEditOperation())){
			_ftprintf(stderr, _T("#ERROR,編集終了失敗(StopEditOperation)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			_tprintf(_T("#ERROR,編集終了失敗(StopEditOperation)。,%d.%s\n,%s"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			return false;
		}
		if(SUCCEEDED(m_ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){;	// 編集を保存するなら VARIANT_TRUE にすること
			_ftprintf(stderr, _T("#保存成功,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
		}else{
			_ftprintf(stderr, _T("#ERROR,保存失敗(StopEditing)。,%d.%s\n,%s"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
			_tprintf(_T("#ERROR,保存失敗(StopEditing)。,%d.%s,%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix), cMsg);
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
MESH_MAP::iterator CSDEBase::fnGetFirstMesh(void) const
{
	MESH_MAP::iterator aMesh = g_MeshMap.find(g_CurrentMesh);
	if(aMesh == g_MeshMap.end()){
		aMesh = g_MeshMap.begin();
	}else{
		aMesh++;
	}
	return aMesh;
}


// 全メッシュ処理関数
bool CSDEBase::executeALL(int cMidSave)
{
	CString aMsg =m_ExecInfo + _T("開始");
	g_IsContinue =false;
	if(!fnStartEdit(aMsg)){
		return false;
	}
	int aCount = 0;
	aMsg = _T("");
	MESH_MAP::iterator aMesh;
	for(aMesh = fnGetFirstMesh();aMesh != g_MeshMap.end(); ++aMesh){
		g_CurrentMesh = aMesh->first;
		_tprintf(_T("Now %s,%d.%s\n"), static_cast<LPCTSTR>(m_ExecInfo), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix));

		fnExecMesh(aMesh->second);
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				_tprintf(_T("処理は中止されました。\n"));
				_ftprintf(stderr, _T("#処理は中止されました。,%d.%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix));
				aMsg = _T("手動中止");
				break;
			}else if(aKey == 'P'){
				//中断
				_tprintf(_T("処理は一時中断されました。\n"));
				_ftprintf(stderr, _T("#処理は一時中断されました。,%d.%s\n"), g_CurrentMesh, static_cast<LPCTSTR>(g_Suffix));
				aMsg = _T("一時停止");
				g_IsContinue = true;
				break;
			}
		}
		if(++aCount == cMidSave){
			aMsg =m_ExecInfo + _T("中間");
			if(!fnMidSave(aMsg)){
				return false;
			}
			aCount = 0;
		}
	}
	if(aMesh == g_MeshMap.end()){
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
bool CSDEBase::fnGetMesh(bool cGetBoundary)
{
	IGeometryPtr ipGeom = g_MeshPolyMap[g_CurrentMesh];
	if(cGetBoundary){
		//ポリゴンではなく、境界の「枠」を取得
		ITopologicalOperatorPtr ipTopo(ipGeom);
		IGeometryPtr ipBoundary;
		ipTopo->get_Boundary(&ipBoundary);
		m_ipMeshGeom = ipBoundary;
		m_ipMeshGeom->putref_SpatialReference( m_ipSpRef );
		m_ipMeshGeom->SnapToSpatialReference();
	}else{
		m_ipMeshGeom = ipGeom;
	}
	IPointCollectionPtr ipPointCollection(ipGeom);
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
		BSTR	hoge;
		ipErrorInfo->GetDescription(&hoge);
		CString aMsg(hoge);
		_ftprintf(stderr, _T("#ERROR %s,%d\n"), aMsg, g_CurrentMesh);
		_tprintf(_T("ERROR %s,%d\n"), aMsg, g_CurrentMesh);
	}else{
		_ftprintf(stderr, _T("#UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
		_tprintf(_T("UNKNOWN COM ERROR,%d\n"), g_CurrentMesh);
	}
}

// 共通属性部分のコピーand/orセット関数
bool CSDEBase::fnSetCommonAttr(IFeaturePtr ipFeature, long cIndex, const IFeaturePtr& ipSrcFeature) const
{
	using namespace sindy::schema::ipc_table;
	using namespace sindy::schema::category::sindyc_table;
	if( m_sindyCImport )
	{
		if( ipSrcFeature )
		{
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_OrgOperatorIndex[cIndex], kOrgOperator);
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_OrgModifyIndex[cIndex], kOrgModifyDate);
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_OrgObjIdIndex[cIndex], kOrgOBJID);
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_UpdateCIndex[cIndex], kUpdateC);
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_CheckCIndex[cIndex], kCheckC);
			fnSetAttrFromFeature(ipFeature, ipSrcFeature, g_MeshcodeIndex[cIndex], _T("MESHCODE"));
		}
	}
	else
	{
		if(FAILED(ipFeature->put_Value(g_ProgNameIndex[cIndex], CComVariant(PROG_NAME)))){
			_ftprintf(stderr, _T("#ERROR,プログラム名セット失敗。\n"));
			return false;
		}

		SYSTEMTIME stSystemTime;
		::GetLocalTime( &stSystemTime);
		CString aStr;
		aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
		if(FAILED(ipFeature->put_Value(g_ProgModifyDateIndex[cIndex], CComVariant(aStr)))){
			_ftprintf(stderr, _T("#更新日時セット失敗。\n"));
			return false;
		}
	}

	if(FAILED(ipFeature->put_Value(g_OperatorIndex[cIndex], CComVariant(g_Operator)))){
		_ftprintf(stderr, _T("#最終更新者セット失敗。\n"));
		return false;
	}

	if(FAILED(ipFeature->put_Value(g_UpdateIndex[cIndex], CComVariant(5)))){ //上海インポート
		_ftprintf(stderr, _T("#最終更新内容セット失敗。\n"));
		return false;
	}

	if( ipSrcFeature ){
		CComVariant aClaim, aOrgClaim;
		ipSrcFeature->get_Value(g_ClaimIndex[cIndex], &aClaim);
		ipFeature->get_Value(g_ClaimIndex[cIndex], &aOrgClaim);
		if(aClaim != aOrgClaim){
			ipFeature->put_Value(g_ClaimIndex[cIndex], aClaim);
		}
		CComVariant aSource, aOrgSource;
		ipSrcFeature->get_Value(g_SourceIndex[cIndex], &aSource);
		ipFeature->get_Value(g_SourceIndex[cIndex], &aOrgSource);
		if(aSource != aOrgSource){
			ipFeature->put_Value(g_SourceIndex[cIndex], aSource);
		}
	}else {
		ipFeature->put_Value(g_ClaimIndex[cIndex], CComVariant(0));
		ipFeature->put_Value(g_PurposeIndex[cIndex], CComVariant(0));
	}

	return true;
}

// ログハンドル（標準タイプ）用のメッセージ出力関数
void CSDEBase::fnPrintLogStyle(ErrorCode cType, int cIndex, int cObjID, LPCTSTR cMsg) const
{
	m_logFile.WriteLog( static_cast<LPCTSTR>(g_LayerName[cIndex]), cObjID, cType, cMsg, g_CurrentMesh );
}

// 空間検索関数
IFeatureCursorPtr CSDEBase::fnSelectByShape(const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel, const CString& subFields) const
{
	if(ipGeom == NULL){
		_ftprintf(stderr, _T("#ERROR,検索用ジオメトリがNULLです。,%d\n"), g_CurrentMesh);
		return NULL;
	}
	if(m_ipFeatureClass == NULL){
		_ftprintf(stderr, _T("#ERROR,検索対象フィーチャクラスがNULLです。,%d\n"), g_CurrentMesh);
		return NULL;
	}
	IFeatureCursorPtr ipCursor;
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR          bstrFieldName;
	m_ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipGeom );
	ipFilter->put_SpatialRel( SpatialRel );
	if( !subFields.IsEmpty() )
		ipFilter->put_SubFields( CComBSTR(subFields) );
	
	if(FAILED(m_ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		fnPrintComError();
		_ftprintf(stderr, _T("#ERROR,関連するフィーチャー取得失敗。,%d\n"), g_CurrentMesh);
		return NULL;
	}
	return ipCursor;
}

// レコード数取得
long CSDEBase::fnGetSelectCountByShape( ID_SET& cIDList, const IGeometryPtr& ipGeom, esriSpatialRelEnum SpatialRel )
{
	IFeatureCursorPtr ipFeatureClass = fnSelectByShape( ipGeom, SpatialRel, sindy::schema::kObjectID );

	long lCount = 0;
	IFeaturePtr ipFeature;
	while( ipFeatureClass->NextFeature(&ipFeature) == S_OK && ipFeature )
	{
		++lCount;
		long oid = 0;
		ipFeature->get_OID( &oid );
		cIDList.insert( oid );
	}
	return lCount;
}

bool CSDEBase::fnSetAttrFromFeature( IFeaturePtr ipFeature, const IFeaturePtr& ipSrcFeature, long index, const CString& fieldName ) const
{
	CComVariant val;
	ipSrcFeature->get_Value( index, &val );
	if( FAILED(ipFeature->put_Value( index, val ) ) )
	{
		_ftprintf(stderr, _T("#ERROR,%sセット失敗。\n"), fieldName);
		return false;
	}
	return true;
}
