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
#include "KP2WorkTxt.h"

#include <string>

#include <ArcHelperEx/GlobalFunctions.h>

// ������
bool CKP2WorkTxt::Init()
{
	// �����ΏۃG���A�擾
	if( m_rParam.m_pairArea.first == _T("-ca") || m_rParam.m_pairArea.first == _T("-cr") )
	{
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		CString strWhere(_T("OBJECTID IS NOT NULL"));
		if( m_rParam.m_pairArea.first == _T("-cr") )
		{
			// �͈͂��擾
			int nPos = m_rParam.m_pairArea.second.Find('-');
			if( nPos > 0 )
			{
				strWhere.Format( _T("%s between '%s00000000000000000' and '%s99999999999999999'"), 
								 klink_point::kAddrCodeOrg, m_rParam.m_pairArea.second.Left(nPos), m_rParam.m_pairArea.second.Mid(nPos+1) );
			}
			else
			{
				std::cerr << "#Error �s�撬���R�[�h�w��t�H�[�}�b�g���s���ł��B(start-end) : " << CT2CA(m_rParam.m_pairArea.second) << std::endl;
				return false;
			}
		}

		ipQuery->put_WhereClause( _bstr_t(strWhere) );
		ipQuery->put_SubFields( _bstr_t(klink_point::kAddrCodeOrg) );

		CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
		CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

		_ICursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipQuery, VARIANT_FALSE) );
		if( ipCursor )
		{
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				CKLinkPoint cKPPoint( ipRow, sindyTableType::klink_point, false, cFields, cNames );
				m_setCode.insert( cKPPoint.GetAddrCodeOrg().Left(5) );
			}
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cl")  )
	{
		if( !LoadCityCodeList(m_rParam.m_pairArea.second) )
		{
			std::cerr << "#Error �s�撬�����X�g����̎s�撬���R�[�h���X�g�����Ɏ��s : " << CT2CA(m_rParam.m_pairArea.second) << std::endl;
			return false;
		}
	}
	else if( m_rParam.m_pairArea.first == _T("-cs") )
	{
		m_setCode.insert( m_rParam.m_pairArea.second );
	}
	else
	{
		std::cerr << "#Error ���s�G���A������ł��܂��� : " << CT2CA(m_rParam.m_pairArea.first) << std::endl;
		return false;
	}

	// �Z���}�X�^�֐ڑ�
	return m_cACHelper.Connect(m_rParam.m_strJDBProp);
}

