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

#include "stdafx.h"
#include "CheckSplit.h"
#include "CheckSplitMesh.h"

#define DEFAULT_MIN_DIST 0.01 // 微小セグメント長（0.01[m] = 1[cm]）

bool CCheckSplit::Init(const Arguments& argument)
{
	m_strBeforeShpPath = argument.BeforeShpPath.c_str();
	m_strBeforeDBPath = argument.BeforeDBPath.c_str();
	m_strAfterShpPath = argument.AfterShpPath.c_str();
	m_strMeshDBPath = argument.MeshDBPath.c_str();
	m_strMeshSchema = argument.MeshSchema.c_str();
	m_dMinSegment = _ttof(argument.MinSegment.c_str());
	
	if(!argument.Tky2JgdPath.empty())
		m_bJGD2000 = true;

	switch(_ttol(argument.CheckMode.c_str())){
	case 1:
		m_CheckMode = check_mode::kSegmentAttr;
		break;
	case 2:
		m_CheckMode = check_mode::kWalkNode;
		break;
	case 3:
		m_CheckMode = check_mode::kHeightNode;
		break;
	case 99:
		m_CheckMode = check_mode::kIntegrity;
		break;
	default:
		return false;
		break;
	}

	// メッシュリスト取得
	if(!LoadMeshlist(argument.MeshlistPath.c_str()))
		return false;

	// 分割タイプ3の場合は高さノードを取得
	if(check_mode::kHeightNode == m_CheckMode){
		IWorkspacePtr ipBeforeDBWork, ipAfterHNShpWork, ipMeshWork;
		if(!GetWorkspace(m_strBeforeDBPath, ipBeforeDBWork) ||
			!GetWorkspace(m_strAfterShpPath, ipAfterHNShpWork) ||
			!GetWorkspace(m_strMeshDBPath, ipMeshWork))
			return false;

		// メッシュリストのメッシュコードにより取得するメッシュクラスが変わる
		CString strMeshClassName;
		if(!m_strMeshSchema.IsEmpty())
			strMeshClassName.Format(_T("%s."), m_strMeshSchema);
		switch(m_lMeshDigit)
		{
		case 4: // 1次メッシュコード
			strMeshClassName.AppendFormat(_T("%s"), sindy::schema::topmesh::kTableName);
			break;
		case 6: // 2次メッシュコード
			strMeshClassName.AppendFormat(_T("%s"), sindy::schema::basemesh::kTableName);
			break;
		case 8: // 都市地図メッシュコード
			strMeshClassName.AppendFormat(_T("%s"), sindy::schema::citymesh::kTableName);
			break;
		default:
			break;
		}
		// 変換前相対高さフィーチャクラス、変換後相対高さテーブル、メッシュ用フィーチャクラス取得
		if(!GetFeatureClass(sindy::schema::height_node::kTableName, ipBeforeDBWork, m_ipBeforeHNodeClass) ||
			!GetTable(shp05::schema::road::rel_height::kTableName, ipAfterHNShpWork, m_ipAfterHNodeTable) ||
			!GetFeatureClass(strMeshClassName, ipMeshWork, m_ipMeshClass)){
			return false;
		}
	}

	return true;
}

bool CCheckSplit::run(bool &bCheckErr)
{
	bool bReturn = true;

	for(const auto mesh : m_setMeshList){
		CLog::GetInstance().PrintLog(false, false, false, true, _T("#") + mesh);
		CLog::GetInstance().PrintLog(true, false, true, false, _T("#") + mesh, false);

		CString strBeforeWorkShpPath = GetWorkShpPath(m_strBeforeShpPath, mesh);
		CString strAfterWorkShpPath = GetWorkShpPath(m_strAfterShpPath, mesh);
		CCheckSplitMesh cCheckSplitMesh;

		// 初期化
		CLog::GetInstance().PrintLog(true, false, true, false, _T(" 初期化..."), false);
		if(!cCheckSplitMesh.Init(mesh, strBeforeWorkShpPath, strAfterWorkShpPath, m_CheckMode, m_ipBeforeHNodeClass, m_ipAfterHNodeTable, m_ipMeshClass, m_bJGD2000, m_dMinSegment)){
			CLog::GetInstance().PrintLog(true, false, true, false, _T("失敗"));
			bReturn = false;
			continue;
		} else
			CLog::GetInstance().PrintLog(true, false, true, false, _T("完了\t"), false);

		// 各テーブルデータ読み込み
		CLog::GetInstance().PrintLog(true, false, true, false, _T("データ読み込み..."), false);
#ifdef _DEBUG
		std::cout << std::endl;
#endif
		if(!cCheckSplitMesh.LoadData()){
			CLog::GetInstance().PrintLog(true, false, true, false, _T("失敗"));
			bReturn = false;
			continue;
		} else 
			CLog::GetInstance().PrintLog(true, false, true, false, _T("完了\t"), false);

		// チェック処理
		CLog::GetInstance().PrintLog(true, false, true, false, _T("データチェック..."), false);
		if(!cCheckSplitMesh.CheckData())
			bCheckErr = true; // エラーがある場合はフラグを立てておく
		CLog::GetInstance().PrintLog(true, false, true, false, _T("完了"));
	}
	return bReturn;
}

bool CCheckSplit::LoadMeshlist(const CString& strMeshlistPath)
{
	CString strMsg;

	//ファイルの読み込み
	std::ifstream ifstream(static_cast<CT2CW>(strMeshlistPath));

	if(!ifstream){
		strMsg.Format(_T("ファイルオープン失敗\t%s"), strMeshlistPath);
		CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
		return false;
	}

	//1行ずつ読み込み
	char szLine[128];
	long lMeshDigit = 0, lMeshDigitBefore = 0;
	while(ifstream.getline(szLine,128)){
		CString strMeshCode(szLine);
		if(strMeshCode.IsEmpty() || strMeshCode.Left(1) == _T("#"))
			continue;

		strMeshCode.Trim(_T(" "));
		strMeshCode.Trim(_T("　"));
		lMeshDigit = strMeshCode.GetLength();

		// メッシュリストのメッシュコードの桁はすべて同じでないとNG（ついでに桁数も4、6、8でないとNG）
		if((0 == lMeshDigitBefore || lMeshDigitBefore == lMeshDigit) &&
			(4 == lMeshDigit || 6 == lMeshDigit || 8 == lMeshDigit)){
			m_setMeshList.insert(strMeshCode);
		} else {
			strMsg.Format(_T("メッシュコードの桁数が統一されていない\t%s"), strMeshlistPath);
			CLog::GetInstance().PrintLog(false, true, false, true, err_type::fatal, strMsg);
			return false;
		}
		lMeshDigitBefore = lMeshDigit;
	}
	m_lMeshDigit = lMeshDigit;

	return true;
}

CString CCheckSplit::GetWorkShpPath(const CString& strBasePath, const CString& strMeshCode)
{
	CString strReturnPath;

	long lMeshDigit = strMeshCode.GetLength();

	switch(lMeshDigit){
		case 4: // 1次メッシュコード
			strReturnPath.Format(_T("%s\\%s\\%s"), strBasePath, strMeshCode, strMeshCode);
			break;
		case 6: // 2次メッシュコード
			strReturnPath.Format(_T("%s\\%s\\%s"), strBasePath, strMeshCode.Left(4), strMeshCode);
			break;
		case 8: // 都市地図メッシュコード
			strReturnPath.Format(_T("%s\\%s\\%s\\%s"), strBasePath, strMeshCode.Left(4), strMeshCode.Left(6), strMeshCode);
			break;
		default:
			break;
	}
	return strReturnPath;
}
