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
 * @file AnnotationClass.h
 * @brief CAnnotationClassクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ANNOTATIONCLASS_H_
#define _ANNOTATIONCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "AnnotationCommunity.h"
#include "RuleCommunity.h"

namespace sindy {
namespace annotation_class {
/**
 * @class CAnnotationClass
 * @brief <b>注記ルールレコードラッパークラス</b>\n
 * このクラスは注記ルールレコード（ANNOTATION_CLASS）のラッパークラスです。
 * このクラスを通じて、全てのフィールド値の取得・設定が可能です。
 * なお、このクラスにはこのクラス特有のメソッドしか定義されていませんが、
 * 継承元のクラスでそれ以外のすべてのメソッドが定義されています。
 */
class SINDYLIB_API CAnnotationClass : public CRow  
{
public:
	ROWCLASS_CONSTRUCTOR( CAnnotationClass, annotation_class )
	//@{ @name 属性取得関数
	/**
	 * @brief SiNDY注記種別コードを取得する
	 * @retval NULLの時は-1
	 * @retval NULL以外の時はannotation_code::ECode
	 */
	 schema::annotation::annotation_code::ECode GetClassCode() const
	{
		return (schema::annotation::annotation_code::ECode)GetLongValueByFieldName( schema::annotation_class::kClass, -1 );
	}
	
	/**
	 * @brief 原稿番号を取得する
	 * @param nLen		[in]	lpszMsNoの収納可能文字列数
	 * @param lpszMsNo	[out]	原稿番号格納バッファ
	 * @return 原稿番号文字列
	 */
	LPCTSTR GetMSNo( int nLen, LPTSTR lpszMsNo ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kMSNo, nLen, lpszMsNo );
	}

	/**
	 * @brief IN注記種別コードを取得する
	 * @param nLen		[in]	lpszClassの収納可能文字列数
	 * @param lpszClass	[out]	IN注記種別コード格納バッファ
	 * @return IN注記種別コード文字列
	 */
	LPCTSTR GetINClass( int nLen, LPTSTR lpszClass ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kInClass, nLen, lpszClass );
	}

