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

// FacilPoint.cpp: CFacilPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include "FacilPoint.h"
#include "crd_cnv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFacilPoint::CFacilPoint():eMeshcode(0),eType(0),eLon(0),eLat(0),eKanji(""),eCode("")
{}
CFacilPoint::CFacilPoint(const CFacilPoint& cPoint) :
	eType(cPoint.eType),
	eMeshcode(cPoint.eMeshcode),
	eLon(cPoint.eLon),
	eLat(cPoint.eLat),
	eKanji(cPoint.eKanji),
	eCode(cPoint.eCode),
	eDFacilPs(cPoint.eDFacilPs),
	eDRoadPs(cPoint.eDRoadPs),
	eX(cPoint.eX),
	eY(cPoint.eY),
	eDistance(0),
	eAccessFind(0)
{}
CFacilPoint::~CFacilPoint()
{}

/**
 * 施設情報ポイント種別の登録
 * @note 施設情報ポイント種別の登録を行います。
 * @param  cVal		[in]	施設情報種別コード値
 * @return なし
 */
void	CFacilPoint::setType(int cVal){
	eType=cVal;
}
/**
 * 施設情報ポイントの正規化座標を計算
 * @note 施設情報ポイントの正規化座標を計算を行います。
 * @param  なし
 * @return なし
 */
void	CFacilPoint::setFacilPointXY(void)
{
	CMeshPos	aMP;
	aMP.LL256toLL(eLon,eLat,&eDFacilPs.eX,&eDFacilPs.eY);
	aMP.LLtoMesh(eDFacilPs.eX,	//緯度
				 eDFacilPs.eY,	//経度
				 2,				//メッシュコードレベル
				 &eMeshcode,	//メッシュコード
				 &eX, &eY);		//正規化座標
}
/**
 * 施設情報ポイントの正規化座標を計算
 * @note 施設情報ポイントの正規化座標を計算を行います。
 * @param  なし
 * @return なし
 */
/*
void	CFacilPoint::setRoadPointXY(int aType,double cLon,double cLat)
{
	double	xmin=.0,xmax=.0,ymin=.0,ymax=.0; 
	switch( aType ){
		case 1:	//最近傍道路
			eDRoadPs.eX = cLon;
			eDRoadPs.eY = cLat;

			//水系の交差判定ロジック修正	2003/10/7
			if( getFacilDY()<getRoadDY()){
				xmin=getFacilDX();	ymin=getFacilDY();
				xmax=getRoadDX();	ymax=getRoadDY();
			}
			else{
				xmin=getRoadDX();	ymin=getRoadDY();
				xmax=getFacilDX();	ymax=getFacilDY();
			}

			//外接四角形の登録
			setCrossArea(xmin,ymin,xmax,ymax);
			break;
		case 2:	//最近傍道路の次に近い道路
			eDRoadPs2.eX = cLon;
			eDRoadPs2.eY = cLat;
			if( getFacilDX()<getRoad2DX()){	xmin=getFacilDX();	xmax=getRoad2DX();}
			else{							xmin=getRoad2DX();	xmax=getFacilDX();}
			if( getFacilDY()<getRoad2DY()){	ymin=getFacilDY();	ymax=getRoad2DY();}
			else{							ymin=getRoad2DY();	ymax=getFacilDY();}
			//外接四角形の登録
			setCrossArea2(xmin,ymin,xmax,ymax);
			break;
		default:
			break;
	}
#ifdef	_DEBUG_
	int	aMeshcode1=0,aX1=0,aY1=0;
	int	aMeshcode2=0,aX2=0,aY2=0;
	CMeshPos	aMP;
	aMP.LLtoMesh(xmin,ymin,
				 2,				//メッシュコードレベル
				 &aMeshcode1,	//メッシュコード
				 &aX1, &aY1);		//正規化座標
	aMP.LLtoMesh(xmax,ymax,
				 2,				//メッシュコードレベル
				 &aMeshcode2,	//メッシュコード
				 &aX2, &aY2);	//正規化座標
//	ATLTRACE(TEXT("交差判定( %d )：[%d,%d-%d - %d,%d-%d]\n"),aType,
//							aMeshcode1,aX1,aY1,aMeshcode2,aX2,aY2);
#endif
}
*/
/**
 * 施設情報ポイント経緯度の登録
 * @note 施設情報ポイント経緯度の登録を行います。
 * @param  cVal		[in]	施設情報種別コード値
 * @return なし
 */
void	CFacilPoint::setLon(long cVal)
{
	eLon=cVal;
}
/**
 * 施設情報ポイント経緯度の登録
 * @note 施設情報ポイント経緯度の登録を行います。
 * @param  cVal		[in]	施設情報種別コード値
 * @return なし
 */
void	CFacilPoint::setLat(long cVal)
{
	eLat=cVal;
}
// = 演算子
CFacilPoint& CFacilPoint::operator = (const CFacilPoint& cPoint)
{
        eType  = cPoint.eType;
		eLon   = cPoint.eLon;
		eLat   = cPoint.eLat;
        eKanji = cPoint.eKanji;
        eCode  = cPoint.eCode;
		eX     = cPoint.eX;
		eY     = cPoint.eY;
		eMeshcode = cPoint.eMeshcode;
        return(*this);
}

// 比較演算子
bool CFacilPoint::operator == (const CFacilPoint& cPoint) const
{       
        if(eKanji != cPoint.eKanji) return(false);
		if(eLon != cPoint.eLon) return(false);
		if(eLat != cPoint.eLat) return(false);
        return(eCode == cPoint.eCode);
}
bool CFacilPoint::operator < (const CFacilPoint& cPoint) const
{
        if(eLon < cPoint.eLon) return(true);
        if(eLon > cPoint.eLon) return(false);
        if(eLat < cPoint.eLat) return(true);
        if(eLat > cPoint.eLat) return(false);
        return(eKanji < cPoint.eKanji);
}
