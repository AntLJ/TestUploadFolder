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

// CheckCityRep.cpp: CheckCityRep �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckCityRep.h"
#include <WinLib/VersionInfo.h>
#include "GlobalFunc.h"

double	gXFactor	= 0.125 / 1280.0;
double	gYFactor	= 5.0 / 60.0 / 960.0;

// �g�p�@
void CheckCityRep::Usage()
{
	cerr << "[�n�Ԓ��L�`�F�b�N�c�[���g�p�@]\n"
		 << "(Usage)CheckCityRep.exe <�e��I�v�V����>\n"
		 << "���I�v�V�����ɂ���\n"
		 << " ���`�F�b�N���[�h�I�v�V����(���ꂩ�K�{�w��)\n"
		 << "   -a                        ... �S���`�F�b�N\n"
		 << "   -l <11���Z���R�[�h���X�g> ... ���X�g�w��`�F�b�N\n" 
		 << "   -r <�J�n�s�撬���R�[�h>-<�I���s�撬���R�[�h> ... �R�[�h�͈͎w��`�F�b�N\n" 
		 << " ���n�Ԓ��L�����͈͎w��I�v�V����(10�i�x�P��)\n"
		 << "   -s <�����͈�> ... �w��͈͂̔��a�����~��������(�f�t�H���g�́A0.002[��200m])\n"
		 << " �����̑��I�v�V����\n"
		 << "   -o <���O�t�@�C��> ... ���O�t�@�C���w��(�K�{)\n"
		 << "�����ϐ��ɂ���(�ǂ���K�{�w��)\n"
		 << " DB_ADDRESS   ... �Z���nSDE�ڑ��v���p�e�B\n"
		 << " DB_MAP       ... �n�}�nSDE�ڑ��v���p�e�B\n"
		 << " FC_CITY_REP_POINT ... �n�ԑ�\�_�t�B�[�`���N���X��\n" 
		 << " FC_CITY_ADMIN     ... �s�s�n�}�s���E�t�B�[�`���N���X\n"
		 << " FC_GOU_POINT      ... ���|�C���g�t�B�[�`���N���X\n"
		 << " FC_CITY_SITE      ... �s�s�n�}�w�i�t�B�[�`���N���X" << endl;
}