	/**
	 * @brief 種別名称を取得する
	 * @param nLen			[in]	lpszClassNameの収納可能文字列数
	 * @param lpszClassName	[out]	種別名称格納バッファ
	 * @return 種別名称文字列
	 */
	LPCTSTR GetClassName( int nLen, LPTSTR lpszClassName ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kClassName, nLen, lpszClassName );
	}

	/**
	 * @brief TOP_FLAGを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTopFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTopFlag, false );
	}

	/**
	 * @brief MIDDLE_FLAGを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMiddleFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMiddleFlag, false );
	}

	/**
	 * @brief BASE_FLAGを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBaseFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBaseFlag, false );
	}

	/**
	 * @brief CITY_FLAGを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCityFlag() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCityFlag, false );
	}

	/**
	 * @brief 表示タイプコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::disptype_rule::ECode GetDispType() const
	{
		return (schema::annotation_class::disptype_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kDispType, -1 );
	}

	/**
	 * @brief トップマップS1表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS1DispRule, -1 );
	}

	/**
	 * @brief トップマップS2表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS2DispRule, -1 );
	}

	/**
	 * @brief トップマップS3表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS3DispRule, -1 );
	}

	/**
	 * @brief トップマップS4表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetTS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kTS4DispRule, -1 );
	}

	/**
	 * @brief トップマップ真位置フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTTruePosition, false );
	}

	/**
	 * @brief トップマップフォントサイズを取得する
	 * @retval -1 NULLまたは取得失敗
	 */
	int GetTFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kTFontSize, -1 );
	}

	/**
	 * @brief トップマップスケールの中抜けフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTExceptScale, false );
	}

	/**
	 * @brief トップマップ２行注記フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTDoubleAnno, false );
	}

	/**
	 * @brief トップマップコピーペーストフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTCopyPaste, false );
	}

	/**
	 * @brief ミドルマップS1表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS1DispRule, -1 );
	}

	/**
	 * @brief ミドルマップS2表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS2DispRule, -1 );
	}

	/**
	 * @brief ミドルマップS3表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS3DispRule, -1 );
	}

	/**
	 * @brief ミドルマップS4表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetMS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kMS4DispRule, -1 );
	}

	/**
	 * @brief ミドルマップ真位置フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMTruePosition, false );
	}

	/**
	 * @brief ミドルマップフォントサイズを取得する
	 * @retval -1 NULLまたは取得失敗
	 */
	int GetMFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kMFontSize, -1 );
	}

	/**
	 * @brief ミドルマップスケールの中抜けフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMExceptScale, false );
	}

	/**
	 * @brief ミドルマップ２行注記フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMDoubleAnno, false );
	}

	/**
	 * @brief ミドルマップコピーペーストフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMCopyPaste, false );
	}

	/**
	 * @brief ミドルマップ同一データの存在フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetMIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kMIDenticalData, false );
	}

	/**
	 * @brief トップマップ同一データの存在フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kTIDenticalData, false );
	}

	/**
	 * @brief ベースマップS1表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS1DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS1DispRule, -1 );
	}

	/**
	 * @brief ベースマップS2表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS2DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS2DispRule, -1 );
	}

	/**
	 * @brief ベースマップS3表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS3DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS3DispRule, -1 );
	}

	/**
	 * @brief ベースマップS4表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetBS4DispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kBS4DispRule, -1 );
	}

	/**
	 * @brief ベースマップ真位置フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBTruePosition, false );
	}

	/**
	 * @brief ベースマップフォントサイズを取得する
	 * @retval -1 NULLまたは取得失敗
	 */
	int GetBFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kBFontSize, -1 );
	}

	/**
	 * @brief ベースマップスケールの中抜けフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBExceptScale, false );
	}

	/**
	 * @brief ベースマップ２行注記フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBDoubleAnno() const
	{
		return GetBoolValueByFieldName(	schema::annotation_class::kBDoubleAnno, false );
	}

	/**
	 * @brief ベースマップコピーペーストフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBCopyPaste, false );
	}

	/**
	 * @brief ベースマップ同一データの存在フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetBIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kBIDenticalData, false );
	}

	/**
	 * @brief 都市地図表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetCDispRule() const
	{
		return (schema::annotation_class::scaledisp_rule::ECode)GetLongValueByFieldName( schema::annotation_class::kCDispRule, -1 );
	}

	/**
	 * @brief 都市地図真位置フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCTruePosition() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCTruePosition, false );
	}

	/**
	 * @brief 都市地図フォントサイズを取得する
	 * @retval -1 NULLまたは取得失敗
	 */
	int GetCFontSize() const
	{
		return GetLongValueByFieldName( schema::annotation_class::kCFontSize, -1 );
	}

	/**
	 * @brief 都市地図スケールの中抜けフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCExceptScale() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCExceptScale, false );
	}

	/**
	 * @brief 都市地図２行注記フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCDoubleAnno() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCDoubleAnno, false );
	}

	/**
	 * @brief 都市地図コピーペーストフラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCCopyPaste() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCCopyPaste, false );
	}

	/**
	 * @brief 都市地図同一データの存在フラグを取得する
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetCIDenticalData() const
	{
		return GetBoolValueByFieldName( schema::annotation_class::kCIDenticalData, false );
	}

	/**
	 * @brief 回転ルール文字列を取得する
	 * @param nLen			[in]	lpszRuleの収納可能文字列数
	 * @param lpszRule		[out]	回転ルール文字列格納バッファ
	 * @return 回転ルール文字列
	 */
	LPCTSTR GetRotationRule( int nLen, LPTSTR lpszRule ) const
	{
		return GetStringValueByFieldName( schema::annotation_class::kRotationRule, nLen, lpszRule );
	}
	//@}

	/**
	 * @brief 表示スケールにあった表示ルールコードを取得する
	 * @return エラー又はNULLの場合は-1
	 */
	schema::annotation_class::scaledisp_rule::ECode GetDispRule( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief フォントサイズを取得する
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval -1 NULLまたは取得失敗
	 */
	int GetFontSize( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>表示スケールに合ったコピーペーストフラグを取得する</b>\n
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval false コピーペースト不可、又は値がNULL、または取得失敗
	 * @retval true コピーペースト可
	 */
	bool GetCopyPaste( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>表示スケールに合った中抜けフラグを取得する</b>\n
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval false 中抜け不可、又は値がNULL、または取得失敗
	 * @retval true 中抜け可
	 */
	bool GetExceptScale( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief 同一データの存在フラグを取得する
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetIDenticalData( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief 真位置フラグを取得する
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetTruePosition( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief ２行注記許可フラグを取得する
	 * @param emTableType	[in]	真位置又は表示文字列レイヤのテーブルタイプ
	 * @retval false 0又はNULL、または取得失敗
	 */
	bool GetDoubleAnno( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>渡されたテーブルタイプで使用可能なルールかどうかをチェックする</b>\n
	 * @retval true 使用可能なルール
	 * @retval false 使用不可能なルール
	 */
	bool IsValidRule( sindyTableType::ECode emTableType ) const;

};

} // annotation_class

} // sindy

#endif // _ANNOTATIONCLASS_H_
