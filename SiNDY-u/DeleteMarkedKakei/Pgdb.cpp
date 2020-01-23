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
#include <coord_converter.h>
#include "Pgdb.h"

namespace {

	//処理対象テーブル
	LPCTSTR gBuildTable[] = {
		_T("BUILDING"),
		_T("BUILDING_STEP"),
		_T("BUILDING_LINE")
	};

	crd_cnv cnv;	///< 座標変換クラス
}

// コンストラクタ
CPgdb::CPgdb(void)
: m_pFile(NULL)
, m_TargetPath(_T(""))
, m_MarkPath(_T(""))
, m_Word(_T(""))
, m_BuildIndex(-1)
{
}

// デストラクタ
CPgdb::~CPgdb(void)
{
}

// 初期化・事前準備
bool CPgdb::init(LPCTSTR cTargetPath, LPCTSTR cMarkPath, LPCTSTR cDelWord, LPCTSTR cDate1, LPCTSTR cDate2, FILE *pFile)
{
	if(pFile){
		m_pFile = pFile;
	}else{
		//通常ここには来ない設計だけど
		m_pFile = stderr;
	}
	if(_tcslen(cDelWord) < 1){
		_ftprintf(m_pFile, _T("#ERROR 削除条件ワードが不適切です。,「%s」\n"), cDelWord);
		_tprintf( _T("ERROR 削除条件ワードが不適切です。,「%s」\n"), cDelWord);
		return false;
	}
	m_Word = cDelWord;
	
	//フォルダの存在だけチェック
	if(_taccess(cTargetPath, 0x04) == 0){
		m_TargetPath = cTargetPath;
	}else{
		_ftprintf(m_pFile, _T("#ERROR ターゲットデータのパスが存在しないかアクセスできません。,%s\n"), cTargetPath);
		_tprintf(_T("ERROR ターゲットデータのパスが存在しないかアクセスできません。,%s\n"), cTargetPath);
		return false;
	}
	if(_taccess(cMarkPath, 0x04) == 0){
		m_MarkPath = cMarkPath;
	}else{
		_ftprintf(m_pFile, _T("#ERROR マーキングデータのパスが存在しないかアクセスできません。,%s\n"), cMarkPath);
		_tprintf(_T("ERROR マーキングデータのパスが存在しないかアクセスできません。,%s\n"), cMarkPath);
		return false;
	}

	//日付文字列を日付型に
	if(_tcslen(cDate1) > 1){
		m_Date[0] = cDate1;
		m_Date[0].ChangeType(VT_DATE);
		_ftprintf(m_pFile, _T("#無属性家形用の日付基準日：,%s\n"), cDate1);
	}else{
		m_Date[0].vt = VT_NULL;
		_ftprintf(m_pFile, _T("#無属性家形用の日付基準日は設定されていません。\n"));
	}
	if(_tcslen(cDate2) > 1){
		m_Date[1] = cDate2;
		m_Date[1].ChangeType(VT_DATE);
		_ftprintf(m_pFile, _T("#属性付き家形用の日付基準日：,%s\n"), cDate2);
	}else{
		m_Date[1].vt = VT_NULL;
		_ftprintf(m_pFile, _T("#属性付き家形用の日付基準日は設定されていません。\n"));
	}
	return true;
}

// マーク家形削除のメイン部
bool CPgdb::execDeleteKakei(LPCTSTR cMeshList)
{
	FILE *pFile;
	if((pFile = _tfopen(cMeshList, _T("rt"))) == NULL){
		_ftprintf(m_pFile, _T("#ERROR メッシュリストが開けません。,%s\n"), cMeshList);
		_tprintf( _T("ERROR メッシュリストが開けません。,%s\n"), cMeshList);
		return false;
	}
	char aBuf[32];
	while(fgets(aBuf, 32, pFile) != NULL){
		if(aBuf[0] != '#'){	//　先頭#はコメント扱い
			fnDeleteKakeiMesh(atol(aBuf));
		}
	}
	fclose(pFile);
	return true;
}