// ������
bool CheckCityRep::Init( int argc, char* argv[] )
{
	if( argc < 3 )
	{
		cerr << "#Error ����������܂���" << endl;
		return false;
	}

	for( int i = 1; i < argc && argv[i][0] == '-'; i++ )
	{
		switch(argv[i][1])
		{
		case 'a':
			m_eCheckMode = kCheckAll;
			break;
		case 'l':
			m_eCheckMode = kCheckByList;
			if( i+1 < argc )
				m_strAddrList = argv[++i];	// �Z���R�[�h���X�g�ǂݍ���
			else
			{
				cerr << "#Error -l�I�v�V�������̈���������܂���" << endl;
				return false;
			}
			break;
		case 'r':
			m_eCheckMode = kCheckByRange;
			if( i+1 < argc )
			{
				CString strRange(argv[++i]);
				if( strRange.GetLength() != 11 || strRange[5] != '-' )
				{
					cerr << "#Error -r�I�v�V�������̈������s�� : " << strRange << endl;
					return false;
				}
				m_strRangeMin = strRange.Mid(0, 5);
				m_strRangeMax = strRange.Mid(6, 5);
			}
			else
			{
				cerr << "#Error -r�I�v�V�������̈���������܂���" << endl;
				return false;
			}
			break;
		case 's':
			if( i+1 < argc )
			{
				m_dSearchRange = _tstof(argv[++i]);
				if( m_dSearchRange <= 0.0 )
				{
					cerr << "#Error �n�Ԓ��L�����͈͂́A0.0���傫���l���w�肵�ĉ����� : " << argv[i] << endl;
					return false;
				}
			}
			else
			{
				cerr << "#Error -s�I�v�V�������̈���������܂���" << endl;
				return false;
			}
			break;
		case 'o':
			if( i+1 < argc )
			{
				m_ofs.open( static_cast<CT2CW>(argv[++i]) );
				if( !m_ofs )
				{
					cerr << "#Error ���O�t�@�C���̃I�[�v���Ɏ��s : " << argv[i] << endl;
					return false;
				}
			}
			else
			{
				cerr << "#Error -o�I�v�V�������̈���������܂���" << endl;
				return false;
			}
			break;
		default:
			cerr << "#Error �s���ȃI�v�V�����ł� : " << argv[i] << endl;
			return false;
		}
	}

	if( !m_ofs.is_open() )
	{
		cerr << "#Error ���O�t�@�C��(-o �I�v�V����)���w�肳��Ă��܂���" << endl;
		return false;
	}

	if( m_eCheckMode == kCheckNone )
	{
		cerr << "#Errro �`�F�b�N���[�h���w�肳��Ă��Ȃ�" << endl;
		return false;
	}

	// ���ϐ�����DB���擾
#ifdef _DEBUG
	_tputenv( _T("DB_ADDRESS=SiNDYTEST/SiNDYTEST/CheckCityRep/5151/coral2") );
	_tputenv( _T("DB_MAP=SiNDYTEST/SiNDYTEST/CheckCityRep/5151/coral2") );
	_tputenv( _T("FC_CITY_REP_POINT=City_Rep_Point") );
	_tputenv( _T("FC_CITY_ADMIN=City_Admin") );
	_tputenv( _T("FC_GOU_POINT=Gou_Point") );
	_tputenv( _T("FC_CITY_SITE=City_Site") );
#endif

	CString strDBAddress, strDBMap;
	if( !strDBAddress.GetEnvironmentVariable(_T("DB_ADDRESS")) )
	{
		cerr << "#Error ���ϐ� DB_ADDRESS �����ݒ�" << endl;
		return false;
	}
	if( !strDBMap.GetEnvironmentVariable(_T("DB_MAP")) )
	{
		cerr << "#Error ���ϐ� DB_MAP �����ݒ�" << endl;
		return false;
	}

	if( NULL == (m_ipWorkspaceAddress = gf::GetWorkspace(strDBAddress)) )
	{
		cerr << "#Error �Z���nSDE�ւ̐ڑ��Ɏ��s : " << strDBAddress << endl;
		return false;
	}
	cout << "#�Z���nSDE�ւ̐ڑ� : " << strDBAddress << endl;

	if( NULL == (m_ipWorkspaceMap = gf::GetWorkspace(strDBMap)) )
	{
		cerr << "#Error �n�}�nSDE�ւ̐ڑ��Ɏ��s : " << strDBMap << endl;
		return false;
	}
	cout << "#�n�}�nSDE�ւ̐ڑ� : " << strDBMap << endl;

	// �t�B�[�`���N���X�擾
	if( !gf::GetFCFromEnv(_T("FC_CITY_REP_POINT"), m_ipWorkspaceAddress, m_ipFC_CityRepPoint)
		|| !gf::GetFCFromEnv(_T("FC_CITY_ADMIN"), m_ipWorkspaceAddress, m_ipFC_CityAdmin) 
		|| !gf::GetFCFromEnv(_T("FC_GOU_POINT"), m_ipWorkspaceAddress, m_ipFC_GouPoint)
		|| !gf::GetFCFromEnv(_T("FC_CITY_SITE"), m_ipWorkspaceMap, m_ipFC_CitySite) )
		return false;

	// �t�B�[���h�C���f�b�N�X�擾
	if( FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kAnno), &mFID_AnnoF))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kPointClass), &mFID_PointClassC))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kAddrCode), &mFID_AddrCode))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kRepType), &mFID_RepTypeC))
		|| FAILED(m_ipFC_CityRepPoint->FindField(_bstr_t(city_rep_point::kChibanNo), &mFID_ChibanNo))
		|| FAILED(m_ipFC_GouPoint->FindField(_bstr_t(gou_point::kGouType), &mFID_GouType))
		|| FAILED(m_ipFC_CitySite->FindField(_bstr_t(city_site::kBgClass), &mFID_BgClassC)) )
	{
		cerr << "#Error �t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s" << endl;
		return false;
	}

	// ���O�w�b�_�[�o��
	CVersion cVer;
	CString strHeader;
	strHeader.Format( _T("#SINDYSTDLOG\n")
					  _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
					  _T("#DB_ADDRESS: %s\n")
					  _T("#DB_MAP: %s"),
					  cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion(), strDBAddress, strDBMap );
	m_ofs << strHeader << endl;

	return true;
}

