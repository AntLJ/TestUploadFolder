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
#include "ErrorMsg.h"

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
		case NgDlcBldWithin:             return _T("Within���Ă��Ȃ�");
		case NgBldDlcContains:           return _T("Contains���Ă��Ȃ�");
		case NgBldDlcTouches:            return _T("Touches���Ă��Ȃ�");
		case NgBldDlcDisjoint:           return _T("Disjoint���Ă��Ȃ�");
		case NgBldDlcCross:              return _T("Cross���Ă���");
		case DlcBldWithin:               return _T("Within���Ă���");
		case BldDlcContains:             return _T("Contains���Ă���");
		case BldDlcTouches:              return _T("Touches���Ă���");
		case BldDlcDisjoint:             return _T("Disjoint���Ă���");
		case BldDlcCross:                return _T("Cross���Ă���");
		case NgExistBld1:                return _T("�Ώ�Bld����NG");
		case NgExistBld2:                return _T("�Ώ�Bld����NG�A�א�Bld����NG");
		case NgExistBld3:                return _T("�Ώ�Bld����NG�A�א�Bld����OK");
		case NgExistBld4:                return _T("�Ώ�Bld����NG�A�א�Bld����OK");
		case NgExistBld5:                return _T("�Ώ�Bld�{�א�Bld�`�����OK");
		case NgBldDlcContTouch:          return _T("�Ώ�Bld����NG");
		case NgBldDlcContTouchNear:      return _T("�א�Bld����NG");
		case NgBldDlcRelation:           return _T("Dlc��Bld�̌`��֌W���s��");

		case NgOther:     return _T("������");
		
		default:                  return _T("�G���[�ł�");
	}
}

CString CLog::GetErrTypeMsg(err_type::ECode errType)
{
	using namespace err_type;
	switch(errType)
	{
		case fatal:   return _T("FATAL");
		case error:   return _T("ERROR");
		case warning: return _T("WARNING");
		case info:    return _T("INFO");
		default:      return _T("UNKOWN");
	}
}

void CLog::PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			const CString& strMsg1,
			const CString& strMsg2,
			const CString& strMsg3)
{
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t\t\t\t%s\t%s\t%s\t\t"), GetErrTypeMsg(errType), strMsg1, strMsg2, strMsg3);
	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					long lOID,
					const CString& strOtherMsg1,
					const CString& strOtherMsg2,
					const CString& strOtherMsg3)
{
	// �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�������Őݒ�
	CString strPrintMsg;
	if(0 <= lOID)
		strPrintMsg.Format(_T("%s\t%s\t%ld\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, lOID, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);
	else
		strPrintMsg.Format(_T("%s\t%s\t\t%ld\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					const CString& strMsg,
					bool bLineBreak)
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

bool CLog::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if(!ofsLogFile.is_open()){
		std::cerr << CT2A(strFilePath) << "�̃I�[�v���Ɏ��s" << std::endl;
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