// メッシュ単位で対象家形を削除
bool CPgdb::fnDeleteKakeiMesh(long cMeshCode)
{
	_tprintf(_T("Now %d\n"), cMeshCode);
	IWorkspacePtr ipTgtWorkspace = fnOpenLocalDataBase(m_TargetPath, cMeshCode);
	if(!ipTgtWorkspace){
		return false;
	}
	IWorkspacePtr ipMarkWorkspace = fnOpenLocalDataBase(m_MarkPath, cMeshCode);
	if(!ipMarkWorkspace){
		return false;
	}
	IFeatureClassPtr ipGou = fnOpenFeatureClass(ipMarkWorkspace, _T("GOU_POINT"));
	if(!ipGou){
		return false;
	}
	for(int i = 0; i < 3; ++i){
		m_ipBuild[i] = fnOpenFeatureClass(ipTgtWorkspace, gBuildTable[i]);
		if(!m_ipBuild[i]){
			return false;
		}
		m_ipBuild[i]->FindField(CComBSTR(_T("ORG_MODIFYDATE")), &m_ModDateIndex[i]);
		if(m_ModDateIndex[i] < 0){
			_ftprintf(m_pFile, _T("#ERROR フィールドが見つかりません「ORG_MODIFYDATE」。,%d,%s\n"), cMeshCode, gBuildTable[i]);
			return false;
		}
	}

	m_ipBuild[0]->FindField(CComBSTR(_T("BLDCLASS_C")), &m_BuildIndex);
	if(m_BuildIndex < 0){
		_ftprintf(m_pFile, _T("#ERROR フィールドが見つかりません「BLDCLASS_C」。,%d,%s\n"), cMeshCode, gBuildTable[0]);
		return false;
	}

	//先に対象の抽出だけ
	std::vector<ID_SET> aIds(3);
	if(!fnGetDeleteTarget(ipGou, aIds)){
		return false;
	}

	//実際の削除
	for(int i = 0; i < 3; ++i){
		fnDeleteObject(aIds[i], i);
	}
	return true;
}

// 指定メッシュコードのPGDBオープン
IWorkspacePtr CPgdb::fnOpenLocalDataBase(LPCTSTR cBaseDir, long cMeshCode)
{
	CString aDatabase;
	aDatabase.Format(_T("%s\\%04d\\%d.mdb"), cBaseDir, cMeshCode/10000, cMeshCode); 

	// パーソナルジオDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// オープンしてみる
	ipWorkspaceFactory->OpenFromFile( CComBSTR( aDatabase ), 0, &ipWorkspace );
	if( !ipWorkspace )
	{
		_ftprintf( m_pFile, _T("#ERROR,%s が開けません。\n"), static_cast<LPCTSTR>(aDatabase));
	}
	return ipWorkspace;
}

// 指定フィーチャクラスオープン
IFeatureClassPtr CPgdb::fnOpenFeatureClass(const IWorkspacePtr& ipWorkspace, LPCTSTR cLayerName)
{
	IFeatureClassPtr ipFeatureClass;
	if(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(	CComBSTR(cLayerName), &ipFeatureClass) != S_OK || !ipFeatureClass){
		_ftprintf(m_pFile, _T("#ERROR,フィーチャークラスが開けません。,%s\n"), cLayerName);
		return NULL;
	}
	return ipFeatureClass;
}

//  削除対象（条件を満たす）のID列抽出
bool CPgdb::fnGetDeleteTarget(const IFeatureClassPtr& ipGou, std::vector<ID_SET>& cIds)
{
#ifdef PART_COMP	//部分一致なら
	//号ポイントに「指定したキーワード」含まれる号ポイントを検索
	CString aWhere;
	aWhere.Format(_T("GOU_NO LIKE '*%s*'"), m_Word);
	aWhere.Replace(_T("#"), _T("[#]"));	//#はSQLで「任意の半角数字」を示す特殊文字扱いのようなので、仕方なく
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("OBJECTID, SHAPE"), aWhere);
#else
	//完全一致なら
	IQueryFilterPtr ipFilter = AheInitQueryFilter( NULL, _T("OBJECTID, SHAPE"), _T("GOU_NO = '%s'"), m_Word);
#endif
	IFeatureCursorPtr ipCursor;

	HRESULT hr = ipGou->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,号ポイントの検索に失敗。(%d)\n"), hr);
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		//1つの号ポイントから3レイヤ分を抽出。日付による条件も考慮
		//家形は直接空間検索、それに関連するステップとラインの順で見て決める
		fnFindTarget(ipFeature, cIds);
	}
	return true;
}

