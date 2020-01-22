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
#include "GPHelper.h"

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

#include <AddrLib/CodeConverter.h>

#include <regex>

static const tr1::wregex g_re( L"[0-9\\-]*" );
// �ړ����ǉ����̔���ɗ��p
const bool MOVEMODE = true;
const bool INSERTMODE = false;

/**
 * @brief	������0�����֐�
 * @return	0����������������(�S��0�̏ꍇ�́A0��Ԃ�)
 */
inline CString CString_TrimLeft0( CString strData )
{
	return strData.TrimLeft(_T("0")).IsEmpty()? _T("0") : strData;
}

// ���|�C���g���� ([Bug 6181]�D��t���O�t�����ŗD��|�C���g�ɂ���)
EResultStat CGPHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CSPGouPoint pGou;			// �ꎞ���|�C���g���i�[�p
	CSPGouPoint pNotPriorGou;	// �D��t���O�Ȃ����|�C���g���ێ��p

	// �܂���16���ōs���E����
	EResultStat emStatus = kO_NOT_FOUND;

	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddr16) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		if( cRec.m_strAddrCodeNew.Mid(16, 4) == "0000" )
		{
			// ���x���|�C���g�̎擾
			const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*m_cGous.begin(sindyTableType::city_admin));

			IPointPtr ipLabelPoint;
			((IAreaPtr)pAdmin->GetShapeCopy())->get_LabelPoint( &ipLabelPoint );

			gf::ConvPoint2MXY( ipLabelPoint, &cRec.m_cMXY_Ref );
			return kW_GP_SAME_ADMIN;
		}

		emStatus = findGPByAdmin( cRec, pGou, kCTAddr16 );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus )
			pNotPriorGou = pGou;	// �D��t���O�Ȃ������ێ�
	}

	// ����11���Ō���
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddr11) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddr11 );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
			pNotPriorGou = pGou;	// �D��t���O�Ȃ������ێ�(��ʍs���E�����Ō������Ă��Ȃ��Ƃ�)		
	}

	// ����11�����m�g�����ԍ��t���O�FON�n�Ō���
	bool bNotPriorExFlag = false;	// �g�����ŗD��t���O�Ȃ������������Ƃ��Ɉꎞ�I�Ɏg�p����t���O
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddrEX) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddrEX );
		if( kO_MATCH_PRIOR_GP == emStatus )
		{
			cRec.m_lExFlag = 1;	// �g�����ԍ��t���O = On�Ō��������̂�
			return getDetailStatus( cRec, pGou, bAddMode );
		}
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
		{
			bNotPriorExFlag = true;
			pNotPriorGou = pGou;	// �D��t���O�Ȃ������ێ�(��ʍs���E�����Ō������Ă��Ȃ��Ƃ�)
		}
	}

	// ����ADDRCODE2�Ō���(Bug8166)
	m_cGous.SelectByWhereClause( MakeWhereClause(cRec, kCTAddrCM) );
	if( m_cGous.size(sindyTableType::city_admin) > 0 )
	{
		emStatus = findGPByAdmin( cRec, pGou, kCTAddrCM );
		if( kO_MATCH_PRIOR_GP == emStatus )
			return getDetailStatus( cRec, pGou, bAddMode );
		else if ( kW_GP_DUPLI_PRIOR == emStatus )
			return kW_GP_DUPLI_PRIOR;
		else if( kO_MATCH_GP == emStatus && !pNotPriorGou )
			pNotPriorGou = pGou;	// �D��t���O�Ȃ������ێ�(��ʍs���E�����Ō������Ă��Ȃ��Ƃ�)
	}

	// �g�����ŗD��t���O�Ȃ����ƈ�v���Ă�����A�����Ń��X�g���R�[�h�̊g���t���O1�ɂ���
	if( bNotPriorExFlag )
		cRec.m_lExFlag = 1;

	// �ǂ̍s���E���x���ł��D��Ȃ����ƈ�v���Ă��Ȃ���΁A��v�Ȃ�
	return !pNotPriorGou? kO_NOT_FOUND : getDetailStatus(cRec, pNotPriorGou, bAddMode);
}

