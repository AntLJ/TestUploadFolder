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
#include "MakeAddrRec.h"
#include <algorithm>

// ��\�_�v���b�gNG�ȓs�s�w�i��ʈꗗ
const static long g_lNGSiteClass[] =
{
	city_site::bg_class::kRiver,                    //!< �͐�
	city_site::bg_class::kLake,                     //!< �r�E��
	city_site::bg_class::kPool,                     //!< �v�[��
	city_site::bg_class::kSea,                      //!< �C
	city_site::bg_class::kInterCityTollExpressway,  //!< �s�s�ԍ�����(�L��)
	city_site::bg_class::kUrbanTollExpressway,      //!< �s�s����(�L��)
	city_site::bg_class::kTollRoad,                 //!< �L�����H
	city_site::bg_class::kNationalHighway,          //!< ����
	city_site::bg_class::kPrincipalPrefecturalRoad, //!< ��v�n����
	city_site::bg_class::kPrefectureNormalRoad,     //!< ��ʓs���{����
	city_site::bg_class::kOtherArterialRoad,        //!< ���̑��������H
	city_site::bg_class::kSideway,                  //!< ����
	city_site::bg_class::kGardenPath,               //!< �뉀�H
	city_site::bg_class::kInterCityFreeExpressway,  //!< �s�s�ԍ�����(����)
	city_site::bg_class::kUrbanFreeExpressway,      //!< �s�s����(����)
	city_site::bg_class::kBreakwater,               //!< �h�g��
	city_site::bg_class::kIncompleteArea,           //!< �s���S�G���A
	city_site::bg_class::kPublicRoadStairs,         //!< �����K�i
	city_site::bg_class::kRunway,                   //!< �����H
	city_site::bg_class::kUnderRepairRoad,          //!< �H�������H
	city_site::bg_class::kTollGateLane,             //!< ���������[��
	-1                                              //!< ���[�v�I��蔻��p
};

// ������
bool CMakeAddrRec::Init( const IFeatureClassPtr& ipCityAdminFC, const IFeatureClassPtr& ipCitySiteFC, const IFeatureClassPtr& ipBuildingFC, const IFeatureClassPtr& ipBuildingRoofFC, const IFeatureClassPtr& ipCityMeshFC, LPCTSTR lpcszCodeConvDB, LPCTSTR lpcszJusyoDB )
{
	m_ipCityAdminFC = ipCityAdminFC;
	m_ipCitySiteFC = ipCitySiteFC;
	m_ipBuildingFC = ipBuildingFC;
	m_ipBuildingRoofFC = ipBuildingRoofFC;
	m_ipCityMeshFC = ipCityMeshFC;

	m_ipCitySiteFC->FindField( _bstr_t(city_site::kBgClass), &m_lBGClass_FIdx );
	m_ipCityMeshFC->FindField( _bstr_t(citymesh::kCreateYear), &m_lCreateYear_FIdx );

	// �R�[�h�ϊ�DB
	if( !m_cConvTable.OpenDB(lpcszCodeConvDB) )
		return false;

	// �ڑ��v���p�e�B�\���`�F�b�N
	CString str( lpcszJusyoDB );
	int nSlaPos = str.Find( '/' ), nAtPos = str.Find( '@' );
	if( nSlaPos == -1 || nAtPos == -1 )
		return progress_out::ProgressError<bool>( false, kError, "�Z���}�X�^�ւ̐ڑ��Ɏ��s : %s", lpcszJusyoDB );

	// �Z��DB����X�֔ԍ����X�g����
	int nPos = 0;
	CString strUser		= str.Tokenize( "/", nPos );
	CString strPass		= str.Tokenize( "@", nPos );
	CString strDBName	= str.Tokenize( "@", nPos );

	OStartup();
	ODatabase cDBSource;
	if( OSUCCESS == cDBSource.Open(strDBName, strUser, strPass) )
	{
		CString strSQL = _T("select ken_code || shi_code || oaza_code || aza_code, nvl(zip_code, '0000000') from areacode_master ")
						 _T("where oaza_code != '000' and haishi_date = '000000'");
		ODynaset cResult( cDBSource, strSQL );
		if( cResult.GetRecordCount() > 0 )
		{
			char szBuff1[12] = "", szBuff2[8] = "";
			do
			{
				cResult.GetFieldValue( 0, szBuff1, sizeof(szBuff1) );	cResult.GetFieldValue( 1, szBuff2, sizeof(szBuff2) );
				m_ZipMap.insert( std::pair<CString, CString>(szBuff1, szBuff2) );
				cResult.MoveNext();
			}while( !cResult.IsEOF() );
		}
	}
	else
		return progress_out::ProgressError<bool>( false, kError, "�Z���}�X�^�ւ̐ڑ��Ɏ��s : %s", lpcszJusyoDB );

	OShutdown();

	if( m_ZipMap.empty() )
		return progress_out::ProgressError<bool>( false, kError, "�X�֔ԍ����X�g�̍쐬�Ɏ��s : %s", lpcszJusyoDB );
	else
		return true;

}

