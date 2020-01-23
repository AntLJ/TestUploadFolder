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
 * @file AnnoGlobals.cpp
 * @brief ���L�n�O���֐�����
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "AnnoGlobals.h"
#include "util.h"
#include "GlobalFunctions.h"
#include "meshutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;
//using namespace city_annotation;
//using namespace annotation_class;

#ifdef SINDYLIB_CALC_TIME
double g_dAheCalcStringBounds_OneLineTime = 0.0;	//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����
double g_dAheCalcStringBounds_OneLine1Time = 0.0;	//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍���ԁi�O���j
double g_dAheCalcStringBounds_OneLine2Time = 0.0;	//!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍���ԁi�㔼�j
double g_dAheCalcStringBounds_DoubleLineTime = 0.0; //!< ��s���L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����
double g_dAheCalcStringBounds_LinerLineTime = 0.0;	//!< ���󒍋L�̃o�E���f�B���O�{�b�N�X�̍쐬�ɂ����鑍����

void AheCalcStringBoundsTraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("AheCalcStringBounds     : �v�����Ԍ��� :\n") );
	TRACEMESSAGE( _T("                        : ��s���L             ������             : %.5lf�b\n"), g_dAheCalcStringBounds_OneLineTime/1000.0 );
	TRACEMESSAGE( _T("                        :                        �O��               : %.5lf�b\n"), g_dAheCalcStringBounds_OneLine1Time/1000.0 );
	TRACEMESSAGE( _T("                        :                        �㔼               : %.5lf�b\n"), g_dAheCalcStringBounds_OneLine2Time/1000.0 );
	TRACEMESSAGE( _T("                        : ��s���L             ������             : %.5lf�b\n"), g_dAheCalcStringBounds_DoubleLineTime/1000.0 );
	TRACEMESSAGE( _T("                        : ���󒍋L             ������             : %.5lf�b\n"), g_dAheCalcStringBounds_LinerLineTime/1000.0 );
	TRACEMESSAGE( _T("                          ======================================================\n"));
}
void AheCalcStringBoundsClearTime()
{
	g_dAheCalcStringBounds_OneLineTime = 0.0;
	g_dAheCalcStringBounds_OneLine1Time = 0.0;
	g_dAheCalcStringBounds_OneLine2Time = 0.0;
	g_dAheCalcStringBounds_DoubleLineTime = 0.0;
	g_dAheCalcStringBounds_LinerLineTime = 0.0;
}

#endif // ifdef SINDYLIB_CALC_TIME

// ���L��ʂ��璍�L��ʕ\�������擾����
LPCTSTR AheAnnoCode2CodeName( schema::annotation::annotation_code::ECode emCode )
{
	switch( emCode )
	{
#define ANNOTATION_CODE(name,id,size,label) case id: return annotation_code_label::name; break;
#include <sindy/schema/annotationcode.h>
#undef ANNOTATION_CODE
		default: break;
	}
	return NULL;
}

// �e�[�u���^�C�v����X�P�[���R�[�h�ɕϊ�����
sindyAnnotationScale AheTableType2AnnotationScale( sindyTableType::ECode emCode )
{
	switch( emCode )
	{
	case sindyTableType::city_disp_line: return sindyAnnoScale_city; break;
	case sindyTableType::c_sc1disp_line: return sindyAnnoScale_csc1; break;
	case sindyTableType::b_sc4disp_line: return sindyAnnoScale_bsc4; break;
	case sindyTableType::b_sc3disp_line: return sindyAnnoScale_bsc3; break;
	case sindyTableType::b_sc2disp_line: return sindyAnnoScale_bsc2; break;
	case sindyTableType::b_sc1disp_line: return sindyAnnoScale_bsc1; break;
	case sindyTableType::m_sc4disp_line: return sindyAnnoScale_msc4; break;
	case sindyTableType::m_sc3disp_line: return sindyAnnoScale_msc3; break;
	case sindyTableType::m_sc2disp_line: return sindyAnnoScale_msc2; break;
	case sindyTableType::m_sc1disp_line: return sindyAnnoScale_msc1; break;
	case sindyTableType::t_sc4disp_line: return sindyAnnoScale_tsc4; break;
	case sindyTableType::t_sc3disp_line: return sindyAnnoScale_tsc3; break;
	case sindyTableType::t_sc2disp_line: return sindyAnnoScale_tsc2; break;
	case sindyTableType::t_sc1disp_line: return sindyAnnoScale_tsc1; break;
	default:
		ERRORLOG(sindyErr_UnknownTableType);
		SASSERT(false);
		return (sindyAnnotationScale)-1;
	}
}

// �e�[�u���^�C�v���璍�L�O���[�v�ɕϊ�����
sindyAnnotationGroup AheTableType2AnnotationGroup( sindyTableType::ECode emCode )
{
	switch( emCode )
	{
	case sindyTableType::city_disp_line:
	case sindyTableType::city_annotation: return sindyAnnoGroup_city; break;
	case sindyTableType::c_sc1disp_line:
	case sindyTableType::d1_annotation: return sindyAnnoGroup_csc1; break; // 1/6250
	case sindyTableType::b_sc4disp_line:
	case sindyTableType::b_sc3disp_line:
	case sindyTableType::b_sc2disp_line:
	case sindyTableType::b_sc1disp_line:
	case sindyTableType::base_annotation: return sindyAnnoGroup_base; break;
	case sindyTableType::m_sc4disp_line:
	case sindyTableType::m_sc3disp_line:
	case sindyTableType::m_sc2disp_line:
	case sindyTableType::m_sc1disp_line:
	case sindyTableType::middle_annotation: return sindyAnnoGroup_middle; break;
	case sindyTableType::t_sc4disp_line:
	case sindyTableType::t_sc3disp_line:
	case sindyTableType::t_sc2disp_line:
	case sindyTableType::t_sc1disp_line:
	case sindyTableType::top_annotation: return sindyAnnoGroup_top; break;
	default:
		ERRORLOG(sindyErr_UnknownTableType);
		SASSERT(false);
		return (sindyAnnotationGroup)-1;
	}
}

// �e�[�u���^�C�v����^�ʒu�e�[�u���^�C�v���擾����
sindyTableType::ECode AheTableType2AnnoPointTableType( sindyTableType::ECode emTableType )
{
	switch( emTableType )
	{
	case sindyTableType::city_disp_line:
	case sindyTableType::city_annotation: return sindyTableType::city_annotation;   break;
	case sindyTableType::c_sc1disp_line:
	case sindyTableType::d1_annotation: return sindyTableType::d1_annotation;     break;
	case sindyTableType::b_sc4disp_line:
	case sindyTableType::b_sc3disp_line:
	case sindyTableType::b_sc2disp_line:
	case sindyTableType::b_sc1disp_line:
	case sindyTableType::base_annotation: return sindyTableType::base_annotation;   break;
	case sindyTableType::m_sc4disp_line:
	case sindyTableType::m_sc3disp_line:
	case sindyTableType::m_sc2disp_line:
	case sindyTableType::m_sc1disp_line:
	case sindyTableType::middle_annotation: return sindyTableType::middle_annotation; break;
	case sindyTableType::t_sc4disp_line:
	case sindyTableType::t_sc3disp_line:
	case sindyTableType::t_sc2disp_line:
	case sindyTableType::t_sc1disp_line:
	case sindyTableType::top_annotation: return sindyTableType::top_annotation;    break;
	default:
		ERRORLOG(sindyErr_UnknownTableType);
		SASSERT(false);
		return sindyTableType::unknown;
	}
}

