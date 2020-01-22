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
 * @file	define.h
 * @brief	各種共通定義値
 * ここに共通で使用できるものは定義していく
 */

#pragma once
#include "Message.h"
#include "Logger.h"


// 接続プロパティキー定義
const CString	DB_ADDRESS = _T("db_address");
const CString	DB_MESH	= _T("db_mesh");
const CString	DB_MAP = _T("db_map");

// 設定ファイルアイテム定義
const CString   LOG_NAME = _T("log_name");
const CString   NG_CHAR = _T("ng_char");
const CString   DEFAULT_LOGNAME = _T("\\%s_%d%02d%02d%02d%02d%02d.log");

// デフォルトの値定義
const CString DEFAULT_CITYCODE = _T("00000");
const CString DEFAULT_ADDRCODE = _T("000000");
const CString DEFAULT_BLOCKCODE = _T("0");
const CString VILLA = _T("999999");

const long INVALID_VALUE = -1L;
const long ABOLITION_CODE = 2L;

typedef std::list<CString>			LAYERLIST;	//!<  レイヤ名リスト
typedef std::map<CString, long>     FIELDSMAP;  //!<  フィールドマップ

/**
* @brief	住所マスタ
*/
struct AddressMaster
{
	long status;	            //!< ステータス
	CString addressName;        //!< 住所名
	CString addressCode;        //!< 11桁住所コード

	/**
	 * @brief  コンストラクタ
	 */
	AddressMaster():status(INVALID_VALUE){};

	/**
	 * @brief  コンストラクタ
	 * @param	[in] inputStatus       ステータス
	 * @param	[in] inputAddressName  住所名
	 * @param	[in] inputAddressCode  11桁住所コード
	 */
	AddressMaster(long inputStatus, const CString& inputAddressName, const CString& inputAddressCode):status(inputStatus), addressName(inputAddressName), addressCode(inputAddressCode){}
};

/**
* @brief	エラー情報
*/
struct ErrorIdInfo
{
	long oId;        //!< オブジェクトID
	long errorCode;  //!< エラーコード
	/**
	 * @brief  コンストラクタ
	 * @param	[in] objectId        //!< オブジェクトID
	 * @param	[in] inputErrorCode  //!< エラーコード
	 */
	ErrorIdInfo(long objectId, long inputErrorCode):oId(objectId), errorCode(inputErrorCode){};

	/**
	* @brief	演算子のオーバロード
	* @param	[in] errorInfo	     エラー情報
	* @retval	true:小さい
	* @retval	false:よりまたは等しい
	*/
	bool operator < (const ErrorIdInfo& errorInfo)const
	{
		if(oId < errorInfo.oId)	return true;
		if(oId > errorInfo.oId)	return false;

		if(errorCode < errorInfo.errorCode)	return true;
		return false;
	}
};

/**
* @brief	情報出力
* @param	[in] lpcsz           情報
* @param	[in] ...             その他付加情報
*/
inline void printProgress( LPCTSTR lpcsz, ... )
{
	CString strMsg;
	if( lpcsz )
	{
		va_list vaList;
		va_start( vaList, lpcsz );
		strMsg.FormatV( lpcsz, vaList );
	}
	std::cout << CT2A(strMsg);
	RUNLOGGER << strMsg;
}


