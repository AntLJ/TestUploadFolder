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

// CheckTelTPError.cpp: CheckTelTPError �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckTelTPError.h"
#include <WinLib/VersionInfo.h>

#include "TelTPHelper.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CheckTelTPError::CheckTelTPError()
{
}

CheckTelTPError::~CheckTelTPError()
{
}

// �g�p�@
void CheckTelTPError::PrintUsage()
{
	std::cout << "[TelTPPoint�ߋ��g���`�F�b�N�c�[��]\n"
			  << "(Usage)CheckTelTPError.exe [���̓t�@�C��] [�o�̓t�@�C��]\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  DB_ADDRESS       ... �Z���n�ڑ��v���p�e�B\n"
			  << "  FC_TELTP_POINT   ... TelTP�|�C���g�t�B�[�`���N���X" << std::endl;
}

// ������
bool CheckTelTPError::Init( int cArgc, _TCHAR* cArgv[] )
{
	if( cArgc < 3 )
	{
		cerr << "����������܂���" << endl;
		return false;
	}

	int i = 1;

	// ���̓t�@�C���̃I�[�v��
	if( !m_cListHandle.loadData(cArgv[i]) )
	{
		cerr << "���̓t�@�C�����J���܂���: " << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// �o�̓t�@�C���̃I�[�v���E�w�b�_�o��
	m_ofs.open( static_cast<CT2CW>(cArgv[++i]) );
	if( !m_ofs.is_open() )
	{
		cerr << "�o�̓t�@�C�����J���܂���:" << CT2CA(cArgv[i]) << endl;
		return false;
	}

	// ���ϐ�����e��ݒ�
	if( !fnSetInfoFromEnv() )
		return false;


	// ���O�w�b�_�[�o��
	CVersion cVer;
	CString strMsg;
	strMsg.Format( _T("#FREESTYLELOG\n")
				   _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
				   _T("FLAG\t�N���[��No.\tObjectID\t�V�[�P���X\tMESH\tX\tY\t���W�t�^���\t���[�U�N���[���t���O\t�d�b�ԍ�\t�f�ڏZ��\t�Z���R�[�h\t����\t�f�ږ���\t�������ʃR�[�h\t���\�[�X\t���b�Z�[�W")
				   _T("\t1:MESH\t1:X\t1:Y\tCOMMENT"),
				   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_ofs << CT2CA(strMsg) << std::endl;

	return	true;
}

// ���s
bool CheckTelTPError::Execute()
{
	// �w���p�[�N���X������
	CTelTPHelper	cTelTPHelper( m_cTelTPPointFC );

	CLList& listRec = m_cListHandle.getRecodList();
	CLList::const_iterator itrE = listRec.end();
	for( CLList::iterator itr = listRec.begin(); itr != itrE; ++itr )
	{
		// ���b�V���R�[�h�E�t�B�[���h����Ȃ�t�^���[�h��
		if( itr->m_cMXY.m_nMesh == 0 )
		{
			m_bAddMode = true;
			cout << CT2CA(itr->m_strSequence) << ": ���W�t�^��..." << endl;
		}
		else
		{
			m_bAddMode = false;
			cout << CT2CA(itr->m_strSequence) << ": �`�F�b�N��..." << endl;
		}
		
		// TEL_TP_POINT ���猟��
		EResultStat emStatus = cTelTPHelper.findPoint(*itr, m_bAddMode);

		outputLog( emStatus, *itr );	// �o��
	}
	return true;
}



//////////////////////////////////
//	�����֐�
//////////////////////////////////

// ���ϐ��擾
bool CheckTelTPError::fnSetInfoFromEnv()
{

#ifdef _DEBUG
//	_tputenv( _T("DB_ADDRESS=SiNDY2009B/SiNDY2009B/SDE.DEFAULT/5151/topaz2") );
//	_tputenv( _T("FC_TELTP_POINT=TelTP_Point") );
	_tputenv( _T("DB_ADDRESS=prodeng2/prodeng2/SDE.DEFAULT/5151/coral2") );
	_tputenv( _T("FC_TELTP_POINT=prodeng2.TelTP_Point") );
#endif

	// �Z��DB�ڑ�
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect(strDBProp) )
	{
		cerr << "�Z��DB�ւ̐ڑ��Ɏ��s : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// �t�B�[�`���N���X�擾
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_TELTP_POINT")) )
	{
		cerr << "���ϐ� FC_TELTP_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	m_cTelTPPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( NULL == m_cTelTPPointFC )
	{
		cerr << "TelTP�|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		return false;
	}
	return true;
}

// ���O�o��
void CheckTelTPError::outputLog( EResultStat emStatus, CLRec &cRec )
{
	CString strLog;

	// FLAG �N���[��No. ObjectID �V�[�P���X
	strLog.Format( _T("0\t%s\t%s\t%s\t"), cRec.m_strClaimNo, cRec.m_strObjectID, cRec.m_strSequence );

	// MESH X Y
	if( cRec.m_cMXY.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY.m_nMesh, cRec.m_cMXY.m_nX, cRec.m_cMXY.m_nY );

	// ���W�t�^���(ADDXY_C) ���[�U�N���[���t���O �d�b�ԍ� �f�ڏZ�� �Z���R�[�h ���� �f�ږ��� �������ʃR�[�h ���\�[�X
	strLog.AppendFormat( _T("%d\t%d\t%s\t%s\t%s\t%s\t%s\t%d\t%s\t"), cRec.m_lAddXY, cRec.m_lUserClaimF, cRec.m_strTelNum, cRec.m_strAddrName, 
								cRec.m_strAddrCode, cRec.m_strKatagaki, cRec.m_strKeisaiName, cRec.m_lPointStat, 
								cRec.m_strSource );
	// ���b�Z�[�W
	if( !cRec.m_strAddComment.IsEmpty() && !m_bAddMode )
		strLog.AppendFormat( _T("%s%s\t"), GetResultMsg(emStatus), cRec.m_strAddComment );
	else
		strLog.AppendFormat( _T("%s\t"), GetResultMsg(emStatus) );
	
	/* �h���C�����g���Ȃ牺�L�𗘗p
	// ���W�t�^���(ADDXY_C) �d�b�ԍ� �f�ڏZ�� �Z���R�[�h ���� �f�ږ��� �������ʃR�[�h ���\�[�X ���b�Z�[�W
	strLog.AppendFormat( _T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"), static_cast<CString>((cTelTPHelper.getDomain( _T("ADDXY_C"), cRec.m_lAddXY )).c_str()), 
								cRec.m_strTelNum, cRec.m_strAddrName, cRec.m_strAddrCode, cRec.m_strKatagaki, cRec.m_strKeisaiName, 
								static_cast<CString>((cTelTPHelper.getDomain( _T("POINTSTAT_C"), cRec.m_lPointStat )).c_str()), cRec.m_strSource, GetResultMsg(emStatus) );
*/
	// 1:MESH�@1:X�@1:Y
	if( cRec.m_cMXY_Ref.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY_Ref.m_nMesh, cRec.m_cMXY_Ref.m_nX, cRec.m_cMXY_Ref.m_nY );

	m_ofs << CT2CA(strLog) << std::endl;
}




