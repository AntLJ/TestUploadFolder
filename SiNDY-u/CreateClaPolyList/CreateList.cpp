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

// CreateList.cpp: CCreateList �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateList.h"
#include <sindy/workspace.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCreateList::CCreateList() : 
	m_lClaimNumberFID(-1),
	m_lUselessF_FID(-1)
{
}

CCreateList::~CCreateList()
{
}

// �g�p�@
void CCreateList::printUsage()
{
	cerr << "�y�N���[���|���S�����X�g�쐬�c�[���g�p�@�z\n"
		 << "(Usage)CrateClaPolyList.exe [LogFile]\n"
		 << "�E���ϐ��ɂ���\n"
		 << "  DB_ANY        �N���|���̂���T�[�o"
		 << "  FC_CLAIMPOLY  �N���|���t�B�[�`���N���X��" << endl;	
}

// ������
bool CCreateList::init( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		cerr << "#Error �����ɕK�����O�t�@�C���͎w�肵�Ă�������" << endl;
		return false;
	}

#ifdef _DEBUG
	_tputenv( _T("DB_ANY=techmap/techmap/SDE.DEFAULT/5151/spinel") );
	_tputenv( _T("FC_CLAIMPOLY=ClaimPoly_Map") );
#endif

	// �c�a�ڑ��v���p�e�B�擾
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ANY")) )
	{
		cerr << "#Error ���ϐ� DB_ANY�̎擾�Ɏ��s" << endl;
		return false;
	}

	// �N���[���|���S���t�B�[�`���N���X���擾
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_CLAIMPOLY")) )
	{
		cerr << "#Error ���ϐ� FC_CLAIMPOLY�̎擾�Ɏ��s" << endl;
		return false;
	}

	// �ڑ�
	IWorkspacePtr ipWorkspace;
	INamePtr ipName( sindy::create_workspace_name(strDBProp) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		ipWorkspace = ipUnknown;
	}

	if( ! ipWorkspace )
	{
		cerr << "#Error SDE�ڑ��Ɏ��s : " << strDBProp << endl;
		return false;
	}
	cerr << "#SDE�ڑ� : " << strDBProp << std::endl;

	// �N���[���|���S���t�B�[�`���N���X�擾
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(strFCName), &m_ipClaimPolyClass );
	if( ! m_ipClaimPolyClass )
	{
		cerr << "#Error �N���[���|���S���t�B�[�`���N���X�̎擾�Ɏ��s : " << strFCName << endl;
		return false;
	}

	// �t�B�[���h�C���f�b�N�X�擾
	if( FAILED(m_ipClaimPolyClass->FindField(CComBSTR(_T("CLAIM_NO")), &m_lClaimNumberFID))
		|| FAILED(m_ipClaimPolyClass->FindField(CComBSTR(_T("USELESS_F")), &m_lUselessF_FID)) )
	{
		cerr << "#Error �t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << std::endl;
		return false;
	}

	// ���O�t�@�C���I�[�v��
	m_fout.open( argv[1] );
	if( !m_fout )
	{
		cerr << "#Error ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[1] << endl;
		return false;
	}

	return true;
}

// ���s
bool CCreateList::execute()
{
	m_fout << "#�I�u�W�F�N�gID,�N���[���ԍ�,�s�v�t���O" << endl;	// ���O�w�b�_�[�o��

	// �S�Ď擾����
	long lAllCount = 0;
	m_ipClaimPolyClass->FeatureCount( NULL, &lAllCount );

	IFeatureCursorPtr ipFeatureCursor;
	m_ipClaimPolyClass->Search( NULL, VARIANT_FALSE, &ipFeatureCursor );

	if( ! ipFeatureCursor )
	{
		cerr << "#Error : �N���[���|���S�����擾�ł��܂���ł���" << endl;
		return false;
	}

	IFeaturePtr ipFeature;
	for( long i = 1; ipFeatureCursor->NextFeature(&ipFeature) == S_OK; ++i )
	{
		makeClaimPolyList( ipFeature );	// ���X�g�쐬

		cerr << i << " / " << lAllCount << " ���I��\r";
	}

	cerr << endl;

	return true;
}

////////////////////////////////////////////
//	�����֐�
////////////////////////////////////////////

// �N���|�����X�g�쐬
void CCreateList::makeClaimPolyList( IFeature* ipFeature )
{
	// �I�u�W�F�N�g�h�c�擾
	long lOID = 0;
	ipFeature->get_OID( &lOID );

	CComVariant vaValue;

	// �N���[���ԍ��擾
	ipFeature->get_Value( m_lClaimNumberFID, &vaValue );
	CString strClaimNo = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();

	// �s�v�t���O�擾
	ipFeature->get_Value( m_lUselessF_FID, &vaValue );
	long lUselessF = vaValue.lVal;

	// �N���[���ԍ��������ďo��
	int nCurPos = 0;
	CString strToken = strClaimNo.Tokenize( _T(",\n"), nCurPos );
	while( strToken != _T("") )
	{
		printInfo( lOID, strToken, lUselessF );
		strToken = strClaimNo.Tokenize( _T(",\n"), nCurPos );
	}
}
