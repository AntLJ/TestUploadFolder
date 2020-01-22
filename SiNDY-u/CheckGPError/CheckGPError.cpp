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

// CheckGPError.cpp: CheckGPError �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckGPError.h"
#include <WinLib/VersionInfo.h>
#include <crd_cnv/coord_converter.h>

#include <ArcHelperEx/GlobalFunctions.h>

#include <AddrLib/Initializer.h>
#include <AddrLib/AddrGlobals.h>
#include <AddrLib/CityAdmin.h>

#include "CSHelper.h"
#include "GPHelper.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CheckGPError::CheckGPError() :
	m_bUpdateMode(false),
	m_bAddMode(false),
	m_bCharAddrMode(false)
{
}

CheckGPError::~CheckGPError()
{
}

// �g�p�@
void CheckGPError::PrintUsage()
{
	std::cout << "[�Z���ߋ��g���`�F�b�N�c�[��]\n"
			  << "(Usage)CheckGPError.exe [�X�V���[�h�I�v�V����] [���̓t�@�C��] [�o�̓t�@�C��]\n"
			  << "�E�X�V���[�h�I�v�V�����ɂ���\n"
			  << "  �X�V���[�h�Ŏ��s����ۂɂ́A��������[-u]��ǉ����Ă��������B\n"
			  << "�E���ϐ��ɂ���\n"
			  << "  DB_ADDRESS       ... �Z���n�ڑ��v���p�e�B\n"
			  << "  DB_ADDRESS_REF   ... �X�V���Q�Ɨp�Z���n�ڑ��v���p�e�B(�C��)\n"
			  << "  FC_GOU_POINT     ... ���|�C���g�t�B�[�`���N���X\n"
			  << "  FC_CITY_ADMIN    ... �s�s�n�}�s���E�N���X\n"
			  << "  FC_CS_ADDR_POINT ... CS�|�C���g�t�B�[�`���N���X\n"
			  << "  JDB_CONNECT      ... �Z���}�X�^�ڑ�������\n"
			  << "  TBL_CODECONV     ... �����t���Z���p�R�[�h�ϊ��e�[�u��(�C��)" << std::endl;
}

// ������
bool CheckGPError::Init( int cArgc, _TCHAR* cArgv[] )
{
	if( cArgc < 3 )
	{
		cerr << "����������܂���" << endl;
		return false;
	}

	int i = 1;

	// �X�V���[�h�ݒ肳��Ă��邩�`�F�b�N
	if( _tcscmp(cArgv[i], _T("-u")) == 0 )
	{
		m_bUpdateMode	= true;		
		++i;
	}

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
				   _T("FLAG\t�N���[��No.\t�Z���R�[�h\t�Z���R�[�h(�V)\tMESH\tX\tY\t��ƖړI\t�g�����ԍ��t���O\t���\�[�X\t���b�Z�[�W")
				   _T("\t1:MESH\t1:X\t1:Y\t2:LAYER\t2:OBJECTID\tCOMMENT"),
				   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
	m_ofs << CT2CA(strMsg) << std::endl;
			
	return true;
}

