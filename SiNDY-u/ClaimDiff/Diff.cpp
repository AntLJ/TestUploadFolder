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

// Diff.cpp: Diff クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Diff.h"

namespace {
	using namespace sindy::schema;
	//使用するフィーチャクラス全部
#ifdef _DEBUG
	LPCSTR DEF_CLAIM_LAYER = claimpoly_map::kTableName;
#else
	// Release版は定義だけにする。
	// 環境変数で指定(fnGetEnv()で指定した値を取得する)
	LPCSTR DEF_CLAIM_LAYER;
#endif

	LPCTSTR DEF_CITY_SITE = city_site::kTableName;
	LPCTSTR DEF_CITY_LINE = city_line::kTableName;
	LPCTSTR DEF_BUILDING_LINE = building_line::kTableName;

	LPCTSTR DEF_BUILDING = building::kTableName;
	LPCTSTR DEF_BUILDING_STEP = building_step::kTableName;
	LPCTSTR DEF_BUILDING_ROOF = building_roof::kTableName;
	LPCTSTR DEF_CITY_STATION = city_station::kTableName;
	LPCTSTR DEF_CITY_RAILWAY = city_railway::kTableName;
	LPCTSTR DEF_CITY_ANNOTATION = city_annotation::kTableName;
	LPCTSTR DEF_CITY_AREA = city_area::kTableName;
	LPCTSTR DEF_CITY_TUNNEL = city_tunnel::kTableName;
	LPCTSTR DEF_CITY_ROAD = city_road::kTableName;
	LPCTSTR DEF_CITY_DISP_LINE = city_disp_line::kTableName;
	LPCTSTR DEF_BASE_SITE = base_site::kTableName;
	LPCTSTR DEF_BASE_LINE = base_line::kTableName;
	LPCTSTR DEF_BASE_CONTOUR = base_contour::kTableName;
	LPCTSTR DEF_BASE_STATION = base_station::kTableName;
	LPCTSTR DEF_BASE_RAILWAY = base_railway::kTableName;
	LPCTSTR DEF_MIDDLE_SITE = middle_site::kTableName;
	LPCTSTR DEF_MIDDLE_LINE = middle_line::kTableName;
	LPCTSTR DEF_MIDDLE_CONTOUR = middle_contour::kTableName;
	LPCTSTR DEF_BASE_ANNOTATION = base_annotation::kTableName;
	LPCTSTR DEF_B_SC4DISP_LINE = b_sc4disp_line::kTableName;
	LPCTSTR DEF_B_SC3DISP_LINE = b_sc3disp_line::kTableName;
	LPCTSTR DEF_B_SC2DISP_LINE = b_sc2disp_line::kTableName;
	LPCTSTR DEF_B_SC1DISP_LINE = b_sc1disp_line::kTableName;
	LPCTSTR DEF_MIDDLE_ANNOTATION = middle_annotation::kTableName;
	LPCTSTR DEF_M_SC4DISP_LINE = m_sc4disp_line::kTableName;
	LPCTSTR DEF_M_SC3DISP_LINE = m_sc3disp_line::kTableName;
	LPCTSTR DEF_M_SC2DISP_LINE = m_sc2disp_line::kTableName;
	LPCTSTR DEF_M_SC1DISP_LINE = m_sc1disp_line::kTableName;
	LPCTSTR DEF_ROAD_LINK = road_link::kTableName;
	LPCTSTR DEF_ROAD_FIELD = road_link::kRoadClass;

#ifdef _DEBUG
	LPCTSTR DEF_CLAIM_NO_FIELD = claimpoly_map::kClaimNo;
#else
	// Release版は定義だけにする。
	// 環境変数で指定(fnGetEnv()で指定した値を取得する)
	LPCTSTR DEF_CLAIM_NO_FIELD;
#endif
	LPCSTR ErrLevel[] = {
		"Info",
		"Warning",
		"Error"
	};
	const double CONV_PARAM = 0.00001; //度をメートルに変換
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


LAYER_TABLE Diff::m_LayerTable[] = {
	LAYER_TABLE(DEF_CITY_ANNOTATION, TYPE_CITY_ANNO),
	LAYER_TABLE(DEF_CITY_DISP_LINE, TYPE_CITY_DISP),
	LAYER_TABLE(DEF_BASE_ANNOTATION, TYPE_BASE_ANNO),
	LAYER_TABLE(DEF_B_SC4DISP_LINE, TYPE_BASE_DISP),
	LAYER_TABLE(DEF_B_SC3DISP_LINE, TYPE_BASE_DISP),
	LAYER_TABLE(DEF_B_SC2DISP_LINE, TYPE_BASE_DISP),
	LAYER_TABLE(DEF_B_SC1DISP_LINE, TYPE_BASE_DISP),
	LAYER_TABLE(DEF_MIDDLE_ANNOTATION, TYPE_MIDDLE_ANNO),
	LAYER_TABLE(DEF_M_SC4DISP_LINE, TYPE_MIDDLE_DISP),
	LAYER_TABLE(DEF_M_SC3DISP_LINE, TYPE_MIDDLE_DISP),
	LAYER_TABLE(DEF_M_SC2DISP_LINE, TYPE_MIDDLE_DISP),
	LAYER_TABLE(DEF_M_SC1DISP_LINE, TYPE_MIDDLE_DISP),
	LAYER_TABLE(DEF_CITY_SITE, TYPE_CITY),
	LAYER_TABLE(DEF_CITY_LINE, TYPE_CITY),
	LAYER_TABLE(DEF_BUILDING, TYPE_BUILD),
	LAYER_TABLE(DEF_BUILDING_STEP, TYPE_BUILD),
	LAYER_TABLE(DEF_BUILDING_LINE, TYPE_BUILD),
	LAYER_TABLE(DEF_BUILDING_ROOF, TYPE_BUILD),
	LAYER_TABLE(DEF_CITY_STATION, TYPE_CITY),
	LAYER_TABLE(DEF_CITY_RAILWAY, TYPE_CITY),
	LAYER_TABLE(DEF_CITY_AREA, TYPE_CITY),
	LAYER_TABLE(DEF_CITY_TUNNEL, TYPE_CITY),
	LAYER_TABLE(DEF_CITY_ROAD, TYPE_CITY),
	LAYER_TABLE(DEF_BASE_SITE, TYPE_BASE),
	LAYER_TABLE(DEF_BASE_LINE, TYPE_BASE),
	LAYER_TABLE(DEF_BASE_CONTOUR, TYPE_BASE),
	LAYER_TABLE(DEF_BASE_STATION, TYPE_BASE),
	LAYER_TABLE(DEF_BASE_RAILWAY, TYPE_BASE),
	LAYER_TABLE(DEF_MIDDLE_SITE, TYPE_BASE),
	LAYER_TABLE(DEF_MIDDLE_LINE, TYPE_BASE),
	LAYER_TABLE(DEF_MIDDLE_CONTOUR, TYPE_BASE),
	LAYER_TABLE(DEF_ROAD_LINK, TYPE_ROAD),
	LAYER_TABLE(NULL, MyDataTypes(-1))
};

Diff::Diff()
: m_DiffX(0)
, m_DiffY(0)
{
}

Diff::~Diff(){
	m_ExpID.clear();
	m_ipClaim = NULL;
	for(int i = 0; m_LayerTable[i].m_LayerName != NULL; ++i){
		m_LayerTable[i].m_ipNewFeatureClass = NULL;
		m_LayerTable[i].m_ipOldFeatureClass = NULL;
		m_LayerTable[i].m_AttrIndex.clear();
	}
}

// 初期化
bool Diff::init(FILE *pFile, const IWorkspacePtr& ipMapWorkspace, const IWorkspacePtr& ipOldMapWorkspace,
		const IWorkspacePtr& ipAnoWorkspace, const IWorkspacePtr& ipOldAnoWorkspace,const IWorkspacePtr& ipBasWorkspace,
		const IWorkspacePtr& ipOldBasWorkspace, const IWorkspacePtr& ipRodWorkspace, const IWorkspacePtr& ipOldRodWorkspace,
		const IWorkspacePtr& ipBldWorkspace, const IWorkspacePtr& ipOldBldWorkspace, const IWorkspacePtr& ipClmWorkspace, const std::set<CString>& setCheckFeatures, bool bOutputAll)
{
	printf("情報の初期化をしています。\n");
	m_pFile = pFile;
	m_CheckFeatures = setCheckFeatures;
	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションを設定
	m_bOutputAll = bOutputAll;
	fnGetEnv();
	((IFeatureWorkspacePtr)ipClmWorkspace)->OpenFeatureClass(CComBSTR(DEF_CLAIM_LAYER), &m_ipClaim);
	if(m_ipClaim == NULL){
		fprintf(stderr, "#Error,フィーチャークラスが開けない。,%s\n", DEF_CLAIM_LAYER);
		return false;
	}
	m_ipClaim->FindField(CComBSTR(DEF_CLAIM_NO_FIELD), &m_ClaimNoIndex);
	if(m_ClaimNoIndex < 0){
		fprintf(stderr, "#Error,フィールドが見つからない。,%s[%s]\n", DEF_CLAIM_NO_FIELD, DEF_CLAIM_LAYER);
		return false;
	}
	esriGeometryType aShapeType;
	m_ipClaim->get_ShapeType(&aShapeType);
	if(aShapeType == esriGeometryPoint){
		if(m_BuffRadius == 0.0){
#ifdef _DEBUG
			m_BuffRadius = 50.0 * CONV_PARAM;
#else
			fprintf(stderr, "#Error,バッファーの半径が設定されていない。環境変数「RADIUS」を設定して下さい。\n");
			return false;
#endif
		}
		fprintf(m_pFile, "#半径約%3.2fmが対象\n", m_BuffRadius/CONV_PARAM);
		m_IsPoint = true;
	}else{
		m_IsPoint = false;
	}
	CComBSTR aLayerName;
	((IDatasetPtr)m_ipClaim)->get_Name(&aLayerName);
	m_LayerName = aLayerName;

	fprintf(m_pFile, "#オブジェクトの比較は、以下の順番で行われます。\n");
	for(int i = 0; m_LayerTable[i].m_LayerName != NULL; ++i){
		// チェック対象フィーチャでに指定されてなければ飛ばす
		if(m_CheckFeatures.find(CString(m_LayerTable[i].m_LayerName).MakeUpper()) == m_CheckFeatures.end())
			continue;
		printf("比較するフィーチャクラスの情報を収集しています。[%s]\n", m_LayerTable[i].m_LayerName);
		IFeatureWorkspacePtr ipNewFeatureWorkspace;
		IFeatureWorkspacePtr ipOldFeatureWorkspace;
		switch(m_LayerTable[i].m_DataType){
			case TYPE_BASE:
				ipNewFeatureWorkspace = ipBasWorkspace;
				ipOldFeatureWorkspace = ipOldBasWorkspace;
				break;
			case TYPE_ROAD:
				ipNewFeatureWorkspace = ipRodWorkspace;
				ipOldFeatureWorkspace = ipOldRodWorkspace;
				break;
			case TYPE_CITY:
				ipNewFeatureWorkspace = ipMapWorkspace;
				ipOldFeatureWorkspace = ipOldMapWorkspace;
				break;

			case TYPE_CITY_ANNO:
			case TYPE_CITY_DISP:
			case TYPE_BASE_ANNO:
			case TYPE_BASE_DISP:
			case TYPE_MIDDLE_ANNO:
			case TYPE_MIDDLE_DISP:
				//ANNO
				ipNewFeatureWorkspace = ipAnoWorkspace;
				ipOldFeatureWorkspace = ipOldAnoWorkspace;
				break;
			case TYPE_BUILD:
				ipNewFeatureWorkspace = ipBldWorkspace;
				ipOldFeatureWorkspace = ipOldBldWorkspace;
				break;
			default:
				fprintf(stderr, "#Error,不正な処理タイプ。(%s)\n", m_LayerTable[i].m_LayerName);
				break;
		}
		
		ipNewFeatureWorkspace->OpenFeatureClass(CComBSTR(m_LayerTable[i].m_LayerName), &(m_LayerTable[i].m_ipNewFeatureClass));
		if(m_LayerTable[i].m_ipNewFeatureClass == NULL){
			fprintf(stderr, "#Error,フィーチャークラスが開けない(新)。,%s\n", m_LayerTable[i].m_LayerName);
			return false;
		}
		ipOldFeatureWorkspace->OpenFeatureClass(CComBSTR(m_LayerTable[i].m_LayerName), &m_LayerTable[i].m_ipOldFeatureClass);
		if(m_LayerTable[i].m_ipOldFeatureClass == NULL){
			fprintf(stderr, "#Error,フィーチャークラスが開けない(旧)。,%s\n", m_LayerTable[i].m_LayerName);
			return false;
		}
		if(m_LayerTable[i].m_DataType != TYPE_ROAD){
			IFieldsPtr ipFields;
			m_LayerTable[i].m_ipOldFeatureClass->get_Fields(&ipFields);
			LONG aCount;
			ipFields->get_FieldCount(&aCount);
			for(int j = 0; j < aCount; ++j){
				IFieldPtr ipField;
				ipFields->get_Field(j, &ipField);
				esriFieldType aFieldType;
				ipField->get_Type(&aFieldType);
				if(aFieldType != esriFieldTypeGeometry && aFieldType != esriFieldTypeOID){
					CComBSTR aName;
					ipField->get_Name(&aName);
					LONG aIndex;
					m_LayerTable[i].m_ipNewFeatureClass->FindField(aName, &aIndex);
					if(aIndex >= 0){
						//新しい方にもあった時だけ
						CComBSTR aFieldName;
						ipField->get_Name(&aFieldName);
						CString aCmp = aFieldName;
						if(aCmp.CompareNoCase("OPERATOR") != 0 
							&& aCmp.CompareNoCase("PURPOSE_C") != 0 
							&& aCmp.CompareNoCase("MODIFYDATE") != 0
							&& aCmp.CompareNoCase("UPDATETYPE_C") != 0
							&& aCmp.CompareNoCase("PROGMODIFYDATE") != 0
							&& aCmp.CompareNoCase("MODIFYPROGNAME") != 0
							&& aCmp.CompareNoCase("USERCLAIM_F") != 0
							&& aCmp.CompareNoCase("TMP_MESHCODE") != 0
							&& aCmp.CompareNoCase("TMP_PRIMID") != 0
							&& aCmp.CompareNoCase("SOURCE") != 0
							&& aCmp.CompareNoCase("SHAPE.LEN") != 0
							&& aCmp.CompareNoCase("SHAPE.AREA") != 0){
							INDEX_PAIR aPair;
							aPair.m_OldIndex = j;
							aPair.m_NewIndex = aIndex;
							m_LayerTable[i].m_AttrIndex.push_back(aPair);
						}
					}
#ifdef _DEBUG
					else{
						printf("フィールド削除?(%s)\n", static_cast<LPCSTR>(CString(aName)));
					}
#endif
				}
			}
		}else{
			m_LayerTable[i].m_ipNewFeatureClass->FindField(CComBSTR(DEF_ROAD_FIELD), &m_NewRoadIndex);
			if(m_NewRoadIndex < 0){
				fprintf(stderr, "#Error,フィールドが見つからない。,%s\n", DEF_ROAD_FIELD);
				return false;
			}
			m_LayerTable[i].m_ipOldFeatureClass->FindField(CComBSTR(DEF_ROAD_FIELD), &m_OldRoadIndex);
			if(m_OldRoadIndex < 0){
				fprintf(stderr, "#Error,フィールドが見つからない。,%s\n", DEF_ROAD_FIELD);
				return false;
			}
		}

		fprintf(m_pFile, "#%d\t%s\n", i+1, m_LayerTable[i].m_LayerName);
	}
	printf("初期化終了。\n");
	return true;
}

bool Diff::execCheckDiff()
{
	LONG aTotalCount;
	m_ipClaim->FeatureCount(NULL, &aTotalCount);
	printf("%d個のクレームポリゴンがあります。\n", aTotalCount);
	fprintf(stderr, "#%d個のクレームポリゴンがあります。\n", aTotalCount);
	fprintf(m_pFile, "#%d個のクレームポリゴンがあります。\n", aTotalCount);
	IFeatureCursorPtr ipCursor;
	m_ipClaim->Search(NULL, VARIANT_FALSE, &ipCursor);
	if(ipCursor != NULL){
		IFeaturePtr ipFeature;
		LONG aCount = 0;
		while(ipCursor->NextFeature(&ipFeature) == S_OK){
			printf("%d/%d\r", ++aCount, aTotalCount);
			fnCheckDiffAll(ipFeature);
		}
		printf("\n");
	}else{
		fprintf(stderr, "#Error,クレームポリゴン検索失敗。\n");
		fprintf(m_pFile, "#Error,クレームポリゴン検索失敗。\n");
	}
	return true;
}

bool Diff::fnCheckDiffAll(IFeaturePtr ipFeature)
{
	bool aResult = true;

	fnGetClaimInfo(ipFeature);
	
	for(int i = 0; m_LayerTable[i].m_LayerName != NULL; ++i){
		if(m_CheckFeatures.find(CString(m_LayerTable[i].m_LayerName).MakeUpper()) != m_CheckFeatures.end())
			aResult &= fnCheckDiffNormal(ipFeature, i);
	}
	if(aResult){
		if(m_ClaimNG){
			fnMsgOut(ERRTYPE_INFO, "違いはないけどクレポリNG。");			
		}else{
			fnMsgOut(ERRTYPE_INFO, "違いなし。");
		}
	}
	return aResult;
}


bool Diff::fnCheckDiffNormal(IFeaturePtr ipFeature, LONG cIndex)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	if(m_CheckFeatures.find(CString(m_LayerTable[cIndex].m_LayerName).MakeUpper()) == m_CheckFeatures.end())
		return true;
	m_LayerTable[cIndex].m_ipNewFeatureClass->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	if(m_IsPoint){
		IGeometryPtr ipBuffered;
		((ITopologicalOperatorPtr)ipGeom)->Buffer(m_BuffRadius, &ipBuffered);
		ipGeom = ipBuffered;
		ISpatialReferencePtr ipSpRef;
		((IGeoDatasetPtr)m_LayerTable[cIndex].m_ipNewFeatureClass)->get_SpatialReference(&ipSpRef);
		ipGeom->putref_SpatialReference(ipSpRef);
		ipGeom->SnapToSpatialReference();
		((ITopologicalOperator2Ptr)ipGeom)->put_IsKnownSimple(VARIANT_FALSE);
		((ITopologicalOperatorPtr)ipGeom)->Simplify();
	}
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
#ifdef _DEBUG
	LONG aObjID;
	ipFeature->get_OID(&aObjID);
	if(aObjID == 10){
		int a = 0;
//		return true;
	}
#endif

