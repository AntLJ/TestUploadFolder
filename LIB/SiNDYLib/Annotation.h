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
 * @file Annotation.h
 * @brief<b>CAnnotationクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ANNOTATION_H_
#define _ANNOTATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "AnnotationCommunity.h"
#include "RuleCommunity.h"
#include "TableType.h"
#include "AheWKSFunctions.h"

namespace sindy {

namespace annotation_class {
class CAnnotationClass;
};

/**
 * @class CAnnotation
 * @brief <b>注記真位置フィーチャ抽象クラス</b>\n
 * このクラスは注記真位置として持つべきインターフェースのみ
 * 持った抽象クラスです。
 */
class SINDYLIB_API CAnnotation : public CFeature
{
public:
	explicit CAnnotation()  : m_pAnnoRule(NULL), m_pAnnoRule2(NULL){}
	virtual ~CAnnotation(){}

	CAnnotation( sindyTableType::ECode emTableType, IFeature* lp ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		SetObject( lp, emTableType );
	}
	
	CAnnotation( sindyTableType::ECode emTableType, _IRow* lp ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		SetObject( lp, emTableType );
	}

	explicit CAnnotation( const CAnnotation& obj ) : m_pAnnoRule(NULL), m_pAnnoRule2(NULL)
	{
		*this = obj;
	}
	
	CAnnotation& operator=( const CAnnotation& obj )
	{
		obj.CopyTo( *this );
		m_pAnnoRule = obj.m_pAnnoRule;
		m_pAnnoRule2 = obj.m_pAnnoRule2;

		return *this;
	}

	//@{ @name チェック関数
	/**
	 * @brief <b>注記文字列１をチェックする</b>\n
	 * ルールに沿った文字列のチェック、及び
	 * GetStringNum1との整合性をチェックします。
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString1() const;

	/**
	 * @brief <b>注記文字列２をチェックする</b>\n
	 * ルールに沿った文字列のチェック、及び
	 * GetStringNum2との整合性をチェックします。
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString2() const;

	/**
	 * @brief <b>注記文字列３をチェックする</b>\n
	 * ルールに沿った文字列のチェック、及び
	 * GetStringNum3との整合性をチェックします。
	 * @see sindy::CheckOrFixAnnotationString
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckString3() const;

	/**
	 * @brief<b>注記文字列の長さをチェックする</b>\n
	 * [rule_check_1_3_4_5E] 注記文字列は名称1>=名称2でなければならない\n
	 * [rule_check_1_3_4_4E] 名称1が存在しないのに名称2が存在する
	 * （このチェックは上のチェックとCheckString1、CheckString2でまかなえるので不要）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckStringLen() const;

	/**
	 * @brief <b>注記種別をチェックする</b>\n
	 * [rule_check_1_3_4_10E] 定義されていない種別コードであればエラー（ルールテーブルに登録されているコードと比較する）\n
	 * 注記種別が都市地図用の種別かどうかをチェックします。
	 * チェックするために注記用ルールテーブルを参照します。
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckAnnoCode() const;

	/**
	 * @brief <b>表示タイプをチェックする</b>\n
	 * 表示タイプと種別の整合が取れているかどうかをチェックします。
	 * チェックするために注記用ルールテーブルを参照します。
	 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
	 * @retval sindyErr_NoErr	整合が取れている
	 * @retval sindyErr_AnnoDispTypeInvalid 整合が取れていない
	 */
	errorcode::sindyErrCode CheckDispType( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>文字列ポリゴンまでの距離をチェックする</b>\n
	 * @paarm pBounds			[in]			文字列ポリゴン（旗揚げ無し形状）
	 * @param emType			[in]			表示位置レイヤテーブルタイプ
	 * @param dThresholdMinPx	[in,optional]	最小閾値（単位：ピクセル、マイナス可、デフォルト：0.0）
	 * @param dThresholdMaxPx	[in,optional]	最大閾値（単位：ピクセル、デフォルト：16.0）
	 * @param pdDistance		[out,optional]	判定距離が返る
	 * @retval sindyErr_NoErr								エラー無し
	 * @retval sindyErr_AnnoToStringBoundsLengthLimitOver	距離が想定外
	 */
	errorcode::sindyErrCode CheckToStringBoundsLength( IGeometry* pBounds, sindyTableType::ECode emType, const double& dThresholdMinPx = 0.0, const double& dThresholdMaxPx = 16.0, double* pdDistance = NULL ) const;
	//@}

	//@{ @name 属性取得関数
	/**
	 * @brief <b>注記文字列１を返す</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameString1( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString1, nLen, lpszName );
	}

	/**
	 * @brief <b>注記文字列２を返す</b>\n
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameString2( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString2, nLen, lpszName );
	}

	/**
	 * @brief <b>注記文字列３を返す</b>\n
	 * この関数は中縮以上のレイヤとの互換性を保つものであり、都市地図の場合は常にエラーになります。
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameString3( int nLen, LPTSTR lpszName ) const
	{
		return GetStringValueByFieldName( schema::annotation::kNameString3, nLen, lpszName );
	}

	/**
	 * @brief <b>対象文字列コードに従って注記文字列を返す</b>\n
	 * @param emCode	[in]	対象文字列コード
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又はNULLの場合は空文字列
	 */
	LPCTSTR GetNameString( schema::disp_line::target_str::ECode emCode, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>注記１文字数を返す</b>\n
	 * @retval -1 エラー又は値がNULL
	 * @retval -1以外 注記１文字数
	 */
	long GetStringNum1() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum1, -1 );
	}