// �\���ʒu�e�[�u���^�C�v����X�P�[���^�C�v������ɕϊ�����
CString AheTableType2ScaleTypeString( sindyTableType::ECode emDispTableType )
{
	CString strMsg;
	switch( emDispTableType )
	{
		case sindyTableType::b_sc4disp_line:
		case sindyTableType::m_sc4disp_line:
		case sindyTableType::t_sc4disp_line: strMsg = _T("S4"); break;
		case sindyTableType::b_sc3disp_line:
		case sindyTableType::m_sc3disp_line:
		case sindyTableType::t_sc3disp_line: strMsg = _T("S3"); break;
		case sindyTableType::b_sc2disp_line:
		case sindyTableType::m_sc2disp_line:
		case sindyTableType::t_sc2disp_line: strMsg = _T("S2"); break;
		case sindyTableType::b_sc1disp_line:
		case sindyTableType::m_sc1disp_line:
		case sindyTableType::t_sc1disp_line: strMsg = _T("S1"); break;
		default: break;
	}
	return strMsg;
}

// �O���[�v���l�����ă��b�V���R�[�h����`�ɕϊ�����
IEnvelopePtr AheMeshCode2Envelope( long lMeshCode, sindyAnnotationGroup emGroup, ISpatialReference* ipSpRef )
{
	// ���b�V���R�[�h�ƃ��b�V���^�C�v�̐������`�F�b�N
	// ���b�V���R�[�h�̌������擾
	TCHAR strMeshCode[10] = _T("");
	_ltot( lMeshCode, strMeshCode, 10 );
	long lFig = lstrlen(strMeshCode);
	// �����ϊ�
	switch( emGroup )
	{
		case sindyAnnoGroup_city:
		case sindyAnnoGroup_csc1:
			if(      8 != lFig )  	lMeshCode = -1; // �s�s�n�}���b�V���R�[�h�ł͂Ȃ�
			break;
		case sindyAnnoGroup_base:
			if(      8 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 6 == lFig )	;
			else    				lMeshCode = -1; // �s�s�n�}�E2�����b�V���R�[�h�ł͂Ȃ�
			break;
		case sindyAnnoGroup_middle:
			if(      8 == lFig )   	lMeshCode = (long)(lMeshCode / 10000);
			else if( 6 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 4 == lFig )	;
			else             		lMeshCode = -1; // �s�s�n�}�E2���E1�����b�V���R�[�h�ł͂Ȃ�
			break;
		case sindyAnnoGroup_top:
			if(      8 == lFig )  	lMeshCode = (long)(lMeshCode / 1000000);
			else if( 6 == lFig )	lMeshCode = (long)(lMeshCode / 10000);
			else if( 4 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 2 == lFig )    ;
			else             		lMeshCode = -1; // �s�s�n�}�E2���E1���E�g�b�v���b�V���R�[�h�ł͂Ȃ�
			break;
		default:                    lMeshCode = -1;
			break;
	}
	if( 0 > lMeshCode )
		return NULL;

	// ���b�V���R�[�h���G���A�ɕϊ�
	// �s�s�n�}�A�x�[�X�͂��̂܂܂ŗǂ����A�~�h����
	// 1��x4�Ȃ̂ŁA�G���A��ϊ�����
	// �g�b�v�͂����
	IEnvelopePtr ipEnv( MESH2ENV( lMeshCode, ipSpRef ) );
	switch( emGroup )
	{
		case sindyAnnoGroup_middle:
		{
			// ���b�V���R�[�h���ꌅ�ڂ������Ȃ�X������++�A��Ȃ�X������--
			// ���b�V���R�[�h��񌅖ڂ������Ȃ�Y������++�A��Ȃ�Y������--
			long lMeshCode2 = ( 1 == lMeshCode % 2 )        ? DECMESH_X(lMeshCode ) : INCMESH_X(lMeshCode );
			lMeshCode2 = ( 1 == (int)(lMeshCode2/100) % 2 ) ? INCMESH_Y(lMeshCode2) : DECMESH_Y(lMeshCode2);
			IEnvelopePtr ipEnv2( MESH2ENV( lMeshCode2, ipSpRef ) );
			ipEnv->Union( ipEnv2 );	// �E��̃��b�V���Ƃ�UNION�����
			break;
		}
		case sindyAnnoGroup_top:
			ipEnv = NULL; // top�}�b�v�͌v�Z������ŁA���������܂��鉿�l���Ȃ��̂Ŗ����B
			break;
		default:
			break;
	}
	_ASSERTE( ipEnv );

	return ipEnv;
}

// �n���ꂽ�e�[�u���^�C�v���^�ʒu�e�[�u���^�C�v���ǂ�������������
bool AheIsAnnoPointTableType(sindyTableType::ECode emTableType)
{
	switch( emTableType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:
		case sindyTableType::base_annotation:
		case sindyTableType::middle_annotation:
		case sindyTableType::top_annotation:
			return true;
		default:
			break;
	}
	return false;
}

// �n���ꂽ�e�[�u���^�C�v���\���ʒu�e�[�u���^�C�v���ǂ�������������
bool AheIsAnnoLineTableType(sindyTableType::ECode emTableType)
{
	switch( emTableType )
	{
		case sindyTableType::city_disp_line:
		case sindyTableType::c_sc1disp_line:
		case sindyTableType::b_sc4disp_line:
		case sindyTableType::b_sc3disp_line:
		case sindyTableType::b_sc2disp_line:
		case sindyTableType::b_sc1disp_line:
		case sindyTableType::m_sc4disp_line:
		case sindyTableType::m_sc3disp_line:
		case sindyTableType::m_sc2disp_line:
		case sindyTableType::m_sc1disp_line:
		case sindyTableType::t_sc4disp_line:
		case sindyTableType::t_sc3disp_line:
		case sindyTableType::t_sc2disp_line:
		case sindyTableType::t_sc1disp_line:
			return true;
		default:
			break;
	}
	return false;
}

