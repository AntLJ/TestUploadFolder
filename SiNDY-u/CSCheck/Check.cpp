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

// Check.cpp: CCheck クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Check.h"
#include <AddrLib/Initializer.h>

namespace {
	LPCTSTR CITY_SITE_NAME = _T("BGCLASS_C");
	LPCTSTR BASE_SITE_NAME = _T("SC4BGCLASS_C");

	LPCTSTR PURPOSE_ID_NAME = _T("PURPOSE_ID");
	LPCTSTR PNAME_NAME = _T("PURPOSE");

	using namespace sindy::schema;
	const LONG gCityInvalidAttr[] ={
		city_site::bg_class::kSands,                    //砂地
		city_site::bg_class::kMarsh,                    //湿地
		city_site::bg_class::kRiver,                    //河川
		city_site::bg_class::kLake,                     //池・湖
		city_site::bg_class::kPool,                     //プール
		city_site::bg_class::kInterCityTollExpressway,  //都市間高速道（有料）
		city_site::bg_class::kUrbanTollExpressway,      //都市高速（有料）
		city_site::bg_class::kTollRoad,                 //有料道路
		city_site::bg_class::kNationalHighway,          //国道
		city_site::bg_class::kPrincipalPrefecturalRoad, //主要地方道
		city_site::bg_class::kPrefectureNormalRoad,     //一般都道府県道
		city_site::bg_class::kOtherArterialRoad,        //その他幹線道路
		city_site::bg_class::kSideway,                  //歩道
		city_site::bg_class::kGardenPath,               //庭園路
		city_site::bg_class::kCenterDivider,            //中央分離帯
		city_site::bg_class::kDamGuideNotExist,         //ダム（便覧記載なし）
		city_site::bg_class::kDamGuideExist,            //ダム（便覧記載あり）
		city_site::bg_class::kRunway,                   //滑走路
		city_site::bg_class::kPublicRoadStairs,         //公道階段
		city_site::bg_class::kInterCityFreeExpressway,  //都市間高速道（無料）
		city_site::bg_class::kUrbanFreeExpressway,      //都市高速（無料）
		city_site::bg_class::kUnderRepairRoad,          //工事中道路
		city_site::bg_class::kTollGateLane,             //料金所レーン
		-1
	};