	/**
	 * @brief <b>注記２文字数を返す</b>\n
	 * @retval -1 エラー又は値がNULL
	 * @retval -1以外 注記２文字数
	 */
	long GetStringNum2() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum2, -1 );
	}

	/**
	 * @brief <b>注記３文字数を返す</b>\n
	 * この関数は中縮以上のレイヤとの互換性を保つものであり、都市地図の場合は常に-1になります。
	 * @retval -1 エラー又は値がNULL
	 * @retval -1以外 注記２文字数
	 */
	long GetStringNum3() const
	{
		return GetLongValueByFieldName( schema::annotation::kStringNum3, -1 );
	}

	/**
	 * @brief <b>対象文字列コードに従って文字数を返す</b>\n
	 * @param emCode	[in]	対象文字列コード
	 * @retval -1 エラー又は値がNULL
	 * @retval -1以外 注記２文字数
	 */
	long GetStringNum( schema::disp_line::target_str::ECode emCode ) const;

	/**
	 * @brief <b>注記種別を返す</b>\n
	 * @retval -1 エラー又はNULL
	 */
	schema::annotation::annotation_code::ECode GetAnnoCode() const
	{
		return (schema::annotation::annotation_code::ECode)GetLongValueByFieldName( schema::annotation::kAnnoCode, -1 );
	}

	/**
	 * @brief <b>表示タイプを返す</b>\n
	 * @param emTableType	[in]	テーブルタイプ
	 * @retval -1 エラー又はNULL
	 * @retval -1以外 disptype_code
	 */
	schema::annotation::disp_type::ECode GetDispType( sindyTableType::ECode emTableType ) const
	{
		switch( emTableType )
		{
			case sindyTableType::city_disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kDispType, -1 );
			case sindyTableType::c_sc1disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kDispType, -1 );
			case sindyTableType::b_sc4disp_line: 
			case sindyTableType::m_sc4disp_line: 
			case sindyTableType::t_sc4disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC4DispType, -1 );
			case sindyTableType::b_sc3disp_line: 
			case sindyTableType::m_sc3disp_line: 
			case sindyTableType::t_sc3disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC3DispType, -1 );
			case sindyTableType::b_sc2disp_line: 
			case sindyTableType::m_sc2disp_line: 
			case sindyTableType::t_sc2disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC2DispType, -1 );
			case sindyTableType::b_sc1disp_line: 
			case sindyTableType::m_sc1disp_line: 
			case sindyTableType::t_sc1disp_line: return (schema::annotation::disp_type::ECode)GetLongValueByFieldName( schema::annotation::kSC1DispType, -1 );
		}
		return (schema::annotation::disp_type::ECode)-1;
	}
	//@}

	/**
	 * @brief <b>記号矩形を計算する</b>\n
	 * @param dMarkLen	[in]	記号一辺長（単位：緯度経度）
	 * @return 矩形
	 */
	IEnvelopePtr CalcSymbolBounds( double dMarkLenX, double dMarkLenY ) const {
		WKSEnvelope cRect;
		CalcSymbolBounds( dMarkLenX, dMarkLenY, cRect );
		IEnvelopePtr ipEnv( CLSID_Envelope );
		ipEnv->PutCoords( cRect.XMin, cRect.YMin, cRect.XMax, cRect.YMax );
		return ipEnv;
	}
	/**
	 * @brief <b>記号矩形を計算する</b>\n
	 * @param dMarkLenX	[in]	X方向記号一辺長（単位：緯度経度）
	 * @param dMarkLenY	[in]	Y方向記号一辺長（単位：緯度経度）
	 * @param cRect		[out]	記号矩形
	 * return sindyErrCode
	 */
	errorcode::sindyErrCode CalcSymbolBounds( double dMarkLenX, double dMarkLenY, WKSEnvelope& cRect ) const;

	/**
	 * @brief <b>表示ルールを取得する</b>\n
	 * 注記ルールから適合するものを検索し、返します。
	 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
	 * @return 表示ルール
	 */
	schema::annotation_class::scaledisp_rule::ECode CalcDispRule( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>注記記号のピクセル数を取得する</b>\n
	 * 種別ごとの記号サイズは種別表に定義されています。
	 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
	 * @return ピクセル数（e.x. 16 が帰ってきたら 16x16の記号矩形であるということ）
	 */
	int CalcMarkSize( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>注記記号の一辺長を計算する</b>\n
	 * @see AheCalcDispLineType21PXLen
	 * @see CalcMarkSize
	 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
	 * @param bCalcX		[in]	X座標値を取得するならtrue、Y座標値を取得するならfalse
	 * @return 記号一辺長（単位：度（緯度経度））
	 */
	double CalcMarkLength( sindyTableType::ECode emTableType, bool bCalcX ) const;

	/**
	 * @brief <b>表示タイプを取得する</b>\n
	 * 注記ルールから取得します。
	 * @retval -1 エラー（種別が存在しない）
	 * @return disptypecode_code::ECode
	 */
	schema::annotation_class::disptype_rule::ECode CalcDispType() const;

	/**
	 * @brief <b>重複しても良いかどうか</b>\n
	 * @retval true 重複可能
	 * @retval false 重複不可能
	 */
	bool CanOverlap() const;

	/**
	 * @brief <b>注記記号を出すことができるかどうか</b>\n
	 * 現在の表示タイプの属性のみで判断します。表示ルールとの整合性は
	 * チェックしません。\n
	 * 表示ルールとの整合性はCheckDispTypeCodeでチェックすることができます。
	 * @param emTableType	[in]	表示位置レイヤのタイプ
	 * @retval true 記号表示可能
	 * @retval false 記号表示不可
	 */
	bool CanShowMark( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>注記文字列のみの表示かどうか</b>\n
	 * 現在の表示タイプの属性のみで判断します。表示ルールとの整合性は
	 * チェックしません。\n
	 * 表示ルールとの整合性はCheckDispTypeCodeでチェックすることができます。
	 * @param emTableType	[in]	表示位置レイヤのタイプ
	 * @retval true 文字列のみの表示
	 * @retval false 文字列のみの表示ではない
	 */
	bool CanShowStrOnly( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>表示するべきスケール値を取得する</b>\n
	 * @param nLevel	[in]	0:S4（都市地図は0のみ） 1:S3 2:S2 3:S1
	 * @retval -1 エラー
	 * @retval -1以外 スケール
	 */
	sindyAnnotationScale GetDispScale( int nLevel ) const;

	/**
	 * @brief <b>ルールキャッシュを返す</b>\n
	 * 地下鉄駅が地上と地下とで異なるルールを使用するため、ルールのキャッシュが
	 * 2つあることへの対応です。
	 * @retval NULL		エラー
	 * @retval NULL以外	ルールレコードオブジェクトのポインタ
	 */
	const annotation_class::CAnnotationClass* GetRule() const;


protected:
	mutable annotation_class::CAnnotationClass*	m_pAnnoRule;			//!< 注記ルール
	mutable annotation_class::CAnnotationClass*	m_pAnnoRule2;			//!< 注記ルール（地下鉄駅（地上）および地下鉄駅（地下）はスケールによってルールが変わるため、そのためのルール） <- なんとかしてー
};

} // sindy

#endif // _ANNOTATION_H_
