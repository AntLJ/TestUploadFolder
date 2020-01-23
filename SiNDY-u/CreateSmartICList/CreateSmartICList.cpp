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
#include "CreateSmartICList.h"
#include <sindy/workspace.h>
#include <sindy/schema/road.h>
#include <sindy/schema/reference.h>

CSmartICList::CSmartICList(void)
{
}

CSmartICList::~CSmartICList(void)
{
}

CSmartICList::CSmartICList(CString strConfigPath)
{
	//各初期化
	CReadConfig cConfig;

	//パラメータの取得
	m_eParams = cConfig.getenv(strConfigPath);

	//出力ログの設定
	openfs(m_eParams.m_strOut);

	//ワークスペースのオープン
	const IWorkspacePtr	ipRoadWorksapce(INamePtr(sindy::create_workspace_name(m_eParams.m_strNwDB))->_Open());

	//フィーチャクラス群の取得
	getfc(ipRoadWorksapce);

	//フィールドインデックスの設定
	gerfiledindex();
}

//------------------初期設定ここから--------------------------------

//ログファイルのオープン
void CSmartICList::openfs(CString strOutPath)
{
	CString strLinkofs, strNodeofs;
	strLinkofs = strOutPath + _T("\\smartic_link.txt");
	strNodeofs = strOutPath + _T("\\smartic_node.txt");

	m_linkofs.open( static_cast<CT2CW>(strLinkofs) );
	m_nodeofs.open( static_cast<CT2CW>(strNodeofs) );

	if(! ( m_linkofs.is_open() && m_nodeofs.is_open() ))
	{
		throw std::runtime_error(_T("ログファイルのオープンに失敗."));
	}

	//header出力
	setlinkheader();
	setnodeheader();
}

//ヘッダの出力
void CSmartICList::setlinkheader()
{
	m_linkofs << _T("#スマートICリンクフォーマット [smartic_link.txt]	Ver.1.0") << std::endl;
	m_linkofs << _T('#') << std::endl;
	m_linkofs << _T("#内容：") << std::endl;
	m_linkofs << _T("#	スマートICリンクのリスト.") << std::endl;
	m_linkofs << _T("#フォーマット：") << std::endl;
	m_linkofs << _T("#	全国1ファイル(SJIS)とする。先頭'#'はコメント行とする。") << std::endl;
	m_linkofs << _T("#	以下、1～2のレコードを<TAB>で区切り１行で1データとする.") << std::endl;
	m_linkofs << _T("#	1.２次メッシュコード	：対象リンクの属する２次メッシュコードを記述.") << std::endl;
	m_linkofs << _T("#	2.オブジェクトID	：<２次メッシュコード>_RLK.dbf のOBJECTIDフィールドの属性値を記述.") << std::endl;
	m_linkofs << _T('#') << std::endl;
}

//ヘッダの出力
void CSmartICList::setnodeheader()
{
	m_nodeofs << _T("#スマートICノードフォーマット [smartic_node.txt]	Ver.1.0") << std::endl;
	m_nodeofs << _T('#') << std::endl;
	m_nodeofs << _T("#内容：") << std::endl;
	m_nodeofs << _T("#	スマートICリンク[smartic_link.txt]間引き後に生じる、浮きノードのリスト.") << std::endl;
	m_nodeofs << _T("#フォーマット：") << std::endl;
	m_nodeofs << _T("#	全国1ファイル(SJIS)とする。先頭'#'はコメント行とする.") << std::endl;
	m_nodeofs << _T("#	以下、1～2のレコードを<TAB>で区切り１行で1データとする.") << std::endl;
	m_nodeofs << _T("#	1.２次メッシュコード	：対象ノードの属する２次メッシュコードを記述.") << std::endl;
	m_nodeofs << _T("#	2.オブジェクトID	：<２次メッシュコード>_RND.dbf のOBJECTIDフィールドの属性値を記述.") << std::endl;
	m_nodeofs << _T('#') << std::endl;
}

//フィーチャクラスの取得
void CSmartICList::getfc(IWorkspacePtr ipNwWorkspace)
{
	IFeatureWorkspacePtr ipNwFW(ipNwWorkspace);
	m_NwFC.ipRoadLinkFC		= ipNwFW->_OpenFeatureClass(::sindy::schema::road_link::kTableName);
	m_NwFC.ipRoadNodeFC		= ipNwFW->_OpenFeatureClass(::sindy::schema::road_node::kTableName);
	m_NwFC.ipBaseMeshFC		= ipNwFW->_OpenFeatureClass(_T("REFERENCE.BaseMesh"));	
}

//フィールドインデックスの取得
void CSmartICList::gerfiledindex()
{
	m_FieldIndex.lFromID	= m_NwFC.ipRoadLinkFC->_FindField(::sindy::schema::road_link::kFromNodeID);
	m_FieldIndex.lToID		= m_NwFC.ipRoadLinkFC->_FindField(::sindy::schema::road_link::kToNodeID);
	m_FieldIndex.lLinkOID	= m_NwFC.ipRoadLinkFC->_FindField(_T("OBJECTID"));

	m_FieldIndex.lMeshcode	= m_NwFC.ipBaseMeshFC->_FindField(::sindy::schema::basemesh::kMeshCode);
}

