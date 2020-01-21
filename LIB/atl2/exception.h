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
 * @brief HRESULT �� E_* �ł������ꍇ�A��O���N�����֐��B
 *
 * @exception _com_error hr �������iE_*�j�ł������B
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
 * @brief COM�W����O�𕶎��񉻂���B
 *
 * @param rError [in] COM�W����O�B
 * @param bGetErrorInfo [in] rErrorInfo �� @b IErrorInfo ���֘A�t�����Ă��Ȃ��Ƃ��A GetErrorInfo() ���g���ēƎ��ɋ��߂邩�B
 * @return ������B
 *
 * @note
 * high_method�������� @b _com_error �ɂ� @b IErrorInfo ���֘A�t���Ă��Ȃ��B<br>
 * ��������ŏ�����A�֘A�t������d�l�ɂȂǂ��Ȃ���Ηǂ��̂ɁB
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