	FEATURE_SET aRecord;
	FEATURE_TABLE aNewTable, aOldTable;

	IFeatureCursorPtr ipCursor;
	if(m_LayerTable[cIndex].m_ipNewFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnMsgOut(ERRTYPE_ERROR, "クレームポリゴンにかかるオブジェクト（新）検索失敗。");
		return false;
	}
	IFeaturePtr ipTgtFeature;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aRecord.m_ObjID);
		aRecord.m_ipFeature = ipTgtFeature;
		aNewTable.insert(aRecord);
	}
	if(m_LayerTable[cIndex].m_ipOldFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnMsgOut(ERRTYPE_ERROR, "クレームポリゴンにかかるオブジェクト（旧）検索失敗。");
		return false;
	}
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aRecord.m_ObjID);
		aRecord.m_ipFeature = ipTgtFeature;
		aOldTable.insert(aRecord);
	}
	if(m_LayerTable[cIndex].m_DataType == TYPE_ROAD){
		return fnCompareRoad(aOldTable, aNewTable, ipGeom, cIndex);
	}else{
		return fnCompareNormal(aOldTable, aNewTable, ipGeom, cIndex);
	}
}

bool Diff::fnCompareNormal(FEATURE_TABLE &cOldTable, FEATURE_TABLE &cNewTable, IGeometryPtr ipGeom, LONG cIndex)
{
	bool aResult = true;
	FEATURE_TABLE::iterator aOld, aNew;

	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションが有効ならaResult = falseでも処理を続行
	for(aOld = cOldTable.begin(); (m_IsPoint || aResult || m_bOutputAll) && aOld != cOldTable.end(); ++aOld){
		m_DiffX = m_DiffY = -1;
		aNew = cNewTable.find(aOld->m_ObjID);
		if(aNew != cNewTable.end()){
			aResult &= fnFeatureCompareNormal(aOld->m_ipFeature, aNew->m_ipFeature, ipGeom, cIndex);

			if(aResult){
				switch(m_LayerTable[cIndex].m_DataType){
					case TYPE_CITY_ANNO:
						aResult &= fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_CITY_DISP_LINE);
						break;
					case TYPE_CITY_DISP:
						aResult &= fnFeatureCompareDisp(aOld->m_ipFeature, cIndex, DEF_CITY_ANNOTATION);
						break;
					case TYPE_BASE_ANNO:
						if(!fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_B_SC4DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_B_SC3DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_B_SC2DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_B_SC1DISP_LINE)){
							aResult = false;
						}
						break;
					case TYPE_BASE_DISP:
						aResult &= fnFeatureCompareDisp(aOld->m_ipFeature, cIndex, DEF_BASE_ANNOTATION);
						break;
					case TYPE_MIDDLE_ANNO:
						if(!fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_M_SC4DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_M_SC3DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_M_SC2DISP_LINE)
							|| !fnFeatureCompareAnno(aOld->m_ipFeature, cIndex, DEF_M_SC1DISP_LINE)){
							aResult = false;
						}
						break;
					case TYPE_MIDDLE_DISP:
						aResult &= fnFeatureCompareDisp(aOld->m_ipFeature, cIndex, DEF_MIDDLE_ANNOTATION);
						break;
					default:
						break;
				}
			}

			cNewTable.erase(aNew);
		}else{
			CString aMsg;
			aMsg.Format("削除された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aOld->m_ObjID);
			fnMsgOut(ERRTYPE_WARNING, aMsg);
			aResult = false;
		}
	}

	m_DiffX = m_DiffY = -1;
	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションが有効ならaResult = falseでも処理を続行
	for(aNew = cNewTable.begin(); (m_IsPoint || aResult || m_bOutputAll) && aNew != cNewTable.end(); ++aNew){
		CString aMsg;
		aMsg.Format("追加された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aNew->m_ObjID);
		fnMsgOut(ERRTYPE_WARNING, aMsg);
		aResult = false;
	}

	return aResult;
}

