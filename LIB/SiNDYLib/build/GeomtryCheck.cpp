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
 * @file GeometryCheck.cpp
 * @brief <b>CGeometry�N���X�`�F�b�N�֐������t�@�C��</b>\n
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "EnumSegment.h"
#include "GeometryRule.h"
#include "ErrorObject.h"
#include "util.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "AheMeshUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FAILED_TO_SIMPLIFY                = _T("Simplify�Ɏ��s���܂���");
	LPCTSTR FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE = _T("put_IsKnownSimple�Ɏ��s���܂���");
	LPCTSTR AREA_CANNOT_BE_EDITED             = _T("�ҏW�\�G���A�ł͂���܂���");
	LPCTSTR AREA_IS_LOCKED                    = _T("���b�N�G���A�ł�");
	LPCTSTR LIGHTNING_BOLT_SHAPE              = _T("��Ȍ`��[%.1lf�x-%.1lf�x�i���~�b�g�F30�x-30�x�j]");
	LPCTSTR PULSE_SHAPE_TURN                  = _T("�p���X/�s�p�^�[��[%.1lf�x�i���~�b�g�F%.1lf�x�`%.1lf�x�j]");
	LPCTSTR FAILED_TO_QUERY_WKSENVELOPE       = _T("ISegment::QueryWKSEnvelope�Ɏ��s");
	LPCTSTR SLIVER_SEGMENT                    = _T("�����Z�O�����g[�ʒu�F%d �����F%.3lfm �����F%.3lfm�ȏ�]");
#else
	LPCTSTR FAILED_TO_SIMPLIFY                = _T("Failed to simplify.");
	LPCTSTR FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE = _T("Failed to invoke put_IsKnownSimple.");
	LPCTSTR AREA_CANNOT_BE_EDITED             = _T("This is not an area that can be edited.");
	LPCTSTR AREA_IS_LOCKED                    = _T("This area is locked.");
	LPCTSTR LIGHTNING_BOLT_SHAPE              = _T("Lightning bolt shape [%.1lf degrees -%.1lf degrees (limit: 30 degrees -30degrees)])");
	LPCTSTR PULSE_SHAPE_TURN                  = _T("Pulse/sharp turn [%.1lf degrees (limit: %.1lf degrees to %.1lf degrees�j]");
	LPCTSTR FAILED_TO_QUERY_WKSENVELOPE       = _T("Failed to ISegment::QueryWKSEnvelope.");
	LPCTSTR SLIVER_SEGMENT                    = _T("Sliver segment [Position: %d, Distance: %.3lfm, Limit: %.3lfm or more]");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
using namespace errorcode;

long GetExteriorRingCount( IGeometry* ipGeom )
{
	long lRet = 0;
	IPolygonPtr ipPoly( ipGeom );
	if( ipPoly != NULL )
		ipPoly->get_ExteriorRingCount( &lRet );

	return lRet;
}

