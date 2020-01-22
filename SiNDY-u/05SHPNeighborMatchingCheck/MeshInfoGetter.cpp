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
#include "MeshInfoGetter.h"
#include "string.h"
#include "function.h"

CMeshInfoGetter::CMeshInfoGetter(std::string aLayerType)
{
	m_LayerType = aLayerType;
}

CMeshInfoGetter::~CMeshInfoGetter(void)
{
}

bool CMeshInfoGetter::GetMeshData(long aMeshCode, TKYMeshInfo *pTKYMeshInfo,int x,int y,const Parameters &rParameters)
{
	//チェック対象メッシュの確定
	pTKYMeshInfo->e_MeshCode	= Function::GetMeshCode(aMeshCode,x,y,rParameters.e_DataSet);

	std::map<long, TKYMeshData>::iterator Titr = m_TKYMeshInfo.find(pTKYMeshInfo->e_MeshCode);
	if( Titr == m_TKYMeshInfo.end() ){
		//SHPから情報ゲット！！
		bool aCheck = false;
		if(! ArcFunction::GetTKYInfo(rParameters,&pTKYMeshInfo->e_TKYMeshData,pTKYMeshInfo->e_MeshCode,m_LayerType,&m_OIDSet,&aCheck))
		{
			return aCheck;//メッシュ矩形がないもしくはデータが存在しなかった為、情報が取得できなかったのか、ただ単に失敗したのかの判断
		}
		//チェック該当メッシュ、その北または東メッシュだった場合、情報を保存
		if((x==0 && y==0)||(x==1 && y==0)||(x==0 && y==1)){
			if(m_TKYMeshInfo.size() >= 19){
				//メッシュ情報は19個以上保存しないので、要素数が19以上の場合、先頭を消す
				m_TKYMeshInfo.erase(m_TKYMeshInfo.begin());
			}
				
			m_TKYMeshInfo.insert(std::pair<long,TKYMeshData>(pTKYMeshInfo->e_MeshCode,pTKYMeshInfo->e_TKYMeshData));

		}

	}
	else{
		//メンバ変数(TKYデータ用コンテナ)から情報ゲット！！
		pTKYMeshInfo->e_TKYMeshData = Titr->second;
	}

	return true;
}

bool CMeshInfoGetter::GetMeshData(long aMeshCode, JGDMeshInfo *pJGDMeshInfo,int x,int y,const Parameters &rParameters)
{
	//チェック対象メッシュの確定
	pJGDMeshInfo->e_MeshCode = Function::GetMeshCode(aMeshCode,x,y,rParameters.e_DataSet);

	std::map<long, JGDMeshData>::iterator Jitr = m_JGDMeshInfo.find(pJGDMeshInfo->e_MeshCode);
	if( Jitr == m_JGDMeshInfo.end() ){
		//SHPから情報ゲット！！
		bool aCheck = false;
		if(! ArcFunction::GetJGDInfo(rParameters,&pJGDMeshInfo->e_JGDMeshData,pJGDMeshInfo->e_MeshCode,m_ipFeatureClass,m_LayerType,m_OIDSet,&aCheck))
		{
			return aCheck;//メッシュ矩形がないもしくはデータが存在しなかった為、情報がゲットできなかったのか、ただ単に失敗したのかの判断
		}
		//チェック該当メッシュ、その北または東メッシュだった場合、情報を保存
		if((x==0 && y==0)||(x==1 && y==0)||(x==0 && y==1)){
			if(m_JGDMeshInfo.size() >= 19){
				//メッシュ情報は19個以上保存しないので、要素数が19以上の場合、先頭を消す
				m_JGDMeshInfo.erase(m_JGDMeshInfo.begin());
			}
				
			m_JGDMeshInfo.insert(std::pair<long,JGDMeshData>(pJGDMeshInfo->e_MeshCode,pJGDMeshInfo->e_JGDMeshData));

		}

	}
	else{
		//メンバ変数(JGDデータ用コンテナ)から情報ゲット！！
		pJGDMeshInfo->e_JGDMeshData = Jitr->second;
	}

	return true;
}


