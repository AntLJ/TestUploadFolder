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

#include "stdafx.h"
#include "TelTPHelper.h"

#include <AddrLib/CSAddrPoint.h>

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace addr;

// TelTP�|�C���g����
EResultStat CTelTPHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CString strWhere;
	strWhere.Format( _T("%s = '%s' and OBJECTID = '%s'"), teltp_point::kSequence, cRec.m_strSequence, cRec.m_strObjectID );

	IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

	long lTelTPCount = m_cTelTPPointFC.FeatureCount( ipQuery );
	IFeatureCursorPtr ipFeatureCursor( m_cTelTPPointFC.Search(ipQuery, VARIANT_FALSE) );

 	if( lTelTPCount > 0 && ipFeatureCursor )
	{
		// �t�B�[�`���擾
		ipFeatureCursor->NextFeature( &m_ipFeature );

		CRow	cTelTPPoint( m_ipFeature, sindyTableType::unknown, false, m_cTelTPPointFC.GetFieldMap(), m_cTelTPPointFC.GetNameString() );

		// ���W�擾�E���b�V���w�x�ɕϊ�
		CMeshXY cMXY;
		gf::ConvPoint2MXY( cTelTPPoint.GetShapeCopy(), &cMXY );

		// ���W�t�^���(ADDXY_C)�擾
		long lAddXY;
		CComVariant vaValue;
		cTelTPPoint.GetValueByFieldName( _T("ADDXY_C"), vaValue );
		lAddXY = (vaValue.vt == VT_I4)? vaValue.lVal : -1;
		if( lAddXY == -1 )
			return kE_FAIL_GET_ADDXY;

		// TelTP����e����擾
		if( !getInfo( cRec, cTelTPPoint ) )
		{
			return kE_FAIL_GET_INFO;
		}

		if( bAddMode )	// �t�^���[�h
		{	
			cRec.m_cMXY		= cMXY;
			cRec.m_lAddXY	= lAddXY;
			return kO_TELTP_ADDED;
		}
		else			// �`�F�b�N���[�h
		{
			// ����V�[�P���X��AddXY_C�`�F�b�N
			CString strWhereSameSeq;
			strWhereSameSeq.Format( _T("%s = '%s'"), teltp_point::kSequence, cRec.m_strSequence );
			IQueryFilterPtr ipQuerySameSeq( AheInitQueryFilter(NULL, NULL, strWhereSameSeq) );

			long lTelTPCountSameSeq = m_cTelTPPointFC.FeatureCount( ipQuerySameSeq );
			if( lTelTPCountSameSeq > 1 )
			{
				vector<long>	vlAddXY;
				bool			bDupliFlag = false;
				cRec.m_strAddComment.Format( _T("(����V�[�P���X:AddXY_C<") );

				IFeatureCursorPtr ipFeatureCursorSameSeq( m_cTelTPPointFC.Search(ipQuerySameSeq, VARIANT_FALSE) );
				IFeaturePtr	ipFeature;
				while( ipFeatureCursorSameSeq->NextFeature( &ipFeature ) == S_OK )
				{
					CRow	cTelTPPointSameSeq( ipFeature, sindyTableType::unknown, false, m_cTelTPPointFC.GetFieldMap(), m_cTelTPPointFC.GetNameString() );
					long	lAddXYSameSeq	= 0;
					cTelTPPointSameSeq.GetValueByFieldName( _T("ADDXY_C"), vaValue );
					lAddXYSameSeq	= (vaValue.vt == VT_I4)? vaValue.lVal : -1;
					cRec.m_strAddComment.AppendFormat( _T("%d "), lAddXYSameSeq );
					if( !vlAddXY.empty() )
					{
						vector<long>::iterator itr = find( vlAddXY.begin(), vlAddXY.end(), lAddXYSameSeq );
						if( itr != vlAddXY.end() )
							bDupliFlag = true;
					}
					vlAddXY.push_back( lAddXYSameSeq );
				}
				cRec.m_strAddComment.Append( _T(">)") );
				if( bDupliFlag == true )
					return kW_DUPLI_TELTP;
			}

			// ���W���ړ����Ă��Ȃ����H
			if( cRec.m_cMXY == cMXY )
			{
				// ADDXY_C���w��ƈႤ (�w�肪�Ȃ��ꍇ�̓N���[������)
				if( cRec.m_lAddXY != lAddXY )
				{
					return kW_TELTP_FAIL_ADDXY;
				}

				// ADDXY_C=1 ���� POINTSTAT_C != 2 (�N���[���ΏۂȂ̂Ɂu�Y���{�݂���v�ȊO)
				if( cRec.m_lAddXY == 1 && cRec.m_lPointStat != 2 )
				{
					return kW_TELTP_FAIL_PSTAT;
				}
			}
			// ���W�ړ�
			else
			{
				cRec.m_cMXY_Ref = cMXY;
				return kW_TELTP_MOVED;
			}
		}
		return kO_TELTP_OK;
	}
	else
	{
		return kW_TELTP_NOT_FOUND;
	}
}