sindyErrCode CGeometry::CheckLogic( const CBasicLogicRule& cRule, CErrorInfos& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
{
	// [CHECK] �`�󂪑��݂��邩�ǂ���
	if( ! IsEmpty() )
	{
		sindyErrCode emErr = sindyErr_NoErr;
		switch( GetGeometryType() )
		{
			case esriGeometryPoint:
			case esriGeometryPolyline:
			{
				IPolylinePtr ipPolyline( (IGeometry*)(*this) );
				if( ipPolyline != NULL )
					ipPolyline->SimplifyNetwork(); // TODO: ����Ɍ`���ς���̂͂������Ȃ��̂�

				// [CHECK] �p�[�g���i�|�����C���A�|�C���g�j
				if( 1 < GetPartCount() )
					cErrInfos.push_back( CErrorInfo( sindyErr_GeometryPartTooMach, sindyErrLevel_ERR ) );

				// [CHECK] ���[�v�`�F�b�N
				if( cRule.DoCheckRoop() )
				{
					VARIANT_BOOL vbIsClosed = VARIANT_FALSE;
					if( SUCCEEDED( ((IPolylinePtr)(IGeometry*)(*this))->get_IsClosed( &vbIsClosed ) ) && vbIsClosed )
					{
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometryRooped, sindyErrLevel_ERR ) );
					}
				}
				break;
			}
			case esriGeometryPolygon:
			{
				ITopologicalOperator3Ptr ipTopo( (IGeometry*)(*this) );
				if( ipTopo != NULL )
				{
					if( SUCCEEDED( ipTopo->put_IsKnownSimple( VARIANT_FALSE ) ) )
					{
						// [CHECK] �V���v���`�F�b�N�i���̕��@�ł̓|���S���݂̂�����������Ă��Ȃ��j
						//         Simplify��������O�ɍs��Ȃ��ƁA�|���S���̎n�I�_�̂����Е��݂̂��ړ�����Ă�ꍇ�Ɍ��o�ł��Ȃ�
						VARIANT_BOOL vaIsSimple = VARIANT_FALSE;
						esriNonSimpleReasonEnum emReason = esriNonSimpleOK;
						if( SUCCEEDED( ipTopo->get_IsSimpleEx( &emReason, &vaIsSimple ) ) )
						{
							if( ! vaIsSimple )
							{
								switch( emReason )
								{
									case esriNonSimpleOK:                 emErr = sindyErr_NoErr;                               break;
									case esriNonSimpleRingOrientation:    emErr = sindyErr_GeometryNonSimpleRingOrientation;    break;
									case esriNonSimpleSegmentOrientation: emErr = sindyErr_GeometryNonSimpleSegmentOrientation; break;
									case esriNonSimpleShortSegments:      emErr = sindyErr_GeometryNonSimpleShortSegments;      break;
									case esriNonSimpleSelfIntersections:  emErr = sindyErr_GeometryNonSimpleSelfIntersections;  break;
									case esriNonSimpleUnclosedRing:       emErr = sindyErr_GeometryNonSimpleUnclosedRing;       break;
									case esriNonSimpleEmptyPart:          emErr = sindyErr_GeometryNonSimpleEmptyPart;          break;
									default:                              emErr = sindyErr_GeometryNonSimple;                   break;
								}
								if( sindyErr_NoErr != emErr )
									cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR ) );
							}
						}
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNonSimple, sindyErrLevel_ERR ) );

						if( SUCCEEDED( ipTopo->Simplify() ) ) // TODO: ����Ɍ`���ς���̂͂������Ȃ��̂�
						{
							// [CHECK] �G�N�X�e���A���i�|�����C���A�|�C���g�j
							if( 1 < GetExteriorRingCount((IGeometry*)*this) ) // <- GetExteriorRingCount�̓����o�Ƃ��邩�AAhe�n�֐��Ƃ��邱��
								cErrInfos.push_back( CErrorInfo( sindyErr_GeometryExteriorTooMach, sindyErrLevel_ERR ) );
						}
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAILED_TO_SIMPLIFY ) );
					}
					else
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE ) );
				}
				break;
			}
			default:
				cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNotSupported, sindyErrLevel_ERR ) );
				break;
		}

		// [CHECK] �ҏW�\�G���A���ǂ���
		emErr = g_cRule.IsEditableArea( cRule.GetTableName(), (IGeometry*)(*this) );
		if( ! ( emErr == sindyErr_NoErr || emErr == sindyErr_NoLogin ) )
			cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, AREA_CANNOT_BE_EDITED ) );

		// [CHECK] ���b�N�G���A���ǂ���
		emErr = g_cRule.IsLockArea( cRule.GetConnectUserName(), cRule.GetTableName(), (IGeometry*)(*this) );
		if( ! ( sindyErr_NoErr == emErr || sindyErr_NoLogin == emErr )  )
			cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, AREA_IS_LOCKED ) );

		// �Z�O�����g�P�ʂł̃`�F�b�N
		CheckSegments( cRule, cErrInfos, ipQuery );

		// �|�C���g�P�ʂł̃`�F�b�N
		CheckPoints( cRule, cErrInfos, ipQuery );
	}
	else
		cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNotFound, sindyErrLevel_ERR ) );

	return cErrInfos.GetDefaultErrCode();
}