	const LONG gBaseInvalidAttr[] ={
		base_site::bg_class::kSea,          //海
		base_site::bg_class::kRiver,        //河川・沢・谷
		base_site::bg_class::kLake,         //湖・池・沼
		base_site::bg_class::kSands,        //砂地
		base_site::bg_class::kMarsh,        //湿地
		base_site::bg_class::kSwimmingPool, //プール
		base_site::bg_class::kDam,          //ダム
		-1
	};
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CCheck::CCheck()
{

}

CCheck::~CCheck()
{

}

bool CCheck::init(IFeatureClassPtr ipAdmClass, IFeatureClassPtr ipGouClass, IFeatureClassPtr ipCSClass, IFeatureClassPtr ipCityClass, IFeatureClassPtr ipCityDividerClass, IFeatureClassPtr ipBaseClass, ITablePtr ipRule, LPCTSTR cJdbConnect)
{
	m_FcAdmin = CFeatureClass(ipAdmClass);
	m_FcGou = CFeatureClass(ipGouClass);
	m_FcCsAddr = CFeatureClass(ipCSClass);
	m_ipCity = ipCityClass;
	m_ipBase = ipBaseClass;
	m_ipCityDivider = ipCityDividerClass;
	m_AdmFld = CSPFieldMap( m_FcAdmin.GetFieldMap() );
	m_AdmName = CSPTableNameString( m_FcAdmin.GetNameString() );
	m_GouFld = CSPFieldMap( m_FcGou.GetFieldMap() );
	m_GouName = CSPTableNameString( m_FcGou.GetNameString() );
	m_CspFld = CSPFieldMap( m_FcCsAddr.GetFieldMap() );
	m_CspName = CSPTableNameString( m_FcCsAddr.GetNameString() );
	m_pGous  = new CGouPoints( m_FcAdmin );
	m_pGous->SetGouPointTable((ITable*)m_FcGou);

	if(!fnInitRule(ipRule)){
		return false;
	}
	m_ipCity->FindField(CComBSTR(CITY_SITE_NAME), &m_CitySiteIndex);
	if(m_CitySiteIndex < 0){
		fprintf(stderr, "#Error,都市地図背景属性フィールドが見つからない。\n");
		return false;
	}

	m_ipBase->FindField(CComBSTR(BASE_SITE_NAME), &m_BaseSiteIndex);
	if(m_BaseSiteIndex < 0){
		fprintf(stderr, "#Error,中縮背景属性フィールドが見つからない。\n");
		return false;
	}

	m_LayerName = m_FcCsAddr.GetOwnerTableName();
	m_GouLayerName = m_FcGou.GetOwnerTableName();
	if(!m_Helper.Connect( cJdbConnect )){
		_ftprintf(stderr, _T("#Error,住所DBの初期化に失敗。\n"));
		return false;
	}
	for(int i = 0; gCityInvalidAttr[i] > 0; i++){
		m_CityAttr.insert(gCityInvalidAttr[i]);
	}
	for(int i = 0; gBaseInvalidAttr[i] > 0; i++){
		m_BaseAttr.insert(gBaseInvalidAttr[i]);
	}
	return true;
}

bool CCheck::execCheck(LPCTSTR cListFile)
{
	bool aIsList;
	//8桁リストアップ
	if(cListFile == NULL){
		//全国
		aIsList = false;
		if(!fnCheckPriorGPAll()){
			return false;
		}
		if(!fnSearch8Code()){
			return false;
		}
	}else{
		//ファイルから
		aIsList = true;
		if(!fnLoadList(cListFile)){
			return false;	
		}
	}
	//8桁単位での処理
	CODE_SET::const_iterator aCode;
	CString aCityCode(_T("00000"));
	for(aCode = m_CodeSet.begin(); aCode != m_CodeSet.end(); aCode++){
		_tprintf(_T("Now %s\n"), static_cast<LPCTSTR>(*aCode));

		// 号ポイントの取得は5桁単位で処理
		CString aTmpCode(*aCode);
		if(aCityCode != aTmpCode.Left(5)) {
			aCityCode = aTmpCode.Left(5);
			m_GouSet.clear();
			//号ポイントの情報を取得
			if(!fnGetGouInfo(aCityCode, aIsList)){
				continue;
			}
			//CheckPriorGP部分
			if(aIsList){
				//全国分の時は一括でやるから必要ない
				fnCheckPriorGP(m_GouSet);
			}
		}

		//従来のチェック
		fnCheckAt8Code(*aCode, aIsList);
		if(_kbhit()){
			int aKey = _getch();
			if(aKey == 'Q'){
				_tprintf(_T("処理は中止されました。\n"));
				_ftprintf(stderr, _T("処理は中止されました。\n"));
				break;
			}
		}
	}
	m_GouSet.clear();
	return true;
}

bool CCheck::fnPointCheck(CCSAddrPoint &cCsp)
{
	LONG aObjID	=cCsp.GetOID();
	ERR_INFO aErrInfo;
	aErrInfo.m_Purpose = fnGetPurpose(cCsp);
//	printf("作業目的 = %s\n", aPurpose);
	aErrInfo.m_AddrCode = cCsp.GetAddrCode();
//	printf("住所コード = %s\n", a20Code);

	TCHAR aInfo[256];
	aErrInfo.m_Source = cCsp.GetStringValueByFieldName(schema::ipc_table::kSource, 256, aInfo);
	if(aErrInfo.m_AddrCode.GetLength() != 20){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】住所コードが20桁でない"), &aErrInfo);
	}
	if(aErrInfo.m_AddrCode.Mid(11, 5) == _T("00000")) {
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】地番コードが「00000」"), &aErrInfo);
	}
	if(fnSameCodeCheck(aObjID, &aErrInfo)){
//		_ftprintf(stderr, _T("0\t%s\t%d\t\t\tError\t0\t同一20桁コードのポイントがある。\t%s\t%s\t%s\n"), static_cast<LPCTSTR>(m_LayerName), aObjID, static_cast<LPCTSTR>(aPurpose), static_cast<LPCTSTR>(a20Code), static_cast<LPCTSTR>(aSource));
	}
	GOU_INFO_SET::const_iterator aSame = m_GouSet.find(GOU_INFO(aErrInfo.m_AddrCode));
	for(;aSame  != m_GouSet.end() && *aSame == aErrInfo.m_AddrCode; aSame++){
		CString aMsg;
		int aErrLv = (aSame->m_PriFlag && !aSame->m_Oaza) ? 0 : 1;
		aMsg.Format(_T("【CSP】同一20桁コードの号ポイント（%s）が存在%s"), aSame->m_PriFlag ? _T("優先") : _T("通常"), aSame->m_Oaza ? _T("（CODE2）") : _T(""));
		fnErrOut(m_LayerName, aObjID, m_GouLayerName, aSame->m_ObjID, aErrLv, aMsg, &aErrInfo);
	}
	CString a11Code = aErrInfo.m_AddrCode.Left(11);
	addrCodeStatus aStatus =  m_Helper.GetCodeStatus(a11Code);
	if(aStatus != addr::kShiko && aStatus != addr::kTmpShiko){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】住所コードが無効"), &aErrInfo);
	}
	if(aErrInfo.m_Source == _T("")){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("【要修正】【CSP】情報ソースに記述がない"), &aErrInfo);
	}
	bool aIsCityArea = false;

