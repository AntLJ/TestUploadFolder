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
#include "MeshMerge.h"
#include "merge_target.h"

using namespace Reflect;
using namespace sindy::schema;

namespace{
	double ACCEPTABLE_X  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（経度）
	double ACCEPTABLE_Y  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（緯度）
}

// コンストラクタ
CMeshMerge::CMeshMerge(void) : 
	m_FloorsIndex(),
	m_BGClsIndex(),
	m_AreaClsIndex(),
	m_AdmClsIndex()
{
	m_ExecInfo = _T("不要構成点削除・メッシュ境界マージ");
}

// デストラクタ
CMeshMerge::~CMeshMerge(void)
{
}

// 初期化関数
bool CMeshMerge::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
					  const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	m_testMode = args.m_testMode;
	m_logFile = args.m_logFile;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		if( g_ShapeType[i] == esriGeometryPolygon || g_ShapeType[i] == esriGeometryPolyline )
		{
			//1つでもポリゴン・ポリラインがあれば実行する
			if(_tgetenv(_T("ACCEPTABLE")) == NULL)
			{
				_tprintf(_T("マージ時誤差を許容しません\n"));
				_ftprintf(stderr, _T("#マージ時誤差を許容しません\n"));
				ACCEPTABLE_X = ACCEPTABLE_Y = 0.0;
			}
			else
			{
				_tprintf(_T("マージ時若干の誤差は許容します\n"));
				_ftprintf(stderr, _T("#マージ時若干の誤差は許容します\n"));
			}

			//背景用のマージ対象リストをsetに格納
			//背景ライン用のマージ対象リストをsetに格納
			//都市エリア用のマージ対象リストをsetに格納
			if(CString(gEnvTable[g_DataIndex].m_Suffix).CompareNoCase(_T("frm")) == 0)
			{
				for( const auto& bgClass : CS_TARGET_LIST ){
					m_CSTarget.insert(bgClass);
				}
				for( const auto& bgClass : CL_TARGET_LIST ){
					m_CLTarget.insert(bgClass);
				}
				for( const auto& areaClass : AP_TARGET_LIST ){
					m_APTarget.insert(areaClass);
				}
			}

			//行政界用のマージ対象リストをsetに格納
			if(CString(gEnvTable[g_DataIndex].m_Suffix).CompareNoCase(_T("adm")) == 0){
				for( const auto& admClass : ADM_TARGET_LIST ){
					m_ADMTarget.insert(admClass);
				}
			}

			return true;
		}
	}
	return false;
}