// １つの号ポイントから、削除対象となる建物・段差・ラインのID列の抽出
bool CPgdb::fnFindTarget(const IFeaturePtr& ipGouFeature, std::vector<ID_SET>& cIds)
{
	IGeometryPtr ipGeom;
	ipGouFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		_ftprintf(m_pFile, _T("#ERROR,号の形状が取れません（通常出てはいけないエラー）。\n"));
		return false;
	}
	CComBSTR          bstrFieldName;
	m_ipBuild[0]->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, esriSpatialRelWithin );
	//ますは家形から
	IFeatureCursorPtr ipCursor;
	HRESULT hr = m_ipBuild[0]->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,家形の検索に失敗。(%d)\n"), hr);
		return false;
	}
	IFeaturePtr ipFeature;
	//最大でも1個しか見つからないはず
	if(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aVal;
		ipFeature->get_Value(m_BuildIndex, &aVal);

		bool aHaveKind = (aVal.vt != VT_NULL && aVal.lVal != 0);

		if(!fnDateCheck(ipFeature, 0, aHaveKind)){
			//日付条件をクリアしていなかったら対象外
			fnPrintLog(gBuildTable[0], ipFeature, 1, _T("指定日以降に編集された家形のため削除しません。"));
			return true;
		}
		//日付条件をクリアしていたら
		//家形に含まれる段差（ステップ）も検索
		IGeometryPtr ipBuildGeom;
		ipFeature->get_Shape(&ipBuildGeom);
		if(!ipBuildGeom){
			_ftprintf(m_pFile, _T("#ERROR,家形の形状が取れません（通常出てはいけないエラー）。\n"));
			return false;
		}
		if(!fnFindInner(ipBuildGeom, cIds[1], 1,aHaveKind)){
			//日付条件をクリアしていないものが1つでもあれば対象外
			fnPrintLog(gBuildTable[0], ipFeature, 1, _T("指定日以降に編集された段差が含まれているため削除しません。"));
			return true;
		}
		//日付条件をクリアしていたら
		//家形に含まれるライン検索
		fnFindInner(ipBuildGeom, cIds[2], 2, aHaveKind);
		long aObjID;
		ipFeature->get_OID(&aObjID);
		cIds[0].insert(aObjID);
	}else{
		long aObjID;
		ipGouFeature->get_OID(&aObjID);
		fnPrintLog(_T("GOU_POINT"), ipGouFeature, 1, _T("対応する家形が見つかりませんでした。"));
	}
	return true;
}

// 日付条件のチェック
bool CPgdb::fnDateCheck(const IFeaturePtr& ipFeature, int cIndex, bool cHaveKind)
{
	int aDataTarget = cHaveKind?1:0;
	if(m_Date[aDataTarget].vt == VT_NULL){
		return true;
	}
	CComVariant aVal;
	ipFeature->get_Value(m_ModDateIndex[cIndex], &aVal);
	if(aVal.vt != VT_NULL && aVal > m_Date[aDataTarget]){
		return false;
	}
	return true;
}

// 家形に含まれる段差・ラインのうち削除対象ID列を抽出
bool CPgdb::fnFindInner(const IGeometryPtr& ipGeom, ID_SET& cIds, int cIndex, bool cHaveKind)
{
	CComBSTR          bstrFieldName;
	m_ipBuild[cIndex]->get_ShapeFieldName( &bstrFieldName );
	ISpatialFilterPtr ipFilter = AheInitSpatialFilter( (ISpatialFilterPtr)AheInitQueryFilter( NULL, bstrFieldName ), ipGeom, bstrFieldName, esriSearchOrderSpatial, esriSpatialRelContains );

	IFeatureCursorPtr ipCursor;
	HRESULT hr = m_ipBuild[cIndex]->Search(ipFilter, VARIANT_TRUE, &ipCursor);
	if(FAILED(hr) || !ipCursor){
		_ftprintf(m_pFile, _T("#ERROR,家形に含まれるオブジェクト検索に失敗。(%d)\n"), hr);
		return false;
	}
	bool aCanDelete = true;
	ID_SET aTmpIds;
	IFeaturePtr ipFeature;
	//最大でも1個しか見つからないはず
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		if(fnDateCheck(ipFeature, cIndex, cHaveKind)){
			//日付条件をクリアしていたら
			long aObjID;
			ipFeature->get_OID(&aObjID);
			aTmpIds.insert(aObjID);
		}else{
			//1個でも日付条件を満たしていなかったら全て対象外
			aCanDelete = false;
			break;
		}
	}
	if(aCanDelete){
		//全部OKな場合だけ削除対象
		foreach(const long aObjID, aTmpIds){
			cIds.insert(aObjID);
		}
	}
	return aCanDelete;
}