bool Diff::fnFeatureCompareNormal(IFeaturePtr ipOldFeature, IFeaturePtr ipNewFeature, IGeometryPtr ipGeom, LONG cIndex)
{
	bool aResult = true;
	esriGeometryType aType;
	LONG aObjID;
	ipOldFeature->get_OID(&aObjID);
	CString aMsg;

	IGeometryPtr ipOldGeom, ipNewGeom;
	ipOldFeature->get_Shape(&ipOldGeom);
	ipNewFeature->get_Shape(&ipNewGeom);
	ipOldGeom->get_GeometryType(&aType);
	if(aType == esriGeometryPolygon){
		ITopologicalOperatorPtr ipTopo = ipGeom;
		IGeometryPtr ipOldClipedGeom, ipNewClipedGeom;
		ipTopo->Intersect(ipOldGeom, esriGeometry2Dimension, &ipOldClipedGeom);
		ipTopo->Intersect(ipNewGeom, esriGeometry2Dimension, &ipNewClipedGeom);
		if(ipOldClipedGeom != NULL && ipNewClipedGeom != NULL){
			((ITopologicalOperator2Ptr)ipOldClipedGeom)->put_IsKnownSimple(VARIANT_FALSE);
			((IPolygonPtr)ipOldClipedGeom)->SimplifyPreserveFromTo();
			((ITopologicalOperator2Ptr)ipNewClipedGeom)->put_IsKnownSimple(VARIANT_FALSE);
			((IPolygonPtr)ipNewClipedGeom)->SimplifyPreserveFromTo();
			ipOldGeom = ipOldClipedGeom;
			ipNewGeom = ipNewClipedGeom;
		}else{
			aMsg.Format("クリップ前の状態で比較\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
			fnMsgOut(ERRTYPE_INFO, aMsg);
		}
	}else if(aType == esriGeometryPolyline){
		if(ipGeom != NULL){
			//注記ラインの時はNULLで来るから
			ITopologicalOperatorPtr ipTopo = ipGeom;
			IGeometryPtr ipOldClipedGeom, ipNewClipedGeom;
			ipTopo->Intersect(ipOldGeom, esriGeometry1Dimension, &ipOldClipedGeom);
			ipTopo->Intersect(ipNewGeom, esriGeometry1Dimension, &ipNewClipedGeom);
			if(ipOldClipedGeom != NULL && ipNewClipedGeom != NULL){
				ipOldGeom = ipOldClipedGeom;
				ipNewGeom = ipNewClipedGeom;
			}else{
				aMsg.Format("クリップ前の状態で比較\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
				fnMsgOut(ERRTYPE_INFO, aMsg);
			}
		}
	}
	aResult = fnGeometryCompare(ipOldGeom, ipNewGeom, aType);
	if(!aResult){
		aMsg.Format("形状が変更された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
		fnMsgOut(ERRTYPE_WARNING, aMsg);
	}
	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションが有効ならaResult = falseでも処理を続行
	for(INDEX_LIST::iterator i = m_LayerTable[cIndex].m_AttrIndex.begin(); (m_IsPoint || aResult || m_bOutputAll) && i !=  m_LayerTable[cIndex].m_AttrIndex.end(); ++i){
		CComVariant aOldVal, aNewVal;
		ipOldFeature->get_Value(i->m_OldIndex, &aOldVal);
		ipNewFeature->get_Value(i->m_NewIndex, &aNewVal);
		if(aOldVal.vt != VT_DATE){
			if(aOldVal != aNewVal){
				ipOldFeature->get_OID(&aObjID);
				aMsg.Format("属性が変更された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
				fnMsgOut(ERRTYPE_WARNING, aMsg);
				aResult = false;
			}
		}else{
			if(aOldVal.date != aNewVal.date){
				ipOldFeature->get_OID(&aObjID);
				aMsg.Format("属性が変更された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
				fnMsgOut(ERRTYPE_WARNING, aMsg);
				aResult = false;
			}
		}
	}
	return aResult;
}

bool Diff::fnGeometryCompare(IGeometryPtr ipOldGeom, IGeometryPtr ipNewGeom, esriGeometryType cType)
{
	bool aResult = true;
	if(cType == esriGeometryPoint){
		IPointPtr ipOldPoint(ipOldGeom);
		IPointPtr ipNewPoint(ipNewGeom);
		double aOldX, aOldY;
		double aNewX, aNewY;
		ipOldPoint->QueryCoords(&aOldX, &aOldY);
		ipNewPoint->QueryCoords(&aNewX, &aNewY);
		if(aOldX != aNewX || aOldY != aNewY){
			m_DiffX = aNewX;
			m_DiffY = aNewY;
			aResult = false;
		}
	}else{
		IPointCollectionPtr ipOldPoints(ipOldGeom);
		IPointCollectionPtr ipNewPoints(ipNewGeom);
		
		LONG aOldCount, aNewCount;
		ipOldPoints->get_PointCount(&aOldCount);
		ipNewPoints->get_PointCount(&aNewCount);
//		if(aOldCount != aNewCount){
//			return false;
//		}
		WKSPoint *aOldPoint = new WKSPoint[aOldCount];
		WKSPoint *aNewPoint = new WKSPoint[aNewCount];

		ipOldPoints->QueryWKSPoints(0, aOldCount, aOldPoint);
		ipNewPoints->QueryWKSPoints(0, aNewCount, aNewPoint);

		for(int i = 0; i < aOldCount; ++i){
			if(aOldPoint[i].X != aNewPoint[i].X || aOldPoint[i].Y != aNewPoint[i].Y){
				m_DiffX = aNewPoint[i].X;
				m_DiffY = aNewPoint[i].Y;
				aResult = false;
				break;
			}
		}
		delete []aOldPoint;
		delete []aNewPoint;
	}
	return aResult;
}

bool Diff::fnGetClaimInfo(IFeaturePtr ipFeature)
{
	ipFeature->get_OID(&m_ClaimInfo.m_ObjID);
	if(m_ExpID.find(m_ClaimInfo.m_ObjID) != m_ExpID.end()){
		m_ClaimNG = true;
	}else{
		m_ClaimNG = false;
	}
	CComVariant aVal;
	ipFeature->get_Value(m_ClaimNoIndex, &aVal);
	if(aVal.vt != VT_BSTR){
		aVal.ChangeType(VT_BSTR);
	}
	m_ClaimInfo.m_Msg = aVal.bstrVal;
	return true;
}


void Diff::fnMsgOut(MyErrorLevel cErrLevel, LPCTSTR cMsg)
{
	if(m_DiffX > 0.0){
		fprintf(m_pFile, "0\t%s\t%d\t%f\t%f\t%s\t0\t%s\t%s\n", static_cast<LPCSTR>(m_LayerName), m_ClaimInfo.m_ObjID, m_DiffX, m_DiffY, ErrLevel[cErrLevel], cMsg, static_cast<LPCSTR>(m_ClaimInfo.m_Msg));
		fprintf(stderr, "%d\t%s\t%s\n", m_ClaimInfo.m_ObjID, ErrLevel[cErrLevel], cMsg);
	}else{
		fprintf(m_pFile, "0\t%s\t%d\t\t\t%s\t0\t%s\t%s\n", static_cast<LPCSTR>(m_LayerName), m_ClaimInfo.m_ObjID, ErrLevel[cErrLevel], cMsg, static_cast<LPCSTR>(m_ClaimInfo.m_Msg));
		fprintf(stderr, "%d\t%s\t%s\n", m_ClaimInfo.m_ObjID, ErrLevel[cErrLevel], cMsg);
	}
}


bool Diff::fnFeatureCompareRoad(IFeaturePtr ipOldFeature, IFeaturePtr ipNewFeature, IGeometryPtr ipGeom, LONG cIndex)
{
	bool aResult = true;
	CComVariant aOldVal, aNewVal;
	ipOldFeature->get_Value(m_OldRoadIndex, &aOldVal);
	ipNewFeature->get_Value(m_NewRoadIndex, &aNewVal);
	if(aOldVal != aNewVal){
		LONG aObjID;
		ipOldFeature->get_OID(&aObjID);
		CString aMsg;
		aMsg.Format("属性が変更された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
		fnMsgOut(ERRTYPE_WARNING, aMsg);
		aResult = false;
	}
	if(aResult){
#ifdef _DEBUG
		long aOldID, aNewID;
		ipOldFeature->get_OID(&aOldID);
		ipNewFeature->get_OID(&aNewID);
#endif
		IGeometryPtr ipOldGeom, ipNewGeom;
		ipOldFeature->get_Shape(&ipOldGeom);
		ipNewFeature->get_Shape(&ipNewGeom);
		ITopologicalOperatorPtr ipTopo = ipGeom;
		IGeometryPtr ipOldClipedGeom, ipNewClipedGeom;
		ipTopo->Intersect(ipOldGeom, esriGeometry1Dimension, &ipOldClipedGeom);
		if(ipOldClipedGeom == NULL){
#if 1
			esriGeometryType aType;
			ipGeom->get_GeometryType(&aType);
			VARIANT_BOOL aTouches ;
			HRESULT hr = ((IRelationalOperatorPtr)ipOldGeom)->Touches(ipGeom,&aTouches);
			if(SUCCEEDED(hr)){
				if(aTouches){
					printf("接するから除外\n");
				}
			}else{
				printf("接触判定失敗\n");
				IErrorInfoPtr	ipErrorInfo;
				::GetErrorInfo(0, &ipErrorInfo);
				if(ipErrorInfo != NULL){
					BSTR	hoge;
					ipErrorInfo->GetDescription(&hoge);
			//		ATLTRACE(hoge);
					fprintf(stderr, "Error,%s\n", static_cast<LPCSTR>(CString(hoge)));
				}else{
			//		ATLTRACE(_T("Unknown Error\n"));
					fprintf(stderr, "Unknown Error\n");
				}
			}
#endif
			//稀にここに来ることがあるらしい。
			//微妙に接するパターンらしいが、判定はできず
			return true;
		}
		ipTopo->Intersect(ipNewGeom, esriGeometry1Dimension, &ipNewClipedGeom);
		if(ipNewClipedGeom == NULL){
			//同上の理由でスルー
			return true;
		}
		aResult = fnGeometryCompare(ipOldClipedGeom, ipNewClipedGeom, esriGeometryPolyline);
		if(!aResult){
			LONG aObjID;
			ipOldFeature->get_OID(&aObjID);
			CString aMsg;
			aMsg.Format("形状が変更された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aObjID);
			fnMsgOut(ERRTYPE_WARNING, aMsg);
		}
	}
	return aResult;
}

bool Diff::fnCompareRoad(FEATURE_TABLE &cOldTable, FEATURE_TABLE &cNewTable, IGeometryPtr ipGeom, LONG cIndex)
{
	bool aResult = true;
	FEATURE_TABLE::iterator aOld, aNew;

	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションが有効ならaResult = falseでも処理を続行
	for(aOld = cOldTable.begin(); (aResult || m_bOutputAll) && aOld != cOldTable.end(); ++aOld){
		aNew = cNewTable.find(aOld->m_ObjID);
		if(aNew != cNewTable.end()){
			aResult = fnFeatureCompareRoad(aOld->m_ipFeature, aNew->m_ipFeature, ipGeom, cIndex);
			cNewTable.erase(aNew);
		}else{
			CString aMsg;
			aMsg.Format("削除された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aOld->m_ObjID);
			fnMsgOut(ERRTYPE_WARNING, aMsg);
			aResult = false;
		}
	}

	// [Bug 10390]差分のあった全てのオブジェクトの差分情報が出力されるオプションが欲しい
	// 差分すべてを出力するオプションが有効ならaResult = falseでも処理を続行
	for(aNew = cNewTable.begin(); (aResult || m_bOutputAll) && aNew != cNewTable.end(); ++aNew){
		CString aMsg;
		aMsg.Format("追加された。\t%s\t%d", m_LayerTable[cIndex].m_LayerName, aNew->m_ObjID);
		fnMsgOut(ERRTYPE_WARNING, aMsg);
		aResult = false;
	}
	return aResult;
}

bool Diff::fnFeatureCompareAnno(IFeaturePtr ipFeature, LONG cIndex, LPCTSTR cDispLayer)
{
	LONG aObjID;
	ipFeature->get_OID(&aObjID);	//新旧共通のはず

	LONG aDispIndex = fnFindTableIndex(cDispLayer);
	if(aDispIndex < 0){
		fprintf(stderr, "#Error,。対応するフィーチャークラスが見つからない。,%s\n", m_LayerTable[cIndex].m_LayerName);
		return false;
	}
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("ANNO_ID = %d", aObjID);
	ipFilter->put_WhereClause(CComBSTR(aWhere));

	FEATURE_SET aRecord;
	FEATURE_TABLE aNewTable, aOldTable;

	IFeatureCursorPtr ipCursor;
	if(m_LayerTable[aDispIndex].m_ipNewFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnMsgOut(ERRTYPE_ERROR, "注記ライン（新）検索失敗。");
		return false;
	}
	IFeaturePtr ipTgtFeature;
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aRecord.m_ObjID);
		aRecord.m_ipFeature = ipTgtFeature;
		aNewTable.insert(aRecord);
	}
	if(m_LayerTable[aDispIndex].m_ipOldFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnMsgOut(ERRTYPE_ERROR, "注記ライン（旧）検索失敗。");
		return false;
	}
	while(ipCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aRecord.m_ObjID);
		aRecord.m_ipFeature = ipTgtFeature;
		aOldTable.insert(aRecord);
	}
	return fnCompareNormal(aOldTable, aNewTable, NULL, aDispIndex);
}

LONG Diff::fnFindTableIndex(LPCTSTR cLayerName)
{
	LONG aIndex;

	for(aIndex = 0; m_LayerTable[aIndex].m_LayerName != NULL; ++aIndex){
		if(strcmp(m_LayerTable[aIndex].m_LayerName, cLayerName) == 0){
			return aIndex;
		}
	}
	return -1;
}

bool Diff::fnFeatureCompareDisp(IFeaturePtr ipFeature, LONG cIndex, LPCTSTR cAnnotationLayer)
{
	LONG aAnnoIDIndex;
	m_LayerTable[cIndex].m_ipOldFeatureClass->FindField(CComBSTR("ANNO_ID"), &aAnnoIDIndex);
	if(aAnnoIDIndex < 0){
		fprintf(stderr, "#Error,。「ANNO_ID」フィールドが見つからない。,%s\n", m_LayerTable[cIndex].m_LayerName);
		return false;
	}
	CComVariant aAnnoID;
	ipFeature->get_Value(aAnnoIDIndex, &aAnnoID);

	LONG aAnnoIndex = fnFindTableIndex(cAnnotationLayer);
	if(aAnnoIndex < 0){
		fprintf(stderr, "#Error,。対応するフィーチャークラスが見つからない。,%s\n", m_LayerTable[cIndex].m_LayerName);
		return false;
	}
	IFeaturePtr ipNewAnnoFeature, ipOldAnnoFeature;
	m_LayerTable[aAnnoIndex].m_ipNewFeatureClass->GetFeature(aAnnoID.lVal, &ipNewAnnoFeature);
	m_LayerTable[aAnnoIndex].m_ipOldFeatureClass->GetFeature(aAnnoID.lVal, &ipOldAnnoFeature);
	return fnFeatureCompareNormal(ipOldAnnoFeature, ipNewAnnoFeature, NULL, aAnnoIndex);
}


bool Diff::readExtFile(LPCTSTR cListFile)
{
	if(cListFile != NULL){
		FILE *pFile;
		if((pFile = fopen(cListFile, "rt")) != NULL){
			char aBuf[256];
			while(fgets(aBuf, 256, pFile) != NULL){
				if(aBuf[0] != '#' && aBuf[0] != '\n'){
					m_ExpID.insert(atol(aBuf));
				}
			}
			fclose(pFile);
		}else{
			fprintf(stderr, "#Error,例外ファイルが開けません。例外なしで処理します。,%s\n", cListFile);
		}
	}
	return true;
}

// 環境変数を取得する
void Diff::fnGetEnv(void)
{
	LPCSTR aEnv = getenv("FC_CLAIMPOLY_MAP");
	if(aEnv != NULL){
		DEF_CLAIM_LAYER = aEnv;
	}
	aEnv = getenv("FD_CLAIM_NO");
	if(aEnv != NULL){
		DEF_CLAIM_NO_FIELD = aEnv;
	}
	aEnv = getenv("RADIUS");
	if(aEnv != NULL){
		m_BuffRadius = atof(aEnv) * CONV_PARAM;
	}else{
		m_BuffRadius = 0.0;
	}

}