// �f�[�^�o��
bool CKP2WorkTxt::WriteData()
{
	// �ƌ`�����N�|�C���g�̃t�B�[���h�C���f�b�N�X�����L���b�V��
	CSPFieldMap cFields( m_rParam.m_cPointFC.GetFieldMap() );
	CSPTableNameString cNames( m_rParam.m_cPointFC.GetNameString() );

	for( std::set<CString>::const_iterator itCode = m_setCode.begin(); itCode != m_setCode.end(); ++itCode )
	{
		m_setWritedOID.clear();
		std::cout << "#" << CT2CA(*itCode) << std::endl;

		// �o�̓t�@�C���I�[�v��
		CString strFile;
		strFile.Format( _T("%s\\kpw%s.txt"), m_rParam.m_strOutDir, *itCode );
		std::ofstream ofs( strFile, std::ios::binary );
		if( !ofs )
		{
			std::cerr << "#Error �o�̓t�@�C���̍쐬�Ɏ��s : " << CT2CA(strFile) << std::endl;
			continue;
		}

		// �w��5�����̍s���E����
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
		CString strWhere;
		strWhere.Format( _T("%s = '%s'"), city_admin::kCityCode, *itCode );
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		long lAdminCount = m_rParam.m_cCityAdminFC.FeatureCount( ipFilter );
		IFeatureCursorPtr ipAdminCursor( m_rParam.m_cCityAdminFC.Search(ipFilter, false) );
		IFeaturePtr ipAdminFeature;
		long lCount = 1;
		while( ipAdminCursor && ipAdminCursor->NextFeature(&ipAdminFeature) == S_OK )
		{
			// �s���E���ɂ���ƌ`�����N�|�C���g�擾
			CCityAdmin cAdmin( ipAdminFeature, sindyTableType::city_admin, false, m_rParam.m_cCityAdminFC.GetFieldMap(), m_rParam.m_cCityAdminFC.GetNameString() );

			ISpatialFilterPtr ipSpFIlter( AheInitSpatialFilter(NULL, cAdmin.GetShapeCopy()) );
			if( ipSpFIlter )
			{				
				IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipSpFIlter, VARIANT_FALSE) );
				IFeaturePtr ipFeature;
				while( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
				{
					CKLinkPoint cKPPoint( ipFeature, sindyTableType::klink_point, false, cFields, cNames );

					// �܂��f�[�^�����Ă��Ȃ� and �s���E�Ɠ���5���������Ă��� �� ���ԃf�[�^��
					if( m_setWritedOID.find(cKPPoint.GetOID()) == m_setWritedOID.end()
						&& cAdmin.GetAddrCode().Left(5) == cKPPoint.GetAddrCodeOrg().Left(5) )
					{
						// �Z���R�[�h�̍ŐV��Ԃ��擾
						CString strNewAddrCode( GetNewestCode(cKPPoint.GetAddrCodeOrg().Left(11)) + cKPPoint.GetAddrCodeOrg().Mid(11) );
						if( strNewAddrCode.GetLength() == 22 )
						{
							// �Z���R�[�h���p�~or�����ł���΃f�[�^�����Ȃ�(Bug7975)
							addrCodeStatus acStatus = m_cACHelper.GetCodeStatus( strNewAddrCode.Left(11) );
							if( kShiko == acStatus || kTmpShiko == acStatus )
							{
								// �s���E�Ƃ̐����m�F
								long lEqualF = 0;
								if( GetEqualF(strNewAddrCode, NULL, &cAdmin, &lEqualF) && lEqualF != 1 )
								{
									WriteData( ofs, cKPPoint, strNewAddrCode, lEqualF );	// �f�[�^�����o��
									m_setWritedOID.insert( cKPPoint.GetOID() );				// �����ς�OID���X�g�ɓo�^
								}
							}
						}
						else
							PrintError( &cKPPoint, _T("ERROR"), _T("�ŐV��Ԃ̏Z���R�[�h�̎擾�Ɏ��s"), cKPPoint.GetAddrCodeOrg() );
					}					
				}
			}
			std::cout << lCount++ << " / " << lAdminCount << " �s���E\r";
		}

		std::cout << std::endl;

		// �w��5�����s���E���ɑ��݂��Ȃ��ꍇ���l�����ĉƌ`�����N�|�C���g���������
		strWhere.Format( _T("%s like '%s%%'"), klink_point::kAddrCodeOrg, *itCode );
		ipFilter->put_WhereClause( _bstr_t(strWhere) );

		long lAllCount = m_rParam.m_cPointFC.FeatureCount( ipFilter );
		IFeatureCursorPtr ipCursor( m_rParam.m_cPointFC.Search(ipFilter, VARIANT_FALSE) );
		if( lAllCount > 0 && ipCursor )
		{
			lCount = 1;
			IFeaturePtr ipFeature;
			while( ipCursor->NextFeature(&ipFeature) == S_OK )
			{
				CKLinkPoint cKPPoint( ipFeature, sindyTableType::klink_point, false, cFields, cNames );
				
				// �s���E����̒��ԃf�[�^�����ł��Ȃ����̂��������ŃJ�o�[����
				if( m_setWritedOID.find(cKPPoint.GetOID()) == m_setWritedOID.end() )
				{
					// �Z���R�[�h�̍ŐV��Ԃ��擾
					CString strNewAddrCode( GetNewestCode(cKPPoint.GetAddrCodeOrg().Left(11)) + cKPPoint.GetAddrCodeOrg().Mid(11) );
					if( strNewAddrCode.GetLength() == 22 )
					{
						// �Z���R�[�h���p�~or�����ł���΃f�[�^�����Ȃ�(Bug7975)
						addrCodeStatus acStatus = m_cACHelper.GetCodeStatus( strNewAddrCode.Left(11) );
						if( kShiko == acStatus || kTmpShiko == acStatus )
						{
							// �s���E�Ƃ̐������擾
							IGeometryPtr ipGeo;
							((IFeature*)cKPPoint)->get_Shape( &ipGeo );
							long lEqualF = 0;
							if( !GetEqualF(strNewAddrCode, ipGeo, NULL, &lEqualF) )
							{						
								PrintError( &cKPPoint, _T("ERROR"), _T("�s���E���擾�ł��Ȃ�") );				
								lEqualF = 1;
							}

							WriteData( ofs, cKPPoint, strNewAddrCode, lEqualF );	// �f�[�^�����o��
						}
					}
					else
						PrintError( &cKPPoint, _T("ERROR"), _T("�ŐV��Ԃ̏Z���R�[�h�̎擾�Ɏ��s"), cKPPoint.GetAddrCodeOrg() );
				}
				std::cout << lCount++ << " / " << lAllCount << " ��\r";
			}
			std::cout << std::endl;
		}
	}
	return true;
}

/////////////////////////////////////////////////
//	����J�����o�֐�
/////////////////////////////////////////////////

