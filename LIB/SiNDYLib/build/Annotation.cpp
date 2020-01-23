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

// Annotation.cpp: CAnnotation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Annotation.h"
#include "AnnoGlobals.h"
#include "AnnotationClass.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;
using namespace annotation_class;
////////////////////////////////////////////////////////////////////////////////
/// チェックルーチン
////////////////////////////////////////////////////////////////////////////////
// 注記文字列１をチェックする
sindyErrCode CAnnotation::CheckString1() const
{
//#ifdef SINDYLIB_EXPORTS
	TCHAR strString[1024];
	return sindy::AheCheckOrFixAnnotationString( GetNameString1( 1024, strString ), GetStringNum1(), GetAnnoCode() );
/*#else
	return sindy::AheCheckOrFixAnnotationString( GetNameString1(), GetStringNum1(), GetAnnoCode() );
#endif // SINDYLIB_EXPORTS*/
}

// 注記文字列２をチェックする
sindyErrCode CAnnotation::CheckString2() const
{
//#ifdef SINDYLIB_EXPORTS
	TCHAR strString[1024];
	return sindy::AheCheckOrFixAnnotationString( GetNameString2( 1024, strString ), GetStringNum2(), GetAnnoCode() );
/*#else
	return sindy::AheCheckOrFixAnnotationString( GetNameString2(), GetStringNum2(), GetAnnoCode() );
#endif // SINDYLIB_EXPORTS*/
}

// 注記文字列３をチェックする
sindyErrCode CAnnotation::CheckString3() const
{
	// 文字列３のチェックは都市地図注記以外
	if( GetTableType() == sindyTableType::city_annotation )
		return sindyErr_NoErr;
	else {
//#ifdef SINDYLIB_EXPORTS
		TCHAR strString[1024];
		return sindy::AheCheckOrFixAnnotationString( GetNameString3( 1024, strString ), GetStringNum3(), GetAnnoCode(), false, true );
/*#else
	return sindy::AheCheckOrFixAnnotationString( GetNameString2(), GetStringNum2(), GetAnnoCode() );
#endif // SINDYLIB_EXPORTS*/
	}
}

// 注記文字列の長さをチェックする
sindyErrCode CAnnotation::CheckStringLen() const
{
	return ( GetStringNum1() < GetStringNum2() ) ? sindyErr_AnnoStrLenInvalid : sindyErr_NoErr;
}

// 注記種別をチェックする
sindyErrCode CAnnotation::CheckAnnoCode() const
{
	// ルールレコード取得
	const annotation_class::CAnnotationClass* pAnnoClass = g_cRule.GetAnnoRule( GetTableType(), GetAnnoCode() );

	// 都市地図用のルールかどうかをチェックする
	if( ! pAnnoClass )
		return sindyErr_AnnoCodeInvalid;
	else
		return sindyErr_NoErr;
}

// 表示タイプをチェックする
sindyErrCode CAnnotation::CheckDispType( sindyTableType::ECode emTableType ) const 
{
	return ( AheCheckDispTypeCodeAndScaleDispRule( GetDispType( emTableType ), CalcDispRule( emTableType ) ) ) ? sindyErr_NoErr : sindyErr_AnnoDispTypeInvalid;
}

