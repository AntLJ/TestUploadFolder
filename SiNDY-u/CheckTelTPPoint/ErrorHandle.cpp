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


const LPCTSTR CErrorHandle::m_lpszErrMsg[] = 
{
	_T("\t\t"),	// �s���ȃG���[�R�[�h�p
	_T("WARNING\tW1\t11���R�[�h�s��v(������)"),
	_T("WARNING\tW2\t11���R�[�h�s��v(������)"),
	_T("WARNING\tW3\t8���R�[�h�s��v"),
	_T("WARNING\tW4\t5���R�[�h�s��v"),
	_T("NOTICE\tN5\t�ۗ��ł���"),
	_T("ERROR\tE6\t�������ʃR�[�h�����t�^�R��"),
	_T("ERROR\tE7\t�s���̒����R�[�h"),
	_T("ERROR\tE8\t�s���E�̋��E��ɂ���܂��͍s���E��ɂȂ�"),
	_T("WARNING\tW9\t�d�b�ԍ����t���[�_�C����"),
	_T("ERROR\tE10\t�ʒu�s���i�s�s�n�}�w�i : %s�j"),
	_T("ERROR\tE11\t�ʒu�s���i���k�w�i : %s�j"),
	_T("ERROR\tE12\t�s�s�n�}�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���"),
	_T("ERROR\tE13\t���k�w�i�̋��E��ɂ���̂Ń`�F�b�N�ł��܂���"),
	_T("ERROR\tE14\t�|�C���g�S�Œ������ʃR�[�h���Ⴄ�|�C���g������"),
	_T("ERROR\tE15\t�g�p�s��POI�t�߂ɐ���"),							// Bug 5701�Ή� 07/11/30�ǉ�
	_T("ERROR\tE16\t����V�[�P���X�����L��")							// Bug 9295�Ή� 12/09/20�ǉ�
};

// ���O�o��
void CErrorHandle::printLog( const CTelTP& ob, int nErrorCode, const std::string* lpstrBGDomain /* = NULL */ )
{
	// �o�͊J�n  �u�Z�����v�u20���Z���R�[�h�v�u�������́v�u�f�ږ��́v�u�������ʁv�u���\�[�X�v�u�d�b�ԍ��v�u���W�t�^��ʁv
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< getErrorMsg( nErrorCode, lpstrBGDomain ) << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t"
		<< ob.m_strTelNum << "\t" << ob.getAddXYDomain() << "\t"
		<< ob.m_lSequence << std::endl;
}

// ���O�o��(�����ǉ��� Bug 5701�Ή� 07/11/30�ǉ�)
void CErrorHandle::printLog( const CTelTP& ob, int nErrorCode, const double dist )
{
	// �o�͊J�n  �u�Z�����v�u20���Z���R�[�h�v�u�������́v�u�f�ږ��́v�u�������ʁv�u���\�[�X�v�u�d�b�ԍ��v�u���W�t�^��ʁv�uTelTP��POI�Ƃ̋����v
	m_fout 
		<< 0 << "\t" << m_strFCName << "\t" << ob.getOID() << "\t\t\t"
		<< m_lpszErrMsg[nErrorCode] << "\t"
		<< ob.m_strAddrName << "\t" << ob.m_strAddrCode << "\t" 
		<< ob.m_strKatagaki << "\t" << ob.m_strKeisai << "\t" 
		<< ob.getPointStatDomain() << "\t" << ob.m_strSource << "\t" << ob.m_strTelNum  << "\t"
		<< ob.getAddXYDomain() << "\t" << ob.m_lSequence << "\t"
		<< dist << std::endl;
}

// �G���[�R�[�h����G���[���b�Z�[�W�擾
const std::string CErrorHandle::getErrorMsg( int nErrorCode, const std::string* lpstrBGDomain /* = NULL */ ) const
{
	switch( nErrorCode )
	{
	case E_ON_NGCITYSITE:
	case E_ON_NGBASESITE:
		{
			char szTmp[150] = {0};
			sprintf_s( szTmp, sizeof(szTmp), m_lpszErrMsg[nErrorCode], lpstrBGDomain->c_str() );

			return szTmp;
		}
		break;

	default:
		return m_lpszErrMsg[nErrorCode];
	}
}