// メッシュ単位での境界上の不要構成点削除と境界マージ関数
bool CMeshMerge::fnExecMesh(COMMAND cCommand)
{
	_ftprintf(stderr, _T("#%d,%s\n"), g_CurrentMesh, _T("メッシュ境界マージ"));
	//メッシュ境界（枠）を取得
	if(!fnGetMesh(true))
	{
		return false;
	}

	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i)
	{
		// City_Roadは超巨大ポリゴンができる可能性が高いのでマージ対象外
		CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
		if( 0 == table.CompareNoCase(city_road::kTableName) )
		{
			_tprintf(_T("CITY_ROADはマージ対象外\n"));
			continue;
		}

		// ポリゴンは全部チェック
		// TODO: 先にSuffix等で判定して、見なくていいものは見ないようにする
		if(g_ShapeType[i] == esriGeometryPolygon)
		{
			// 処理対象フィーチャクラス設定（検索等に使用）
			m_ipFeatureClass = m_ipFeatureClasses[i];

			// 特殊な処理・比較をする属性があればインデックスを取得しておく
			// 階数は高い方にあわせる
			// 種別コードはフィーチャクラスによりマージしないコードがある(merge_target.h参照)

			// 階数？
			long aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(building_step::kFloors), &aIndex);
			m_FloorsIndex[i] = aIndex;

			// 都市背景？
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_site::kBgClass), &aIndex);
			m_BGClsIndex[i] = aIndex;

			// 都市エリア？
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_area::kAreaClass), &aIndex);
			m_AreaClsIndex[i] = aIndex;

			// 行政界？
			aIndex = -1;
			m_ipFeatureClass->FindField(CComBSTR(city_admin::kAreaClass), &aIndex);
			m_AdmClsIndex[i] = aIndex;

			// メッシュ境界の不要構成点削除
			fnDeleteDisusedPoint(i);

			// メッシュ境界マージ
			bool aLoop = true;
			while(aLoop){
				// メッシュ境界上のshapeをマージする
				aLoop = fnShapeMerge(i);
			}
		}
		else if(g_ShapeType[i] == esriGeometryPolyline)
		{
			// bug 10519 でライン系フィーチャもマージするようにした

			// Building_Line, City_Line以外はマージ対象外
			CString table = gEnvTable[g_DataIndex].m_OrgTableNames[i];
			if( 0 != table.CompareNoCase(building_line::kTableName) &&
				0 != table.CompareNoCase(city_line::kTableName) )
			{
				cout << (CStringA)table << "：マージ対象外" << endl;
				continue;
			}
			cout << (CStringA)table << "：マージ処理" << endl;

			// 処理対象フィーチャクラス設定（検索等に使用）
			m_ipFeatureClass = m_ipFeatureClasses[i];

			// City_Lineは要種別確認
			if( 0 == table.CompareNoCase(city_line::kTableName) )
			{
				long aIndex = -1;
				m_ipFeatureClass->FindField(CComBSTR(city_line::kBgClass), &aIndex);
				m_CLClsIndex[i] = aIndex;
			}

			// メッシュ境界マージ
			bool aLoop = true;
			while(aLoop){
				// メッシュ境界上のshapeをマージする
				aLoop = fnShapeMerge(i);
			}
		}

		for( auto& oid : m_deleteIDList )
		{
			IFeaturePtr ipDeleteTarget;
			m_ipFeatureClass->GetFeature( oid, &ipDeleteTarget );

			IGeometryPtr ipDeleteGeom;
			ipDeleteTarget->get_ShapeCopy( &ipDeleteGeom );

			if(FAILED(ipDeleteTarget->Delete()))
			{
				fnPrintLogStyle(error, i, oid, _T("フィーチャー削除失敗。"));
				continue;
			}
		}
	}
	return true;
}

// レイヤ単位のメッシュ境界上の不要構成点削除関数
bool CMeshMerge::fnDeleteDisusedPoint(long cIndex)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom);
	if( !ipCursor ){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aResult &= fnCheckAndDelete(ipFeature, cIndex);
	}
	return aResult;
}

// フィーチャ単位のメッシュ境界上の不要構成点削除関数
bool CMeshMerge::fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex)
{
	long aObjID = 0;
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_ShapeCopy( &ipSrcShape );
	bool aResult = false;
	int aCount = 0;
	IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aCount);
	if(aCount > 0 && ipResult != NULL){
		if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
			fnSetCommonAttr(ipFeature, cIndex, NULL);
			if(SUCCEEDED(ipFeature->Store())){
				fnPrintLogStyle(info, cIndex, aObjID, _T("不要構成点削除。"));
				aResult = true;
			}else{
				fnPrintComError();
				fnPrintLogStyle(error, cIndex, aObjID, _T("不要構成点削除更新失敗。"));
			}
		}else{
			fnPrintComError();
			fnPrintLogStyle(error, cIndex, aObjID, _T("不要構成点削除 Shape設定失敗。"));
		}
	}else{
		aResult = true;
	}

	return aResult;
}

