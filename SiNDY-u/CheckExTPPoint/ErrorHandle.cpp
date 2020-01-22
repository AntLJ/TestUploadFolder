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

// ErrorHandle.cpp: CErrorHandle �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorHandle.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

const LPCTSTR CErrorHandle::m_lpszErrMsg[] = 
{
	_T("\t\t"),	// �s���ȃG���[�R�[�h�p
	_T("NOTICE\tN1\tCS�|�C���g�Ŏ擾(������)"),
	_T("WARNING\tW2\t11���R�[�h�s��v(������)"),
	_T("ERROR\tE3\t�X�敄�����قȂ�(������)"),
	_T("ERROR\tE4\tGOU�g�������Ȃ̂ɖ�����(������)"),
	_T("WARNING\tW5\t11���R�[�h�s��v(������)"),
	_T("ERROR\tE6\t�������ʃR�[�h�I�����(TPG����)"),
	_T("NOTICE\tN7\t11���R�[�h�s��v(������)"),
	_T("WARNING\tW8\t11���R�[�h�s��v(������)"),
	_T("NOTICE\tN9\t�X�敄�����قȂ�(������)"),
	_T("NOTICE\tN10\t11���R�[�h�s��v(������)"),
	_T("WARNING\tW11\t11���R�[�h�s��v(������)"),
	_T("NOTICE\tN12\t�X�敄�����قȂ�(������)"),
	_T("WARNING\tW13\t�s���E�s��v�Ȃ̂Ƀf�[�^�ƈ�v(������)"),
	_T("WARNING\tW14\t�s���E�s��v�Ȃ̂Ƀf�[�^�ƈ�v(������)"),
	_T("ERROR\tE15\t�������ʃR�[�h�����t�^�R��"),
	_T("WARNING\tW16\t5���R�[�h�s��v"),
	_T("ERROR\tE17\t�X�敄���g���̏�Ƀ|�C���g"),
	_T("ERROR\tE18\t�s���̒����R�[�h"),
	_T("ERROR\tE19\t�s���E�̋��E��ɂ���܂��͍s���E��ɂȂ�"),
	_T("ERROR\tE20\t�ʒu�s��(�s�s�n�}�w�i : %s)"),
	_T("ERROR\tE21\t�ʒu�s��(���k�w�i : %s)"),
	_T("ERROR\tE22\t�s�s�n�}�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���"),
	_T("ERROR\tE23\t���k�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���"),
	_T("ERROR\tE24\t�X�敄���i�e�n�ԁj��v�i�����ς݁j"),
	_T("WARNING\tW25\t8���R�[�h�s��v�iGOU�����j"),
	_T("WARNING\tW26\t8���R�[�h�s��v�iGOU�g�������j"),
	_T("WARNING\tW27\t5���R�[�h�s��v[�A���v�X]"),
	_T("WARNING\tW28\t8���R�[�h�s��v[�A���v�X]"),
	_T("NOTICE\tN29\t11���R�[�h�s��v�i�����ρj[�A���v�X]"),
	_T("NOTICE\tN30\t11���R�[�h�s��v�i�������j[�A���v�X]"),
	_T("NOTICE\tN31\t�X�敄�����قȂ�i�����ρj[�A���v�X]"),
	_T("ERROR\tE32\t�ʒu�s���i�s�s�n�}�w�i : %s�j[�A���v�X]"),
	_T("ERROR\tE33\t�ʒu�s���i���k�w�i : %s�j[�A���v�X]"),
	_T("ERROR\tE34\t�s�s�n�}�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���[�A���v�X]"),
	_T("ERROR\tE35\t���k�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���[�A���v�X]"),
	_T("ERROR\tE36\t����20�������L��")		// Bug 9296�Ή�
};

// ���O�o��
void CErrorHandle::printLog( const CExTP& ob, int nErrorCode, const std::string& strBGDomain /* = "" */ )
{
	// �o�͊J�n  �u�Z�����v�u20���Z���R�[�h�v�u�������́v�u�f�ږ��́v�u�������ʁv�u���\�[�X�v�u�Z�������|�C���g���t���O�v
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< getErrorMsg( getErrorCode(nErrorCode, ob.m_ePointStatC), strBGDomain ) << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" << ob.m_lAddPointF << std::endl;
}

// ���O�o�́i����20���R�[�h�ێ�����̃I�u�W�F�N�gID�ǉ��ŁiBug 9296�Ή��j
void CErrorHandle::printLog( const CExTP& ob, int nErrorCode, const long lOID )
{
	// �o�͊J�n  �u�Z�����v�u20���Z���R�[�h�v�u�������́v�u�f�ږ��́v�u�������ʁv�u���\�[�X�v�u�Z�������|�C���g���t���O�v�u����20���R�[�h�ێ�����̃I�u�W�F�N�gID�v
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< m_lpszErrMsg[nErrorCode] << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" 
		<< ob.m_lAddPointF <<"\t" << lOID << std::endl;
}

/**
 * �����֐�
 */

// �G���[�啪�ނ���A�G���[�R�[�h���擾
int CErrorHandle::getErrorCode( int nErrorState, POINTSTAT_C ePointStat )
{
	// �啪�ރG���[�R�[�h�Ȃ�A��������
	switch( nErrorState )
	{
	case E_MISMATCH_ADDRCODE11_NOTSEIBI:
		
		switch( ePointStat )
		{
		case GOU_SEIBI:		return N_GOU_CSPOINT_NOTSEIBI;
		case REFNUM_DIFF:	return N_REF_ADDRCODE_NOTSEIBI;
		case NUM_NOTKEISAI:	return N_NUM_ADDRCODE_NOTSEIBI;
		}
		break;

	case E_MISMATCH_ADDRCODE11_SEIBI:
		
		switch( ePointStat )
		{
		case GOU_SEIBI:		return W_GOU_ADDRCODE_SEIBI;
		case GOU_EXSEIBI:	return W_EXGOU_ADDRCODE_SEIBI;
		case REFNUM_DIFF:	return W_REF_ADDRCODE_SEIBI;
		case NUM_NOTKEISAI:	return W_NUM_ADDRCODE_SEIBI;
		}
		break;
	
	case E_MISMATCH_GAIKU:

		switch( ePointStat )
		{
		case GOU_SEIBI:		return E_GOU_GAIKU_SEIBI;
		case REFNUM_DIFF:	return N_REF_GAIKU_NOTSEIBI;
		case NUM_NOTKEISAI:	return N_NUM_GAIKU_NOTSEIBI;
		}
		break;

	case E_MATCH_GAIKU:

		switch( ePointStat )
		{
		case GOU_EXSEIBI:	return E_EXGOU_GAIKU_SEIBI;
		}
		break;

	default:
		return nErrorState;
	}

	return nErrorState;
}

// �G���[�R�[�h����G���[���b�Z�[�W�擾
const std::string CErrorHandle::getErrorMsg( int nErrorCode, const std::string& strBGDomain /* = "" */ ) const
{
	switch( nErrorCode )
	{
	case E_ON_NGCITYSITE:
	case E_ON_NGBASESITE:
	case E_ALPS_ON_NGCITYSITE:
	case E_ALPS_ON_NGBASESITE:
		{
			char szTmp[150] = {0};
			sprintf_s( szTmp, sizeof(szTmp), m_lpszErrMsg[nErrorCode], strBGDomain.c_str() );

			return szTmp;
		}
		break;

	default:
		return m_lpszErrMsg[nErrorCode];
	}
}