// ID列指定でオブジェクトの削除
bool CPgdb::fnDeleteObject(const ID_SET& cIds, int cIndex)
{
	foreach(const long aObjID, cIds){
		IFeaturePtr ipFeature;
		m_ipBuild[cIndex]->GetFeature(aObjID, &ipFeature);
		if(ipFeature){
#ifndef _DEBUG
			ipFeature->Delete();
#endif
			fnPrintLog(gBuildTable[cIndex], ipFeature, 0, _T("ポリゴン削除"));
		}else{
			_ftprintf(m_pFile, _T("#ERROR,IDによるオブジェクト取得に失敗。（出てはいけない）,%s,%d\n"), gBuildTable[cIndex], aObjID);
			return false;
		}
	}
	return true;
}

// メッシュ・XY形式のログ出力
void CPgdb::fnPrintLog(LPCTSTR cLayerName, const IFeaturePtr& ipFeature, int cErrLv, LPCTSTR cMsg)
{
	static LPCTSTR aErrMsg[] = {
		_T("INFO"),
		_T("WARNING"),
		_T("ERROR")
	};
	long aObjID;
	ipFeature->get_OID(&aObjID);
	int aMesh = -1, aX = -1, aY = -1;
	fnGetMXY(ipFeature, aMesh, aX, aY);
	_ftprintf(m_pFile, _T("%d\t%d\t%d\t%s\t%ld\t%s\t%s\n"), aMesh, aX, aY, cLayerName, aObjID, aErrMsg[cErrLv], cMsg);
}

// フィーチャからログ出力用のメッシュ・XYを計算
bool CPgdb::fnGetMXY(const IFeaturePtr& ipFeature, int& cMesh, int& cX, int& cY)
{
	cMesh = cX = cY = 0;
	//ポイント、ポリゴン、ライン別にメッシュ、X,Yを取得
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(!ipGeom){
		return false;
	}
	esriGeometryType aType;
	ipGeom->get_GeometryType(&aType);
	switch(aType){
		case esriGeometryPoint:
			{
				//ポイントはその点
				double aX, aY;
				((IPointPtr)ipGeom)->QueryCoords(&aX, &aY);
				cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
			}
			break;
		case esriGeometryPolygon:
			{
				//ポリゴンは、ラベルポイント又は始点
				IAreaPtr ipArea = ipGeom;
				if(!ipArea){
					//極小だとありえるっぽい
					IPointPtr ipPoint;
					((IPointCollectionPtr)ipGeom)->get_Point(0, &ipPoint);
					if(!ipPoint){
						_ftprintf(m_pFile, _T("#ERROR,始点も代表点も取れないポリゴン。（出てはいけないはず）\n"));
						return false;
					}
					double aX, aY;
					ipPoint->QueryCoords(&aX, &aY);
					cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
				}else{
					IPointPtr ipPoint;
					ipArea->get_LabelPoint(&ipPoint);
					if(!ipPoint){
						((IPointCollectionPtr)ipGeom)->get_Point(0, &ipPoint);
						if(!ipPoint){
							_ftprintf(m_pFile, _T("#ERROR,始点も代表点も取れないポリゴン。（出てはいけないはず）\n"));
							return false;
						}
					}
					double aX, aY;
					ipPoint->QueryCoords(&aX, &aY);
					cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
				}
			}
			break;
		case esriGeometryPolyline:
			{
				//ラインは、構成点的に真ん中付近の点（2点だと始点）
				long aCount;
				((IPointCollectionPtr)ipGeom)->get_PointCount(&aCount);
				aCount /= 2;
				--aCount;
				IPointPtr ipPoint;
				((IPointCollectionPtr)ipGeom)->get_Point(aCount, &ipPoint);
				if(!ipPoint){
					_ftprintf(m_pFile, _T("#ERROR,中間点が取れないポリライン。（出てはいけないはず）\n"));
					return false;
				}
				double aX, aY;
				ipPoint->QueryCoords(&aX, &aY);
				cnv.LLtoMesh(aX, aY, 64, &cMesh, &cX, &cY);
			}
			break;
		default:
			{
				_ftprintf(m_pFile, _T("#ERROR,未対応のShapeType。（出てはいけない）\n"));
				return false;
			}
			break;
	}
	return true;
}
