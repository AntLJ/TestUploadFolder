#include "stdafx.h"
#include "Log.h"

bool CLog::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if(!ofsLogFile.is_open()){
		std::cerr << CT2A(strFilePath) << "‚ÌƒI[ƒvƒ“‚ÉŽ¸”s" << std::endl;
		return false;
	}
	return true;
}

bool CLog::SetRunLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsRunLog);
}

bool CLog::SetErrLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsErrLog);
}

void CLog::PrintLog(bool bCout, bool bCerr, bool bRun, bool bErr, const CString& strMsg, bool bLineBreak)
{
	if(bCout){
		if(bLineBreak)
			std::cout << CT2A(strMsg) << std::endl;
		else
			std::cout << CT2A(strMsg);
	}

	if(bCerr){
		if(bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg);
	}

	if(bRun){
		if(bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg);
	}

	if(bErr){
		if(bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg);
	}
}
