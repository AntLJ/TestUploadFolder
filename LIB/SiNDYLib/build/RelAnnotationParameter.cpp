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

/**
 * @file RelAnnotationParameter.cpp
 * @brief <b>CRelAnnotationParameter�N���X�����t�@�C��</b>
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @vertion $Id$
 */
#include "stdafx.h"
#include "RelAnnotationParameter.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "AnnoGlobals.h"
#include "Rule.h"
#include "ErrorObject.h"
#include "AheGeometryConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR NUMBER_OF_CHARACTERS   = _T("%s ������:%d");
	LPCTSTR STRINGS_ARE_DUPLICATE  = _T("(��̐^�ʒu�ɕR�t���\�������񓯎m���d��)");
	LPCTSTR ANGLE                  = _T("�p�x[%.1lf]");
	LPCTSTR CHARACTERS_INFO        = _T("������:%s ������:%d ���ҕ�����:%d");
	LPCTSTR THRESHOLD_AND_DISTANCE = _T("%s:%d 臒l�F%.20lf-%.20lf �����F%.20lf�i%.1lfpx�j");
	LPCTSTR CLASS_CODE             = _T("%s:%d ��ʃR�[�h:%d ���:%s");
	LPCTSTR CITY_MESH              = _T("�s�s�n�}");
	LPCTSTR DISPPOINT_SHAPE_NULL   = _T("�\���ʒu�̌`��NULL�A����1�_��������܂���");
#else
	LPCTSTR NUMBER_OF_CHARACTERS   = _T("%s number of characters:%d");
	LPCTSTR STRINGS_ARE_DUPLICATE  = _T("(Display character strings associated with one true position are duplicate)");
	LPCTSTR ANGLE                  = _T("Angle[%.1lf]");
	LPCTSTR CHARACTERS_INFO        = _T("Character string:%s Number of characters:%d Expected number of characters:%d");
	LPCTSTR THRESHOLD_AND_DISTANCE = _T("%s:%d Threshold�F%.20lf-%.20lf Distance�F%.20lf(%.1lfpx)");
	LPCTSTR CLASS_CODE             = _T("%s:%d Class code:%d Class %s");
	LPCTSTR CITY_MESH              = _T("City mesh");
	LPCTSTR DISPPOINT_SHAPE_NULL   = _T("The shape of display point is NULL or only one point");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

#ifdef SINDYLIB_CALC_TIME
double g_dCRelAnnotationParameter_CopyTime = 0.0;		//!< �I�u�W�F�N�g�R�s�[����
double g_dCreateParameterCache_AllTime = 0.0;			//!< CreateParameterCache�̑�����
double g_dCreateParameterCache_RuleCacheTime = 0.0;		//!< CreateParameterCache�̃��[���L���b�V������
double g_dCreateParameterCache_AnnotationTime = 0.0;	//!< CreateParameterCache�̒��L�^�ʒu�L����`�쐬����
double g_dCreateParameterCache_DispLineTime = 0.0;		//!< CreateParameterCache�̒��L�\���ʒu�o�E���f�B���O�{�b�N�X�쐬����

void CRelAnnotationParameter::TraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("CRelAnnotationParameter : �v�����Ԍ��� :\n") );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("                        : CreateParameterCache ������             : %.5lf�b\n"), g_dCreateParameterCache_AllTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache ���[���L���b�V��   :   %.5lf�b\n"), g_dCreateParameterCache_RuleCacheTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache �^�ʒu�L���b�V��   :   %.5lf�b\n"), g_dCreateParameterCache_AnnotationTime/1000.0 );
	TRACEMESSAGE( _T("                        : CreateParameterCache �\���ʒu�L���b�V�� :   %.5lf�b\n"), g_dCreateParameterCache_DispLineTime/1000.0 );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("CRelAnnotationParameter : CopyCRelAnnotationParameter ������      : %.5lf�b\n"), g_dCRelAnnotationParameter_CopyTime/1000.0 );
	TRACEMESSAGE( _T("                          ======================================================\n"));
}
#endif // ifdef SINDYLIB_CALC_TIME

void CRelAnnotationParameter::DeleteCache( bool bRecursive/* = false*/ )
{
	CRelAnnotation::DeleteCache(bRecursive);
	DeleteParameter();
}

// �����o�ϐ�������������
void CRelAnnotationParameter::Init()
{
	m_bIsCached = false;
}