// �s���E����n�ԑ�\�_����
void CMakeAddrRec::MakePntRecordByAdmin( const CAdminInfo& cAdmin, PNTMAP& rContainer )
{
	// ���ɍs���E�����\�_����������Ă���Ζ��Ȃ�
	CString strTmpCode = cAdmin.m_strAddrCode + m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku, false );
	if( rContainer.find(strTmpCode) == rContainer.end() )
	{
		// ����16���ƂȂ�s���E�����ɂ����݂��邩�m�F
		CString strWhere;
		strWhere.Format( _T("%s = '%s' and %s = '%s' and %s = '%s'"), 
						 city_admin::kCityCode, cAdmin.m_strAddrCode.Left(5), city_admin::kAddrCode, cAdmin.m_strAddrCode.Mid(5), city_admin::kExtGaikuFugo, cAdmin.m_strExtGaiku );
		IQueryFilterPtr ipQuery( CLSID_QueryFilter );
		ipQuery->put_WhereClause( CComBSTR(strWhere) );

		long lCount = 0;
		m_ipCityAdminFC->FeatureCount( ipQuery, &lCount );
		IGeometryPtr ipTargetGeo;
		if( lCount == 1 )		// �����̂��̂����̂܂܎g�p
		{
			ipTargetGeo = cAdmin.GetGeometry();
		}
		else if( lCount > 1 )	// ���� -> [�G���A�� > �ʐύL] �̗D��x�Ő�������
		{
			ITableSortPtr ipTableSort( CLSID_TableSort );
			ipTableSort->putref_Table( (ITablePtr)m_ipCityAdminFC );
			ipTableSort->put_Fields( CComBSTR(_T("AREACLASS_C, SHAPE.AREA")) );
			ipTableSort->put_Ascending( _bstr_t(_T("AREACLASS_C")), VARIANT_FALSE );	// �G���A���D��
			ipTableSort->put_Ascending( _bstr_t(_T("SHAPE.AREA")), VARIANT_TRUE );		// �ʐύL�����D��
			ipTableSort->putref_QueryFilter( ipQuery );
			ipTableSort->Sort( NULL );

			_ICursorPtr ipCursor;
			ipTableSort->get_Rows( &ipCursor );
			IFeaturePtr ipFeature;
			if( ipCursor && ((IFeatureCursorPtr)ipCursor)->NextFeature(&ipFeature) == S_OK )
				ipFeature->get_Shape( &ipTargetGeo );
		}

		if( ipTargetGeo )
		{
			// �s���E�����\�_����
			CPntRec cRepPnt;
			cRepPnt.m_strAddrCode = cAdmin.m_strAddrCode;
			cRepPnt.m_lRepDomariF = 1;	// ��\�_-�G���A �n�Ԏ~�܂�-OFF
			cRepPnt.m_strPntName = cAdmin.m_strExtGaiku;
			cRepPnt.m_strPntYomi = cAdmin.m_strExtGaikuYomi;
			cRepPnt.m_strZipCode = GetZipCode( cAdmin.m_strAddrCode );	// �Y��11���R�[�h����X�֔ԍ����擾

			WKSEnvelope wksEnv;
			ipTargetGeo->Envelope->QueryWKSCoords( &wksEnv );

			IPointPtr ipRepPoint( CLSID_Point ), ipKeepPoint;
			bool bSucceeded = false;
			int nDiv = 1;
			while( nDiv < 32 )
			{		
				double dXRange = (wksEnv.XMax - wksEnv.XMin) / (double)(nDiv * 2.0);
				double dYRange = (wksEnv.YMax - wksEnv.YMin) / (double)(nDiv * 2.0);
				for( int i = 0; i < nDiv; ++i )		// Y����
				{
					WKSPoint p;
					p.Y = (i == 0)? wksEnv.YMin + dYRange : wksEnv.YMin + (dYRange * (i * 2 + 1));
					for( int j = 0; j < nDiv; ++j )	// X����
					{
						p.X = (j == 0)? wksEnv.XMin + dXRange : wksEnv.XMin + (dXRange * (j * 2 + 1));
						ipRepPoint->PutCoords( p.X, p.Y );

						// �w�i�`�F�b�N
						if( !IsNGSite(ipRepPoint) )
						{
							cRepPnt.m_strAddrCode += m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku );
							cRepPnt.SetCoord( ipRepPoint );
							rContainer.insert( std::pair<CString, CPntRec>(cRepPnt.m_strAddrCode, cRepPnt) );
							return;
						}
						else
						{
							// �ŏ��̋�`�����_�́A�w�i��NG�̏ꏊ�ł�����ȊO�őÓ��Ȃ��̂��Ȃ�������̗p����̂ŕێ�
							if( nDiv == 1 )
								ipKeepPoint = ipRepPoint;
						}
					}
				}
				nDiv *= 2;	// ��`���������₷
			}
			// �Ó��Ȃ��̂Ȃ�������A�ŏ��̋�`�����_���\�_�ɍ̗p
			if( ipKeepPoint )
			{
				cRepPnt.m_strAddrCode += m_cConvTable.GetPntCode( cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku );
				cRepPnt.SetCoord( ipKeepPoint );
				rContainer.insert( std::pair<CString, CPntRec>(cRepPnt.m_strAddrCode, cRepPnt) );
			}
		}
		else
			progress_out::ProgressError<int>( 0, kFatal, "�X�敄������n�ԑ�\�_�����ł��܂��� : %d", cAdmin.GetOID() );
	}
}