// ���L��������`�F�b�N�E�C������
sindyErrCode AheCheckOrFixAnnotationString( LPCTSTR lpcszString, long lStringLen, schema::annotation::annotation_code::ECode emAnnoCode, bool bIsCity/* = true*/, bool bIsString3/* = false*/, bool bFix/* = false*/, CString* pstrFixString/* = NULL*/ )
{
	USES_CONVERSION;

	// �����`�F�b�N
	if( ( bFix   && pstrFixString == NULL ) ||
		( ! bFix && pstrFixString != NULL ) )
		return sindyErr_ArgIsConsistency;					// bFix��pstrFixString�̓y�A
/*	if( lStringLen < 0 ) 
		return sindyErr_ArgLimitOver;		// �����񒷂����͂���*/ // <- �����񒷂�NULL�̏ꍇ��-1������̂ł����ŃG���[�ɂ��Ȃ�

	if( 0 >= lStringLen && ! lpcszString ) return sindyErr_NoErr;	// ������NULL�y�ѕ����񒷂�0�ȉ��̎��́A�����̒l��NULL�̎��Ȃ̂ŁA�����OK�ŕԂ�

	CString strString( lpcszString );					// ���L������
	long lRealLen = CString_GetLength2( strString );	// ���ۂ̕�����

/*	// ���ۂ̕����񒷂ƒ��L�������Ƃ̔�r
	if( lRealLen != lStringLen )
		return sindyErr_AnnoStrLenNotSame; <- ���L�������͈ꕶ��1�o�C�g�Ƃ��Čv�Z����̂ŁA�����Ń`�F�b�N���Ă̓_���I�I*/
	long lStrCount = 0;

	// �R�x���̂ƎR�x�W���l����ʂ��邽�߁A�s�s�n�}�ł͂Ȃ�������R�ł��Ȃ��ꍇ��kMountainRange�Ƃ���
	// �`�F�b�N��ʂ�
	// kMountainRange�ɓ��ʂȃ��[����ݒ肷��ꍇ�́A�ʂ̎�ʂɂ��邱�ƁI�I
	// �ȉ��AkMountain�Ƃ��ă`�F�b�N���Ă���Ƃ��͑S�ĎR�x�W���l�Ƃ��Ẵ`�F�b�N�ƂȂ�
	if( emAnnoCode == schema::annotation::annotation_code::kMountain )
	{
		if( bIsCity || ! bIsString3 )
			emAnnoCode = schema::annotation::annotation_code::kMountainRange;
	}

	// [rule_check_1_2_1_1E] ����������`�F�b�N
//	long lFigure = 0;			//!< �R�x�̏ꍇ�̐������̌���
	bool bFindPeriod = false;	//!< ���p�s���I�h�����ꂽ���ǂ���
	for( long i = 0; i < lRealLen; ++i )
	{
		// �S��SJIS�ɒ����Ĕ�r
		CString str = CString_GetAt2( strString, i );
		LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

		switch( emAnnoCode )
		{
			// �R�x�̏ꍇ
			case schema::annotation::annotation_code::kMountain:
/*				// [rule_check_1_2_1_4E] �R�x�̏ꍇ�ibIsSingle=true����bUsePeriod=true�j�͐������̌�����3�ȉ��ł���K�v������
				if( ! bFindPeriod )
				{
					++lFigure;	// �������̌������J�E���g
					if( lFigure >= 3 )
						return sindyErr_AnnoStrFigureOverFlow;
				}
				if( isasciiperiod( lpstr[0] ) ) bFindPeriod = true;	// ���p�s���I�h����������t���O�𗧂ĂĂ���
*/
				// [rule_check_1_2_1_15E] �R�x�̏ꍇ�͏����_���ʂ܂ł͑��݂���K�v������
				if( isasciiperiod( lpstr[0] ) )
				{
					if( lRealLen - 1 > i )	// �s���I�h�ŏI����Ă��Ȃ����ǂ����̃`�F�b�N
						bFindPeriod = true;
					else
						return sindyErr_AnnoStrMountainInvalid;
				} 
				else {
					if( bFindPeriod && isasciiperiod( lpstr[0] ) ) // �s���I�h���P��ł��邩�ǂ����̃`�F�b�N
						return sindyErr_AnnoStrMountainInvalid;
				}
			// �����ԍ��E��v�n�����ԍ��̏ꍇ
			case schema::annotation::annotation_code::kNationalRouteNumber:
			case schema::annotation::annotation_code::kMainPrefRouteNumber:
				// ���p���l�̂�OK�i�R�x�̏ꍇ�̓s���I�h��OK�j
				if( ! ( isasciinumber( lpstr[0] ) || ( emAnnoCode == schema::annotation::annotation_code::kMountain && isasciiperiod( lpstr[0] ) ) ) )
					return sindyErr_AnnoStrIsNotHalfNumber;
				// �擪�����p0�ȊO�̂�OK
				if( i == 0 && isasciizero( lpstr[0] ) )
					return sindyErr_AnnoStrFistIsHalfZero;

				// ������J�E���^�𑝉�
				++lStrCount;

				break;
			// �S�p�̏ꍇ�i��L�ȊO�j
			default:
				if (
				( issjiskanji(              lpstr[0], lpstr[1] ) ) ||	// �S�p����
				( issjiswidehiragana(       lpstr[0], lpstr[1] ) ) ||	// �S�p�Ђ炪��
				( issjiswidekatakana(       lpstr[0], lpstr[1] ) ) ||	// �S�p�J�^�J�i
				( issjiswidenumber(         lpstr[0], lpstr[1] ) ) ||	// �u�O�v�`�u�X�v
				( issjiswidealphabet(       lpstr[0], lpstr[1] ) ) ||	// �u���v�`�u�y�v
				( issjiswidegreek(          lpstr[0], lpstr[1] ) ) ||	// �S�p�M���V�A����
				( issjiswidekigou(          lpstr[0], lpstr[1] ) ) ||	// �S�p���p�\����L���i���̑��j
				// �ȉ��͈�ԏ��߂ɂ����Ă͂����Ȃ��H�i���L�̋K���́H�j
				( issjiswideperiod(         lpstr[0], lpstr[1] ) ) ||	// �u�D�v
				( issjiswidenyoro(          lpstr[0], lpstr[1] ) ) ||	// �u�`�v
				( issjiswidenun(            lpstr[0], lpstr[1] ) ) ||	// �u�X�v
				( issjiswideshime(          lpstr[0], lpstr[1] ) ) ||	// �u�Y�v
				( issjiswidenakaguro(       lpstr[0], lpstr[1] ) ) ||	// �u�E�v
				( issjiswidesmallkakko(     lpstr[0], lpstr[1] ) ) ||	// �u�i�v
				( issjiswidesmalltojikakko( lpstr[0], lpstr[1] ) ) ||	// �u�j�v
				( issjiswidechouon(         lpstr[0], lpstr[1] ) ) ||	// �u�[�v
				( issjiswidekanma(          lpstr[0], lpstr[1] ) ) ||	// �u�A�v
				( issjiswideminus(          lpstr[0], lpstr[1] ) ) 		// �u�|�v
				){
#if 0 // [bug 3035] ����Ȃ�
					// [rule_check_1_2_1_9E] �S�p0�͐擪�ɗ��Ă͂����Ȃ�
					if( i == 0 && 0x82 == (unsigned char)((lpstr[0])&0xff) && 0x4f == (unsigned char)((lpstr[1])&0xff) )
						return sindyErr_AnnoStrFirstIsWideZero;
					// [rule_check_1_2_1_14E] �S�p�����L���͑S�p�Ђ炪�ȁE�S�p�J�^�J�i�̌����i�n�C�t���̃`�F�b�N�͂ǂ�������悢���킩��Ȃ��c�j
					if( issjiswidechouon( lpstr[0], lpstr[1] ) )
					{
						CString strPrev = CString_GetAt2( strString, i-1 );
						LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
						if( ! ( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) || issjiswidehiragana( lpstrPrev[0], lpstrPrev[1] ) ) )
							return sindyErr_AnnoStrInvalidChouon;
					}
#endif // if 0
/*					// �S�p�}�C�i�X�͐����̌����
					else if( issjiswideminus( lpstr[0], lpstr[1] ) )
					{
						CString strPrev = strText.GetAt2(i-1);
						LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
						if( issjiswidenumber( lpstrPrev[0], lpstrPrev[1] ) != TRUE )
							continue;
					}
					*/
				}
				else
					return sindyErr_AnnoStrIsNotWide;

				// ������J�E���^�𑝉�
				lStrCount += 2;

				break;
		}
	}

	// ���L�������Ǝ��ۂ̕������������Ă��邩�ǂ����`�F�b�N
	if( lStrCount != lStringLen ) return sindyErr_AnnoStrLenNotSame;

