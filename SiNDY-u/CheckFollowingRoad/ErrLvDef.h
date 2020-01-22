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

namespace err_lv
{

/**
 * @brief	エラーレベル
 */
enum error_level
{
	kWarn, //!< WARNING
	kError, //!< ERROR
	kFatal //!< FATAL
};

/**
* @brief エラーレベルに応じて特定の文字列を返す
* @param errLv[in] エラーレベル
*/
inline uh::tstring getStrErrLv( const error_level errLv )
{
	uh::tstring strErrLv;

	switch (errLv)
	{
	case kWarn: strErrLv = _T("WARNING"); break;
	case kError: strErrLv = _T("ERROR"); break;
	case kFatal: strErrLv = _T("FATAL"); break;
	default:
		assert(false);
		break;
	}

	return strErrLv;
}

} // err_lv
