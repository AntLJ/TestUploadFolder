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

#include <SiNDYLib/Feature.h>

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CBaseAdmin
 * @brief	中縮行政界クラス
 */
class CBaseAdmin : public CFeature
{
	ROWCLASS_CONSTRUCTOR( CBaseAdmin, base_admin )

	/**
	 * @brief	<b>市区町村コード取得</b>
	 * @return	市区町村コード
	 */
	CString GetCityCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_admin::kCityCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>市区町村名取得</b>
	 * @note	※ Initializer.h の Init_AreaCodeHelper関数を呼び出してから使用すること！
	 * @return	市区町村名取得
	 */
	CString GetCityName() const;

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * CRow::DeleteCacheをオーバロード
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strCityName = _T("");
	}

private:

	mutable	CString m_strCityName;	//!< 市区町村名称(キャッシュ用)
};

} // namespace addr
