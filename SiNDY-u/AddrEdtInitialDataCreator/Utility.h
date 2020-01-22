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

#pragma once

#include "stdafx.h"
#include <regex>
#include <TDC/useful_headers/str_util.h>

namespace Utility
{

/**
 * @brief 実行モード
 */
enum ExecuteType
{
	None,         // 未指定
	PlaceNameRep, // 地名代表ポイントインポート
	AddrPoly,     // 住所ポリゴンインポート
	AddrPoint,    // 住所ポイントインポート
	AddrAll       // 住所ポリゴン＋住所ポリゴンインポート
};

/**
 * @brief 実行ログ出力ステータス
 */
enum RunLogMsgStatus
{
	Normal,    // ステータスなし
	Info,      // INFO
	Error,     // ERROR
	Fatal      // FATAL
};

}

/**
 * @brief 市区町村コード検索用regex
 * @note 5桁の半角数字
 */
const std::wregex citycode_re(L"^[0-9]{5}$");

/**
 * @brief 拡張コードリスト情報
 */
struct ExtcodeListRec
{
	/**
	 * @brief   コンストラクタ
	 */
	ExtcodeListRec() {}

	/**
	 * @brief   コンストラクタ
	 * @param   code [in]  拡張コード
	 * @param   name [in]  名称
	 * @param   yomi [in]  読み
	 */
	ExtcodeListRec(const CString& code, const CString& name, const CString& yomi) :
											extcode(code), extName(name), extYomi(yomi) {}

	CString extcode;  //!< 拡張コード
	CString extName;  //!< 名称
	CString extYomi;  //!< 読み
};

/**
 * @brief 住所ポリゴン情報
 */
struct EdtAddrPolygonRec
{
	/**
	 * @brief   コンストラクタ
	 */
	EdtAddrPolygonRec() : oid(-1), addrType(0), colorCode(0) {}

	/**
	 * @brief   住所コード2設定
	 * @param   addrcode [in]  住所コード
	 */
	void setAddrcode2(const CString& addrcode) 
	{
		addrcode2 = ( 0 == addrcode.CompareNoCase(_T(" ")) ) ? _T("") : addrcode;
	}

	long oid;            //!< オブジェクトID
	CString citycode;    //!< 市区町村コード
	CString addrcode1;   //!< 住所コード1
	CString extName1;    //!< 拡張名称（公称）
	CString extYomi1;    //!< 拡張読み（公称）
	CString addrcode2;   //!< 住所コード2
	CString extName2;    //!< 拡張名称（通称）
	CString extYomi2;    //!< 拡張読み（通称）
	CString gaikuFugo;   //!< 街区符号
	int addrType;        //!< 住所整備種別コード
	int colorCode;       //!< 街区色コード
	IGeometryPtr geo;    //!< 形状
};

/**
 * @brief 住所ポイント情報
 */
struct EdtAddrPointRec
{
	/**
	 * @brief   コンストラクタ
	 */
	EdtAddrPointRec() : oid(-1), priorityF(0), ignoreF(0), pointType(0),
						confirmC(0), onAddrPolyBoundary(false)  {}

	/**
	 * @brief   住所コード（11桁）取得
	 * @return 住所コード（11桁）
	 */
	CString getAddrcode11() const { return ( citycode + addrcode ); }

	long oid;            //!< オブジェクトID
	CString citycode;    //!< 市区町村コード
	CString addrcode;    //!< 住所コード
	CString gouNo;       //!< 号番号
	CString yomi;        //!< 読み
	int priorityF;       //!< 優先フラグ
	int ignoreF;         //!< 街区符号無効フラグ
	int pointType;       //!< ポイント整備種別コード
	int confirmC;        //!< 確認ステータスコード
	CString geoBldId;    //!< Geospace建物ID
	CString ipcBldId;    //!< iPC建物ID
	IGeometryPtr geo;    //!< 形状
	bool onAddrPolyBoundary; //!< 住所ポリゴンの境界上に存在するか（true：存在する、false：存在しない）
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
 * @brief 拡張コードリスト情報
 */
struct CountInfo
{
	/**
	 * @brief   コンストラクタ
	 */
	CountInfo() {}

	/**
	 * @brief   コンストラクタ
	 * @param   layerName [in]  レイヤ名
	 */
	CountInfo(const CString& layerName) :
					layer(layerName), totalCount(0), importCount(0), exceptCount(0) {}


	/**
	 * @brief   ログ出力用文字列取得
	 * @return ログ出力用文字列
	 */
	CString getLogMsg() const { 
		return uh::str_util::format(_T("%s\t%ld\t%ld\t%ld\n"),
									layer, totalCount, importCount, exceptCount);
	}

	/**
	 * @brief   インポート件数リセット
	 */
	void resetImportCount() { importCount = 0; }


	CString layer;      //!< レイヤ名
	long totalCount;    //!< 対象全件数
	long importCount;   //!< インポート件数
	long exceptCount;   //!< インポート除外件数
};

/**
 * @brief 実行ログメッセージ出力
 * @param[in]  msg           出力メッセージ
 * @param[in]  runMsgStatus  実行ログ出力ステータス
 * @param[out] isWriteStdOut 標準出力に出力するかどうか（true：する、false：しない）
 */
void writeMessage(const CString& msg, Utility::RunLogMsgStatus runMsgStatus, bool isWriteStdOut);

/**
 * @brief エラーログメッセージ出力
 * @param[in]  layerName     レイヤ名
 * @param[in]  oid           オブジェクトID
 * @param[in]  errLevel      エラーレベル
 * @param[in]  msg           メッセージ
 * @param[in]  valueList     値リスト
 */
void writeErrMsg(const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const std::vector<CString>& valueList);

/**
 * @brief エラーログメッセージ出力
 * @param[in]  layerName     レイヤ名
 * @param[in]  oid           オブジェクトID
 * @param[in]  errLevel      エラーレベル
 * @param[in]  msg           メッセージ
 * @param[in]  valueList     値
 */
void writeErrMsg(const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const CString& value);