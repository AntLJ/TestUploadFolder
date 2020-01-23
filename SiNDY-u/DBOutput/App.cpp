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

#include "StdAfx.h"
#include "App.h"

#include <iomanip>
#include <sindy/workspace.h>
#include <crd_cnv/coord_converter.h>
static crd_cnv g_cCrdCnv;	/// crd_cnv�C���X�^���X

// ������
void CApp::printUsage()
{
	std::cerr << "�yDB�_���p�[�g�p�@�z\n"
			  << "(Usage)DBOutput.exe <�ڑ�DB> <�e�[�u����> <�ݒ�t�@�C��> <���̑��I�v�V����> (> �o�̓��O)\n"
			  << "�E���̑��I�v�V�����ɂ���\n"
			  << "  -w [Where��]         ... �����ōi�荞�݂����Ƃ��Ɏg�p���ĉ�����\n"
			  << "  -ol [OBJECTID���X�g] ... �w�肵��OBJECITID���X�g�̂��̂̂ݏo�͑Ώ�(-w �� -ml�Ƃ̕��p�s��)\n"
			  << "  -ml [���b�V�����X�g] ... �w�肵�����b�V�����X�g���̃f�[�^�̂ݏo�͑Ώ�\n"
			  << "  -lp ... �|���S���ɑ΂�����W�l���A�d�S�ł͂Ȃ����x���|�C���g�Ŏ擾\n"
			  << "  -s  ... �w�肵�����ڂ��e�[�u���ɖ����ꍇ�G���[�Ƃ��Ȃ�(�X�L�b�v���ďo�͂���)" << std::endl;
}

// ������
bool CApp::init( int argc, TCHAR* argv[] )
{
	if( argc < 4 )
	{
		std::cerr << "#ERROR �����̐����s���ł�" << std::endl;
		return false;
	}

	// �ڑ�
	INamePtr ipName( sindy::create_workspace_name(argv[1]) );
	if( ipName )
	{
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		m_ipWorkspace = ipUnknown;
	}

	if( ! m_ipWorkspace )
	{
		std::cerr << "#ERROR DB�ڑ��Ɏ��s : " << CT2CA(argv[1]) << std::endl;
		return false;
	}
	std::cerr << "#�ڑ��J�n : " << CT2CA(argv[1]) << std::endl;

#ifdef SiNDY
	// [bug11555]�Ή� �� ora�G���[���p
	CComBSTR bstrProgID;
	if(S_OK != IWorkspaceNamePtr(ipName)->get_WorkspaceFactoryProgID(&bstrProgID)){
		std::cerr << "#ERROR ���[�N�X�y�[�X���擾�Ɏ��s" << std::endl;
		return false;
	}

	// SDE�ڑ����̂�SQL���s
	if(0 <= CString(bstrProgID).Find(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))){
		if (FAILED(m_ipWorkspace->ExecuteSQL(CComBSTR("alter session set cursor_sharing=exact")))) {
			std::cerr << "#ERROR SDE�ւ�CURSOR_SHARING�ύX�Ɏ��s" << std::endl;
			return false;
		}
	}
