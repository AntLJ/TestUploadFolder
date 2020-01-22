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
#include "CSHelper.h"

#include <AddrLib/CSAddrPoint.h>

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace addr;

// CS�|�C���g����
EResultStat CCSHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CString strWhere;
	strWhere.Format( _T("%s = '%s'"), cs_addr_point::kAddrCode, cRec.m_strAddrCodeNew );

	IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

	long lCSCount = m_cCSPointFC.FeatureCount( ipQuery );
	IFeatureCursorPtr ipFeatureCursor( m_cCSPointFC.Search(ipQuery, VARIANT_FALSE) );
 	if( lCSCount > 0 && ipFeatureCursor )
	{
		// �t�B�[�`���擾
		IFeaturePtr	ipFeature;
		ipFeatureCursor->NextFeature( &ipFeature );

		CCSAddrPoint cCSPoint( ipFeature, sindyTableType::cs_addr_point, false, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
		
		// ���W�擾�E���b�V���w�x�ɕϊ�
		CMeshXY cMXY;
		gf::ConvPoint2MXY( cCSPoint.GetShapeCopy(), &cMXY );

		if( lCSCount > 1 )
		{
			// CS�|�C���g����������
			cRec.m_cMXY_Ref = cMXY;
			return kW_DUPLI_CS;
		}
		else
		{
			// ���\�[�X�̎擾
			CComVariant	vaValue;
			cCSPoint.GetValueByFieldName(ipc_table::kSource, vaValue );
			cRec.m_strSource = (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

			// [Bug 6180]����16���̍s���E�����݂���H(CS�|�C���g��4��[0000]�̂Ƃ�)
			if( "0000" == cRec.m_strAddrCodeNew.Mid(16, 4) )
			{
				strWhere.Format( _T("citycode='%s' and addrcode='%s' and gaikufugo=%d"), 
								 cRec.m_strAddrCodeNew.Left(5), cRec.m_strAddrCodeNew.Mid(5, 6), _tstol(cRec.m_strAddrCodeNew.Mid(11, 5)) );
				if( m_cCityAdminFC.FeatureCount(AheInitQueryFilter(NULL, NULL, strWhere)) > 0 )
				{
					cRec.m_cMXY_Ref = cMXY;
					return kW_CS_SAME_ADMIN;
				}		
			}

			if( bAddMode )	// �t�^���[�h
			{	
				cRec.m_cMXY = cMXY;
				return kO_CS_ADDED;
			}
			else	// �`�F�b�N���[�h
			{
				if( cRec.m_cMXY == cMXY )
				{
					return kO_CS_OK;
				}
				else
				{
					cRec.m_lEditPointID = cCSPoint.GetOID();
					cRec.m_cMXY_Ref = cMXY;
					return kW_CS_MOVED;
				}
			}
		}

	}
	else
		return kO_NOT_FOUND;
}

// CS�|�C���g�ړ�
EResultStat CCSHelper::movePoint( const CLRec& cRec )
{
	CCSAddrPoint cCSPoint( m_cCSPointFC.GetFeature(cRec.m_lEditPointID), sindyTableType::cs_addr_point, false, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
	if( cCSPoint )
	{		
		IPointPtr ipPoint( gf::CreateMXYPoint(cRec.m_cMXY) );
		if( ipPoint )
		{
			// �����l�̐ݒ�
			cCSPoint.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// �ŏI�X�V��
			cCSPoint.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// ��ƖړI
			cCSPoint.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// �ŏI�X�V���e(�`��ύX)
			cCSPoint.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// �ŏI�v���O�����X�V����
			cCSPoint.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// �ŏI�X�V�v���O������
			cCSPoint.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ���[�U�N���[���t���O
			cCSPoint.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// ���\�[�X
			cCSPoint.SetShape( ipPoint );																	// �`��

			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cCSPoint.Store( cErrObjs ) )
				return kW_CS_MOVED; // CS�|�C���g�ړ�
		}
	}
	return kE_CS_MOVED_FAIL; // CS�|�C���g�ړ����s
}

// CS�|�C���g�ǉ�
EResultStat CCSHelper::insertPoint( CLRec& cRec )
{
	// �t�B�[�`���̍쐬
	CRow cRow;
	CCSAddrPoint cCSPoint;
	cCSPoint.SetObject( m_cCSPointFC.CreateRow(cRow), sindyTableType::cs_addr_point, true, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
	if( cCSPoint )
	{
		IPointPtr ipPoint( gf::CreateMXYPoint(cRec.m_cMXY) );
		if( ipPoint )
		{
			// �����l�̐ݒ�
			cCSPoint.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// �ŏI�X�V��
			cCSPoint.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// ��ƖړI
			cCSPoint.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// �ŏI�X�V���e(�`��ύX)
			cCSPoint.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// �ŏI�v���O�����X�V����
			cCSPoint.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// �ŏI�X�V�v���O������
			cCSPoint.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ���[�U�N���[���t���O
			cCSPoint.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// ���\�[�X
			cCSPoint.SetValueByFieldName( cs_addr_point::kAddrCode, CComVariant(cRec.m_strAddrCodeNew) );	// �Z���R�[�h
			cCSPoint.SetShape( ipPoint );																	// �`��

			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cCSPoint.Store(cErrObjs) )
			{
				cRec.m_lEditPointID = cCSPoint.GetOID();
				return kW_CS_INSERT; // CS�|�C���g�ǉ�
			}
		}
	}

	return kE_CS_INSERT_FAIL; // CS�|�C���g�ǉ����s
}
