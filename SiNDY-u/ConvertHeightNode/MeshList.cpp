#include "stdafx.h"
#include "MeshList.h"

const long MINBASEMESH = 100000;
const long MAXBASEMESH = 999999;

MeshList::MeshList(void)
{
}


MeshList::~MeshList(void)
{
}

bool MeshList::SetMeshList(LPCTSTR meshListPath)
{
	CString errMsg;
	// コンテナ初期化
	m_meshes.clear();
	// ファイルオープン
	std::ifstream ifs(meshListPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%sの読み込みに失敗しました"), meshListPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// メッシュ取得
	std::string secondMesh;
	while (std::getline(ifs, secondMesh)) {
		CString record = CA2CT(secondMesh.c_str());
		record.Trim();
		if (record.IsEmpty())
			continue;
		// 数値変換
		long mesh = _ttol(record);
		// 範囲チェック(細かいチェックはできないので6桁ってことだけ)
		if (MINBASEMESH > mesh || MAXBASEMESH < mesh) {
			errMsg.Format(_T("メッシュコードが不正です。：%d"), mesh);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		if (! m_meshes.insert(mesh).second) {
			errMsg.Format(_T("メッシュコード取得に失敗しました。:%d"), mesh);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
	}
	return true;
}

