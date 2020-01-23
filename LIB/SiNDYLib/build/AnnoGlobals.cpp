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
 * @brief 注記系外部関数実装
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
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
double g_dAheCalcStringBounds_OneLineTime = 0.0;	//!< 一行注記のバウンディングボックスの作成にかかる総時間
double g_dAheCalcStringBounds_OneLine1Time = 0.0;	//!< 一行注記のバウンディングボックスの作成にかかる総時間（前半）
double g_dAheCalcStringBounds_OneLine2Time = 0.0;	//!< 一行注記のバウンディングボックスの作成にかかる総時間（後半）
double g_dAheCalcStringBounds_DoubleLineTime = 0.0; //!< 二行注記のバウンディングボックスの作成にかかる総時間
double g_dAheCalcStringBounds_LinerLineTime = 0.0;	//!< 線状注記のバウンディングボックスの作成にかかる総時間

void AheCalcStringBoundsTraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("AheCalcStringBounds     : 計測時間結果 :\n") );
	TRACEMESSAGE( _T("                        : 一行注記             総時間             : %.5lf秒\n"), g_dAheCalcStringBounds_OneLineTime/1000.0 );
	TRACEMESSAGE( _T("                        :                        前半               : %.5lf秒\n"), g_dAheCalcStringBounds_OneLine1Time/1000.0 );
	TRACEMESSAGE( _T("                        :                        後半               : %.5lf秒\n"), g_dAheCalcStringBounds_OneLine2Time/1000.0 );
	TRACEMESSAGE( _T("                        : 二行注記             総時間             : %.5lf秒\n"), g_dAheCalcStringBounds_DoubleLineTime/1000.0 );
	TRACEMESSAGE( _T("                        : 線状注記             総時間             : %.5lf秒\n"), g_dAheCalcStringBounds_LinerLineTime/1000.0 );
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

// 注記種別から注記種別表示名を取得する
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

// テーブルタイプからスケールコードに変換する
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

// テーブルタイプから注記グループに変換する
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

// テーブルタイプから真位置テーブルタイプを取得する
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

// 表示位置テーブルタイプからスケールタイプ文字列に変換する
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

// グループを考慮してメッシュコードを矩形に変換する
IEnvelopePtr AheMeshCode2Envelope( long lMeshCode, sindyAnnotationGroup emGroup, ISpatialReference* ipSpRef )
{
	// メッシュコードとメッシュタイプの整合をチェック
	// メッシュコードの桁数を取得
	TCHAR strMeshCode[10] = _T("");
	_ltot( lMeshCode, strMeshCode, 10 );
	long lFig = lstrlen(strMeshCode);
	// 桁数変換
	switch( emGroup )
	{
		case sindyAnnoGroup_city:
		case sindyAnnoGroup_csc1:
			if(      8 != lFig )  	lMeshCode = -1; // 都市地図メッシュコードではない
			break;
		case sindyAnnoGroup_base:
			if(      8 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 6 == lFig )	;
			else    				lMeshCode = -1; // 都市地図・2次メッシュコードではない
			break;
		case sindyAnnoGroup_middle:
			if(      8 == lFig )   	lMeshCode = (long)(lMeshCode / 10000);
			else if( 6 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 4 == lFig )	;
			else             		lMeshCode = -1; // 都市地図・2次・1次メッシュコードではない
			break;
		case sindyAnnoGroup_top:
			if(      8 == lFig )  	lMeshCode = (long)(lMeshCode / 1000000);
			else if( 6 == lFig )	lMeshCode = (long)(lMeshCode / 10000);
			else if( 4 == lFig )	lMeshCode = (long)(lMeshCode / 100);
			else if( 2 == lFig )    ;
			else             		lMeshCode = -1; // 都市地図・2次・1次・トップメッシュコードではない
			break;
		default:                    lMeshCode = -1;
			break;
	}
	if( 0 > lMeshCode )
		return NULL;

	// メッシュコードをエリアに変換
	// 都市地図、ベースはそのままで良いが、ミドルは
	// 1次x4なので、エリアを変換する
	// トップはしらん
	IEnvelopePtr ipEnv( MESH2ENV( lMeshCode, ipSpRef ) );
	switch( emGroup )
	{
		case sindyAnnoGroup_middle:
		{
			// メッシュコード下一桁目が偶数ならX方向に++、奇数ならX方向に--
			// メッシュコード上二桁目が偶数ならY方向に++、奇数ならY方向に--
			long lMeshCode2 = ( 1 == lMeshCode % 2 )        ? DECMESH_X(lMeshCode ) : INCMESH_X(lMeshCode );
			lMeshCode2 = ( 1 == (int)(lMeshCode2/100) % 2 ) ? INCMESH_Y(lMeshCode2) : DECMESH_Y(lMeshCode2);
			IEnvelopePtr ipEnv2( MESH2ENV( lMeshCode2, ipSpRef ) );
			ipEnv->Union( ipEnv2 );	// 右上のメッシュとのUNIONを取る
			break;
		}
		case sindyAnnoGroup_top:
			ipEnv = NULL; // topマップは計算が特殊で、しかもあまりやる価値がないので無視。
			break;
		default:
			break;
	}
	_ASSERTE( ipEnv );

	return ipEnv;
}