// �����R�[�h����n�ԑ�\�_����
void CMakeAddrRec::MakePntRecordByGou( const GOUMAP& rGouRec, PNTMAP& rContainer )
{
	CPntRec cPntRec;	// �ꎞ�ϐ�
	GOULIST listGou, listTpg;	// GOU�|�C���g, TP�|�C���g
	CString strBaseCode;	// �����Ώ�16���R�[�h
	for( GOUMAP::const_iterator itrGou = rGouRec.begin(); itrGou != rGouRec.end(); ++itrGou )
	{
		// ���ɒn�ԑ�\�_��������Ă��� or �����̒n�Ԃ̂Ƃ��́A����
		if( !addr_func::IsNumStr(itrGou->first.Mid(11, 5)) && rContainer.find(itrGou->first.Left(16)) == rContainer.end() )
		{
			if( strBaseCode != itrGou->first.Left(16) )
			{
				if( !strBaseCode.IsEmpty() )
				{
					// �Z���R�[�h�؂�ւ��̃^�C�~���O�Ȃ̂ŁA�n�ԑ�\�_����
					CGouRec cGouRec;
					long lRepDomariF = SelectPntFromGouList( listGou, listTpg, cGouRec );
					rContainer.insert( std::pair<CString, CPntRec>(strBaseCode, CPntRec(cGouRec, lRepDomariF)) );	// �����R�[�h����n�ԃ��R�[�h�ɕϊ����ēo�^
				}

				// ���Z�b�g
				strBaseCode = itrGou->first.Left(16);
				listGou.clear(); listTpg.clear();
			}

			// GOU or TPG�ŕ�����B
			if( 1 == itrGou->second.m_lGouType )	listGou.push_back( itrGou->second );
			else									listTpg.push_back( itrGou->second );
		}
	}

	// �Ō�Ɏc�������R�[�h������n�ԑ�\�_����
	if( !listGou.empty() || !listTpg.empty() )
	{
		CGouRec cGouRec;
		long lRepDomariF = SelectPntFromGouList( listGou, listTpg, cGouRec );
		rContainer.insert( std::pair<CString, CPntRec>(strBaseCode, CPntRec(cGouRec, lRepDomariF)) );
	}
}