// 不要構成点を削除した後の形状と削除したポイント数を求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
IGeometryPtr CMeshMerge::fnCroppingShape(IGeometryPtr ipSrcGeom, int& cCount)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart = 0;
	ipGeoCol->get_GeometryCount(&aPart);
	
	cCount = 0;
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cCount);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_ipSpRef);
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = fnCroppingRing(ipSubGeom, cCount);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// 不要構成点を削除した後の形状と削除したポイント数を求める関数（リング単位）。
IGeometryPtr CMeshMerge::fnCroppingRing(IGeometryPtr ipSrcGeom, int& cCount)
{
	//メッシュのコーナーの点は、一見不要だが、マージ前には必要になる場合が
	//あるため、残すように変更。
	//縦又は横に3メッシュ以上にわたるポリゴンの場合、メッシュコーナーを
	//除く処理が困難なため、各メッシュの範囲内のみを対象とするよう変更
	IGeometryPtr ipResultGeom = ipSrcGeom;
	bool aDel = false;
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	IPointPtr ipPoint;
	ipPointCollection->get_PointCount( &aPointCount );
	if(aPointCount > 4){
		for( int j = 0; j < aPointCount - 2; ++j) {
			double aBaseX, aBaseY;
			double aNextX1, aNextX2, aNextY1, aNextY2;
			ipPointCollection->get_Point( j, &ipPoint);
			ipPoint->QueryCoords(&aBaseX, &aBaseY);
			if(aBaseX == m_MinX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MinX && aNextX2 == m_MinX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--aPointCount;
						j--;
						continue;
					}
				}
			}else if(aBaseX == m_MaxX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--j;
						continue;
					}
				}
			}
			if(aBaseY == m_MinY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MinY && aNextY2 == m_MinY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						++cCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						--j;
					}
				}
			}else if(aBaseY == m_MaxY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--j;
					}
				}
			}
		}
	}
	//始点,終点が削除対象となる時の処理
	if(aPointCount > 4){
		double aBaseX, aBaseY;
		double aNextX1, aNextX2, aNextY1, aNextY2;
		ipPointCollection->get_Point( 0, &ipPoint);
		ipPoint->QueryCoords(&aBaseX, &aBaseY);
		if(aBaseX == m_MinX){
			ipPointCollection->get_Point( 1, &ipPoint); //始点の次
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);	//終点の1つ前
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MinX && aNextX2 == m_MinX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseX == m_MaxX){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aBaseY == m_MinY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MinY && aNextY2 == m_MinY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseY == m_MaxY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aDel){
			IRingPtr ipRing = ipResultGeom;
			if(ipRing != NULL){
				ipRing->Close();
			}else{
				IPolygonPtr ipPolygon = ipResultGeom;
				if(ipPolygon != NULL){
					ipPolygon->Close();
					ipPolygon->SimplifyPreserveFromTo();
				}else{
					ipResultGeom = NULL;
				}
			}
		}
	}
	return ipResultGeom;
}

// レイヤ単位でメッシュ境界のマージをする関数
bool CMeshMerge::fnShapeMerge(long cIndex/*, const IFeatureCursorPtr& ipCursor*/)
{
	// メッシュ境界で複数回マージする必要のあるオブジェクトというのが多々有り
	// それらは Aとマージ→形状取り直し→Bとマージ→・・・ と処理する必要があるため、
	// カーソルを何度か取り直すような処理になっている。

	bool mergeSuccess = false;
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom);
	if( !ipCursor )
	{
		return false;
	}

	m_mergedIDList.clear();
	IFeaturePtr ipFeature;
	while( !mergeSuccess )
	{
		if( ipCursor->NextFeature(&ipFeature) != S_OK || !ipFeature )
		{
			break;
		}
		long aObjID = 0;
		ipFeature->get_OID(&aObjID);
		if(m_mergedIDList.find(aObjID) == m_mergedIDList.end())
		{
			//リストになければ実行
			if( fnIsTargetKind(ipFeature, cIndex) )
			{
				if( fnSearchTargetsAndMerge(ipFeature, cIndex) )
				{
					mergeSuccess = true;
					break;
				}
			}
			else
			{
				// マージ対象外の属性であればリストに追加
				m_mergedIDList.insert(aObjID);
			}
		}
	}

	return mergeSuccess;
}