	IGeometryPtr ipGeom = cCsp.GetShapeCopy();
	const CCityAdmin *pAdm =  cCsp.GetCityAdmin(m_FcAdmin);
	if(pAdm != NULL){
		CString aAddrCode = pAdm->GetAddrCode();
		city_admin::area_class::ECode aAreaF = pAdm->GetAreaClass();
#ifdef KAJO
		CString aAddr2 = pAdm->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdm->GetConvType();
#endif
		long aFugoCode = pAdm->GetGaikuFugo();
		if(aAreaF == city_admin::area_class::kInArea){
			aIsCityArea = true;
		}
		if(aFugoCode > 0){
			CString aComp16 = aAddrCode;
#if 1	//暫定対応
			if(aComp16.Left(11) == aErrInfo.m_AddrCode.Left(11)){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】行政界（符号界）と住所コード11桁が一致"), &aErrInfo);
#else
			if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("16桁が同じ。"), &aErrInfo);
#endif
			}else{
				CString aComp8 = aComp16.Left(8);
				if(aComp8 != a11Code.Left(8)){
					if(aComp16.Left(5) != a11Code.Left(5)){
						fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【CSP】行政界と住所コード5桁が不一致"), &aErrInfo);
					}else{
						fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("【CSP】行政界と住所コード8桁が不一致"), &aErrInfo);
					}
					//[bug 8248] 対応　加除では本来は通り名フラグで判断しなければならないが、コードで対応
					if(aComp8.Left(3) == _T("261")){
						if(m_Helper.GetAddrKanji(aComp8+ _T("000")) == m_Helper.GetAddrKanji(a11Code.Left(8)+ _T("000"))){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】行政界と住所コード8桁は不一致だが大字名称が一致（京都市）"), &aErrInfo);
						}
					}
					//[bug 8248] 対応　加除では本来は通り名フラグで判断しなければならないが、コードで対応　ここまで
				}else{
#ifdef KAJO
					if(aAddr2 != _T("000000")){
						aComp16 = aAddrCode.Left(5) + aAddr2 + aAddrCode.Mid(11,5);
						if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("【CSP】行政界（CODE2）と住所コード16桁が一致"), &aErrInfo);
						}
					}
#else
					if(aType == city_admin::conv_type::kOazaGeneration){
						aComp16 = aAddrCode.Left(8) + _T("000") + aAddrCode.Mid(11,5);
						if(aComp16 == aErrInfo.m_AddrCode.Left(16)){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("16桁が同じ(大字も生成)。"), &aErrInfo);
						}
					}
