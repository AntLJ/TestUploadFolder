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
	std::string sLayer, sNid, sPid, sMesh, sErrMsg;			// �G���[���O�\���p
	std::vector<std::string> vecPoiList;

	// POI�t�@�C�����X�g�Ǎ�
	if (!nsCommon::loadList2vec(sPoiListPath.c_str(), vecPoiList, sErrMsg)) {
		ERR_LOG(sLayer, sNid, sPid, sMesh, sErrMsg);
		return false;
	}

	// POI�t�@�C���Ǎ�
	auto iteList = vecPoiList.begin();
	for (; iteList != vecPoiList.end(); ++iteList) {
		// �ΏۃR���e���c�R�[�h�̃f�[�^�̂݃R���e�i�ɋl����
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
		std::string sLayer, sNid, sPid, sMesh;			// �G���[���O�\���p
		ERR_LOG(sLayer, sNid, sPid, sMesh, (boost::format("�t�@�C���Ǎ����s, file=%s") % filename).str());
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
				case 0: // �Ǘ��R�[�h
					lMngCode = _ttoi(token);
					break;
				case 1: // �f�[�^���ރR�[�h
					lClassCode = _ttoi(token);
					break;
				case 2: // �X�܃R�[�h
					lStoreCode = _ttoi(token);
					break;
				case 16: // �o�x
					p.X = _tcstod(token, 0);
					break;
				case 17: // �ܓx
					p.Y = _tcstod(token, 0);
					break;
				default:
					break;
			}
		}

		// �R���e���c�R�[�h�擾
		std::string sContCode = (boost::format("%d%05d") % lClassCode % lStoreCode).str();

		// �擾�����R���e���c�R�[�h���`�F�b�N�ΏۃR���e���c�R�[�h�R���e�i�ɂ��邩�`�F�b�N
		auto iteContCode = std::find(setContCode.begin(), setContCode.end(), sContCode);
		if (iteContCode == setContCode.end()) {
			continue;
		}

		// �ΏۃR���e���c�R�[�h�̊Ǘ��R�[�h�E�o�x�ܓx���R���e�i�ɋl����
		mapPoiData.insert(MAP_POI_DATA::value_type(lMngCode, p));
	}

	return true;
}