// フィーチャ毎にマージ可能なものをマージする関数
bool CMeshMerge::fnSearchTargetsAndMerge(IFeaturePtr ipSrcFeature, long cIndex)
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_Shape( &ipGeom );

	long srcObjID = 0;
	ipSrcFeature->get_OID( &srcObjID );

	IFeatureCursorPtr ipFeatureCursor = fnSelectByShape( ipGeom );
	if( !ipFeatureCursor )
		return false;

	IFeaturePtr ipTgtFeature;
	bool aRet = false;
	std::map<long,CAdapt<IFeaturePtr>> targetFeatures; // key:oid val:IFeature
	while( ipFeatureCursor->NextFeature( &ipTgtFeature ) == S_OK && ipTgtFeature )
	{
		long tgtObjID = 0;
		ipTgtFeature->get_OID( &tgtObjID );
		if( srcObjID == tgtObjID )
			continue;

		int aResult = -1;
		aResult = fnIsNeedMerge( ipSrcFeature, ipTgtFeature, cIndex );
		switch( aResult )
		{
		case 2:
			{
				// 処理中のメッシュではないメッシュ境界で一致している場合(3メッシュくらいまたいでいるときなど)
				aRet = false;
				break;
			}
		case 1:
			{
				targetFeatures[tgtObjID] = ipTgtFeature;
				break;
			}
		case 0:
			{
#if 0 // 大量に出るから特にメッセージは出さないでおく(元々そうしてたみたいだし)
				CString aMsg;
				aMsg.Format(_T("メッシュ境界で形状不一致！\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, tgtObjID, aMsg);
#endif
				break;
			}
		default:
			{
				CString aMsg;
				aMsg.Format(_T("マージできませんでした！\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, tgtObjID, aMsg);
				aRet = false;
				break;
			}
		}
	}

	// ライン系はメッシュ境界上の1点で3つ以上接してたらエラーにしておしまいにする。
	esriGeometryType geomType = esriGeometryNull;
	ipGeom->get_GeometryType( &geomType );
	if( geomType == esriGeometryPolyline )
	{
		// targetFeatures が2以上だとメッシュ境界上のある1点で3本以上接している
		if( targetFeatures.size() > 1 )
		{
			CString aMsg;
			aMsg.Format(_T("メッシュ境界で接するラインが3本以上あるためマージしません\t%d"), srcObjID);
			fnPrintLogStyle(info, cIndex, srcObjID, aMsg);

			// まとめてチェック済み扱いにする
			for( auto& target : targetFeatures )
			{
				m_mergedIDList.insert( target.first );
			}
			return false;
		}
	}

	for( auto& target : targetFeatures )
	{
		IFeaturePtr ipMergeFeature = target.second;
		if( fnCompAttr( ipSrcFeature, ipMergeFeature, cIndex ) )
		{
			bool aMergeResult = false;
			//IDの小さい方に吸収する。
			if( srcObjID < target.first )
			{
				aMergeResult = fnMerge( ipSrcFeature, ipMergeFeature, cIndex );
			}
			else
			{
				aMergeResult = fnMerge( ipMergeFeature, ipSrcFeature, cIndex );
			}

			if( aMergeResult )
			{
				CString aMsg;
				aMsg.Format(_T("マージしました\t%d"), srcObjID);
				fnPrintLogStyle(info, cIndex, target.first, aMsg);
				aRet = true;
			}
		}
		else
		{
			if( m_FloorsIndex[cIndex] != -1 )
			{
				//階数情報のみの違いは特別に吸収
				long aFloors = 0;
				if( ( aFloors = fnCompAttrSp( ipSrcFeature, ipMergeFeature, cIndex ) ) > 0 )
				{
					bool aMergeResult = false;
					//IDの小さい方に吸収する。
					if( srcObjID < target.first )
					{
						//階数は高い方を取る
						ipSrcFeature->put_Value(m_FloorsIndex[cIndex], CComVariant(aFloors));
						aMergeResult= fnMerge(ipSrcFeature, ipMergeFeature, cIndex);
					}
					else
					{
						ipMergeFeature->put_Value(m_FloorsIndex[cIndex], CComVariant(aFloors));
						aMergeResult= fnMerge(ipMergeFeature, ipSrcFeature, cIndex);
					}

					if( aMergeResult )
					{
						CString aMsg;
						aMsg.Format(_T("マージしました\t%d"), srcObjID);
						fnPrintLogStyle(info, cIndex, target.first, aMsg);
						aRet = true;
					}
				}
				else
				{
					CString aMsg;
					aMsg.Format(_T("隣接する属性が違う！（階数以外も）\t%d"), srcObjID);
					fnPrintLogStyle(error, cIndex, target.first, aMsg);
				}
			}
			else
			{
				CString aMsg;
				aMsg.Format(_T("隣接する属性が違う！\t%d"), srcObjID);
				fnPrintLogStyle(error, cIndex, target.first, aMsg);
			}
		}
	}

	return aRet;
}

// 形状的にマージ対象になるかを判定する関数（メッシュ境界上で線分を共有していること）
int CMeshMerge::fnIsNeedMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	IGeometryPtr ipSrcGeom, ipTgtGeom;

	long srcObjID = 0;
	ipSrcFeature->get_OID(&srcObjID);
	if(FAILED(ipSrcFeature->get_Shape( &ipSrcGeom )))
	{
		fnPrintLogStyle(error, cIndex, srcObjID, _T("SrcShapeの取得失敗。"));
		return -1;
	}

	long tgtObjID = 0;
	ipTgtFeature->get_OID(&tgtObjID);
	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom )))
	{
		fnPrintLogStyle(error, cIndex, tgtObjID, _T("TgtShapeの取得失敗。"));
		return -1;
	}

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	long aPointCountS = 0;
	long aPointCountT = 0;
	ipPointCollectionS->get_PointCount( &aPointCountS );
	ipPointCollectionT->get_PointCount( &aPointCountT );

	// 一応両方のジオメトリタイプ確認
	esriGeometryType srcGeomType = esriGeometryNull;
	esriGeometryType tgtGeomType = esriGeometryNull;
	ipSrcGeom->get_GeometryType( &srcGeomType );
	ipTgtGeom->get_GeometryType( &tgtGeomType );

	if( srcGeomType == esriGeometryPolygon && tgtGeomType == esriGeometryPolygon ){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for(int i = 0; i < aPointCountS -1; i++){
			if(aSrcPoints[i].X == m_MinX && aSrcPoints[i+1].X == m_MinX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MinX  && aTgtPoints[j+1].X == m_MinX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].X == m_MaxX && aSrcPoints[i+1].X == m_MaxX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MaxX  && aTgtPoints[j+1].X == m_MaxX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MinY && aSrcPoints[i+1].Y == m_MinY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MinY  && aTgtPoints[j+1].Y == m_MinY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MaxY && aSrcPoints[i+1].Y == m_MaxY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MaxY  && aTgtPoints[j+1].Y == m_MaxY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}
		}
	}else if( srcGeomType == esriGeometryPolyline && tgtGeomType == esriGeometryPolyline ){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for( auto srcPoint : aSrcPoints ){
			if( srcPoint.X == m_MinX ){
				// 左のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == m_MinX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.X == m_MaxX ){
				// 右のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.X == m_MaxX ){
						if( fnCompXY( srcPoint.Y, tgtPoint.Y, ACCEPTABLE_Y) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.Y == m_MinY ){
				// 下のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == m_MinY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return 1;
						}
					}
				}
			}else if( srcPoint.Y == m_MaxY ){
				// 上のメッシュのオブジェクトとマージ
				for( auto tgtPoint : aTgtPoints ){
					if( tgtPoint.Y == m_MaxY ){
						if( fnCompXY( srcPoint.X, tgtPoint.X, ACCEPTABLE_X) ){
							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

//　属性的にマージ対象になるかを判定する関数（決められた属性が一致すること）
bool CMeshMerge::fnCompAttr(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i)
	{
		FieldTable fieldTable = gEnvTable[g_DataIndex].m_Fields[cIndex][i];
		// 比較しなくて良いフィールドなら飛ばす
		if( !fieldTable.m_Compare )
			continue;

		long index = 0;
		index = fieldTable.m_SindyIndex;
		CComVariant aSrcVal, aTgtVal;
		ipSrcFeature->get_Value(index, &aSrcVal);
		ipTgtFeature->get_Value(index, &aTgtVal);

		// GEOSPACE_IDは特殊対応
		if( 0 == CString(building::kGeospaceID).CompareNoCase(fieldTable.m_fieldName) )
		{
			long aSrcObjID = 0, aTgtObjID = 0;
			ipSrcFeature->get_OID(&aSrcObjID);
			ipTgtFeature->get_OID(&aTgtObjID);

			// 両方NULLならマージする
			if( aSrcVal.vt == VT_NULL && aTgtVal.vt == VT_NULL )
				return true;

			// 片方がNULLならマージする(fnMerge()の動作変えたらこっちも見直すこと)
			// OBJECTID若い方が残るので、OBJECTIDが若いほうがNULLならGEOSPACE_IDを入れといてやる必要がある
			if( aSrcVal.vt != VT_NULL && aTgtVal.vt == VT_NULL )
			{
				if( aTgtObjID < aSrcObjID )
					ipTgtFeature->put_Value( index, aSrcVal );
				fnPrintLogStyle(info, cIndex, aTgtObjID, _T("GEOSPACE_IDがNULLの建物とマージ。"));
				return true;
			}
			if( aSrcVal.vt == VT_NULL && aTgtVal.vt != VT_NULL )
			{
				if( aSrcObjID < aTgtObjID )
					ipSrcFeature->put_Value( index, aTgtVal );
				fnPrintLogStyle(info, cIndex, aSrcObjID, _T("GEOSPACE_IDがNULLの建物とマージ。"));
				return true;
			}
			// 両方値が入っていて同じ値であればマージする
			if( aSrcVal.vt != VT_NULL && aTgtVal.vt != VT_NULL )
			{
				// 値違うならログにメッセージを出す
				if(aSrcVal != aTgtVal)
				{
					fnPrintLogStyle(info, cIndex, aTgtObjID, _T("GEOSPACE_IDが異なるためマージしません。"));
					return false;
				}
				return true;
			}
		}
		else
		{
			// GEOSPACE_ID以外は単純に比較
			if(aSrcVal != aTgtVal)
			{
				return false;
			}
		}
	}
	return true;
}

// 片方をマージした形状に更新し、片方を削除して、マージ後の状態を作る関数。
bool CMeshMerge::fnMerge(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	long aObjID = 0;
	ipSrcFeature->get_OID(&aObjID);

	IGeometryPtr ipSrcGeom, ipTgtGeom;

	if(FAILED(ipSrcFeature->get_ShapeCopy( &ipSrcGeom )))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("SrcShapeの取得失敗。"));
		return false;
	}

	if(FAILED(ipTgtFeature->get_ShapeCopy( &ipTgtGeom ))) // 変更加えるわけでは無いけれど念のためget_ShapeCopyを使う
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("TgtShapeの取得失敗。"));
		return false;
	}

	ITopologicalOperatorPtr ipSrcTopo = ipSrcGeom;
	IGeometryPtr ipTgtGeom2;
	if( FAILED(ipSrcTopo->Union(ipTgtGeom, &ipTgtGeom2)) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("Union失敗。"));
		return false;
	}

	if( !ipTgtGeom2 )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("マージ失敗。"));
		return false;
	}

	if( FAILED(ipTgtGeom2->putref_SpatialReference(m_ipSpRef)) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("putref_SpatialReference失敗。"));
		return false;
	}

	if(FAILED(((ITopologicalOperator2Ptr)ipTgtGeom2)->put_IsKnownSimple(VARIANT_FALSE)))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("put_IsKnownSimple失敗。"));
		return false;
	}

	if(FAILED(((ITopologicalOperatorPtr)ipTgtGeom2)->Simplify()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("Simplify失敗。"));
		return false;
	}

	if(FAILED(ipSrcFeature->putref_Shape(ipTgtGeom2)))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("マージ後Shape設定失敗。"));
		return false;
	}

	if( !fnSetCommonAttr(ipSrcFeature, cIndex, NULL) )
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("SetCommonAttr()失敗。"));
		return false;
	}

	if(FAILED(ipSrcFeature->Store()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("フィーチャー更新失敗。"));
		fnPrintComError();
		return false;
	}

	if(FAILED(ipTgtFeature->Delete()))
	{
		fnPrintLogStyle(error, cIndex, aObjID, _T("フィーチャー削除失敗。"));
		return false;
	}

	return true;
}