#endif
				}
			}
		}else{
			CString aComp11 = aAddrCode;
			if(aComp11 == a11Code){
				fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】行政界と住所コード11桁が一致"), &aErrInfo);
			}else{
				CString aComp8 = aComp11.Left(8);
				if(aComp8 != a11Code.Left(8)){
					if(aComp11.Left(5) != a11Code.Left(5)){
						fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【CSP】行政界と住所コード5桁が不一致"), &aErrInfo);
					}else{
						fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("【CSP】行政界と住所コード8桁が不一致"), &aErrInfo);
					}
					//[bug 8248] 対応　加除では本来は通り名フラグで判断しなければならないが、コードで対応
					if(aComp8.Left(3) == _T("261")){
#ifndef _DEBUG
						CString aCmpName = m_Helper.GetAddrKanji(aComp8+ _T("000"));
						CString aOrgName = m_Helper.GetAddrKanji(a11Code.Left(8) + _T("000"));
						if(aCmpName == aOrgName){
#else
						if(m_Helper.GetAddrKanji(aComp8+ _T("000")) == m_Helper.GetAddrKanji(a11Code.Left(8)+ _T("000"))){
#endif
							fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】行政界と住所コード8桁は不一致だが大字名称が一致（京都市）"), &aErrInfo);
						}
					}
					//[bug 8248] 対応　加除では本来は通り名フラグで判断しなければならないが、コードで対応　ここまで
				}else{
#ifdef KAJO
					if(aAddr2 != _T("000000")){
						aComp11 = aAddrCode.Left(5) + aAddr2;
						if(aComp11 == a11Code){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("【CSP】行政界（CODE2）と住所コード11桁が一致"), &aErrInfo);
						}
					}
#else
					if(aType == city_admin::conv_type::kOazaGeneration){
						aComp11 = aAddrCode.Left(8) + _T("000");
						if(aComp11 == a11Code){
							fnErrOut(m_LayerName, aObjID, NULL, -1, 1, _T("11桁が同じ(大字も生成)。"), &aErrInfo);
						}
					}
#endif
				}
			}
		}
	}else{
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【要修正】【CSP】行政界ポリゴンの取得に失敗"), &aErrInfo);
	}
	if(aIsCityArea && fnCitySiteCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【CSP】不正な種別の都市地図背景ポリゴン上に存在"), &aErrInfo);
	}
	if(aIsCityArea && !fnCityDividerCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【CSP】都市料金所分離帯ポリゴン上に存在"), &aErrInfo);
	}
	if(fnBaseSiteCheck(ipGeom, aObjID)){
		fnErrOut(m_LayerName, aObjID, NULL, -1, 0, _T("【CSP】不正な種別の中縮背景ポリゴン上に存在"), &aErrInfo);
	}
	return true;
}

CString CCheck::fnGetPurpose(CCSAddrPoint &cCsp)
{
	CString aName;
	long aPID = cCsp.GetLongValueByFieldName(schema::ipc_table::kPurpose, 0);
	CString aWhere;
	if(aPID != 0){
		PURPOSE_MAP::iterator aPurp = m_PurposeMap.find(aPID);
		if(aPurp != m_PurposeMap.end()){
			aName = aPurp->second;
		}else{
			fprintf(stderr, "#Error,不正な作業目的,%d\n", aPID);
		}
	}else{
		TCHAR aBuf[32];
		aName = cCsp.GetStringValueByFieldName(schema::ipc_table::kModifyProgName, 32, aBuf);
	}
	return aName;
}

bool CCheck::fnSameCodeCheck(LONG cObjID, const ERR_INFO* cpErrInfo)
{
	CS_CODE_MAP::const_iterator it = m_CsMap20.find(cpErrInfo->m_AddrCode);
	for(; it != m_CsMap20.end() && it->first == cpErrInfo->m_AddrCode; it++){
		if(it->second != cObjID){
			fnErrOut(m_LayerName, cObjID, m_LayerName, it->second, 0, _T("【要修正】【CSP】同一20桁コードのCSポイントが存在"), cpErrInfo);
		}
	}
	m_CsMap20.erase(cpErrInfo->m_AddrCode);
	return true;
}

bool CCheck::fnCitySiteCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipCity->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipCity->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("【CSP】都市地図背景ポリゴンの取得に失敗"), NULL);
		return false;
	}
	bool aIsInvalid = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAttr;
		ipFeature->get_Value(m_CitySiteIndex, &aAttr);
		if(fnIsInvalidCityAttr(aAttr.lVal)){
			return true;
		}
		aIsInvalid = false;
	}
	return aIsInvalid;
}

bool CCheck::fnCityDividerCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipCityDivider->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	long count = 0;
	if(m_ipCityDivider->FeatureCount(ipFilter, &count) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("【CSP】都市料金所分離帯ポリゴンの取得に失敗"), NULL);
		return true; // エラー内容が違うため、失敗しているがtrueを返す
	}
	// CSPointがCITY_DIVIDER上にある場合はエラー
	if(count > 0)
		return false;

	return true;
}

