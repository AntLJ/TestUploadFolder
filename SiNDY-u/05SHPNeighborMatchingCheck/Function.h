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

#pragma once
#include <set>
#include <map>
#include "crd_cnv.h"

namespace Function{
	//string → wstring
	std::wstring filestr(const char* str);
	//隣接メッシュの取得middle用
	long GetMeshCode(long aMeshCode, long x,long y,std::string aDataSet);
}



//IDの名前とインデックス格納用構造体
struct ID{
	std::string e_Name;
	long e_Index;
};


//ID格納用構造体
struct IDs{
	ID e_ID;
	ID e_DIVID;
};


namespace ArcFunction{
	//ワークスペース取得～フィーチャクラス取得
	bool GetFeatureClass(IFeatureClassPtr &ipFeatureClass,std::string aSHPPath,std::string aDataSet);
	//SHP(TKY)のデータを取得する関数
	bool GetTKYInfo(const Parameters &rParameters, TKYMeshData *pTKYMeshData, long aMeshCode,/*IFeatureClassPtr &ipMeshClass,*/std::string aLayerType,std::map<long,FID_Set> *pOIDSet, bool *pCheck);
	//SHP(JGD)のデータを取得する関数
	bool GetJGDInfo(const Parameters &rParameters, JGDMeshData *pJGDMeshData, long aMeshCode,IFeatureClassPtr &ipMeshClass,std::string aLayerType,std::map<long,FID_Set> &rOIDSet,bool *pCheck);
	//SHPファイルのワークスペース取得～フィーチャクラス取得
	bool GetSHPFeatureClass(IFeatureClassPtr &ipFeatureClass,IDs *pIDs, std::string aOrgPath,std::string aLayerType,std::string aDataSet, long aMeshCode,std::string aGeoType, bool *pCheck, bool aWalkFlag);
	//指定されたメッシュのSHPデータが格納されているパス作成関数
	bool MakeSHPPath(std::string aOrgPath,std::string aDataSet, long aMeshCode,std::string *pSHPPath);
	//フィルタ与え、カーソルを取得し、そのデータをコンテナに格納する関数
	template <class Type>
	bool GetObjectInfo(std::map<ObjectDataKey,Type> *pMeshData, IQueryFilterPtr ipQueryFilter,IFeatureClassPtr &ipFeatureClass, std::string aLayerType, long aMeshCode,std::string aGeoType,std::map<long,FID_Set> *pOIDSet,IDs aIDs)
	{
		std::set<long> aOIDSet;
		//カーソル取得し、ぶん回す
		IFeatureCursorPtr ipFeatureCursor = ipFeatureClass->_Search(ipQueryFilter,VARIANT_FALSE);
		IFeaturePtr ipFeature;

		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL )
		{
			Type aObjectData;//TKYObjectDataもしくはJGDObjectData格納用変数

			//FID取得
			long aFID = 0;
			ipFeature->get_OID(&aFID);
			aObjectData.e_FID = aFID;
			//OBJECTIDまたはそれに相当するID取得
			ObjectDataKey aObjectKey;
			CComVariant aOID;
			if(FAILED(ipFeature->get_Value(aIDs.e_ID.e_Index, &aOID))){
				std::cerr << aMeshCode << "_" << aLayerType << "FID:" << aFID << "の" << aIDs.e_ID.e_Name << "取得に失敗しました" << std::endl;
				return false;
			}
			aObjectKey.e_OID = aOID.lVal;
			//DIVID取得
			CComVariant aDIVID;
			if( ! aIDs.e_DIVID.e_Name.empty() ) {
				if(FAILED(ipFeature->get_Value(aIDs.e_DIVID.e_Index, &aDIVID))){
					std::cerr << aMeshCode << "_" << aLayerType << "FID:" << aFID << "の" << aIDs.e_DIVID.e_Name << "取得に失敗しました" << std::endl;
					return false;
				}
				aObjectKey.e_DIVID = aDIVID.lVal;
			}
			//形状取得
			IGeometryPtr ipGeometry;
			if(FAILED(ipFeature->get_ShapeCopy( &ipGeometry))){
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "の形状の取得に失敗しました" << std::endl;
				return false;
			}
			//形状のタイプ取得
			esriGeometryType aShapeType;
			if(FAILED(  ipGeometry->get_GeometryType( &aShapeType))){
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "の形状種別の取得に失敗しました" << std::endl;
				return false;
			}
			//ポリゴンの場合、複数のリンクで構成されているかチェックする。
			if(aShapeType == esriGeometryPolygon){
				IGeometryCollectionPtr ipGeometryCollection(ipGeometry);
				long aCount;
				ipGeometryCollection->get_GeometryCount(&aCount);
				if( aCount > 1){
					aObjectData.e_multi = true;
				}else
				{
					aObjectData.e_multi = false;
				}
			}else
			{
				aObjectData.e_multi =false;
			}

			
			aObjectData.e_ShapeType = aShapeType;
			//構成点などチェックに必要な情報を取得、またメッシュ境界線に接している場合のみ、コンテナにデータ挿入
			bool aCheck = false;
			if( aShapeType == esriGeometryPoint ){
				//ポイント用関数
				if(! GetPointInfo(ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
				   return false;
			}
			else if( aShapeType == esriGeometryPolyline){
				//ポリライン用関数
				if( !GetPolylineInfo( ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
					return false;
			}
			else if( aShapeType == esriGeometryPolygon){
				//ポリライン&ポリゴン用関数
				if(! GetPolygonInfo(ipGeometry,&aObjectData,aGeoType,aMeshCode,&aOIDSet,aFID,&aCheck))
					return false;
			}
			else{
				std::cerr << aMeshCode << "_" << aLayerType << " FID:" << aFID << "の形状に対応していません開発者に問い合わせてください" << std::endl;
				return false;
			}

			//コンテナにデータ挿入
			if( aCheck ){
				pMeshData->insert(std::pair<ObjectDataKey,Type>(aObjectKey,aObjectData));
			}

	
		}
		if(aGeoType == "TKY"){
			pOIDSet->insert(std::pair<long,FID_Set>(aMeshCode,aOIDSet));
		}

		return true;

	}
	
	//ポイント用オブジェクト情報取得関数
	template <class Type>
	bool GetPointInfo(const IGeometryPtr &ipGeometry, Type *pObjectData,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck) 
	{
		//形状を点に
		IPointPtr ipPoint(ipGeometry);
		
		//座標取得
		coord aCoord;
		if(FAILED(ipPoint->get_X(&aCoord.e_X)) || FAILED(ipPoint->get_Y(&aCoord.e_Y))){
			std::cerr << "緯度経度取得に失敗" << std::endl;
			return false;
		}
		if(aGeoType == "TKY"){
			crd_cnv aCrd;
			int X,Y;
			if(aCrd.LLtoMesh2(aCoord.e_X,aCoord.e_Y,aMeshCode,&X,&Y) == -1){
				std::cerr << "緯度経度の座標変換に失敗しました" << std::endl;
				return false;
			}
			//middleの場合の座標の範囲は 0 <= X <= 2000000, 0 <= Y <= 2000000
			//middle以外の座標の範囲は 0 <= X <= 1000000, 0 <= Y <= 1000000
			//座標よりメッシュ境界線上に接しているオブジェクトか判断する。
			long MAX_Coord = 0;
			if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
				MAX_Coord = 2000000;
			}else 
			{
				MAX_Coord = 1000000;
			}

			if((X == 0) || ( X == MAX_Coord) || ( Y == 0) || ( Y == MAX_Coord))
			{
				Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,0,aCoord);
				pOIDSet->insert(aFID);
				*pCheck = true;
			}
			
		}else
		{
		//座標をコンテナに挿入
		Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,0,aCoord);
		*pCheck = true;
		}

		return true;
	}

