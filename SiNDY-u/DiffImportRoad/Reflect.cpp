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
#include "Reflect.h"
#include "Import.h"
#include "MeshMerge.h"
#include "SeparatePolygon.h"

namespace Reflect{
	//ここはヘッダ参照
	const long NO_CHG_MODE = 0x1;
	const long TDC_MODE = 0x02;

	long g_Mode = 0;
	long g_CurrentMesh = -1;
	bool g_IsContinue = false;
	MESH_SET g_MeshSet;
	long g_NowProccess = -1;
	CString g_Operator;
	CString g_FixUser;

} //namespace Reflect

using namespace Reflect;

// コンストラクタ
CReflect::CReflect(void)
: m_OldPath(_T(""))
, m_NewPath(_T(""))
{
}

// デストラクタ
CReflect::~CReflect(void)
{
	g_MeshSet.clear();
}

// 初期化関数（最初の一回だけでいいもの）
bool CReflect::init_once(const CString cConnectStr[2], LPCTSTR cOldPath, LPCTSTR cNewPath, LPCTSTR cMeshList, long cMode)
{
	g_Mode = cMode;
	m_ConnectStr[0] = cConnectStr[0];
	m_ConnectStr[1] = cConnectStr[1];
	m_OldPath = cOldPath;
	m_NewPath = cNewPath;
	if(!fnFirstOnlyInit(cMeshList)){
		return false;
	}
	return true;
}

// 初期化関数（中断しても、処理開始前に必ず呼ばれる）
bool CReflect::init(void)
{
	//接続とフィーチャクラス類の準備
	if(!m_LayerManage.init(m_ConnectStr)){
		return false;
	}
	return true;
}

// 最初の1回のみ実行すればよい初期化関数
bool CReflect::fnFirstOnlyInit(LPCTSTR cMeshList)
{
#ifdef _DEBUG
	g_Operator = _T("for_debug");
	g_FixUser = _T("FIX201301");
#else
	LPCTSTR aOperator = _tgetenv(_T("OPERATOR"));
	if(aOperator != NULL){
		g_Operator = aOperator;
	}else{
		_ftprintf(stderr, _T("#環境変数「OPERATOR」がセットされていません\n"));
		_tprintf(_T("環境変数「OPERATOR」がセットされていません\n"));
		return false;
	}
	LPCTSTR aFixUser = _tgetenv(_T("FIX_USER"));
	if(aFixUser != NULL){
		g_FixUser = aFixUser;
	}else{
		_ftprintf(stderr, _T("#環境変数「FIX_USER」がセットされていません\n"));
		_tprintf(_T("環境変数「FIX_USER」がセットされていません\n"));
		return false;
	}
#endif

	if(_taccess(m_OldPath, 0x04) != 0){
		_ftprintf(stderr, _T("#ERROR 編集前データのパスが存在しないかアクセスできません,%s\n"), m_OldPath);
		_tprintf( _T("ERROR 編集前データのパスが存在しないかアクセスできません,%s\n"), m_OldPath);
		return false;
	}
	if(_taccess(m_NewPath, 0x04) != 0){
		_ftprintf(stderr, _T("#ERROR 編集後データのパスが存在しないかアクセスできません,%s\n"), m_NewPath);
		_tprintf( _T("ERROR 編集後データのパスが存在しないかアクセスできません,%s\n"), m_NewPath);
		return false;
	}
	if(m_OldPath == m_NewPath){
		_ftprintf(stderr, _T("#ERROR 編集前のデータパスと編集後データのパスが同じです,%s,%s\n"), m_OldPath, m_NewPath);
		_tprintf(_T("ERROR 編集前のデータパスと編集後データのパスが同じです,%s,%s\n"), m_OldPath, m_NewPath);
		return false;
	}

	//メッシュリスト読み込み
	if(!fnMeshLoad(cMeshList)){
		return false;
	}
	g_NowProccess = 0;
	return true;
}


/*
	編集差分反映作業メイン関数。次の（以前は個別だった）機能が組み込まれている
	・差分取り込み
	・不要構成点削除
	・メッシュ境界マージ
	・マルチパートポリゴン分割
	・不要構成点削除
*/
bool CReflect::execReflect(void)
{
	//差分インポート
	if(g_NowProccess == 0){
		CImport aImport(&m_LayerManage);
		if(aImport.init(m_OldPath, m_NewPath)){
			if(!aImport.executeALL(5)){
				return false;
			}
		}
	}

	//不要構成点削除+メッシュ境界マージ
	if(g_NowProccess == 1){
		CMeshMerge aMeshMerge(&m_LayerManage);
		if(aMeshMerge.init()){
			//ポリゴンがある時だけ実行
			if(!aMeshMerge.executeALL()){
				return false;
			}
		}
	}

	////マルチパートポリゴン解消
	if(g_NowProccess == 2){
		CSeparatePolygon aSeparatePolygon(&m_LayerManage);
		if(aSeparatePolygon.init()){
			//ポリゴンがある時だけ実行
			if(!aSeparatePolygon.executeALL()){
				return false;
			}
		}
	}
	m_LayerManage.unInit();	//全部破棄(切断)
	return g_IsContinue;
}

// メッシュリストの読み込み関数
bool CReflect::fnMeshLoad(LPCTSTR cMeshList)
{
	if(g_MeshSet.size() == 0){
		FILE *pFile;
		if((pFile = _tfopen(cMeshList, _T("rt"))) != NULL){
			char aBuf[16];
			while(fgets(aBuf, 16, pFile) != NULL){
				if(aBuf[0] == '#'){
					continue;
				}
				long aMesh = atol(aBuf);
				g_MeshSet.insert(aMesh);
			}
			fclose(pFile);
		}else{
			_ftprintf(stderr, _T("#ERROR,リストファイルが開けません,%s\n"), cMeshList);
			_tprintf(_T("ERROR,リストファイルが開けません,%s\n"), cMeshList);
			return false;
		}
	}
	return true;
}

