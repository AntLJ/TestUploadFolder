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
 * @file RelAnnotation.h
 * @brief <b>CRelAnnotationクラス定義ヘッダファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @vertion $Id$
 */

#ifndef _RELANNOTATION_H_
#define _RELANNOTATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "Annotation.h"
#include "DispLine.h"

namespace sindy {

class CErrorObjects;

namespace annotation_class {
class CAnnotationClass;
} // namespace annotation_class

extern double g_dTotalSetDispLineTime;	//!< SetDispLineにかかる総時間

class DispIterator
{
public:
	DispIterator( sindyTableType::ECode __T, bool __r = false )
	{
		__reverce = __r;
		switch( __T )
		{
			case sindyTableType::city_annotation:	__disp = sindyTableType::city_disp_line; break;
			case sindyTableType::d1_annotation:		__disp = sindyTableType::c_sc1disp_line; break;
			case sindyTableType::base_annotation:	__disp = ( __reverce ) ? sindyTableType::b_sc1disp_line : sindyTableType::b_sc4disp_line; break;
			case sindyTableType::middle_annotation:	__disp = ( __reverce ) ? sindyTableType::m_sc1disp_line : sindyTableType::m_sc4disp_line; break;
			case sindyTableType::top_annotation:	__disp = ( __reverce ) ? sindyTableType::t_sc1disp_line : sindyTableType::t_sc4disp_line; break;
			default: __disp = sindyTableType::unknown; break;
		}
	}
	DispIterator( sindyAnnotationGroup __T, bool __r = false )
	{
		__reverce = __r;
		switch( __T )
		{
			case sindyAnnoGroup_city:	__disp = sindyTableType::city_disp_line; break;
			case sindyAnnoGroup_csc1:	__disp = sindyTableType::c_sc1disp_line; break;
			case sindyAnnoGroup_base:	__disp = ( __reverce ) ? sindyTableType::b_sc1disp_line : sindyTableType::b_sc4disp_line; break;
			case sindyAnnoGroup_middle:	__disp = ( __reverce ) ? sindyTableType::m_sc1disp_line : sindyTableType::m_sc4disp_line; break;
			case sindyAnnoGroup_top:	__disp = ( __reverce ) ? sindyTableType::t_sc1disp_line : sindyTableType::t_sc4disp_line; break;
			default: __disp = sindyTableType::unknown; break;
		}
	}
	const DispIterator& operator++() {
		switch( __disp )
		{
			case sindyTableType::b_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::b_sc3disp_line; break;
			case sindyTableType::b_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc4disp_line : sindyTableType::b_sc2disp_line; break;
			case sindyTableType::b_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc3disp_line : sindyTableType::b_sc1disp_line; break;
			case sindyTableType::b_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::b_sc2disp_line : sindyTableType::unknown; break;
			case sindyTableType::m_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::m_sc3disp_line; break;
			case sindyTableType::m_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc4disp_line : sindyTableType::m_sc2disp_line; break;
			case sindyTableType::m_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc3disp_line : sindyTableType::m_sc1disp_line; break;
			case sindyTableType::m_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::m_sc2disp_line : sindyTableType::unknown; break;
			case sindyTableType::t_sc4disp_line: __disp = ( __reverce ) ? sindyTableType::unknown        : sindyTableType::t_sc3disp_line; break;
			case sindyTableType::t_sc3disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc4disp_line : sindyTableType::t_sc2disp_line; break;
			case sindyTableType::t_sc2disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc3disp_line : sindyTableType::t_sc1disp_line; break;
			case sindyTableType::t_sc1disp_line: __disp = ( __reverce ) ? sindyTableType::t_sc2disp_line : sindyTableType::unknown; break;
			default: __disp = sindyTableType::unknown; break;
		}
		return *this;
	}
	const DispIterator& operator--() {
		__reverce = ! __reverce;
		operator++();
		__reverce = ! __reverce;
	}
	const sindyTableType::ECode& operator*() const { return __disp; }
	bool operator !=( const DispIterator& __sc ) const { return ( ! ( **this == *__sc)  ); }
	bool operator ==( const DispIterator& __sc ) const { return ( **this == *__sc ); }
private:
	sindyTableType::ECode __disp;
	bool __reverce;
};

/**
 * @class CRelAnnotation
 * @brief <b>注記管理クラス</b>\n
 * このクラスは注記真位置をベースとした継承クラスで、
 * 関連する表示位置フィーチャを管理します。
 */
class SINDYLIB_API CRelAnnotation : public CAnnotation
{
	ROWCLASS_CONSTRUCTOR( CRelAnnotation, unknown )