#endif

	// �e�[�u���擾
	IFeatureWorkspacePtr(m_ipWorkspace)->OpenTable( CComBSTR(argv[2]), &m_ipTable );
	if( NULL == m_ipTable )
	{
		std::cerr << "#ERROR �e�[�u���̎擾�Ɏ��s : " << CT2CA(argv[2]) << std::endl;
		return false;
	}
	std::cerr << "#�o�͑Ώۃe�[�u�� : " << CT2CA(argv[2]) << std::endl;

	// �e�[�u���ɐ}�`���܂ނ��ǂ����H
	bool bShape = false;
	if( IFeatureClassPtr(m_ipTable) )
		bShape = true;

	// �I�v�V����
	for( int i = 4; i < argc; ++ i )
	{
		if( _tcscmp(argv[i], _T("-w")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:	m_emQueryMode = kWhere;		break;
			case kMesh:m_emQueryMode = kMeshWhere;	break;
			case kOID:
				std::cerr << "#ERROR -w �� -ol �I�v�V�����͓����g�p�ł��܂���" << std::endl;
				return false;			
			default:
				break;
			}
			m_strWhereClause = argv[++i];
			if( m_strWhereClause.IsEmpty() )
			{
				std::cerr << "#ERROR -w �I�v�V������ where �傪�w�肳��Ă��܂���" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-ol")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:	m_emQueryMode = kOID;	break;
			default:
				std::cerr << "#ERROR -ol �I�v�V�����́A���̃I�v�V�����Ɠ����g�p�ł��܂���" << std::endl;
				return false;
			}
			m_strOIDList = argv[++i];
			if( m_strOIDList.IsEmpty() )
			{
				std::cerr << "#ERROR -ol �I�v�V������OID���X�g�t�@�C�����w�肳��Ă��܂���" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-ml")) == 0 )
		{
			switch(m_emQueryMode)
			{
			case kNone:		m_emQueryMode = kMesh;		break;
			case kWhere:	m_emQueryMode = kMeshWhere;	break;
			case kOID:
				std::cerr << "#ERROR -ml �� -ol �I�v�V�����͓����g�p�ł��܂���" << std::endl;
				return false;
			default:
				break;
			}
			m_strMeshList = argv[++i];
			if( m_strMeshList.IsEmpty() )
			{
				std::cerr << "#ERROR -ml �I�v�V�����Ƀ��b�V�����X�g�t�@�C�����w�肳��Ă��܂���" << std::endl;
				return false;
			}
			if( !bShape )
			{
				std::cerr << "#ERROR �}�`�Ȃ��̃e�[�u�����w�肵���ꍇ -ml �I�v�V�����͎g�p�ł��܂���" << std::endl;
				return false;
			}
		}
		else if( _tcscmp(argv[i], _T("-lp")) == 0 )
		{
			m_bLabelPoint = true;
		}
		else if( _tcscmp(argv[i], _T("-s")) == 0 )
		{
			m_bSkip = true;
		}
		else
		{
			std::cerr << "#ERROR �s���Ȃ��̑��I�v�V�����ł� : " << argv[i] << std::endl;
			return false;
		}
	}

	// �ݒ�t�@�C���ǂݍ���
	if( !m_cInit.init(argv[3], m_ipTable, bShape, m_bSkip) )
		return false;

	std::cout.precision(12);

	return true;
}

// ���s
bool CApp::execute()
{
	bool bResult = true;

	// �����i�胂�[�h�ɉ����ď���
	if( m_emQueryMode != kOID )	// OID���X�g�ȊO
	{
		std::list<long> listMesh;
		switch( m_emQueryMode )
		{
		case kMesh:
		case kMeshWhere:
			if( !loadList(m_strMeshList, listMesh) ) {
				std::cerr << "#ERROR ���b�V�����X�g�̓ǂݍ��݂Ɏ��s" << std::endl;
				return false;
			}
			break;
		default:
			listMesh.push_back( 0 );	// ���[�v��1��̂݉񂷂悤�ɒǉ�
			break;
		}

		for( std::list<long>::const_iterator itr = listMesh.begin(); itr != listMesh.end(); ++itr )
		{
			CString strPostErrMsg;
			if( *itr != 0 )
				strPostErrMsg.Format("���b�V���R�[�h : %ld", *itr);

			ISpatialFilterPtr ipQuery( CLSID_SpatialFilter );
			if( kWhere == m_emQueryMode || kMeshWhere == m_emQueryMode )
				ipQuery->put_WhereClause( CComBSTR(m_strWhereClause) );

			// ���b�V�����X�g�w�莞�́A���b�V����`����
			if( kMesh == m_emQueryMode || kMeshWhere == m_emQueryMode )
			{
				IGeometryPtr ipMeshGeo( GetMeshShape(*itr) );
				CComBSTR bstr;
				((IFeatureClassPtr)m_ipTable)->get_ShapeFieldName( &bstr );
				if( ipMeshGeo && bstr )
				{
					ipQuery->put_GeometryField( bstr );
					ipQuery->putref_Geometry( ipMeshGeo );
					ipQuery->put_SpatialRel( esriSpatialRelIntersects );
					ipQuery->put_SearchOrder( esriSearchOrderSpatial );
				}
				else
				{
					std::cerr << "#ERROR ���b�V����`���擾�ł��܂���B" << CT2CA(strPostErrMsg) << std::endl;
					bResult = false;
				}
			}

			_ICursorPtr ipCursor;
			m_ipTable->Search( ipQuery, VARIANT_FALSE, &ipCursor );
			if( !ipCursor )
			{
				std::cerr << "#ERROR ���R�[�h�̌����Ɏ��s���܂����B" << CT2CA(strPostErrMsg) << std::endl;
				bResult = false;
				continue;
			}

			int iCount = 0;
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				if( !output( ipRow, strPostErrMsg ) )
				{
					bResult = false;
				}
				iCount++;
			}
			if( iCount == 0 )
				std::cerr << "#INFO ���R�[�h������܂���ł����B" << CT2CA(strPostErrMsg) << std::endl;
		}
		
	}
	else	// OID���X�g
	{
		// OID���X�g�ǂݍ���
		std::list<long> listOID;
		if( ! loadList(m_strOIDList, listOID) )
		{
			std::cerr << "#ERROR OID���X�g�̓ǂݍ��݂Ɏ��s" << std::endl;
			return false;
		}

		for( std::list<long>::const_iterator itr = listOID.begin(); itr != listOID.end(); ++itr )
		{
			CString strPostErrMsg;
			strPostErrMsg.Format("OID : %ld", *itr);

			_IRowPtr ipRow;
			m_ipTable->GetRow( *itr, &ipRow );
			if( ipRow )
			{
				if( !output( ipRow, strPostErrMsg ) )
				{
					bResult = false;
				}
			}
			else
			{
				std::cerr << "#ERROR �Y���h�c�̃I�u�W�F�N�g�͑��݂��܂���B" << CT2CA(strPostErrMsg) << std::endl;
				bResult = false;
			}
		}
	}
	return bResult;
}

