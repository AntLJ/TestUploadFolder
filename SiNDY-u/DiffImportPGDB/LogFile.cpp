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
#include "LogFile.h"
#include <TDC/useful_headers/str_util.h>
#include <WinLib/VersionInfo.h>

CLogFile::CLogFile( const CString& baseDirectory )
{
	m_logDirctory = baseDirectory;
}


CLogFile::~CLogFile(void)
{
}


void CLogFile::WriteLog( const CString& layer, long objectId, ErrorCode errorCode, const CString& message, long meshCode )
{
	CStringA stdLog;
	stdLog.Format(
		"%s\t%s\t\t\t%s\t0\t%s\t%s",
		(CStringA)layer, 
		(CStringA)uh::str_util::ToString( objectId ),
		ChangeErrorCode2ErrorLevel( errorCode ),
		(CStringA)message,
		(CStringA)uh::str_util::ToString( meshCode )
		);
	CStringA errLog;
	errLog.Format( "0\t%s", stdLog );

	auto& execMesh = m_execMesh.find( meshCode );
	if( execMesh == m_execMesh.end() )
	{
		cerr << errLog << endl;
		return;
	}

	if( errorCode == error || errorCode == critical )
	{
		// Error, Critical�͕W���o�͂ɂ��o��
		cout << stdLog << endl;
	}
	
	WriteLogCore(meshCode, errLog);
	return;
}

void CLogFile::WriteLog( const CString& layer, long objectId, double lon, double lat, ErrorCode errorCode,
						 const CString& message, long meshCode )
{
	CStringA stdLog;
	stdLog.Format(
		"%s\t%s\t%lf\t%lf\t%s\t0\t%s\t%s",
		(CStringA)layer, 
		(CStringA)uh::str_util::ToString( objectId ),
		lon,
		lat,
		ChangeErrorCode2ErrorLevel( errorCode ),
		(CStringA)message,
		(CStringA)uh::str_util::ToString( meshCode )
		);
	CStringA errLog;
	errLog.Format( "0\t%s", stdLog );

	auto& execMesh = m_execMesh.find( meshCode );
	if( execMesh == m_execMesh.end() )
	{
		cerr << errLog << endl;
		return;
	}

	if( errorCode == error || errorCode == critical )
	{
		// Error, Critical�͕W���o�͂ɂ��o��
		cout << stdLog << endl;
	}

	WriteLogCore(meshCode, errLog);
	return;
}

void CLogFile::WriteLogCore(long meshCode, const CStringA& errLog)
{
	CString logFileName;
	logFileName.Format( _T("%s\\%d.txt"), m_logDirctory, meshCode );
	std::ofstream ofs( logFileName, ios::out|ios::app );

	if( !ofs.is_open() )
	{
		cerr << (LPCSTR)errLog << endl;
		return;
	}

	ofs << (LPCSTR)errLog << endl;
	ofs.close();
}

void CLogFile::InitLogFile( long meshCode )
{
	CString logFileName;
	logFileName.Format( _T("%s\\%d.txt"), m_logDirctory, meshCode );
	std::ofstream ofs( logFileName, ios::out|ios::app );

	if( !ofs.is_open() )
	{
		cerr << "���O�t�@�C�����J���܂���ł����F" << meshCode << endl;
		return;
	}

	ofs << "#SINDYSTDLOG" << endl;

	time_t startTime;
	time(&startTime);
	// �c�[���o�[�W����
	CVersion cVer;
	CStringA version;
	version.Format(
		"#%s FILEVERSION %s PRODUCTVERSION %s\n",
		(CStringA)cVer.GetOriginalFilename(),
		(CStringA)cVer.GetFileVersion(),
		(CStringA)cVer.GetProductVersion());
	ofs << version << endl;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CStringA time;
	time.Format( "#�J�n����: %s\n", (CStringA)uh::str_util::ToString( &sysTime ) );
	ofs << time << endl;

	ExecTime execTime;
	execTime.m_startTime = startTime;
	m_execMesh[meshCode] = execTime;

	ofs.close();
}

void CLogFile::CloseLogFile()
{
	for( auto& execMesh : m_execMesh )
	{
		CString logFileName;
		logFileName.Format( _T("%s\\%d.txt"), m_logDirctory, execMesh.first );
		std::ofstream ofs( logFileName, ios::out|ios::app );
		if( !ofs.is_open() )
			continue;

		time(&execMesh.second.m_endTime);
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		CStringA time;
		time.Format( "#�I������: %s\n", (CStringA)uh::str_util::ToString( &sysTime ) );
		ofs << (LPCSTR)time << endl;

		int hour = 0, minute = 0, second = 0;
		double t = difftime( execMesh.second.m_endTime, execMesh.second.m_startTime);
		hour = int(t/3600);
		minute = (int(t)%3600)/60;
		second = int(t)%60;
		CStringA totalTime;
		totalTime.Format("#�g�[�^������ %d����%d��%d�b\n", hour, minute, second);
		ofs << (LPCSTR)totalTime << endl;

		ofs.close();
	}
}