#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
	// �����P�ʂł̃`�F�b�N�ł͖ʓ|�Ȃ��̂��ȉ��ōs��
	CString strLast2, strLast3;
	strLast2.Format(_T("%s%s"), CString_GetAt2( strString, lRealLen-2 ), CString_GetAt2( strString, lRealLen-1 ) );
	strLast3.Format(_T("%s%s"), CString_GetAt2( strString, lRealLen-3 ), strLast2 );
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK

	switch( emAnnoCode )
	{
#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
		// [rule_check_1_2_1_5E] �C���^�[�`�F���W�̏ꍇ�͕�����Ōオ�u�h�b�v�A�������񐔂�4�ȏ�
		case annotation_code::kOtherHighwayIC:
		case annotation_code::kHighwayIC:
		case annotation_code::kNationalTollIC:
		case annotation_code::kDrivewayIC:
		case annotation_code::kMainTollIC:
		case annotation_code::kMainDrivewayIC:
		case annotation_code::kPrefTollIC:
		case annotation_code::kPrefDrivewayIC:
		case annotation_code::kOtherTollIC:
		case annotation_code::kOtherDrivewayIC:
			if( strLast2.Compare(_T("�h�b")) == 0 )
			{
				if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
			}
			else
				return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			break;
		// [rule_check_1_2_1_6E] �s�s�n�}���L�ȊO�̏ꍇ�A�W�����N�V�����͕�����Ōオ�u�i�b�v�A�������񐔂�4�ȏ�
		// [rule_check_1_2_1_11E] �s�s�n�}���L�̏ꍇ�A�W�����N�V�����͕�����Ōオ�u�i�b�s�v�łȂ���΂Ȃ�Ȃ�
		case annotation_code::kHighwayCityHighwayJC:
		case annotation_code::kHighwayJC:
		case annotation_code::kCityHighwayJC:
		case annotation_code::kNationalTollJC:
		case annotation_code::kNationalDrivewayJC:
		case annotation_code::kMainTollJC:
		case annotation_code::kMainDrivewayJC:
		case annotation_code::kPrefTollJC:
		case annotation_code::kPrefDrivewayJC:
		case annotation_code::kOtherTollJC:
		case annotation_code::kOtherDrivewayJC:
			// �s�s�n�}���L�̏ꍇ
			if( bIsCity )
			{
				if( strLast3.Compare(_T("�i�b�s")) != 0 )
					return sindyErr_AnnoStrJCButLast3IsNotJCT;
			}
			// �s�s�n�}���L�ȊO�̏ꍇ
			else {
				if( strLast2.Compare(_T("�i�b")) == 0 ) 
				{
					if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
				}
				else
					return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			}
			break;
		// [rule_check_1_2_1_7E] �t�F���[�^�[�~�i���̏ꍇ�͕�����Ōオ�u�e�s�v�A�������񐔂�4�ȏ�
		case annotation_code::kFerryTerminal:
			if( strLast2.Compare(_T("�e�s")) == 0 )
			{
				if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
			}
			else
				return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			break;
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK

		// [rule_check_1_2_1_12E] ��v�n�����ԍ��̏ꍇ�͊���1�`151�܂Łi�\���^�C�v�̃`�F�b�N�͑��ł��j
		case schema::annotation::annotation_code::kMainPrefRouteNumber:
			if( ! strString.IsEmpty() )
			{
				// ���l�ɕϊ����Ē��ׂ�
				long lNumber = _ttol( strString );
				if( ! ( 1 <= lNumber && lNumber <= 151 ) )
					return sindyErr_AnnoStrRouteNumberOverFlow;
			}
			break;
		// [rule_check_1_2_1_13E] �����ԍ��̏ꍇ��1�`58�A101�`108�A112�`507
		case schema::annotation::annotation_code::kNationalRouteNumber:
			if( ! strString.IsEmpty() )
			{
				// ���l�ɕϊ����Ē��ׂ�
				long lNumber = _ttol( strString );
				if( !(  ( 1   <= lNumber && lNumber <= 58  ) || 
						( 101 <= lNumber && lNumber <= 108 ) || 
						( 112 <= lNumber && lNumber <= 507 ) ) )
					return sindyErr_AnnoStrRouteNumberOverFlow;
			}
			break;
		// [rule_check_1_2_1_15E] �R�x�̏ꍇ�͏����_���ʂ܂ł͑��݂���K�v������
		case schema::annotation::annotation_code::kMountain:
			if( ! bFindPeriod )	
				return sindyErr_AnnoStrMountainInvalid;
			break;
		default: break;
	}

	return sindyErr_NoErr;
}

// �\���ʒu���C���^�C�v����1�|�C���g������̋������Z�o����
double AheCalcDispLineType21PTLen( sindyTableType::ECode emTableType, bool bCalcX )
{
	switch( emTableType )
	{
		case sindyTableType::init_disp_point:
		case sindyTableType::city_disp_line:
		case sindyTableType::city_disp_point: return ( bCalcX ) ? ANNOTATION_1PT_CITY_X : ANNOTATION_1PT_CITY_Y;
		case sindyTableType::c_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_CSC1_X : ANNOTATION_1PT_CSC1_Y; // 2006.08.10 F.Abe
		case sindyTableType::base_disp_point:
		case sindyTableType::b_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_BSC4_X : ANNOTATION_1PT_BSC4_Y;
		case sindyTableType::b_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_BSC3_X : ANNOTATION_1PT_BSC3_Y;
		case sindyTableType::b_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_BSC2_X : ANNOTATION_1PT_BSC2_Y;
		case sindyTableType::b_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_BSC1_X : ANNOTATION_1PT_BSC1_Y;
		case sindyTableType::m_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_MSC4_X : ANNOTATION_1PT_MSC4_Y;
		case sindyTableType::m_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_MSC3_X : ANNOTATION_1PT_MSC3_Y;
		case sindyTableType::m_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_MSC2_X : ANNOTATION_1PT_MSC2_Y;
		case sindyTableType::m_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_MSC1_X : ANNOTATION_1PT_MSC1_Y;
		case sindyTableType::t_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_TSC4_X : ANNOTATION_1PT_TSC4_Y;
		case sindyTableType::t_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_TSC3_X : ANNOTATION_1PT_TSC3_Y;
		case sindyTableType::t_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_TSC2_X : ANNOTATION_1PT_TSC2_Y;
		case sindyTableType::t_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PT_TSC1_X : ANNOTATION_1PT_TSC1_Y;
		default:
			ERRORLOG(sindyErr_ArgLimitOver);
			SASSERT(false);
	}
	return 0.0;
}

