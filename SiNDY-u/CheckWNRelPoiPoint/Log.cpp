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
#include "Log.h"

bool CLog::m_bRunLog = false;
bool CLog::m_bErrLog = false;
std::ofstream CLog::m_ofsRunLog;
std::ofstream CLog::m_ofsErrLog;

CLog::CLog() {
}

CLog::~CLog() {
	if (m_bRunLog) {
		m_ofsRunLog.close();
	}
	if (m_bErrLog) {
		m_ofsErrLog.close();
	}
}

bool CLog::initLog(CParam& cParam) {
	if (!cParam.getRunLog().empty()) {
		if (!OpenLog(ETYPE_RUN, cParam.getRunLog())) {
			return false;
		}
		m_bRunLog = true;
	}

	if (!cParam.getErrLog().empty()) {
		if( !OpenLog(ETYPE_ERR, cParam.getErrLog())) {
			return false;
		}
		m_bErrLog = true;

		// 初回のみヘッダ出力
		std::string sHeader = "LAYER\tNODE_ID\tPOI_ID\tMESHCODE\tERROR_LEVEL\tMESSAGE";
		if (m_bErrLog) {
			m_ofsErrLog << sHeader << std::endl;
		}
	}

	CVersion cVer;
	std::string sMsg = (boost::format( "%s VERSION %s" )
						% CT2CA( cVer.GetOriginalFilename() ).m_psz
						% CT2CA( cVer.GetFileVersion() ).m_psz ).str();
	WriteRunLog(sMsg);
	WriteRunLog(cParam.getOption());

	return true;
}

void CLog::WriteRunLog(const std::string& sMsg) {
	std::cout << sMsg.c_str() << std::endl;
	if (m_bRunLog) {
		m_ofsRunLog << sMsg.c_str() << std::endl;
	}
}

void CLog::WriteErrLog(const std::string& sLayer,
					   const std::string& sNodeId,
					   const std::string& sPoiId,
					   const std::string& sMeshCode,
					   const std::string& sErrType,
					   const std::string& sMsg) {
	std::string sLayerDsp = sLayer;
	std::string sNodeIdDsp = sNodeId;
	std::string sPoiIdDsp = sPoiId;
	std::string sMeshCodeDsp = sMeshCode;
	if (sLayer == NULL_CHAR) {
		sLayerDsp = DSP_HYPHEN;
	}
	if (sNodeId == NULL_CHAR) {
		sNodeIdDsp = DSP_HYPHEN;
	}
	if (sPoiId == NULL_CHAR) {
		sPoiIdDsp = DSP_HYPHEN;
	}
	if (sMeshCode == NULL_CHAR) {
		sMeshCodeDsp = DSP_HYPHEN;
	}

	std::string sErrLine = (boost::format("%s\t%s\t%s\t%s\t%s\t%s")
							% sLayerDsp
							% sNodeIdDsp
							% sPoiIdDsp
							% sMeshCodeDsp
							% sErrType
							% sMsg).str();

	if (m_bErrLog) {
		m_ofsErrLog << sErrLine << std::endl;
	}
	else {
		std::cerr << sErrLine << std::endl;
	}

	return;
}

void CLog::WriteRunLogSTART(const SYSTEMTIME& sysTime) {
	CString csStartMsg = _T("start : ");
	CString csTotal;
	WriteRunLogTIME(sysTime, csStartMsg, csTotal);
}

void CLog::WriteRunLogEND(const SYSTEMTIME& sysTime, const CString& csTotal) {
	CString csEndMsg = _T("\nend   : ");
	WriteRunLogTIME(sysTime, csEndMsg, csTotal);
}

void CLog::WriteRunLogNotNewLine(const std::string& sMsg) {
	std::cout << sMsg.c_str() << std::flush;
	if (m_ofsRunLog) {
		m_ofsRunLog << sMsg.c_str() << std::flush;
	}
}

void CLog::WriteRunLogTIME(const SYSTEMTIME& sysTime, const CString& csMessage, const CString& csTotal) {
	WriteRunLog((boost::format("%s%04d/%02d/%02d %02d:%02d:%02d %s\n")
					% CT2CA(csMessage)
					% sysTime.wYear
					% sysTime.wMonth
					% sysTime.wDay
					% sysTime.wHour
					% sysTime.wMinute
					% sysTime.wSecond
					% CT2CA(csTotal)).str());
}

bool CLog::OpenLog(const LOG_TYPE eType, const std::string& strFilePath) {
	CString csLogDir((nsCommon::getPath(strFilePath)).c_str());

	if (!PathIsDirectory(csLogDir)) {
		// ディレクトリが存在しない場合、ディレクトリを作成する
		if (!MakeSureDirectoryPathExists(csLogDir + "\\")) {
			// ディレクトリ作成失敗
			std::cerr << "ERROR\tログディレクトリ作成失敗, dir=" << CT2CA(csLogDir) << std::endl;
			return NULL;
		}
	}

	switch (eType) {
		case ETYPE_RUN:
			m_ofsRunLog.open(strFilePath.c_str());
			if (m_ofsRunLog.fail()) {
				std::cerr << "ERROR\tファイルオープン失敗, file=" << strFilePath.c_str() << std::endl;
				return false;
			}
			break;

		case ETYPE_ERR:
			m_ofsErrLog.open(strFilePath.c_str());
			if (m_ofsErrLog.fail()) {
				std::cerr << "ERROR\tファイルオープン失敗, file=" << strFilePath.c_str() << std::endl;
				return false;
			}
			break;
	}

	return true;
}
