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

// AChiban.cpp: AChiban クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AChiban.h"

namespace {
LPCTSTR CITY_CODE_NAME = _T("CITYCODE");
LPCTSTR ADDR_CODE_NAME = _T("ADDRCODE");
//#define FUGO_CODE_NAME "GAIKUFUGO"
LPCTSTR AREA_FLAG_NAME = _T("AreaClass_c");
LPCTSTR CHIBAN_NUMBER = _T("CHIBAN_NO");
LPCTSTR CHIBAN_CLASS = _T("POINTCLASS_C");
LPCTSTR CHIBAN_TYPE = _T("ANNO_F");

LPCTSTR MODIFY_PROG_NAME = _T("MODIFYPROGNAME");
LPCTSTR MODIFY_DATE_NAME = _T("PROGMODIFYDATE");
LPCTSTR PROG_NAME = _T("AjustChiban");

//#define CROP_DISTANCE (0.0003) //ポイント間の最小値過去との互換を保ったつもり（実測およそ）
//#define CROP_DISTANCE (0.0001953) 
const double CROP_DISTANCE = 0.0004; //過去との互換を保ったつもり（多すぎたからちょっと厳しく）
//#define BUF_DISTANCE  (0.0001) //検索するポイントの行政界境界からの距離
const double BUF_DISTANCE  = 0.0002; //検索するポイントの行政界境界からの距離

}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AChiban::AChiban()
: m_ChgMode(false)
, m_LayerName(_T(""))
{

}

AChiban::AChiban(IWorkspacePtr ipWorkspace)
{
	m_ipWorkspace = ipWorkspace;
}

AChiban::~AChiban()
{
	m_IDSet.clear();
}

bool AChiban::init(IFeatureClassPtr ipGyoseiClass, IFeatureClassPtr  ipChibanClass, bool cChgMode)
{
	m_ipGyosei = ipGyoseiClass;
	m_ipChiban = ipChibanClass;

	CComBSTR aName;
	((IDatasetPtr)m_ipChiban)->get_Name(&aName);
	m_LayerName = aName;
	m_ChgMode = cChgMode;
	m_ipGyosei->FindField(CComBSTR(AREA_FLAG_NAME), &m_AreafIndex);
	if(m_AreafIndex < 0){
		_ftprintf(stderr, _T("#エリアフラグのフィールドが見つからない。\n"));
		return false;
	}
	m_ipChiban->FindField(CComBSTR(CHIBAN_TYPE), &m_CTypeIndex);
	if(m_CTypeIndex < 0){
		_ftprintf(stderr, _T("#地番注記フラグのフィールドが見つからない。\n"));
		return false;
	}
	m_ipChiban->FindField(CComBSTR(CHIBAN_CLASS), &m_CClassIndex);
	if(m_CClassIndex < 0){
		_ftprintf(stderr, _T("#代表点の種別フィールドが見つからない。\n"));
		return false;
	}
	m_ipChiban->FindField(CComBSTR(ADDR_CODE_NAME), &m_ElevenIndex);
	if(m_ElevenIndex < 0){
		_ftprintf(stderr, _T("#代表点の11桁コードフィールドが見つからない。\n"));
		return false;
	}
	if(m_ChgMode){
		if(m_ipChiban->FindField(CComBSTR(MODIFY_PROG_NAME), &m_ProgNameIndex) != S_OK){
			_ftprintf(stderr, _T("#最終更新プログラム名フィールドが見つからない。[%s]\n"), MODIFY_PROG_NAME);
			return false;
		}
		if(m_ProgNameIndex < 0){
			_ftprintf(stderr, _T("#最終更新プログラム名フィールドが見つからない。[%s]\n"), MODIFY_PROG_NAME);
			return false;
		}
		if(m_ipChiban->FindField(CComBSTR(MODIFY_DATE_NAME), &m_ModifyDateIndex) != S_OK){
			_ftprintf(stderr, _T("#最終更新日付フィールドが見つからない。[%s]\n"), MODIFY_DATE_NAME);
			return false;
		}
		if(m_ModifyDateIndex < 0){
			_ftprintf(stderr, _T("#最終更新日付フィールドが見つからない。[%s]\n"), MODIFY_DATE_NAME);
			return false;
		}
	}
	return true;
}