// �����R�[�h�쐬
void CMakeAddrRec::MakeGouRecordByDB( const CAdminInfo& cAdmin, const CGouInfo& cGou, GOUMAP& rContainer )
{
	// ���ԍ����� or ���~����(���~�擾���s) or [*](���Ȃ��Z��) �Ȃ疳��
	if( cGou.GetGouNo() != _T("") && cGou.GetGouNoYomi() != _T("") && cGou.GetGouNo() != _T("*") )
	{
		// �����t���Z���H
		const EADDR_STATUS emAddrStatus = IsCharAddr( cAdmin, cGou );
		if( emAddrStatus != kNormal )
		{
			CGouRec cRec;
			cRec.m_lOID = cGou.GetOID();
			cRec.m_lGouType = cGou.m_lGouType;
			cRec.m_strAddrCode = cAdmin.m_strAddrCode;
			cRec.m_bPriorF = cGou.m_bPriorF;												// �D��
			cRec.m_bInArea = (cAdmin.m_lAreaClass_C == city_admin::area_class::kInArea);	// �G���A���O

			// 20���R�[�h���J�n
			if( !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty() )	// �g���X�敄������(�A���g�����ԍ��łȂ��Ƃ�)
			{
				// �n�� : �g���X�敄��
				// ��   : ���ԍ�
				cRec.m_strAddrCode.AppendFormat( _T("%05s%04s"), 
							 					 m_cConvTable.GetPntCode(cAdmin.m_strAddrCode, cAdmin.m_strExtGaiku),
												 (emAddrStatus == kExtGaiku)? cGou.GetGouNo() : m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, cGou.GetGouNo()) );

				cRec.m_strPntName = cAdmin.m_strExtGaiku;
				cRec.m_strPntYomi = cAdmin.m_strExtGaikuYomi;
				cRec.m_strGouName = cGou.GetGouNo();
				cRec.m_strGouYomi = cGou.GetGouNoYomi();
			}
			else
			{
				if( !cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 )	// �X�敄������
				{
					// �n�� : �X�敄��// ��   : ���ԍ�
					cRec.m_strAddrCode.AppendFormat( _T("%05d%04s"), cAdmin.m_lGaikuFugo, m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, cGou.GetGouNo()) );
					CString strGaikuFugo;
					strGaikuFugo.Format( _T("%d"), cAdmin.m_lGaikuFugo );
					bool bRet = false;
					cRec.m_strPntName = strGaikuFugo;
					cRec.m_strPntYomi = strGaikuFugo;
					cRec.m_strGouName = cGou.GetGouNo();
					cRec.m_strGouYomi = cGou.GetGouNoYomi();
				}
				else												// �X�敄���Ȃ�
				{
					// �n�� : ���ԍ�(��؂肠��Ƃ��́A�ŏ��̋�؂�܂�)
					// ��   : ���ԍ�(��؂肠��Ƃ��́A�ŏ��̋�؂�ȍ~)
					CString strPntName = cGou.GetTokenCount() == 1? cGou.GetGouNo() : cGou.GetGouNo(0, 1);
					CString strGouName = cGou.GetTokenCount() == 1? _T("") : cGou.GetGouNo(1);
					
					cRec.m_strAddrCode.AppendFormat( _T("%05s%04s"), 
												addr_func::IsNormalPnt(strPntName)? strPntName : m_cConvTable.GetPntCode(cAdmin.m_strAddrCode, strPntName), 
												(addr_func::IsNormalGou(strGouName) || strGouName.IsEmpty())? strGouName : m_cConvTable.GetGouCode(cAdmin.m_strAddrCode, strGouName) );

					cRec.m_strPntName = strPntName;
					cRec.m_strPntYomi = cGou.GetTokenCount() == 1? cGou.GetGouNoYomi() : cGou.GetGouNoYomi(0, 1);
					cRec.m_strGouName = strGouName;
					cRec.m_strGouYomi = cGou.GetTokenCount() == 1? _T("") : cGou.GetGouNoYomi(1);
				}		
			}

			cRec.SetCoord( cGou.GetGeometry() );
			cRec.m_dKakeiArea = GetContainsBldArea( cGou.GetGeometry() );	// ���|�C���g���ƌ`���ɂ��邩�����ׂ�
			cRec.m_strZipCode = GetZipCode( cAdmin.m_strAddrCode );			// �Y��11���R�[�h����X�֔ԍ����擾
			rContainer.insert( std::pair<CString, CGouRec>(cRec.m_strAddrCode, cRec) );		// ���R�[�h�ǉ�

			// �ʏ̍s���E�̂Ƃ��AADDRCODE2�ł�����쐬
			if( cAdmin.m_strOazaAzaCode2 != _T("000000") )
			{
				// [Bug7068]11���R�[�h�ōēx�R�[�h���蓖�Ă��s���B
				CString strTmpAdminCode = cRec.m_strAddrCode.Left(5) + cAdmin.m_strOazaAzaCode2;

				cRec.m_strAddrCode.Format( _T("%s%05s%04s"), 
										   strTmpAdminCode, 
										   addr_func::IsNormalPnt(cRec.m_strPntName)? cRec.m_strPntName : m_cConvTable.GetPntCode(strTmpAdminCode, cRec.m_strPntName), 
										   (addr_func::IsNormalGou(cRec.m_strGouName) || cRec.m_strGouName.IsEmpty())? cRec.m_strGouName : m_cConvTable.GetGouCode(strTmpAdminCode, cRec.m_strGouName) );
				cRec.m_bTsushoConv = true;
				rContainer.insert( std::pair<CString, CGouRec>(cRec.m_strAddrCode, cRec) );	// ���R�[�h�ǉ�
			}
		}
	}
}