	template <class Type>
	bool GetPolylineInfo(const IGeometryPtr &ipGeometry, Type *pObjectData ,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck)
	{

		//形状を点列の集合に変換
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long aPointCount;
		if(FAILED(ipPointCollection->get_PointCount(&aPointCount))){
			std::cerr << "構成点の取得に失敗" << std::endl;
			return false;
		}

		//TKYの場合、始点、終点が境界線上にあるかチェック
		//メッシュ境界線に接しているかのチェック用変数(JGDはかならずture)
		bool aborderchk = false;
		if( aGeoType == "TKY")
		{
			IPointPtr ipFromPoint,ipToPoint;
			if(FAILED(ipPointCollection->get_Point(0,&ipFromPoint)) || FAILED(ipPointCollection->get_Point((aPointCount-1),&ipToPoint)))
			{
				std::cerr << "始点or終点の取得に失敗" << std::endl;
				return false;
			}
			crd_cnv aCrd;
			//座標の取得
			coord aFromCoord,aToCoord;
			if(FAILED(ipFromPoint->get_X(&aFromCoord.e_X))||FAILED(ipFromPoint->get_Y(&aFromCoord.e_Y))||FAILED(ipToPoint->get_X(&aToCoord.e_X))||FAILED(ipToPoint->get_Y(&aToCoord.e_Y)))
			{
				std::cerr << "始点or終点の緯度経度取得に失敗しました" <<std::endl;
				return false;
			}

			int From_X,From_Y,To_X,To_Y;
			if( (aCrd.LLtoMesh2(aFromCoord.e_X,aFromCoord.e_Y,aMeshCode,&From_X,&From_Y)==-1)||(aCrd.LLtoMesh2(aToCoord.e_X,aToCoord.e_Y,aMeshCode,&To_X,&To_Y)==-1))
			{
				std::cerr << "始点or終点の緯度経度→座標変換に失敗しました" << std::endl;
				return false;
			}

			//middleの場合の座標の範囲は 0 <= X <= 2000000, 0 <= Y <= 2000000
			//middle以外の座標の範囲は 0 <= X <= 1000000, 0 <= Y <= 1000000
			//座標よりメッシュ境界線上に接しているオブジェクトか判断する。　
			long MAX_Coord = 0;
			if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
				MAX_Coord = 2000000;
			}else 
			{
				MAX_Coord = 1000000;
			}

			//始点が境界線上にあるかのチェック
			if((From_X == 0) || (From_X == MAX_Coord) || (From_Y == 0) || (From_Y == MAX_Coord))
			{
				aborderchk = true;
			}//終点が境界線上にあるかのチェック
			else if((To_X == 0) || (To_X == MAX_Coord) || (To_Y == 0) || (To_Y == MAX_Coord))
			{
				aborderchk = true;
			}

		}
		else if( aGeoType == "JGD")
		{
			aborderchk = true;
			*pCheck = true;
		}