// ���s
bool CheckGPError::Execute()
{
	if( m_bUpdateMode )
	{
		cout << "���s���[�h : �X�V" << endl;
		if( !fnEditStart() )
			return false;
	}
	else
		cout << "���s���[�h : �`�F�b�N" << endl;

	// �w���p�[�N���X������
	CCSHelper	m_cCSHelper( m_cCSPointFC, m_cCityAdminFC );
	CGPHelper	m_cGPHelper( m_cGouPointFC, m_cGouPointFC_Ref, m_cCityAdminFC, m_cCityAdmin_Ref );

	CLList& listRec = m_cListHandle.getRecodList();
	CLList::const_iterator itrE = listRec.end();
	for( CLList::iterator itr = listRec.begin(); itr != itrE; ++itr )
	{
		// ���b�V���R�[�h�E�t�B�[���h����Ȃ�t�^���[�h��
		if( itr->m_cMXY.m_nMesh == 0 )
		{
			m_bAddMode = true;
			cout << CT2CA(itr->m_strAddrCode) << ": ���W�t�^��..." << endl;
		}
		else
		{
			m_bAddMode = false;
			cout << CT2CA(itr->m_strAddrCode) << ": �`�F�b�N��..." << endl;
		}

		// �����t���Z���R�[�h�l���łȂ��ꍇ�A�����t���Z���͏����Ώۂ���O��
		if( !m_bCharAddrMode && itr->IsCharAddrCode() )
		{
			m_ofs << "#�����t���Z���l�����[�h�łȂ��ׁA�����t���Z�����R�[�h�͏������X�L�b�v���܂� : " << CT2CA(itr->m_strAddrCode) << std::endl;
			continue;
		}

		// �ŐV11���Z���R�[�h�ɒu��
		itr->m_strAddrCodeNew = m_cAcHelper.GetNewestCode( itr->m_strAddrCode.Left(11) ) + itr->m_strAddrCode.Mid(11, 9);

		// CS_ADDR_POINT ���猟��(�����t���Z���̏ꍇ�́A��΂�)
		EResultStat emStatus = !itr->IsCharAddrCode()? m_cCSHelper.findPoint(*itr, m_bAddMode) : kO_NOT_FOUND;
		if( kO_NOT_FOUND != emStatus )
		{
			// �X�V���[�h�Ȃ�A�ړ��������{
			if( kW_CS_MOVED == emStatus && m_bUpdateMode )
			{
				emStatus = m_cCSHelper.movePoint(*itr);
			}
		}
		else
		{
			// CS_ADDR_POINT ���Ȃ���� GOU_POINT ���猟��
			emStatus = m_cGPHelper.findPoint( *itr, m_bAddMode );
			if( kO_NOT_FOUND != emStatus )
			{
				// �X�V���[�h�Ȃ�A�ړ��������{
				if( (kW_GP_MOVED == emStatus || kW_GP_NOT_PRIOR == emStatus) && m_bUpdateMode )
				{
					emStatus = m_cGPHelper.movePoint( *itr, kW_GP_MOVED == emStatus );
				}
			}
			else
			{
				// �X�V���[�h�Ȃ�A�ǉ��������{
				if( m_bUpdateMode )
				{
					// ��ɂǂ��������`�Ń|�C���g��ǉ����邩���`�F�b�N
					emStatus = checkInsertPoint( *itr );
					switch( emStatus )
					{
					case kW_CS_INSERT:
						if( itr->IsCharAddrCode() )
							emStatus = KW_CS_INSERT_CANNOT_CHAR;
						else
							emStatus = m_cCSHelper.insertPoint(*itr);
						break;
					case kW_GP_INSERT:
						emStatus = m_cGPHelper.insertPoint( *itr );
						break;
					default:
						break;
					}
				}
			}
		}
		outputLog( emStatus, *itr );	// �o��
	}

	if( m_bUpdateMode )
	{
		// �ҏW�I��
		if( !fnEditStop() )
			return false;
	}

	return true;
}

//////////////////////////////////
//	�����֐�
//////////////////////////////////

