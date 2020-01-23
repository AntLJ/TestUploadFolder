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
#include "LogManager.h"
#include "ErrorLevelDef.h"
#include "CheckStopPoint.h"

using namespace std;

// �R���X�g���N�^
CLogManager::CLogManager(void)
{
}

// �f�X�g���N�^
CLogManager::~CLogManager(void)
{
	Close();
}

// �����ݒ�
bool CLogManager::Init( const CString& logPath )
{
	return Open( logPath );
}

// ���O�t�@�C���I�[�v��
bool CLogManager::Open( const CString& logPath )
{
	m_ofs.open( logPath );
	if( !m_ofs || !m_ofs.is_open() ){
		cerr << "#ERROR ���O�t�@�C�����J���܂���F" << CStringA( logPath ) << endl;
		return false;
	}
	return true;
}

// ���O�t�@�C���N���[�Y
void CLogManager::Close()
{
	if( m_ofs.is_open() )
	{
		WriteInfo( "#�I������ : %s", GetTime() );
		m_ofs.close();
	}
}

// ���ݎ����擾
CStringA CLogManager::GetTime()
{
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CStringA strTime;
	strTime.Format( "%d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
}

// ���o���[�h�̃��O�w�b�_�[�o��
void CLogManager::WriteExtractLogHeader()
{
	WriteInfo( "#FREESTYLELOG" );
	CStringA header;
	header = "FLAG\tLAYER\tOBJECTID\tMESHCODE\tERRORLEVEL\tMESSAGE";
	// ���{���Ǝ����H���ő�HSiNDY�ゾ�Ƃ܂��Ⴄ��������Ȃ�����10����Α���邾�낤
	for( long i = 1; i <= 10; ++i )
	{
		CStringA str;
		str.Format( "\tROAD_LINK%d", i );
		header += str;
		str.Format( "\tROADCLASS_C%d", i );
		header += str;
	}
	WriteInfo( header );
	WriteInfo( "#�J�n���� : %s", GetTime() );
}

// �`�F�b�N���[�h�̃��O�w�b�_�[�o��
void CLogManager::WriteCheckLogHeader()
{
	WriteInfo( "#FREESTYLELOG" );
	CStringA header;
	header = "FLAG\tLAYER\tOBJECTID\tLON\tLAT\tERRORLEVEL\tERRORCODE\tMESSAGE";
	// Info1�`3�̃J��������ǉ�
	for( int i=1; i<=3; ++i )
	{
		header.AppendFormat("\tInfo%d", i);
	}
	header.AppendFormat("\tCOMMENT");
	WriteInfo( header );
	WriteInfo( "#�J�n���� : %s", GetTime() );
}

// ���O�ɔC�ӂ̕�����o��
void CLogManager::WriteInfo( LPCSTR message, ... )
{
	va_list vaList = NULL;
	va_start( vaList, message );

	CStringA strLog;
	strLog.FormatV( message, vaList );
	m_ofs << strLog << endl;
}

// ���o���O�o��
void CLogManager::WriteExtractLog( const CStringA& layer, long oid, long meshcode, const CStringA& errlevel, const CStringA& message, const CStringA& linkInfo )
{
	m_ofs << "0\t" << layer << "\t" << oid << "\t" << meshcode << "\t" << errlevel << "\t" << message << "\t" << linkInfo << endl;
}

/**
 * @brief �G���[���x�����當������擾
 * @param elevel [in] �G���[���x��
 * @return �G���[���x����\��������
 */
inline CStringA convErrorLevel2Str(ERROR_LEVEL elevel)
{
	switch(elevel)
	{
	case kError:
		return "ERROR";
	case kWarn:
		return "WARNING";
	default:
		break;
	}
	return "";
}

// �`�F�b�N���O�o��
void CLogManager::WriteCheckLog( const CStringA& layer, long oid, long errorCode, double lon, double lat, long meshcode, long linkID, long nodeID )
{
	// �o�ܓx�i�[�p������
	CStringA strLon, strLat;
	//�o�ܓx�͏����_�ȉ���7���ŏo��
	strLon.Format("%.7lf", lon);
	strLat.Format("%.7lf", lat);

	m_ofs << "0\t" << layer << "\t" << oid << "\t" << strLon << "\t" << strLat << "\t";

	// �G���[�R�[�h�ƃ��b�Z�[�W���o��
	switch(errorCode)
	{
#define ERROR_CODE(code,level,...) case code: m_ofs << convErrorLevel2Str(level) << "\t" << code << "\t" << #__VA_ARGS__; break;
#include "ErrorCode.h"
#undef ERROR_CODE
		default: m_ofs << "���̃G���[�R�[�h�ɂ͑Ή����Ă��܂���\tERROR\t" << errorCode << "\t"; break;
	}
	m_ofs << "\t" << meshcode << "\t" << linkID << "\t" << nodeID << "\t" << endl;
}