// ���|�C���g�ړ�
EResultStat CGPHelper::movePoint( CLRec& cRec, bool bPriorF )
{
	CGouPoint cGou( m_cGouPointFC.GetFeature(cRec.m_lEditPointID), sindyTableType::gou_point, false, m_cGouPointFC.GetFieldMap(), m_cGouPointFC.GetNameString() );

	// �ړ���|�C���g����
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( cGou && ipPoint )
	{		
		// �ړ��O��̍s���E���`�F�b�N
		ISpatialFilterPtr ipOldSpFilter( AheInitSpatialFilter(NULL, cGou.GetShapeCopy()) );
		ISpatialFilterPtr ipNewSpFilter( AheInitSpatialFilter(NULL, ipPoint) );

		long lOldCount = m_cCityAdminFC.FeatureCount(ipOldSpFilter), lNewCount = m_cCityAdminFC.FeatureCount(ipNewSpFilter);
		IFeatureCursorPtr ipOldCursor( m_cCityAdminFC.Search(ipOldSpFilter, VARIANT_FALSE) ), ipNewCursor( m_cCityAdminFC.Search(ipNewSpFilter, VARIANT_FALSE) );
		if( 1 == lOldCount && 1 == lNewCount && ipOldCursor && ipNewCursor )
		{
			IFeaturePtr ipOldAdminF, ipNewAdminF;
			if( ipOldCursor->NextFeature( &ipOldAdminF ) == S_OK && ipNewCursor->NextFeature( &ipNewAdminF ) == S_OK )
			{
				CCityAdmin cOldAdmin( ipOldAdminF, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );
				CCityAdmin cNewAdmin( ipNewAdminF, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

				// �s���E�����Z���R�[�h��r
				if( cOldAdmin.GetAddrCode(cRec.IsCharAddrCode()) != cNewAdmin.GetAddrCode(cRec.IsCharAddrCode()) )
					return kW_GP_MOVED_DIFF_ADMIN;	// �y�v�m�F�z�ړ��O��ōs���E�̏Z���R�[�h���قȂ�

				// �������炪�ړ�����
				// �����l�̐ݒ�
				cGou.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// �ŏI�X�V��
				cGou.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// ��ƖړI
				cGou.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// �ŏI�X�V���e(�`��ύX)
				cGou.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// �ŏI�v���O�����X�V����
				cGou.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// �ŏI�X�V�v���O������
				cGou.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ���[�U�N���[���t���O
				cGou.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// ���\�[�X
				cGou.SetValueByFieldName( gou_point::kPriority, CComVariant(1L) );							// �D��t���O
				cGou.SetValueByFieldName( gou_point::kGouType, CComVariant(1L) );							// ���^�C�v[Bug6318]
				cGou.SetShape( ipPoint );																	// �`��

				// �X�V��DB���Q�Ƃ��āA�ړ���Ƃ��Ă���ׂ��|�C���g���擾�A��r���� (bug 9300)
				if( !checkGPByReferenceDB( cRec, cNewAdmin, cGou, MOVEMODE ) )
						return kE_MOVED_REF_FAIL;
		
			}
			else
				return kW_GP_MOVED_FAIL;
		}
		else
			return kW_GP_MOVED_FAIL;

		CErrorObjects cErrObjs;
		if( sindyErr_NoErr == cGou.Store(cErrObjs) )
			return bPriorF? kW_GP_MOVED : kW_GP_MOVED_NOT_PRIOR;
		else
			return kW_GP_MOVED_FAIL;
	}
	else
		return kW_GP_MOVED_FAIL;
}

// ���|�C���g�ǉ�
EResultStat CGPHelper::insertPoint( CLRec& cRec )
{
	// �t�B�[�`���̍쐬�ƈړ���|�C���g�̐���
	CRow cRow;
	CGouPoint cGou;
	cGou.SetObject( m_cGouPointFC.CreateRow(cRow), sindyTableType::gou_point, true, m_cGouPointFC.GetFieldMap(), m_cGouPointFC.GetNameString() );
	IPointPtr ipPoint = gf::CreateMXYPoint( cRec.m_cMXY );
	if( cGou && ipPoint )
	{
		// ���W�ʒu�̍s���E������
		ISpatialFilterPtr ipSpFilter( AheInitSpatialFilter(NULL, ipPoint) );

		long lCount = m_cCityAdminFC.FeatureCount( ipSpFilter );
		IFeatureCursorPtr ipFeatureCursor( m_cCityAdminFC.Search(ipSpFilter, VARIANT_FALSE) );
		if( lCount == 1 && ipFeatureCursor )
		{
			IFeaturePtr	ipAdminFeature;
			ipFeatureCursor->NextFeature( &ipAdminFeature );

			CCityAdmin cAdmin( ipAdminFeature, sindyTableType::city_admin, false, m_cCityAdminFC.GetFieldMap(), m_cCityAdminFC.GetNameString() );

			CString	strGouNo;
			long lExFlag = 0;

			// �����t���n�Ԃ̏ꍇ���l��
			CString strTmpGouNo1;
			if( cRec.IsCharPntCode() )
			{
				strTmpGouNo1 = CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(11, 5));
				if( strTmpGouNo1.IsEmpty() )
					return kE_PNTNAME_GET_FAIL;	// �����n�Ԗ��̎擾���s
			}
			else
				strTmpGouNo1 = CString_TrimLeft0(cRec.m_strAddrCodeNew.Mid(11, 5));

			// �����t�����̏ꍇ���l��
			CString strTmpGouNo2;
			if( cRec.IsCharGouCode() )
			{
				strTmpGouNo2 = CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(16, 4));
				if( strTmpGouNo2.IsEmpty() )
					return kE_GOUNAME_GET_FAIL;	// ���������̎擾���s
			}
			else
				strTmpGouNo2 = CString_TrimLeft0(cRec.m_strAddrCodeNew.Mid(16, 4));

			// �s���E��16�P�^�ƈ�v���邩�H
			if( cAdmin.GetAddrCode(cRec.IsCharAddrCode()).GetLength() == 16 )
			{
				if( cRec.m_strAddrCodeNew.Left(16) != cAdmin.GetAddrCode(cRec.IsCharAddrCode()) )
				{
					lExFlag = 1;	// �g�����Ƃ��Ēǉ�
					if( cRec.m_strAddrCodeNew.Mid(16, 4) != "0000" )
						strGouNo = strTmpGouNo1 + "-" + strTmpGouNo2;
					else
						strGouNo = strTmpGouNo1;
				}
				else
					strGouNo = strTmpGouNo2;
			}
			else
			{
				if( cRec.m_strAddrCodeNew.Mid(16, 4) != "0000" )
					strGouNo = strTmpGouNo1 + "-" + strTmpGouNo2;
				else
					strGouNo = strTmpGouNo1;
			}

			// ���ԍ��ɕ������܂܂��ꍇ(���p������[-]�ȊO�������Ă�����)�́A�擪��[#]��}��
			if( ! tr1::regex_match((const wchar_t*)strGouNo, g_re) )
				strGouNo = _T("#") + strGouNo;

			// �����l�̐ݒ�
			cGou.SetValueByFieldName( gou_point::kOperator, CComVariant(gf::GetCurAccount()) );
			cGou.SetValueByFieldName( gou_point::kPurpose, CComVariant(cRec.m_lPurpose) );
			cGou.SetValueByFieldName( gou_point::kUpdateType, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kProgModifyDate,	CComVariant(gf::GetCurDateStr()) );
			cGou.SetValueByFieldName( gou_point::kModifyProgName,	CComVariant(PROGRAM_NAME) );
			cGou.SetValueByFieldName( gou_point::kUserClaim, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kSource, CComVariant(cRec.m_strClaimNo) );
			cGou.SetValueByFieldName( gou_point::kPriority, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kExpGouNo, CComVariant(lExFlag) );
			cGou.SetValueByFieldName( gou_point::kGouType, CComVariant(1L) );
			cGou.SetValueByFieldName( gou_point::kGouNo, CComVariant(strGouNo) );
			cGou.SetShape( ipPoint );

			// �X�V��DB���Q�Ƃ��āA�ړ���Ƃ��Ă���ׂ��|�C���g���擾�A��r���� (bug 9300)
			if( !checkGPByReferenceDB( cRec, cAdmin, cGou, INSERTMODE ) )
				return kE_INSERT_REF_FAIL;
			
			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cGou.Store(cErrObjs) )
			{
				cRec.m_lEditPointID = cGou.GetOID();
				cRec.m_lExFlag = lExFlag;
				return lExFlag? kW_GP_INSERT_EX : kW_GP_INSERT;
			}
			else
				return lExFlag? kE_GP_INSERT_EX_FAIL : kE_GP_INSERT_FAIL;
		}
	}

	return kE_GP_INSERT_FAIL;
}