// 属性的にマージ対象になるかを判定する関数（建物高さを特別扱いしたもの）
int CMeshMerge::fnCompAttrSp(IFeaturePtr ipSrcFeature, IFeaturePtr ipTgtFeature, long cIndex)
{
	int aResult = -1;
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_fieldName != NULL; ++i)
	{
		// i番目が比較対象フィールドでなければ飛ばす
		if( !gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Compare )
			continue;

		CComVariant vaValueSrc, vaValueTgt;
		ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &vaValueSrc);
		ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex, &vaValueTgt);

		// 階数に違いがある場合は高い方に合わせる
		if(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SindyIndex == m_FloorsIndex[cIndex])
		{
			if(vaValueSrc.lVal > vaValueTgt.lVal)
			{
				aResult = vaValueSrc.lVal;
			}
			else
			{
				aResult = vaValueTgt.lVal;
			}
		}
		else
		{
			// 階数以外は単純に比較
			if(vaValueSrc != vaValueTgt)
			{
				return -1;
			}
		}
	}
	return aResult;
}

// 許容範囲を持たせた座標（値）の一致確認関数
bool CMeshMerge::fnCompXY(double cPoint1, double cPoint2, double cAcceptable)
{
	if(fabs(cPoint1 - cPoint2) <= cAcceptable){
		return true;
	}else{
		return false;
	}
}