// ���s
bool CheckCityRep::Execute()
{
	m_ofs << "#�J�n����: " << gf::GetCurrentTime() << endl;

	// �Z���R�[�h���X�g�쐬
	if( !fnMakeACList() )
		return false;

	((IGeoDatasetPtr)m_ipFC_CityRepPoint)->get_SpatialReference(&m_ipSpRef);

	// �Z���R�[�h�i11���j�̃��[�v	
	for( AddrCodeList::iterator	it = mACList.begin(); it != mACList.end(); ++it )
	{
		// �W���P�ʂŋ�ԃL���b�V�����쐬���Ă������AISpatialCache�E�p�̂��߁A��ԃL���b�V�������Ŏ��s����悤�ύX�iBug9843�j
		cout << "#" << *it << " �`�F�b�N��..." << endl;

		IGeometryPtr ipG_CityAdmin( fnUniteCityAdmin(*it) );
		if( ! ipG_CityAdmin )
			continue;

		IFeatureCursorPtr ipFCur_CityRepPoint( gf::SelectByShape(ipG_CityAdmin, m_ipFC_CityRepPoint, NULL, esriSpatialRelContains) );
		if( ! ipFCur_CityRepPoint )
			continue;

		// CityRepPoint �̃��[�v�i�^�[�Q�b�g�j
		IFeaturePtr	ipF_CityRepPoint;
		while(ipFCur_CityRepPoint->NextFeature(&ipF_CityRepPoint) == S_OK)
		{
			CComVariant	aVarTmp;
			
			// �n�ԑ�\�_�ȊO�͖���
			ipF_CityRepPoint->get_Value(mFID_PointClassC, &aVarTmp);
			if(aVarTmp.lVal != 0) continue;

			// �n�Ԓ��L�t���O�������Ă�����͖̂���
			ipF_CityRepPoint->get_Value(mFID_AnnoF, &aVarTmp);
			if(aVarTmp.lVal == 1) continue;

			// �Z���R�[�h�擾
			ipF_CityRepPoint->get_Value(mFID_AddrCode, &aVarTmp);
			CString strAddrCode = (aVarTmp.vt == VT_BSTR)? CString(aVarTmp.bstrVal) : NULL;
			aVarTmp.Clear();

			IGeometryPtr	ipG_CityRepPoint, ipG_Buffer;
			ipF_CityRepPoint->get_Shape(&ipG_CityRepPoint);
			((ITopologicalOperatorPtr)ipG_CityRepPoint)->Buffer(m_dSearchRange, &ipG_Buffer);

			// CityRepPoint �̃��[�v�i���Ӂj �c �n�Ԓ��L�t���O�̗����Ă���|�C���g�������OK
			bool aAnnoExist = false;
			IFeatureCursorPtr ipFCur_CityRepPoint2( gf::SelectByShape(ipG_Buffer, m_ipFC_CityRepPoint, NULL, esriSpatialRelIntersects) );
			IFeaturePtr	ipF_CityRepPoint2;
			while( ipFCur_CityRepPoint2->NextFeature(&ipF_CityRepPoint2) == S_OK )
			{
				ipF_CityRepPoint2->get_Value(mFID_AnnoF, &aVarTmp);
				if(aVarTmp.lVal == 1) {
					aAnnoExist = true;
					break;
				}
			}

			// ���ӂɒn�Ԓ��L�Ȃ��Ƃ�
			if( !aAnnoExist )
			{
				// ��\�_�E�n�Ԏ~�܂�t���O���u�G���A�v�̏ꍇ		�� �G���[
				// ��\�_�E�n�Ԏ~�܂�t���O���u�G���A�v�łȂ��ꍇ	�� ���|�C���g�iGOU�j�R���Ȃ�G���[
				ipF_CityRepPoint->get_Value(mFID_RepTypeC, &aVarTmp);
				if( aVarTmp.lVal > 1 )
				{
					long lGouCount = 0;
					IFeatureCursorPtr	ipFCur_GouPoint = gf::SelectByShape( ipG_CityRepPoint, m_ipFC_GouPoint, NULL, esriSpatialRelIntersects );
					if( ipFCur_GouPoint )
					{
						IFeaturePtr	ipF_GouPoint;
						while( ipFCur_GouPoint->NextFeature(&ipF_GouPoint) == S_OK )
						{
							ipF_GouPoint->get_Value(mFID_GouType, &aVarTmp);
							if( aVarTmp.lVal == 1 )
								++lGouCount;
						}
					}

					if( lGouCount == 0 )
						continue;
				}

				// �w�i���`�F�b�N�@�i���L�����ΏۂłȂ��w�i��̏ꍇ�͖��Ȃ��j
				long aCSCount = 0;
				IFeatureCursorPtr	ipFCur_CitySite = gf::SelectByShape( ipG_CityRepPoint, m_ipFC_CitySite, &aCSCount, esriSpatialRelWithin );
				if(aCSCount < 1) continue;	// �{���̓G���[�i�׊X�H��ɂ����Ă͂Ȃ�Ȃ��j

				IFeaturePtr	ipF_CitySite;
				if(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK)
				{
					ipF_CitySite->get_Value(mFID_BgClassC, &aVarTmp);
					switch(aVarTmp.lVal)
					{
					case city_site::bg_class::kBlock:              // �X��
					case city_site::bg_class::kForest:             // �R��
					case city_site::bg_class::kLawnOther:          // �Ő������̑��Βn
					case city_site::bg_class::kPark:               // �A�͒n
					case city_site::bg_class::kCenterDividerPark:  // ���������сi�A�͒n�j
					case city_site::bg_class::kHospital:           // �a�@
					case city_site::bg_class::kSchool:             // �w�Z
					case city_site::bg_class::kPlant:              // �H��
					case city_site::bg_class::kOtherSite:          // �ܑ��n���̑��~�n
						break;
					default:
						continue;
					}
				}

				// �w�i���`�F�b�N�A�i���L��`�����H�ɂ������Ă�������Ȃ��j
				IEnvelopePtr	ipE_AnnoRect(CLSID_Envelope);
				ipF_CityRepPoint->get_Value(mFID_ChibanNo, &aVarTmp);
				CString strChiban;
				strChiban.Format( _T("%ld"), aVarTmp.lVal );
				double	aX = 0.0, aY = 0.0;
				((IPointPtr)ipG_CityRepPoint)->QueryCoords(&aX, &aY);
				ipE_AnnoRect->PutCoords(aX - gXFactor * strChiban.GetLength() / 2.0, aY - gYFactor / 2.0, aX + gXFactor * strChiban.GetLength() / 2.0, aY + gYFactor / 2.0);
				ipE_AnnoRect->putref_SpatialReference(m_ipSpRef);
				ipE_AnnoRect->SnapToSpatialReference();
				ipFCur_CitySite = gf::SelectByShape((IGeometryPtr)ipE_AnnoRect, m_ipFC_CitySite, &aCSCount, esriSpatialRelIntersects );
				if(aCSCount < 1) continue;	// �{���̓G���[�i�׊X�H��ɂ����Ă͂Ȃ�Ȃ��j

				bool	aInvalidBG = false;
				while(ipFCur_CitySite->NextFeature(&ipF_CitySite) == S_OK)
				{
					using namespace city_site::bg_class;
					ipF_CitySite->get_Value(mFID_BgClassC, &aVarTmp);
					switch(aVarTmp.lVal) {
					case kInterCityTollExpressway:  // �s�s�ԍ������i�L���j
					case kUrbanTollExpressway:      // �s�s�����i�L���j
					case kTollRoad:                 // �L�����H
					case kNationalHighway:          // ����
					case kPrincipalPrefecturalRoad: // ��v�n����
					case kPrefectureNormalRoad:     // ��ʓs���{����
					case kOtherArterialRoad:        // ���̑��������H
					case kInterCityFreeExpressway:  // �s�s�ԍ������i�����j
					case kUrbanFreeExpressway:      // �s�s�����i�����j
					case kUnderRepairRoad:          // �H�������H
					case kTollGateLane:             // ���������[��
						aInvalidBG	= true;
						break;
					}
				}
				if(aInvalidBG) continue;

				// �����܂ŗ�����G���[�o��
				long lOID = 0;
				ipF_CityRepPoint->get_OID( &lOID );

				CString strLog;
				strLog.Format( _T("0\t%s\t%ld\t\t\tERROR\t\t�n�Ԓ��L�t���O�������Ă��Ȃ�\t%s"),
							   gf::GetEnvValue(_T("FC_CITY_REP_POINT")), lOID, strAddrCode );
				m_ofs << strLog << endl;
			}
		}

	}

	m_ofs << "#�I������: " << gf::GetCurrentTime() << endl;

	return true;

}

