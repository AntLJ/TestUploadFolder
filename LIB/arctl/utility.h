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

#ifndef ARCTL_UTILITY_H_
#define ARCTL_UTILITY_H_

namespace arctl {

/**
 * @defgroup create_uid_group UIDオブジェクト生成関数
 * @brief UIDオブジェクトを生成する。
 */
//@{

/**
 * @brief GUID文字列からUIDオブジェクトを生成する。
 */
inline IUIDPtr create_uid(const VARIANT& rGUIDString)
{
	IUIDPtr ipUID(__uuidof(UID));
	ipUID->put_Value(rGUIDString);
	return ipUID;
}

/**
 * @brief GUIDからUIDオブジェクトを生成する。
 */
inline IUIDPtr create_uid(REFGUID rguid)
{
	OLECHAR lpsz[_MAX_PATH];
	if(::StringFromGUID2(rguid, lpsz, _MAX_PATH) != 0) {
		return create_uid(_variant_t(lpsz));
	}
	else {
#ifdef _ASSERT
		_ASSERT(FALSE);
#endif // _ASSERT
		return 0;
	}
}

//@}

} // namespace arctl

#endif // ARCTL_UTILITY_H_