// �p�����[�^�L���b�V�����폜����
void CRelAnnotationParameter::DeleteParameter()
{
	m_listAnnoRect.clear();
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRows = it->second.begin(); itRows != it->second.end(); ++itRows )
			static_cast<CDispLine&>(**itRows).ClearParameterCache();
	}
	m_pAnnoRule = m_pAnnoRule2 = NULL;
	m_bIsCached = false;
}

// �R�s�[����
sindyErrCode CRelAnnotationParameter::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // SINDYLIB_CALC_TIME

	// CRelAnnotationParameter�ȊO�͒ʂ��Ȃ�
	CRelAnnotationParameter* pDest = dynamic_cast<CRelAnnotationParameter*>( &dest );
	if( ! pDest )
		return sindyErr_ArgIsConsistency;

	sindyErrCode emErr = CRelAnnotation::CopyTo( dest, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	pDest->m_bIsCached = m_bIsCached;
	pDest->m_pAnnoRule = m_pAnnoRule;
	pDest->m_pAnnoRule2 = m_pAnnoRule2;
	pDest->m_listAnnoRect = m_listAnnoRect;

#ifdef SINDYLIB_CALC_TIME
	g_dCRelAnnotationParameter_CopyTime += (double)GetTickCount() - dStartFunc;
#endif // SINDYLIB_CALC_TIME

	return emErr;
}