// ���ϐ��擾
bool CheckGPError::fnSetInfoFromEnv()
{
#ifdef _DEBUG
	_tputenv( _T("JDB_CONNECT=y1205/y1205@delltcp") );
	_tputenv( _T("DB_ADDRESS=PRODENG1/PRODENG1/PRODENG1.CheckGPError_test/5151/coral2") );	
	_tputenv( _T("DB_ADDRESS_REF=PRODENG1/PRODENG1/PRODENG1.CheckGPError_20120801214147/5151/coral2") );
	_tputenv( _T("FC_GOU_POINT=Gou_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_CS_ADDR_POINT=CS_Addr_Point") );
	_tputenv( _T("TBL_CODECONV=Test\\CodeConvTable.mdb") );
#endif

	// �Z��DB�ڑ�
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariable(_T("JDB_CONNECT")) )
	{
		cerr << "���ϐ� JDB_CONNECT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	if( !m_cAcHelper.Connect(strDBProp) )
	{
		cerr << "�Z��DB�ւ̐ڑ��Ɏ��s : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// SDE�ڑ�
	if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "���ϐ� DB_ADDRESS ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect(strDBProp) )
	{
		cerr << "SDE�ڑ��Ɏ��s : " << CT2CA(strDBProp) << endl;
		return false;
	}

	// �X�V���[�h�̂Ƃ��͍X�V���f�[�^DB���Q�Ƃ��邩�ǂ����m�F���� (bug9300)
	if( m_bUpdateMode )
	{
		if( !strDBProp.GetEnvironmentVariable(_T("DB_ADDRESS_REF")) )
		{
			cerr << "���ϐ� DB_ADDRESS_REF ���ݒ肳��Ă��܂���" << endl;
			return false;
		}
		else
		{
			if( !m_cWorkspace_Ref.Connect(strDBProp) )
			{
				cerr << "SDE�ڑ��Ɏ��s : " << CT2CA(strDBProp) << endl;
				return false;
			}
			m_ofs << "#�X�V����DB�F" << CT2CA(strDBProp) << "���Q�Ƃ��܂�" << std::endl;
		}
	}

	// �t�B�[�`���N���X�擾
	CString strFCName;
	if( !strFCName.GetEnvironmentVariable(_T("FC_GOU_POINT")) )
	{
		cerr << "���ϐ� FC_GOU_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	m_cGouPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cGouPointFC )
	{
		cerr << "���|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		return false;
	}
	if( m_bUpdateMode )
	{
		m_cGouPointFC_Ref.SetObject( m_cWorkspace_Ref.OpenTable(strFCName) );
		if( !m_cGouPointFC_Ref )
		{
			cerr << "���|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		}
	}

	if( !strFCName.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )
	{
		cerr << "���ϐ� FC_CITY_ADMIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	m_cCityAdminFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cCityAdminFC )
	{
		cerr << "�s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		return false;
	}	
	if( m_bUpdateMode )
	{
		m_cCityAdmin_Ref.SetObject( m_cWorkspace_Ref.OpenTable(strFCName) );
		if( !m_cCityAdmin_Ref )
		{
			cerr << "�s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		}
	}

	if( !strFCName.GetEnvironmentVariable(_T("FC_CS_ADDR_POINT")) )
	{
		cerr << "���ϐ� FC_CS_ADDR_POINT ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	m_cCSPointFC.SetObject( m_cWorkspace.OpenTable(strFCName) );
	if( !m_cCSPointFC )
	{
		cerr << "CS�|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(strFCName) << std::endl;
		return false;
	}

	// �����t���Z���p�R�[�h�ϊ��e�[�u���ǂݍ���(���ϐ��w�肵��΁A�����t���Z�����Ή����郂�[�h)
	CString strCodeConvTbl;
	if( strCodeConvTbl.GetEnvironmentVariable(_T("TBL_CODECONV")) )
	{
		if( !addr::Init_CodeConverter(strCodeConvTbl) )
		{
			cerr << "�����t���Z���p�R�[�h�ϊ��e�[�u���̏������Ɏ��s : " << CT2CA(strCodeConvTbl) << std::endl;
			return false;
		}
		m_bCharAddrMode = true;
		m_ofs << "#�����t���Z�����l�����ă`�F�b�N���܂�" << std::endl;
	}
	else
	{
		cerr << "���ϐ� TBL_CODECONV �����ݒ�ׁ̈A�����t���Z���͍l�����܂���" << std::endl;
		m_ofs << "#���ϐ� TBL_CODECONV �����ݒ�ׁ̈A�����t���Z���͍l�����܂���" << std::endl;
	}

	return true;
}

// �ҏW�J�n
bool CheckGPError::fnEditStart()
{
	// �܂��́A�ҏW�p�q�o�[�W�����쐬(CheckGPError_yyyymmddhhmmss)
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strChildVer;
	strChildVer.Format( _T("%s_%04d%02d%02d%02d%02d%02d"), PROGRAM_NAME, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	IVersionedWorkspacePtr ipVerWorkspace( (IWorkspace*)m_cWorkspace );
	if( !ipVerWorkspace )
	{
		cerr << "#Error �o�[�W�����Ή��ł���܂���" << endl;
		return false;
	}

	// DEFAULT�o�[�W�����͂���
	IVersionPtr ipCurVersion( (IWorkspace*)m_cWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName( &bstr );
	CString strTmp(bstr);
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		cerr << "#Error �f�t�H���g�o�[�W�����ł��̂ŏI�����܂� : " << CT2CA(strTmp) << endl;
		return false;
	}

	// ���ɂ��̃o�[�W���������݂��邩�`�F�b�N
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(strChildVer), &ipChildVersion );
	if( !ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(strChildVer), &ipChildVersion );
		if( !ipChildVersion )
		{
			cerr << "#Error �ҏW�p�o�[�W�����̍쐬�Ɏ��s : " << CT2CA(strChildVer) << endl;
			return false;
		}

		// �q�o�[�W�����ɃR�����g�ǉ�
		CString strComment;
		strComment.Format( _T("CheckGPError�ɂ��ҏWVer %s"), gf::GetCurAccount() );
		ipChildVersion->put_Description( CComBSTR(strComment) );			
	}

	cout << "�ҏW�p�o�[�W���� : " << CT2CA(strChildVer) << " �̍쐬������" << endl;

	m_cWorkspace = (IWorkspacePtr)ipChildVersion;

	// �ҏW�J�n
	if( sindyErr_NoErr != m_cWorkspace.StartEditing(false) || sindyErr_NoErr != m_cWorkspace.StartEditOperation() )
	{
		cerr << "#Error �ҏW�J�n�Ɏ��s" << endl;
		return false;
	}

	// �ҏW�p���[�N�X�y�[�X����CS/GP�t�B�[�`���N���X��蒼��������
	m_cGouPointFC.SetObject( m_cWorkspace.OpenTable(m_cGouPointFC.GetNameString()->GetTableName()) );
	if( !m_cGouPointFC )
	{
		cerr << "#Error ���|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s" << CT2CA(m_cGouPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}
	m_cCSPointFC.SetObject( m_cWorkspace.OpenTable(m_cCSPointFC.GetNameString()->GetTableName()) );
	if( !m_cCSPointFC )
	{
		cerr << "#Error CS�|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA(m_cCSPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}

	return true;
}

// �ҏW�I��
bool CheckGPError::fnEditStop()
{
	if( sindyErr_NoErr != m_cWorkspace.StartEditOperation() || sindyErr_NoErr != m_cWorkspace.StopEditing(true) )
	{
		m_cWorkspace.AbortEditOperation();
		cerr << "#Error �ҏW�I���Ɏ��s" << endl;
		return false;
	}
	else
		return true;
}

// �|�C���g�ǉ��ʒu�̃`�F�b�N
EResultStat CheckGPError::checkInsertPoint( CLRec& cRec )
{
	// ���b�V���w�x���o�ܓx�ϊ�
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( ipPoint )
	{
		// ���W�ʒu�̍s���E������
		ISpatialFilterPtr ipFilter( AheInitSpatialFilter(NULL, ipPoint) );
		long lCount = m_cCityAdminFC.FeatureCount(ipFilter);
		if( lCount == 1 )
		{
			IFeatureCursorPtr ipFeatureCursor( m_cCityAdminFC.Search(AheInitSpatialFilter(NULL, ipPoint), VARIANT_FALSE) );
			if( ipFeatureCursor )
			{
				IFeaturePtr	ipFeature;
				ipFeatureCursor->NextFeature( &ipFeature );

				CCityAdmin cAdmin( ipFeature, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

				const CString& strAdminCode = cAdmin.GetAddrCode().Left(11);

				// �Z���R�[�h��r(�s���E���ʏ̂Ȃ炻����l��[Bug7257,Bug8166])
				if( cRec.m_strAddrCodeNew.Left(11) == strAdminCode.Left(11) 
					|| (cAdmin.GetOazaAzaCode2() != _T("000000") && cRec.m_strAddrCodeNew.Left(11) == strAdminCode.Left(5) + cAdmin.GetOazaAzaCode2()) )
				{
					return kW_GP_INSERT;	// �s���E�Ɛ������Ă���̂ŁAGOU�Œǉ�
				}
				else
					return kW_CS_INSERT;	// �s���E�Ɛ������Ă���̂ŁACS�Œǉ�
			}
		}

		return kE_INSERT_ADMIN_FAIL;
	}
	else
		return kE_INSERT_FAIL;
}

// ���O�o��
void CheckGPError::outputLog( EResultStat emStatus, CLRec &cRec )
{
	if( cRec.m_strAddrCode == cRec.m_strAddrCodeNew )
		cRec.m_strAddrCodeNew = "";

	CString strLog;
	strLog.Format( _T("0\t%s\t%s\t%s\t"), cRec.m_strClaimNo, cRec.m_strAddrCode, cRec.m_strAddrCodeNew );

	if( cRec.m_cMXY.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY.m_nMesh, cRec.m_cMXY.m_nX, cRec.m_cMXY.m_nY );

	strLog.AppendFormat( _T("%d\t%d\t%s\t%s\t"), cRec.m_lPurpose, cRec.m_lExFlag, cRec.m_strSource, GetResultMsg(emStatus) );

	if( cRec.m_cMXY_Ref.m_nMesh == 0 )
		strLog.Append( _T("\t\t\t") );
	else
		strLog.AppendFormat( _T("%d\t%d\t%d\t"), cRec.m_cMXY_Ref.m_nMesh, cRec.m_cMXY_Ref.m_nX, cRec.m_cMXY_Ref.m_nY );

	switch( emStatus )
	{
	case kW_CS_MOVED:
	case kW_CS_INSERT:
		if( m_bUpdateMode )
			strLog.AppendFormat( _T("%s\t%d\t"), m_cCSPointFC.GetNameString()->GetOwnerTableName(), cRec.m_lEditPointID );
		else
			strLog.Append( _T("\t\t") );
		break;
	case kW_GP_MOVED:
	case kW_GP_MOVED_NOT_PRIOR:
	case kW_GP_INSERT:
	case kW_GP_INSERT_EX:
		if( m_bUpdateMode )
			strLog.AppendFormat( _T("%s\t%d\t"), m_cGouPointFC.GetNameString()->GetOwnerTableName(), cRec.m_lEditPointID );
		else
			strLog.Append( _T("\t\t") );
		break;
	default:
		strLog.Append( _T("\t\t") );
		break;
	}

	m_ofs << CT2CA(strLog) << std::endl;
}