// �s�撬���R�[�h���X�g�ǂݍ���
bool CKP2WorkTxt::LoadCityCodeList( const _TCHAR* lpcszFile )
{
	std::ifstream ifs( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		while( !ifs.eof() )
		{
			std::string strBuff;
			std::getline( ifs, strBuff );
			if( !strBuff.empty() )
				m_setCode.insert( CString(strBuff.c_str()) );
		}
		return true;		
	}
	else
		return false;
}

// �ŐV11���Z���R�[�h�擾
CString CKP2WorkTxt::GetNewestCode( const _TCHAR* lpcszAddrCode )
{
	std::map<CString, CString>::const_iterator it = m_mapNewestAddrCode.find( lpcszAddrCode );
	if( it == m_mapNewestAddrCode.end() )
	{
		CString strSQL;
		strSQL.Format( _T("select getnewestcode( '%s' ) from dual"), lpcszAddrCode );
		CJusyoRecordSet cRecord;
		m_cACHelper.Select( strSQL, cRecord );
		if( cRecord.GetRecordCount() == 1 )
		{
			CString strNewCode( cRecord.GetStringValue((long)0) );
			if( strNewCode.GetLength() == 11 )
			{
				m_mapNewestAddrCode.insert( std::pair<CString, CString>(lpcszAddrCode, strNewCode) );
				return strNewCode;
			}
			else
				return _T("");
		}
		else
			return _T("");
	}
	else
		return it->second;

}

// ���񋟌��R�[�h�ϊ�
const _TCHAR* CKP2WorkTxt::ConvInfoSrc( klink_point::info_src::ECode eInfoSrc )
{
	switch( eInfoSrc )
	{
	case klink_point::info_src::kUnlooked:				return _T("AA");	// ������(���W�Ȃ�)
	case klink_point::info_src::kWallLinkData:			return _T("03");	// �ƌ`�����N�f�[�^
	case klink_point::info_src::kGouMaintenanceData:	return _T("13");	// �������f�[�^
	case klink_point::info_src::kWorking:				return _T("BB");	// ��ƒ�
	default:	return _T("");
	}
}

// ���͗p���W���x�R�[�h�ϊ�
const _TCHAR CKP2WorkTxt::ConvInputSeido( klink_point::input_seido::ECode eInputSeido )
{
	switch( eInputSeido )
	{
	case klink_point::input_seido::kNone:						return _T('A');		// ������
	case klink_point::input_seido::kNoVerification:				return _T('B');		// ������
	case klink_point::input_seido::kMultipleWallCounterData:	return _T('C');		// �Ή��ƌ`����(�f�[�^)
	case klink_point::input_seido::kNotWallCounterData:			return _T('D');		// �Ή��ƌ`�Ȃ�(�f�[�^)
	case klink_point::input_seido::kAddrClericalMistake:		return _T('E');		// �Z������L
	case klink_point::input_seido::kUnmatchedName:				return _T('G');		// ���̕s��v
	case klink_point::input_seido::kUnmatchedShape:				return _T('H');		// �`��s��v
	case klink_point::input_seido::kMultipleWallCounterSrc:		return _T('I');		// �Ή��ƌ`����(����) 
	case klink_point::input_seido::kNotWallCounterSrc:			return _T('J');		// �Ή��ƌ`�Ȃ�(����) 
	case klink_point::input_seido::kUnmatchedBlock:				return _T('K');		// �X��s��v
	case klink_point::input_seido::kOther:						return _T('F');		// ���̑�
	default:													return _T('0');
	}
}

// �����[�X�p���W���x�R�[�h�ϊ�
const _TCHAR CKP2WorkTxt::ConvReleaseSeido( klink_point::release_seido::ECode eReleaseSeido )
{
	switch( eReleaseSeido )
	{
	case klink_point::release_seido::kNone:					return _T('A');		// ������
	case klink_point::release_seido::kMunicipalityBorder:	return _T('1');		// �s��S�����E
	case klink_point::release_seido::kOazaBorder:			return _T('2');		// ���E�厚�E
	case klink_point::release_seido::kAzaBorder:			return _T('3');		// ���ځE��(�厚)�E
	case klink_point::release_seido::kBlock:				return _T('4');		// �X��(�n��)
	case klink_point::release_seido::kPinpoint:				return _T('5');		// �s���|�C���g
	case klink_point::release_seido::kWall:					return _T('6');		// �ƌ`
	case klink_point::release_seido::kUnmatchedBlock:		return _T('K');		// �X��s��v
	case klink_point::release_seido::kOther:				return _T('F');		// ���̑�
	default:	return _T('*');
	}
}