// �p�����[�^�L���b�V�����쐬����
sindyErrCode CRelAnnotationParameter::UpdateParameter( bool bForce/* = false */ )
{
	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	static const bool bCalcFlagGeom = false; // ���g���`�󂪗~�����ꍇ��CDispLine::CalcStringBounds���ʂɌĂԂ���

	if( bForce )
		DeleteParameter();

	if( ! IsCreatedParameterCache() )
	{
		const annotation_class::CAnnotationClass* pRule = GetRule();
		if( pRule )
		{
#ifdef SINDYLIB_CALC_TIME
		g_dCreateParameterCache_RuleCacheTime += (double)GetTickCount() - dStartFunc;
		double dStartAnno = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			// �^�ʒu��`�L���b�V��
			for( int nDispLayerCount = 0; nDispLayerCount < GetDispLayerCount(); ++nDispLayerCount )
			{
				WKSEnvelope cRect;
				sindyTableType::ECode emDispLayerType = GetDispLayer( nDispLayerCount );
				emErr = CalcSymbolBounds( CalcMarkLength( emDispLayerType, true ), CalcMarkLength( emDispLayerType, false ), cRect );
				if( sindyErr_NoErr != emErr )
					ERRORLOG( emErr );
				m_listAnnoRect.push_back( cRect );
			}
			
#ifdef SINDYLIB_CALC_TIME
			g_dCreateParameterCache_AnnotationTime += (double)GetTickCount() - dStartAnno;
			double dStartDisp = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			// �\���ʒu��`�E�`��L���b�V�� TODO: CDispLine�Ɏ�������ق����ǂ�
			long lSecondLineStartIndex;
			WKSEnvelope cLineRect = {0.0}, cStringRect = {0.0};
			sindyAnnotationLineType emLineType = (sindyAnnotationLineType)-1;

			for( const_iterator it = begin(); it != end(); ++it )
			{
				for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
				{
					IGeometryPtr ipGeom;
					CDispLine& cDisp = *static_cast<CDispLine*>(itDisp->get());
					int nPointCount = cDisp.GetShape()->GetPointCount();
					WKSPoint* pStringRects = NULL;
					LOGASSERTEM_IF( nPointCount > 1, sindyErr_GeometryNotFound, DISPPOINT_SHAPE_NULL ) // �`�󂪖����f�[�^������ꍇ������ibug 5300�j TODO: OID���o����悤�ɂ��Ă�������
					{
						pStringRects = new WKSPoint[(nPointCount-1)*5]; // �Ō�̓_�͊܂܂Ȃ�
						// �\���ʒu�p�����[�^�v�Z
						emLineType = cDisp.CalcParameter( cDisp.IsAltitudeValue( GetAnnoCode() ), &lSecondLineStartIndex, pStringRects, &cLineRect, &ipGeom );
						// �\���^�C�v������̏ꍇ�̓��C���^�C�v������Ƃ���ibug 6537�j
						if( pRule->GetDispType() == schema::annotation_class::disptype_rule::kLinear )
							emLineType = sindyAnnoLine_Linear;
					}
					// ���g�������ʂ��ǂ���
					bool bIsFlag = ( pRule->GetDispType() == schema::annotation_class::disptype_rule::kFlag ) ? true : false;

					// �R�s�[�\���ǂ���
					bool bCanCopy = pRule ->GetCopyPaste( cDisp.GetTableType() );

					// ���g���̏ꍇ�͊��g���`���t�^
					if( bCalcFlagGeom && bIsFlag )
					{
						emErr = cDisp.CalcStringBounds( (IPoint*)GetShape()->GetPoint(0), ipGeom ); // TODO: �^�ʒu�|�C���g���ʁc
						if( sindyErr_NoErr != emErr )
							ERRORLOG( emErr );
					}
					// �\���ʒu�o�E���f�B���O�{�b�N�X�̋�`���擾
					if( ipGeom )
					{
						IEnvelopePtr ipEnv;
						ipGeom->get_Envelope( &ipEnv );
						ipEnv->QueryCoords( &cStringRect.XMin, &cStringRect.YMin, &cStringRect.XMax, &cStringRect.YMax );
					}
					// �L���b�V�����Z�b�g
					cDisp.SetParameterCache( nPointCount - 1, lSecondLineStartIndex, pStringRects, cStringRect, ipGeom, emLineType, bIsFlag, bCanCopy );
				}
			}
#ifdef SINDYLIB_CALC_TIME
			g_dCreateParameterCache_DispLineTime += (double)GetTickCount() - dStartDisp;
#endif // ifdef SINDYLIB_CALC_TIME

			// �L���b�V���I��
			m_bIsCached = true;
		}
		else {
			emErr = sindyErr_RuleCantCached;
			ERRORLOG( emErr );	// ���[���L���b�V�����쐬����Ă��Ȃ�
			SASSERT( m_pAnnoRule );
		}
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCreateParameterCache_AllTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emErr;
}

// �^�ʒu�ƕ\���ʒu�̃Z�b�g�ł̒P�́E�����`�F�b�N���s��
//sindyErrCode CRelAnnotationParameter::Check( std::list<CErrorObject>& listErr, bool bDoUnitCheck/* = true*/, bool bDoComplexCheck/* = true*/, double dGapThreshold/* = 0.5*/ ) const
sindyErrCode CRelAnnotationParameter::CheckLogic( CErrorObjects& cErrObjects, bool bDoUnitCheck/* = true*/, bool bDoComplexCheck/* = true*/, const double& dThresholdMinPx/* = 0.0*/, const double& dThresholdMaxPx/* = 16.0*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( g_bDisableCheckToNoSubstance )
	{
		// ���̂��Ȃ����́i�V�K�쐬���ꂽ���̂������j��
		// �`�F�b�N�Ώۂ���O�� bug 6779
		if( ! ( p || m_bCreated ) )
			return sindyErr_NoErr;
	}

	if( bDoUnitCheck && bDoComplexCheck )
	{
		const annotation_class::CAnnotationClass* pRule = GetValidRule(); // ��ʒP�ʂ̃��[��

		// �^�ʒu
		if( bDoUnitCheck )
		{
			// �^�ʒu�P�̃`�F�b�N
			TCHAR lpszNameString[_NAMESTRING_MAX_LEN+1];
			if( sindyErr_NoErr != ( emErr = CheckString1() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString1( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum1() ) ) );
			if( sindyErr_NoErr != ( emErr = CheckString2() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString2( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum2() ) ) );
			if( sindyErr_NoErr != ( emErr = CheckString3() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), NUMBER_OF_CHARACTERS, GetNameString3( _NAMESTRING_MAX_LEN, lpszNameString ), GetStringNum3() ) ) );
//			if( sindyErr_NoErr != ( emErr = CheckStringLen() ) )	cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) ); <- [bug 3035] ����Ȃ�
			if( sindyErr_NoErr != ( emErr = CheckAnnoCode() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) );
			if( sindyErr_NoErr != ( emErr = CheckDispType() ) )		cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0) ) ) );//, _T("%s:%d ��ʃR�[�h:%d ���:%s �\�����[��:%d �\���^�C�v:%d"), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ), CalcDispRuleCode( GetTableType() ), GetDispTypeCode( GetTableType() ) ) ) );

		}

		// �\���ʒu
		bool bHasS5 = false, bHasS4 = false, bHasS3 = false, bHasS2 = false, bHasS1 = false;			// �e�X�P�[���p�̕\���ʒu���݊m�F�p�i�������`�F�b�N�E�\����ʃ`�F�b�N�p�j
		bool bHasS4MtName = false, bHasS3MtName = false, bHasS2MtName = false, bHasS1MtName = false;	// �R�x���̑��݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
		bool bHasS4MtAlt  = false, bHasS3MtAlt  = false, bHasS2MtAlt  = false, bHasS1MtAlt  = false;	// �R�x�W�����݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
		TCHAR strNameString[_NAMESTRING_MAX_LEN+1] = _T("");						// �Ή�������i�[�o�b�t�@

		schema::annotation::annotation_code::ECode emAnnoCode = GetAnnoCode();	// ���L���
		for( const_iterator it = begin(); it != end(); ++it )
		{
			//-------------------------------------------------------------------------------------------
			// [bug 9221],[bug 9223]�Ή��Œǉ�
			for( const_rows_iterator itDispSrc = it->second.begin(); itDispSrc != it->second.end(); ++itDispSrc )
			{
				const CDispLine& cDispSrc( static_cast<const CDispLine&>(**itDispSrc) ); // ��r���̕\�����C��
				IGeometryPtr	ipGeomSrc = cDispSrc.GetStringBounds();

				IRelationalOperatorPtr ipRelOpe(ipGeomSrc);

				for( const_rows_iterator itDispTgt = itDispSrc; itDispTgt != it->second.end(); ++itDispTgt )
				{
					if( itDispTgt == itDispSrc ) continue;
					const CDispLine& cDispTgt( static_cast<const CDispLine&>(**itDispTgt) ); // ��r��̕\�����C��
					VARIANT_BOOL	vOverlap;
					LOGASSERTE_IF( SUCCEEDED( ipRelOpe->Disjoint ( cDispTgt.GetStringBounds(), &vOverlap ) ), sindyErr_GeometryFunctionFailed )
					{
						if( vOverlap == VARIANT_FALSE ) {
							cErrObjects.push_back( CErrorObject( cDispSrc, CErrorInfo( sindyErr_AnnoDispBoundsIsOverlaped, sindyErrLevel_ERR, NULL, NULL, cDispSrc.GetMaxScale(), STRINGS_ARE_DUPLICATE ) ) );
						}
					}
				}
			}
			//-------------------------------------------------------------------------------------------
			for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
			{
				const CDispLine& cDisp( static_cast<const CDispLine&>(**itDisp) );
				// �\���ʒu�P�̃`�F�b�N 
				if( bDoUnitCheck )
				{
					double dErrAngle = 0.0;
					if( sindyErr_NoErr != ( emErr = cDisp.CheckAnnoID() ) )									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("AnnoID[%d]"), cDisp.GetAnnoID() ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckTargetStr() ) )								cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("TargetStr[%d]"), cDisp.GetTargetStr() ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckAngle( cDisp.m_emLineType, &dErrAngle ) ) )	cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), ANGLE, dErrAngle ) ) );
					if( sindyErr_NoErr != ( emErr = cDisp.CheckLine( pRule->GetDispType() ) ) )             cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale() ) ) ); // (bug 6537)
				}

				// �\��������擾
				schema::disp_line::target_str::ECode emTargetStrCode = cDisp.GetTargetStr();	// �\�������ԍ�
				CAnnotation::GetNameString( emTargetStrCode, _NAMESTRING_MAX_LEN, strNameString );

				// �t�H���g�T�C�Y�`�F�b�N�i�ȑO�͒P�̃`�F�b�N�ɓ����Ă������A�\�������񂪕K�v�Ȃ̂ŁA�����`�F�b�N�Ɉړ��j
				if( sindyErr_NoErr != ( emErr = cDisp.CheckFontSize( strNameString ) ) ) cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, ( sindyErr_AnnoDispFontSizeInvalidWarn == emErr ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), _T("%d"), cDisp.GetFontSize() ) ) );

				// �������ƍ\���_���������Ă��邩�ǂ����`�F�b�N�ibug 3778�j
				// �����E�����ԍ��͏��1�����Ƃ��Đ�����
				long nVertexNum = cDisp.GetShape()->GetPointCount();
				long nStringNum = CString_GetLength2( CString(strNameString) );

				switch( emAnnoCode )
				{
					case schema::annotation::annotation_code::kNationalRouteNumber:
					case schema::annotation::annotation_code::kMainPrefRouteNumber: nStringNum = 1; break;
					default: break;
				}
				if( sindyErr_NoErr != ( emErr = ( nStringNum != nVertexNum - 1 ) ? sindyErr_AnnoStringNumNotEqualVertexNum : sindyErr_NoErr ) ) 
					cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1,cDisp.GetMaxScale(), CHARACTERS_INFO, strNameString, nStringNum, nVertexNum-1 ) ) );

				// �������`�F�b�N�i�s�s�n�}�ȊO�j
				// ���łɃ��O�\���p��S1�`S4�̕���������
				switch( cDisp.GetTableType() )
				{
					case sindyTableType::city_disp_line: bHasS5 = true; break;
					case sindyTableType::c_sc1disp_line: bHasS5 = true; break;
					case sindyTableType::b_sc4disp_line:
					case sindyTableType::m_sc4disp_line:
					case sindyTableType::t_sc4disp_line:
						bHasS4 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// �R�x���̑��݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS4MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS4MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc3disp_line:
					case sindyTableType::m_sc3disp_line:
					case sindyTableType::t_sc3disp_line:
						bHasS3 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// �R�x���̑��݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS3MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS3MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc2disp_line:
					case sindyTableType::m_sc2disp_line:
					case sindyTableType::t_sc2disp_line:
						bHasS2 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// �R�x���̑��݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS2MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS2MtAlt  = true; break;
								default: break;
							}
						}
						break;
					case sindyTableType::b_sc1disp_line:
					case sindyTableType::m_sc1disp_line:
					case sindyTableType::t_sc1disp_line:
						bHasS1 = true;
						if( schema::annotation::annotation_code::kMountain == emAnnoCode )		// �R�x���̑��݊m�F�p�i��ʂ��R�x�̏ꍇ�̂ݎg�p�A�����ʑΉ��I�I�I [bug 4049]�j
						{
							switch( emTargetStrCode )
							{
								case schema::disp_line::target_str::kStr1:	bHasS1MtName = true; break;
								case schema::disp_line::target_str::kStr3: bHasS1MtAlt  = true; break;
								default: break;
							}
						}
						break;

					default: break;
				}

				// �^�ʒu����\��������|���S���܂ł̋����`�F�b�N�i�L���\���ł�����̂͑S�āj
				if( CanShowMark( cDisp.GetTableType() ) )
				{
					double dDistance = 0.0;
					if( sindyErr_NoErr != ( emErr = CheckToStringBoundsLength( cDisp.m_ipGeom, cDisp.GetTableType(), dThresholdMinPx, dThresholdMaxPx, &dDistance ) ) )
					{
						// ���O�̂��߂�臒l���v�Z�iCAnnotation::CheckToStringBoundsLength���j
						double d1PXLen = AheCalcDispLineType21PXLen( cDisp.GetTableType(), true );
						double dMarkHalfLen = ( d1PXLen * (double)CalcMarkSize( cDisp.GetTableType() ) ) / (double)2.0;	// �L����`�̔����̋����i��{�j
						double dThresholdMin = dMarkHalfLen + dThresholdMinPx * d1PXLen;			// �ŏ�臒l
						double dThresholdMax = dMarkHalfLen + dThresholdMaxPx * d1PXLen;			// �ő�臒l
						double dDifferencePx = ( dDistance - dMarkHalfLen ) / AheCalcDispLineType21PXLen( cDisp.GetTableType(), true );	// ���i�s�N�Z���j
						cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), THRESHOLD_AND_DISTANCE, GetTableName(), GetOID(), dThresholdMin, dThresholdMax, dDistance, dDifferencePx ) ) );
					}
				}
				// ������݂̂ŃR�s�[�s�A���s�s�n�}�̏ꍇ�͕�����|���S���̒��ɐ^�ʒu�����邩�ǂ����`�F�b�N
				// ���g���̏ꍇ�͕�����|���S���̒��ɐ^�ʒu���Ȃ����ǂ����`�F�b�N
				bool bCanShowStrOnly = CanShowStrOnly( cDisp.GetTableType() );  // <- ������ă��[������Ȃ��āA���̑�������ˁH�����̓��[���Ń`�F�b�N���ׂ� <- �R�����g�A�E�g�ɂȂ��Ă����ǁA���̑�������Ȃ��Ƃ��߂ł���H�H�H
	/*			bool bCanShowStrOnly = false;	// ������݂̂̕\�����ł��郋�[�����ǂ����̃t���O
				switch( CalcDispRuleCode( cDisp.GetTableType() ) )
				{
					case annotation_class::scaledisprule_code::kOXOXX:
					case annotation_class::scaledisprule_code::kStrOnly:
					case annotation_class::scaledisprule_code::kStrOnlyMust: bCanShowStrOnly = true; break;
					default: bCanShowStrOnly = false; break;
				}*/
				if( ( bCanShowStrOnly && ! cDisp.m_bCanCopy && ( cDisp.GetTableType() == sindyTableType::city_disp_line /* || cDisp.GetTableType() == sindyTableType::c_sc1disp_line*/ /* || cDisp.GetTableType() == sindyTableType::b_sc4disp_line || cDisp.GetTableType() == sindyTableType::m_sc4disp_line [bug 3035] ����Ȃ�*/ ) ) || 
					cDisp.m_bIsFlag )
				{
					IRelationalOperatorPtr ipRelOpe;
					LOGASSERTE_IF( SUCCEEDED( cDisp.m_ipGeom->QueryInterface( IID_IRelationalOperator, (void**)&ipRelOpe ) ), sindyErr_COMQueryInterfaceFailed )
					{
						VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
						LOGASSERTE_IF( SUCCEEDED( ipRelOpe->Disjoint( (IGeometryPtr)((IPoint*)GetShape()->GetPoint(0)), &vbDisjoint ) ), sindyErr_GeometryFunctionFailed )
						{
							// ���g����������݂̂Ȃ̂ŁA��Ɋ��g�����`�F�b�N����K�v������
							// �s�s�n�}�̏ꍇ�́A���g���̎�ʂ͕�����`�ň͂ނ݂̂ŁA��������ɐ^�ʒu�����݂���̂ŉ��̃`�F�b�N�i���[�������������Ǝv���c�j
							if( cDisp.m_bIsFlag && cDisp.GetTableType() != sindyTableType::city_disp_line )
							{
								if( !vbDisjoint )
								{
									emErr = sindyErr_AnnoFlagButStringBoundsInside;
									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), CLASS_CODE, GetTableName(), GetOID(), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ) ) ) );
								}
							}
							else if( bCanShowStrOnly )
							{
								if( vbDisjoint )
								{
									emErr = sindyErr_AnnoNoMarkButStringBoundsOutside;
									cErrObjects.push_back( CErrorObject( cDisp, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, cDisp.GetMaxScale(), CLASS_CODE, GetTableName(), GetOID(), GetAnnoCode(), AheAnnoCode2CodeName( GetAnnoCode() ) ) ) );
								}
							}
						}
					}
				}
			}
		}
		// �������`�F�b�N�i�s�s�n�}�ȊO�j
		if( GetTableType() != sindyTableType::city_annotation )
		{
			LOGASSERTE_IF( pRule, sindyErr_RuleNotFound )
			{
				if( ! pRule->GetExceptScale( m_emTableType ) )
				{
					if( ( schema::annotation::annotation_code::kMountain != emAnnoCode && ( ( !bHasS4 && ( bHasS3 || bHasS2 || bHasS1 ) ) || ( bHasS4 && !bHasS3 && ( bHasS2 || bHasS1 ) ) || ( bHasS4 && bHasS3 && (! bHasS2 ) && bHasS1 ) ) ) ||
						( schema::annotation::annotation_code::kMountain == emAnnoCode && ( ( !bHasS4MtName && ( bHasS3MtName || bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtName && !bHasS3MtName && ( bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtName && bHasS3MtName && (! bHasS2MtName ) && bHasS1MtName ) ) ) ||
						( schema::annotation::annotation_code::kMountain == emAnnoCode && ( ( !bHasS4MtAlt  && ( bHasS3MtAlt  || bHasS2MtAlt  || bHasS1MtAlt  ) ) || ( bHasS4MtAlt  && !bHasS3MtAlt  && ( bHasS2MtAlt  || bHasS1MtAlt  ) ) || ( bHasS4MtAlt  && bHasS3MtAlt  && (! bHasS2MtAlt  ) && bHasS1MtAlt  ) ) ) )
					{
						CString strMsg;
						if( schema::annotation::annotation_code::kMountain != emAnnoCode )
						{
							if( !bHasS4 && (  bHasS3 ||   bHasS2 || bHasS1 ) ) strMsg = _T("S4");
							else if(  bHasS4 &&   !bHasS3 && ( bHasS2 || bHasS1 ) ) strMsg = _T("S3");
							else if( bHasS4 && bHasS3 && (! bHasS2 ) && bHasS1 ) strMsg = _T("S2");
						}
						else {
							if( ( !bHasS4MtName && (  bHasS3MtName || bHasS2MtName || bHasS1MtName ) ) || ( !bHasS4MtAlt && (  bHasS3MtAlt || bHasS2MtAlt || bHasS1MtAlt ) ) ) strMsg = _T("S4");
							else if( ( bHasS4MtName && !bHasS3MtName && ( bHasS2MtName || bHasS1MtName ) ) || ( bHasS4MtAlt && !bHasS3MtAlt && ( bHasS2MtAlt || bHasS1MtAlt ) ) ) strMsg = _T("S3");
							else if( ( bHasS4MtName && bHasS3MtName && (! bHasS2MtName ) && bHasS1MtName ) || ( bHasS4MtAlt && bHasS3MtAlt && (! bHasS2MtAlt ) && bHasS1MtAlt ) ) strMsg = _T("S2");
						}
						emErr = sindyErr_AnnoDispExceptExist;
						cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(0), _T("%s"), strMsg ) ) );
					}
				}
			}
		}

		// �^�ʒu�ŋL���݂̂��邢�͕\�������ɂȂ��Ă���̂ɁA�����񃉃C�������݂�����̂��Ȃ����ǂ����`�F�b�N����
		// �����񃉃C�����Ȃ��̂ɐ^�ʒu�ŕ�����݂̂�L���{������ɂȂ��Ă�����̂��Ȃ����ǂ����`�F�b�N���� [bug 4000]
		for( scale_iterator itScale = scale_begin(); itScale != scale_end(); ++itScale )
		{
			bool bHasDispLine = false;
			int nShowLevel = 0;
			CString strMsg;

			switch( *itScale )
			{
				case sindyTableType::b_sc1disp_line:
				case sindyTableType::m_sc1disp_line:
				case sindyTableType::t_sc1disp_line: bHasDispLine = bHasS1; nShowLevel = 3; strMsg = _T("S1"); break;
				case sindyTableType::b_sc2disp_line:
				case sindyTableType::m_sc2disp_line:
				case sindyTableType::t_sc2disp_line: bHasDispLine = bHasS2; nShowLevel = 2; strMsg = _T("S2"); break;
				case sindyTableType::b_sc3disp_line:
				case sindyTableType::m_sc3disp_line:
				case sindyTableType::t_sc3disp_line: bHasDispLine = bHasS3; nShowLevel = 1; strMsg = _T("S3"); break;
				case sindyTableType::b_sc4disp_line:
				case sindyTableType::m_sc4disp_line:
				case sindyTableType::t_sc4disp_line: bHasDispLine = bHasS4; nShowLevel = 0; strMsg = _T("S4"); break;
				case sindyTableType::c_sc1disp_line: bHasDispLine = bHasS5; nShowLevel = 0; strMsg = _T("D1"); break;
				case sindyTableType::city_disp_line: bHasDispLine = bHasS5; nShowLevel = 0; strMsg = CITY_MESH; break;
					break;
				default:
					_ASSERTE(false);
					break;
			}
			schema::annotation::disp_type::ECode emDispTypeCode = GetDispType( *itScale );	// �\����ʎ擾

			// �^�ʒu�ŋL���݂̂��邢�͕\�������ɂȂ��Ă���̂ɁA�����񃉃C�������݂��邩�ǂ����̃`�F�b�N
			if( ( schema::annotation::disp_type::kSym == emDispTypeCode || schema::annotation::disp_type::kNone == emDispTypeCode ) && bHasDispLine )
			{
				emErr = sindyErr_AnnoDispLineShouldNotExist;
				cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(nShowLevel), _T("%s"), strMsg ) ) );
			}
			// �����񃉃C�����Ȃ��̂ɐ^�ʒu�ŕ�����݂̂�L���{������ɂȂ��Ă�����̂��Ȃ����ǂ����`�F�b�N����
			if( ( schema::annotation::disp_type::kStr == emDispTypeCode || schema::annotation::disp_type::kSymStr == emDispTypeCode || schema::annotation::disp_type::kPoiStr == emDispTypeCode ) && ( ! bHasDispLine ) )
			{
				emErr = sindyErr_AnnoDispLineShouldExist;
				cErrObjects.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, GetDispScale(nShowLevel), _T("%s"), strMsg ) ) );
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