//------------------初期設定ここまで--------------------------------

//処理の実行
void CSmartICList::Execute()
{
	//UPDOWNCLASS_Cが5のリンクのOID属するMESHCODE を取得、出力。
	//FROM_NODE_ID, TO_NODE_ID,も取得する
	std::cerr << m_eParams.m_strOut << _T("\\smartic_link.txt");
	CreateLinkList();
	std::cerr << _T("\tOK") << std::endl;

	//スマートICリンクリスト間引き後に浮きノードになるリストの作成
	std::cerr << m_eParams.m_strOut << _T("\\smartic_node.txt");
	CreateFloatNodeList();
	std::cerr << _T("\tOK") << std::endl;
}

//スマートリンクリストの作成
void CSmartICList::CreateLinkList()
{
	//出力データ成型用
	llSET llOutSet;

	IFeatureCursorPtr ipFCr = m_cDBManager.GetFeatureCursor(m_NwFC.ipRoadLinkFC, _T("UPDOWNCLASS_C = 5"));

	IFeaturePtr ipFeature;

	while(S_OK == ipFCr->NextFeature(&ipFeature))
	{
		//OIDの取得
		long lOID = m_cDBManager.GetOID(ipFeature);

		//出力用リストの作成
		CreateList(lOID, GetMeshCode(m_cDBManager.GetGeom(ipFeature), esriSpatialRelWithin), llOutSet);

		//情報の格納
		m_SmartICLinkSet.insert(lOID);
		m_SmartICNodeSet.insert(m_cDBManager.GetValue(ipFeature, m_FieldIndex.lFromID).lVal);
		m_SmartICNodeSet.insert(m_cDBManager.GetValue(ipFeature, m_FieldIndex.lToID).lVal);
	}
	//リスト出力
	Output(llOutSet, m_linkofs);
}

/*
m_SmartICNodeSetに格納されているノードに接続しているリンクを取得し、
m_SmartICLinkSetに格納されているリンクしか接続されていなかった場合は浮きノードとなる
*/
void CSmartICList::CreateFloatNodeList()
{	
	//出力データ成型用
	llSET llOutSet;

	//ノードを1オブジェクトずつチェック
	for(ConstItr aIter = m_SmartICNodeSet.begin(); aIter != m_SmartICNodeSet.end(); ++aIter)
	{
		//ノードの形状を取得する
		IGeometryPtr ipGeom = m_cDBManager.GetGeomFromOID(*aIter, m_NwFC.ipRoadNodeFC);

		//ノード形状に接触しているリンクOIDの取得
		vaSET vaTouchLinkIDSet = m_cDBManager.GetHitValSet(m_NwFC.ipRoadLinkFC, ipGeom, m_FieldIndex.lLinkOID, esriSpatialRelTouches);
		
		//該当ノードにSmartICリンクしか接続していない場合は浮きノードになる
		if(IsFloatNode(vaTouchLinkIDSet))
		{
			CreateList(*aIter, GetMeshCode(ipGeom, esriSpatialRelIntersects), llOutSet);
		}
	}
	//リスト出力
	Output(llOutSet, m_nodeofs);
}

//SmartICリンク間引き後浮きノードになるかどうかのチェック
bool CSmartICList::IsFloatNode(const vaSET&  vaTouchLinkIDSet)
{
	for(ConstVaItr aIter = vaTouchLinkIDSet.begin(); aIter != vaTouchLinkIDSet.end(); ++aIter)
	{
		CComVariant vaLinkOID = *aIter;
		if(m_SmartICLinkSet.end() == m_SmartICLinkSet.find(vaLinkOID.lVal))
			return false;
	}
	return true;
}

//メッシュコードの取得
vaSET CSmartICList::GetMeshCode(IGeometryPtr ipGeom, esriSpatialRelEnum eStType)
{
	return m_cDBManager.GetHitValSet(m_NwFC.ipBaseMeshFC, ipGeom, m_FieldIndex.lMeshcode, eStType);
}

//出力用リストの作成
void CSmartICList::CreateList(long lOID, const vaSET& vaMeshCodeSet, llSET& llOutSet)
{
	for(ConstVaItr aIter = vaMeshCodeSet.begin(); aIter != vaMeshCodeSet.end(); ++aIter)
	{
		CComVariant vaMeshCode = *aIter;
		//出力データを成型するためにコンテナに登録
		llOutSet.insert(std::pair<long,long>(vaMeshCode.lVal, lOID));
	}
}

//作成したリストを出力する
void CSmartICList::Output(const llSET& llOutSet, std::ofstream& ofs)
{
	for(ConstItr2 aIter = llOutSet.begin(); aIter != llOutSet.end(); ++aIter)
	{
		//対象メッシュのみを出力する
		if(m_eParams.m_lMeshSet.find(aIter->first) != m_eParams.m_lMeshSet.end())
		{
			ofs << aIter->first << _T("\t") << aIter->second << std::endl;
		}
	}

}