// �����R�[�h�ꗗ�����\�Z���Ƀt�B���^�����O
void CMakeAddrRec::SelectRepGouRecord( const GOUMAP& rGouMap, GOUMAP& rContainer )
{
	typedef GOUMAP::const_iterator GOUMAP_ITR;
	for( GOUMAP_ITR itr = rGouMap.begin(); itr != rGouMap.end(); ++itr )
	{
		// ����20���R�[�h�������邩�m�F
		if( rContainer.find(itr->first) == rContainer.end() )
		{
			if( rGouMap.count(itr->first) == 1 )
			{
				rContainer.insert( std::pair< CString, CGouRec>(itr->first, itr->second) );	// 1���Ȃ炻�ꂪ��\�Z��
			}
			else
			{
				GOULIST listPrior, listGou;
				std::pair< GOUMAP_ITR, GOUMAP_ITR> itrPair = rGouMap.equal_range( itr->first );
				for( ; itrPair.first != itrPair.second; ++itrPair.first )
				{
					// �D�捆(�ʏ̍s���E�ł̐����͗D��Ƃ��Ȃ�)�ƗD��łȂ����ŏꍇ�킯
					if( itrPair.first->second.m_bPriorF && !itrPair.first->second.m_bTsushoConv )
						listPrior.push_back( itrPair.first->second );
					else
						listGou.push_back( itrPair.first->second );	
				}

				// �D�捆�̐��ɂ���ď���������
				switch( listPrior.size() )
				{
				case 1:	// �D��t���O��Ȃ炻�ꂪ��\�Z��
					rContainer.insert( std::pair<CString, CGouRec>(itr->first, *(listPrior.begin())) );
					break;
				case 0:	// �\�[�g��A��Ԑ擪�ɗ������̂���\�Z��
					std::sort( listGou.begin(), listGou.end(), addr_func::CompareGouRec );
					rContainer.insert( std::pair<CString, CGouRec>(itr->first, *(listGou.begin())) );
					break;
				default:	// �D�捆�����̂Ƃ��́A�G���[
					for( GOULIST::const_iterator itr = listPrior.begin(); itr != listPrior.end(); ++itr )
					{
						object_out::ObjectError( gou_point::kTableName, itr->m_lOID, _T("����20���ŗD�捆������\t%s"), itr->m_strAddrCode );
					}
					break;
				}

				// ���C���C�e���[�^������20�������X�L�b�v(�����������ēx���Ȃ��Ă悢�̂�)
				std::advance( itr, listPrior.size() + listGou.size() -1 );
			}
		}
	}
}

//////////////////////////////////////////////
//	�����֐�
//////////////////////////////////////////////