// �\���ʒu���C���^�C�v����1�s�N�Z��������̋������Z�o����
double AheCalcDispLineType21PXLen( sindyTableType::ECode emTableType, bool bCalcX )
{
	switch( emTableType )
	{
		case sindyTableType::city_disp_line:
		case sindyTableType::city_disp_point: return ( bCalcX ) ? ANNOTATION_1PX_CITY_X : ANNOTATION_1PX_CITY_Y;
		case sindyTableType::c_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_CSC1_X : ANNOTATION_1PX_CSC1_Y; // 2006.08.10 F.Abe
		case sindyTableType::base_disp_point:
		case sindyTableType::b_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_BSC4_X : ANNOTATION_1PX_BSC4_Y;
		case sindyTableType::b_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_BSC3_X : ANNOTATION_1PX_BSC3_Y;
		case sindyTableType::b_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_BSC2_X : ANNOTATION_1PX_BSC2_Y;
		case sindyTableType::b_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_BSC1_X : ANNOTATION_1PX_BSC1_Y;
		case sindyTableType::m_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_MSC4_X : ANNOTATION_1PX_MSC4_Y;
		case sindyTableType::m_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_MSC3_X : ANNOTATION_1PX_MSC3_Y;
		case sindyTableType::m_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_MSC2_X : ANNOTATION_1PX_MSC2_Y;
		case sindyTableType::m_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_MSC1_X : ANNOTATION_1PX_MSC1_Y;
		case sindyTableType::t_sc4disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_TSC4_X : ANNOTATION_1PX_TSC4_Y;
		case sindyTableType::t_sc3disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_TSC3_X : ANNOTATION_1PX_TSC3_Y;
		case sindyTableType::t_sc2disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_TSC2_X : ANNOTATION_1PX_TSC2_Y;
		case sindyTableType::t_sc1disp_line:  return ( bCalcX ) ? ANNOTATION_1PX_TSC1_X : ANNOTATION_1PX_TSC1_Y;
		default:
			ERRORLOG(sindyErr_ArgLimitOver);
			SASSERT(false);
	}
	return 0.0;
}