/////////////////////////////////////
//	�����֐�
/////////////////////////////////////

// �`�F�b�N�^�C�v���Ƃ̍s���EWhere��쐬
CString CGPHelper::MakeWhereClause( const CLRec& cRec, ECheckType emCType )
{
	CString strWhere;
	strWhere.Format( _T("%s='%s'"), city_admin::kCityCode, cRec.m_strAddrCodeNew.Left(5) );
	switch( emCType )
	{
	case kCTAddr16:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6) );
		if( !cRec.IsCharPntCode() )
			strWhere.AppendFormat( _T(" and %s=%d"), city_admin::kGaikuFugo, _tstol(cRec.m_strAddrCodeNew.Mid(11, 5)) );
		else
			strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kExtGaikuFugo, CCodeConverter::GetCodeConverter().GetPntName(cRec.m_strAddrCodeNew.Left(11), cRec.m_strAddrCodeNew.Mid(11, 5)) );
		break;
	case kCTAddr11:
		strWhere.AppendFormat( _T(" and %s='%s' and %s is null"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6), city_admin::kGaikuFugo );
		if( cRec.IsCharAddrCode() )
			strWhere.AppendFormat( _T(" and %s is null"), city_admin::kExtGaikuFugo );
		break;
	case kCTAddrEX:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode, cRec.m_strAddrCodeNew.Mid(5, 6) );
		if( !cRec.IsCharAddrCode() )
			strWhere.AppendFormat( _T(" and %s is not null"), city_admin::kGaikuFugo );
		else
			strWhere.AppendFormat( _T(" and (%s is not null or %s is not null)"), city_admin::kGaikuFugo, city_admin::kExtGaikuFugo );
		break;
	case kCTAddrCM:
		strWhere.AppendFormat( _T(" and %s='%s'"), city_admin::kAddrCode2, cRec.m_strAddrCodeNew.Mid(5, 6) );
		break;
	default:
		return NULL;
	}

	return strWhere;
}

