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
		case NgRWLDuplicateData:             return _T("�f�[�^���d�����Ă���");
		case NgRWLExistFromNodeInRWN:        return _T("FROMNODE��RWN�ɑ��݂��Ȃ�");
		case NgRWLFromNodeAndRWNGeomSame:    return _T("FROMNODEID��RWN�Ǝn�_�̌`�󂪕s��v");
		case NgRWLExistToNodeInRWN:          return _T("TONODEID��RWN�����݂��Ȃ�");
		case NgRWLToNodeAndRWNGeomSame:      return _T("TONODEID��RWN�ƏI�_�̌`�󂪕s��v");
		case NgRWLSmallSegment:              return _T("�����Z�O�����g�����݂���");
		case NgRWLSameAttribute:             return _T("����OBJECTID�Q�̑������s��v");
		case NgRWLSequenceDivID:             return _T("����OBJECTID�Q��DIVID���s�A��");
		case NgRWLDuplicateVertex:           return _T("�\���_���d�����Ă���");
		case NgRWLDuplicateVertexAllData:    return _T("�S�f�[�^���ō\���_���d�����Ă���");
		case NgRWLDuplicateSegment:          return _T("�S�f�[�^���ŃZ�O�����g���d�����Ă���");
		case NgRWLFirstDivLinkFromDivIDNot0: return _T("�ŏ���DIVID���������N��FROMDIVID��0�łȂ�");
		case NgRWLLastDivLinkToDivIDNot0:    return _T("�Ō��DIVID���������N��TODIVID��0�łȂ�");
		case NgRWLLoopLink:                  return _T("���[�v�����N�����݂���");
		case NgRWLGroupLink:                 return _T("�O���[�v���ł��Ȃ�");

		case NgRWNDuplicateData:             return _T("�f�[�^���d�����Ă���");
		case NgRWNDivOIDisMinimumRWLOID:     return _T("DIVID�����m�[�hOBJECTID���ڑ������N�̍ŏ�OBJECTID�ɂȂ��Ă��Ȃ�");
		case NgRWNSequenceDivID:             return _T("DIVID��������OBJECTID�̃m�[�h�������N�̌�����DIVID���A�����Ă��Ȃ�");
		case NgRWNDuplicateGeom:             return _T("�`�󂪏d�����Ă���");
		case NgRWNExistInRWL:                return _T("RWL��FROMTONODE�ɑ��݂��Ȃ�");
		//case NgRWNGeomSameRWL:               return _T("RWL��FROMTONODE�Ɍ`���v���Ȃ�");

		case NgRSADuplicateData:             return _T("�f�[�^���d�����Ă���");

		case NgRWLExistInBefore:             return _T("�ϊ��O�f�[�^�ɑ��݂��Ȃ�");
		case NgRWLSameAttrAsBefore:          return _T("�ϊ��O�����ƕs��v");
		case NgRWLSameShapeAsBefore:         return _T("�ϊ��O�`��ƕs��v");
		case NgRWLSameAttrAsSplitData:       return _T("������f�[�^�̑������m���s��v");
		case NgRWLSameRoadAttrAsSplitData:   return _T("ROADATTR_C���ϊ��O�����ƕs��v�i�����N���������������j");
		case NgRWLSameWalkableAsSplitData:   return _T("WALKABLE_C���ϊ��O�����ƕs��v�i���s�҃m�[�h�����E���s�҃����N�}�[�W�����j");

		case NgRWNExistInBefore:             return _T("�ϊ��O�f�[�^�ɑ��݂��Ȃ�");
		case NgRWNSameAttrAsBefore:          return _T("�ϊ��O�����ƕs��v");
		//case NgRWNSameShapeAsBefore:         return _T("�ϊ��O�`��ƕs��v");

		case NgRWLExistInAfter:              return _T("�ϊ���f�[�^�ɑ��݂��Ȃ�");
		case NgRWLMergeData:                 return _T("�}�[�W�Ώۃ����N�łȂ�");
		case NgRWLMergeData2:                return _T("�}�[�W�Ώۃ����N���l�ԋ��p�łȂ�");

		case NgRWNSameShape:                 return _T("�ϊ���f�[�^�ɓ���`��̃f�[�^�����݂��Ȃ�");

		case NgHNDuplicateData:              return _T("�f�[�^���d�����Ă���");
		case NgHNExistInRWL:                 return _T("�Ή����郊���N�����݂��Ȃ�");
		case NgHNSameHeight:                 return _T("����OBJECTID�̃����N�Őڑ��_�̍������s��v");
		//case NgHNFrom0Height:                return _T("����LINKIDID�̃����N�ŘA����Ԃ̊J�n�_�̍�����0�łȂ�");
		//case NgHNTo0Height:                  return _T("����LINKIDID�̃����N�ŘA����Ԃ̏I���_�̍�����0�łȂ�");

		case NgHNExistInBefore:              return _T("�ϊ��O�f�[�^�ɑ��݂��Ȃ�");
		//case NgHNSameAttrAsBefore:           return _T("�ϊ��O�����ƕs��v");
		//case NgHNSameShapeAsBefore:          return _T("�ϊ��O�`��ƕs��v");

		case NgHNExistInAfter:               return _T("�ϊ���f�[�^�ɑ��݂��Ȃ�");
		case NgHNSameLinkIdAsAfter:          return _T("�ϊ���LINKID�ƕs��v");
		case NgHNSameAttrAsAfter:            return _T("�ϊ��㑮���ƕs��v");

		case NgHNExistInAfterRWN:            return _T("�ϊ���RWN�Ƀf�[�^�����݂��Ȃ�");
		case NgHNReconstractHNData:          return _T("�����m�[�h�f�[�^���č\�z�ł��Ȃ�");
		case NgHNReconstractNGHNData:        return _T("�����m�[�h�f�[�^���č\�z�����f�[�^���s��");

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
	strPrintMsg.Format(_T("%s\t\t\t\t\t%s\t%s\t%s\t\t"), GetErrTypeMsg(errType), strMsg1, strMsg2, strMsg3);
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
					long lDivID,
					const CString& strOtherMsg1,
					const CString& strOtherMsg2,
					const CString& strOtherMsg3)
{
	// �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�������Őݒ�
	CString strDivID;
	if(-1 != lDivID)
		strDivID.Format(_T("%ld"), lDivID);
	CString strPrintMsg;
	if(lOID > 0)
		strPrintMsg.Format(_T("%s\t%s\t%ld\t%s\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, lOID, strDivID, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);
	else
		strPrintMsg.Format(_T("%s\t%s\t\t\t%ld\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);

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
			std::cout << CT2A(strMsg) << std::flush;
	}

	if(bCerr){
		if(bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg) << std::flush;
	}

	if(bRun){
		if(bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg) << std::flush;
	}

	if(bErr){
		if(bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg) << std::flush;
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
