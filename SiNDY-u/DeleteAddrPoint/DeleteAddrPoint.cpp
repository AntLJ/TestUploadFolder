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

// DeleteAddrPoint.cpp: DeleteAddrPoint �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeleteAddrPoint.h"
#include "GlobalFunc.h"
#include <WinLib/VersionInfo.h>

#include <crd_cnv/coord_converter.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DeleteAddrPoint::DeleteAddrPoint()
{
}

DeleteAddrPoint::~DeleteAddrPoint()
{
}

// �g�p�@
void DeleteAddrPoint::PrintUsage()
{
	std::cout << "[�Z���|�C���g�폜�c�[��]\n"
			  << "(Usage)DeleteAddrPoint.exe [���̓t�@�C��] [���O�t�@�C��]\n"
			  << "���ϐ�\n"
			  << "DB_MAIN      ... �Z���n�ڑ��v���p�e�B\n"
			  << "FC_TARGET    ... �폜�Ώۃt�B�[�`���N���X\n" << std::endl;
}

// ������
bool DeleteAddrPoint::Init(int cArgc, _TCHAR *cArgv[])
{
	if( cArgc < 3 )
	{
		std::cerr << "#Error ����������܂���" << std::endl;
		return false;
	}

	// �폜�\���C�����Z�b�g�Ɋi�[
	fnSetDeleteLayer();

	// ���ϐ��擾��DB�ڑ�
	if( !fnSetInfoFromEnv() )
		return false;

	int		i = 0;

	// ���̓t�@�C��
	CString	aOIDListName;
	aOIDListName = cArgv[++i];

	if( !fnLoadOIDList( aOIDListName ) )
	{
		cerr << "#Error OID���X�g�̓ǂݍ��݂Ɏ��s:" << CT2CA( aOIDListName ) << endl;
		return false;
	}
	
	// �o�̓t�@�C���̃I�[�v��
	m_ofs.open( static_cast<CT2CW>(cArgv[++i]) );
	if( !m_ofs.is_open() )
	{
		cerr << "#Error �o�̓t�@�C�����J���܂���:" << CT2CA( cArgv[i] ) << endl;
		return false;
	}
	
	// �w�b�_�o��
	fnPrintHeader();

	return true;
}

// ���O�Ƀw�b�_�[���o��
void DeleteAddrPoint::fnPrintHeader()
{
	// �Œ蕔���̏o��
	m_ofs << "# FREESTYLELOG" << endl;

	CVersion cVer;
	m_ofs << "# " << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " << CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;

	CString csStartTime = gf::GetCurDateStr();
	m_ofs << "# �J�n���ԁF" << CT2CA(csStartTime) << endl;

	m_ofs << "FLAG\tMESH\tX\tY\tLAYERNAME\t";

	// DB����t�B�[���h�����擾���A�S�ăw�b�_�ɏo��
	IFieldsPtr	ipFields;
	long		iFieldNum = 0;
	ipFields	= m_cTargetPointFC.GetFields();
	ipFields->get_FieldCount( &iFieldNum );

	IFieldPtr	ipField;
	CComBSTR	bsFieldName;
	// SHAPE�t�B�[���h�ȊO�̃t�B�[���h�S�ďo��
	for( int i = 0 ; i < iFieldNum ; i++ )
	{
		ipFields->get_Field( i, &ipField );
		esriFieldType	fType;
		ipField->get_Type( &fType );
		if( fType != esriFieldTypeGeometry )
		{
			ipField->get_Name( &bsFieldName );
			m_ofs << CT2CA(bsFieldName) << "\t";
		}
		// �폜�����̍ۂ̏o�͂Ɏg�����߁ASHAPE�t�B�[���h�̔ԍ����擾���Ă���
		else
			m_iGeometryFieldCount = i;
	}

	// �Œ蕔���̏o��
	m_ofs << "STATUS\tCOMMENT" << endl;
}

// ���s
bool DeleteAddrPoint::Execute()
{
	// �q�o�[�W�����쐬
	if( !fnCreateChildVer() )
		return false;
	
	// �ҏW�J�n
	if( !fnEditStart() )
		return false;

	// �폜����
	if( !fnDelete() )
		return false;

	// �ҏW�I��
	if( !fnEditStop() )
		return false;

	return true;
}