// 文字列ポリゴンまでの距離をチェックする
sindyErrCode CAnnotation::CheckToStringBoundsLength( IGeometry* pBounds, sindyTableType::ECode emType, const double& dThresholdMinPx/* = 0.0*/, const double& dThresholdMaxPx/* = 16.0*/, double* pdDistance/* = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 引数チェック
	LOGASSERTEERR_IF( dThresholdMinPx <= dThresholdMaxPx && 0.0 <= dThresholdMaxPx, sindyErr_ArgIsConsistency )
	{
		if( CanShowMark( emType ) )
		{
			// 記号のみの場合に文字列ポリゴンがNULLになることを予想して、pBoundsがNULLでもエラーとしない
			if( pBounds )
			{
				// 閾値
				double d1PXLen = AheCalcDispLineType21PXLen( emType, true );
				double dMarkHalfLen = ( d1PXLen * (double)CalcMarkSize( emType ) ) / (double)2.0;	// 記号矩形の半分の距離（基本）
				double dThresholdMin = dMarkHalfLen + dThresholdMinPx * d1PXLen;					// 最小閾値
				double dThresholdMax = dMarkHalfLen + dThresholdMaxPx * d1PXLen;					// 最大閾値

				double dDistance = GetShape()->ReturnDistance( pBounds );
				if( ! ( dThresholdMin <= dDistance && dDistance <= dThresholdMax ) )
					emErr = sindyErr_AnnoToStringBoundsLengthLimitOver;

				if( pdDistance ) *pdDistance = dDistance;
			}
		}
	}

	return emErr;
}

// 記号矩形を計算する
sindyErrCode CAnnotation::CalcSymbolBounds( double dMarkLenX, double dMarkLenY, WKSEnvelope& cRect ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( dMarkLenX >= 0.0 && dMarkLenY >= 0.0 )
	{
		// 記号矩形を作成する
		CPoint cP( (IPointPtr)(IGeometry*)*GetShape() );
		cRect.XMin = cP.GetX() - dMarkLenX/2.0;
		cRect.YMin = cP.GetY() - dMarkLenY/2.0;
		cRect.XMax = cP.GetX() + dMarkLenX/2.0;
		cRect.YMax = cP.GetY() + dMarkLenY/2.0;
	}
	else
		emErr = sindyErr_ArgLimitOver;

	return emErr;
}

// 表示ルールを取得する
schema::annotation_class::scaledisp_rule::ECode CAnnotation::CalcDispRule( sindyTableType::ECode emTableType ) const
{
	schema::annotation_class::scaledisp_rule::ECode emScaleDispRuleCode = schema::annotation_class::scaledisp_rule::kNone; // 返り値

	const annotation_class::CAnnotationClass* pAnnoClass = g_cRule.GetAnnoRule( GetTableType(), GetAnnoCode() );
	if( pAnnoClass != NULL )
	{
		switch( emTableType )
		{
			case sindyTableType::city_disp_line: emScaleDispRuleCode = pAnnoClass->GetCDispRule(); break;
			case sindyTableType::c_sc1disp_line: emScaleDispRuleCode = pAnnoClass->GetCDispRule(); break; // 2006.08.10 F.Abe
			case sindyTableType::b_sc4disp_line: emScaleDispRuleCode = pAnnoClass->GetBS4DispRule(); break;
			case sindyTableType::b_sc3disp_line: emScaleDispRuleCode = pAnnoClass->GetBS3DispRule(); break;
			case sindyTableType::b_sc2disp_line: emScaleDispRuleCode = pAnnoClass->GetBS2DispRule(); break;
			case sindyTableType::b_sc1disp_line: emScaleDispRuleCode = pAnnoClass->GetBS1DispRule(); break;
			case sindyTableType::m_sc4disp_line: emScaleDispRuleCode = pAnnoClass->GetMS4DispRule(); break;
			case sindyTableType::m_sc3disp_line: emScaleDispRuleCode = pAnnoClass->GetMS3DispRule(); break;
			case sindyTableType::m_sc2disp_line: emScaleDispRuleCode = pAnnoClass->GetMS2DispRule(); break;
			case sindyTableType::m_sc1disp_line: emScaleDispRuleCode = pAnnoClass->GetMS1DispRule(); break;
			case sindyTableType::t_sc4disp_line: emScaleDispRuleCode = pAnnoClass->GetTS4DispRule(); break;
			case sindyTableType::t_sc3disp_line: emScaleDispRuleCode = pAnnoClass->GetTS3DispRule(); break;
			case sindyTableType::t_sc2disp_line: emScaleDispRuleCode = pAnnoClass->GetTS2DispRule(); break;
			case sindyTableType::t_sc1disp_line: emScaleDispRuleCode = pAnnoClass->GetTS1DispRule(); break;
			default: SASSERT(false); break;
		}
	}
	else {
		ERRORLOG(sindyErr_AnnoCodeInvalid);	// 種別が存在しない
		SASSERT( pAnnoClass != NULL );
	}

	return emScaleDispRuleCode;
}

#define ANNOTATION_CODE(name,id,size,label) case schema::annotation::annotation_code::name: nSize = size; break;

// 注記記号のピクセル数を取得する
int CAnnotation::CalcMarkSize( sindyTableType::ECode emTableType ) const
{
	int nSize = 16;	// 返り値

	switch( GetDispType( emTableType ) )
	{
		case schema::annotation::disp_type::kSymStr:
		case schema::annotation::disp_type::kSym:
		{
			// 通常の記号は種別表に定義されているサイズを使用する
			switch( GetAnnoCode() )
			{
#include <sindy/schema/annotationcode.h>
				default: LOGASSERT(false,sindyErr_AnnoCodeInvalid); break;
			}
			break;
		}
		case schema::annotation::disp_type::kPoiStr: nSize = 4;  break;	// 指示点
		default: nSize = 0; break;
	}
	return nSize;
}

#undef ANNOTATION_CODE

// 注記記号の一辺長を計算する
double CAnnotation::CalcMarkLength(  sindyTableType::ECode emTableType, bool bCalcX ) const
{
	return AheCalcDispLineType21PXLen( emTableType, bCalcX ) * (double)CalcMarkSize( emTableType );
}

// 表示タイプを取得する
schema::annotation_class::disptype_rule::ECode CAnnotation::CalcDispType() const
{
	schema::annotation_class::disptype_rule::ECode emCode = (schema::annotation_class::disptype_rule::ECode)-1;	// 返り値

	const annotation_class::CAnnotationClass* pAnnoClass = g_cRule.GetAnnoRule( GetTableType(), GetAnnoCode() );
	LOGASSERT_IF( pAnnoClass != NULL, sindyErr_AnnoCodeInvalid )
		emCode = pAnnoClass->GetDispType();

	return emCode;
}

// 重複しても良いかどうか
// POIと注記の統合を機に地下駅系も重複不可となった
bool CAnnotation::CanOverlap() const
{
	return false;

	//bool bRet = false;	// 返り値

	//switch( GetAnnoCode() )
	//{
	//case schema::annotation::annotation_code::kShinkansenSubSta:
	//case schema::annotation::annotation_code::kJRSubSta:
	//case schema::annotation::annotation_code::kQuasiPublicSubSta:
	//case schema::annotation::annotation_code::kPrivateLineSubSta:
	//case schema::annotation::annotation_code::kSubwaySubSta:
	//case schema::annotation::annotation_code::kMonorailSubSta:
	//case schema::annotation::annotation_code::kShinkotsuSubSta: bRet = true; break;
	//default: break;
	//}
	//return bRet;
}

// 注記記号を出すことができるかどうか
bool CAnnotation::CanShowMark( sindyTableType::ECode emTableType ) const
{
	bool bRet = false;	// 返り値

	switch( GetDispType( emTableType ) )
	{
		case schema::annotation::disp_type::kSymStr:
		case schema::annotation::disp_type::kPoiStr:
		case schema::annotation::disp_type::kSym:
			bRet = true;
			break;
		default:
			break;
	}
	return bRet;
}

// 注記文字列のみの表示かどうか
bool CAnnotation::CanShowStrOnly( sindyTableType::ECode emTableType ) const
{
	bool bRet = false;	// 返り値

	switch( GetDispType( emTableType ) )
	{
		case schema::annotation::disp_type::kStr:
			bRet = true;
			break;
		default:
			break;
	}
	return bRet;
}

// 表示するべきスケール値を取得する
sindyAnnotationScale CAnnotation::GetDispScale( int nLevel ) const
{
	sindyAnnotationScale nRet = (sindyAnnotationScale)-1;	// 返り値

	switch( nLevel )
	{
		case 0:
			switch( GetTableType() )
			{
				case sindyTableType::city_annotation:	nRet = sindyAnnoScale_city; break;
				case sindyTableType::base_annotation:	nRet = sindyAnnoScale_bsc4; break;
				case sindyTableType::middle_annotation:	nRet = sindyAnnoScale_msc4; break;
				case sindyTableType::top_annotation:	nRet = sindyAnnoScale_tsc4; break;
				default: LOGASSERT(false,sindyErr_UnknownTableType ); break;
			}
			break;
		case 1:
			switch( GetTableType() )
			{
				case sindyTableType::base_annotation:	nRet = sindyAnnoScale_bsc3; break;
				case sindyTableType::middle_annotation:	nRet = sindyAnnoScale_msc3; break;
				case sindyTableType::top_annotation:	nRet = sindyAnnoScale_tsc3; break;
				default: LOGASSERT(false,sindyErr_UnknownTableType ); break;
			}
			break;
		case 2:
			switch( GetTableType() )
			{
				case sindyTableType::base_annotation:	nRet = sindyAnnoScale_bsc2; break;
				case sindyTableType::middle_annotation:	nRet = sindyAnnoScale_msc2; break;
				case sindyTableType::top_annotation:	nRet = sindyAnnoScale_tsc2; break;
				default: LOGASSERT(false,sindyErr_UnknownTableType ); break;
			}
			break;
		case 3:
			switch( GetTableType() )
			{
				case sindyTableType::base_annotation:	nRet = sindyAnnoScale_bsc1; break;
				case sindyTableType::middle_annotation:	nRet = sindyAnnoScale_msc1; break;
				case sindyTableType::top_annotation:	nRet = sindyAnnoScale_tsc1; break;
				default: LOGASSERT(false,sindyErr_UnknownTableType ); break;
			}
			break;
		default: LOGASSERT(false, sindyErr_ArgLimitOver); break;
	}
	return nRet;
}

// 対照文字列コードに従って文字数を返す
long CAnnotation::GetStringNum( schema::disp_line::target_str::ECode emCode ) const
{
	switch( emCode )
	{
		case schema::disp_line::target_str::kStr1: return GetStringNum1(); break;
		case schema::disp_line::target_str::kStr2: return GetStringNum2(); break;
		case schema::disp_line::target_str::kStr3: return GetStringNum3(); break;
		default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // ターゲット文字列コードがおかしい
	}
	return -1;
}

// 対象文字列コードに従って注記文字列を返す
LPCTSTR CAnnotation::GetNameString( schema::disp_line::target_str::ECode emCode, int nLen, LPTSTR lpszName ) const
{
	switch( emCode )
	{
		case schema::disp_line::target_str::kStr1: return GetNameString1( nLen, lpszName ); break;
		case schema::disp_line::target_str::kStr2: return GetNameString2( nLen, lpszName ); break;
		case schema::disp_line::target_str::kStr3: return GetNameString3( nLen, lpszName ); break;
		default: ERRORLOG(sindyErr_AnnoDispTargetStrInvalid); break; // ターゲット文字列コードがおかしい
	}
	return (LPCTSTR)NULL;
}

// 適切なルールキャッシュを返す
const annotation_class::CAnnotationClass* CAnnotation::GetRule() const
{
	const annotation_class::CAnnotationClass* pRule = NULL;	// 返り値

	// ルール未取得なら取得する
	if( ! ( m_pAnnoRule || m_pAnnoRule2 ) )
	{
		// ルールキャッシュ
		schema::annotation::annotation_code::ECode emAnnoCode = GetAnnoCode();	// 注記種別
		const annotation_class::CAnnotationClassTable* pAnnoRuleTable = g_cRule.GetAnnotationClassTable();

		// ルールキャッシュが作成されていなければパラメータキャッシュも作成できない
		if( pAnnoRuleTable )
		{
			bool bIsMultiRule = ( emAnnoCode == schema::annotation::annotation_code::kSubwaySta || emAnnoCode == schema::annotation::annotation_code::kSubwaySubSta ) ? true : false;	// pAnnoRule2を使用するかどうか
			for( annotation_class::CAnnotationClassTable::const_iterator it = pAnnoRuleTable->begin(); it != pAnnoRuleTable->end(); ++it )
			{
				if( it->GetClassCode() == emAnnoCode )
				{
					if( ! m_pAnnoRule )
					{
						m_pAnnoRule = const_cast<CAnnotationClass*>(&(*it));
						if( ! bIsMultiRule ) break;
					}
					else {
						m_pAnnoRule2 = const_cast<CAnnotationClass*>(&(*it));
						break;
					}
				}
			}
			LOGASSERT_IF( m_pAnnoRule && ( ( bIsMultiRule && m_pAnnoRule2 ) || ( ! bIsMultiRule && ! m_pAnnoRule2 ) ), sindyErr_RuleNotFound );	// 確認
		}
	}

	schema::annotation::annotation_code::ECode emCode = GetAnnoCode();
	if( schema::annotation::annotation_code::kSubwaySta == emCode || schema::annotation::annotation_code::kSubwaySubSta == emCode )
	{
		if( m_pAnnoRule && m_pAnnoRule->IsValidRule( GetTableType() ) )
			pRule = m_pAnnoRule;
		else if( m_pAnnoRule2 && m_pAnnoRule2->IsValidRule( GetTableType() ) )
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