// �X�̕������`����S�̂̃o�E���f�B���O�{�b�N�X���v�Z����
IGeometryPtr AheCalcStringBounds( const WKSPoint* pStringsBounds, long lStringCount, sindyAnnotationLineType emLineType, long lSecondLineStartIndex, ISpatialReference* ipSpRef )
{
	IGeometryPtr ipBound;	// �Ԃ�l

	// �����`�F�b�N
	LOGASSERTE_IF( lStringCount > 0 && lSecondLineStartIndex >= 0 && lStringCount > lSecondLineStartIndex, sindyErr_ArgLimitOver )
	{
#ifdef SINDYLIB_CALC_TIME
		double dStartOneLine = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

		// ��s���L�̏ꍇ�͒P����ConvexHull���o�E���f�B���O�{�b�N�X�Ƃ���
		if( emLineType & sindyAnnoLine_Single )
		{
#ifdef SINDYLIB_CALC_TIME
			double dStartOneLine_1 = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			double dMaxX, dMaxY, dMinX, dMinY;
			dMaxX = dMinX = pStringsBounds[0].X;
			dMaxY = dMinY = pStringsBounds[0].Y;
			for( long i = 1; i < lStringCount*5; ++i )
			{
				dMaxX = max( dMaxX, pStringsBounds[i].X );
				dMinX = min( dMinX, pStringsBounds[i].X );
				dMaxY = max( dMaxY, pStringsBounds[i].Y );
				dMinY = min( dMinY, pStringsBounds[i].Y );
			}
#ifdef SINDYLIB_CALC_TIME
			g_dAheCalcStringBounds_OneLine1Time += (double)GetTickCount() - dStartOneLine_1;
			double dStartOneLine_2 = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			LOGASSERT_IF( SUCCEEDED( ipBound.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
			{
				ipBound->putref_SpatialReference( ipSpRef );
				WKSPoint* pPoints = new WKSPoint[5];
				pPoints[0].X = pPoints[1].X = pPoints[4].X = dMinX;
				pPoints[0].Y = pPoints[3].Y = pPoints[4].Y = dMinY;
				pPoints[2].X = pPoints[3].X = dMaxX;
				pPoints[1].Y = pPoints[2].Y = dMaxY;
				((IPointCollectionPtr)ipBound)->SetWKSPoints( 5, const_cast<WKSPoint*>(pPoints) );
				delete [] pPoints;	
			}
#ifdef SINDYLIB_CALC_TIME
			g_dAheCalcStringBounds_OneLine2Time += (double)GetTickCount() - dStartOneLine_2;
			g_dAheCalcStringBounds_OneLineTime += (double)GetTickCount() - dStartOneLine;
#endif // ifdef SINDYLIB_CALC_TIME
		}
		// ��s���L�̏ꍇ�͈�s�ڂƓ�s�ڂ�OR���o�E���f�B���O�{�b�N�X�Ƃ���
		else if( emLineType & sindyAnnoLine_Double )
		{
#ifdef SINDYLIB_CALC_TIME
			double dStartDoubleLine = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			if( SUCCEEDED( ipBound.CreateInstance( CLSID_Polygon ) ) )
			{
				ipBound->putref_SpatialReference( ipSpRef );
				IPointCollectionPtr ipCol1, ipCol2;
/*				IGeometryPtr ipBound1, ipBound2;
				if( SUCCEEDED( ipCol1.CreateInstance( CLSID_Polygon ) ) && SUCCEEDED( ipCol2.CreateInstance( CLSID_Polygon ) ) )
				{
					if( SUCCEEDED( ipBound1.CreateInstance( CLSID_Polygon ) ) )
					{
						ipCol1->SetWKSPoints( lSecondLineStartIndex*5, const_cast<WKSPoint*>(pStringsBounds) );
						((IPolygonPtr)ipCol1)->SimplifyPreserveFromTo();
						((ITopologicalOperatorPtr)ipCol1)->ConvexHull( &ipBound1 );
					}
					else {
						ERRORLOG(sindyErr_COMCreateInstanceFailed);
						SASSERT(ipBound1 != NULL );
					}
					if( SUCCEEDED( ipBound2.CreateInstance( CLSID_Polygon ) ) )
					{
						ipCol2->SetWKSPoints( (lStringCount-lSecondLineStartIndex)*5, const_cast<WKSPoint*>(&(pStringsBounds[lSecondLineStartIndex*5])) );
						((IPolygonPtr)ipCol2)->SimplifyPreserveFromTo();
						((ITopologicalOperatorPtr)ipCol2)->ConvexHull( &ipBound2 );
					}
					else {
						ERRORLOG(sindyErr_COMCreateInstanceFailed);
						SASSERT( ipBound2 != NULL );
					}
					ISegmentCollectionPtr ipCol( ipBound );
					if( ipCol != NULL )
					{
						ipCol->AddSegmentCollection( (ISegmentCollectionPtr)ipBound1 );
						ipCol->AddSegmentCollection( (ISegmentCollectionPtr)ipBound2 );
						((IPolygonPtr)ipBound)->SimplifyPreserveFromTo();
					}
				}
				else {
					ERRORLOG(sindyErr_COMCreateInstanceFailed);
					SASSERT( ipCol1 != NULL && ipCol2 != NULL );
				}*/
				// ��s�ڂ̍ő�ŏ����o��
				double dMaxX1 = 0.0, dMaxY1 = 0.0, dMinX1 = 0.0, dMinY1 = 0.0;
				for( long i = 0; i < lSecondLineStartIndex*5; ++i )
				{
					if( i == 0 )
					{
						dMaxX1 = dMinX1 = pStringsBounds[i].X;
						dMaxY1 = dMinY1 = pStringsBounds[i].Y;
					}
					else {
						dMaxX1 = max( dMaxX1, pStringsBounds[i].X );
						dMinX1 = min( dMinX1, pStringsBounds[i].X );
						dMaxY1 = max( dMaxY1, pStringsBounds[i].Y );
						dMinY1 = min( dMinY1, pStringsBounds[i].Y );
					}
				}
				// ��s�ڂ̍ő�ŏ����o��
				double dMaxX2 = 0.0, dMaxY2 = 0.0, dMinX2 = 0.0, dMinY2 = 0.0;
				for( long i = lSecondLineStartIndex*5; i < lStringCount*5; ++i )
				{
					if( i == lSecondLineStartIndex*5 )
					{
						dMaxX2 = dMinX2 = pStringsBounds[i].X;
						dMaxY2 = dMinY2 = pStringsBounds[i].Y;
					}
					else {
						dMaxX2 = max( dMaxX2, pStringsBounds[i].X );
						dMinX2 = min( dMinX2, pStringsBounds[i].X );
						dMaxY2 = max( dMaxY2, pStringsBounds[i].Y );
						dMinY2 = min( dMinY2, pStringsBounds[i].Y );
					}
				}
				// ���Ԃ̍ő�E�ŏ����o��
				double dMaxXC = 0.0, dMaxYC = 0.0, dMinXC = 0.0, dMinYC = 0.0;
				std::list<double> listDouble;
				listDouble.push_back( dMaxX1 );
				listDouble.push_back( dMaxX2 );
				listDouble.push_back( dMinX1 );
				listDouble.push_back( dMinX2 );
				listDouble.sort();
				std::list<double>::const_iterator it = listDouble.begin();
				dMinXC = *(++it);
				dMaxXC = *(++it);
				listDouble.clear();
				listDouble.push_back( dMaxY1 );
				listDouble.push_back( dMaxY2 );
				listDouble.push_back( dMinY1 );
				listDouble.push_back( dMinY2 );
				listDouble.sort();
				it = listDouble.begin();
				dMinYC = *(++it);
				dMaxYC = *(++it);
				// 3�̋�`��UNION�����
				IPointCollectionPtr ipCol, ipColTmp;
				IGeometryPtr ipUnion;
				LOGASSERT_IF( SUCCEEDED( ipCol.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
				{
					((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
					WKSPoint* pPoints = new WKSPoint[5];
					pPoints[0].X = pPoints[1].X = pPoints[4].X = dMinX1;
					pPoints[0].Y = pPoints[3].Y = pPoints[4].Y = dMinY1;
					pPoints[2].X = pPoints[3].X = dMaxX1;
					pPoints[1].Y = pPoints[2].Y = dMaxY1;
					LOGASSERT_IF( 
						SUCCEEDED( ipCol->SetWKSPoints( 5, const_cast<WKSPoint*>(pPoints) ) ) && 
						SUCCEEDED( ((IPolygonPtr)ipCol)->SimplifyPreserveFromTo() ) &&
						SUCCEEDED( ipColTmp.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
					{
						((IGeometryPtr)ipColTmp)->putref_SpatialReference( ipSpRef );
						pPoints[0].X = pPoints[1].X = pPoints[4].X = dMinXC;
						pPoints[0].Y = pPoints[3].Y = pPoints[4].Y = dMinYC;
						pPoints[2].X = pPoints[3].X = dMaxXC;
						pPoints[1].Y = pPoints[2].Y = dMaxYC;
/*						LOGASSERT_IF( 
							SUCCEEDED( ipColTmp->SetWKSPoints( 5, const_cast<WKSPoint*>(pPoints) ) ) &&
							SUCCEEDED( ((IPolygonPtr)ipColTmp)->SimplifyPreserveFromTo() ), sindyErr_GeometryFunctionFailed )
						{
							ipColTmp->QueryInterface( IID_IGeometry, (void**)&ipBound );
						}*/
						
						LOGASSERT_IF( 
							SUCCEEDED( ipColTmp->SetWKSPoints( 5, const_cast<WKSPoint*>(pPoints) ) ) &&
							SUCCEEDED( ((IPolygonPtr)ipColTmp)->SimplifyPreserveFromTo() ) &&
							SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->Union( (IGeometryPtr)ipColTmp, &ipUnion ) ), sindyErr_GeometryFunctionFailed )
						{
							ipCol = ipUnion;
							pPoints[0].X = pPoints[1].X = pPoints[4].X = dMinX2;
							pPoints[0].Y = pPoints[3].Y = pPoints[4].Y = dMinY2;
							pPoints[2].X = pPoints[3].X = dMaxX2;
							pPoints[1].Y = pPoints[2].Y = dMaxY2;
							LOGASSERT_IF(
								SUCCEEDED( ipColTmp->SetWKSPoints( 5, const_cast<WKSPoint*>(pPoints) ) ) &&
								SUCCEEDED( ((IPolygonPtr)ipColTmp)->SimplifyPreserveFromTo() ), sindyErr_GeometryFunctionFailed )
							{
								if( FAILED( ((ITopologicalOperatorPtr)ipCol)->Union( (IGeometryPtr)ipColTmp, &ipBound ) ) )
								{
									ERRORLOG(sindyErr_GeometryFunctionFailed);
									SASSERT(false);
								}
							}
						}
					}
					delete [] pPoints;	
				}
			}
#ifdef SINDYLIB_CALC_TIME
			g_dAheCalcStringBounds_DoubleLineTime += (double)GetTickCount() - dStartDoubleLine;
#endif // ifdef SINDYLIB_CALC_TIME
		}
		// ���󒍋L�̏ꍇ�͑O���g��ConvexHull
		else if( emLineType & sindyAnnoLine_Linear )
		{
#ifdef SINDYLIB_CALC_TIME
			double dStartLinerLine = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			IPointCollectionPtr ipCol;
			IGeometryPtr ipBoundTmp;
#if 0 // ��������`2��UNION����ConvexHull�������@���Ɠ���̌`�󂪏����ł��Ȃ��ꍇ������
			if( SUCCEEDED( ipCol.CreateInstance( CLSID_Polygon ) ) )
			{
				((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
				for( long i = 0; i < lStringCount-1; ++i )
				{
					if( FAILED( ipCol->SetWKSPoints( 10, const_cast<WKSPoint*>(&(pStringsBounds[i*5])) ) ) ||	// �_����Z�b�g
						FAILED( ((IPolygonPtr)ipCol)->SimplifyPreserveFromTo() ) ||								// ConvexHull����ɂ͕K�v
						FAILED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ) ) 				// ConvelHull
					{
						// ��L�̕��@�͏��X�蔲�������A�X�̃|���S����UNION�����葁���i�͂��j
						// �ł��A���ꂼ��̃{�b�N�X���d�Ȃ��Ă���ƃG���[���N����
						// ���̏ꍇ�͌X�̃|���S���ɂ��Ă���UNION���āA�����ConvexHull����
#ifdef _DEBUG
						for( long j = i*5; j < i*5+10; ++j )	ATLTRACE(_T("���󒍋LBOX : %3d : %.20lf %.20lf\n"), j - i*5, pStringsBounds[j].X, pStringsBounds[j].Y );
#endif // ifdef _DEBUG
						IPointCollectionPtr ipColTmp1, ipColTmp2;
						LOGASSERTE_IF( SUCCEEDED( ipColTmp1.CreateInstance( CLSID_Polygon ) ) &&
										  SUCCEEDED( ipColTmp2.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
						{
							((IGeometryPtr)ipColTmp1)->putref_SpatialReference( ipSpRef );
							((IGeometryPtr)ipColTmp2)->putref_SpatialReference( ipSpRef );
							ITopologicalOperatorPtr ipTopo;
							LOGASSERTE_IF( SUCCEEDED( ipColTmp1->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
							{
								IGeometryPtr ipGeomTmp;
								LOGASSERTE_IF( SUCCEEDED( ipTopo->Union( (IGeometryPtr)ipColTmp2, &ipGeomTmp ) ), sindyErr_GeometryFunctionFailed )
								{
									LOGASSERTE_IF( SUCCEEDED( ipGeomTmp->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
									{
										LOGASSERTE_IF( SUCCEEDED( ipTopo->Simplify() ) &&
													   SUCCEEDED( ipTopo->ConvexHull( &ipBoundTmp ) ), sindyErr_GeometryFunctionFailed );
									}
								}
							}
						}
/*						for( long j = i*5; j < i*5+10; ++j )	ATLTRACE(_T("���󒍋LBOX : %3d : %.20lf %.20lf\n"), j - i*5, pStringsBounds[j].X, pStringsBounds[j].Y );
						ERRORLOG(sindyErr_GeometryFunctionFailed);
						SASSERT(false);*/
					}
					else {
						// UNION�����
						// �{����ConstructUnion�������Ƃ��낾���A�G���[���o��ꍇ������̂�
						// �X��UNION�����悤�ɂ���
						if( ipBound == NULL )
							ipBound = AheCreateClone( ipBoundTmp );
						else {
							IGeometryPtr ipUnion;
							if( SUCCEEDED( ((ITopologicalOperatorPtr)ipBound)->Union( ipBoundTmp, &ipUnion ) ) )
								ipBound = ipUnion;
							else {
								ERRORLOG(sindyErr_GeometryFunctionFailed);
								SASSERT(false);
							}
						}
					}
				}
			}
#else // if 0
			LOGASSERTE_IF( SUCCEEDED( ipCol.CreateInstance( CLSID_Multipoint ) ), sindyErr_COMCreateInstanceFailed )
			{
				((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
				// ������݂̂�1�����̏ꍇ�����邽�ߏꍇ�킯
				if( lStringCount == 1 )
				{
						LOGASSERTE_IF( SUCCEEDED( ipCol->SetWKSPoints( 5, const_cast<WKSPoint*>(&(pStringsBounds[0])) ) ) &&						// �_����Z�b�g
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->Simplify() )	&&								// ConvexHull����ɂ͕K�v
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ), sindyErr_GeometryFunctionFailed ) // ConvelHull
						{
								ipBound = AheCreateClone( ipBoundTmp );
						}

				}
				else
				{
					for( long i = 0; i < lStringCount-1; ++i )
					{
						LOGASSERTE_IF( SUCCEEDED( ipCol->SetWKSPoints( 10, const_cast<WKSPoint*>(&(pStringsBounds[i*5])) ) ) &&						// �_����Z�b�g
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->Simplify() ) &&												// ConvexHull����ɂ͕K�v
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ), sindyErr_GeometryFunctionFailed ) // ConvelHull
						{
							// UNION�����
							// �{����ConstructUnion�������Ƃ��낾���A�G���[���o��ꍇ������̂�
							// �X��UNION�����悤�ɂ���
							if( ipBound == NULL )
								ipBound = AheCreateClone( ipBoundTmp );
							else {
								IGeometryPtr ipUnion;
								LOGASSERTE_IF( SUCCEEDED( ((ITopologicalOperatorPtr)ipBound)->Union( ipBoundTmp, &ipUnion ) ), sindyErr_GeometryFunctionFailed )
									ipBound = ipUnion;
							}
						}
					}
				}
			}
#endif // if 0
#ifdef SINDYLIB_CALC_TIME
			g_dAheCalcStringBounds_LinerLineTime += (double)GetTickCount() - dStartLinerLine;
#endif // ifdef SINDYLIB_CALC_TIME
		}
		else {
			ERRORLOG(sindyErr_ArgLimitOver);
			SASSERT(false);
		}

		LOGASSERTE_IF( NULL != ipBound && SUCCEEDED( ((ITopologicalOperatorPtr)ipBound)->Simplify() ), sindyErr_GeometryFunctionFailed );
	}
	
	return ipBound;
}

// �\���^�C�v�ƃX�P�[���\�����[���̘_�������`�F�b�N����
bool AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::ECode emDispTypeCode, schema::annotation_class::scaledisp_rule::ECode emScaleDispRuleCode )
{
	bool bRet = false;	// �Ԃ�l�i�������Ȃ������߁A�f�t�H���g�̓G���[�Ƃ��Ă������Ɓj

	// �`�F�b�N��ǉ����鎞�̎w�j�F
	// scaledisp_rule�ɑ΂���OK��schema::annotation::disp_type�𖾎����A����ȊO��
	// �G���[�Ƃ��邱�ƁB
	// �܂�A�����ŋL�q�����schema::annotation::disp_type�Ɋւ��Ă�OK�ȃp�^�[���݂̂�
	// ����B����ɂ��A����R�[�h���������ꍇ�ɂƂ肠�����G���[�Ƃ���
	// �o�͂����悤�ɂȂ�A�������Ȃ��Ȃ�B
	switch( emScaleDispRuleCode )
	{
	case schema::annotation_class::scaledisp_rule::kOOOOO:					// �L��+������ ... �w���_���\
	case schema::annotation_class::scaledisp_rule::kAny:					// �Ȃ�ł�����
		bRet = true; break;
	case schema::annotation_class::scaledisp_rule::kOXOXO:					// �L��+������(�w���_�s��) ... �w���_���E�L���̂ݕs��
	case schema::annotation_class::scaledisp_rule::kNotDirPointMustStr:		// �~   |�L+��|    �~|���̂�|�\����	 �c �w���_�s������K�{
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kOOXXX:					// �L���̂�
	case schema::annotation_class::scaledisp_rule::kMarkOnly:				// �L���i�w���_�s�j�̂�
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kOXOXX:					// ������̂�
	case schema::annotation_class::scaledisp_rule::kStrOnly:				// ������̂�
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kXXXXX:					// �\������
	case schema::annotation_class::scaledisp_rule::kNone:					// �\������
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
				bRet = true; break;
				break;
			default: break;
		}
		break;
	// �V�����ǉ����ꂽ���[��
	case schema::annotation_class::scaledisp_rule::kNotDirPoint:			// �~   |�L+��|�L�̂�|���̂�|�\����	 �c �w���_�s��
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kAnyMust:				// �w+��|�L+��|�L�̂�|���̂�|�~		 �c ���ł��L��i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustMark:				// �w+��|�L+��|�L�̂�|    �~|�\����	 �c �L���K�{
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;		
	case schema::annotation_class::scaledisp_rule::kMustMarkMust:			// �w+��|�L+��|�L�̂�|    �~|�~		 �c �L���K�{�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustStr:				// �w+��|�L+��|    �~|���̂�|�\����	 �c ������K�{
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustStrMust:			// �w+��|�L+��|    �~|���̂�|�~		 �c ������K�{�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustPair:				// �w+��|�L+��|    �~|    �~|�\����	 �c �L���ƕ����K���y�A
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustPairMust:			// �w+��|�L+��|    �~|    �~|�~		 �c �L���ƕ����K���y�A�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMust:		// �~   |�L+��|�L�̂�|���̂�|�~		 �c �w���_�s�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMultMark:	// �~   |�L+��|�L�̂�|    �~|�\����	 �c �w���_�s�L���K�{
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMultMarkMust:// �~   |�L+��|�L�̂�|    �~|�~		 �c �w���_�s�L���K�{�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMustStrMust:	// �~   |�L+��|    �~|���̂�|�~		 �c �w���_�s������K�{�i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkStr:				// �~   |�L+��|    �~|    �~|�\����	 �c �L���i�w���_�s�j+������̂�
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkStrMust:			// �~   |�L+��|    �~|    �~|�~		 �c �L���i�w���_�s�j+������̂݁i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkOnlyMust:			// �~   |   �~|�L�̂�|    �~|�~		 �c �L���i�w���_�s�j�̂݁i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kStrOnlyMust:			// �~   |   �~|    �~|���̂�|�~		 �c ������̂݁i�\���K�{�j
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	default:
		break;
	}
	return bRet;
}

// �\���^�C�v�Ə�ʂ̕\���^�C�v�̘_�������`�F�b�N����
bool AheCheckDispTypeAndUpperDispType( schema::annotation::disp_type::ECode emDispType, schema::annotation::disp_type::ECode emUpperDispType )
{
	// ��������Ԃł̉��ʃX�P�[���ւ̉e���x������
	// �ȉ��̏��ԂŌ��肷��
	// 
	// �L���{������    ��
	// �w���_�{������   
	// ������̂�
	// �L���̂�
	// �\���Ȃ�        ��
	//
	// �Ⴆ�΁AS4�ŋL���݂̂ł���΁AS3�͋L���݂̂��\���Ȃ������I���ł��Ȃ��B
	// S4�ŕ�����݂̂ł���΁AS3�ł͕�����݂̂��L���݂̂��\���Ȃ������I���ł��Ȃ��B
	bool bRet = false; // �Ԃ�l

	if( emDispType == emUpperDispType )
		bRet = true;
	else {
		switch( emUpperDispType )
		{
			case schema::annotation::disp_type::kSymStr:
				switch( emDispType )
				{
					case schema::annotation::disp_type::kPoiStr:
					case schema::annotation::disp_type::kStr:
					case schema::annotation::disp_type::kSym:
					case schema::annotation::disp_type::kNone:
						bRet = true;							
						break;
					default: break;
				}
				break;
			case schema::annotation::disp_type::kPoiStr:
				switch( emDispType )
				{
					case schema::annotation::disp_type::kStr:
					case schema::annotation::disp_type::kSym:
					case schema::annotation::disp_type::kNone:
						bRet = true;							
						break;
					default: break;
				}
				break;
			case schema::annotation::disp_type::kStr:
				switch( emDispType )
				{
					case schema::annotation::disp_type::kSym:
					case schema::annotation::disp_type::kNone:
						bRet = true;							
						break;
					default: break;
				}
				break;
			case schema::annotation::disp_type::kSym:
				switch( emDispType )
				{
					case schema::annotation::disp_type::kNone:
						bRet = true;							
						break;
					default: break;
				}
				break;
			default: break;
		}
	}

	return bRet;
}

// �e�X�P�[���̕\���^�C�v�̘_�������`�F�b�N����
bool AheCheckDispType( schema::annotation::disp_type::ECode emDispTypeS1, schema::annotation::disp_type::ECode emDispTypeS2,
					  schema::annotation::disp_type::ECode emDispTypeS3, schema::annotation::disp_type::ECode  emDispTypeS4, bool* pRetScale/* = NULL*/ )
{
	bool bRet = true; // �Ԃ�l

	// ������
	if( pRetScale )
		pRetScale[0] = pRetScale[1] = pRetScale[2] = pRetScale[3] = true;

	// S4�̃`�F�b�N
	if( schema::annotation::disp_type::kNone == emDispTypeS4 ) // �\�������Ȃ�G���[
	{
		bRet = false;
		if( pRetScale )	
			pRetScale[3] = false;
	}

	// S4�ȊO�̃`�F�b�N
	// �X���[�����Ƃ��ɃG���[�ɂȂ�悤�ɏ����l�ݒ�
	schema::annotation::disp_type::ECode emTargetDispType = schema::annotation::disp_type::kStr; // ���݂̃X�P�[��
	schema::annotation::disp_type::ECode emUpperDispType = schema::annotation::disp_type::kNone;  // ��ʂ̃X�P�[��
	for( int i = 1; i <= 3; ++i )
	{
		switch( i )
		{
			case 1: emTargetDispType = emDispTypeS1; emUpperDispType = emDispTypeS2; break;
			case 2: emTargetDispType = emDispTypeS2; emUpperDispType = emDispTypeS3; break;
			case 3: emTargetDispType = emDispTypeS3; emUpperDispType = emDispTypeS4; break;
			default:
				break;
		}

		if( ! AheCheckDispTypeAndUpperDispType( emTargetDispType, emUpperDispType ) )
		{
			bRet = false;
			if( pRetScale )
				pRetScale[i-1] = false; // S1 -> pErrScale[0]�Ȃ̂�-1
		}
	}

	return bRet;
}

} // namespace sindy