// �폜�Ώۃ��C����쐬
void DeleteAddrPoint::fnSetDeleteLayer()
{
	m_csSetDeleteLayer.insert(_T("gou_point"));
	m_csSetDeleteLayer.insert(_T("cs_addr_point"));
	m_csSetDeleteLayer.insert(_T("klink_point"));
	m_csSetDeleteLayer.insert(_T("base_rep_point"));
	m_csSetDeleteLayer.insert(_T("extp_point"));
	m_csSetDeleteLayer.insert(_T("teltp_point"));
}

// ���ϐ�������擾
bool DeleteAddrPoint::fnSetInfoFromEnv()
{
#ifdef	_DEBUG
//	_tputenv( _T("DB_MAIN=FIX200911/FIX200911/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("DB_MAIN=SINDYTEST/SINDYTEST/DeleteAddrPoint/5151/galatea") );
//	_tputenv( _T("DB_MAIN=SINDYTEST/SINDYTEST/SDE.DEFAULT/5151/galatea") );
//	_tputenv( _T("DB_MAIN=FIX201001/FIX201001/SDE.DEFAULT/5151/topaz2") );
//	_tputenv( _T("DB_MAIN=SINUP2009B/SINUP2009B/SDE.EDT_SINUP2009B/5151/garnet2") );
	//_tputenv( _T("FC_TARGET=TelTP_Point") );
	//_tputenv( _T("FC_TARGET=FIX200911.TelTP_Point") );
	//_tputenv( _T("FC_TARGET=GOU_POINT") );
	_tputenv( _T("FC_TARGET=Gou_Point") );
#endif

	// SDE�ڑ�
	CString strDBProp;
	if( !strDBProp.GetEnvironmentVariableW( _T("DB_MAIN") ) )
	{
		cerr << "#Error ���ϐ� DB_MAIN ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	if( !m_cWorkspace.Connect( strDBProp ) )
	{
		cerr << "#Error SDE�ڑ��Ɏ��s : " << CT2CA( strDBProp ) << endl;
		return false;
	}

	// �t�B�[�`���N���X�擾
	if( !m_strFCName.GetEnvironmentVariableW( _T("FC_TARGET")) )
	{
		cerr << "#Error ���ϐ� FC_TARGET ���ݒ肳��Ă��܂���" << endl;
		return false;
	}
	// �폜�\���C���ȊO�G���[
	CString	strLayerName;
	int		iPos	= 0;
	iPos			= m_strFCName.Find(_T("."));
	if( iPos > 0 )
		strLayerName	= m_strFCName.Mid( ++iPos );
	else
		strLayerName	= m_strFCName;

	if( m_csSetDeleteLayer.find( strLayerName.MakeLower() ) == m_csSetDeleteLayer.end() )
	{
		cerr << "#Error �w�肵�����C���͍폜�\���C���ł͂���܂��� : " << CT2CA( m_strFCName ) << endl;
		return false;
	}

	m_cTargetPointFC.SetObject( m_cWorkspace.OpenTable( m_strFCName ) );
	if( m_cTargetPointFC == NULL )
	{
		cerr << "#Error �폜�Ώۃt�B�[�`���N���X�̎擾�Ɏ��s : " << CT2CA( m_strFCName ) << endl;
		return false;
	}

	return true;
}

// �폜�����p�q�o�[�W�������쐬
bool DeleteAddrPoint::fnCreateChildVer()
{
	// �o�[�W�����p�̎��ԕ�����擾
	CString		csVarTime = gf::GetCurDateStr( FORVER );

	// �ҏW�p�q�o�[�W�����쐬(DeleteAddrPoint_yyyymmddhhmmss)
	CVersion	cVer;
	CString		csChildVer = cVer.GetInternalName();
	csChildVer	= csChildVer + "_" + csVarTime;

	// �o�[�W������Ή����C���͂���
	IFeatureClassPtr	ipTargetFC( m_cTargetPointFC.OpenTable( m_strFCName ));
	IDatasetPtr			ipDataset( ipTargetFC );
	INamePtr			ipName;
	ipDataset->get_FullName( &ipName );			// IName�擾
	IWorkspacePtr		ipWorkspace;
	ipDataset->get_Workspace( &ipWorkspace );	// IWorkspace�擾

	// �o�[�W�����Ή��̔��ʂ́AIFeatureWorkspaceManage::IsRegisteredAsVersioned ���\�b�h���g�p����
	VARIANT_BOOL		vBool = VARIANT_FALSE;
	IFeatureWorkspaceManagePtr	ipFeatureWorkspaceManage( ipWorkspace );
	ipFeatureWorkspaceManage->IsRegisteredAsVersioned( ipName, &vBool );
	if( vBool == VARIANT_FALSE )
	{
		cerr << "#Error �w�背�C�����o�[�W������Ή��̂��ߏI��" << endl;
		return false;
	}

	// DEFAULT�o�[�W�����͂���
	IVersionPtr ipCurVersion( (IWorkspace*)m_cWorkspace );
	CComBSTR bstr;
	ipCurVersion->get_VersionName( &bstr );
	CString strTmp(bstr);
	if( strTmp.Find(_T("DEFAULT")) != -1 )
	{
		cerr << "#Error �f�t�H���g�o�[�W�����̂��ߏI��" << endl;
		return false;
	}

	// ���ɂ��̃o�[�W���������݂��邩�`�F�b�N
	IVersionedWorkspacePtr ipVerWorkspace( (IWorkspace*)m_cWorkspace );
	IVersionPtr ipChildVersion;
	ipVerWorkspace->FindVersion( CComBSTR(csChildVer), &ipChildVersion );
	if( NULL == ipChildVersion )
	{
		ipCurVersion->CreateVersion( CComBSTR(csChildVer), &ipChildVersion );
		if( NULL == ipChildVersion )
		{
			cerr << "#Error �ҏW�p�o�[�W�����̍쐬�Ɏ��s : " << CT2CA(csChildVer) << endl;
			return false;
		}

		// �q�o�[�W�����ɃR�����g�ǉ�
		CString strComment;
		strComment.Format( _T("DeleteAddrPoint �ɂ��ҏWVer %s"), gf::GetCurAccount() );
		ipChildVersion->put_Description( CComBSTR(strComment) );			
	}
	cout << "�ҏW�p�o�[�W���� : " << CT2CA(csChildVer) << " �̍쐬������" << endl;

	// workspace���쐬�����q�o�[�W�����ɕύX
	m_cWorkspace = (IWorkspacePtr)ipChildVersion;

	return true;
}

// OID���X�g���J���Avector�Ɋi�[
bool DeleteAddrPoint::fnLoadOIDList( CString aOIDListName )
{
	ifstream	fin;
	fin.open( aOIDListName );
	if( !fin )
	{
		cerr << "#Error �t�@�C�����I�[�v���ł��܂���" << endl;
		return false;
	}

	int		i = 0;
	while(1)
	{
		char	acBuf[1024];
		i++;

		fin.getline( acBuf, 1024 );

		// �I�[�Ȃ�΃��[�v�𔲂���
		if( fin.eof() )
			break;

		if( !fin )
		{
			cerr << "�y " << i << " �s�ځz�ǂݍ��ݎ��s" << endl;
			fin.close();
			return false;
		}
		string	asBuf	= acBuf;
		if(asBuf.find_first_not_of("0123456789") != string::npos)
		{
			cerr << "�y " << i << " �s�ځz�s����ID( " << asBuf << " )" << endl;
			fin.close();
			return false;
		}
		long	aOID = atol( asBuf.c_str() );
		m_lOIDList.push_back( aOID );
	}
	fin.close();

	return true;
}

// �ҏW�J�n
bool DeleteAddrPoint::fnEditStart()
{
	HRESULT	aResult;
	aResult = m_cWorkspace.StartEditing( VARIANT_FALSE );
	if(FAILED(aResult))
	{
		cerr << "StartEditing ���s" << endl;
		return false;
	}
	aResult = m_cWorkspace.StartEditOperation();
	if(FAILED(aResult))
	{
		cerr << "StartEditOperation ���s" << endl;
		m_cWorkspace.StopEditing(VARIANT_FALSE);
		return false;
	}

	// �ҏW�p���[�N�X�y�[�X����폜�Ώۃt�B�[�`���N���X��蒼��������
	m_cTargetPointFC.SetObject( m_cWorkspace.OpenTable(m_cTargetPointFC.GetNameString()->GetTableName()) );
	if( NULL == m_cTargetPointFC )
	{
		cerr << "#Error �폜�Ώۃ|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s" << CT2CA(m_cTargetPointFC.GetNameString()->GetTableName()) << std::endl;
		return false;
	}

	return true;
}

// �폜����
bool DeleteAddrPoint::fnDelete()
{
	// OID���X�g�̃��[�v
	list<long>::iterator	itrOID;
	for( itrOID = m_lOIDList.begin() ; itrOID != m_lOIDList.end() ; itrOID++ )
	{
		CString strWhere;
		strWhere.Format( _T("OBJECTID = %d"), *itrOID );

		IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

		long lTarCount						= m_cTargetPointFC.FeatureCount( ipQuery );
		IFeatureCursorPtr ipFeatureCursor	= m_cTargetPointFC.Search( ipQuery, VARIANT_FALSE );

		if( lTarCount > 0 && ipFeatureCursor )
		{
			cout << "�y " << (*itrOID) << " �z�Ή��f�[�^�m�F";

			IFeaturePtr		ipFeature;
			while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK )
			{
				IGeometryPtr	ipGeo;
				ipFeature->get_ShapeCopy( &ipGeo );
				IPointPtr		ipPoint( ipGeo );
				gf::ConvPoint2MXY( ipPoint, &m_cMXY );
				m_ofs << "0\t" << m_cMXY.m_nMesh << "\t" << m_cMXY.m_nX << "\t" << m_cMXY.m_nY << "\t" << CT2CA(m_strFCName) << "\t";

				// DB����S�Ă̏����擾���A�o��
				IFieldsPtr	ipFields;
				long		iFieldNum = 0;
				ipFields	= m_cTargetPointFC.GetFields();
				ipFields->get_FieldCount( &iFieldNum );

				IFieldPtr	ipField;
				CComBSTR	bsValue;
				CComVariant	vaValue;
				SYSTEMTIME st;

				// SHAPE�t�B�[���h�ȊO�̃t�B�[���h�S�ďo��
				for( int i = 0 ; i < iFieldNum  ; i++ )
				{
					if( i == m_iGeometryFieldCount )
						continue;

					ipFeature->get_Value( i, &vaValue );
					switch( vaValue.vt )
					{
					case VT_I2:
						m_ofs << vaValue.iVal << "\t";
						break;
					case VT_I4:
						m_ofs << vaValue.lVal << "\t";
						break;
					case VT_BSTR:
						m_ofs << CT2CA(vaValue.bstrVal) << "\t";
						break;
					case VT_DATE:
						VariantTimeToSystemTime(vaValue.date, &st);
						m_ofs << st.wYear << "/" << st.wMonth << "/" << st.wDay << " " << st.wHour << ":" << st.wMinute <<  ":"  << st.wSecond << "\t";
						break;
					case VT_EMPTY:
					case VT_NULL:
					default:
						m_ofs << "\t";
						break;
					}
				}

				if( ipFeature->Delete() == S_OK )
				{
					cout << "...�폜����" << endl;			
					m_ofs << "�폜����";
				} 
				else 
				{
					cout << "...�폜���s" << endl;	
					m_ofs << "�폜���s";
				}

				m_ofs << endl;
			}
		}
	}
	return true;
}

// �ҏW�I��
bool DeleteAddrPoint::fnEditStop()
{	
	if(FAILED( m_cWorkspace.StopEditOperation() ))
	{
		cerr << "StopEditOperation ���s" << endl;
		return false;
	}
	if(FAILED( m_cWorkspace.StopEditing(true) ))
	{
		cerr << "StopEditing ���s" << endl;
		return false;
	}

	// �I�����ԏo��
	CString csEndTime = gf::GetCurDateStr();
	m_ofs << "# �I�����ԁF" << CT2CA(csEndTime) << endl;

	return true;
}
