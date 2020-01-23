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
		case NgOpenFile:          return _T("�t�@�C���I�[�v�����s");
		case NgOpenWorkspace:     return _T("���[�N�X�y�[�X�ڑ����s");
		case NgOpenTable:         return _T("�e�[�u���擾���s");
		case NgGetFields:         return _T("�t�B�[���h���X�g�擾���s");
		case NgGetField:          return _T("�t�B�[���h�擾���s");
		case NgGetRow:            return _T("���R�[�h�擾���s");
		case NgGetRowBuffer:      return _T("���R�[�h�o�b�t�@�擾���s");
		case NgGetShape:          return _T("�`��擾���s");
		//case NgGetDomain:         return _T("�h���C���擾���s");
		//case NgGetDomainField:    return _T("�h���C���t�B�[���h�擾���s");
		//case NgGetFieldMap:       return _T("�t�B�[���h�}�b�v�擾���s");
		case NgGetCursor:         return _T("�J�[�\���擾���s");
		case NgGetAttr:           return _T("�����擾���s");
		//case NgGetDomainValue:    return _T("�h���C���l�擾���s");
		//case NgGetDataset:        return _T("�f�[�^�Z�b�g�擾���s");
		case NgGetFieldType:      return _T("�t�B�[���h�^�C�v�擾���s");
		case NgGetFieldIndex:     return _T("�t�B�[���h�C���f�b�N�X�擾���s");
		case NgGetClipShape:      return _T("�N���b�v�`��擾���s");
		case NgGetGeomType:       return _T("�W�I���g���^�C�v�擾���s");
		//case NgGetPointCol:       return _T("�|�C���g�R���N�V�����擾���s");
		case NgGetVertexCount:    return _T("�\���_���擾���s");
		//case NgGetFieldLength:    return _T("�t�B�[���h���擾���s");
		case NgGetFieldPrecision: return _T("���x�擾���s");

		case NgCreateDir:         return _T("�f�B���N�g���쐬���s");
		case NgMeshCodeDigit:     return _T("���b�V���R�[�h�̌����������Ă��Ȃ�");
		case NgFieldType:         return _T("�t�B�[���h�^�C�v���ϊ��ΏۊO");
		case NgCreateClone:       return _T("�N���[���쐬���s");
		case NgGeomType:          return _T("�W�I���g���^�C�v�����Ή�");
		case NgIntersect:         return _T("�N���b�s���O���s");

		case NgMoveFile:          return _T("�t�@�C���ړ����s");
		case NgCreateTable:       return _T("�e�[�u���쐬���s");

		case NgPutRowAttr:        return _T("���R�[�h�����t�^���s");
		case NgPutRowShape:       return _T("���R�[�h�`��t�^���s");
		case NgPutFieldAttr:      return _T("�t�B�[���h�����^���s");
		case NgPutWhere:          return _T("Where��w�莸�s");
		//case NgPutSubFields:      return _T("SubFields�w�莸�s");
		case NgPutSpRel:          return _T("�`�󌟍����@�ݒ莸�s");
		case NgPutRefGeom:        return _T("�����`��ݒ莸�s");

		//case NgFGDB:              return _T("FGDB�łȂ�");
		//case NgStartEdit:         return _T("�ҏW�J�n���s");
		//case NgAbortEdit:         return _T("�ҏW�j�����s");
		//case NgStopEdit:          return _T("�ҏW�I�����s");
		//case NgStore:             return _T("Store�������s");
		case NgFlush:             return _T("Flush�������s");

		case NgAttrFormatEssField:            return _T("�K�{�����i�ϊ����ƕϊ���j���w�肳��Ă��Ȃ�");
		case NgAttrFormatOrgFieldEmpty:       return _T("�ϊ����t�B�[���h������");
		case NgAttrFormatDstFieldEmpty:       return _T("�ϊ���t�B�[���h������");
		case NgAttrFormatOrgFieldExceptNewOp: return _T("�ϊ����t�B�[���h���ɐV�K�쐬�t�B�[���h�I�v�V�����ȊO���w�肳��Ă���");
		case NgAttrFormatOrgFieldName:        return _T("�ϊ����t�B�[���h�����݂��Ȃ�");
		case NgAttrFormatDstFieldSetOp:       return _T("�ϊ���t�B�[���h�ɃI�v�V�������w�肳��Ă���");
		case NgAttrFormatOrgFieldDuplicate:   return _T("�ϊ����t�B�[���h���d�����Ă���");
		case NgAttrFormatDstFieldDuplicate:   return _T("�ϊ���t�B�[���h���d�����Ă���");
		case NgAttrFormatNewOp:               return _T("�V�K�t�B�[���h�I�v�V�����͕ϊ����t�B�[���h���i��1�t�B�[���h�j�݂̂Ɏg�p�\");
		case NgAttrFormatNoOp:                return _T("�I�v�V�����w��Ȃ��͕ϊ���t�B�[���h���i��2�t�B�[���h�j�̂݋��e");
		case NgAttrFormatUnknownOp:           return _T("�ݒ�I�v�V�����s��");
		case NgAttrFormatValue:               return _T("�ݒ�l�̃t�H�[�}�b�g�s��");
		case NgAttrFormatValueAps:            return _T("�ݒ�l�̃t�H�[�}�b�g�s���i�u'�v���s���j");
		case NgAttrFormatFieldInfo:           return _T("�t�B�[���h�ݒ�̃t�H�[�}�b�g�s��");
		case NgAttrFormatUnknownFieldInfo:    return _T("�t�B�[���h�ݒ�̐ݒ荀�ڂ��s��");
		case NgAttrFormatFieldInfoDuplicate:  return _T("�t�B�[���h�ݒ�̐ݒ荀�ڂ��d��");
		case NgAttrFormatRound:               return _T("�ۂߍ��ݐݒ�̃t�H�[�}�b�g�s��");
		case NgAttrFormatRoundAps:            return _T("�ۂߍ��ݐݒ�̃t�H�[�}�b�g�s���i�u'�v�s���j");
		case NgAttrFormatRoundDuplicate:      return _T("�ۂߍ��ݐݒ�̐ݒ荀�ڂ��d��");
		case NgAttrFormatDefaultDupicate:     return _T("�����l�ݒ�̐ݒ荀�ڂ��d��");
		case NgAttrFormatAttrAps:             return _T("�����؂�o���ݒ�̃t�H�[�}�b�g�s���i�u'�v�s���j");
		case NgAttrFormatAttrDuplicate:       return _T("�����؂�o���ݒ肪�d��");
		case NgAttrFormatNewDefault:          return _T("�V�K�쐬�t�B�[���h�ɏ����l�ݒ肪����Ă��Ȃ�");
		case NgAttrFormatNewFieldValue:       return _T("�V�K�쐬�t�B�[���h�ɑ����l�ݒ肪����Ă���");
		case NgAttrFormatNewRound:            return _T("�V�K�쐬�t�B�[���h�Ɋۂߍ��ݐݒ肪����Ă���");
		case NgAttrFormatExistDefault:        return _T("�����t�B�[���h�ɏ����l�ݒ肪����Ă���");
		case NgAttrFormatExistAttr:           return _T("�����t�B�[���h�ɑ����؂�o���l�ݒ肪����Ă���");
		case NgAttrFormatNewFieldInfoType:    return _T("�V�K�쐬�t�B�[���h�Ƀt�B�[���h�ݒ��TYPE���w�肪����Ă��Ȃ�");
		case NgAttrFormatExistFieldInfoType:  return _T("�����t�B�[���h�Ƀt�B�[���h�ݒ��TYPE���w�肪����Ă���");
		case NgAttrFormatFieldInfoType:       return _T("�t�B�[���h�ݒ��TYPE�ݒ�l���s��");
		case NgAttrFormatFieldInfoTypeValue:  return _T("�t�B�[���h�ݒ�Ō^�ɂ������������w�肳��Ă��Ȃ�");
		case NgAttrFormatFieldInfoValue:      return _T("�t�B�[���h�ݒ�ő����l���s��");
		case NgAttrFormatFieldInfoScale:      return _T("�t�B�[���h�ݒ��SCALE�ݒ�l���s��");
		case NgAttrFormatFieldInfoPrecision:  return _T("�t�B�[���h�ݒ��PRECISION�ݒ�l���s��");
		case NgAttrFormatFieldInfoLength:     return _T("�t�B�[���h�ݒ��LENGTH�ݒ�l���s��");

		case NgAddRow:            return _T("���R�[�h�ǉ����s");
		case NgAddField:          return _T("�t�B�[���h�ǉ����s");
		//case NgAddAttr:           return _T("�����ǉ����s");
		//case NgAddDomain:         return _T("�h���C���ǉ����s");

		//case NgUpdRow:            return _T("���R�[�h�X�V���s");
		//case NgUpdField:          return _T("�t�B�[���h�X�V���s");
		//case NgUpdAttr:           return _T("�����X�V���s");
		//case NgUpdDomain:         return _T("�h���C���X�V���s");

		//case NgDelRow:            return _T("���R�[�h�폜���s");
		//case NgDelField:          return _T("�t�B�[���h�폜���s");
		//case NgDelAttr:           return _T("�����폜���s");
		//ase NgDelDomain:         return _T("�h���C���폜���s");
		case NgDelFile:           return _T("�t�@�C���폜���s");

		//case GetManyRow:          return _T("�������R�[�h�擾");
		case ExistField:          return _T("�����t�B�[���h");
		case ChangePrecision:     return _T("���x�ݒ�ύX");
		

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


void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeatureClassPtr ipFeatureClass)
{
	CComBSTR bstrName;
	IDatasetPtr(ipFeatureClass)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), 0, strOtherMsg);

}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					ITablePtr ipTable)
{
	CComBSTR bstrName;
	IDatasetPtr(ipTable)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), 0, strOtherMsg);

}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeatureClassPtr ipFeatureClass,
					long lOID)
{
	CComBSTR bstrName;
	IDatasetPtr(ipFeatureClass)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), lOID, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					ITablePtr ipTable,
					long lOID)
{
	CComBSTR bstrName;
	IDatasetPtr(ipTable)->get_Name(&bstrName);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, CString(bstrName), lOID, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					IFeaturePtr ipFeature)
{
	long lOID = 0;
	ipFeature->get_OID(&lOID);
	IObjectClassPtr ipClass;
	ipFeature->get_Class(&ipClass);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, strOtherMsg, IFeatureClassPtr(ipClass), lOID);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg,
					_IRowPtr ipRow)
{
	long lOID = 0;
	ipRow->get_OID(&lOID);
	ITablePtr ipTable;
	ipRow->get_Table(&ipTable);
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, strOtherMsg, ipTable, lOID);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strOtherMsg)
{
	PrintLog(bCout, bCerr, bRun, bErr, errType, errCode, _T(""), 0, strOtherMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					long lOID,
					const CString& strOtherMsg)
{
	// �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�������Őݒ�
	CString strPrintMsg;
	if(lOID > 0)
		strPrintMsg.Format(_T("%s\t%s\t%ld\t%ld\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, lOID, errCode, GetErrCodeMsg(errCode), strOtherMsg);
	else
		strPrintMsg.Format(_T("%s\t%s\t\t%ld\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, errCode, GetErrCodeMsg(errCode), strOtherMsg);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					const CString& strMsg)
{
	if(bCout)
		std::cout << CT2A(strMsg) << std::endl;

	if(bCerr)
		std::cerr << CT2A(strMsg) << std::endl;

	if(bRun)
		m_ofsRunLog << CT2A(strMsg) << std::endl;

	if(bErr)
		m_ofsErrLog << CT2A(strMsg) << std::endl;
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
