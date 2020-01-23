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
 * @file DispLine.cpp
 * @brief CDispLine�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 *
 * $Id$
 */
#include "stdafx.h"
#include "DispLine.h"
#include "util.h"
#include "AnnoGlobals.h"
#include "GlobalFunctions.h"
#include "AheWKSFunctions.h"
#include "../../WinLib/math2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
double g_dCalcStringBoundsTime = 0.0;
double g_dCalcFlagStringBoundsTime = 0.0;
double g_dCalcPositionTime = 0.0;
double g_dCalcParameterTime = 0.0;
double g_dQueryPointsTime = 0.0;

void CDispLine::TraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("CDispLine               : �v�����Ԍ��� :\n") );
	TRACEMESSAGE( _T("                        : CalcStringBounds     ������             : %.5lf�b\n"), g_dCalcFlagStringBoundsTime/1000.0 );
	TRACEMESSAGE( _T("                        : CalcPosition         ������             : %.5lf�b\n"), g_dCalcPositionTime/1000.0 );
	TRACEMESSAGE( _T("                        : CalcParameter        ������             : %.5lf�b\n"), g_dCalcParameterTime/1000.0 );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("                        :                      QueryWKSPoints     : %.5lf�b\n"), g_dQueryPointsTime/1000.0 );
	TRACEMESSAGE( _T("                        :                      AheCalcStringBounds: %.5lf�b\n"), g_dCalcStringBoundsTime/1000.0 );
	TRACEMESSAGE( _T("                          ======================================================\n"));
}
#endif // ifdef SINDYLIB_CALC_TIME

using namespace errorcode;

void CDispLine::Init()
{
	CRow::Init();
	m_lSecondLineStartIndex = -1;
	m_nStringRectsSize = -1;
	m_pStringRects = NULL;
	m_cRect.XMin = m_cRect.YMin = m_cRect.XMax = m_cRect.YMax = -1;
	m_emLineType = (sindyAnnotationLineType)-1;
	m_bIsFlag = false;
	m_bCanCopy = false;
}

void CDispLine::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	CRow::DeleteCache(bDeleteContainerCache);
	ClearParameterCache();
}

void CDispLine::ClearParameterCache()
{
	if( m_pStringRects )
	{
		delete [] m_pStringRects;
		m_pStringRects = NULL;
	}
}

// �p�����[�^�L���b�V�����Z�b�g����
void CDispLine::SetParameterCache( int nStringRectSize, long lSecondLineStartIndex, WKSPoint* pStringRects, const WKSEnvelope& cRect, IGeometry* ipGeom, sindyAnnotationLineType emLineType, bool bIsFlag, bool bCanCopy )
{
	m_nStringRectsSize = nStringRectSize;
	m_pStringRects = pStringRects;
	m_cRect = cRect;
	m_ipGeom = ipGeom;
	m_emLineType = emLineType;
	m_bIsFlag = bIsFlag;
	m_bCanCopy = bCanCopy;
	m_lSecondLineStartIndex = lSecondLineStartIndex;
}

