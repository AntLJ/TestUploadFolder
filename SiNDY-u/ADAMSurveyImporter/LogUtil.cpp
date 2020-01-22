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

#include "LogUtil.h"
#include "TimeUtil.h"

std::stringstream LogUtil::_strLogMsg;
uh::tstring LogUtil::_dirPath = _T(".\\");
long LogUtil::_errCount = 0;
long LogUtil::_warCount = 0;

LogUtil::LogUtil(void)
{
}

LogUtil::~LogUtil(void)
{
}

void LogUtil::print(const std::string& msg)
{
	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
}

void LogUtil::printDebug(const std::string& msg)
{
#ifdef _DEBUG
	std::cout << "\t[DEBUG]" + msg << std::endl;

	_strLogMsg << "\t[DEBUG]" +msg << std::endl;
#endif
}

void LogUtil::printWarning(const std::string& msg)
{
	std::cerr << "\t[WAR]" + msg << std::endl;

	_strLogMsg <<"\t[WAR]" + msg << std::endl;

	_warCount++;
}

void LogUtil::printError(const std::string& msg)
{
	std::cerr << "\t[ERR]" + msg << std::endl;

	_strLogMsg <<"\t[ERR]" + msg << std::endl;

	_errCount++;
}

void LogUtil::print(const uh::tstring& msg)
{
	LogUtil::print(uh::toStr(msg));
}

void LogUtil::flushLine(const std::string& msg)
{
	std::cout << msg << "\r" << std::flush;
}

void LogUtil::printDebug(const uh::tstring& msg)
{
#ifdef _DEBUG
	LogUtil::printDebug(uh::toStr(msg));
#endif
}

void LogUtil::printWarning(const uh::tstring& msg)
{
	LogUtil::printWarning(uh::toStr(msg));
}

void LogUtil::printError(const uh::tstring& msg)
{
	LogUtil::printError(uh::toStr(msg));
}

void LogUtil::print(const std::ostream& msg)
{
	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
}

void LogUtil::printDebug(const std::ostream& msg)
{
#ifdef _DEBUG
	std::cout << "\t[DEBUG]";
	std::cout << msg << std::endl;

	_strLogMsg << "\t[DEBUG]";
	_strLogMsg << msg << std::endl;
#endif
}

void LogUtil::printWarning(const std::ostream& msg)
{
	std::cerr << "\t[WAR]";
	std::cerr << msg << std::endl;

	_strLogMsg << "\t[WAR]";
	_strLogMsg << msg << std::endl;

	_warCount++;
}

void LogUtil::printError(const std::ostream& msg)
{
	std::cerr << "\t[ERR]";
	std::cerr << msg << std::endl;

	_strLogMsg << "\t[ERR]";
	_strLogMsg << msg << std::endl;

	_errCount++;
}

void LogUtil::print(const boost::program_options::options_description& msg)
{
	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
}

void LogUtil::printDebug(const boost::program_options::options_description& msg)
{
#ifdef _DEBUG
	std::cout << "\t[DEBUG]";
	std::cout << msg << std::endl;

	_strLogMsg << "\t[DEBUG]";
	_strLogMsg << msg << std::endl;
#endif
}

void LogUtil::printWarning(const boost::program_options::options_description& msg)
{
	std::cerr << "\t[WAR]";
	std::cerr << msg << std::endl;

	_strLogMsg << "\t[WAR]";
	_strLogMsg << msg << std::endl;

	_warCount++;
}

void LogUtil::printError(const boost::program_options::options_description& msg)
{
	std::cerr << "\t[ERR]";
	std::cerr << msg << std::endl;

	_strLogMsg << "\t[ERR]";
	_strLogMsg << msg << std::endl;

	_errCount++;
}

void LogUtil::write()
{
	LogUtil::_writeLog(_dirPath);
}

void LogUtil::write(const uh::tstring& dir)
{
	setLogDirPath(dir);
	LogUtil::_writeLog(_dirPath);
}

void LogUtil::_writeLog(const uh::tstring& filename)
{
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);

	uh::tstring path =  filename + _T("\\Log_ADAMSurveyImporter_") + TimeUtil::GetTime() + _T(".txt");

	std::ofstream of;
	of.open(uh::toStr(path), std::ios::out);

	of << _strLogMsg.str() << std::endl;
}

void LogUtil::setLogDirPath(const uh::tstring& dir)
{
	_dirPath = dir;
}