//////////////////////////////////
//	����J�֐�
//////////////////////////////////

// OID�E���b�V�����X�g�ǂݍ���(���p)
bool CApp::loadList( LPCTSTR lpcszFile, std::list<long>& rList )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		std::copy( std::istream_iterator<long>(ifs), std::istream_iterator<long>(), std::back_inserter(rList) );
		return ifs.eof();
	}
	else
		return false;
}

// �w�胁�b�V���R�[�h�̃��b�V���`��擾
IGeometryPtr CApp::GetMeshShape( long lMeshCode )
{
	if( !m_ipMeshFC )
	{
		CString strMeshFName;
		if( 1000 < lMeshCode && lMeshCode < 10000 )				strMeshFName = _T("REFERENCE.MiddleMesh");
		else if( 100000 < lMeshCode && lMeshCode < 1000000 )	strMeshFName = _T("REFERENCE.BaseMesh");
		else if( 10000000 < lMeshCode )	strMeshFName =			strMeshFName = _T("REFERENCE.CityMesh");
		else
			return NULL;

		IFeatureWorkspacePtr(m_ipWorkspace)->OpenFeatureClass( _bstr_t(strMeshFName), &m_ipMeshFC );
		if( !m_ipMeshFC )
			return NULL;
	}

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	CString strWhere;
	strWhere.Format( _T("MESHCODE = %ld"), lMeshCode );
	ipQuery->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr ipCursor;
	m_ipMeshFC->Search( ipQuery, VARIANT_FALSE, &ipCursor );
	IFeaturePtr ipFeature;
	if( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
	{
		IGeometryPtr ipGeo;
		ipFeature->get_Shape( &ipGeo );
		return ipGeo;
	}
	else
		return NULL;
}

// �o��
bool CApp::output( _IRow* ipRow, const CString& strPostErrMsg )
{
	try
	{
		// �ŏ��Ɉʒu���o�͂���K�v���邩���ׂ�B�K�v�Ȃ獶�L�Ɉʒu���擾(�ܓx�o�x)
		WKSPoint point;
		point.X = 0.0; point.Y = 0.0;
		int nMeshcode = 0, nMeshX = 0, nMeshY = 0;
		IGeometryPtr ipGeo = NULL;
		if( m_cInit.getShapeFieldCount() > 0 )
		{
			if( FAILED( IFeaturePtr(ipRow)->get_Shape( &ipGeo ) ) || !ipGeo )
			{
				std::cerr << "#ERROR �}�`�̎擾�Ɏ��s " << CT2CA(strPostErrMsg) << std::endl;
				return false;
			}
			if( !gf::getLonLat( ipGeo, point, m_bLabelPoint ) )
			{
				std::cerr << "#ERROR �ܓx�o�x�̎擾�Ɏ��s " << CT2CA(strPostErrMsg) << std::endl;
				return false;
			}
		}

		CComVariant vaValue;
		const FIELDLIST& listField = m_cInit.getFieldList();
		for( FIELDLIST::const_iterator itr = listField.begin(); itr != listField.end(); ++itr )
		{
			if( itr != listField.begin() )		std::cout << "\t";

			// SHAPE�t�B�[���h�łȂ����ŏꍇ�킯
			if( !itr->m_bShapeField )
			{
				// �_�~�[�t�B�[���h���ۂ��ŏꍇ����
				if( !itr->m_bDummy )
				{
					if( FAILED( ipRow->get_Value( itr->m_lFID, &vaValue ) ) )
					{
						std::cerr << "#ERROR �l�̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}

					// �ʏ� or _DOMAIN����Ȃ����ŏꍇ����
					if( !itr->m_bDomain )
						std::cout << vaValue;
					else
					{
						CString strDomain;
						if( !gf::getDomain( ipRow, itr->m_lFID, vaValue.lVal, strDomain ) )	// _DOMAIN�Ȃ�΁A�h���C�����擾����
						{
							std::cerr << "#ERROR �h���C���̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
							std::cout << std::endl;
							return false;
						}
						std::cout << strDomain;
					}

					vaValue.Clear();
				}
				else
					std::cout << itr->m_strDummyVal;
			}
			else
			{
				// SHAPE
				if( FN_LONGITUDE == itr->m_strName )		std::cout << point.X;
				else if( FN_LATITUDE == itr->m_strName )	std::cout << point.Y;
				else if( FN_POINTS == itr->m_strName )		// [Bug 5691]
				{
					LONG lCount = 0;
					if( !gf::getPointCount( ipGeo, lCount ) )
					{
						std::cerr << "#ERROR �l�̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << lCount;
				}
				else if( FN_ISLINEAR == itr->m_strName )	// [Bug 5691]
				{
					bool bResult = false;
					if( !gf::isLinearObject(ipGeo, bResult) )
					{
						std::cerr << "#ERROR �l�̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << bResult? 1 : 0;
				}
				else if( FN_LENGTH == itr->m_strName )		// [Bug 7086]
				{
					double dLen = -1.0;
					if( !gf::getLength(ipGeo, dLen) )
					{
						std::cerr << "#ERROR �l�̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << dLen;
				}
				else if( FN_AREA == itr->m_strName )		// [Bug 7086]
				{
					double dArea = -1.0;
					if( !gf::getArea(ipGeo, dArea) )
					{
						std::cerr << "#ERROR �l�̎擾�Ɏ��s(" << CT2CA(itr->m_strName) << ") " << CT2CA(strPostErrMsg) << std::endl;
						std::cout << std::endl;
						return false;
					}
					std::cout << dArea;
				}
				else
				{
					if( nMeshcode == 0 )
						g_cCrdCnv.LLtoMesh(  point.X, point.Y, m_cInit.getMeshLevel(), &nMeshcode, &nMeshX, &nMeshY, 1 );

					if( FN_MESHX == itr->m_strName )		std::cout << nMeshX;
					else if( FN_MESHY == itr->m_strName )	std::cout << nMeshY;
					else									std::cout << nMeshcode;				
				}
			}
		}
		std::cout << std::endl;
		return true;
	}
	catch( _com_error& e )
	{
		std::cerr << "#COM ERROR ���� : " << e.ErrorMessage() << " " << CT2CA(strPostErrMsg) << std::endl;
	}
	catch( std::exception& e )
	{
		std::cerr << "#��O���� : " << e.what() << " " << CT2CA(strPostErrMsg) << std::endl;
	}
	return false;
}
