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

#ifndef __REGMANAGER_H_
#define __REGMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegManagerImpl
{
public:
	CRegManagerImpl() : m_hKey(HKEY_CURRENT_USER) {};
	CRegManagerImpl(LPCTSTR lpcszKey, HKEY hKey=HKEY_CURRENT_USER) { SetOpenKey(lpcszKey,hKey); };
	virtual ~CRegManagerImpl(){};
	operator CRegManagerImpl& () { return *( (CRegManagerImpl*)(this) ); }

	CComVariant& operator[](long idx) { return m_mapRegValues[idx].second; }
	CComVariant& operator[](_U_STRINGorID KeyName) { return m_mapRegValues[regkey_find(KeyName)].second; }

	typedef map<long, pair<pair<CString, long>, CComVariant>>::iterator regkey_iterator;
	typedef map<long, pair<pair<CString, long>, CComVariant>>::const_iterator regkey_const_iterator;
	regkey_iterator regkey_begin() { return m_mapRegValues.begin(); }
	regkey_iterator regkey_end() { return m_mapRegValues.end(); }
	regkey_const_iterator regkey_begin() const { return m_mapRegValues.begin(); }
	regkey_const_iterator regkey_end() const { return m_mapRegValues.end(); }
	void regkey_clear() { m_mapRegValues.clear(); }
	bool regkey_empty() const { return m_mapRegValues.empty(); }
	long regkey_size() const { return m_mapRegValues.size(); }

	void regkey_insert(_U_STRINGorID KeyName, long nType, const CComVariant& vaDefault=NULL)
	{
		if ( regkey_find( KeyName ) == -1 )
			m_mapRegValues[m_mapRegValues.size()] = pair<pair<CString, long>, CComVariant>( pair<CString, long>(KeyName.m_lpstr, nType), vaDefault );
	}
	long regkey_find(_U_STRINGorID KeyName) const
	{
		long Idx = -1;
		for ( regkey_const_iterator it = regkey_begin(); it != regkey_end(); ++it ) {
			if ( it->second.first.first == KeyName.m_lpstr ) {
				Idx = it->first;
				break;
			}
		}

		return Idx;
	}

	void SetOpenKey(LPCTSTR lpcszKey, HKEY hKey=HKEY_CURRENT_USER) { m_strRegOpenKey = lpcszKey; m_hKey = hKey; }

	CComVariant& GetValue(long idx) { return m_mapRegValues[idx].second; }
	CComVariant& GetValue(_U_STRINGorID KeyName) { return GetValue( regkey_find( KeyName ) ); }

	void SetValue(long idx, const CComVariant& vaValue) { m_mapRegValues[idx].second = vaValue; }
	void SetValue(_U_STRINGorID KeyName, const CComVariant& vaValue) { return SetValue( regkey_find( KeyName ), vaValue ); }

	void Load(bool bIsCreateKey=true)
	{
		if ( !m_hKey || m_strRegOpenKey.IsEmpty() )
			return;

		HKEY hKey = NULL;
		DWORD dwDisposition = 0;

		if ( bIsCreateKey )
			RegCreateKeyEx( m_hKey, m_strRegOpenKey, 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );
		else
			RegOpenKeyEx( m_hKey, m_strRegOpenKey, 0, KEY_ALL_ACCESS, &hKey );

		if ( hKey ) {
			for ( regkey_iterator it = regkey_begin(); it != regkey_end(); ++it ) {
				DWORD dwSize = 0, dwType = 0;
				CComVariant vaValue;

				if ( RegQueryValueEx( hKey, it->second.first.first, 0, &dwType, NULL, &dwSize ) == ERROR_SUCCESS ) {
					switch ( it->second.first.second ) {
						case REG_SZ:	// 文字列
							{
								TCHAR* pszData = new TCHAR[dwSize];

								RegQueryValueEx( hKey, it->second.first.first, 0, &dwType, (LPBYTE)pszData, &dwSize );
								it->second.second = CComBSTR(pszData);

								delete [] pszData;
							}
							break;
						case REG_DWORD:	// DWORD
							{
								DWORD dwValue = 0;

								RegQueryValueEx( hKey, it->second.first.first, 0, &dwType, (LPBYTE)&dwValue, &dwSize );

								it->second.second = dwValue;
							}
							break;
						case REG_BINARY:	// Binary
							{
								BYTE* pByte = new BYTE[dwSize];

								RegQueryValueEx( hKey, it->second.first.first, 0, &dwType, (LPBYTE)pByte, &dwSize );

								IStreamPtr ipStream;	//!< IStream

								// IStream作成
								::CreateStreamOnHGlobal( 0, TRUE, &ipStream );

								if ( ipStream ) {
									// サイズを0にする。
									ULARGE_INTEGER ulnSize = {0, 0};
									ipStream->SetSize( ulnSize );

									// 先頭に移動
									LARGE_INTEGER zeroPos = {0, 0};
									ULARGE_INTEGER StreamSize = {0, 0};
									ipStream->Seek( zeroPos, STREAM_SEEK_SET, &StreamSize );

									// IStreamに書込
									ipStream->Write( pByte, dwSize, NULL );

									// 先頭に移動
									ipStream->Seek( zeroPos, STREAM_SEEK_SET, &StreamSize );
								}

								it->second.second = (IUnknown*)ipStream;

								delete [] pByte;
							}
							break;
						default:
							ATLASSERT(FALSE);
							break;
					}
				}
			}

			RegCloseKey( hKey );
		}
	}

	void Save(bool bIsCreateKey=true) const
	{
		if ( !m_hKey || m_strRegOpenKey.IsEmpty() )
			return;

		HKEY hKey = NULL;
		DWORD dwDisposition = 0;

		if ( bIsCreateKey )
			RegCreateKeyEx( m_hKey, m_strRegOpenKey, 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );
		else
			RegOpenKeyEx( m_hKey, m_strRegOpenKey, 0, KEY_ALL_ACCESS, &hKey );

		if ( hKey ) {
			for ( regkey_const_iterator it = regkey_begin(); it != regkey_end(); ++it )
			{
				switch ( it->second.first.second ) {
					case REG_SZ:	// 文字列
						{
							if ( it->second.second.vt == VT_BSTR ) {
								CString strTmp = it->second.second.bstrVal;
								DWORD dwBufferSize = strTmp.GetLength();

								RegSetValueEx( hKey, it->second.first.first, 0, REG_SZ, (LPBYTE)(LPTSTR)(LPCTSTR)strTmp, dwBufferSize );
							}
						}
						break;
					case REG_DWORD:	// DWORD
						{
							DWORD dwValue = it->second.second.lVal;

							RegSetValueEx( hKey, it->second.first.first, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue) );
						}
						break;
					case REG_BINARY:	// Binary
						{
							if ( it->second.second.vt == VT_UNKNOWN ) {
								IStreamPtr ipStream = it->second.second.punkVal;

								if ( ipStream ) {
									ULONG ByteSize = 0;

									// 先頭に移動
									LARGE_INTEGER zeroPos = {0, 0};
									ULARGE_INTEGER StreamSize = {0, 0};
									ipStream->Seek( zeroPos, STREAM_SEEK_SET, &StreamSize );

									// サイズ取得
									STATSTG stat = {0};
									ipStream->Stat(&stat, STATFLAG_NONAME);
									ByteSize = stat.cbSize.LowPart;

									CoTaskMemFree(stat.pwcsName);

									if ( ByteSize > 0 ) {
										BYTE* pByte = new BYTE[ByteSize];

										// 先頭に移動
										ipStream->Seek( zeroPos, STREAM_SEEK_SET, &StreamSize );

										// 読込
										ipStream->Read( pByte, (ULONG)ByteSize, NULL );

										// 先頭に移動
										ipStream->Seek( zeroPos, STREAM_SEEK_SET, &StreamSize );

										DWORD dwBufferSize = ByteSize;
										RegSetValueEx( hKey, it->second.first.first, 0, REG_BINARY, (LPBYTE)pByte, dwBufferSize );

										delete [] pByte;
									}
								}
							}
						}
						break;
					default:
						ATLASSERT(FALSE);
						break;
				}
			}

			RegCloseKey( hKey );
		}
	}