// �R�s�[����
sindyErrCode CDispLine::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
	// CDispLine�ȊO�͒ʂ��Ȃ�
	CDispLine* pDest = dynamic_cast<CDispLine*>( &dest );
	if( ! pDest )
		return sindyErr_ArgIsConsistency;

	sindyErrCode emErr = CRow::CopyTo( dest, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	if( sindyErr_NoErr == emErr )
	{
		pDest->m_nStringRectsSize = m_nStringRectsSize;
		if( pDest->m_pStringRects )
			delete [] pDest->m_pStringRects;
		if( m_pStringRects )
		{
			pDest->m_pStringRects = new WKSPoint[m_nStringRectsSize];
			CopyMemory( pDest->m_pStringRects, m_pStringRects, sizeof(WKSPoint) );
		}
		else
			pDest->m_pStringRects = NULL;
		pDest->m_lSecondLineStartIndex = m_lSecondLineStartIndex;
		pDest->m_cRect = m_cRect;
		pDest->m_ipGeom = AheCreateClone( m_ipGeom );
		pDest->m_emLineType = m_emLineType;
		pDest->m_bIsFlag = m_bIsFlag;
		pDest->m_bCanCopy = m_bCanCopy;
	}
	return emErr;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// �`�F�b�N�֐�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// �t�H���g�T�C�Y�l���`�F�b�N����
sindyErrCode CDispLine::CheckFontSize( LPCTSTR lpcszNameString ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	switch( GetFontSize() )
	{
		// [bug 3768] �t�H���g�T�C�Y��10�̎��́A�������g�p����Ă��Ȃ��ꍇ�Ɍ���G���[�A�������g�p����Ă���Ȃ�x���Ƃ���
		case schema::disp_line::font_size::k10:
		{
			bool bHasKanji = false;							// �������g���Ă��邩�ǂ���

			USES_CONVERSION;

			// SJIS�Ŕ�r����̂ŁA�R�[�h�ϊ�
			LPCSTR lpcszNameStringA = T2CA(lpcszNameString);
			size_t nLen = strlen(lpcszNameStringA);
			for( size_t i = 0; i < nLen; ++i )
			{
				bool bIsWide = false;
				if( issjiswide( lpcszNameStringA[i], lpcszNameStringA[i+1] ) )
				{
					bIsWide = true;
					if( issjiskanji( lpcszNameStringA[i], lpcszNameStringA[i+1] ) )
					{
						bHasKanji = true;
						break;
					}
				}
				if( bIsWide ) ++i;
			}
			emErr = ( bHasKanji ) ? sindyErr_AnnoDispFontSizeInvalidKanji : sindyErr_AnnoDispFontSizeInvalidWarn;
		}
		break;
		case schema::disp_line::font_size::k12:
		case schema::disp_line::font_size::k14:
		case schema::disp_line::font_size::k16:
		case schema::disp_line::font_size::k20:
		case schema::disp_line::font_size::k24: break;
		default: emErr = sindyErr_AnnoDispFontSizeInvalid; break;
	}
	return emErr;
}

// �֘A���L�^�ʒuID���`�F�b�N����
sindyErrCode CDispLine::CheckAnnoID() const
{
	return ( GetAnnoID() < 0 ) ? sindyErr_AnnoDispRelIDInvalid : sindyErr_NoErr;
}

// �Q�ƕ�����R�[�h���`�F�b�N����
sindyErrCode CDispLine::CheckTargetStr() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	switch( GetTargetStr() )
	{
		case schema::disp_line::target_str::kStr1:
		case schema::disp_line::target_str::kStr2: break;
		case schema::disp_line::target_str::kStr3: emErr = ( GetTableType() == sindyTableType::city_annotation ) ? sindyErr_AnnoDispTargetStrInvalid : sindyErr_NoErr; break;
		default: emErr = sindyErr_AnnoDispTargetStrInvalid; break;
	}
	return emErr;
}

// �\�����C���̊p�x���`�F�b�N����
sindyErrCode CDispLine::CheckAngle( sindyAnnotationLineType emLineType, double* dErrAngle ) const // TODO: ����������Ȃ����
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( emLineType & sindyAnnoLine_Linear )	// �`�F�b�N����̂͐��󒍋L�̂�
	{
		LOGASSERTEERR_IF( GetShape(), sindyErr_GeometryNotCached )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)(*GetShape()))->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
			{
				long lCount = 0;
				ipCol->get_PointCount( &lCount );
				WKSPoint* pPoints = new WKSPoint[lCount];
				LOGASSERTEERR_IF( SUCCEEDED( ipCol->QueryWKSPoints( 0, lCount, pPoints ) ), sindyErr_GeometryFunctionFailed )
				{
					for( long i = 0; i < lCount - 3; ++i )	// 3�_�̊p�x���`�F�b�N����̂�4�_�i��������3�j�ȏ�
					{
						double angle = rad2angle( WKS3PAngle( pPoints[i], pPoints[i+1], pPoints[i+2] ) );
						if( ! ( (double)135.0 <= angle && angle <= (double)225.0 ) )
						{
							emErr = sindyErr_AnnoStrAngleInvalid;
							if( dErrAngle ) *dErrAngle = angle;
							break;
						}
					}

				}
				delete [] pPoints;
			}
		}
	}

	return emErr;
}