bool CCheck::fnBaseSiteCheck(IGeometryPtr ipGeom, LONG cObjID)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipBase->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );

	IFeatureCursorPtr ipCursor;
	if(m_ipBase->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fnErrOut(m_LayerName, cObjID, NULL, -1, 0, _T("【CSP】中縮背景ポリゴンの取得に失敗"), NULL);
		return false;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAttr;
		ipFeature->get_Value(m_BaseSiteIndex, &aAttr);
		if(fnIsInvalidBaseAttr(aAttr.lVal)){
			return true;
		}
	}
	return false;
}

bool CCheck::fnIsInvalidCityAttr(LONG cAttr)
{
#if 0
	for(int i = 0; gCityInvalidAttr[i] >= 0; i++){
		if(gCityInvalidAttr[i] == cAttr){
			return true;
		}
	}
	return false;
#else
	if(m_CityAttr.find(cAttr) != m_CityAttr.end()){
		return true;
	}
	return false;
#endif
}

bool CCheck::fnIsInvalidBaseAttr(LONG cAttr)
{
#if 0
	for(int i = 0; gBaseInvalidAttr[i] >= 0; i++){
		if(gBaseInvalidAttr[i] == cAttr){
			return true;
		}
	}
	return false;
#else
	if(m_BaseAttr.find(cAttr) != m_BaseAttr.end()){
		return true;
	}
	return false;
#endif
}

bool CCheck::fnLoadList(LPCTSTR cListFile)
{
	FILE *pFile;
	if((pFile = _tfopen(cListFile, _T("rt"))) != NULL){
		TCHAR aBuf[32];
		while(_fgetts(aBuf, 32, pFile) != NULL){
			if(aBuf[0] != '#'){
				CString aCode = aBuf;
				aCode.Replace(_T("\n"), _T(""));
				m_CodeSet.insert(aCode);
			}
		}
		fclose(pFile);
	}else{
		_ftprintf(stderr, _T("#Error,リストファイルが開けません。,%s\n"), cListFile);
		return false;
	}
	return true;
}

bool CCheck::fnGetGouInfo(LPCTSTR cCityCode, bool cIsList)
{
	// 指定住所コードを持つ行政界を検索
	m_pGous->SelectByAddrCode( cCityCode );
	// 各行政界内にある号ポイント取得・住所コード出力
	for( CCityAdmin::const_rows_iterator itAdmin = m_pGous->begin(m_pGous->GetTableType()); itAdmin != m_pGous->end(m_pGous->GetTableType()); ++itAdmin )
	{
		// 行政界内にある号ポイント取得
		const CSPCityAdmin pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
#ifdef KAJO
		CString aAddr2 = pAdmin->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdmin->GetConvType();
#endif
		for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
		{
			const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itGou);
			GOU_INFO aInfo;
			aInfo.m_20Code = pGou->GetAddrCode();
			aInfo.m_ObjID = pGou->GetOID();
			aInfo.m_PriFlag = pGou->IsPrior();
			if(aInfo.m_20Code == _T("")){
				if(cIsList && aInfo.m_PriFlag){
					fnErrOut(m_GouLayerName, aInfo.m_ObjID, NULL, -1, 0, _T("【要修正】【GOU】号ポイント（優先）の20桁化に失敗"), NULL);
				}
				continue;
			}
#ifdef _DEBUG
			if(aInfo.m_20Code == _T("01101002009000030012")){
				int a = 0;
			}
#endif
			aInfo.m_Oaza = false;
			m_GouSet.insert(aInfo);
#ifdef KAJO
			if(aAddr2 != _T("000000")){
				CString aTmpCpde = aInfo.m_20Code;
				aInfo.m_20Code = aTmpCpde.Left(5) + aAddr2 + aTmpCpde.Mid(11, 9);
				aInfo.m_Oaza = true;
				m_GouSet.insert(aInfo);
			}
#else
			if(aType == city_admin::conv_type::kOazaGeneration){
				CString aTmpCpde = aInfo.m_20Code;
				aInfo.m_20Code = aTmpCpde.Left(8) + _T("000") + aTmpCpde.Mid(11, 9);
				aInfo.m_Oaza = true;
				m_GouSet.insert(aInfo);
			}
#endif
		}
	}
	return true;
}

