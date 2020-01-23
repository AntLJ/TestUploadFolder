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

#ifndef __VARIANT_STRING_H__
#define __VARIANT_STRING_H__

/// _variant_t‚©‚çCString‚Ö‚ÌŒ^•ÏŠ·‚ðŽx‰‡‚·‚é‚½‚ß‚¾‚¯‚ÌƒNƒ‰ƒX
class CVariantString : public CString
{
public:
	CVariantString() {}

	CVariantString(const _variant_t& rVariant)
	{
		switch(rVariant.vt) {
		case VT_BSTR:
			CString::operator=(rVariant.bstrVal);
			break;
		case VT_NULL:
			CString::operator=(_T("<null>"));
			break;
		case VT_ERROR:
			CString::operator=(_T("<error>"));
			break;
		default:
			CString::operator=(static_cast<LPCTSTR>(static_cast<_bstr_t>(rVariant)));
			break;
		}
	}

	CVariantString(const CVariantString& rString) :
	CString(rString)
	{
	}
};

#endif // __VARIANT_STRING_H__
