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

#ifndef ATL2_EXCEPTION_H_
#define ATL2_EXCEPTION_H_

#include <comdef.h>
#include <string>

namespace atl2 {

/**
 * @brief HRESULT が E_* であった場合、例外を起こす関数。
 *
 * @exception _com_error hr が負数（E_*）であった。
 */
inline HRESULT valid(HRESULT hr)
{
	if(FAILED(hr)) {
		IErrorInfo* ipErrorInfo = 0;
#pragma warning(push)
#if _MSC_VER >= 1400
#pragma warning(disable : 6031)
#endif // _MSC_VER >= 1400
		::GetErrorInfo(0, &ipErrorInfo);
#pragma warning(pop)
		::_com_raise_error(hr, ipErrorInfo);
	}

	return hr;
}

/**
 * @brief COM標準例外を文字列化する。
 *
 * @param rError [in] COM標準例外。
 * @param bGetErrorInfo [in] rErrorInfo に @b IErrorInfo が関連付けられていないとき、 GetErrorInfo() を使って独自に求めるか。
 * @return 文字列。
 *
 * @note
 * high_methodが投げる @b _com_error には @b IErrorInfo が関連付いていない。<br>
 * だったら最初から、関連付けられる仕様になどしなければ良いのに。
 */
inline std::basic_string<TCHAR> stringize(const _com_error& rError, bool bGetErrorInfo = true)
{
	USES_CONVERSION;

#pragma warning(push)
#if _MSC_VER >= 1400
#pragma warning(disable : 6255)
#endif // _MSC_VER >= 1400
	_bstr_t bstrDescription(rError.Description());
	if(bstrDescription.length()) {
		return OLE2CT(bstrDescription);
	}
	else {
		if(bGetErrorInfo) {
			IErrorInfoPtr ipErrorInfo;
			if(::GetErrorInfo(0, &ipErrorInfo) == S_OK) {
				CComBSTR bstr;
				if(ipErrorInfo->GetDescription(&bstr) == S_OK && bstr.Length() > 0)
					return OLE2CT(bstr);
			}
		}

		return rError.ErrorMessage();
	}
#pragma warning(pop)

}

} // namespace atl2

#endif // ATL2_EXCEPTION_H_