// TelTP�|�C���g�̏��擾
bool CTelTPHelper::getInfo( CLRec& cRec, CRow cTelTPPoint )
{
	// TelTP����e������擾
	CComVariant vaValue;

	// ���[�U�N���[���t���O
	cTelTPPoint.GetValueByFieldName( _T("USERCLAIM_F"), vaValue );
	cRec.m_lUserClaimF		= (vaValue.vt == VT_I4)? vaValue.lVal : -1;

	// �d�b�ԍ�
	cTelTPPoint.GetValueByFieldName( _T("TEL_NUM"), vaValue );
	cRec.m_strTelNum		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// �f�ڏZ��
	cTelTPPoint.GetValueByFieldName( _T("ADDRNAME"), vaValue );
	cRec.m_strAddrName		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// �Z���R�[�h
	cTelTPPoint.GetValueByFieldName( _T("ADDRCODE"), vaValue );
	cRec.m_strAddrCode		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// ����
	cTelTPPoint.GetValueByFieldName( _T("KATAGAKINAME"), vaValue );
	cRec.m_strKatagaki		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// �f�ږ���
	cTelTPPoint.GetValueByFieldName( _T("KEISAINAME"), vaValue );
	cRec.m_strKeisaiName	= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// �������ʃR�[�h
	cTelTPPoint.GetValueByFieldName( _T("POINTSTAT_C"), vaValue );
	cRec.m_lPointStat		= (vaValue.vt == VT_I4)? vaValue.lVal : -1;
		
	// ���\�[�X
	cTelTPPoint.GetValueByFieldName( ipc_table::kSource, vaValue );
	cRec.m_strSource		= (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	return true;
}

// �w���ʂ̃h���C���擾(���󖢎g�p�A�g�p���邽�߂ɂ�CheckTelTPError::outputLog��CTelTPHelper��ǉ�����B)
const std::string CTelTPHelper::getDomain(LPCTSTR lpszDomainName, long lDomainCode) const
{
	// ���W�t�^���
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	ipFields->FindField( CComBSTR(lpszDomainName), &lFID );
	IFieldPtr ipField;
	ipFields->get_Field( lFID, &ipField );
	if( ipField == NULL )
		return "";

	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	if( ipDomain == NULL )
		return "";

	ICodedValueDomainPtr ipCVDoamin( ipDomain );
	if( ipCVDoamin == NULL )
		return "";

	USES_CONVERSION;

	LONG lDomainCount = 0;
	ipCVDoamin->get_CodeCount( &lDomainCount );
	for( LONG i=0;i<lDomainCount;++i )
	{
		CComVariant vaValue;
		ipCVDoamin->get_Value( i, &vaValue );
		if( static_cast<LONG>(lDomainCode) == vaValue.lVal ){
			CComBSTR bstr;
			ipCVDoamin->get_Name( i, &bstr );
			return OLE2A(bstr);
		}
	}
	
	return "";
}
