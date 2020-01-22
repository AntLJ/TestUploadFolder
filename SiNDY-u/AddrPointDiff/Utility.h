#pragma once

#include "stdafx.h"

/**
 * @brief セパレータ（半角ハイフン）
 */
const CString SEPARATOR_HALF_HYPHEN = _T("-");

/**
 * @brief セパレータ（全角ハイフン）
 */
const CString SEPARATOR_FULL_HYPHEN = _T("‐");

/**
 * @brief 同一住所存在種別
 */
enum AddrExistAttr
{
	ExistSameAddrInArea,    // 対象エリア内に同一住所が存在する
	ExistSameAddrOutArea,   // 対象エリア外に同一住所が存在する
	ExistOtherAddrInArea,   // 対象エリア内に異なる住所が存在する
	NoExist                 // 存在しない
};

/**
 * @brief 全角化
 * @param[in]  str 変換対象文字列
 * @return 全角化した文字列
 */
inline CString ToFull(LPCTSTR str)
{
	INT nTextLen = lstrlen( str ) + 1;
	CString strTextBuf;
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, str, nTextLen, strTextBuf.GetBuffer(nTextLen), nTextLen );

	CString strResult(strTextBuf);
	strTextBuf.ReleaseBuffer();
	return strResult;
}

/**
 * @brief 半角スペースのみの場合、半角スペースを削除する（Shape向け）
 * @param[in]  value 対象文字列
 * @return 除去後の文字列
 */
inline CString replaceStr(const CString& value)
{
	return ( _T(" ") == value ) ? _T("") : value;
}

/**
 * @brief 数値のみで構成されているか
 * @param[in]  target   確認対象文字列
 * @retval  true  数値のみ
 * @retval  false 数値のみではない
 */
inline bool isNumber(const CString& target)
{
	return ( target.SpanIncluding(_T("0123456789０１２３４５６７８９")) == target );
}

namespace mid_addr_schema
{
	namespace addr_point
	{
		extern const _TCHAR kTableName[];     //!< テーブル名

		extern const _TCHAR kGeoBFid[];       //!< GEOSPACE建物ID
		extern const _TCHAR kIpcObjId[];      //!< 中間ファイルオブジェクトID
		extern const _TCHAR kPlaceCd[];       //!< 住所11桁コード
		extern const _TCHAR kStreetCd[];      //!< 通り名フラグ
		extern const _TCHAR kAddress1[];      //!< 住所文字列1
		extern const _TCHAR kAddress2[];      //!< 住所文字列2
		extern const _TCHAR kAddress3[];      //!< 住所文字列3
		extern const _TCHAR kSourceCd[];      //!< 出典フラグ
		extern const _TCHAR kPtnF[];          //!< 処理パターンフラグ
		extern const _TCHAR kOrderCd[];       //!< 採用順フラグ
		extern const _TCHAR kJudgCd[];        //!< 採用判定理由フラグ
		extern const _TCHAR kGeoSf[];         //!< GEOSPACE住所の信頼度フラグ
		extern const _TCHAR kIpcSf[];         //!< インクリメントP住所の信頼度フラグ
		extern const _TCHAR kShinraiF[];      //!< 信頼度フラグ（手作業入力時）
		extern const _TCHAR kHyoki[];         //!< 図面表記上の地番

	} // addr_point

} // mid_addr_schema