bool CCheck::fnCheckAt8Code(LPCTSTR cAddrCode, bool cNeedLoad)
{
	if(cNeedLoad){
		//リストから読み込み
		IQueryFilterPtr ipFilter(CLSID_QueryFilter);
		CString aWhere;
		aWhere.Format(_T("%s LIKE '%s%%'"), cs_addr_point::kAddrCode, cAddrCode);
		ipFilter->put_WhereClause(CComBSTR(aWhere));
		_ICursorPtr ipCursor = m_FcCsAddr.Search(ipFilter, VARIANT_FALSE);
		if(ipCursor == NULL){
			return false;
		}
		_IRowPtr ipRow;
		m_CsMap.clear();
		m_CsMap20.clear();
		while(ipCursor->NextRow(&ipRow) == S_OK){
			CCSAddrPoint aCsp(ipRow, sindyTableType::cs_addr_point, false, m_CspFld, m_CspName);
			CString aCode = aCsp.GetAddrCode();
			m_CsMap.insert(CS_MAP::value_type(aCode.Left(8), aCsp));
			m_CsMap20.insert(CS_CODE_MAP::value_type(aCode, aCsp.GetOID()));
		}
	}
	CS_MAP::iterator aRec = m_CsMap.find(cAddrCode);
	for(; aRec != m_CsMap.end() && aRec->first == cAddrCode; aRec++){
		fnPointCheck(aRec->second);
	}
	return true;
}

bool CCheck::fnSearch8Code(void)
{
	_tprintf(_T("CSポイント全国分の8桁コードを取得しています。・・・"));
	//CSから全国分の8桁取得
	_ICursorPtr ipCursor = m_FcCsAddr.Search(NULL, VARIANT_FALSE);
	if(ipCursor == NULL){
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CCSAddrPoint aCsp(ipRow, sindyTableType::cs_addr_point, false, m_CspFld, m_CspName);
		CString aCode = aCsp.GetAddrCode();
		if(aCode == _T("")){
			fnErrOut(m_LayerName, aCsp.GetOID(), NULL, -1, 0, _T("【要修正】【CSP】住所コード未設定"), NULL);
			continue;
		}
		m_CodeSet.insert(aCode.Left(8));
		m_CsMap.insert(CS_MAP::value_type(aCode.Left(8), aCsp));
		m_CsMap20.insert(CS_CODE_MAP::value_type(aCode, aCsp.GetOID()));
	}
	_tprintf(_T("終了\n"));
	return true;
}

bool CCheck::fnCheckPriorGP(GOU_INFO_SET &cGouSet)
{
	CString aCurrentCode = _T("");
//	long aCurrentID = 0;
	GOU_INFO_SET::const_iterator aOrg = cGouSet.begin();
	for(GOU_INFO_SET::const_iterator aCmp = cGouSet.begin(); aCmp != cGouSet.end(); aCmp++){
		if(aCmp->m_PriFlag){
			if(aCurrentCode == aCmp->m_20Code){
				ERR_INFO aErrInfo;
				aErrInfo.m_AddrCode = aCurrentCode;
				if(aOrg->m_Oaza || aCmp->m_Oaza){
#ifdef KAJO
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("【GOU】同一20桁コードの号ポイント（優先）が存在（CODE2）"), &aErrInfo);
#else
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("同一20桁コードの優先号ポイントがある(大字も生成)。"), &aErrInfo);
#endif
				}else{
					fnErrOut(m_GouLayerName, aOrg->m_ObjID, m_GouLayerName, aCmp->m_ObjID, 0, _T("【要修正】【GOU】同一20桁コードの号ポイント（優先）が存在"), &aErrInfo);
				}
			}else{
				aCurrentCode = aCmp->m_20Code;
//				aCurrentID = aCmp->m_ObjID;
				aOrg = aCmp;
			}
		}
	}
	return true;
}