// �s���E�Ƃ̐������擾
bool CKP2WorkTxt::GetEqualF( const CString& strKPAddrCode, IGeometry* ipGeo, const CCityAdmin* pAdmin, long* pEqualF )
{
	// �n�Ԃ�[00000]�ō���not[0000]�ȕs���f�[�^�𖳌���
	if( strKPAddrCode.Mid(11, 5) == _T("00000") && strKPAddrCode.Mid(16, 4) != _T("0000") )
	{
		*pEqualF = 2;
		return true;
	}

	// �s���E�擾(���E��ɂ���Ƃ��́A�����ꂩ�Ɛ��������΂n�j�Ƃ���)
	if( !pAdmin )
	{
		// �܂��R�t���ĂȂ��Ȃ猟��
		ISpatialFilterPtr ipSpFIlter( AheInitSpatialFilter(NULL, ipGeo) );
		if( ipSpFIlter )
		{
			long lCount = m_rParam.m_cCityAdminFC.FeatureCount( ipSpFIlter );
			if( lCount > 0 )
			{
				IFeatureCursorPtr ipFeatureCursor = m_rParam.m_cCityAdminFC.Search( ipSpFIlter, false );
				IFeaturePtr ipFeature;
				while( ipFeatureCursor && ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
				{
					CCityAdmin cAdmin( ipFeature, sindyTableType::city_admin, false, m_rParam.m_cCityAdminFC.GetFieldMap(), m_rParam.m_cCityAdminFC.GetNameString() );
				
					// ���E���ڃR�[�h�� 001 �` 099 �Ȃ�11����v�A���̑���8����v�m�F
					if( 0 < _tstoi(cAdmin.GetAddrCode().Mid(8, 3)) && _tstoi(cAdmin.GetAddrCode().Mid(8, 3)) < 100 )
					{
						if( strKPAddrCode.Left(11) == cAdmin.GetAddrCode().Left(11) )
						{
							*pEqualF = 0;
							return true;
						}
					}
					else
					{
						if( strKPAddrCode.Left(8) == cAdmin.GetAddrCode().Left(8) )
						{
							*pEqualF = 0;
							return true;
						}
					}
				}
				*pEqualF = 1;
				return true;
			}
		}
	}
	else
	{
		// ���ɕR�t���s���E�킩���Ă�Ƃ�

		// ���E���ڃR�[�h�� 001 �` 099 �Ȃ�11����v�A���̑���8����v�m�F
		if( 0 < _tstoi(pAdmin->GetAddrCode().Mid(8, 3)) && _tstoi(pAdmin->GetAddrCode().Mid(8, 3)) < 100 )
		{
			if( strKPAddrCode.Left(11) == pAdmin->GetAddrCode().Left(11) )
			{
				*pEqualF = 0;
				return true;
			}
		}
		else
		{
			if( strKPAddrCode.Left(8) == pAdmin->GetAddrCode().Left(8) )
			{
				*pEqualF = 0;
				return true;
			}
		}
		*pEqualF = 1;
		return true;
	}

	// �s���E�擾�ł��Ȃ��Ȃ�G���[
	*pEqualF = 1;
	return false;	
}

// �f�[�^�����o��
void CKP2WorkTxt::WriteData( std::ofstream& stream, const CKLinkPoint& cKPPoint, const CString& strAddrCode, long lEqualF )
{
	// ���b�V���R�[�h�E�o�x�E�ܓx�擾
	WKSPoint point;
	((IPointPtr)cKPPoint.GetShapeCopy())->QueryCoords( &point.X, &point.Y );

	// �f�[�^�����o��
	CString strData;
	strData.Format( _T("%d,%s,%s,%c,%c,%d,%d,%s,%s,%d,%s,%d,%.12f,%.12f"), 
					cKPPoint.GetOID(),									// OID
					strAddrCode,									// �Z���R�[�h
					ConvInfoSrc(cKPPoint.GetInfoSrc()),				// ���񋟌��R�[�h
					ConvInputSeido(cKPPoint.GetInputSeido()),		// ���͗p���W���x�R�[�h
					ConvReleaseSeido(cKPPoint.GetReleaseSeido()),	// �����[�X�p���W���x�R�[�h
					lEqualF,										// �s���E�Ƃ̐�����
					cKPPoint.GetFloor(),							// �e�i���g�K��
					cKPPoint.GetKeisaiName(),						// �f�ږ���
					cKPPoint.GetKatagakiName(),						// ��������
					cKPPoint.GetBuildInfoSrc(),						// �r�����񋟌��R�[�h
					cKPPoint.GetBuildName(),						// �r������
					GetMeshCode(point.X, point.Y), point.X, point.Y	);				// ���b�V���R�[�h���W
	stream << CT2CA(strData) << "\n";

	stream.flush();
}