	//@{ @name メンバ初期化
	/**
	 * @brief <b>表示位置をセットする</b>\n
	 * 表示位置をセットします。対応する真位置が既にセットされていなくてもセットできますが、
	 * 呼び出し側でセットされる方が対応する真位置であることを保証する必要があります。
	 * @param emTableType			[in]			テーブルタイプ
	 * @param ipAnnotationFeature	[in]			表示位置フィーチャ
	 * @param bDetatch				[in,optional]	trueにするとキャッシュを作成し、フィーチャをデタッチする（デフォルト：false）
	 * @param ipFields				[in,optional]	フィールド対応表オブジェクト(shared_ptr)
	 * @param spNameString			[in,optional]	テーブル名等管理オブジェクト(shared_ptr)
	 * @retval FALSE 初期化に失敗
	 */
	errorcode::sindyErrCode SetDispLine( sindyTableType::ECode emTableType, IFeature* ipDispLineFeature, bool bDetatch = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	//@}
	//@{ @name チェックルーチン
	/**
	 * @brief <b>注記文字列と真位置の位置関係をチェックする</b>\n
	 * 真位置に対して文字列の位置が、上下左右・縦書き横書きのうち許されている
	 * パターンであるかどうかをチェックします。
	 * [rule_check_1_1_4_3E] 注記文字列と記号の位置関係のチェック\n
	 * [rule_check_1_1_4_4E] 注記文字列と記号の位置関係のチェック（山岳）\n
	 * [rule_check_1_1_4_5E] 注記文字列と記号の位置関係のチェック（峠）\n
	 * [rule_check_1_1_4_9E] 注記文字列と記号の位置関係のチェック\n
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckPosition() const;

	/**
	 * @brief <b>真位置記号と文字列の間隔をチェックする</b>\n
	 */
	errorcode::sindyErrCode CheckAnnotationGap() const;

	/**
	 * @brief <b>表示タイプをチェックする</b>\n
	 * 中縮、ミドル、トップはS1からS4すべてをチェックします。
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckDispType() const;
	//@}

	/**
	 * @biref <b>キャッシュを作成する</b>\n
	 * @param bDetatch			[in,optional]	trueにするとキャッシュ作成後に切り離す（デフォルト：false）
	 * @param ipFields			[in,optional]	フィールド対応表オブジェクト(shared_ptr)
	 * @param spNameString		[in,optional]	テーブル名等管理オブジェクト(shared_ptr)
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>表示位置フィーチャの数を取得する</b>\n
	 * テーブルタイプには注記表示位置フィーチャのタイプを渡してください。
	 * @param emTableType	[in]	テーブルタイプ
	 * @retval -1 エラー
	 * @retval 0以上 表示位置フィーチャの数
	 */
	long GetDispLineCount( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>表示位置フィーチャを取得する</b>\n
	 * @param lIndex		[in]	インデックス番号
	 * @param emTableType	[in]	表示位置フィーチャテーブルタイプ
	 * return 表示位置フィーチャ
	 */
	const CDispLine* GetDispLine( long lIndex, sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>表示位置フィーチャを取得する</b>\n
	 * @param lIndex		[in]	インデックス番号
	 * @param emTableType	[in]	表示位置フィーチャテーブルタイプ
	 * @return 表示位置フィーチャ
	 */
	CDispLine* GetDispLine( long lIndex, sindyTableType::ECode emTableType );

	/**
	 * @brief <b>注記文字列と真位置の位置関係を取得する</b>\n
	 */
//	sindyAnnotationPosition CalcPosition() const;

	/**
	 * @brief <b>スケールにあった注記文字列を取得する</b>\n
	 * @param emDisp	[in]	表示位置レイヤタイプ
	 * @param nLen		[in]	lpszNameの収納可能文字列数
	 * @param lpszName	[out]	領域を確保されているバッファ
	 * @return エラー又は空文字列の場合はNULL
	 */
	LPCTSTR GetNameString( long lIndex, sindyTableType::ECode emTableType, int nLen, LPTSTR lpszName ) const;
	/**
	 * @brief <b>スケールにあった注記文字数を取得する</b>\n
	 * @param emDisp	[in]	表示位置レイヤタイプ
	 * @retval -1 エラー
	 * @retval -1以外 注記文字数
	 */
	long GetStringNum( long lIndex, sindyTableType::ECode emTableType ) const;

	/**
	* @brief <b>コピー注記が存在するかどうか</b>\n
	* コピー注記が存在するかどうか確認します。
	* @warning 山岳の場合標高値はコピーとして扱います。
	* @param emDispTableType [in] 確認対象テーブルタイプ
	* @retval true コピー注記が存在する
	* @retval false コピー注記が存在しない
	*/
	bool HasCopyDispLine( sindyTableType::ECode emDispTableType ) const;

	//@{ @name 編集系
	/**
	 * @brief <b>指定オブジェクトを削除する</b>\n
	 * 指定された真位置・表示位置を削除します。
	 * 指定されたオブジェクトが真位置の場合、関連する表示位置は
	 * すべて削除対象となります。
	 * 指定されたオブジェクトが表示位置の場合、状況に応じて真位置
	 * の属性が変化する場合があります（e.x. 記号＋文字列 -> 記号のみ）。
	 * 削除する際に関連属性と整合が取れない場合は全削除となります。
	 * @param ipRow [in] 削除対象（表示位置 or 真位置）
	 * @note 削除フラグを立てるだけなので、実際に削除するにはStoreを
	 * 呼ぶ必要があります。
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( _IRow* ipRow, CErrorObjects& cErrs );
	//@}

	//@{ @name 注記スケールイテレータ
	typedef DispIterator scale_iterator;
	scale_iterator scale_begin() const { scale_iterator sc = DispIterator(GetTableType()); return sc; }
	scale_iterator scale_end() const { scale_iterator sc = DispIterator(sindyTableType::unknown); return sc; }
	//@}
	/**
	 * @brief <b>注記真位置に合った表示位置レイヤ数を返す</b>\n
	 * 都市地図の場合は1、それ以外は4になります。
	 * @retval 1 都市地図の場合
	 * @retval 4 それ以外の場合
	 * @retval -1 テーブルタイプが未定
	 */
	int GetDispLayerCount() const;

	/**
	 * @brief <b>表示位置レイヤのテーブルタイプを返す</b>\n
	 * 表示位置レイヤがいくつあるかはGetDispLayerCountで調べることができます。
	 * @retval S4or都市地図表示位置レイヤ インデックス番号0
	 * @retval S3 インデックス番号1
	 * @retval S2 インデックス番号2
	 * @retval S1 インデックス番号3
	 * @retval -1 インデックス番号が範囲外、又はテーブルタイプが未定
	 */
	sindyTableType::ECode GetDispLayer( int index ) const;
};

} // sindy

#endif // _RELANNOTATION_H_