// �����t���Z���H
EADDR_STATUS CMakeAddrRec::IsCharAddr( const CAdminInfo& cAdmin, const CGouInfo& cGou )
{
	// ���ԍ��̐擪[#]�Ȃ當���t��
	if( cGou.m_bMoji )
		return !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty()? kMojiGou_ExtGaiku : kMojiGou;

	if( !cGou.m_bExGouF && !cAdmin.m_strExtGaiku.IsEmpty() )	// �g���X�敄������
	{
		// ��؂肪2�ȏ�Ȃ當���t����
		if( cGou.GetTokenCount() > 1 )
			return kSepOver_ExtGaiku;

		// ����10,000�ȏ�Ȃ當���t����
		return (_tstol(cGou.GetGouNo(0, 1)) > 9999)? kNoOver_ExtGaiku : kExtGaiku;
	}
	else														// �g���X�敄���Ȃ�
	{
		// �g�����łȂ�+�X�敄������ŋ�؂肪2�ȏ�Ȃ當���t���Ƃ��Ĉ���
		// �g�����ŋ�؂肪2�ȏ�Ȃ當���t���Ƃ��Ĉ���(Bug7287)
		if( (!cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 && cGou.GetTokenCount() > 1) || (cGou.m_bExGouF && cGou.GetTokenCount() > 2) )
			return kSepOver;

		// �X�敄���Ȃ��ŋ�؂肪3�ȏ�Ȃ當���t���Ƃ��Ĉ���
		if( cAdmin.m_lGaikuFugo == 0 && cGou.GetTokenCount() > 2 )
			return kSepOver;

		// �Ō�ɒn�Ԃ�100,000�ȏ� ����10,000�ȏ�Ȃ當���t���Ƃ��Ĉ���
		if( !cGou.m_bExGouF && cAdmin.m_lGaikuFugo > 0 )	// �X�敄������
		{
			return (_tstol(cGou.GetGouNo(0, 1)) > 9999)? kNoOver : kNormal;
		}
		else												// �X�敄���Ȃ�
		{
			return (_tstol(cGou.GetGouNo(0, 1)) > 99999) 
					|| (cGou.GetTokenCount() > 1 && _tstol(cGou.GetGouNo(1, 2)) > 9999) ?
					kNoOver : kNormal;
		}
	}
}

// �v���b�gNG�Ȕw�i�H
bool CMakeAddrRec::IsNGSite( IPoint* ipPoint )
{
	CComVariant vaValue;
	IFeaturePtr ipFeature;
	long lSiteCount = 0;
	IFeatureCursorPtr ipSiteCursor = gf::SelectByShape( ipPoint, m_ipCitySiteFC, &lSiteCount, esriSpatialRelWithin, NULL, city_site::kBgClass );
	if( lSiteCount > 0 && ipSiteCursor )
	{
		while( ipSiteCursor->NextFeature(&ipFeature) == S_OK )
		{
			ipFeature->get_Value( m_lBGClass_FIdx, &vaValue );
			for( int  i = 0; g_lNGSiteClass[i] != -1; ++i )
			{
				if( g_lNGSiteClass[i] == vaValue.lVal )
					return true;
			}
		}
		return false;
	}
	else
	{
		// �s�s�n�}�G���A���Ȃ�׊X�H�Ȃ̂ŁANG
		long lMeshCount = 0;
		IFeatureCursorPtr ipMeshCursor = gf::SelectByShape( ipPoint, m_ipCityMeshFC, &lMeshCount, esriSpatialRelWithin, NULL, citymesh::kCreateYear );
		if( lMeshCount > 0 && ipMeshCursor )
		{
			while( ipMeshCursor->NextFeature(&ipFeature) == S_OK )
			{
				ipFeature->get_Value( m_lCreateYear_FIdx, &vaValue ); 
				if( vaValue.vt != VT_NULL )
					return true;	// �׊X�H�ɂȂ�̂�NG
			}
		}
		return false;
	}
}

// �ƌ`�|���S�����牮��|���S���̖ʐώ擾
double CMakeAddrRec::GetRoofArea( const IGeometryPtr& ipBldGeometry )
{	
	double roofArea = 0;
	IFeatureCursorPtr ipFeaturecursor = gf::SelectByShape( ipBldGeometry, m_ipBuildingRoofFC );
	if( ipFeaturecursor )
	{
		IFeaturePtr ipFeature;
		while( ipFeaturecursor->NextFeature(&ipFeature) == S_OK && ipFeature )
		{
			IGeometryPtr ipGeo;
			ipFeature->get_Shape( &ipGeo );
			
			double tmpArea = 0;
			((IAreaPtr)ipGeo)->get_Area( &tmpArea );
			roofArea += tmpArea;
		}
	}

	return roofArea;
}

