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
#include "GetLayerInfo.h"
#include "GetInfo.h"
#include "Merge.h"
#include <iostream>
#include <sstream>
#include <atl2/io.h>


CGetLayerInfo::CGetLayerInfo(long aMeshCode,std::wstring aAllow)
{
	m_MeshCode = aMeshCode;
	m_Allow = aAllow;
}

CGetLayerInfo::~CGetLayerInfo(void)
{
}

bool CGetLayerInfo::Merge5MeshInfo(std::wstring &rDirName, std::wstring & rLayerName, std::set<CheckKey> &rMergeSet)
{

	long aShapeType;

	if( (m_MeshCode <= 6848)&&(m_MeshCode >= 2936)) //ミドルマップの場合
	{
		 aShapeType = Merge5MiddleMesh(rDirName,rLayerName);
	}
	else if( (m_MeshCode <= 684827)&&(m_MeshCode >= 303640)) //ベースマップの場合
	{
		aShapeType = Merge5BaseMesh(rDirName,rLayerName);
	}
	else if( (m_MeshCode <= 68482777)&&(m_MeshCode >= 30364000))//シティマップの場合
	{
		aShapeType = Merge5CityMesh(rDirName,rLayerName);
	}
	else
	{
		std::cerr << m_MeshCode <<"は対象外のメッシュです" <<std::endl;
		return false;
	}

	if(_tcscmp(m_Allow.c_str(),_T("No"))==0)
	{
		CMerge aMerge;
		bool aOverlap = false;
		if(aMerge.MakeList(m_MainMap, m_NorthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_SouthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_EastMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeList(m_MainMap, m_WestMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}

		return aOverlap;
	}
	else if(_tcscmp(m_Allow.c_str(),_T("Yes"))==0)
	{
		CMerge aMerge;
		bool aOverlap = false;
		if(aMerge.MakeListAllowError(m_MainMap, m_NorthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_SouthMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_EastMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}
		if(aMerge.MakeListAllowError(m_MainMap, m_WestMap, rMergeSet, aShapeType)){
			aOverlap = true;
		}

		return aOverlap;
	}
	else
	{
		std::cout<<"第5引数が正しく設定されていません"<<std::endl;

		return false;
	}

}

long CGetLayerInfo::Merge5MiddleMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;
	
	CGetInfo aGetInfo;
	//着目メッシュの情報をコンテナに
	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName  << _T("\\") << m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}

	//着目メッシュの上のメッシュの情報をコンテナに
	long aNorthMeshCode = m_MeshCode+200;	//上のメッシュのメッシュコード
	std::wostringstream aNorthMeshDir;			
	aNorthMeshDir << rDirName << _T("\\") << aNorthMeshCode;
	aDirPath= aNorthMeshDir.str();

	if(! aGetInfo.GetInformation( m_NorthMap,aDirPath,rLayerName,aNorthMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}
	
	//着目メッシュの下のメッシュの情報をコンテナに
	long aSouthMeshCode = m_MeshCode-200;	
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << _T("\\") << aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();
	if(! aGetInfo.GetInformation( m_SouthMap,aDirPath,rLayerName,aSouthMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"の情報の取得に失敗しました。" << std::endl;
	}
	
	//着目メッシュの右のメッシュの情報をコンテナに
	long aEastMeshCode = m_MeshCode+2;
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << _T("\\") << aEastMeshCode;
	aDirPath = aEastMeshDir.str();
	if(! aGetInfo.GetInformation( m_EastMap,aDirPath,rLayerName,aEastMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"の情報の取得に失敗しました。" << std::endl;
	}

	//着目メッシュの左のメッシュの情報をコンテナに
	long aWestMeshCode = m_MeshCode-2;
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << _T("\\") << aWestMeshCode;
	aDirPath = aWestMeshDir.str();
	if(! aGetInfo.GetInformation( m_WestMap,aDirPath,rLayerName,aWestMeshCode,aShapeType)){
		std::cerr<< m_MeshCode << "_" <<  rLayerName <<"の情報の取得に失敗しました。" << std::endl;
	}

	return aShapeType;
}



long CGetLayerInfo::Merge5BaseMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;

	CGetInfo aGetInfo;
	//着目メッシュの情報をコンテナに
	long aFirstMeshCode = (m_MeshCode/100);
	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName << _T("\\") << aFirstMeshCode << _T("\\") << m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}

	//着目メッシュと上下メッシュの境界線が1次メッシュの境界線になってないかの判定用変数
	long aNorthSouthCheck = (m_MeshCode%100)/10;

	//着目メッシュの上のメッシュコード取得
	long aNorthMeshCode;			
	if( aNorthSouthCheck == 7 ){
	    aNorthMeshCode = m_MeshCode - 70 + 10000;	
	}
	else{
		aNorthMeshCode = m_MeshCode + 10;
	}

	//1次メッシュコード取得
	long aNorthFirstMeshCode = (aNorthMeshCode/100);
	//パスの作成
	std::wostringstream aNorthMeshDir;			
	aNorthMeshDir << rDirName << _T("\\") << aNorthFirstMeshCode << _T("\\") << aNorthMeshCode;
	aDirPath = aNorthMeshDir.str();
	if(! aGetInfo.GetInformation( m_NorthMap, aDirPath, rLayerName,aNorthMeshCode,aShapeType)){
		std::cerr << aNorthMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}

	//着目メッシュの下のメッシュのメッシュコード
	long aSouthMeshCode;

	//下のメッシュの情報をコンテナに
	if( aNorthSouthCheck == 0 ){
		aSouthMeshCode = m_MeshCode + 70 - 10000;
	}
	else{
		aSouthMeshCode = m_MeshCode - 10;
	}
	//1次メッシュコード取得
	long aSouthFirstMeshCode = (aSouthMeshCode/100);
	//パスの作成
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << _T("\\") << aSouthFirstMeshCode << _T("\\")<< aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();
	
	if(! aGetInfo.GetInformation( m_SouthMap, aDirPath, rLayerName,aSouthMeshCode,aShapeType)){
		std::cerr << aSouthMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}
		

	//着目メッシュと左右メッシュの境界線が1次メッシュの境界線になってないかの判定用変数
	long aEastWestCheck = m_MeshCode%10;
		
	//着目メッシュの右のメッシュのメッシュコード
	long aEastMeshCode;

	//右のメッシュの情報をコンテナに
	if( aEastWestCheck == 7){
		aEastMeshCode = m_MeshCode - 7 + 100;
	}
	else{
		aEastMeshCode = m_MeshCode + 1;
	}
	//1次メッシュコード取得
	long aEastFirstMeshCode = aEastMeshCode/100;
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << _T("\\") << aEastFirstMeshCode << _T("\\") << aEastMeshCode;
	aDirPath = aEastMeshDir.str();
	if(! aGetInfo.GetInformation( m_EastMap, aDirPath, rLayerName,aEastMeshCode,aShapeType)){
		std::cerr << aEastMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}

	//着目メッシュの左のメッシュのメッシュコードナ
	long aWestMeshCode;

	//左のメッシュの情報をコンテナに
	if ( aEastWestCheck == 0){
		aWestMeshCode = m_MeshCode + 7 -100;
	}
	else{
		aWestMeshCode = m_MeshCode - 1;
	}
	//1次メッシュコード取得
	long aWestFirstMeshCode = aWestMeshCode/100;
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << _T("\\") << aWestFirstMeshCode << _T("\\") << aWestMeshCode;
	aDirPath = aWestMeshDir.str();
	if(! aGetInfo.GetInformation( m_WestMap, aDirPath, rLayerName,aWestMeshCode,aShapeType)){
		std::cerr << aWestMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}

	return aShapeType;


}


long CGetLayerInfo::Merge5CityMesh(std::wstring &rDirName, std::wstring &rLayerName)
{
	long aShapeType;

	CGetInfo aGetInfo;

	long aSecondMeshCode = (m_MeshCode/100);
	long aFirstMeshCode = (aSecondMeshCode/100);

	std::wostringstream aMainMeshDir;
	aMainMeshDir << rDirName << _T("\\") << aFirstMeshCode << _T("\\") << aSecondMeshCode << _T("\\") <<m_MeshCode;
	std::wstring aDirPath = aMainMeshDir.str();
	if(! aGetInfo.GetInformation( m_MainMap, aDirPath, rLayerName,m_MeshCode,aShapeType)){
		std::cerr << m_MeshCode << "_" << rLayerName << "の情報の取得に失敗しました。" << std::endl;
	}




	//着目メッシュと上下メッシュの境界線が2次メッシュの境界線になってないかの判定用変数
	long aFirstNorthSouthCheck = (m_MeshCode%100)/10;
	//着目メッシュと上下メッシュの境界線が1次メッシュの境界線になってないかの判定用変数
	long aSecondNorthSouthCheck = (m_MeshCode%10000)/1000;
	//着目メッシュの上のコード
	long aNorthMeshCode;
	//着目メッシュの上のメッシュコード取得
	if(aFirstNorthSouthCheck == 7){
		if( aSecondNorthSouthCheck == 7){
			aNorthMeshCode = m_MeshCode-70-7000+1000000;
		}
		else{
			aNorthMeshCode = m_MeshCode-70+1000;
		}
	}
	else{
		aNorthMeshCode = m_MeshCode+10;
	}
	//2次、1次メッシュの取得
	long aNorthSecondMeshCode = aNorthMeshCode/100;
	long aNorthFirstMeshCode = aNorthSecondMeshCode/100;
	//パス取得
	std::wostringstream aNorthMeshDir;
	aNorthMeshDir << rDirName << "\\" << aNorthFirstMeshCode << "\\" << aNorthSecondMeshCode << "\\" << aNorthMeshCode;
	aDirPath = aNorthMeshDir.str();

	//着目メッシュの上のメッシュの情報取得
	if(! aGetInfo.GetInformation( m_NorthMap, aDirPath, rLayerName, aNorthMeshCode,aShapeType)){
		std::cerr << aNorthMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。\n" <<std::endl;
	}

	//着目メッシュの下のコード
	long aSouthMeshCode;
	//着目メッシュの下のメッシュのメッシュコード取得
	if(aFirstNorthSouthCheck == 0){
		if(aSecondNorthSouthCheck == 0){
			aSouthMeshCode = m_MeshCode +70 +7000 -1000000;
		}
		else{
			aSouthMeshCode = m_MeshCode +70 -1000;
		}
	}
	else{
		aSouthMeshCode = m_MeshCode -10;
	}
	//2次、1次メッシュの取得
	long aSouthSecondMeshCode = aSouthMeshCode/100;
	long aSouthFirstMeshCode = aSouthSecondMeshCode/100;
	//パス取得
	std::wostringstream aSouthMeshDir;
	aSouthMeshDir << rDirName << "\\" << aSouthFirstMeshCode << "\\" << aSouthSecondMeshCode << "\\" << aSouthMeshCode;
	aDirPath = aSouthMeshDir.str();

	//着目メッシュの下のメッシュの情報取得
	if(! aGetInfo.GetInformation( m_SouthMap, aDirPath, rLayerName, aSouthMeshCode,aShapeType)){
		std::cerr << aSouthMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。\n" <<std::endl;
	}

	//着目メッシュと左右メッシュの境界線が2次メッシュの境界線になってないかの判定用変数
	long aFirstEastWestCheck = (m_MeshCode%100)/10;
	//着目メッシュと左右メッシュの境界線が1次メッシュの境界線になってないかの判定用変数
	long aSecondEastWestCheck = (m_MeshCode%10000)/1000;

	//着目メッシュの右のメッシュのコード格納用
	long aEastMeshCode;
	//着目メッシュの右のメッシュコード取得
	if( aFirstEastWestCheck == 7){
		if( aSecondEastWestCheck == 7){
			aEastMeshCode = m_MeshCode -7 -700 +10000;
		}
		else{
			aEastMeshCode = m_MeshCode -7 +100;
		}
	}
	else{
		aEastMeshCode = m_MeshCode +1;
	}
	//2次、1次メッシュの取得
	long aEastSecondMeshCode = aEastMeshCode/100;
	long aEastFirstMeshCode = aEastSecondMeshCode/100;
	//パス取得
	std::wostringstream aEastMeshDir;
	aEastMeshDir << rDirName << "\\" << aEastFirstMeshCode << "\\" << aEastSecondMeshCode << "\\" << aEastMeshCode;
	aDirPath = aEastMeshDir.str();

	//着目メッシュの右のメッシュの情報取得
	if(! aGetInfo.GetInformation( m_EastMap, aDirPath, rLayerName, aEastMeshCode,aShapeType)){
		std::cerr << aEastMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。\n" <<std::endl;
	}

	//着目メッシュの左のメッシュのコード格納用
	long aWestMeshCode;
	//着目メッシュの左のメッシュのコード取得
	if( aFirstEastWestCheck == 0){
		if( aSecondEastWestCheck == 0){
			aWestMeshCode = m_MeshCode +7+700 -10000;
		}
		else{
			aWestMeshCode = m_MeshCode +7-100;
		}
	}
	else{
		aWestMeshCode = m_MeshCode -1;
	}
	//2次、1次メッシュの取得
	long aWestSecondMeshCode = aWestMeshCode/100;
	long aWestFirstMeshCode = aWestSecondMeshCode/100;
	//パス取得
	std::wostringstream aWestMeshDir;
	aWestMeshDir << rDirName << "\\" << aWestFirstMeshCode << "\\" << aWestSecondMeshCode << "\\" << aWestMeshCode;
	aDirPath = aWestMeshDir.str();

	//着目メッシュの左のメッシュの情報取得
	if(! aGetInfo.GetInformation( m_WestMap, aDirPath, rLayerName, aWestMeshCode ,aShapeType)){
		std::cerr << aWestMeshCode << "_" << rLayerName << "の情報の取得に失敗しました。データが\n" <<std::endl;
	}

	return aShapeType;

}