bool AChiban::execAjustChiban(LPCTSTR cAddrCode)
{
	m_TargetCode = cAddrCode;
//	m_CityCode = atoi(m_TargetCode.Left(5));
	/// クエリフィルタ作成
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString		sWhere;

	int aLen = m_TargetCode.GetLength();
	if(aLen == 5){
		sWhere.Format(_T("%s = '%s'"), CITY_CODE_NAME, cAddrCode); //5桁一致
	}else if(aLen == 11){
		sWhere.Format(_T("%s = '%s' AND %s = '%s'"), CITY_CODE_NAME, m_TargetCode.Left(5), ADDR_CODE_NAME, m_TargetCode.Right(6)); //11桁一致
	}else if(aLen == 8){
		sWhere.Format(_T("%s = '%s' AND %s LIKE '%s%%'"), CITY_CODE_NAME, m_TargetCode.Left(5), ADDR_CODE_NAME, m_TargetCode.Right(3)); //8桁一致
	}else{
		_ftprintf(stderr, _T("5桁、8桁、11桁で指定して下さい。[%s]\n"), cAddrCode);
		return false;
	}

	ipFilter->put_WhereClause(CComBSTR(sWhere));
	long lCount;
	m_ipGyosei->FeatureCount(ipFilter, &lCount);
	if( lCount  < 1) {
		_ftprintf(stderr, _T("#該当する行政界がありません。,%s\n"), cAddrCode);
		return false;
	}
	_ftprintf(stderr, _T("#%s,%d個の行政界が見つかりました。\n"), cAddrCode, lCount);
	IFeatureCursorPtr ipCursor;
	if(m_ipGyosei->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		_ftprintf(stderr, _T("#行政界の検索失敗,%s\n"), cAddrCode);
		return false;
	}
	_tprintf(_T("検査しています。\n"));
	IFeaturePtr ipFeature;
	bool aResult = false;
	LONG aCount = 0;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aCount++;
		CComVariant aAreaF;
		ipFeature->get_Value(m_AreafIndex, &aAreaF);
		if(aAreaF.lVal == 1){
			aResult |= fnCheckChiban(ipFeature);
		}
	}
	if(aCount != lCount){
		_ftprintf(stderr, _T("#Error,%d/%d個の行政界しか処理していません。,%s\n"), aCount, lCount, cAddrCode);
	}
	return aResult;
}

bool AChiban::unInit()
{
	m_IDSet.clear();
	return true;
}

bool AChiban::fnCheckChiban(IFeaturePtr ipAdmFeature)
{
	IGeometryPtr ipGeom;
	ipAdmFeature->get_Shape(&ipGeom);

	IGeometryPtr ipAdmLine;
	((ITopologicalOperatorPtr)ipGeom)->get_Boundary(&ipAdmLine);
	//行政界境界から一定距離の代表点を検索
	IGeometryPtr ipBuffer;
	((ITopologicalOperatorPtr)ipAdmLine)->Buffer(BUF_DISTANCE, &ipBuffer);

	if(ipBuffer == NULL){
		LONG aObjID;
		ipAdmFeature->get_OID(&aObjID);
		_ftprintf(stderr, _T("#Error,行政界の検索バッファーがNULL,%s,%d\n"), static_cast<LPCTSTR>(m_TargetCode), aObjID);
		IErrorInfoPtr	ipErrorInfo;
		::GetErrorInfo(0, &ipErrorInfo);
		if(ipErrorInfo != NULL){
			BSTR	hoge;
			ipErrorInfo->GetDescription(&hoge);
	//		ATLTRACE(hoge);
			_ftprintf(stderr, _T("#Error,%s\n"), static_cast<LPCTSTR>(CString(hoge)));
		}else{
	//		ATLTRACE(_T("Unknown Error\n"));
			_ftprintf(stderr, _T("#Unknown Error\n"));
		}
		return false;
	}
	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR          bstrFieldName;
	m_ipChiban->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipBuffer );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

#ifdef _DEBUG
	LONG aObjID2;
	ipAdmFeature->get_OID(&aObjID2);
	_tprintf(_T("Adm = %d\n"), aObjID2);