// ���|�C���g�����݂���ƌ`�̖ʐώ擾
double CMakeAddrRec::GetContainsBldArea( const IPointPtr& ipPoint )
{
	if( ipPoint )
	{
		IFeatureCursorPtr ipFeatureCursor = gf::SelectByShape( ipPoint, m_ipBuildingFC, NULL, esriSpatialRelWithin );
		if( ipFeatureCursor )
		{
			double dTmpArea = 0.0, dMaxArea = 0.0, dRoofArea = 0.0;
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature )
			{
				IGeometryPtr ipGeo;
				ipFeature->get_Shape( &ipGeo );
				
				((IAreaPtr)ipGeo)->get_Area( &dTmpArea );
				// ����Building_Roof������΂��̕��̖ʐς͈����Ȃ��Ă͂����Ȃ�
				dRoofArea = GetRoofArea( ipGeo );
				dTmpArea -= dRoofArea;
				if( dMaxArea < dTmpArea )
					dMaxArea = dTmpArea;
			}
			return dMaxArea;
		}
	}
	return 0.0;
}

// �����R�[�h����n�ԑ�\�_�ƂȂ肤�鍆�|�C���g�I��
long CMakeAddrRec::SelectPntFromGouList( const GOULIST& listGou, const GOULIST& listTpg, CGouRec& rGouRec, bool bOazaIgnore /* = true */ )
{
	const GOULIST& listTarget = (!listGou.empty())? listGou : listTpg;
	GOULIST listPriorChiban, listChiban, listNotChiban, listOazaMode;
	
	// �n�Ԏ~�܂�t���O�|�C���g���擾
	for( GOULIST::const_iterator itr = listTarget.begin(); itr != listTarget.end(); ++itr )
	{
		// [Bug7068] �ʏ̍s���E�ɂ�鐶���͒n�ԑ�\�_�����Ɋ�{�I�Ɏg�p���Ȃ�
		if( ! itr->m_bTsushoConv || ! bOazaIgnore )
		{
			if( itr->m_strAddrCode.Right(4) == _T("0000") )
			{
				// �n�Ԏ~�܂��������ɗD�捆�ŏꍇ�킯
				if( itr->m_bPriorF )	listPriorChiban.push_back( *itr );
				else					listChiban.push_back( *itr );
			}
			else
				listNotChiban.push_back( *itr );
		}
		else
			listOazaMode.push_back( *itr );
	}

	long lChibanCount    = (signed)listChiban.size() + (signed)listPriorChiban.size(); // �n�Ԏ~�܂茏��
	long lNotChibanCount = (signed)listNotChiban.size();                               // �n�Ԏ~�܂�łȂ�����
	long lOazaModeCount  = (signed)listOazaMode.size();                                // �厚���[�h����
	long lTpgCount       = 0;                                                          // TPG�R���̌���(���R�������݂���ꍇ�̂ݎg�p)

	// ���R����TPG�R�������݂���ہA���R���݂̂Ńt���O������s���Ă��邽�ߕs�����ƂȂ��Ă���������̂��߁ATPG�R���̌������擾
	if( !listGou.empty() )
		lTpgCount = (signed)listTpg.size();

	if( lChibanCount == 1 )	// �n�Ԏ~�܂�1�_
	{
		// �Y���|�C���g��I��
		rGouRec = !listChiban.empty()? *(listChiban.begin()) : *(listPriorChiban.begin());

		// �n�ԃt���O�ݒ莞�́AlistOazaMode��TPG�R���̌������l�����Ȃ��Ƃ��������Ȃ邽�߁A���̂悤�ɏC�� 2013/01/23
		return (lNotChibanCount > 0 || lOazaModeCount > 0 || lTpgCount > 0 )? 4 : 2;	// ����16���ŕ�������[��\�_-���� �n�Ԏ~�܂�-ON]�E����16���ŕ����Ȃ�[��\�_-OFF  �n�Ԏ~�܂�-ON]
	}
	else if( lChibanCount > 1 )	// �n�Ԏ~�܂蕡��
	{
		if( !listPriorChiban.empty() )	// �D�悠��Ȃ炻���I��(��\�_-���� �n�Ԏ~�܂�-ON)
		{
			rGouRec = *(listPriorChiban.begin());
			return 4;

		}
		else	// �D��Ȃ��Ȃ�n�Ԏ~�܂�|�C���g�S�E�n�Ԏ~�܂�łȂ��|�C���g�S(��)�����\�_�����肷��( ��\�_-���� �n�Ԏ~�܂�-ON )
		{
			if( lNotChibanCount == 0 )
			{
				if( lChibanCount == 2 )	// �n�Ԏ~ 2�_ �n�Ԏ~�łȂ� 0�_
				{
					rGouRec = addr_func::CompareGouRecPos(*(listChiban.begin()), *(++listChiban.begin())) <= 0? *(listChiban.begin()) : *(++listChiban.begin());
					return 4;
				}
				else					// �n�Ԏ~ 3�_�ȏ� �n�Ԏ~�łȂ� 0�_
				{
					SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// ��������Z�o
					return 4;
				}
			}
			else
			{
				// �n�Ԏ~ ���� �n�Ԏ~�łȂ� 1�_�ȏ�
				SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// ��������Z�o
				return 4;
			}
		}
	}
	else	// �n�Ԏ~�܂�|�C���g�Ȃ�
	{
		if( lNotChibanCount == 1 )	// �n�Ԏ~�łȂ� 1�_
		{
			// ���Y�|�C���g�I��
			rGouRec = *(listNotChiban.begin());

			// �n�ԃt���O�ݒ莞�́AlistOazaMode��TPG�R���̌������l�����Ȃ��Ƃ��������Ȃ邽�߁A���̂悤�ɏC�� 2013/01/23
			return (lOazaModeCount > 0 || lTpgCount > 0)? 5 : 3;	// ����16���ŕ�������[��\�_-���� �n�Ԏ~�܂�-OFF]�E����16���ŕ����Ȃ�[��\�_-OFF  �n�Ԏ~�܂�-OFF]
		}
		else if( lNotChibanCount == 2 )		// �n�Ԏ~�łȂ� 2�_
		{
			// �n�Ԏ~�܂�łȂ��|�C���g�S(��)�����\�_�����肷��( ��\�_-���� �n�Ԏ~�܂�-OFF )
			rGouRec = addr_func::CompareGouRecPos( *(listNotChiban.begin()), *(++listNotChiban.begin())) <= 0? *(listNotChiban.begin()) : *(++listNotChiban.begin());
			return 5;
		}
		else if( lNotChibanCount > 2 )		// �n�Ԏ~�łȂ� 3�_�ȏ�
		{
			// �n�Ԏ~�܂�łȂ��|�C���g�S(��)�����\�_�����肷��( ��\�_-���� �n�Ԏ~�܂�-OFF )
			SelectPntFromGouDist( listChiban, listNotChiban, rGouRec );	// ��������Z�o
			return 5;
		}
		else	// �n�Ԏ~�łȂ� 0�_
		{
			// �厚�������ɂ��|�C���g�S(��)����n�ԑ�\�_���߂�
			return SelectPntFromGouList( listOazaMode, listTpg, rGouRec, false );
		}
	}
}