bool CCheck::fnInitRule(ITablePtr ipRule)
{
	long aPNameIndex, aPIDIndex;
	ipRule->FindField(CComBSTR(PNAME_NAME), &aPNameIndex);
	if(aPNameIndex < 0){
		fprintf(stderr, "#Error,ルールの作業目的名称フィールドが見つからない。\n");
		return false;
	}
	
	ipRule->FindField(CComBSTR(PURPOSE_ID_NAME), &aPIDIndex);
	if(aPIDIndex < 0){
		fprintf(stderr, "#Error,ルールの作業目的IDフィールドが見つからない。\n");
		return false;
	}
	_ICursorPtr ipCursor;
	if(ipRule->Search(NULL, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "#Error,作業目的検索失敗\n");
		return false;
	}
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CComVariant aPName, aPID;
		ipRow->get_Value(aPNameIndex, &aPName);
		ipRow->get_Value(aPIDIndex, &aPID);
		m_PurposeMap.insert(PURPOSE_MAP::value_type(aPID.lVal, CString(aPName.bstrVal)));
	}
	return true;
}

bool CCheck::fnCheckPriorGPAll(void)
{
	_tprintf(_T("優先フラグ付きの号を検査しています。・・・"));
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format(_T("%s = 1"), schema::gou_point::kPriority);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	_ICursorPtr ipCursor = m_FcGou.Search(ipFilter, VARIANT_FALSE);
	if(ipCursor == NULL){
		return false;
	}
	GOU_INFO_SET aPriSet;
	_IRowPtr ipRow;
	while(ipCursor->NextRow(&ipRow) == S_OK){
		CGouPoint cGou(ipRow,sindyTableType::gou_point,false, m_GouFld, m_GouName );
		const CCityAdmin *pAdm = cGou.GetCityAdmin(m_FcAdmin);
#ifdef KAJO
		CString aAddr2 = pAdm->GetOazaAzaCode2();
#else
		city_admin::conv_type::ECode aType= pAdm->GetConvType();
#endif
		GOU_INFO aInfo;
		aInfo.m_20Code = cGou.GetAddrCode();
		if(aInfo.m_20Code == _T("")){
			fnErrOut(m_GouLayerName, cGou.GetOID(), NULL, -1, 0, _T("【要修正】【GOU】号ポイント（優先）の20桁化に失敗"), NULL);
			continue;
		}
		aInfo.m_ObjID = cGou.GetOID();
		aInfo.m_PriFlag = true;
		aInfo.m_Oaza = false;
		aPriSet.insert(aInfo);
#ifdef KAJO
		if(aAddr2 != _T("000000")){
			CString aTmpCpde = aInfo.m_20Code;
			aInfo.m_20Code = aTmpCpde.Left(5) + aAddr2 + aTmpCpde.Mid(11, 9);
			aInfo.m_Oaza = true;
			aPriSet.insert(aInfo);
		}
#else
		if(aType == city_admin::conv_type::kOazaGeneration){
			CString aTmpCpde = aInfo.m_20Code;
			aInfo.m_20Code = aTmpCpde.Left(8) + _T("000") + aTmpCpde.Mid(11, 9);
			aInfo.m_Oaza = true;
			aPriSet.insert(aInfo);
		}
#endif
#ifdef _DEBUG
	_tprintf(_T("サイズ=%d\n"), aPriSet.size());
#endif
	}
#ifdef _DEBUG
	_tprintf(_T("サイズ=%d\n"), aPriSet.size());
#endif
	bool aResult = fnCheckPriorGP(aPriSet);
	_tprintf(_T("終了\n"));
	return aResult;
}

void CCheck::fnErrOut(LPCTSTR cLayer1, long cObjID1, LPCTSTR cLayer2, long cObjID2, int cLevel, LPCTSTR cErrMsg, const ERR_INFO* cpErrInfo)
{
	static LPCTSTR aMsg[] = {_T("ERROR"), _T("WARNING")};
	_ftprintf(stderr, _T("0\t%s\t%d\t"), cLayer1, cObjID1);
	if(cLayer2 != NULL){
		_ftprintf(stderr, _T("%s\t%d\t"), cLayer2, cObjID2);
	}else{
		_ftprintf(stderr, _T("\t\t"));
	}
	_ftprintf(stderr, _T("%s\t%s\t"), aMsg[cLevel], cErrMsg);
	if(cpErrInfo != NULL){
		_ftprintf(stderr, _T("%s\t%s\t%s\t\n"), static_cast<LPCTSTR>(cpErrInfo->m_Purpose), static_cast<LPCTSTR>(cpErrInfo->m_AddrCode), static_cast<LPCTSTR>(cpErrInfo->m_Source));
	}else{
		_ftprintf(stderr, _T("\t\t\t\n"));
	}
}
