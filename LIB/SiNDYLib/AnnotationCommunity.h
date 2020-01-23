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
 * @file AnnotationCommunity.h
 * @brief ���L�e�[�u���t�B�[���h�E�h���C����`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ANNOTATIONCOMMUNITY_H_
#define _ANNOTATIONCOMMUNITY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

namespace sindy {

	/**
	 * @namespace sindy::annotation_code_label
	 * @brief ���L��ʕ\������`
	 */
	namespace annotation_code_label {
#define ANNOTATION_CODE(name,id,size,label) extern SINDYLIB_API const TCHAR name[];
#include <sindy/schema/annotationcode.h>
#undef ANNOTATION_CODE
	} // annotation_code_label

	/**
	 * @brief ���L�����ԊԊu�^�C�v
	 */
	enum sindyAnnotationStringInterval {
		sindyAnnoStrInterval_None,		//!< �Ԋu�Ȃ�
		sindyAnnoStrInterval_Half,		//!< ���p
		sindyAnnoStrInterval_Full,		//!< �S�p
		sindyAnnoStrInterval_Double,	//!< ��{�p
		sindyAnnoStrInterval_Same,		//!< ���Ԋu
		sindyAnnoStrInterval_Road,		//!< �����E��v�n�����ԍ��p
	};

	/**
	 * @brief <b>�\�����C���^�C�v</b>\n
	 * �r�b�g���Z�\
	 */
	enum sindyAnnotationLineType {
		sindyAnnoLine_Horizonal = 0x0001,		//!< ������
		sindyAnnoLine_Vertical	= 0x0002,		//!< �c����
		sindyAnnoLine_Single	= 0x0004,		//!< ��s
		sindyAnnoLine_Double	= 0x0008,		//!< ��s
		sindyAnnoLine_Linear	= 0x0010,		//!< ����
	};

	/**
	 * @brief ���L������ƋL���̈ʒu�֌W
	 */
	enum sindyAnnotationPosition {
		sindyAnnoPos_UpperHorizonal,	//!< ���L����E������
		sindyAnnoPos_UpperVertical,		//!< ���L����E�c����
		sindyAnnoPos_LowerHorizonal,	//!< ���L�����E������
		sindyAnnoPos_LowerVertical,		//!< ���L�����E�c����
		sindyAnnoPos_RightHorizonal,	//!< ���L���E�E������
		sindyAnnoPos_LeftHorizonal,		//!< ���L�����E������
		sindyAnnoPos_Other,				//!< ��L�ȊO�i���󒍋L�̏ꍇ�Ȃǁj
	};

	//@{ @name �X�P�[�����Ƃ�1pixel������̒����i�P�ʁF�x�i�ܓx�o�x�j�j\n
	// ���̃p�����[�^�͑S�Čv�Z�ɂ���ďo���܂����B
	// �ȉ��ɁA���̃v���O�����������Ă����܂��B
	// SiNDY�Ŏg�p���Ă��郌�C���݂̂��Œ��͓ǂݍ��񂾏�Ԃ�
	// ���s���Ă��������B
	// @code
	// double dOldScale;
	// IPointPtr ipMapFromPoint, ipMapToPoint;
	// double dLenX = 0, dLenY = 0;
	// 
	// // ���݂�dpi�l���擾
	// double dDPI_X = GetDeviceCaps( hDisplayDC, LOGPIXELSX );
	// double dDPI_Y = GetDeviceCaps( hDisplayDC, LOGPIXELSY );
	//
	// ipDispTrans->get_ScaleRatio( &dOldScale );
	// for( long scale = 3125; scale <= 25600000; scale *= 2 )
	// {
	//     if( scale == 6250 ) continue;
	//     
	//     CString strDefName;
	//     switch( scale )
	//     {
	//     case 3125: strDefName = _T("ANNOTATION_1PX_CITY_"); break;
	//     case 12500: strDefName = _T("ANNOTATION_1PX_BSC4_"); break;
	//     case 25000: strDefName = _T("ANNOTATION_1PX_BSC3_"); break;
	//     case 50000: strDefName = _T("ANNOTATION_1PX_BSC2_"); break;
	//     case 100000: strDefName = _T("ANNOTATION_1PX_BSC1_"); break;
	//     case 200000: strDefName = _T("ANNOTATION_1PX_MSC4_"); break;
	//     case 400000: strDefName = _T("ANNOTATION_1PX_MSC3_"); break;
	//     case 800000: strDefName = _T("ANNOTATION_1PX_MSC2_"); break;
	//     case 1600000: strDefName = _T("ANNOTATION_1PX_MSC1_"); break;
	//     case 3200000: strDefName = _T("ANNOTATION_1PX_TSC4_"); break;
	//     case 6400000: strDefName = _T("ANNOTATION_1PX_TSC3_"); break;
	//     case 12800000: strDefName = _T("ANNOTATION_1PX_TSC2_"); break;
	//     case 25600000: strDefName = _T("ANNOTATION_1PX_TSC1_"); break;
	//     }
	//     ipDispTrans->put_ScaleRatio( scale );
	//     
	//     ipDispTrans->ToMapPoint( 0, 0, &ipMapFromPoint );
	//     ipDispTrans->ToMapPoint( 1, 0, &ipMapToPoint );
	//     ((IProximityOperatorPtr)ipMapFromPoint)->ReturnDistance( ipMapToPoint, &dLenX );
	//     ipDispTrans->ToMapPoint( 0, 0, &ipMapFromPoint );
	//     ipDispTrans->ToMapPoint( 0, 1, &ipMapToPoint );
	//     ((IProximityOperatorPtr)ipMapFromPoint)->ReturnDistance( ipMapToPoint, &dLenY );
	//     ATLTRACE( _T("const static double %sX = %.25lf;\nconst static double %sY = %.25lf;\n"), strDefName, dLenX, strDefName, dLenY );
	// }
	// ipDispTrans->put_ScaleRatio( dOldScale );
	// @endcode
	const static double ANNOTATION_1PX_CITY_X = 0.0000074358155188747332000;
	const static double ANNOTATION_1PX_CITY_Y = 0.0000074358155046638785000;

	// 2006.08.10 F.Abe
	const static double ANNOTATION_1PX_CSC1_X = 0.0000148716310377494664000;
	const static double ANNOTATION_1PX_CSC1_Y = 0.0000148716310093277570000;

	const static double ANNOTATION_1PX_BSC4_X = 0.0000297432620186555140000;
	const static double ANNOTATION_1PX_BSC4_Y = 0.0000297432620186555140000;
	const static double ANNOTATION_1PX_BSC3_X = 0.0000594865240373110280000;
	const static double ANNOTATION_1PX_BSC3_Y = 0.0000594865240373110280000;
	const static double ANNOTATION_1PX_BSC2_X = 0.0001189730480746220600000;
	const static double ANNOTATION_1PX_BSC2_Y = 0.0001189730480746220600000;
	const static double ANNOTATION_1PX_BSC1_X = 0.0002379460961492441100000;
	const static double ANNOTATION_1PX_BSC1_Y = 0.0002379460961421386900000;
	const static double ANNOTATION_1PX_MSC4_X = 0.0004758921922984882300000;
	const static double ANNOTATION_1PX_MSC4_Y = 0.0004758921922913828000000;
	const static double ANNOTATION_1PX_MSC3_X = 0.0009517843845685547400000;
	const static double ANNOTATION_1PX_MSC3_Y = 0.0009517843845756601700000;
	const static double ANNOTATION_1PX_MSC2_X = 0.0019035687691371095000000;
	const static double ANNOTATION_1PX_MSC2_Y = 0.0019035687691513203000000;
	const static double ANNOTATION_1PX_MSC1_X = 0.0038071375383026407000000;
	const static double ANNOTATION_1PX_MSC1_Y = 0.0038071375383168515000000;
	const static double ANNOTATION_1PX_TSC4_X = 0.0076142750766337031000000;
	const static double ANNOTATION_1PX_TSC4_Y = 0.0076142750766265976000000;
	const static double ANNOTATION_1PX_TSC3_X = 0.0152285501532389840000000;
	const static double ANNOTATION_1PX_TSC3_Y = 0.0152285501532460900000000;
	const static double ANNOTATION_1PX_TSC2_X = 0.0304571003064921800000000;
	const static double ANNOTATION_1PX_TSC2_Y = 0.0304571003064921800000000;
	const static double ANNOTATION_1PX_TSC1_X = 0.0609142006129843590000000;
	const static double ANNOTATION_1PX_TSC1_Y = 0.0609142006129843590000000;
	//@{ @name �X�P�[�����Ƃ�1pt������̒����i�P�ʁF�x�i�ܓx�o�x�j�j\n
	// ���̃p�����[�^�͑S�Čv�Z�ɂ���ďo���܂����B
	// �ȉ��ɁA���̃v���O�����������Ă����܂��B
	// SiNDY�Ŏg�p���Ă��郌�C���݂̂��Œ��͓ǂݍ��񂾏�Ԃ�
	// ���s���Ă��������B
	// @code
	// double dOldScale;
	// IPointPtr ipMapFromPoint, ipMapToPoint;
	// double dLenX = 0, dLenY = 0;
	// 
	// // ���݂�dpi�l���擾
	// double dDPI_X = GetDeviceCaps( hDisplayDC, LOGPIXELSX );
	// double dDPI_Y = GetDeviceCaps( hDisplayDC, LOGPIXELSY );
	//
	// ipDispTrans->get_ScaleRatio( &dOldScale );
	// for( long scale = 3125; scale <= 25600000; scale *= 2 )
	// {
	//     if( scale == 6250 ) continue;
	//     
	//     CString strDefName;
	//     switch( scale )
	//     {
	//     case 3125: strDefName = _T("ANNOTATION_1PT_CITY_"); break;
	//     case 12500: strDefName = _T("ANNOTATION_1PT_BSC4_"); break;
	//     case 25000: strDefName = _T("ANNOTATION_1PT_BSC3_"); break;
	//     case 50000: strDefName = _T("ANNOTATION_1PT_BSC2_"); break;
	//     case 100000: strDefName = _T("ANNOTATION_1PT_BSC1_"); break;
	//     case 200000: strDefName = _T("ANNOTATION_1PT_MSC4_"); break;
	//     case 400000: strDefName = _T("ANNOTATION_1PT_MSC3_"); break;
	//     case 800000: strDefName = _T("ANNOTATION_1PT_MSC2_"); break;
	//     case 1600000: strDefName = _T("ANNOTATION_1PT_MSC1_"); break;
	//     case 3200000: strDefName = _T("ANNOTATION_1PT_TSC4_"); break;
	//     case 6400000: strDefName = _T("ANNOTATION_1PT_TSC3_"); break;
	//     case 12800000: strDefName = _T("ANNOTATION_1PT_TSC2_"); break;
	//     case 25600000: strDefName = _T("ANNOTATION_1PT_TSC1_"); break;
	//     }
	//     ipDispTrans->put_ScaleRatio( scale );
	//     
	//     ipDispTrans->ToMapPoint( 0, 0, &ipMapFromPoint );
	//     ipDispTrans->ToMapPoint( 1, 0, &ipMapToPoint );
	//     ((IProximityOperatorPtr)ipMapFromPoint)->ReturnDistance( ipMapToPoint, &dLenX );
	//     dLenX *= ( 1.0 / 72.0 ) * dDPI_X;
	//     ipDispTrans->ToMapPoint( 0, 0, &ipMapFromPoint );
	//     ipDispTrans->ToMapPoint( 0, 1, &ipMapToPoint );
	//     ((IProximityOperatorPtr)ipMapFromPoint)->ReturnDistance( ipMapToPoint, &dLenY );
	//     dLenY *= ( 1.0 / 72.0 ) * dDPI_Y;
	//     ATLTRACE( _T("const static double %sX = %.25lf;\nconst static double %sY = %.25lf;\n"), strDefName, dLenX, strDefName, dLenY );
	// }
	// ipDispTrans->put_ScaleRatio( dOldScale );
	// @endcode
	const static double ANNOTATION_1PT_CITY_X = 0.0000099144206918329765000;
	const static double ANNOTATION_1PT_CITY_Y = 0.0000099144206728851714000;

	// 2006.08.10 F.ABe
	const static double ANNOTATION_1PT_CSC1_X = 0.0000198288413836659530000;	// D1�X�P�[��
	const static double ANNOTATION_1PT_CSC1_Y = 0.0000198288413457703428000;	// D1�X�P�[��

	const static double ANNOTATION_1PT_BSC4_X = 0.0000396576826915406850000;
	const static double ANNOTATION_1PT_BSC4_Y = 0.0000396576826915406850000;
	const static double ANNOTATION_1PT_BSC3_X = 0.0000793153653830813710000;
	const static double ANNOTATION_1PT_BSC3_Y = 0.0000793153653830813710000;
	const static double ANNOTATION_1PT_BSC2_X = 0.0001586307307661627400000;
	const static double ANNOTATION_1PT_BSC2_Y = 0.0001586307307661627400000;
	const static double ANNOTATION_1PT_BSC1_X = 0.0003172614615323254800000;
	const static double ANNOTATION_1PT_BSC1_Y = 0.0003172614615228515600000;
	const static double ANNOTATION_1PT_MSC4_X = 0.0006345229230646509700000;
	const static double ANNOTATION_1PT_MSC4_Y = 0.0006345229230551769900000;
	const static double ANNOTATION_1PT_MSC3_X = 0.0012690458460914063000000;
	const static double ANNOTATION_1PT_MSC3_Y = 0.0012690458461008802000000;
	const static double ANNOTATION_1PT_MSC2_X = 0.0025380916921828125000000;
	const static double ANNOTATION_1PT_MSC2_Y = 0.0025380916922017605000000;
	const static double ANNOTATION_1PT_MSC1_X = 0.0050761833844035209000000;
	const static double ANNOTATION_1PT_MSC1_Y = 0.0050761833844224684000000;
	const static double ANNOTATION_1PT_TSC4_X = 0.0101523667688449370000000;
	const static double ANNOTATION_1PT_TSC4_Y = 0.0101523667688354640000000;
	const static double ANNOTATION_1PT_TSC3_X = 0.0203047335376519770000000;
	const static double ANNOTATION_1PT_TSC3_Y = 0.0203047335376614520000000;
	const static double ANNOTATION_1PT_TSC2_X = 0.0406094670753229040000000;
	const static double ANNOTATION_1PT_TSC2_Y = 0.0406094670753229040000000;
	const static double ANNOTATION_1PT_TSC1_X = 0.0812189341506458080000000;
	const static double ANNOTATION_1PT_TSC1_Y = 0.0812189341506458080000000;
	//@}
	/**
	 * @brief ���L��X�P�[��
	 */
	enum sindyAnnotationScale {
		sindyAnnoScale_city = 3125,											//!< �s�s�n�}��X�P�[��

		// 2006.08.10 F.Abe
		sindyAnnoScale_csc1 = 6250,											//!< D1

		sindyAnnoScale_bsc4 = 12500,										//!< ���kS4��X�P�[��
		sindyAnnoScale_bsc3 = 25000,										//!< ���kS3��X�P�[��
		sindyAnnoScale_bsc2 = 50000,										//!< ���kS2��X�P�[��
		sindyAnnoScale_bsc1 = 100000,										//!< ���kS1��X�P�[��
		sindyAnnoScale_msc4 = 200000,										//!< �~�h��S4��X�P�[��
		sindyAnnoScale_msc3 = 400000,										//!< �~�h��S3��X�P�[��
		sindyAnnoScale_msc2 = 800000,										//!< �~�h��S2��X�P�[��
		sindyAnnoScale_msc1 = 1600000,										//!< �~�h��S1��X�P�[��
		sindyAnnoScale_tsc4 = 3200000,										//!< �g�b�vS4��X�P�[��
		sindyAnnoScale_tsc3 = 6400000,										//!< �g�b�vS3��X�P�[��
		sindyAnnoScale_tsc2 = 12800000,										//!< �g�b�vS2��X�P�[��
		sindyAnnoScale_tsc1 = 25600000,										//!< �g�b�vS1��X�P�[��
	};

	/// ���L�O���[�v
	enum sindyAnnotationGroup {
		sindyAnnoGroup_city,   //!< �s�s�n�}
		sindyAnnoGroup_csc1,   //!< 1/6250
		sindyAnnoGroup_base,   //!< ���k
		sindyAnnoGroup_middle, //!< �~�h��
		sindyAnnoGroup_top,    //!< �g�b�v
	};
} // namespace sindy

#endif // _ANNOTATIONCOMMUNITY_H_
