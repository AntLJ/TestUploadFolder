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
#include "Common.h"
#include "Log.h"

bool nsCommon::loadList2vec(const std::string& file, std::vector<std::string>& vecList, std::string& sErrMsg) {
	std::ifstream ifs(file);
	if (!ifs) {
		sErrMsg = (boost::format("ファイル読込失敗, file=%s") % file).str();
		return false;
	}

	copy(std::istream_iterator<std::string>(ifs),
		 std::istream_iterator<std::string>(),
		 back_inserter(vecList));

	if (vecList.size() == 0) {
		sErrMsg = (boost::format("ファイル内容不正(0件), file=%s") % file).str();
		return false;
	}

	return ifs.eof();
}

bool nsCommon::loadList2set(const std::string& file, std::set<std::string>& setList, std::string& sErrMsg) {
	std::ifstream ifs(file);
	if (!ifs) {
		sErrMsg = (boost::format("ファイル読込失敗, file=%s") % file).str();
		return false;
	}

	std::string sLine;
	while (getline(ifs, sLine)) {
		boost::trim(sLine);
		if (sLine.empty()) {
			continue;
		}
		setList.insert(sLine);
	}

	if (setList.size() == 0) {
		sErrMsg = (boost::format("ファイル内容不正(0件), file=%s") % file).str();
		return false;
	}

	return ifs.eof();
}

bool nsCommon::getFieldIndex(const ITablePtr ipTable,
							const CString& csField,
							long& lIndex,
							std::string& sErrMsg) {
	IFieldsPtr ipFields;
	if (FAILED(ipTable->get_Fields(&ipFields))) {
		sErrMsg = (boost::format("フィールド取得失敗, field=%s") % CT2CA(csField)).str();
		return false;
	}

	if (FAILED(ipFields->FindField(CComBSTR(csField), &lIndex)) || (-1 == lIndex)) {
		sErrMsg = (boost::format("フィールドインデックス取得失敗, field=%s") % CT2CA(csField)).str();
		return false;
	}

	return true;
}

std::string nsCommon::getLayerName(const std::string& sFilePath) {
	CString csTblFilePath(sFilePath.c_str());
	CString csFileName = csTblFilePath.Right(csTblFilePath.GetLength()-csTblFilePath.ReverseFind(_T('\\'))-1);
	CString csLayerName = csFileName.Left(csFileName.ReverseFind(_T('.')));

	return (boost::format("%s") % CT2CA(csLayerName)).str();
}

std::string nsCommon::getPath(const std::string& sFilePath) {
	CString csTblFilePath(sFilePath.c_str());
	CString csTargetPath = csTblFilePath.Left(csTblFilePath.ReverseFind(_T('\\')));

	return (boost::format("%s") % CT2CA(csTargetPath)).str();
}
