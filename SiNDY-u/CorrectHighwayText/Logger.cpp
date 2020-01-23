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

#include <WinLib/VersionInfo.h>
#include <atltime.h>

#include "Logger.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// boost
#include <boost/filesystem/path.hpp>

// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex CRunLogger::s_mutex;

// �t�@�C���I�[�v��
bool CLogger::Initialize(const Arguments& args)
{
	if (!m_Log || m_Log.is_open())
		return false;

	m_Log.open(uh::toStr(args.m_runlog.c_str()));
	if (!m_Log)
		return false;

	writeHeader(args);

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log(const uh::tstring& strMessage)
{
	m_Log << uh::toStr(strMessage) << std::endl;
}


// ���O�փ��b�Z�[�W�o��
void CLogger::Log(const TCHAR* format, ...)
{
	CString cstr;
	if (format)
	{
		va_list args;
		va_start(args, format);
		cstr.FormatV(format, args);
		va_end(args);
	}
	Log(uh::toTStr(cstr.GetString()));
}


// << �I�y���[�^�̃I�[�o�[���[�h
CLogger& CLogger::operator<<(const uh::tstring& strMessage)
{
	m_Log << uh::toStr(strMessage);
	return *this;
}



// �G���[���x���t���̃��b�Z�[�W�o��
void CRunLogger::Fatal(const uh::tstring& strMessage)
{
	Log(_T("%s FATAL -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Error(const uh::tstring& strMessage)
{
	Log(_T("%s ERROR -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Warn(const uh::tstring& strMessage)
{
	Log(_T("%s WARN  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Info(const uh::tstring& strMessage)
{
	Log(_T("%s INFO  -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}

void CRunLogger::Debug(const uh::tstring& strMessage)
{
	Log(_T("%s DEBUG -- : %s"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str());
}


void CRunLogger::writeHeader(const Arguments& args)
{
	// �c�[����
	CVersion version;
	Log(_T("%s PRODUCTVERSION %s"), version.GetInternalName(), version.GetProductVersion());
	Log(_T("�ڑ�����\t%s"), args.m_target.c_str());
	Log(_T("�J�n����:%s"), log_utility::GetDateTimeInfo().c_str());
}


void CRunLogger::writeExitLog(bool ret)
{
	Log(_T("�I������:%s"), log_utility::GetDateTimeInfo().c_str());
	Log(ret ? _T("����I��") : _T("�ُ�I��"));
}

namespace log_utility
{
	// �����̏��𕶎���ŕԂ�
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format(_T("[%Y-%m-%d %H:%M:%S]"));

		return uh::toTStr(strTime.GetString());
	}
}