// �����R�[�h�̈ʒu�֌W����n�ԑ�\�_�ƂȂ肤�鍆�|�C���g�I��
void CMakeAddrRec::SelectPntFromGouDist( const GOULIST& listChiban, const GOULIST& listNotChiban, CGouRec& rGouRec )
{
	// �n�Ԏ~�܂肠��΂����D��
	const GOULIST& listTarget = !listChiban.empty()? listChiban : listNotChiban;

	// �P�_�����Ɋe�_�ԋ����̑��a����ԏ��������̂���蒆���ɂ���Ƃ��đ�\�_�ɂ���
	double dMinDist = -1.0;
	for( GOULIST::const_iterator itrTarget = listTarget.begin(); itrTarget != listTarget.end(); ++itrTarget )
	{
		double dSumDist = 0.0;
		// �n�Ԏ~�܂�|�C���g�S
		for( GOULIST::const_iterator itrChiban = listChiban.begin(); itrChiban != listChiban.end(); ++itrChiban )
		{
			if( itrTarget->m_lOID != itrChiban->m_lOID )	dSumDist += addr_func::GetDistGouRec( *itrTarget, *itrChiban );
		}

		// �n�Ԏ~�܂�łȂ��|�C���g�S
		for( GOULIST::const_iterator itrChiban = listNotChiban.begin(); itrChiban != listNotChiban.end(); ++itrChiban )
		{
			if( itrTarget->m_lOID != itrChiban->m_lOID )	dSumDist += addr_func::GetDistGouRec( *itrTarget, *itrChiban );
		}

		if( dMinDist == -1.0 || dMinDist >= dSumDist )
		{
			// �����������Ƃ��́A��蓌�E���k��D��
			if( dMinDist != dSumDist || (dMinDist == dSumDist && addr_func::CompareGouRecPos(rGouRec, *itrTarget) == 1) )
			{				
				dMinDist = dSumDist;
				rGouRec = *itrTarget;
			}
		}
	}
}
