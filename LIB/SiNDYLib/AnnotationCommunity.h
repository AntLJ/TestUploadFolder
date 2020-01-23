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
 * @brief 注記テーブルフィールド・ドメイン定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
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
	 * @brief 注記種別表示名定義
	 */
	namespace annotation_code_label {
#define ANNOTATION_CODE(name,id,size,label) extern SINDYLIB_API const TCHAR name[];
#include <sindy/schema/annotationcode.h>
#undef ANNOTATION_CODE
	} // annotation_code_label

	/**
	 * @brief 注記文字間間隔タイプ
	 */
	enum sindyAnnotationStringInterval {
		sindyAnnoStrInterval_None,		//!< 間隔なし
		sindyAnnoStrInterval_Half,		//!< 半角
		sindyAnnoStrInterval_Full,		//!< 全角
		sindyAnnoStrInterval_Double,	//!< 二倍角
		sindyAnnoStrInterval_Same,		//!< 等間隔
		sindyAnnoStrInterval_Road,		//!< 国道・主要地方道番号用
	};

	/**
	 * @brief <b>表示ラインタイプ</b>\n
	 * ビット演算可能
	 */
	enum sindyAnnotationLineType {
		sindyAnnoLine_Horizonal = 0x0001,		//!< 横書き
		sindyAnnoLine_Vertical	= 0x0002,		//!< 縦書き
		sindyAnnoLine_Single	= 0x0004,		//!< 一行
		sindyAnnoLine_Double	= 0x0008,		//!< 二行
		sindyAnnoLine_Linear	= 0x0010,		//!< 線状
	};

	/**
	 * @brief 注記文字列と記号の位置関係
	 */
	enum sindyAnnotationPosition {
		sindyAnnoPos_UpperHorizonal,	//!< 注記が上・横書き
		sindyAnnoPos_UpperVertical,		//!< 注記が上・縦書き
		sindyAnnoPos_LowerHorizonal,	//!< 注記が下・横書き
		sindyAnnoPos_LowerVertical,		//!< 注記が下・縦書き
		sindyAnnoPos_RightHorizonal,	//!< 注記が右・横書き
		sindyAnnoPos_LeftHorizonal,		//!< 注記が左・横書き
		sindyAnnoPos_Other,				//!< 上記以外（線状注記の場合など）
	};

	//@{ @name スケールごとの1pixel当たりの長さ（単位：度（緯度経度））\n
	// このパラメータは全て計算によって出しました。
	// 以下に、そのプログラムを示しておきます。
	// SiNDYで使用しているレイヤのみを最低一つは読み込んだ状態で
	// 実行してください。
	// @code
	// double dOldScale;
	// IPointPtr ipMapFromPoint, ipMapToPoint;
	// double dLenX = 0, dLenY = 0;
	// 
	// // 現在のdpi値を取得
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
	//@{ @name スケールごとの1pt当たりの長さ（単位：度（緯度経度））\n
	// このパラメータは全て計算によって出しました。
	// 以下に、そのプログラムを示しておきます。
	// SiNDYで使用しているレイヤのみを最低一つは読み込んだ状態で
	// 実行してください。
	// @code
	// double dOldScale;
	// IPointPtr ipMapFromPoint, ipMapToPoint;
	// double dLenX = 0, dLenY = 0;
	// 
	// // 現在のdpi値を取得
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
	const static double ANNOTATION_1PT_CSC1_X = 0.0000198288413836659530000;	// D1スケール
	const static double ANNOTATION_1PT_CSC1_Y = 0.0000198288413457703428000;	// D1スケール

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
	 * @brief 注記基準スケール
	 */
	enum sindyAnnotationScale {
		sindyAnnoScale_city = 3125,											//!< 都市地図基準スケール

		// 2006.08.10 F.Abe
		sindyAnnoScale_csc1 = 6250,											//!< D1

		sindyAnnoScale_bsc4 = 12500,										//!< 中縮S4基準スケール
		sindyAnnoScale_bsc3 = 25000,										//!< 中縮S3基準スケール
		sindyAnnoScale_bsc2 = 50000,										//!< 中縮S2基準スケール
		sindyAnnoScale_bsc1 = 100000,										//!< 中縮S1基準スケール
		sindyAnnoScale_msc4 = 200000,										//!< ミドルS4基準スケール
		sindyAnnoScale_msc3 = 400000,										//!< ミドルS3基準スケール
		sindyAnnoScale_msc2 = 800000,										//!< ミドルS2基準スケール
		sindyAnnoScale_msc1 = 1600000,										//!< ミドルS1基準スケール
		sindyAnnoScale_tsc4 = 3200000,										//!< トップS4基準スケール
		sindyAnnoScale_tsc3 = 6400000,										//!< トップS3基準スケール
		sindyAnnoScale_tsc2 = 12800000,										//!< トップS2基準スケール
		sindyAnnoScale_tsc1 = 25600000,										//!< トップS1基準スケール
	};

	/// 注記グループ
	enum sindyAnnotationGroup {
		sindyAnnoGroup_city,   //!< 都市地図
		sindyAnnoGroup_csc1,   //!< 1/6250
		sindyAnnoGroup_base,   //!< 中縮
		sindyAnnoGroup_middle, //!< ミドル
		sindyAnnoGroup_top,    //!< トップ
	};
} // namespace sindy

#endif // _ANNOTATIONCOMMUNITY_H_
