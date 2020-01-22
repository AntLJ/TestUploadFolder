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
#include "RLK.h"
#include <atl2/io.h>

CRLK::CRLK(IFeatureClassPtr ipFeatureClass)
{
    m_ipFeatureClass = ipFeatureClass;

}

CRLK::~CRLK(void)
{
}

bool CRLK::GetInfo(std::vector<LINK> & rLinkSet, std::wstring rLinkFileName, bool bWalkOpt)
{
	HRESULT hRetObjct, hRetFrom, hRetTo;

	//OBJECTIDﾌｨｰﾙﾄﾞ、FROMNODEIDﾌｨｰﾙﾄﾞ、TONODEIDﾌｨｰﾙﾄﾞのｲﾝﾃﾞｯｸｽ取得
	long lObjectIDIndex, lFromNodeIDIndex, lToNodeIDIndex;
	hRetObjct = m_ipFeatureClass->FindField(_T("OBJECTID"), &lObjectIDIndex);
	hRetFrom = m_ipFeatureClass->FindField(_T("FROMNODEID"), &lFromNodeIDIndex);
	hRetTo = m_ipFeatureClass->FindField(_T("ToNodeID"), &lToNodeIDIndex);
	if(FAILED(hRetObjct) || (-1 == lObjectIDIndex) ||
	   FAILED(hRetFrom) || (-1 == lFromNodeIDIndex) ||
	   FAILED(hRetTo) || (-1 == lToNodeIDIndex)){
		std::cerr << rLinkFileName << ":フィールドインデックスの取得に失敗しました"<<std::endl;
		return false;
	}

	//FROMDIVIDﾌｨｰﾙﾄﾞ、TODIVIDﾌｨｰﾙﾄﾞのｲﾝﾃﾞｯｸｽ取得
	long lFromDivIDIndex, lToDivIDIndex;
	if(bWalkOpt){
		hRetFrom = m_ipFeatureClass->FindField(_T("FROMDIVID"), &lFromDivIDIndex);
		hRetTo = m_ipFeatureClass->FindField(_T("TODIVID"), &lToDivIDIndex);
		if(FAILED(hRetFrom) || (-1 == lFromDivIDIndex) ||
		   FAILED(hRetTo) || (-1 == lToDivIDIndex)){
			std::cerr << rLinkFileName << ":フィールドインデックスの取得に失敗しました"<<std::endl;
			return false;
		}
	}

	//ﾌｨｰﾁｬｰｶｰｿﾙの取得
	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipFeatureClass->Search(NULL,VARIANT_FALSE, &ipFeatureCursor))){

		std::cerr << rLinkFileName <<":ﾌｨｰﾁｬｰｶｰｿﾙの取得に失敗しました。"<<std::endl;
		return false;
	}

	//ﾌｨｰﾁｬｰｶｰｿﾙが尽きるまでﾌｨｰﾁｬｰを巡回
	IFeaturePtr ipFeature;
	while (ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature!=NULL)){
		long lOID = ipFeature->GetOID();
		LINK LinkInfo;

		LinkInfo.FID = lOID;

		CComVariant vaObjectID,vaFromNodeID,vaToNodeID;
		//OBJECTIDﾌｨｰﾙﾄﾞの値を取得
		if(FAILED(ipFeature->get_Value(lObjectIDIndex, &vaObjectID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID <<"\tOBJECTIDの取得に失敗"<<std::endl;
			continue;
		}
		else{
			LinkInfo.ObjectID = vaObjectID.lVal;
		}
		//FROMNODEIDﾌｨｰﾙﾄﾞの値の取得
		if(FAILED(ipFeature->get_Value(lFromNodeIDIndex, &vaFromNodeID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\tFROMNODEIDの取得に失敗"<<std::endl;
			continue;
		}
		else{
			LinkInfo.FromNodeID = vaFromNodeID.lVal;
		}
		//TONODEIDﾌｨｰﾙﾄﾞの値の取得
		if(FAILED(ipFeature->get_Value(lToNodeIDIndex, &vaToNodeID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID << "\tTONODEIDの取得に失敗"<<std::endl;
			continue;
		}
		else{
			LinkInfo.ToNodeID = vaToNodeID.lVal;
		}

		LinkInfo.FromDivID = -1;		//初期値設定(RLKの場合は-1固定とする)
		LinkInfo.ToDivID = -1;			//初期値設定(RLKの場合は-1固定とする)
		if(bWalkOpt){
			CComVariant vaFromDivID,vaToDivID;
			//FROMDIVIDﾌｨｰﾙﾄﾞの値の取得
			if(FAILED(ipFeature->get_Value(lFromDivIDIndex, &vaFromDivID))){
				std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\tFROMDIVIDの取得に失敗"<<std::endl;
				continue;
			}
			else{
				LinkInfo.FromDivID = vaFromDivID.lVal;
			}
			//TODIVIDﾌｨｰﾙﾄﾞの値の取得
			if(FAILED(ipFeature->get_Value(lToDivIDIndex, &vaToDivID))){
				std::cerr<< rLinkFileName << "\tFID:" << lOID << "\tTODIVIDの取得に失敗"<<std::endl;
				continue;
			}
			else{
				LinkInfo.ToDivID = vaToDivID.lVal;
			}
		}

		//形状を取得
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeometry))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\t形状の取得に失敗"<<std::endl;
			continue;
		}
		VARIANT_BOOL check;
		ipGeometry->get_IsEmpty(&check);
		if(check){
			std::cout << "error\t" << rLinkFileName << "\tFID:" << lOID <<"\t形状が存在しません。" << std::endl;
			continue;
		}

		//形状を点列に変換
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long lPointCount;
		if(FAILED(ipPointCollection->get_PointCount(& lPointCount))){
			std::cerr << rLinkFileName << "\tFID:" << lOID <<"\t構成点数取得に失敗"<<std::endl;
			continue;
		}

		//構成点数が2未満であれば
		if(lPointCount < 2){
			std::cout << "error" << rLinkFileName << "\tFID:" << lOID << "\t構成点数が2未満です."<<std::endl;
			continue;
		}

		//始点座標取得
		IPointPtr ipPoint;
		if(FAILED(ipPointCollection->get_Point(0,&ipPoint)
			||ipPoint->get_X(&LinkInfo.FirstPoint.X)
			||ipPoint->get_Y(&LinkInfo.FirstPoint.Y))){
			std::cerr << rLinkFileName << "\tFID:" << lOID << "\t始点の座標取得に失敗"<<std::endl;
			continue;
		}

		//終点座標取得
		if(FAILED(ipPointCollection->get_Point(lPointCount-1,&ipPoint)
			||ipPoint->get_X(&LinkInfo.EndPoint.X)
			||ipPoint->get_Y(&LinkInfo.EndPoint.Y))){
				std::cerr <<  rLinkFileName << "\tFID:" << lOID <<"\t終点の座標取得に失敗"<<std::endl;
				continue;
		}

		//LinkInfoをvectorコンテナに格納
		rLinkSet.push_back(LinkInfo);
	}
	return true;
  }


