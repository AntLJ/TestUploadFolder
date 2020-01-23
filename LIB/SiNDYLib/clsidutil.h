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

#ifndef _SMWUTIL_H_
#define _SMWUTIL_H_

CLSID CLSIDFromUID( IUID* ipUID );
CLSID CLSIDFromVARIANT( const VARIANT& vaUID );
IUIDPtr UIDFromVARIANT( const VARIANT& vaUID );
IUIDPtr UIDFromCLSID( const CLSID& clsid );
IUIDPtr CopyUID( IUID* ipUID );

inline CLSID CLSIDFromUID( IUID* ipUID )
{
	CLSID clsid = {0};

	if( ipUID )
	{
		CComVariant vaGUID;
		long lSubType;
		ipUID->get_Value( &vaGUID );
		ipUID->get_SubType( &lSubType );

		if( VT_BSTR == vaGUID.vt )
		{
			CString strGUID( vaGUID.bstrVal );
			if( _T('{') == strGUID[0] )
				CLSIDFromString( vaGUID.bstrVal, &clsid );
			else {
				CString strGUID2;
				strGUID2.Format(_T("%s.%d"), strGUID, lSubType );
				CLSIDFromProgID( CComBSTR( strGUID2 ), &clsid );
			}
		}
		else _ASSERTE(false);
	}
	else _ASSERTE(false);

	return clsid;
}

inline CLSID CLSIDFromVARIANT( const VARIANT& vaUID )
{
	CLSID clsid = {0};

	switch( vaUID.vt )
	{
		case VT_BSTR: clsid = CLSIDFromUID( UIDFromVARIANT( vaUID ) ); break;
		case VT_UNKNOWN: clsid = CLSIDFromUID( (IUID*)(IUIDPtr)vaUID.punkVal ); break;
		default: _ASSERTE(false); break;
	}

	return clsid;
}

inline IUIDPtr UIDFromString( BSTR bstrUID )
{
	IUIDPtr ipUID;
	if( bstrUID )
	{
		if( SUCCEEDED( ipUID.CreateInstance( CLSID_UID ) ) )
		{
			if( FAILED( ipUID->put_Value( CComVariant(bstrUID) ) ) ) _ASSERTE(false);
#ifdef _DEBUG
			CComVariant vaValue;
			ipUID->get_Value( &vaValue );
			vaValue.ChangeType( VT_BSTR );
			CLSID clsid = {0};
			CLSIDFromString( vaValue.bstrVal, &clsid );
			_ASSERTE( clsid != CLSID() );
#endif // ifdef _DEBUG
			ipUID->put_SubType(-1);
		}
		else _ASSERTE(false);
	}
	return ipUID;
}

/**
 * @brief <b>VARIANT->IUID変換</b>\n
 * VARIANTの中身がIUIDの場合でも必ずコピーが返ります
 * @param vaUID	[in]	VARIANT
 * @return IUIDPtr
 */
inline IUIDPtr UIDFromVARIANT( const VARIANT& vaUID )
{
	IUIDPtr ipUID;
	switch( vaUID.vt )
	{
		case VT_UNKNOWN: ipUID = CopyUID( (IUIDPtr)vaUID.punkVal ); break;
		case VT_DISPATCH: ipUID = CopyUID( (IUIDPtr)vaUID.pdispVal ); break;
		case VT_BSTR: ipUID = UIDFromString( vaUID.bstrVal ); break;
		default: _ASSERTE(false); break;
	}
	return ipUID;
}

inline IUIDPtr UIDFromCLSID( const CLSID& clsid )
{
	IUIDPtr ipUID;
	LPOLESTR lplpsz = NULL;
	if( SUCCEEDED( StringFromCLSID( clsid, &lplpsz ) ) )
	{
		if( SUCCEEDED( ipUID.CreateInstance( CLSID_UID ) ) )
		{
			if( SUCCEEDED( ipUID->put_Value( CComVariant(lplpsz) ) ) )
			{
			}
			else
				ipUID = NULL;
		}
	}
	::CoTaskMemFree( lplpsz );
	lplpsz = NULL;

	return ipUID;
}

inline IUIDPtr CopyUID( IUID* ipUID )
{
	IUIDPtr ipNewUID;

	if( ipUID )
	{
		if( SUCCEEDED( ipNewUID.CreateInstance( CLSID_UID ) ) )
		{
			CComVariant vaGUID;
			long lSubType = 0;
			ipUID->get_Value(&vaGUID);
			ipUID->get_SubType( &lSubType );

			ipNewUID->put_Value( vaGUID );
			ipNewUID->put_SubType( lSubType );
		}
		else _ASSERTE(false);
	}
	else _ASSERTE(false);

	return ipNewUID;
}

#endif // ifndef _SMWUTIL_H_