extern IGeometryPtr _WKSPoints2Polygon( long lPointCount, WKSPoint* pPoints, ISpatialReference* ipSpRef );

// ���݊i�[����Ă���t�B�[�`���̌`��̃R�s�[��Ԃ�
IGeometryPtr CRelAnnotationParameter::GetShapeCopy( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, bool bDiff/* = false*/, long lParam/* = -1*/ ) const
{
	// �X�P�[�����w�肳��Ă��Ȃ��ꍇ��CRow::GetShapeCopy�ɏ������܂킷
	if( 0 > lParam )
		return CRow::GetShapeCopy( bLatest, bOriginal, bFeatureChangedOriginal, bDiff );

	// �L���`��E������|���S����Ԃ�
	sindyTableType::ECode emDispTableType = sindyTableType::unknown; // �Ώۂ̕\���ʒu
	long lIndex = 0; // �L����`�̃C���f�b�N�X
	for( scale_iterator it = scale_begin(); it != scale_end(); ++it, ++lIndex )
	{
		if( AheTableType2AnnotationScale( *it ) == (sindyAnnotationScale)lParam )
		{
			emDispTableType = *it;
			break;
		}
	}

	if( emDispTableType == sindyTableType::unknown ) // lParam�Ɛ^�ʒu���}�b�`���Ȃ��ꍇ�ie.x. lParam=3125, table type=base_annotation�j
		return IGeometryPtr();

	ISpatialReferencePtr ipSpRef( GetShape()->GetSpatialReference() );
	IGeometryCollectionPtr ipEnumGeom( CLSID_GeometryBag ); // ConstructUnion�p
	((IGeometryPtr)ipEnumGeom)->putref_SpatialReference( ipSpRef );

	// �^�ʒu��`�|���S��	
	IGeometryPtr ipAnnoPoly;
	{
		annorect_const_iterator it = annorect_begin();
		std::advance( it, lIndex );
		IEnvelopePtr ipEnv( CLSID_Envelope );
		ipEnv->putref_SpatialReference( ipSpRef );
		ATLTRY( SUCCEEDED( ipEnv->PutWKSCoords( const_cast<WKSEnvelope*>(&*it) ) ) );
		ipAnnoPoly = ToPolygon( ipEnv );
		ATLTRY( SUCCEEDED( ipEnumGeom->AddGeometry( ipAnnoPoly ) ) ); // �ǉ�
	}
	// �\���ʒu�|���S��
	const_iterator it = find( emDispTableType );
	if( it != end() )
	{
		for( const_rows_iterator itDisp = it->second.begin(); itDisp != it->second.end(); ++itDisp )
		{
			const CDispLine& cDisp = static_cast<const CDispLine&>(**itDisp);
			ATLTRY( SUCCEEDED( ipEnumGeom->AddGeometry( _WKSPoints2Polygon( cDisp.m_nStringRectsSize * 5, cDisp.m_pStringRects, ipSpRef ) ) ) ); // �ǉ�
		}
	}
	IGeometryPtr ipRet( AheCreateClone( ipAnnoPoly ) ); // �֋X�I�ɐ^�ʒu�|���S������쐬
	ATLTRY( SUCCEEDED( ((ITopologicalOperatorPtr)ipRet)->ConstructUnion( (IEnumGeometryPtr)ipEnumGeom ) ) ); // UNION

	return ipRet;
}

// �K�؂ȃ��[���L���b�V����Ԃ�
const annotation_class::CAnnotationClass* CRelAnnotationParameter::GetValidRule() const
{
	const annotation_class::CAnnotationClass* pRule = NULL;	// �Ԃ�l

	schema::annotation::annotation_code::ECode emCode = GetAnnoCode();
	if( schema::annotation::annotation_code::kSubwaySta == emCode || schema::annotation::annotation_code::kSubwaySubSta == emCode )
	{
		if( m_pAnnoRule && m_pAnnoRule->IsValidRule( m_emTableType ) )
			pRule = m_pAnnoRule;
		else if( m_pAnnoRule2 && m_pAnnoRule2->IsValidRule( m_emTableType ) )
			pRule = m_pAnnoRule2;
		else {
			LOGASSERT(false,sindyErr_RuleNotFound);
		}
	}
	else {
		LOGASSERTE_IF( m_pAnnoRule, sindyErr_RuleNotFound )
			pRule = m_pAnnoRule;
	}

	return pRule;
}

} // sindy
