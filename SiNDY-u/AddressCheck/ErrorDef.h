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
 * @enum	emErrLevel
 * @brief	エラーレベル
 */
enum emErrLevel
{
	kOK,
	kNotice,
	kWarn,
	kError,
	kCritical
} ERROR_LEVEL;

/**
 * @enum	emCheck_ErrCode
 * @brief	エラーコード
 */
enum emCheck_ErrCode
{
	kUnknown = -1,	// 不明なエラー
#define ERROR_CODE( code, value, level, msg_jp ) code = value,
#include "ErrorCode.h"
#undef ERROR_CODE
};

/**
 * @struct	ErrInfo
 * @brief	エラー情報格納構造体
 */
struct ErrInfo
{
	/**
	 * @brief  コンストラクタ
	 */
	ErrInfo() : m_emCode(kUnknown), m_oID(-1L){ m_point.X = 0.0; m_point.Y = 0.0; }

	/**
	 * @brief  コンストラクタ
	 * @param	[in]  emCode       //!< エラーコード
	 * @param	[in]  lpcszLayer   //!< レイヤ名
	 * @param	[in]  lOID         //!< OID
	 * @param	[in]  point        //!< エラー位置
	 */
	ErrInfo( emCheck_ErrCode emCode, LPCTSTR lpcszLayer, long lOID, WKSPoint point ) : 
		m_emCode(emCode), m_layer(lpcszLayer), m_oID(lOID), m_point(point){}

	emCheck_ErrCode    m_emCode;     //!< エラーコード
	CString            m_layer;      //!< レイヤ名
	long               m_oID;        //!< OID
	WKSPoint           m_point;      //!< エラー位置
};
