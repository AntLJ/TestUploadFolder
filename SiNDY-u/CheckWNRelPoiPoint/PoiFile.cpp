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
#include "PoiFile.h"

CPoiFile::CPoiFile(void) {
}

CPoiFile::~CPoiFile(void) {
}

bool CPoiFile::readPoiList(const std::string& sPoiListPath,
						   std::set<std::string>& setContCode,
						   MAP_POI_DATA& mapPoiData) {
	std::string sLayer, sNid, sPid, sMesh, sErrMsg;			// エラーログ表示用
	std::vector<std::string> vecPoiList;

	// POIファイルリスト読込
	if (!nsCommon::loadList2vec(sPoiListPath.c_str(), vecPoiList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}

	// POIファイル読込
	auto iteList = vecPoiList.begin();
	for (; iteList != vecPoiList.end(); ++iteList) {
		// 対象コンテンツコードのデータのみコンテナに詰込み
		if (!getPoiData(iteList->c_str(), setContCode, mapPoiData)) {
			return false;
		}
	}

	return true;
}

bool CPoiFile::getPoiData(const std::string& filename,
						  std::set<std::string>& setContCode,
						  MAP_POI_DATA& mapPoiData) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::string sLayer, sNid, sPid, sMesh;			// エラーログ表示用
		ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("ファイル読込失敗, file=%s") % filename).str());
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::wstring rec = utf8tosjis(line);
		CStringTokenizer tokenizer(CString(rec.c_str()), _T("\t"), TOKEN_RET_EMPTY_ALL);
		int token_count = tokenizer.CountTokens();
		long lMngCode, lClassCode, lStoreCode;
		WKSPoint p;
		for (int i = 0; i < token_count; ++i) {
			CString token = tokenizer.GetNextToken();
			switch (i) {
				case 0: // 管理コード
					lMngCode = _ttoi(token);
					break;
				case 1: // データ分類コード
					lClassCode = _ttoi(token);
					break;
				case 2: // 店舗コード
					lStoreCode = _ttoi(token);
					break;
				case 16: // 経度
					p.X = _tcstod(token, 0);
					break;
				case 17: // 緯度
					p.Y = _tcstod(token, 0);
					break;
				default:
					break;
			}
		}

		// コンテンツコード取得
		std::string sContCode = (boost::format("%d%05d") % lClassCode % lStoreCode).str();

		// 取得したコンテンツコードがチェック対象コンテンツコードコンテナにあるかチェック
		auto iteContCode = std::find(setContCode.begin(), setContCode.end(), sContCode);
		if (iteContCode == setContCode.end()) {
			continue;
		}

		// 対象コンテンツコードの管理コード・経度緯度をコンテナに詰込み
		mapPoiData.insert(MAP_POI_DATA::value_type(lMngCode, p));
	}

	return true;
}