// �w��s���E���̍��|�C���g�m�F
EResultStat CGPHelper::findGPByAdmin( const CLRec& cRec, CSPGouPoint& rGou, ECheckType emCType )
{
	std::list<CSPGouPoint> listGou;		// 20����v+�D��t���O�Ȃ�
	std::list<CSPGouPoint> listPriorGou;	// 20����v+�D��t���O�p

	CCityAdmin::const_rows_iterator itAdminEnd = m_cGous.end(sindyTableType::city_admin);
	for( CCityAdmin::const_rows_iterator itAdmin = m_cGous.begin(sindyTableType::city_admin); itAdmin != itAdminEnd; ++itAdmin )
	{
		const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);

		CGouPoint::const_rows_iterator itGouEnd = pAdmin->end(sindyTableType::gou_point);
		for( CGouPoint::const_rows_iterator itGou = pAdmin->begin(sindyTableType::gou_point); itGou != itGouEnd; ++itGou )
		{
			const boost::shared_ptr<CGouPoint> pGou = boost::static_pointer_cast<CGouPoint>(*itGou);

			// 11��+�g�����������Ɋg�����łȂ��Ȃ��΂�
			if( kCTAddrEX == emCType && !pGou->IsExGouNo() )
				continue;

			// 20���R�[�h����v�H
			CString strGouCode( pGou->GetAddrCode(cRec.IsCharAddrCode()) );
			if( strGouCode.GetLength() == 20 )
			{
				// �ʏ̖�����Ή��̏ꍇ�A�厚�E���R�[�h��ADDRCODE2���̗p
				if( kCTAddrCM == emCType )
					strGouCode.Format( _T("%s%s%s"), strGouCode.Left(5), pAdmin->GetOazaAzaCode2(), strGouCode.Mid(11) );

				if( strGouCode == cRec.m_strAddrCodeNew )
				{
					if( pGou->IsPrior() )	listPriorGou.push_back( pGou );
					else					listGou.push_back( pGou );
				}
			}
		}
	}

	// �D��t���O����ƈ�v�����H
	if( !listPriorGou.empty() )
	{
		rGou = *(listPriorGou.begin());
		return listPriorGou.size() == 1? kO_MATCH_PRIOR_GP : kW_GP_DUPLI_PRIOR;
	}
	else if( !listGou.empty() )	// �D��t���O�Ȃ��ƈ�v�����H
	{
		rGou = *(listGou.begin());
		return kO_MATCH_GP;
	}
	else
		return kO_NOT_FOUND;
}

