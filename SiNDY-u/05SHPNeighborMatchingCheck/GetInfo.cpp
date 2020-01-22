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
#include "GetInfo.h"
#include "GetPointInfo.h"
#include "GetPolyLineInfo.h"
#include "GetPolygonInfo.h"
#include <sstream>
#include <atl2/io.h>


CGetInfo::CGetInfo(void)
{
}


CGetInfo::~CGetInfo(void)
{
}

bool CGetInfo::GetInformation(std::multimap<Key,Info> & rInfoMap, std::wstring & rDirName, std::wstring & rLayerName, long aMeshCode,long & rFeatureType)
{
	
	//HRESULT hr;

	//ディレクトリの存在確認
	if(! PathIsDirectory(rDirName.c_str()))
	{
		std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "は存在しません。" << std::endl;
		return true;
	}

	//読み取りファイル名の作成
	std::wostringstream aFile;
	aFile << aMeshCode << _T("_") << rLayerName;

	std::wstring aFileName = aFile.str();

	//読み取りファイルのフルパス作成
	std::wostringstream aPath;
	aPath << rDirName << "\\"<<aFileName<<".shp";

	std::wstring aFileFullPath = aPath.str();

	//ファイルの存在確認
	if(! PathFileExists(aFileFullPath.c_str()))
	{
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "は存在しません。"<<std::endl;
		return true;
	}

	
	
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_ShapefileWorkspaceFactory );

	//ワークスペース取得
	IWorkspacePtr ipWorkspace;
	if( FAILED(ipWorkspaceFactory->OpenFromFile( T2BSTR( rDirName.c_str()), 0 , &ipWorkspace ) )){
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "のワークスペース取得に失敗しました。" << std::endl;
		return false;
	}

	

	//フィーチャクラスの取得
	IFeatureClassPtr ipFeatureClass;
	if(FAILED(IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(T2BSTR(aFileName.c_str()), &ipFeatureClass))){
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName  <<"フィーチャークラス取得に失敗しました" << std::endl;
		return false;
	}

	long lOBJECTIDIndex;

	if(rLayerName == _T("DCL")){
		if(FAILED( ipFeatureClass->FindField(_T("BLDID"),&lOBJECTIDIndex) ) ){
			std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "のBLDIDインデックスの検索に失敗しました"<<std::endl;
			return false;
		}
	}
	else if(( rLayerName == _T("XDA")) || (rLayerName == _T("XDM")) ){
		if(FAILED( ipFeatureClass->FindField(_T("ANNOID"),&lOBJECTIDIndex) ) ){
			std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "のANNOIDインデックスの検索に失敗しました"<<std::endl;
			return false;
		}
	}
	else if( rLayerName == _T("MLA")){
		if(FAILED( ipFeatureClass->FindField(_T("IPC_ID"),&lOBJECTIDIndex) ) ){
			std::cerr << rDirName << "\\" << aMeshCode << "_" << rLayerName << "のIPC_IDインデックスの検索に失敗しました" << std::endl;
			return false;
		}
	}
	else{
		//OBJECTIDのフィールド上のインデックス取得
		if(FAILED( ipFeatureClass->FindField(_T("OBJECTID"),&lOBJECTIDIndex) ) ){
			std::cerr <<  rDirName << "\\" << aMeshCode << "_" << rLayerName << "のOIDインデックスの検索に失敗しました"<<std::endl;
			return false;
	
		}
	}

	//フィーチャカーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED( ipFeatureClass->Search(NULL, VARIANT_FALSE, & ipFeatureCursor))){
		
		std::cerr << rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "カーソルの取得に失敗しました" << std::endl;
		return false;

	}

	IFeaturePtr ipFeature;
	//フィーチャカーソルが尽きるまでフィーチャを巡回
	while( ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature != NULL))
	{
		//FID取得
		long lOID = ipFeature->GetOID();


		CComVariant ObjectID;						//ObjectI
		if(FAILED(ipFeature->get_Value(lOBJECTIDIndex, &ObjectID))){
			std::cout << rDirName << "\\" << aMeshCode << "_" << rLayerName << "フィーチャID:" <<lOID<<"のOBJECTIDの取得に失敗" << std::endl;
			continue;
		}

		Key aKey;                       //キー（オブジェクトID,X,Y座標）格納用　構造体
	    Info aInfo;						//メッシュコード、補間点番号格納用　構造体

		aInfo.e_MeshCode = aMeshCode;
		//aKey.e_OID = ObjectID.lVal;
		
		
		
		//形状の取得
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy( &ipGeometry ))){
			std::cout << rDirName << "\\" << aMeshCode << "_" << rLayerName << "フィーチャID:" <<lOID << "形状の取得に失敗" << std::endl;
			continue;
		}

		//形状の種類の取得
		esriGeometryType aShapeType;
		if(FAILED( ipGeometry->get_GeometryType( &aShapeType ))){
			std::cout<< rDirName << "\\" <<  aMeshCode << "_" << rLayerName << "フィーチャID:"<< lOID << "形状のタイプの取得に失敗" << std::endl;
			continue;
		}
		rFeatureType = aShapeType;
		//ポイント情報取得
		if (aShapeType == esriGeometryPoint){
			aKey.e_OID = 0;
			aInfo.e_OID = ObjectID.lVal;

			CGetPointInfo aGetPointInfo;
			if(! aGetPointInfo.GetPoint( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "フィーチャID:"<< lOID << "ポイント情報取得に失敗" << std::endl;
			}
		}
		else if (aShapeType == 3){
			aKey.e_OID = 0;
			aInfo.e_OID = ObjectID.lVal;
			
			CGetPolyLineInfo aGetPolyLineInfo;
			if(! aGetPolyLineInfo.GetPolyline( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "フィーチャID:"<< lOID << "ポイント情報取得に失敗" << std::endl;
			}
		}
		else if (aShapeType == 4){
			aKey.e_OID = ObjectID.lVal;
			aInfo.e_OID = 0;
			CGetPolygonInfo aGetPolygonInfo;
			if(! aGetPolygonInfo.GetPolygon( ipGeometry, rInfoMap, & aKey, & aInfo)){
				std::cout << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "フィーチャID:"<< lOID << "ポイント情報取得に失敗" << std::endl;
			}
		}
		else{
			std::cout << "このツールは" << rDirName << "\\" <<   aMeshCode << "_" << rLayerName << "フィーチャID:"<< lOID << "のタイプに対応していません" << std::endl;
		}



	}

return true;

}