// �Z�O�����g�n�`�F�b�N
sindyErrCode CGeometry::CheckSegments(const CBasicLogicRule& cRule, CErrorInfos& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
{
	using namespace sindy::schema;

	// �Z�O�����g�n�`�F�b�N�̓|�C���g�n�ȊO
	esriGeometryType emType = GetGeometryType();
	if( esriGeometryPoint == emType || esriGeometryMultipoint == emType )
		return cErrInfos.GetDefaultErrCode();

	for( const_part_iterator itPart = part_begin(); itPart != part_end(); ++itPart )
	{
		CGeometry cPart( *itPart );
		double dPrevDeg = -1.0;	// �O��̊p�x�i��Ȍ`��`�F�b�N�p�j
		const_segment_iterator it = cPart.segment_begin(ipQuery), it2 = cPart.segment_begin(ipQuery); ++it2;
		for( const_segment_iterator itEnd = cPart.segment_end(); it != itEnd; ++it, ++it2 )
		{
			_ISegmentPtr ipSegment;
			// it���ŏI�Z�O�����g�̏ꍇ�͌��݂̃p�[�g�̍ŏ����r�ΏۂƂ���
			// �������AipQuery���^�����Ă��邱�Ƃ��l�����AIsLastInPart��
			// ���Ɍ��肷��B
			if( it2 == itEnd )
			{
				if( it.IsLastInPart() ) // ipQuery�ɂ��ŏI�Z�O�����g�����Ȃ��ꍇ������
				{
					// [Bug 5937] �s�p�^�[���ɂȂ�Ȃ��͂��Ȃ̂Ɍx�����o��
					// ���C���n�͍ŏI�Z�O�����g�̃`�F�b�N���Ȃ��Ă����͂��B
					if ( emType == esriGeometryPolygon || emType == esriGeometryRing ) {
						// �Z�O�����g��������Ȃ��ꍇ�͔�΂�
						long lSegCount = 0;
						((ISegmentCollectionPtr)*itPart)->get_SegmentCount( &lSegCount );
						if( 1 < lSegCount )
							((ISegmentCollectionPtr)*itPart)->get_Segment( 0, &ipSegment );
					}
				}
			}
			else
				ipSegment = *it2;

			if( ipSegment )
			{
				// [CHECK] �Z�O�����g���Ȃ��p�x�`�F�b�N
				double dDeg = it.degree( (ILinePtr)ipSegment );
				if( cRule.GetMinDeg() <= dDeg && dDeg <= cRule.GetMaxDeg() )
				{
					// TODO �ȉ��A�S�����C������ʈ������������ɂȂ��Ă��邽�߁A�`��`�F�b�N���t�B�[�`���N���X�ʂɂ���Ȃǂ��đΉ�����ׂ�
					// [SJ�S���S�H���Ή�] �S�����C���ł͈�Ȍ`����쐬�ł���悤�ɂ��邽�߁A�`�F�b�N�͍s��Ȃ�
					if( esriGeometryPolyline == emType && lstrcmpi( sj::railway_link::kTableName, cRule.GetTableName() ) != 0)
					{
						// [CHECK] ��ȃ`�F�b�N�i�|�����C���̂݁j
						if( dPrevDeg <= 30 && dDeg <= 30 )
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_ERR, NULL, -1, -1, LIGHTNING_BOLT_SHAPE, dPrevDeg, dDeg ) ); // �p�x�G���[
					}
				}
				else
					cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_WARN, NULL, -1, -1, PULSE_SHAPE_TURN, dDeg, cRule.GetMinDeg(), cRule.GetMaxDeg() ) ); // �p�x�G���[// bug 8463 #11�Ōx���ɕύX
				dPrevDeg = dDeg;
			}

			if( /*bForRoadLink &&*/ it2.IsFirstInPart() )
			{
				// [CHECK] �J�n�E�I���̂Ȃ��p�ƃZ�O�����g�̋����`�F�b�N
				// �����Ă�
			}

			// [CHECK] �����Z�O�����g�`�F�b�N
			// TODO : �����Z�O�����g�̎����C���̋@�\���c�K�v�����B
			if( cRule.GetMinLen() > 0 )
			{
/*				static std::map<IUnknown*, std::map<double, double> > g_mapMeter;	// ���ꂼ��̋�ԎQ�Ƃł̋����ϊ��}�b�v

				double dMinLen = -1.0;	// ��ԎQ�Ɩ��̃��~�b�g����
				ISpatialReferencePtr ipSpRef;
				it->get_SpatialReference( &ipSpRef );
				bool bFind = false;
				std::map<IUnknown*, std::map<double, double> >::iterator itmapSpRef = g_mapMeter.find( (IUnknown*)(IUnknownPtr)ipSpRef );
				if( g_mapMeter.end() != itMapSpRef )
				{
					std::map<double, double>::iterator itmapCnv = itmapSpRef.find( cRule.GetMinLen() );
					if( itMapSpRef.end() != itMapCnv )
					{
						dMinLen = itMapCnv.second();
						bFind = true;
					}
				}
				if( ! bFind )
				{

				}*/
				WKSEnvelope env = {0};
				SUCCEEDED_IF( it->QueryWKSEnvelope( &env ), sindyErr_GeometryFunctionFailed, _T("%s"), FAILED_TO_QUERY_WKSENVELOPE )
				{
					if( cRule.GetMinLen() > AheGETDIST( env.XMin, env.YMin, env.XMax, env.YMax ) )
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentTooShort, sindyErrLevel_ERR, NULL, -1, -1, SLIVER_SEGMENT, it.index(), AheGETDIST( env.XMin, env.YMin, env.XMax, env.YMax ), cRule.GetMinLen() ) );	// �����Z�O�����g�G���[
				}
			}
		}
		// [CHECK] �ׂ荇��Ȃ��Z�O�����g���m�̐ڐG����
		if( cRule.DoCheckOneselfTouches() )
		{
			// �ēx�g�p���邽�߁A������
			it.Reset(); ++it;
			for( const_segment_iterator itEnd = cPart.segment_end(); it != itEnd; ++it )
			{
				// ���ȐڐG����́Aseg1�ɗׂ荇��Ȃ�seg2���Aseg1�ɐڐG���Ă���ꍇ�ɂɃG���[�Ƃ�����̂ł���i���Ȍ�����OK�j�B
				// ���݂̃A���S���Y���ł̓Z�O�����g�[�_���d�Ȃ��Ă���ꍇ���G���[�ƂȂ��Ă��܂��B
				// AheIsOnselfCross�ł�OK�����A���̂��߂ɖc��ȃR�[�h�������Ă���B���炭�G���[�ƂȂ��Ă����Ȃ��Ǝv����
				// �̂ŁA�Z�O�����g���m�̋����𑪂�A0�̏ꍇ��Polyline�ɕϊ�����IRelationalOperator::Touches�Ŕ��f����悤��
				// ����B
				// ���̃Z�O�����g���琔���Ď��̎�����
				// ���[�v���܂킵�ă`�F�b�N
				IProximityOperatorPtr ipProx( *it );
				IRelationalOperatorPtr ipRelSeg;
				// it2������
				HRESULT hr = ((IEnumSegment*)it2)->SetAt( it.part(), it.index() + 2 );
				if ( SUCCEEDED(hr) )
				{
					++it2;
					// �C���f�b�N�X�t�̎��ɁALastInPart�����������삵�Ȃ���������̂ŁB
					if ( it.index() + 2 == it2.index() )
					{
						for( ; it2 != cPart.segment_end(); ++it2 )
						{
							double dDistance = -1;
							if( SUCCEEDED( ipProx->ReturnDistance( *it2, &dDistance ) ) )
							{
								if( 0.0 == dDistance ) // 0���肵�Ă悢�H�H
								{
									if( ! ipRelSeg ) // TODO: �~�ʂ��l���Ȃ��ėǂ��̂ł���΁AIRelationalOperator���g��Ȃ��Ă�FROM�ATO�̃|�C���g����̋������݂邾����OK�ł́H�H
										ipRelSeg = ToPolyline( *it );
									// ������0�̏ꍇ��Polyline�ɕϊ����ăe�X�g
									VARIANT_BOOL vbTouches = VARIANT_FALSE;
									if( SUCCEEDED( ipRelSeg->Touches( ToPolyline( *it2 ), &vbTouches ) ) && vbTouches )
									{
										cErrInfos.push_back( CErrorInfo( sindyErr_GeometryOneselfTouches, sindyErrLevel_ERR ) );
									}
								}
							}
						}
					}
				}
			}
		}

		// [CHECK] �Œ�Z�O�����g��
		if( emType == esriGeometryPolyline ||
			emType == esriGeometryPolygon )
		{
			long lCount = 0;
			ISegmentCollectionPtr ipSegCol( (IGeometry*)*itPart );
			if( ipSegCol )
				ipSegCol->get_SegmentCount( &lCount );

			if( ( emType == esriGeometryPolyline && 1 > lCount ) ||
				( emType == esriGeometryPolygon  && 3 > lCount ) )
				cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentCountIsNotEnough, sindyErrLevel_ERR ) );	// �Z�O�����g������Ȃ�
		}
	}
	return cErrInfos.GetDefaultErrCode();
}

// �|�C���g�n�`�F�b�N
sindyErrCode CGeometry::CheckPoints( const CBasicLogicRule&/* cRule*/, CErrorInfos&/* cErrInfos*/, IEnvelope* /*ipQuery = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	// * @li �Œ�|�C���g���`�F�b�N�i�K�{�j
	// * @li �d���\���_�`�F�b�N�i�K�{�j

	return emErr;
}

} // sindy
