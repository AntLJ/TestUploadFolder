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

#include <atlstr.h>


namespace uh{

namespace variant_util{
	
	/**
	* @brief 空文字判定
	* @param val [in] VARIANT
	* @retrn VT_NULL、VT_EMPTY、またはVT_BSTRで空文字の場合はtrue 
	*/
	inline bool isNullOrEmpty( const CComVariant& val)
	{
		if( (val.vt == VT_NULL) || (val.vt == VT_EMPTY) )
			return true;

		if( val.vt == VT_BSTR)
			return (wcslen( val.bstrVal ) == 0);

		return false;
	}
	
	/**
	* @brief vtを文字列化
	*        デバッグ、ログ出力用
	* @param val [in] VARIANT
	* @retrn VARIANTTYPE
	*/
	inline CString toTypeString( const CComVariant& val)
	{
		if( val.vt & VT_BYREF )
		{
			CComVariant va(val);
			va.vt = val.vt & ~VT_BYREF;
			return _T("VT_BYREF | ") + toTypeString(va);
		}
		
		if( val.vt & VT_ARRAY )
		{
			CComVariant va(val);
			va.vt = val.vt & ~VT_ARRAY;
			return _T("VT_ARRAY | ") + toTypeString(va);
		}
		
		if( val.vt & VT_RESERVED )
		{
			CComVariant va(val);
			va.vt = val.vt & ~VT_RESERVED;
			return _T("VT_RESERVED | ") + toTypeString(va);
		}

		switch( val.vt )
		{
		case VT_EMPTY    :         return  _T("VT_EMPTY");                 break;
		case VT_NULL     :         return  _T("VT_NULL");                 break;
		case VT_I2       :         return  _T("VT_I2");                 break;
		case VT_I4       :         return  _T("VT_I4");                 break;
		case VT_R4       :         return  _T("VT_R4");                 break;
		case VT_R8       :         return  _T("VT_R8");                 break;
		case VT_CY       :         return  _T("VT_CY");                 break;
		case VT_DATE     :         return  _T("VT_DATE");                 break;
		case VT_BSTR     :         return  _T("VT_BSTR");                 break;
		case VT_DISPATCH :         return  _T("VT_DISPATCH");                break;
		case VT_ERROR    :         return  _T("VT_ERROR");                 break;
		case VT_BOOL     :         return  _T("VT_BOOL");           break;
		case VT_VARIANT  :         return  _T("VT_VARIANT");                 break;
		case VT_UNKNOWN  :         return  _T("VT_UNKNOWN");                 break;
		case VT_DECIMAL  :         return  _T("VT_DECIMAL");                 break;
		case VT_I1       :         return  _T("VT_I1");              break;
		case VT_UI1      :         return  _T("VT_UI1");              break;
		case VT_UI2      :         return  _T("VT_UI2");              break;
		case VT_UI4      :         return  _T("VT_UI4");              break;
		case VT_I8       :         return  _T("VT_I8");              break;
		case VT_UI8      :         return  _T("VT_UI8");              break;
		case VT_INT      :         return  _T("VT_INT");              break;
		case VT_UINT     :         return  _T("VT_UINT");              break;
		case VT_VOID     :         return  _T("VT_VOID");              break;
		case VT_HRESULT  :         return  _T("VT_HRESULT");                  break;
		case VT_PTR      :         return  _T("VT_PTR");              break;
		case VT_SAFEARRAY:         return  _T("VT_SAFEARRAY");           break;
		case VT_CARRAY   :         return  _T("VT_CARRAY");           break;
		case VT_USERDEFINED :      return  _T("VT_USERDEFINED");           break;
		case VT_LPSTR    :         return  _T("VT_LPSTR");           break;
		case VT_LPWSTR   :         return  _T("VT_LPWSTR");           break;
		case VT_RECORD   :         return  _T("VT_RECORD");           break;
		case VT_INT_PTR  :         return  _T("VT_INT_PTR");           break;
		case VT_UINT_PTR :         return  _T("VT_UINT_PTR");           break;
		case VT_FILETIME :         return  _T("VT_FILETIME");           break;
		case VT_BLOB     :         return  _T("VT_BLOB");       break;
		case VT_STREAM   :         return  _T("VT_STREAM");           break;
		case VT_STORAGE  :         return  _T("VT_STORAGE");           break;
		case VT_STREAMED_OBJECT :  return  _T("VT_STREAMED_OBJECT"); 	        break;
		case VT_STORED_OBJECT   :  return  _T("VT_STORED_OBJECT");             break;
		case VT_BLOB_OBJECT     :  return  _T("VT_BLOB_OBJECT");             break;
		case VT_CF       :         return  _T("VT_CF");    break;
		case VT_CLSID    :         return  _T("VT_CLSID");        break;
		case VT_VERSIONED_STREAM : return  _T("VT_VERSIONED_STREAM");        break;
		case VT_BSTR_BLOB :        return  _T("VT_BSTR_BLOB"); 	   break;
		case VT_VECTOR   : 		   return  _T("VT_VECTOR"); 	   break;
		case VT_ARRAY    : 		   return  _T("VT_ARRAY"); 	   break;
		case VT_BYREF    : 		   return  _T("VT_BYREF"); 	   break;
		case VT_RESERVED : 		   return  _T("VT_RESERVED"); 	   break;
		case VT_ILLEGAL  : 		   return  _T("VT_ILLEGAL"); 	   break;
		default:
			break;
		}

		return _T("Unkown Type");
	}

} // variant_util
} // uh
