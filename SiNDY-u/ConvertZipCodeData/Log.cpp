#include "stdafx.h"
#include "Log.h"

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
		case NgAddrDBConnectString:			return _T("�Z��DB�ڑ�������s��(%s)");
		
		// �X�֔ԍ��ƏZ���R�t��
		case NgDuplicateRelAddr:			return _T("�X�֔ԍ��ɕR�t���Z�����d��(%s:%s)");
		case NgAddrCode:					return _T("�Z���R�[�h�̌������s��(%s:%s)");
		case NgAddr11Dir:					return _T("�Z���f�[�^�̃f�B���N�g���Ƀt�@�C�������݂��Ȃ�(%s)");
		case NgAddr11File:					return _T("�Z���f�[�^�̃f�B���N�g����addr**.txt�ȊO�̃t�@�C�������݂���(%s)");
		case NgAddr11FileOpen:				return _T("�t�@�C���I�[�v�����s(%s)");
		case NgAddr11Record:				return _T("�Z���f�[�^�̃��R�[�h����������(%s)");
		case NgAddr11Duplicate:				return _T("11���R�[�h���d��(%s)");
		case NgAddr11AccCode2:				return _T("�Z���R�[�h��11���Ő��x2(%s)");

		// �X�֔ԍ��|���S��
		case NgZipPriority:					return _T("�D��x�ɑΉ�����X�֔ԍ������݂��Ȃ�(%s)");
		case NgOverlapZipValue:				return _T("�X�֔ԍ��|���S����ZIPCODE1,ZIPCODE2�̐ݒ肪��������(%s,%s)");
		case NgSameZipCode:					return _T("ZIPCODE1,ZIPCODE2�ɓ����X�֔ԍ����ݒ肳��Ă���");

		// �d�S
		case NgGetCentroid:					return _T("�d�S�擾���s");
		case NgGetCentroid2:				return _T("�d�S�擾���s�i��O�j");

		// �s�撬����\�_
		case NgDuplicateAddrRec:			return _T("��\�_����������(%s)");
		case NgAddrCode5Digit:				return _T("�Z���R�[�h��5���łȂ�(%s)");
		case NgZipCodeUnder00:				return _T("�X�֔ԍ��̉�2�����u00�v�łȂ�(%s)");

		// ArcObjects�֘A
		case NgDBConnect:					return _T("DB�ڑ����s(%s)");
		case NgOpenTable:					return _T("�e�[�u���I�[�v�����s");
		case NgPutWhereClause:				return _T("�N�G���t�B���^�ݒ莸�s");
		case NgGetCursor:					return _T("�J�[�\���擾���s");
		case NgGetFields:					return _T("�t�B�[���h�Q�擾���s");
		case NgGetFieldCount:				return _T("�t�B�[���h���擾���s");
		case NgGetField:					return _T("%ld�t�B�[���h�ڎ擾���s");
		case NgGetFieldName:				return _T("�t�B�[���h���擾���s");
		case NgGetValue:					return _T("�f�[�^�擾���s");
		case NgGetEnumDatasetName:			return _T("�f�[�^�Z�b�g�����X�g�擾���s(%s)");
		case NgResetEnumDatasetName:		return _T("�f�[�^�Z�b�g�����X�g���Z�b�g���s(%s)");
		case NgGetTableName:				return _T("�f�[�^�Z�b�g���̖��̎擾���s(%s)");
		case NgDeleteTable:					return _T("�e�[�u���폜���s");
		case NgGetShape:					return _T("�`��擾���s");
		case NgGetOID:						return _T("OID�擾���s");
		case NgQueryInterface:				return _T("QueryInterface���s");

		case NgPutName:						return _T("�t�B�[���h���̐ݒ莸�s(%s)");
		case NgPutType:						return _T("�t�B�[���h�^�C�v�ݒ莸�s(%s)");
		case NgPutEditable:					return _T("�ҏW���ݒ莸�s(%s)");
		case NgPutIsNullable:				return _T("NULL���ݒ莸�s(%s)");
		case NgPutLength:					return _T("�����ݒ莸�s(%s)");
		case NgPutPrecision:				return _T("Precision�ݒ莸�s(%s)");
		case NgPutSpRef:					return _T("��ԎQ�Ɛݒ莸�s");
		case NgPutGeoType:					return _T("�`��^�C�v�ݒ莸�s");
		case NgPutGeoDef:					return _T("GeometryDef�ݒ莸�s");
		case NgPutCoords:					return _T("�|�C���g�ݒ莸�s(%s)");
		case NgAddField:					return _T("�t�B�[���h�ǉ����s(%s)");
		case NgCreateTable:					return _T("�e�[�u���쐬���s");

		case NgInsertCursor:				return _T("Insert�p�̃J�[�\���擾���s");
		case NgCreateRowBuffer:				return _T("���R�[�h�o�b�t�@�擾���s");
		case NgPutValue:					return _T("�����ݒ莸�s");
		case NgInsertRow:					return _T("���R�[�h�ǉ����s");
		case NgFlush:						return _T("Flush���s");
		case NgPutShape:					return _T("�`��ݒ莸�s");

		case NgUnion:						return _T("�`��}�[�W���s");

		// ���̑�
		case NgOther:
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