private:
	CString m_strRegOpenKey;
	HKEY m_hKey;
map<long, pair<pair<CString, long>, CComVariant>> m_mapRegValues;	//!< map<インデックス, pair<pair<キー名,型>,値>>
};

struct SINDY_REGTYPE_ENTRY {
	LPCTSTR lpcszValue;
	long nType;
};

#define BEGIN_REGTYPE_MAP(x) \
	static const struct SINDY_REGTYPE_ENTRY* GetRegTypeMap() { \
	static const struct SINDY_REGTYPE_ENTRY _Info[] = {
#define REGTYPE_ENTRY( lpcszValue, nType ) { \
	lpcszValue, nType },
#define END_REGTYPE_MAP() \
	{ NULL, REG_DWORD } };\
	 return &_Info[0]; }

template<class T>
class CRegManager : public vector<CRegManagerImpl>
{
	typedef vector<CRegManagerImpl> baseclass;
public:
	CRegManager(){};
	operator CRegManager<T>& () { return *( (CRegManager<T>*)(this) ); }
	virtual ~CRegManager(){};

	void reg_init(long nCount, LPCTSTR lpcszKey, HKEY hKey=HKEY_CURRENT_USER)
	{
		CRegManagerImpl pRegManager( lpcszKey, hKey );

		const SINDY_REGTYPE_ENTRY* pInfo = T::GetRegTypeMap();

		for ( long i = 0; i < nCount; ++i )
			pRegManager.regkey_insert( pInfo[i].lpcszValue, pInfo[i].nType );

		push_back( pRegManager );
	};

	long reg_insert(LPCTSTR lpcszKey, HKEY hKey=HKEY_CURRENT_USER)
	{
		CRegManagerImpl pRegManager( lpcszKey, hKey );

		push_back( pRegManager );

		return size() - 1;
	}

	void Load(bool bIsCreateKey=true)
	{
		for ( baseclass::iterator it = begin(); it != end(); ++it )
			(*it).Load( bIsCreateKey );
	}

	void Save(bool bIsCreateKey=true) const
	{
		for ( baseclass::const_iterator it = begin(); it != end(); ++it )
			(*it).Save( bIsCreateKey );
	}
};

#endif // ifndef __REGMANAGER_H_