		//境界線上に接してしたら情報格納
		if( aborderchk){
			int i = 0;
			for(i;i < aPointCount; i++)
			{
				//i番目のiPointoPtr情報取得
				IPointPtr ipPoint;
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
					||FAILED(ipPoint->get_X(&(aCoord.e_X))) ||FAILED(ipPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "構成点情報の取得に失敗しました" << std::endl;
					return false;
				}
	
				//iPointPtr格納コンテナ、座標格納コンテナに挿入
				pObjectData->e_PointSet.insert(std::pair<long,IPointPtr>(i,ipPoint));
				Insert(&pObjectData->e_PointInfo,&pObjectData->e_CoordInfo,i,aCoord);
		
			}
			if(aGeoType == "TKY"){
			//FIDをコンテナに格納
			pOIDSet->insert(aFID);
			*pCheck = true;
			}
		}

	
		return true;
	}



	//ポリゴン用オブジェクト情報取得関数
	template <class Type>
	bool GetPolygonInfo(const IGeometryPtr &ipGeometry, Type *pObjectData/*std::map<long,IPointPtr> *pPointSet, std::multimap<Type1,Type2> *pPointInfo*/,std::string aGeoType,long aMeshCode,std::set<long> *pOIDSet,long &aFID,bool *pCheck)
	{
		//形状を点列の集合に変換
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long aPointCount;
		if(FAILED(ipPointCollection->get_PointCount(&aPointCount))){
			std::cerr << "構成点数の取得に失敗" << std::endl;
			return false;
		}
		//構成点がメッシュ境界に接しているかのチェック(TKYだけ)
		bool aborderchk = false;
		if( aGeoType == "TKY")
		{
			IPointPtr ipchkPoint;
			int i=0;
			for(i;i < aPointCount;i++)
			{
				//i番目のiPointPtr情報取得
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipchkPoint))
				||FAILED(ipchkPoint->get_X(&(aCoord.e_X))) ||FAILED(ipchkPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "構成点情報の取得に失敗しました" << std::endl;
					return false;
				}
				int X,Y;
				crd_cnv aCrd_cnv;
				if(aCrd_cnv.LLtoMesh2(aCoord.e_X,aCoord.e_Y,aMeshCode,&X,&Y) == -1){
					std::cerr << "緯度経度の座標変換に失敗しました" << std::endl;
					return false;
				}

				//middleの場合の座標の範囲は 0 <= X <= 2000000, 0 <= Y <= 2000000
				//middle以外の座標の範囲は 0 <= X <= 1000000, 0 <= Y <= 1000000
				//座標よりメッシュ境界線上に接しているオブジェクトか判断する。　
				long MAX_Coord = 0;
				if( (aMeshCode >= 2922) && (aMeshCode <= 6853) ){
					MAX_Coord = 2000000;
				}else 
				{
					MAX_Coord = 1000000;
				}
				if((X == 0) || ( X == MAX_Coord) || ( Y == 0) || ( Y == MAX_Coord))
				{
					aborderchk = true;
					break;
				}
			}
		}
		else if( aGeoType == "JGD")
		{
			aborderchk = true;
			*pCheck = true;
		}

		//構成点の情報を取得し、コンテナpPotinSet(iPointPtr格納コンテナ),pPointInfo(各補間点座標格納コンテナ) 
		if(aborderchk){
			IPointPtr ipPoint;
			int i=0;
			for (i;i < aPointCount;i++)
			{
				//i番目のiPointPtr情報取得
				coord aCoord;
				if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
					||FAILED(ipPoint->get_X(&(aCoord.e_X))) ||FAILED(ipPoint->get_Y(&(aCoord.e_Y))))
				{
					std::cout << "構成点情報の取得に失敗しました" << std::endl;
					return false;
				}	

				//iPointPtr格納コンテナ、座標格納コンテナに挿入
				(pObjectData->e_PointSet).insert(std::pair<long,IPointPtr>(i,ipPoint));
				Insert( &pObjectData->e_PointInfo, &pObjectData->e_CoordInfo,i,aCoord);
			
			}
			if(aGeoType == "TKY"){
			//FIDをコンテナに格納
			pOIDSet->insert(aFID);
			*pCheck = true;
			}
		}

		return true;
	}

	bool Insert(std::multimap<long,coord> *pPointInfo, std::multimap<coord,long> *pCoordInfo, long aNum,coord aCoord);



}
