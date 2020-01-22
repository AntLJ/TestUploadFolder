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
#include "CHECK.h"
#include <math.h>
#include <iomanip>

//小数点以下の表示桁数
#define  DECIMAL 15

CCHECK::CCHECK(void)
{
}

CCHECK::~CCHECK(void)
{
}

bool CCHECK::CheckLinkSHP(std::map<std::pair<long, long>, WKSPoint> & rNodeSet, std::vector<LINK> & rLinkSet,long rSecondMesh, bool bWalkOpt)
{
	std::vector<LINK>::iterator Linkitr = rLinkSet.begin();
	std::map<std::pair<long, long>, WKSPoint>::iterator Nodeitr;

	bool aCheck = true;

	while(Linkitr != rLinkSet.end())
	{
		long aID,FID;								//aID:FROMNODEID,TONODEID格納用 OID:リンクのOID
		long aDivID = -1;							//aDivID:FROMDIVID,TODIVID格納用。初期値-1
		WKSPoint aFFirstPoint,aFEndPoint;			//aFFirstPoint:リンクレイヤから得た始点座標格納用,aFEndPoint:リンクレイヤから得た終点座標格納用
		WKSPoint aTFirstPoint,aTEndPoint;			//aTFirstPoint:ノードレイヤから得た始点座標検証用の座標格納用,aTEndPoint：ノードレイヤから得た終点座標検証用の座標格納用
		double aFirstDeltaX=0,aFirstDeltaY=0;		//リンクレイヤ始点座標とFromNodeIDを持つノードレイヤの座標の誤差格納用
		double aEndDeltaX=0,aEndDeltaY=0;			//リンクレイヤ終点座標とToNodeIDを持つノードレイヤの座標の誤差格納用
		bool aFirstCheck=true;                      //エラーの種類を特定するための変数
		bool aEndCheck = true;                      //エラーの種類を特定するための変数
		std::pair<long, long> pairID;

		//リンクのFID
		FID = Linkitr->FID;

		//始点のチェック
		aID = Linkitr->FromNodeID;
		aFFirstPoint = Linkitr->FirstPoint;
		
		if(bWalkOpt){
			aDivID = Linkitr->FromDivID;
		}
		pairID.first = aID;
		pairID.second = aDivID;

		//FromNodeIDを持つノードの座標をノードレイヤから検索
		Nodeitr = rNodeSet.find(pairID);

		if(Nodeitr == rNodeSet.end()){
			std::cout << "error\t" << rSecondMesh << "\t" << "FID:" << FID << "のリンクの始点ノード("  << "OBJECTID:" << aID;
			if(bWalkOpt){
				std::cout << ", DIVID:" << aDivID;
			}
			std::cout << ")は存在しません" << std::endl;
			aCheck = false;
		}
		else{
			aTFirstPoint = Nodeitr->second;
			//リンクレイヤの始点座標と、FromNodeIDを持つノードレイヤの座標を比較
			if((aFFirstPoint.X != aTFirstPoint.X )|| (aFFirstPoint.Y != aTFirstPoint.Y)){

				aFirstDeltaX= fabs(aFFirstPoint.X-aTFirstPoint.X);
				aFirstDeltaY= fabs(aFFirstPoint.Y-aTFirstPoint.Y);

				if((aFirstDeltaX != 0) || (aFirstDeltaY != 0)){

					//error
					aFirstCheck = false;
					aCheck = false;
				}
			}
		}

		//終点のチェック

		aID = Linkitr->ToNodeID;
		aFEndPoint = Linkitr->EndPoint;

		if(bWalkOpt){
			aDivID = Linkitr->ToDivID;
		}
		pairID.first = aID;
		pairID.second = aDivID;

		//ToNodeIDを持つノードの座標をノードレイヤから検索
		Nodeitr = rNodeSet.find(pairID);
		aTEndPoint = Nodeitr->second;

		if(Nodeitr == rNodeSet.end()){
			std::cout << "error\t" << rSecondMesh << "\t" << "FID:" << FID << "のリンクの終点ノード("  << "OBJECTID:" << aID;
			if(bWalkOpt){
				std::cout << ", DIVID:" << aDivID;
			}
			std::cout << ")は存在しません" << std::endl;
			aCheck=false;
		}
		else{
			//リンクレイヤの終点座標と、ToNodeIDを持つノードレイヤの座標を比較
			if((aFEndPoint.X != aTEndPoint.X) ||(aFEndPoint.Y != aTEndPoint.Y)){

				aEndDeltaX= fabs(aFEndPoint.X-aTEndPoint.X);
				aEndDeltaY= fabs(aFEndPoint.Y-aTEndPoint.Y);

				if((aEndDeltaX != 0) || (aEndDeltaY != 0)){

					//error
					aEndCheck = false;
					aCheck = false;
				}
			}
		}

		//エラーの種類を出力
		if((aFirstCheck == false) && (aEndCheck == false)){
			if((aFFirstPoint.X == aTEndPoint.X) && (aFFirstPoint.Y==aTEndPoint.Y)&&
				(aFEndPoint.X == aTFirstPoint.X)&&(aFEndPoint.Y == aTFirstPoint.Y)){
					std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\tリンクの向きが逆です"<<std::endl;
			}
			else{
				std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\t起終点の位置に差違があります"<<std::endl;
				std::cout << "\t\t" << "起点座標の差違(" << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaY << ")" << std::endl;
				std::cout << "\t\t" << "終点座標の差違(" << std::fixed << std::setprecision(DECIMAL) << aEndDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aEndDeltaY << ")" << std::endl;
			}
		}
		else if(aFirstCheck == false){
			std::cout << "error\t" << rSecondMesh << "\tFID:" << FID <<"\t起点の位置に差違があります"<<std::endl;
			std::cout << "\t\t" << "起点座標の差違(" << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aFirstDeltaY << ")" << std::endl;
		}
		else if(aEndCheck == false){
			std::cout<< "error\t" << rSecondMesh << "\tFID:" << FID <<"\t終点の位置に差違があります"<<std::endl;
			std::cout << "\t\t" << "終点座標の差違(" << std::fixed << std::setprecision(DECIMAL) << aEndDeltaX << "," << std::fixed << std::setprecision(DECIMAL) << aEndDeltaY << ")" << std::endl;
		}
		Linkitr++;
	}
	return aCheck;
}
