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

CString Log::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch (errCode)
	{
		// �I�v�V�����֘A
		case NgDBConnect:			return _T("DB�ڑ����s(%s)");
		case NgOpenTable:			return _T("%s�e�[�u���I�[�v�����s");
		case NgGetIndex:			return _T("%s�e�[�u��%s�t�B�[���h�̃C���f�b�N�X�擾���s");
		case NgSetFilter:			return _T("%s�e�[�u���̌����t�B���^�ݒ莸�s");
		case NgGetRowCount:			return _T("%s�e�[�u���̃��R�[�h���擾���s");
		case NgGetCursor:			return _T("%s�e�[�u���̃J�[�\���擾���s");
		case NgGetOID:				return _T("%s�e�[�u����OBJECTID�擾���s");
		case NgGetVal:				return _T("%s�擾���s");

		case FailOpenInputFile:		return _T("�t�@�C���I�[�v���Ɏ��s���܂����B%s");
		case NgDictionaryFieldNum:	return _T("�����t�@�C���Ƀt�B�[���h�����s���ȍs������܂�:%d�s");
		case NgDictionaryFieldVal:	return _T("�����t�@�C����NULL�t�B�[���h�����݂��܂�:%d�s");
		case NgDictionaryRecord:	return _T("�����t�@�C���ɕ����O��ŕ����񂪈Ⴄ�s�����݂��܂��F%d�s");
		case DuplicateDictionary:	return _T("��������ɑ΂��鎫���f�[�^���������݂��܂��B:d�s");
		case DuplicateData:			return _T("���R�[�h���d�����Ă��܂�");
		case NoRelData:				return _T("�֘A�t��%s���R�[�h�����݂��Ȃ�");
		case NullStrData:			return _T("�\���p���̂ւ̕�������H�����̌��ʁA���̂�NULL�ƂȂ������ߕϊ����΂��܂�");

		case FailDelFile:			return _T("�t�@�C���폜�Ɏ��s�B�t�@�C�����F%s");
		case NoInited:				return _T("�o�̓N���X������������Ă��܂���");
		case FailOpenOutputFile:	return _T("�o�̓t�@�C���I�[�v���Ɏ��s�B�t�@�C�����F%s");

		case NgSplitFunc:			return _T("��`����Ă��Ȃ������t�@���N�V�������w�肳��Ă��܂�");
		case NgParenConsis:			return _T("�i�j�̈ʒu�����̂Ɠǂ݂Ő��������Ă��܂���");
		case NgSpClsConsis:			return _T("�������������̂Ɠǂ݂Ő��������Ă��܂���");
		case DiffSplitCount:		return _T("���̂Ɠǂ݂ŕ��������قȂ�܂�");
		case UnsymmetricalParen:	return _T("�J�b�R�����Ă��Ȃ������񂪑��݂��܂�");
		case UnsymmetricalWord:		return _T("������̖��̂Ɠǂ݂̒��ɁA�Е��̂�NULL�̂��̂����݂��܂�");

		default:					return _T("��`����Ă��Ȃ��G���[�R�[�h�ł�");
	}
}

CString Log::GetErrTypeMsg(err_type::ECode errType)
{
	using namespace err_type;
	switch (errType)
	{
		case fatal:		return _T("FATAL");
		case error:		return _T("ERROR");
		case warning:	return _T("WARNING");
		case info:		return _T("INFO");
		default:		return _T("UNKNOWN");
	}
}

void Log::PrintLog1(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, errCode);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t\t\t%ld\t%s\t"), GetErrTypeMsg(errType), errCode, strMsg);
	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
	return;
}

void Log::PrintLog2(bool bCout,
				   	 bool bCerr,
					 bool bRun,
					 bool bErr,
					 err_type::ECode errType,
					 err_code::ECode errCode,
					 const CString& strTableName,
					 const long oid,
					 ...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, oid);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	// �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�������Őݒ�
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t%s\t%ld\t%ld\t%s\t"), GetErrTypeMsg(errType), strTableName, oid, errCode, strMsg);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
	return;
}


void Log::PrintLog(bool bCout,
	bool bCerr,
	bool bRun,
	bool bErr,
	const CString& strMsg,
	bool bLineBreak)
{
	if (bCout) {
		if (bLineBreak)
			std::cout << CT2A(strMsg) << std::endl;
		else
			std::cout << CT2A(strMsg);
	}

	if (bCerr) {
		if (bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg);
	}

	if (bRun && m_ofsRunLog.is_open()) {
		if (bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg);
	}

	if (bErr && m_ofsErrLog.is_open()) {
		if (bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg);
	}
	return;
}

bool Log::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if (!ofsLogFile.is_open()) {
		std::cerr << CT2A(strFilePath) << "�̃I�[�v���Ɏ��s" << std::endl;
		return false;
	}
	return true;
}

bool Log::SetRunLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsRunLog);
}

bool Log::SetErrLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsErrLog);
}
