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

// CreateExTPTxt.cpp: CreateExTPTxt �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateExTPTxt.h"
#include <map>

#include <crd_cnv/coord_converter.h>
#include <sindy/workspace.h>

static crd_cnv	g_cCrdCnv;	// ���W�ϊ��N���X�C���X�^���X

// �g�p�@
void CreateExTPTxt::Usage()
{
	std::cout << "[XY�t�^DB�pExTP�|�C���g�f�[�^�o�̓c�[��]\n"
	  		  << "(Usage)CreateExTPTxt.exe [SDE�ڑ���] [�t�B�[�`���N���X��] [�o�̓t�@�C��]" << std::endl;
}

// ������
bool CreateExTPTxt::Init( int argc, char** argv )
{
	if( argc != 4 )
	{
		std::cerr << "#Eror �����̐����s���ł�" << std::endl;
		return false;
	}

	// ���ϐ��̒l�擾���ADB�ڑ�
	IWorkspacePtr ipWorkspace;
	INamePtr ipName = sindy::create_workspace_name( argv[1] );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		ipWorkspace = ipUnknown;
	}
	if( NULL == ipWorkspace )
	{
		std::cerr << "#Error SDE�ڑ����s : " << argv[1] << std::endl;
		return false;
	}
	std::cout << "#SDE�ڑ� : " << argv[1] << std::endl;

	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass( _bstr_t(argv[2]), &m_ipExTPClass );
	if( NULL == m_ipExTPClass )
	{
		std::cerr << "#Error ExTP�|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << argv[2] << std::endl;
		return false;
	}

	// ���O�t�@�C���I�[�v��
	m_ofs.open( static_cast<CT2CW>(argv[3]) );
	if( !m_ofs )
	{
		std::cerr << "#Error ���O�t�@�C���I�[�v���Ɏ��s : " << argv[3] << std::endl;
		return false;
	}

	return true;
}

// ���s
void CreateExTPTxt::Execute()
{
	// �t�B�[���h�C���f�b�N�X�擾
	long lAddrCode_FIdx = -1, lInfoSrc_FIdx = -1;
	if( FAILED(m_ipExTPClass->FindField(_bstr_t("ADDRCODE"), &lAddrCode_FIdx)) 
		|| FAILED(m_ipExTPClass->FindField( _bstr_t("INFOSRC_C"), &lInfoSrc_FIdx)) )
	{
		std::cerr << "#Error �t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << std::endl;
		return;
	}
	
	// �t�B�[�`���̌��� ... [Bug6577]���x4���Ώۂɒǉ� [Bug8625]HLP���Ώۂɒǉ�
	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->PutWhereClause( _bstr_t("AddPoint_f = 0 and ((InfoSrc_c IN (1, 3, 4) and PointStat_c in (2, 3, 4, 5, 6, 7, 8, 9)) or InfoSrc_c = 2)") );

	IFeatureCursorPtr ipCursor;
	m_ipExTPClass->Search( ipQuery, VARIANT_FALSE, &ipCursor );
	if( ipCursor )
	{
		IFeaturePtr ipFeature;
		while( ipCursor->NextFeature(&ipFeature) == S_OK )
		{
			CExTPRec cExTP;

			ipFeature->get_OID( &cExTP.m_lOID );	// OID

			// ���W�l�̎擾
			IGeometryPtr ipGeo;
			ipFeature->get_Shape( &ipGeo );
			((IPointPtr)ipGeo)->QueryCoords( &cExTP.m_point.X, &cExTP.m_point.Y );
			
			// �Z���R�[�h
			CComVariant	vaValue;
			ipFeature->get_Value( lAddrCode_FIdx, &vaValue );
			cExTP.m_strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
			vaValue.Clear();

			// �������W�񋟌��R�[�h
			ipFeature->get_Value( lInfoSrc_FIdx, &vaValue );
			cExTP.m_lInfoSrc = vaValue.lVal;

			AddExTP( cExTP );	// �ǉ�
		}

		OutputExTP();	// �o��
	}
}

///////////////////////////////////////////////////////
// �����֐�
///////////////////////////////////////////////////////

// ExTP�ǉ�
void CreateExTPTxt::AddExTP( const CExTPRec& cExTP )
{
	EXTPREC_MAP::iterator itr = m_mapExTP.find( cExTP.m_strAddrCode );
	if( itr != m_mapExTP.end() )
	{
		// [Bug6577]���ɑ��݂���ꍇ�A�ȉ��̏����𖞂�������X���b�v
		// (1) INFOSRC_C���u2�v�ȊO��D��
		// (2) INFOSRC_C�������Ȃ�A�I�u�W�F�N�gID���傫������D��
		if( itr->second.m_lInfoSrc == 2 )
		{
			if( cExTP.m_lInfoSrc != 2 || itr->second.m_lOID < cExTP.m_lOID )
				itr->second = cExTP;
		}
		else
		{
			if( cExTP.m_lInfoSrc != 2 && itr->second.m_lOID < cExTP.m_lOID )
				itr->second = cExTP;
		}
	}
	else
	{
		m_mapExTP.insert( std::pair<CString, CExTPRec>(cExTP.m_strAddrCode, cExTP) );	// ���ʂɒǉ�
	}
}

// ���R�[�h�o��
void CreateExTPTxt::OutputExTP()
{
	for( EXTPREC_MAP::const_iterator itr = m_mapExTP.begin(); itr != m_mapExTP.end(); ++itr )
	{
		// 2�����b�V��XY�̎擾
		int nMeshCode = 0, nMeshX = 0, nMeshY = 0;
		g_cCrdCnv.LLtoMesh( itr->second.m_point.X, itr->second.m_point.Y, 2, &nMeshCode, &nMeshX, &nMeshY, 1 );

		// �������W�񋟌��R�[�h����IP�R�[�h�ɕϊ�
		CString strIPCode;
		strIPCode = "07";	// [Bug9101] 12�t���ExTP�͑S��IPCODE[76]

		// �Z���R�[�h������W���x/��\�_�E�n�Ԏ~�܂�t���O�̐ݒ�
		int nSeido = itr->second.m_strAddrCode.Right(4) == _T("0000")? 4 : 5;
		CString strChibanDomariF = (nSeido == 4)? _T("2") : _T("");
		
		// �f�[�^�o�͊J�n
		m_ofs << nMeshCode << "\t" << nMeshX << "\t" << nMeshY << "\t"  << strIPCode << "\t" << nSeido << "\t"
			  << itr->second.m_strAddrCode.Mid(0, 2) << "\t" << itr->second.m_strAddrCode.Mid(2, 3) << "\t" 
			  << itr->second.m_strAddrCode.Mid(5, 3) << "\t" << itr->second.m_strAddrCode.Mid(8, 3) << "\t"
			  << itr->second.m_strAddrCode.Mid(11,5) << "\t" << itr->second.m_strAddrCode.Mid(16,4) << "\t"
			  << strChibanDomariF << std::endl;
	}
}