// ���R�[�h�ƈ�v�������|�C���g�̊֌W����ڍ׃X�e�[�^�X�擾
EResultStat CGPHelper::getDetailStatus( CLRec& cRec, const CSPGouPoint& pGou, bool bAddMode )
{
	// ���\�[�X�́A�������Ŏ擾����悤�ɂ���
	CComVariant	vaValue;
	pGou->GetValueByFieldName(ipc_table::kSource, vaValue );
	cRec.m_strSource = (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

	// ���MeshXY���擾
	CMeshXY cTmpMXY;
	gf::ConvPoint2MXY( pGou->GetShapeCopy(), &cTmpMXY );

	// �D��t���O����Ȃ��ŏꍇ�킯
	if( pGou->IsPrior() )
	{
		if( bAddMode )
		{
			cRec.m_cMXY = cTmpMXY;
			return (1 == pGou->GetGouType())? kO_GP_ADDED_PRIOR : kW_GP_ADDED_PRIOR_TPG;
		}
		else
		{
			if( cRec.m_cMXY == cTmpMXY )
			{
				return (1 == pGou->GetGouType())? kO_GP_OK : kW_GP_PRIOR_TPG;
			}
			else
			{
				cRec.m_cMXY_Ref = cTmpMXY;
				cRec.m_lEditPointID = pGou->GetOID();
				return kW_GP_MOVED;
			}
		}
	}
	else
	{
		cRec.m_cMXY_Ref = cTmpMXY;
		cRec.m_lEditPointID = pGou->GetOID();
		return kW_GP_NOT_PRIOR;
	}
}

// �ړ��E�ǉ��\��|�C���g�ƎQ��DB�̃|�C���g�Ƃ̔�r (bug9300)
bool CGPHelper::checkGPByReferenceDB( CLRec& cRec, CCityAdmin& cAdmin, CGouPoint& cGou, bool bIsMovePoint )
{
	// �Q�ƍs���E�Ƃ̔�r
	IPointPtr ipPointRefGou = gf::CreateMXYPoint( cRec.m_cMXY );	
	if( !ipPointRefGou )
		return false;
	
	// ���W�ʒu�̍s���E������
	ISpatialFilterPtr ipRefAdminFilter( AheInitSpatialFilter(NULL, ipPointRefGou) );
	long lRefAdminCount = m_cCityAdminFC_Ref.FeatureCount( ipRefAdminFilter );
	if( lRefAdminCount != 1 )
		return false;

	IFeatureCursorPtr ipRefAdminCursor( m_cCityAdminFC_Ref.Search(ipRefAdminFilter, VARIANT_FALSE) );
	if( !ipRefAdminCursor )
		return false;

	IFeaturePtr	ipRefAdminFeature;
	if( S_OK == ipRefAdminCursor->NextFeature( &ipRefAdminFeature ) && ipRefAdminFeature )
	{
		CCityAdmin cRefAdmin( ipRefAdminFeature, sindyTableType::city_admin, false, m_cCityAdminFC_Ref.GetFieldMap(), m_cCityAdminFC_Ref.GetNameString() );

		// CITYCODE�`�F�b�N
		if( 0 != cAdmin.GetCityCode().CompareNoCase( cRefAdmin.GetCityCode() ) )
			return false;

		// ADDRCODE�`�F�b�N
		if( 0 != cAdmin.GetOazaAzaCode().CompareNoCase( cRefAdmin.GetOazaAzaCode() ) )
			return false;
		
		// EXTGAIKUFUGO�`�F�b�N
		if( 0 != cAdmin.GetExGaikuFugo().CompareNoCase( cRefAdmin.GetExGaikuFugo() ) )
			return false;
			
		// ADDRCODE2�`�F�b�N
		if( 0 != cAdmin.GetOazaAzaCode2().CompareNoCase( cRefAdmin.GetOazaAzaCode2() ) )
			return false;

		// ADDRCLASS_C �͈ړ����̂݃`�F�b�N
		if( bIsMovePoint )
		{
			if( cAdmin.GetAddrClass() != cRefAdmin.GetAddrClass() )
				return false;
		}
	}
	else
		return false;
	

	// �Q�ƃ|�C���g�Ƃ̔�r
	ISpatialFilterPtr ipRefGouFilter( AheInitSpatialFilter(NULL, ipPointRefGou) );
	long lRefGouCount = m_cGouPointFC_Ref.FeatureCount(ipRefGouFilter);
	if( 1 > lRefGouCount )
		return false;

	IFeatureCursorPtr ipRefGouCursor( m_cGouPointFC_Ref.Search(ipRefGouFilter, VARIANT_FALSE) );
	if( !ipRefGouCursor )
		return false;
	
	IFeaturePtr ipRefGou;
	while( S_OK == ipRefGouCursor->NextFeature( &ipRefGou ) && ipRefGou )
	{
		CGouPoint cRefGou( sindyTableType::gou_point, ipRefGou );
			
		// PRIORITY_F�`�F�b�N
		if( cGou.IsPrior() == cRefGou.IsPrior() )
		{
			// GAIKUFUGO�AGOU_NO�AEXTGOU_F�`�F�b�N
			// �擾�ł���20���R�[�h(�����t�l������)����v���邩�ǂ����ł��킹�Ĕ���
			if( 0 == cGou.GetAddrCode(true).CompareNoCase( cRefGou.GetAddrCode(true) ) )
				// �����̃`�F�b�N���ʂ�|�C���g��������΂��̎��_��return����
				return true;
		}
	}

	// ���|�C���g�̃`�F�b�N�ʂ炸
	return false;
}