// �s���E�R�[�h���X�g�쐬
bool CheckCityRep::fnMakeACList()
{
	cout << "#�Z���R�[�h���X�g�쐬��...";

	CString strWhere, strSubField;
	strWhere.Format( _T("%s > '00000' and %s = 1"), city_admin::kCityCode, city_admin::kAreaClass );
	strSubField.Format( _T("distinct(%s || %s)"), city_admin::kCityCode, city_admin::kAddrCode );

	switch(m_eCheckMode)
	{
	case kCheckByRange:
		strWhere.AppendFormat( _T(" and %s between '%s' and '%s'"), city_admin::kCityCode, m_strRangeMin, m_strRangeMax );
	case kCheckAll:
		{
			IQueryDefPtr	ipQDef_CityAdmin;
			((IFeatureWorkspacePtr)m_ipWorkspaceAddress)->CreateQueryDef(&ipQDef_CityAdmin);
			ipQDef_CityAdmin->put_Tables(CComBSTR(gf::GetEnvValue(_T("FC_CITY_ADMIN"))));
			ipQDef_CityAdmin->put_SubFields(CComBSTR(strSubField));
			ipQDef_CityAdmin->put_WhereClause(CComBSTR(strWhere));

			_ICursorPtr ipCursor;
			ipQDef_CityAdmin->Evaluate(&ipCursor);
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				CComVariant vaValue;
				ipRow->get_Value(0, &vaValue);
				CString strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : NULL;
				mACList.insert(strAddrCode);
			}
		}
		break;
	case kCheckByList:
		{
			ifstream ifs( static_cast<CT2CW>(m_strAddrList) );
			if( !ifs )
			{
				cout << "�s�撬���R�[�h���X�g�̓ǂݍ��݂Ɏ��s : " << m_strAddrList << endl;
				return false;
			}

			while( !ifs.eof() )
			{
				string str;
				getline( ifs, str );
				if( !str.empty() )
					mACList.insert( str.c_str() );
			}
		}
		break;
	default:
		break;
	}

	cout << "����" << endl;

	return true;
}