// �\�����C���`��̐��������`�F�b�N����
sindyErrCode CDispLine::CheckLine( schema::annotation_class::disptype_rule::ECode emDispTypeRule ) const
{
	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

	// ���C���^�C�v���[���ƃ��C���^�C�v�̐����`�F�b�N�ibug 6537�j
	switch( emDispTypeRule )
	{
		case schema::annotation_class::disptype_rule::kLinear:
			if( sindyAnnoLine_Linear != m_emLineType )
				emErr = sindyErr_AnnoDispLineTypeInvalid; // ���[�����u����v�Ȃ̂Ƀ��C���^�C�v���u����v�ł͂Ȃ�
			break;
		default:
			if( sindyAnnoLine_Linear == m_emLineType )
					emErr = sindyErr_AnnoDispLineTypeInvalid; // ���[�����u����v����Ȃ��̂Ƀ��C���^�C�v���u����v
			break;
	}
	// 2�s���L�̏ꍇ�Ɉꕶ���������݂��Ȃ��s�����邩�ǂ����̃`�F�b�N�ibug 6537�j
	// �܂�Ԃ�����X���������ł��邩�ǂ����̃`�F�b�N�ibug 6537�j
	if( (sindyAnnotationLineType)(sindyAnnoLine_Double|sindyAnnoLine_Horizonal) == m_emLineType )
	{
		if( m_lSecondLineStartIndex == 1 ||                     // ��s�ڂ��ꕶ�������Ȃ�
			m_nStringRectsSize - m_lSecondLineStartIndex == 1 ) // ��s�ڂ��ꕶ�������Ȃ�
			emErr = sindyErr_AnnoDispLineIllegalDouble; // �ꕶ���������݂��Ȃ��s������
		else {
			if( GetShape()->GetPoint(m_lSecondLineStartIndex).GetX() - 
				GetShape()->GetPoint( m_lSecondLineStartIndex - 1).GetX() >= 0 )
				emErr = sindyErr_AnnoDispLineIllegalDouble; // �܂�Ԃ�������0�ȏ�
		}
	}

	return emErr;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���̑��̊֐�
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// 1pt������̒������擾����
double CDispLine::Calc1PTLen( bool bCalcX ) const
{
	switch( GetTableType() )
	{
	case sindyTableType::city_disp_line: return ( bCalcX ) ? ANNOTATION_1PT_CITY_X : ANNOTATION_1PT_CITY_Y; break;
	case sindyTableType::c_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_CSC1_X : ANNOTATION_1PT_CSC1_Y; break;
	case sindyTableType::b_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC4_X : ANNOTATION_1PT_BSC4_Y; break;
	case sindyTableType::b_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC3_X : ANNOTATION_1PT_BSC3_Y; break;
	case sindyTableType::b_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC2_X : ANNOTATION_1PT_BSC2_Y; break;
	case sindyTableType::b_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC1_X : ANNOTATION_1PT_BSC1_Y; break;
	case sindyTableType::m_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC4_X : ANNOTATION_1PT_MSC4_Y; break;
	case sindyTableType::m_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC3_X : ANNOTATION_1PT_MSC3_Y; break;
	case sindyTableType::m_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC2_X : ANNOTATION_1PT_MSC2_Y; break;
	case sindyTableType::m_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC1_X : ANNOTATION_1PT_MSC1_Y; break;
	case sindyTableType::t_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC4_X : ANNOTATION_1PT_TSC4_Y; break;
	case sindyTableType::t_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC3_X : ANNOTATION_1PT_TSC3_Y; break;
	case sindyTableType::t_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC2_X : ANNOTATION_1PT_TSC2_Y; break;
	case sindyTableType::t_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC1_X : ANNOTATION_1PT_TSC1_Y; break;
	default:
		SASSERT(false);
		ERRORLOG(sindyErr_InvalidTypeInfo);	// �^��񂪈�v���Ȃ�
	}
	return 0.0;
}

// �\�������O�`���v�Z����
errorcode::sindyErrCode CDispLine::CalcStringBounds( IPoint* pAnnoPoint, IGeometry* ipBound ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	IPointCollectionPtr ipCol( ipBound );

	if( NULL != ipCol && NULL != pAnnoPoint )
	{
		VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
		if( SUCCEEDED( ((IRelationalOperatorPtr)ipBound)->Disjoint( pAnnoPoint, &vbDisjoint ) ) )
		{
			if( vbDisjoint )
			{
				const CGeometry* pGeom = GetShape();
				if( pGeom )
				{
					// �^�ʒu���W�擾
					WKSPoint pb;
					pAnnoPoint->QueryCoords( &pb.X, &pb.Y );

					// �_����擾
					long lCount = 0;
					ipCol->get_PointCount( &lCount );
					WKSPoint* pPoints = new WKSPoint[lCount];
					ipCol->QueryWKSPoints( 0, lCount, pPoints );


					// �����o�������쐬����̂ɓK�����Z�O�����g��������
					double dAngleMax = 0.0; // �e�Z�O�����g��From�ATo�Ɛ^�ʒu�̊p�x�̍��̍ő�l
					long lIndexMax = -1;	// ��L���L�^��������i�̒l

					for( long i = 0; i < lCount - 1; ++i )
					{
						bool bIsCross = false;

						long j = i + 1;
						while( j != i )
						{
							long jn = ( j < lCount - 1 ) ? j+1 : 0; // j�̎��̒l

							// �e�Z�O�����g��From�ATo�Ɛ^�ʒu�����Ԑ�����ipBound�ƌ������Ȃ����̂̂ݏ���
							if( WKSIsCross( pb, pPoints[i],   pPoints[j], pPoints[jn], false ) ||
								WKSIsCross( pb, pPoints[i+1], pPoints[j], pPoints[jn], false ) )
							{
								bIsCross = true;
								break;
							}
							// �J�E���^����
							j = ( j < lCount - 1 ) ? ++j : j=0;
						}
						if( ! bIsCross )
						{
							// �e�Z�O�����g��From�ATo�Ɛ^�ʒu�̊p�x�̍������A��ԑ傫�Ȃ��̂��̗p����
							double dAngle = WKS3PAngle( pPoints[i], pb, pPoints[i+1] );
							if( dAngleMax < dAngle ) 
							{
								lIndexMax = i;
								dAngleMax = dAngle;
							}
						}
					}

					if( lIndexMax < 0 )
					{
						emErr = sindyErr_AlgorithmFailed; // �A���S���Y���G���[
						SASSERT( lIndexMax >= 0 );
						lIndexMax = 0;	// �G���[�̏ꍇ�͂��傤���Ȃ��̂�0�Ԗڂ��g�p
					}
					
					// �����o���̂��߂�2�_���쐬����
					// �̑��Z�O�����g�̒��_���瓙�����ɂ���2�_���v�Z
					WKSPoint pc, pc1, pc2;
					pc.X = (pPoints[lIndexMax].X+pPoints[lIndexMax+1].X)/(double)2.0;
					pc.Y = (pPoints[lIndexMax].Y+pPoints[lIndexMax+1].Y)/(double)2.0;
					
					// �̑��Z�O�����g�̋����̔����ƁA���߂�_�ƒ��_�̋����̔䗦���o��
					double dRatio = (Calc1PTLen(true)*(double)3.0)/(WKS2PLength(pPoints[lIndexMax],pPoints[lIndexMax+1])/(double)2.0);

					pc1.X = dRatio*pPoints[lIndexMax+1].X + ((double)1.0-dRatio)*pc.X;
					pc1.Y = dRatio*pPoints[lIndexMax+1].Y + ((double)1.0-dRatio)*pc.Y;
					pc2.X = dRatio*pPoints[lIndexMax].X + ((double)1.0-dRatio)*pc.X;
					pc2.Y = dRatio*pPoints[lIndexMax].Y + ((double)1.0-dRatio)*pc.Y;
					
					// �\���_���č쐬����
		/*			WKSPoint pPointsNew[3];
					pPointsNew[0] = pc2; pPointsNew[1] = pb; pPointsNew[2] = pc1;
					ipCol->InsertWKSPoints( lIndexMax, 3, pPointsNew );
					((IPolygonPtr)ipCol)->SimplifyPreserveFromTo();*/
					WKSPoint* pPointsNew = new WKSPoint[lCount*3];
					for( long i = 0; i < lCount+3; ++i )
					{
						if( i <= lIndexMax )
							pPointsNew[i] = pPoints[i];
						else if( i == lIndexMax + 1 )
							pPointsNew[i] = pc2;
						else if( i == lIndexMax + 2 )
							pPointsNew[i] = pb;
						else if( i == lIndexMax + 3 )
							pPointsNew[i] = pc1;
						else
							pPointsNew[i] = pPoints[i-3];
					}
					ipCol->RemovePoints( 0, lCount );
					ipCol->SetWKSPoints( lCount+3, pPointsNew );
					((IPolygonPtr)ipCol)->SimplifyPreserveFromTo();
					// ��n��
					delete [] pPointsNew;
					delete [] pPoints;
				}
				else {
					emErr = sindyErr_GeometryNotCached;
					ERRORLOG(emErr);
					SASSERT(pGeom);
				}
			}
		}
		else {
			emErr = sindyErr_GeometryFunctionFailed;
			ERRORLOG(emErr);
			SASSERT(false);
		}
	}
	else {
		emErr = sindyErr_COMInterfaceIsNull;
		ERRORLOG(emErr);
		SASSERT(NULL != ipCol && NULL != pAnnoPoint);
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCalcFlagStringBoundsTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emErr;
}

// �n���ꂽ�|�C���g�ɑ΂��ĕ\���ʒu���c���E���̂ǂ̈ʒu�ɂ��邩�𔻒肷��
sindyAnnotationPosition CDispLine::CalcPosition( const CPoint& cPoint, sindyAnnotationLineType emLineType, const WKSEnvelope& cRect ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyAnnotationPosition emPos = sindyAnnoPos_Other;	// �Ԃ�l

	if( emLineType != -1 )
	{
		// �E���A�����̔���
		if( cRect.XMin > cPoint.GetX() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_RightHorizonal;
		}
		else if( cRect.XMax < cPoint.GetX() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_LeftHorizonal;
		}
		// ��c���A���c���̔���i�A���S���Y���ケ���炪�D��ɂȂ�j
		if( cRect.YMin > cPoint.GetY() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_UpperHorizonal;
			else if( emLineType | sindyAnnoLine_Vertical )
				emPos = sindyAnnoPos_UpperVertical;
		}
		else if( cRect.YMax < cPoint.GetY() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_LowerHorizonal;
			else if( emLineType | sindyAnnoLine_Vertical )
				emPos = sindyAnnoPos_LowerVertical;
		}
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCalcPositionTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emPos;
}

/*// ���L������̊O�`�|���S�����擾����
IGeometryPtr CDispLine::CalcStringBounds( long nLen, double** dEnvs ) const
{
	IPointCollectionPtr ipGeomCol( CLSID_Polygon );
	//IPointCollectionPtr ipEnv( CLSID_Ring );
	WKSPoint* pEnv = new WKSPoint[nLen*5];
	for( long i = 0; i < nLen; ++i )
	{
		pEnv[i*5].X   = dEnvs[i][0]; pEnv[i*5].Y   = dEnvs[i][1];
		pEnv[i*5+1].X = dEnvs[i][0]; pEnv[i*5+1].Y = dEnvs[i][3];
		pEnv[i*5+2].X = dEnvs[i][2]; pEnv[i*5+2].Y = dEnvs[i][1];
		pEnv[i*5+3].X = dEnvs[i][2]; pEnv[i*5+3].Y = dEnvs[i][3];
		pEnv[i*5+4].X = dEnvs[i][0]; pEnv[i*5+4].Y = dEnvs[i][1];
	}
	ipGeomCol->SetWKSPoints( nLen*5, pEnv );
	delete [] pEnv;

	// ConvexHull���O�`�Ƃ���
	IGeometryPtr ipOutGeom;
	if( SUCCEEDED( ((IPolygonPtr)ipGeomCol)->SimplifyPreserveFromTo() ) )
	{
		if( FAILED( ((ITopologicalOperatorPtr)ipGeomCol)->ConvexHull( &ipOutGeom ) ) )
			ERRORLOG(sindyErr_GeometryFunctionFailed);
	}
	else
		ERRORLOG(sindyErr_GeometryFunctionFailed);

	return ipOutGeom;
}*/

// �e��p�����[�^���v�Z����
sindyAnnotationLineType CDispLine::CalcParameter( bool bIsHalfString, long* pSecondLineStartIndex/* = NULL*/, WKSPoint* pStringRects/* = NULL*/, WKSEnvelope* pRect/* = NULL*/, IGeometry** ipStringBounds/* = NULL*/ ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyAnnotationLineType emLineType = (sindyAnnotationLineType)-1;	// �Ԃ�l

	IGeometryPtr ipShape = (IGeometryPtr)(IGeometry*)(*GetShape());
	IPointCollectionPtr ipCol( ipShape );
	WKSPoint* pPoints = NULL;					// �|�C���g��i�[�p�i�������ׁ̈j
	long lStringCount = 0;						// �|�C���g���i�[�p�i�ŏI�_�͊܂܂Ȃ��j
	WKSPoint* pStringBounds = NULL;				// �X�̕�����`�i�[�p
	long lSecondLineStartIndex = 0;				// ��s���L�̏ꍇ�̊J�n�C���f�b�N�X�i�[�p

/*	// ������
	if( pRect ) *pRect = WKSRect{ -1, -1, -1, -1 }; <- �J�n�_�ŏ��������邩�炢��Ȃ��񂶂��H*/

#ifdef SINDYLIB_CALC_TIME
	double dStartQueryPoints = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	if( ipCol != NULL )
	{
		ipCol->get_PointCount( &lStringCount );
		lStringCount--;	// �ŏI�_�͊܂߂Ȃ�
		pPoints = new WKSPoint[lStringCount];
		ipCol->QueryWKSPoints( 0, lStringCount, pPoints );

		if( ipStringBounds != NULL )
			pStringBounds = new WKSPoint[lStringCount*5];

	}

#ifdef SINDYLIB_CALC_TIME
	g_dQueryPointsTime += (double)GetTickCount() - dStartQueryPoints;
#endif // ifdef SINDYLIB_CALC_TIME

	if( pPoints )
	{
		double dStrLenX = ( GetFontSize() * Calc1PTLen( true ) )/(double)( ( bIsHalfString ) ? 2 : 1 );	// �ꕶ��������̕�
		double dStrLenY = GetFontSize() * Calc1PTLen( false );											// �ꕶ��������̍���
		double dX = 0, dY = 0, dXPrev = 0, dYPrev = 0;
		int nChangeX = 0, nChangeY = 0;				// �ύX�`�F�b�N�p
//		int nRow = 1;							// �s��

		// �ŏI�_�̑O�̓_�܂ł̍��W���`�F�b�N
		for( long i = 0; i < lStringCount; ++i )
		{
			// �X�̕������`�쐬
			if( ipStringBounds != NULL )
			{
				pStringBounds[i*5].X   = pPoints[i].X;            pStringBounds[i*5].Y   = pPoints[i].Y;
				pStringBounds[i*5+1].X = pPoints[i].X;            pStringBounds[i*5+1].Y = pPoints[i].Y + dStrLenY;
				pStringBounds[i*5+2].X = pPoints[i].X + dStrLenX; pStringBounds[i*5+2].Y = pPoints[i].Y + dStrLenY;
				pStringBounds[i*5+3].X = pPoints[i].X + dStrLenX; pStringBounds[i*5+3].Y = pPoints[i].Y;
				pStringBounds[i*5+4].X = pPoints[i].X;            pStringBounds[i*5+4].Y = pPoints[i].Y;
			}

			// �J�n�_���i�[
			if( 0 == i )
			{
				dX = dXPrev = pPoints[i].X;
				dY = dYPrev = pPoints[i].Y;

				if( pRect )
				{
					pRect->XMin = pPoints[i].X;
					pRect->YMin = pPoints[i].Y;
					pRect->XMax = pPoints[i].X;
					pRect->YMax = pPoints[i].Y;
				}
			}
			else {
				// �ő�E�ŏ�����
				if( pRect )
				{
					pRect->XMin = min( pRect->XMin, dX );
					pRect->YMin = min( pRect->YMin, dY );
					pRect->XMax = max( pRect->XMax, dX );
					pRect->YMax = max( pRect->YMax, dY );
				}

				if( dXPrev != pPoints[i].X )
					++nChangeX;
				if( dYPrev != pPoints[i].Y )
				{
					++nChangeY;
					lSecondLineStartIndex = i; // ��s���L�Ɣ��f���ꂽ�ꍇ�͂��ꂪpSecondLineStartIndex�ɔ��f�����
				}
				dXPrev = pPoints[i].X;
				dYPrev = pPoints[i].Y;
			}
		}
		// ��s��s�A�c�����`�F�b�N
		// �ꕶ�������Ȃ��ꍇ�͈�s��
		switch( lStringCount )
		{
			// �ꕶ�������Ȃ��ꍇ�͈�s��
			case 1: emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single); break;
			// �񕶎������Ȃ��ꍇ�͈�s�c���A����
			case 2:
				if( 2 == nChangeX && 2 == nChangeY )
					emLineType = sindyAnnoLine_Linear;
				else if( 1 == nChangeY )
					emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single);
				else
					emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Vertical|sindyAnnoLine_Single);
				break;
			// �O�����ȏ�
			default:
				// Y������2��ȏ㊎��X������1��ȏ�ς���Ă��������
				if( 1 < nChangeY && 0 < nChangeX )
					emLineType = sindyAnnoLine_Linear;
				else {
					if( 0 == nChangeY )
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single);
					else if( 1 == nChangeY )
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Double);
					else
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Vertical|sindyAnnoLine_Single);
				}
				break;
		}
		// �_��͂�������Ȃ��̂ō폜
		delete [] pPoints;
		pPoints = NULL;

		if( pStringRects )
			CopyMemory( pStringRects, pStringBounds, sizeof(WKSPoint)*(lStringCount*5) );

		if( pSecondLineStartIndex )
			*pSecondLineStartIndex = ( emLineType & sindyAnnoLine_Double ) ? lSecondLineStartIndex : 0;

#ifdef SINDYLIB_CALC_TIME
		double dStartStringBounds = GetTickCount();
#endif // SINDYLIB_CALC_TIME

		// �o�E���f�B���O�{�b�N�X�쐬
		if( ipStringBounds )
		{
			SASSERT( *ipStringBounds == NULL );

			IGeometryPtr ipStringBoundsTmp = AheCalcStringBounds( pStringBounds, lStringCount, emLineType, lSecondLineStartIndex, GetShape()->GetSpatialReference() );
			if( ipStringBoundsTmp != NULL ) { *ipStringBounds = ipStringBoundsTmp; (*ipStringBounds)->AddRef(); }
		}

#ifdef SINDYLIB_CALC_TIME
		g_dCalcStringBoundsTime += (double)GetTickCount() - dStartStringBounds;
#endif // ifdef SINDYLIB_CALC_TIME

	}
	// ��n��
	delete [] pStringBounds;
	pStringBounds = NULL;

#ifdef SINDYLIB_CALC_TIME
	g_dCalcParameterTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emLineType;
}

// �\���ł���X�P�[���̍ő�l���擾����
sindyAnnotationScale CDispLine::GetMaxScale() const
{
	return AheTableType2AnnotationScale( GetTableType() );
}

} // sindy
