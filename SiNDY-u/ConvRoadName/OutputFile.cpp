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
#include "OutputFile.h"

bool OutputFile::init()
{
	// 既存ファイルの削除
	if (!deleteFile(m_roadNamePath) || !deleteFile(m_relRoadNameLinkPath) || !deleteFile(m_spltRoadNamePath))
		return false;
	m_inited = true;
	return true;
}

// ファイル出力
bool OutputFile::write(const std::set<RoadNameRecord>& rdNameRecs, const std::set<RelNameLinkRecord>& rlNameLnkRecs, const std::set<SpltNameRecord>& spltRdNameRecs)
{
	if (!m_inited) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NoInited);
		return false;
	}
	if (!writeRoadName(rdNameRecs))
		return false;
	if (!writeRelNameLink(rlNameLnkRecs))
		return false;
	if (!writeSplitRoadName(spltRdNameRecs))
		return false;
	return true;
}

// ファイル削除
bool OutputFile::deleteFile(LPCTSTR filePath)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	hFind = FindFirstFile(CT2CW(filePath), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		if (0 == DeleteFile(CT2CW(filePath))) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailDelFile, filePath);
			return false;
		}
	}
	return true;
}

// 道路名称ファイル出力
bool OutputFile::writeRoadName(const std::set<RoadNameRecord>& rdNameRecs)
{
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t道路名称ファイル出力"));
	std::ofstream ofs(m_roadNamePath, std::ios::app | std::ios::binary);
	if (!ofs.is_open()) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailOpenOutputFile, m_roadNamePath);
		return false;
	}
	long size = rdNameRecs.size();
	long counter = 0;
	for (const auto roadNameRecord : rdNameRecs) {
		++counter;
		if (counter % 100 == 0) {
			std::cout << "\t" << counter << " / " << size << " 件\r";
		}
		CStringW record;
		record.Format(_T("%ld\t%ld\t%s\t%s"), roadNameRecord.m_nameID, roadNameRecord.m_nameCls, roadNameRecord.m_name, roadNameRecord.m_yomi);
		ofs << UTF16toUTF8(record) << "\r\n";
	}
	std::cout << "\t" << counter << " / " << size << " 件" << std::endl;
	ofs.close();
	return true;
}

// 道路リンクに紐づく道路出力
bool OutputFile::writeRelNameLink(const std::set<RelNameLinkRecord>& rlNameLnkRecs)
{
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t道路リンクに紐づく道路名称ファイル出力"));
	std::ofstream ofs(m_relRoadNameLinkPath, std::ios::app | std::ios::binary);
	if (!ofs.is_open()) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailOpenOutputFile, m_relRoadNameLinkPath);
		return false;
	}
	long size = rlNameLnkRecs.size();
	long counter = 0;
	for (const auto relNameLinkRecord : rlNameLnkRecs) {
		++counter;
		if (counter % 100 == 0) {
			std::cout << "\t" << counter << " / " << size << " 件\r";
		}
		CStringW record;
		record.Format(_T("%ld\t%ld\t%ld"), relNameLinkRecord.m_roadOID, relNameLinkRecord.m_nameID, relNameLinkRecord.m_nameCls);
		ofs << UTF16toUTF8(record) << "\r\n";
	}
	std::cout << "\t" << counter << " / " << size << " 件" << std::endl;
	ofs.close();
	return true;
}

// 道路名称分割データ出力
bool OutputFile::writeSplitRoadName(const std::set<SpltNameRecord>& spltRdNameRecs)
{
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t道路名称分割ファイル出力"));
	std::ofstream ofs(m_spltRoadNamePath, std::ios::app | std::ios::binary);
	if (!ofs.is_open()) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailOpenOutputFile, m_spltRoadNamePath);
		return false;
	}
	long size = spltRdNameRecs.size();
	long counter = 0;
	for (const auto spltNameRecord : spltRdNameRecs)
	{
		++counter;
		if (counter % 100 == 0) {
			std::cout << "\t" << counter << " / " << size << " 件\r";
		}
		CStringW record;
		record.Format(_T("%ld\t%ld\t%ld\t%s\t%s"), spltNameRecord.m_nameID, spltNameRecord.m_spltCls, spltNameRecord.m_spltSeq, spltNameRecord.m_spltName, spltNameRecord.m_spltYomi);
		ofs << UTF16toUTF8(record) << "\r\n";
	}
	std::cout << "\t" << counter << " / " << size << " 件" << std::endl;
	ofs.close();
	return true;
}