// マージ対象属性かを判定する関数
bool CMeshMerge::fnIsTargetKind(const IFeaturePtr& ipFeature, const long cIndex)
{
	CString table = gEnvTable[g_DataIndex].m_OrgTableNames[cIndex];
	if(m_BGClsIndex[cIndex] >= 0 && table.CompareNoCase( city_site::kTableName ) == 0){
		// 背景ポリゴンなら
		CComVariant aVal;
		ipFeature->get_Value(m_BGClsIndex[cIndex], &aVal);
		if(m_CSTarget.find(aVal.lVal) == m_CSTarget.end()){
			// マージ対象リストになければ
			return false;
		}
	}else if(m_AreaClsIndex[cIndex] >= 0 && table.CompareNoCase( city_area::kTableName ) == 0){
		// 都市エリアなら
		CComVariant aVal;
		ipFeature->get_Value(m_AreaClsIndex[cIndex], &aVal);
		if(m_APTarget.find(aVal.lVal) == m_APTarget.end()){
			// マージ対象リストになければ
			return false;
		}
	}else if(m_AdmClsIndex[cIndex] >= 0 && table.CompareNoCase( city_admin::kTableName ) == 0){
		// 行政界なら
		CComVariant aVal;
		ipFeature->get_Value(m_AdmClsIndex[cIndex], &aVal);
		if(m_ADMTarget.find(aVal.lVal) == m_ADMTarget.end()){
			// マージ対象リストになければ
			return false;
		}
	}else if(m_CLClsIndex[cIndex] >= 0 && table.CompareNoCase( city_line::kTableName ) == 0){
		// City_Line
		CComVariant aVal;
		ipFeature->get_Value(m_CLClsIndex[cIndex], &aVal);
		if(m_CLTarget.find(aVal.lVal) == m_CLTarget.end()){
			// マージ対象リストになければ
			return false;
		}
	}
	// 基本的には全部マージ
	return true;
}
