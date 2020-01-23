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
 * @file AbleLayersTable.h
 * @brief <b>CAbleLayersTableクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ABLELAYERS_H_
#define _ABLELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"
#include "AttributeRistrict.h"

namespace sindy {
/**
 * @class CAbleLayers
 * @brief <b>属性制限付き編集可能レイヤ管理レコード用抽象クラス</b>\n
 * このクラスは抽象クラスです。
 */
class SINDYLIB_API CAbleLayers : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CAbleLayers, unknown )
	/**
	 * @brief <b>属性・形状などのキャッシュを作成する</b>\n
	 * フィールド対応表、テーブル名キャッシュオブジェクトが無ければ作成してから作成しますが、
	 * その場合ほかのオブジェクトと共有できないためリソースを多く必要とします。
	 * リソースがどうでもよい場合、及びオブジェクトが少ない場合を除いて極力フィールド対応表
	 * とテーブル名キャッシュオブジェクトを渡すようにしてください。\n
	 * 既にキャッシュが存在する場合はキャッシュを作成しません。また、
	 * bDetatchをtrueにすると、キャッシュ作成後に
	 * _IRow*を切り離します。
	 * @param bDetach			[in,optional]	処理後_IRow*を切り離す場合はtrue
	 * @param ipFields			[in,optional]	フィールド対応表オブジェクト(shared_ptr)
	 * @param spNameString		[in,optional]	テーブル名等管理オブジェクト(shared_ptr)
	 * @return sindyErrCode;
	 */
	virtual errorcode::sindyErrCode CreateCache( bool bDetatch = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>渡されたオブジェクトが編集可能かどうかチェックする</b>\n
	 * @param cRow			[in]	チェック対象オブジェクト
	 * @param nLen			[in]	lpszMessage収納可能文字数
	 * @param lpszMessage	[out]	チェック結果メッセージ
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>属性制限1を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict1( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限2を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict2( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限3を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict3( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限4を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict4( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限5を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict5( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限6を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict6( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限7を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict7( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限8を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict8( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限9を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict9( int nLen, LPTSTR lpszRistrict ) const = 0;

	/**
	 * @brief <b>属性制限10を取得する</b>\n
	 * @param nLen			[in]	lpszRistrictの収納可能文字列数
	 * @param lpszRistrict	[out]	取得文字列を格納するバッファ
	 * @retval LPCTSTR 属性制限
	 * @retval NULL エラー又はNULL
	 */
	virtual LPCTSTR GetRistrict10( int nLen, LPTSTR lpszRistrict ) const = 0;

	//@{ @name 属性設定関数
	/**
	 * @brief <b>属性制限1を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict1( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限2を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict2( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限3を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict3( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限4を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict4( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限5を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict5( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限6を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict6( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限7を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict7( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限8を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict8( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限9を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict9( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>属性制限10を設定する</b>\n
	 * @param lpcszRictrict	[in]	属性制限文字列
	 * @reurn sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetRistrict10( LPCTSTR lpcszRistrict ) = 0;
	//@}
protected:
	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 */
	void DeleteCache( bool bDeleteContainerCache = false );
private:
	CAttributeRistrictsOr	m_cRistrict;	//!< 属性制限キャッシュ用
};

} // sindy

#endif // _ABLELAYERS_H_