long LogUtil::getErrCount()
{
	return _errCount;
}

long LogUtil::getWarCount()
{
	return _warCount;
}

void LogUtil::printLine()
{
	LogUtil::print("--------------------------------------------");
}

void LogUtil::printDoubleLine()
{
	LogUtil::print("=============================================");
}

void LogUtil::printParam(const uh::tstring& param, const uh::tstring& value)
{
	LogUtil::print((uh::tformat(_T(" --%-10s\t%s")) % param % value).str());
}

void LogUtil::printParam(const std::string& param, const std::string& value)
{
	LogUtil::printParam(uh::toTStr(param), uh::toTStr(value));
}


void LogUtil::printTableState(const uh::tstring& name, const uh::tstring& table, bool state)
{
	uh::tstring strState;
	if(state)
	{
		strState = _T("Ú‘±OK");
	}
	else
	{
		strState = _T("Ú‘±NG");
	}

	LogUtil::printTableState(name, table, strState);
}

void LogUtil::printTableState(const std::string& name, const std::string& table, bool state)
{
	LogUtil::printTableState(uh::toTStr(name), uh::toTStr(table), state);
}

void LogUtil::printTableState(long id, const std::string& table, bool state)
{
	uh::tstring strState;
	if(state)
	{
		strState = _T("Ú‘±OK");
	}
	else
	{
		strState = _T("Ú‘±NG");
	}
	if(state)
	{
		LogUtil::print((uh::tformat(_T("\t%-40s\t%s")) % uh::toTStr(table) % strState).str());
	}
	else
	{
		LogUtil::printError((uh::tformat(_T("%ld\t%-40s\t%s")) % id % uh::toTStr(table) % strState).str());
	}
}

void LogUtil::printTableState(const uh::tstring& name, const uh::tstring& table, const uh::tstring& state)
{
	uh::tstring nametable = name + _T("(") + table  + _T(")");
	LogUtil::print((uh::tformat(_T(" %-40s\t%s")) % nametable % state).str());
}

void LogUtil::printTableState(const std::string& name, const std::string& table, const std::string& state)
{
	LogUtil::printTableState(uh::toTStr(name), uh::toTStr(table), uh::toTStr(state));
}

void LogUtil::print(long id, const std::string& errmsg)
{
	std::string msg = " errID:" + std::to_string(id) + "/t" + errmsg;
	
	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
}

void LogUtil::print(long id, const std::string& errmsg, const std::string& value,const std::string& target)
{
	std::string msg = " errID:" + std::to_string(id) + "\t" + errmsg + "\t" + value + "\t" + target;

	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
}

void LogUtil::printDebug(long id, const std::string& errmsg)
{
#ifdef _DEBUG
	std::string msg = "\t[DEBUG]errID:" + std::to_string(id) + "\t" + errmsg;
	
	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
#endif
}

void LogUtil::printDebug(long id, const std::string& errmsg, const std::string& value,const std::string& target)
{
#ifdef _DEBUG
	std::string msg = "\t[DEBUG]errID:" + std::to_string(id) + "\t" + errmsg + "\t" + value + "\t" + target;

	std::cout << msg << std::endl;
	_strLogMsg << msg << std::endl;
#endif
}

void LogUtil::printWarning(long id, const std::string& errmsg)
{
	std::string msg = "\t[WAR]errID:" + std::to_string(id) + "\t" + errmsg;
	
	std::cerr << msg << std::endl;
	_strLogMsg << msg << std::endl;

	_warCount++;
}

void LogUtil::printWarning(long id, const std::string& errmsg, const std::string& value,const std::string& target)
{
	std::string msg = "\t[WAR]errID:" + std::to_string(id) + "\t" + errmsg + "\t" + value + "\t" + target;

	std::cerr << msg << std::endl;
	_strLogMsg << msg << std::endl;

	_warCount++;
}

void LogUtil::printError(long id, const std::string& errmsg)
{
	std::string msg = "\t[ERR]errID:" + std::to_string(id) + "\t" + errmsg;
	
	std::cerr << msg << std::endl;
	_strLogMsg << msg << std::endl;

	_errCount++;
}

void LogUtil::printError(long id, const std::string& errmsg, const std::string& value,const std::string& target)
{
	std::string msg = "\t[ERR]errID:" + std::to_string(id) + "\t" + errmsg + "\t" + value + "\t" + target;

	std::cerr << msg << std::endl;
	_strLogMsg << msg << std::endl;

	_errCount++;
}