// 渡されたテーブルタイプが真位置テーブルタイプかどうかを検査する
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

// 渡されたテーブルタイプが表示位置テーブルタイプかどうかを検査する
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

// 注記文字列をチェック・修復する
sindyErrCode AheCheckOrFixAnnotationString( LPCTSTR lpcszString, long lStringLen, schema::annotation::annotation_code::ECode emAnnoCode, bool bIsCity/* = true*/, bool bIsString3/* = false*/, bool bFix/* = false*/, CString* pstrFixString/* = NULL*/ )
{
	USES_CONVERSION;

	// 引数チェック
	if( ( bFix   && pstrFixString == NULL ) ||
		( ! bFix && pstrFixString != NULL ) )
		return sindyErr_ArgIsConsistency;					// bFixとpstrFixStringはペア
/*	if( lStringLen < 0 ) 
		return sindyErr_ArgLimitOver;		// 文字列長が負はだめ*/ // <- 文字列長がNULLの場合は-1がくるのでここでエラーにしない

	if( 0 >= lStringLen && ! lpcszString ) return sindyErr_NoErr;	// 文字列がNULL及び文字列長が0以下の時は、両方の値がNULLの時なので、これはOKで返す

	CString strString( lpcszString );					// 注記文字列
	long lRealLen = CString_GetLength2( strString );	// 実際の文字列長

/*	// 実際の文字列長と注記文字数との比較
	if( lRealLen != lStringLen )
		return sindyErr_AnnoStrLenNotSame; <- 注記文字数は一文字1バイトとして計算するので、ここでチェックしてはダメ！！*/
	long lStrCount = 0;

	// 山岳名称と山岳標高値を区別するため、都市地図ではなく文字列３でもない場合はkMountainRangeとして
	// チェックを通す
	// kMountainRangeに特別なルールを設定する場合は、別の種別にすること！！
	// 以下、kMountainとしてチェックしているときは全て山岳標高値としてのチェックとなる
	if( emAnnoCode == schema::annotation::annotation_code::kMountain )
	{
		if( bIsCity || ! bIsString3 )
			emAnnoCode = schema::annotation::annotation_code::kMountainRange;
	}

	// [rule_check_1_2_1_1E] 文字を一つずつチェック
//	long lFigure = 0;			//!< 山岳の場合の整数部の桁数
	bool bFindPeriod = false;	//!< 半角ピリオドが現れたかどうか
	for( long i = 0; i < lRealLen; ++i )
	{
		// 全てSJISに直して比較
		CString str = CString_GetAt2( strString, i );
		LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

		switch( emAnnoCode )
		{
			// 山岳の場合
			case schema::annotation::annotation_code::kMountain:
/*				// [rule_check_1_2_1_4E] 山岳の場合（bIsSingle=true且つbUsePeriod=true）は整数部の桁数が3以下である必要がある
				if( ! bFindPeriod )
				{
					++lFigure;	// 整数部の桁数をカウント
					if( lFigure >= 3 )
						return sindyErr_AnnoStrFigureOverFlow;
				}
				if( isasciiperiod( lpstr[0] ) ) bFindPeriod = true;	// 半角ピリオドを見つけたらフラグを立てておく
*/
				// [rule_check_1_2_1_15E] 山岳の場合は小数点第一位までは存在する必要がある
				if( isasciiperiod( lpstr[0] ) )
				{
					if( lRealLen - 1 > i )	// ピリオドで終わっていないかどうかのチェック
						bFindPeriod = true;
					else
						return sindyErr_AnnoStrMountainInvalid;
				} 
				else {
					if( bFindPeriod && isasciiperiod( lpstr[0] ) ) // ピリオドが単一であるかどうかのチェック
						return sindyErr_AnnoStrMountainInvalid;
				}
			// 国道番号・主要地方道番号の場合
			case schema::annotation::annotation_code::kNationalRouteNumber:
			case schema::annotation::annotation_code::kMainPrefRouteNumber:
				// 半角数値のみOK（山岳の場合はピリオドもOK）
				if( ! ( isasciinumber( lpstr[0] ) || ( emAnnoCode == schema::annotation::annotation_code::kMountain && isasciiperiod( lpstr[0] ) ) ) )
					return sindyErr_AnnoStrIsNotHalfNumber;
				// 先頭が半角0以外のみOK
				if( i == 0 && isasciizero( lpstr[0] ) )
					return sindyErr_AnnoStrFistIsHalfZero;

				// 文字列カウンタを増加
				++lStrCount;

				break;
			// 全角の場合（上記以外）
			default:
				if (
				( issjiskanji(              lpstr[0], lpstr[1] ) ) ||	// 全角漢字
				( issjiswidehiragana(       lpstr[0], lpstr[1] ) ) ||	// 全角ひらがな
				( issjiswidekatakana(       lpstr[0], lpstr[1] ) ) ||	// 全角カタカナ
				( issjiswidenumber(         lpstr[0], lpstr[1] ) ) ||	// 「０」～「９」
				( issjiswidealphabet(       lpstr[0], lpstr[1] ) ) ||	// 「ａ」～「Ｚ」
				( issjiswidegreek(          lpstr[0], lpstr[1] ) ) ||	// 全角ギリシア文字
				( issjiswidekigou(          lpstr[0], lpstr[1] ) ) ||	// 全角利用可能特殊記号（その他）
				// 以下は一番初めにあってはいけない？（注記の規則は？）
				( issjiswideperiod(         lpstr[0], lpstr[1] ) ) ||	// 「．」
				( issjiswidenyoro(          lpstr[0], lpstr[1] ) ) ||	// 「～」
				( issjiswidenun(            lpstr[0], lpstr[1] ) ) ||	// 「々」
				( issjiswideshime(          lpstr[0], lpstr[1] ) ) ||	// 「〆」
				( issjiswidenakaguro(       lpstr[0], lpstr[1] ) ) ||	// 「・」
				( issjiswidesmallkakko(     lpstr[0], lpstr[1] ) ) ||	// 「（」
				( issjiswidesmalltojikakko( lpstr[0], lpstr[1] ) ) ||	// 「）」
				( issjiswidechouon(         lpstr[0], lpstr[1] ) ) ||	// 「ー」
				( issjiswidekanma(          lpstr[0], lpstr[1] ) ) ||	// 「、」
				( issjiswideminus(          lpstr[0], lpstr[1] ) ) 		// 「－」
				){
#if 0 // [bug 3035] いらない
					// [rule_check_1_2_1_9E] 全角0は先頭に来てはいけない
					if( i == 0 && 0x82 == (unsigned char)((lpstr[0])&0xff) && 0x4f == (unsigned char)((lpstr[1])&0xff) )
						return sindyErr_AnnoStrFirstIsWideZero;
					// [rule_check_1_2_1_14E] 全角長音記号は全角ひらがな・全角カタカナの後限定（ハイフンのチェックはどうしたらよいかわからない…）
					if( issjiswidechouon( lpstr[0], lpstr[1] ) )
					{
						CString strPrev = CString_GetAt2( strString, i-1 );
						LPSTR lpstrPrev = T2A( (LPTSTR)((LPCTSTR)strPrev) );
						if( ! ( issjiswidekatakana( lpstrPrev[0], lpstrPrev[1] ) || issjiswidehiragana( lpstrPrev[0], lpstrPrev[1] ) ) )
							return sindyErr_AnnoStrInvalidChouon;
					}
#endif // if 0
/*					// 全角マイナスは数字の後限定
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

				// 文字列カウンタを増加
				lStrCount += 2;

				break;
		}
	}

	// 注記文字数と実際の文字数が合っているかどうかチェック
	if( lStrCount != lStringLen ) return sindyErr_AnnoStrLenNotSame;

#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
	// 文字単位でのチェックでは面倒なものを以下で行う
	CString strLast2, strLast3;
	strLast2.Format(_T("%s%s"), CString_GetAt2( strString, lRealLen-2 ), CString_GetAt2( strString, lRealLen-1 ) );
	strLast3.Format(_T("%s%s"), CString_GetAt2( strString, lRealLen-3 ), strLast2 );
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK

	switch( emAnnoCode )
	{
#ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK
		// [rule_check_1_2_1_5E] インターチェンジの場合は文字列最後が「ＩＣ」、且つ文字列数が4以上
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
			if( strLast2.Compare(_T("ＩＣ")) == 0 )
			{
				if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
			}
			else
				return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			break;
		// [rule_check_1_2_1_6E] 都市地図注記以外の場合、ジャンクションは文字列最後が「ＪＣ」、且つ文字列数が4以上
		// [rule_check_1_2_1_11E] 都市地図注記の場合、ジャンクションは文字列最後が「ＪＣＴ」でなければならない
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
			// 都市地図注記の場合
			if( bIsCity )
			{
				if( strLast3.Compare(_T("ＪＣＴ")) != 0 )
					return sindyErr_AnnoStrJCButLast3IsNotJCT;
			}
			// 都市地図注記以外の場合
			else {
				if( strLast2.Compare(_T("ＪＣ")) == 0 ) 
				{
					if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
				}
				else
					return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			}
			break;
		// [rule_check_1_2_1_7E] フェリーターミナルの場合は文字列最後が「ＦＴ」、且つ文字列数が4以上
		case annotation_code::kFerryTerminal:
			if( strLast2.Compare(_T("ＦＴ")) == 0 )
			{
				if( lStringLen < 4 ) return sindyErr_AnnoStrICJCFTMustOver4;
			}
			else
				return sindyErr_AnnoStrICJCFTButLast2IsNotICJCFT;
			break;
#endif // ifdef SINDYLIB_DO_ANNO_ITJCFTFTCHECK

		// [rule_check_1_2_1_12E] 主要地方道番号の場合は且つ1～151まで（表示タイプのチェックは他でやる）
		case schema::annotation::annotation_code::kMainPrefRouteNumber:
			if( ! strString.IsEmpty() )
			{
				// 数値に変換して調べる
				long lNumber = _ttol( strString );
				if( ! ( 1 <= lNumber && lNumber <= 151 ) )
					return sindyErr_AnnoStrRouteNumberOverFlow;
			}
			break;
		// [rule_check_1_2_1_13E] 国道番号の場合は1～58、101～108、112～507
		case schema::annotation::annotation_code::kNationalRouteNumber:
			if( ! strString.IsEmpty() )
			{
				// 数値に変換して調べる
				long lNumber = _ttol( strString );
				if( !(  ( 1   <= lNumber && lNumber <= 58  ) || 
						( 101 <= lNumber && lNumber <= 108 ) || 
						( 112 <= lNumber && lNumber <= 507 ) ) )
					return sindyErr_AnnoStrRouteNumberOverFlow;
			}
			break;
		// [rule_check_1_2_1_15E] 山岳の場合は小数点第一位までは存在する必要がある
		case schema::annotation::annotation_code::kMountain:
			if( ! bFindPeriod )	
				return sindyErr_AnnoStrMountainInvalid;
			break;
		default: break;
	}

	return sindyErr_NoErr;
}

// 表示位置レイヤタイプから1ポイントあたりの距離を算出する
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

// 表示位置レイヤタイプから1ピクセルあたりの距離を算出する
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

// 個々の文字列矩形から全体のバウンディングボックスを計算する
IGeometryPtr AheCalcStringBounds( const WKSPoint* pStringsBounds, long lStringCount, sindyAnnotationLineType emLineType, long lSecondLineStartIndex, ISpatialReference* ipSpRef )
{
	IGeometryPtr ipBound;	// 返り値

	// 引数チェック
	LOGASSERTE_IF( lStringCount > 0 && lSecondLineStartIndex >= 0 && lStringCount > lSecondLineStartIndex, sindyErr_ArgLimitOver )
	{
#ifdef SINDYLIB_CALC_TIME
		double dStartOneLine = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

		// 一行注記の場合は単純にConvexHullをバウンディングボックスとする
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
		// 二行注記の場合は一行目と二行目のORをバウンディングボックスとする
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
				// 一行目の最大最小を出す
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
				// 二行目の最大最小を出す
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
				// 隙間の最大・最小を出す
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
				// 3つの矩形のUNIONを取る
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
		// 線状注記の場合は前後一組のConvexHull
		else if( emLineType & sindyAnnoLine_Linear )
		{
#ifdef SINDYLIB_CALC_TIME
			double dStartLinerLine = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

			IPointCollectionPtr ipCol;
			IGeometryPtr ipBoundTmp;
#if 0 // ↓文字矩形2個をUNIONしてConvexHullを取る方法だと特定の形状が処理できない場合がある
			if( SUCCEEDED( ipCol.CreateInstance( CLSID_Polygon ) ) )
			{
				((IGeometryPtr)ipCol)->putref_SpatialReference( ipSpRef );
				for( long i = 0; i < lStringCount-1; ++i )
				{
					if( FAILED( ipCol->SetWKSPoints( 10, const_cast<WKSPoint*>(&(pStringsBounds[i*5])) ) ) ||	// 点列をセット
						FAILED( ((IPolygonPtr)ipCol)->SimplifyPreserveFromTo() ) ||								// ConvexHullするには必要
						FAILED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ) ) 				// ConvelHull
					{
						// 上記の方法は少々手抜きだが、個々のポリゴンをUNIONするより早い（はず）
						// でも、それぞれのボックスが重なっているとエラーが起きる
						// その場合は個々のポリゴンにしてからUNIONして、さらにConvexHullする
#ifdef _DEBUG
						for( long j = i*5; j < i*5+10; ++j )	ATLTRACE(_T("線状注記BOX : %3d : %.20lf %.20lf\n"), j - i*5, pStringsBounds[j].X, pStringsBounds[j].Y );
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
/*						for( long j = i*5; j < i*5+10; ++j )	ATLTRACE(_T("線状注記BOX : %3d : %.20lf %.20lf\n"), j - i*5, pStringsBounds[j].X, pStringsBounds[j].Y );
						ERRORLOG(sindyErr_GeometryFunctionFailed);
						SASSERT(false);*/
					}
					else {
						// UNIONを取る
						// 本当はConstructUnionしたいところだが、エラーが出る場合があるので
						// 個々でUNIONを取るようにする
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
				// 文字列のみで1文字の場合があるため場合わけ
				if( lStringCount == 1 )
				{
						LOGASSERTE_IF( SUCCEEDED( ipCol->SetWKSPoints( 5, const_cast<WKSPoint*>(&(pStringsBounds[0])) ) ) &&						// 点列をセット
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->Simplify() )	&&								// ConvexHullするには必要
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ), sindyErr_GeometryFunctionFailed ) // ConvelHull
						{
								ipBound = AheCreateClone( ipBoundTmp );
						}

				}
				else
				{
					for( long i = 0; i < lStringCount-1; ++i )
					{
						LOGASSERTE_IF( SUCCEEDED( ipCol->SetWKSPoints( 10, const_cast<WKSPoint*>(&(pStringsBounds[i*5])) ) ) &&						// 点列をセット
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->Simplify() ) &&												// ConvexHullするには必要
										SUCCEEDED( ((ITopologicalOperatorPtr)ipCol)->ConvexHull( &ipBoundTmp ) ), sindyErr_GeometryFunctionFailed ) // ConvelHull
						{
							// UNIONを取る
							// 本当はConstructUnionしたいところだが、エラーが出る場合があるので
							// 個々でUNIONを取るようにする
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

// 表示タイプとスケール表示ルールの論理性をチェックする
bool AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::ECode emDispTypeCode, schema::annotation_class::scaledisp_rule::ECode emScaleDispRuleCode )
{
	bool bRet = false;	// 返り値（抜けをなくすため、デフォルトはエラーとしておくこと）

	// チェックを追加する時の指針：
	// scaledisp_ruleに対してOKなschema::annotation::disp_typeを明示し、それ以外を
	// エラーとすること。
	// つまり、ここで記述されるschema::annotation::disp_typeに関してはOKなパターンのみと
	// する。それにより、今後コードが増えた場合にとりあえずエラーとして
	// 出力されるようになり、抜けがなくなる。
	switch( emScaleDispRuleCode )
	{
	case schema::annotation_class::scaledisp_rule::kOOOOO:					// 記号+文字列 ... 指示点化可能
	case schema::annotation_class::scaledisp_rule::kAny:					// なんでもあり
		bRet = true; break;
	case schema::annotation_class::scaledisp_rule::kOXOXO:					// 記号+文字列(指示点不可) ... 指示点化・記号のみ不可
	case schema::annotation_class::scaledisp_rule::kNotDirPointMustStr:		// ×   |記+文|    ×|文のみ|表示無	 … 指示点不可文字列必須
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kOOXXX:					// 記号のみ
	case schema::annotation_class::scaledisp_rule::kMarkOnly:				// 記号（指示点不可）のみ
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kOXOXX:					// 文字列のみ
	case schema::annotation_class::scaledisp_rule::kStrOnly:				// 文字列のみ
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kXXXXX:					// 表示無し
	case schema::annotation_class::scaledisp_rule::kNone:					// 表示無し
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
				bRet = true; break;
				break;
			default: break;
		}
		break;
	// 新しく追加されたルール
	case schema::annotation_class::scaledisp_rule::kNotDirPoint:			// ×   |記+文|記のみ|文のみ|表示無	 … 指示点不可
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
	case schema::annotation_class::scaledisp_rule::kAnyMust:				// 指+文|記+文|記のみ|文のみ|×		 … 何でも有り（表示必須）
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
	case schema::annotation_class::scaledisp_rule::kMustMark:				// 指+文|記+文|記のみ|    ×|表示無	 … 記号必須
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
	case schema::annotation_class::scaledisp_rule::kMustMarkMust:			// 指+文|記+文|記のみ|    ×|×		 … 記号必須（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustStr:				// 指+文|記+文|    ×|文のみ|表示無	 … 文字列必須
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
	case schema::annotation_class::scaledisp_rule::kMustStrMust:			// 指+文|記+文|    ×|文のみ|×		 … 文字列必須（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kStr:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustPair:				// 指+文|記+文|    ×|    ×|表示無	 … 記号と文字必ずペア
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMustPairMust:			// 指+文|記+文|    ×|    ×|×		 … 記号と文字必ずペア（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kPoiStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMust:		// ×   |記+文|記のみ|文のみ|×		 … 指示点不可（表示必須）
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
	case schema::annotation_class::scaledisp_rule::kNotDirPointMultMark:	// ×   |記+文|記のみ|    ×|表示無	 … 指示点不可記号必須
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMultMarkMust:// ×   |記+文|記のみ|    ×|×		 … 指示点不可記号必須（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kNotDirPointMustStrMust:	// ×   |記+文|    ×|文のみ|×		 … 指示点不可文字列必須（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
			case schema::annotation::disp_type::kStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkStr:				// ×   |記+文|    ×|    ×|表示無	 … 記号（指示点不可）+文字列のみ
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kNone:
			case schema::annotation::disp_type::kSymStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkStrMust:			// ×   |記+文|    ×|    ×|×		 … 記号（指示点不可）+文字列のみ（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSymStr:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kMarkOnlyMust:			// ×   |   ×|記のみ|    ×|×		 … 記号（指示点不可）のみ（表示必須）
		switch( emDispTypeCode )
		{
			case schema::annotation::disp_type::kSym:
				bRet = true; break;
			default: break;
		}
		break;
	case schema::annotation_class::scaledisp_rule::kStrOnlyMust:			// ×   |   ×|    ×|文のみ|×		 … 文字列のみ（表示必須）
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

// 表示タイプと上位の表示タイプの論理性をチェックする
bool AheCheckDispTypeAndUpperDispType( schema::annotation::disp_type::ECode emDispType, schema::annotation::disp_type::ECode emUpperDispType )
{
	// 中抜け状態での下位スケールへの影響度合いは
	// 以下の順番で決定する
	// 
	// 記号＋文字列    弱
	// 指示点＋文字列   
	// 文字列のみ
	// 記号のみ
	// 表示なし        強
	//
	// 例えば、S4で記号のみであれば、S3は記号のみか表示なししか選択できない。
	// S4で文字列のみであれば、S3では文字列のみか記号のみか表示なししか選択できない。
	bool bRet = false; // 返り値

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

// 各スケールの表示タイプの論理性をチェックする
bool AheCheckDispType( schema::annotation::disp_type::ECode emDispTypeS1, schema::annotation::disp_type::ECode emDispTypeS2,
					  schema::annotation::disp_type::ECode emDispTypeS3, schema::annotation::disp_type::ECode  emDispTypeS4, bool* pRetScale/* = NULL*/ )
{
	bool bRet = true; // 返り値

	// 初期化
	if( pRetScale )
		pRetScale[0] = pRetScale[1] = pRetScale[2] = pRetScale[3] = true;

	// S4のチェック
	if( schema::annotation::disp_type::kNone == emDispTypeS4 ) // 表示無しならエラー
	{
		bRet = false;
		if( pRetScale )	
			pRetScale[3] = false;
	}

	// S4以外のチェック
	// スルーしたときにエラーになるように初期値設定
	schema::annotation::disp_type::ECode emTargetDispType = schema::annotation::disp_type::kStr; // 現在のスケール
	schema::annotation::disp_type::ECode emUpperDispType = schema::annotation::disp_type::kNone;  // 上位のスケール
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
				pRetScale[i-1] = false; // S1 -> pErrScale[0]なので-1
		}
	}

	return bRet;
}

} // namespace sindy