#endif

	IFeatureCursorPtr ipFeatureCursor;
	if(m_ChgMode){
		if(m_ipChiban->Update(ipFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK){
			_ftprintf(stderr, _T("#Error,既存の代表点検索失敗,%s\n"), static_cast<LPCTSTR>(m_TargetCode));
			return false;
		}
	}else{
		if(m_ipChiban->Search(ipFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK){
			_ftprintf(stderr, _T("#Error,既存の代表点検索失敗,%s\n"), static_cast<LPCTSTR>(m_TargetCode));
			return false;
		}
	}
	bool aResult = false;
	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK){
		LONG aObjID;
		ipFeature->get_OID(&aObjID);
		m_IDSet.insert(aObjID); //いずれにしても次からは対象外

		CComVariant aNowType;
		ipFeature->get_Value(m_CClassIndex, &aNowType);
		if(aNowType.vt == VT_NULL || aNowType.lVal != 0){
			continue;
		}
		CComVariant aFlag;
		ipFeature->get_Value(m_CTypeIndex, &aFlag);
		if(aFlag.vt != VT_NULL && aFlag.lVal == 1){
			if(fnAjust(ipFeature)){
				aResult = true;
				if(m_ChgMode){
					//一定距離内に地番注記があれば、自分が消える
					ipFeature->put_Value(m_CTypeIndex, CComVariant(0));
					//嶋野さんのパクリ
					CString aStr = _T("");
					SYSTEMTIME stSystemTime;
					//嶋野さんのパクリ　ここまで

					if(FAILED(ipFeature->put_Value(m_ProgNameIndex, CComVariant(PROG_NAME)))){
						_ftprintf(stderr, _T("#プログラム名セット失敗。\n"));
						return false;
					}
					//嶋野さんのパクリ
					::GetLocalTime( &stSystemTime);
					aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
							stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
							stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
					//嶋野さんのパクリ　ここまで
					if(FAILED(ipFeature->put_Value(m_ModifyDateIndex, CComVariant(aStr)))){
						_ftprintf(stderr, _T("#更新日時セット失敗。\n"));
						return false;
					}
					if(FAILED(ipFeatureCursor->UpdateFeature(ipFeature))){
						_ftprintf(stderr, _T("#フラグ変更失敗。,%d\n"), aObjID);
					}else{
						_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\t更新\n"), static_cast<LPCTSTR>(m_LayerName), aObjID);
					}
				}else{
					_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\t更新が必要\n"), static_cast<LPCTSTR>(m_LayerName), aObjID);
				}
			}
		}
	}
	return aResult;
}

bool AChiban::fnAjust(IFeaturePtr ipSrcFeature)
{
	bool aResult = false;
	IGeometryPtr ipGeom;
	ipSrcFeature->get_Shape(&ipGeom);

	//ポイントから一定距離のポイントを検索
	IGeometryPtr ipBuffer;
	((ITopologicalOperatorPtr)ipGeom)->Buffer(CROP_DISTANCE, &ipBuffer);
	if(ipBuffer == NULL){
		LONG aObjID;
		ipSrcFeature->get_OID(&aObjID);
		_ftprintf(stderr, _T("#Error,代表点の検索バッファーがNULL,%s,%d\n"), static_cast<LPCTSTR>(m_TargetCode), aObjID);
		return false;
	}

	ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
	CComBSTR          bstrFieldName;
	m_ipChiban->get_ShapeFieldName( &bstrFieldName );
	ipFilter->put_GeometryField( bstrFieldName );
	ipFilter->putref_Geometry( ipBuffer );
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipFeatureCursor;
	if(m_ipChiban->Search(ipFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK){
		_ftprintf(stderr, _T("#既存の代表点検索失敗,%s\n"), static_cast<LPCTSTR>(m_TargetCode));
		return false;
	}

	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK){
		LONG aObjID;
		ipFeature->get_OID(&aObjID);
		IDSET::iterator aID = m_IDSet.find(aObjID);
		if(aID == m_IDSet.end()){ //自分自身でも、既に検査済みのものでもなければ
			CComVariant aFlag;
			ipFeature->get_Value(m_CTypeIndex, &aFlag);
			if(aFlag.vt != VT_NULL && aFlag.lVal == 1){ //注記フラグが立っていれば
				aResult = true;
				break;
			}
		}
	}
	return aResult;
}
