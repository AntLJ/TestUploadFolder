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

// AnnotationClass.cpp: CAnnotationClass クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnnotationClass.h"
#include "AnnoGlobals.h"
#include "sindycomutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace annotation_class {
using namespace errorcode;

//  表示スケールにあった表示ルールコードを取得する
schema::annotation_class::scaledisp_rule::ECode CAnnotationClass::GetDispRule( sindyTableType::ECode emTableType ) const
{
	switch( emTableType )
	{
		case sindyTableType::city_disp_line: return GetCDispRule(); break;
		case sindyTableType::c_sc1disp_line: return GetCDispRule(); break;
		case sindyTableType::b_sc4disp_line: return GetBS4DispRule(); break;
		case sindyTableType::b_sc3disp_line: return GetBS3DispRule(); break;
		case sindyTableType::b_sc2disp_line: return GetBS2DispRule(); break;
		case sindyTableType::b_sc1disp_line: return GetBS1DispRule(); break;
		case sindyTableType::m_sc4disp_line: return GetMS4DispRule(); break;
		case sindyTableType::m_sc3disp_line: return GetMS3DispRule(); break;
		case sindyTableType::m_sc2disp_line: return GetMS2DispRule(); break;
		case sindyTableType::m_sc1disp_line: return GetMS1DispRule(); break;
		case sindyTableType::t_sc4disp_line: return GetTS4DispRule(); break;
		case sindyTableType::t_sc3disp_line: return GetTS3DispRule(); break;
		case sindyTableType::t_sc2disp_line: return GetTS2DispRule(); break;
		case sindyTableType::t_sc1disp_line: return GetTS1DispRule(); break;
		default:
			ERRORLOG( sindyErr_UnknownTableType );
			SASSERT(false);
			break;
	}
	return (schema::annotation_class::scaledisp_rule::ECode)-1;
}

// フォントサイズを取得する
int CAnnotationClass::GetFontSize( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:     return GetCFontSize(); break;
		case sindyTableType::base_annotation:   return GetBFontSize(); break;
		case sindyTableType::middle_annotation: return GetMFontSize(); break;
		case sindyTableType::top_annotation:    return GetTFontSize(); break;
		default:
			ERRORLOG( sindyErr_UnknownTableType );
			SASSERT(false);
			break;
	}
	return -1;
}

// 表示スケールに合ったコピーペーストフラグを取得する
bool CAnnotationClass::GetCopyPaste( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:	
		case sindyTableType::d1_annotation:		return GetCCopyPaste(); break;
		case sindyTableType::base_annotation:	return GetBCopyPaste(); break;
		case sindyTableType::middle_annotation:	return GetMCopyPaste(); break;
		case sindyTableType::top_annotation:	return GetTCopyPaste(); break;
		default:
			ERRORLOG( sindyErr_UnknownTableType );
			SASSERT(false);
			break;
	}
	return false;
}

// 表示スケールに合った中抜けフラグを取得する
bool CAnnotationClass::GetExceptScale( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:		return GetCExceptScale(); break;
		case sindyTableType::base_annotation:	return GetBExceptScale(); break;
		case sindyTableType::middle_annotation:	return GetMExceptScale(); break;
		case sindyTableType::top_annotation:	return GetTExceptScale(); break;
		default:
			ERRORLOG( sindyErr_UnknownTableType );
			SASSERT(false);
			break;
	}
	return false;
}

// 同一データの存在フラグを取得する
bool CAnnotationClass::GetIDenticalData( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:		return GetCIDenticalData(); break;
		case sindyTableType::base_annotation:	return GetBIDenticalData(); break;
		case sindyTableType::middle_annotation:	return GetMIDenticalData(); break;
		case sindyTableType::top_annotation:	return GetTIDenticalData(); break;
		default:
			ERRORLOG( sindyErr_UnknownTableType );
			SASSERT(false);
			break;
	}
	return false;
}

// 真位置フラグを取得する
bool CAnnotationClass::GetTruePosition( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:     return GetCTruePosition(); break;
		case sindyTableType::base_annotation:   return GetBTruePosition(); break;
		case sindyTableType::middle_annotation: return GetMTruePosition(); break;
		case sindyTableType::top_annotation:    return GetTTruePosition(); break;
		default:
			return false;
			break;
	}
}

// ２行注記許可フラグを取得する
bool CAnnotationClass::GetDoubleAnno( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:     return GetCDoubleAnno(); break;
		case sindyTableType::base_annotation:   return GetBDoubleAnno(); break;
		case sindyTableType::middle_annotation: return GetMDoubleAnno(); break;
		case sindyTableType::top_annotation:    return GetTDoubleAnno(); break;
		default:
			return false;
			break;
	}
}

// 渡されたテーブルタイプで使用可能なルールかどうかをチェックする
bool CAnnotationClass::IsValidRule( sindyTableType::ECode emTableType ) const
{
	sindyTableType::ECode emPointType = AheTableType2AnnoPointTableType( emTableType );
	switch( emPointType )
	{
		case sindyTableType::city_annotation:
		case sindyTableType::d1_annotation:     return GetCityFlag(); break;
		case sindyTableType::base_annotation:   return GetBaseFlag(); break;
		case sindyTableType::middle_annotation: return GetMiddleFlag(); break;
		case sindyTableType::top_annotation:    return GetTopFlag(); break;
		default:
			return false;
			break;
	}
}

} // annotation_class

} // sindy