// ����11���R�[�h�̍s���E���ЂƂ܂Ƃ߂ɂ���
IGeometryPtr CheckCityRep::fnUniteCityAdmin( const CString& strAddrCode )
{
	IGeometryPtr ipG_Return(CLSID_Polygon);
	ipG_Return->putref_SpatialReference( m_ipSpRef );

	CString strWhere;
	strWhere.Format( _T("%s = '%s' and %s = '%s' and %s = 1"), 
					 city_admin::kCityCode, strAddrCode.Left(5), city_admin::kAddrCode, strAddrCode.Mid(5, 6), city_admin::kAreaClass );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( _bstr_t(strWhere) );

	IFeatureCursorPtr	ipFeatureCursor;
	m_ipFC_CityAdmin->Search( ipQuery, VARIANT_FALSE, &ipFeatureCursor );
	if( ipFeatureCursor )
	{
		IFeaturePtr	ipFeature;
		while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK)
		{
			IGeometryPtr	ipG_CityAdmin, ipG_Tmp;
			ipFeature->get_Shape(&ipG_CityAdmin);
			((ITopologicalOperatorPtr)ipG_Return)->Union(ipG_CityAdmin, &ipG_Tmp);
			ipG_Return = ipG_Tmp;
		}

		return ipG_Return;
	}
	else
		return NULL;
}

