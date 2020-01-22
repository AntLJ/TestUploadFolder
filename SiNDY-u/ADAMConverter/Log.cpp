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

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{

		// �t�@�C���֘A
		case NgLoadCnvParam:		return _T("�n��p�����[�^�t�@�C���ǂݍ��ݎ��s(%s)");
		case NgGetSubFields:		return _T("[%s]�F�ϊ����e�[�u�� �T�u�t�B�[���h�擾���s");
		case NgOpenOutputFile:		return _T("�o�̓t�@�C���I�[�v�����s(%s)");
		case NgEncode:				return _T("�G���R�[�h�^�C�v�s��");
		case NgExistField:			return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ �ϊ����e�[�u��[%s]�Ƀt�B�[���h[%s]�͑��݂��Ȃ�");

		// �o�̓t�H�[�}�b�g�֘A
		case NgOpEncode:			return _T("�I�v�V�����F�����R�[�h�I�v�V�����ɕs���l(%s)");
		case NgOpDelimiter:			return _T("�I�v�V�����F�f���~�^�I�v�V�����ɕs���l(%s)");
		case NgOpNewline:			return _T("�I�v�V�����F���s�R�[�h�I�v�V�����ɕs���l(%s)");
		case NgOpGeoref:			return _T("�I�v�V�����F���n�n�I�v�V�����ɕs���l(%s)");
		case NgOpShapeType:			return _T("�I�v�V�����F�`��^�C�v�I�v�V�����ɕs���l(%s)");
		case NgOpPolygonType:		return _T("�I�v�V�����F�|���S�������I�v�V�����ɕs���l(%s)");


		// �ϊ��e�[�u���֘A
		case NgLoadConvTable:				return _T("�ϊ��e�[�u�� �ǂݍ��ݎ��s(%s)");
		case DuplicateConvTableGroup:		return _T("[%s]�F�ϊ��e�[�u�� �O���[�v���d��");
		case NgConTableTab:					return _T("[%s]�F�ϊ��e�[�u�� ���R�[�h�^�u����%ld�łȂ�(%s)");
		case DuplicateConvTableAttribute:	return _T("[%s]�F�ϊ��e�[�u�� %s���d��");
		case UnknownConvTableAttribute:		return _T("[%s]�F�ϊ��e�[�u�� �s���ȃ^�O(%s)");
		case NoneConvTableGroup:			return _T("�ϊ��e�[�u�� �O���[�v��1�����݂��Ȃ�");
		case NgMatchGroup:					return _T("�ϊ��e�[�u���ƕϊ��t�B�[���h�Ή��\�ŃO���[�v����v���Ȃ�(%s)");
		case NoneConvTableAttribute:		return _T("[%s]�F�ϊ��e�[�u�� %s�����ݒ�");
		case NoneConvTableGroup2:			return _T("�ϊ��e�[�u�� �K�{���ڂ͐ݒ肳��Ă��邪�O���[�v�������ݒ�");
		case NgConvTableFieldName:			return _T("[%s]�F�ϊ��e�[�u�� %s %s�e�[�u�������݂��Ȃ�");
		case UnknownSearchType:				return _T("[%s]�F�ϊ��e�[�u�� �s���Ȍ����^�C�v(%s)");
		case NgOidTableName:				return _T("[%s]�F�ϊ��e�[�u�� OID�t�B�[���h���s���I�h��؂�ɂȂ��Ă��Ȃ�(%s)");

		// �ϊ��t�B�[���h�Ή��\�֘A
		case NgLoadConvField:				return _T("�ϊ��t�B�[���h�Ή��\ �ǂݍ��ݎ��s(%s)");
		case DuplicateConvFieldGroup:		return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ �O���[�v���d��");
		case NoneConvFieldGroup:			return _T("�ϊ��t�B�[���h�Ή��\ �ϊ��t�B�[���h�Ή��\ �O���[�v��1�����݂��Ȃ�");
		case NgConvFieldTab:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ ���R�[�h�^�u����%ld�ȏ�łȂ�(%s)");
		case UnknownConvFieldAttribute:		return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ �I�v�V���������s��(%s)");
		case NoneConvFieldSrcFieldComma:	return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ �ϊ����t�B�[���h�Ƀs���I�h���Ȃ�(%s)");
		case NgConvFieldFieldType:			return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s�̌^�w��ɕs���l(%s)");
		case NgConvFieldLength:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s�̗񕝂ɕs���l(%s)");
		case NgConvFieldNull:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��NULL���l�ɕs���l(%s)");
		case DuplicateConvFieldRoundValue:	return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s�̊ۂߍ��݃I�v�V������1�Α��ɂȂ�(%s %s)");
		case NgConvFieldOptionValue:		return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�l��񂪕s��");
		case DuplicateRange:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�͈͏��̐ݒ�͈͂��d��(%s%s %s%s / %s%s %s%s)");
		case NoneRequiredRange:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�͈͏��̕K�{���ڂ���");
		case NgSelectRange:					return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�͈͏��̑I�����ڂ��s��");
		case NgConfigRange:					return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�͈͏��͈̔͐ݒ肪�s��(%s%s)");
		case NoneConvFieldGroup2:			return _T("�ϊ��t�B�[���h�Ή��\ �t�B�[���h�Ή����ڂ͐ݒ肳��Ă��邪�O���[�v�������ݒ�");
		case DuplicateDstField:				return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ �o�̓t�B�[���h�����d��(%s)");
		case NgRangeValue:					return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�͈͏��̐ݒ�l�������łȂ�(%s)");
		case NgNullValue:					return _T("[%s]�F�ϊ��t�B�[���h�Ή��\ %s��%s�l��񂪋󕶎��ɂȂ��Ă���");

		// �o�̓f�[�^�쐬
		case NgShapeType:					return _T("�`��^�C�v�s��");
		case NgDataType:					return _T("�f�[�^�^�s��");
		case NgGeomType:					return _T("�W�I���g���^�C�v�s��");
		case NgTOKYOtoJGD2000_RP:			return _T("���{���n�n->���E���n�n�ϊ����s");
		case NgCoordIntegerDigitOver:		return _T("%s�l�̐������I�[�o�[(�����F%ld, �f�[�^�F%s)");
		case NgPointCoord:					return _T("�`��s��(�|�C���g)");

		// �o�̓f�[�^�`�F�b�N
		case NgCheckValidValue:				return _T("�L���l�O([%s.%s] %s)");
		case NgCheckValidRange:				return _T("�L���͈͒l�O([%s.%s] %s)");
		case NgCheckErrValue:				return _T("�G���[�l��([%s.%s] %s)");
		case NgCheckErrRange:				return _T("�G���[�͈͒l��([%s.%s] %s)");
		case NgCheckNull:					return _T("NULL�`�F�b�N�G���[([%s.%s] %s)");
		case NgCheckIntegerDigitOver:		return _T("�������s��([%s.%s] %s)");
		case NgCheckStringLength:			return _T("�����񐔕s��([%s.%s] %s)");
		case NgCheckFieldType:				return _T("�t�B�[���h�^�C�v�s��([%s.%s] %s)");

		// ArcObjects�֘A
		case NgDBConnect:					return _T("DB�ڑ����s(%s)");
		case NgOpenTable:					return _T("[%s]�F%s�e�[�u���I�[�v�����s");
		case NgGetRowCount:					return _T("[%s]�F���R�[�h���擾���s");
		case NgGetCursor:					return _T("[%s]�F�J�[�\���擾���s");
		case NgQueryDef:					return _T("[%s]�FQueryDef�擾���s");
		case NgJoinTable:					return _T("[%s]�F�e�[�u�������ݒ莸�s");
		case NgSetFilter:					return _T("[%s]�F�����t�B���^�ݒ莸�s");
		case NgEvalute:						return _T("[%s]�FEvaluate���s");
		case NgGetFields:					return _T("[%s]�F�t�B�[���h�Q�擾���s");
		case NgGetFieldCount:				return _T("[%s]�F�t�B�[���h���擾���s");
		case NgGetField:					return _T("[%s]�F%ld�t�B�[���h�ڎ擾���s");
		case NgGetGeomDef:					return _T("[%s]�F%ld�t�B�[���h�ڎ擾���s�i�`��t�B�[���h�j");
		case NgGetHasZ:						return _T("[%s]�FZ�l�g�p�t���O�擾���s");
		case NgGetHasM:						return _T("[%s]�FM�l�g�p�t���O�擾���s");
		case NgGetGeomType:					return _T("[%s]�F�`��^�C�v�擾���s");
		case NgNextRowCount:				return _T("[%s]�F�������R�[�h�擾���s");
		case NgGetValueCount:				return _T("[%s]�F�����J�����擾���s");
		case NgFindField:					return _T("[%s]�F%s %s.%s�t�B�[���h�����݂��Ȃ�");
		case NgSetSubFields:				return _T("[%s]�F�o�͑Ώۃt�B�[���h�ݒ莸�s");
		case NgSetPostfixClause:			return _T("[%s]�FPostfixClause�ݒ莸�s");
		case NgSetPrefixClause:				return _T("[%s]�FPrefixClause�ݒ莸�s");

		case NgGetValue:					return _T("�f�[�^�擾���s");
		case NgQueryInterface:				return _T("%s->%s�ϊ����s");
		case NgGetPointCount:				return _T("�\���_���擾���s");
		case NgGetPoint:					return _T("�|�C���g�擾���s");
		case NgGetExteriorRingCount:		return _T("�O�������O���擾���s");
		case NgQueryExteriorRings:			return _T("�O�������O�擾���s");
		case NgGetInteriorRingCount:		return _T("���������O���擾���s");
		case NgQueryInteriorRings:			return _T("���������O�擾���s");
		case NgQueryCoord:					return _T("�|�C���g�̈ܓx�o�x�擾���s");
		case NgGetZ:						return _T("Z�l�擾���s");
		case NgGetM:						return _T("M�l�擾���s");

		// ���̑�
		case NgChangeType:					return _T("ChangeType���s");

		default:							return _T("�G���[�ł�");
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
		default:      return _T("UNKNOWN");
	}
}

void CLog::PrintLog1(bool bCout,
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
}

void CLog::PrintLog2(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					const CString strOID,
					...)
{
	CString strMsg;
	va_list arglist;
	va_start(arglist, strOID);
	strMsg.FormatV(GetErrCodeMsg(errCode), arglist);
	va_end(arglist);

	// �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�������Őݒ�
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t%s\t%s\t%ld\t%s\t"), GetErrTypeMsg(errType), strTableName, strOID, errCode, strMsg);

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

	if(bRun && m_ofsRunLog.is_open()){
		if(bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg);
	}

	if(bErr && m_ofsErrLog.is_open()){
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
