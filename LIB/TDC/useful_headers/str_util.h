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
#include <vector>
#include <boost/tokenizer.hpp>

namespace uh{

namespace str_util{
	/**
	 * @brief long型を文字列に変換する
	 * @param[in]	l	long値
	 * @return 変換後の文字列
	 */
	inline CString ToString( long l ){
		CString str;
		str.Format( _T("%ld"), l );
		return str;
	}
	
	/**
	 * @brief int型を文字列に変換する
	 * @param[in]	i	int値
	 * @return 変換後の文字列
	 */
	inline CString ToString( int i ){
		CString str;
		str.Format( _T("%d"), i );
		return str;
	}

	/**
	 * @brief unsigned long型を文字列に変換する
	 * @param[in]	l	unsigned long値
	 * @return 変換後の文字列
	 */
	inline CString ToString( unsigned long l ){
		CString str;
		str.Format( _T("%ul"), l );
		return str;
	}
	
	/**
	 * @brief unsigned int型を文字列に変換する
	 * @param[in]	i	unsigned int値
	 * @return 変換後の文字列
	 */
	inline CString ToString( unsigned int i ){
		CString str;
		str.Format( _T("%u"), i );
		return str;
	}

	/**
	 * @brief short型を文字列に変換する
	 * @param[in]	s	short値
	 * @retrn 変換後の文字列
	 */
	inline CString ToString( short s ){
		CString str;
		str.Format( _T("%d"), s );
		return str;
	}

	/**
	 * @brief double型を文字列に変換する
	 * @param[in]	l	double値
	 * @param[in]	fmt	CString::Format()に指定する書式（デフォルト：%.1lf）
	 * @return 変換後の文字列
	 */
	inline CString ToString( double d, const CString& fmt = _T("%.1lf") )
	{
		CString str;
		str.Format( fmt, d );
		return str;
	}

	/**
	 * @brief float型を文字列に変換する
	 * @param[in]	f	float値
	 * @param[in]	fmt	CString::Format()に指定する書式（デフォルト：%.1f）
	 * @return 変換後の文字列
	 */
	inline CString ToString( float f, const CString& fmt = _T("%.1f") )
	{
		CString str;
		str.Format( fmt, f );
		return str;
	}
	
	/**
	 * @brief 文字列を文字列に変換する
	 *        join()から呼ばれる用
	 * @param[in]	str	文字列
	 * @return 自分自身
	 */
	inline CString ToString( const CString& str )
	{
		return str;
	}
	/**
	 * @brief CLSIDを文字列に変換する
	 * @param[in] clsid CLSID
	 * @return CLSIDを文字列化したもの
	 */
	inline CString ToString( const CLSID& clsid )
	{
		LPOLESTR lpStr;
		::StringFromCLSID(clsid, &lpStr);
		CString strCLSID(lpStr);
		::CoTaskMemFree(lpStr);
		return strCLSID;
	}

	// 2013/10/02 GS対応でiMapCheckのGetTime()を移植
	/**
	 * @brief	LPSYSTEMTIMEを文字列に変換する
	 * @param	lpSystemTime	[in]	日付
	 * @param	fmt	[in]	CString::Format()に指定する書式（デフォルト：%04d/%02d/%02d %02d:%02d:%02d）
	 * @return	CString	変換後の文字列(yyyy/MM/dd tt:mm:ss)
	 */
	inline CString ToString( LPSYSTEMTIME lpSystemTime, const CString& fmt = _T("%04d/%02d/%02d %02d:%02d:%02d"))
	{
		SYSTEMTIME stSystemTime;
		CopyMemory( &stSystemTime, lpSystemTime, sizeof(SYSTEMTIME));
		CString str;
		str.Format( fmt, stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

		return str;
	}

	// 2013/10/02 GS対応でiMapCheckのValue2String()を移植
	/**
	 * @brief  Variant型を文字列に変換する
	 * @note ドメインが設定されていないと明確な場合にのみ使用すること
	 *         "ドメインが設定されているかもしれない"場合には、こちらを使わず第2引数にマップを渡すほうを使う
	 * @param[in]	vaValue	フィールド値
	 * @param[in]	fmt	CString::Format()に指定する書式（デフォルト：%.1f）
	 * @retval	CString	変換後の文字列(フィールド値によって書式は変わる)
	 */
	inline CString ToString( const VARIANT& vaValue, const CString& fmt = _T("%.1f") )
	{
		CString str;
		switch( vaValue.vt )
		{
			case VT_I2:
			{
				str = ToString(vaValue.iVal);
				break;
			}

			case VT_I4:
			{
				str = ToString(vaValue.intVal);
				break;
			}
			
			case VT_R4:
			{
				str = ToString(vaValue.fltVal, fmt);
				break;
			}
			
			case VT_R8:
			{
				str = ToString(vaValue.dblVal, fmt);
				break;
			}
			
			case VT_BSTR:
			{
				str = vaValue.bstrVal;
				break;
			}

			case VT_DATE:
			{
				CComVariant va( vaValue );
				va.ChangeType(VT_BSTR);
				str = va.bstrVal;
				break;
			}
			
			// VT_EMPTY、VT_NULLの場合は空文字を返す
			case VT_EMPTY:
			case VT_NULL:
			{
				break;
			}

			default:
			{
				str.Format( _T("not suport.(vt: %ld)"), vaValue.vt );
				break;
			}
		}
		return str;
	}
	inline CStringA ToStringA( const VARIANT& vaValue, const CString& fmt = _T("%.1f") )
	{
		return CStringA(ToString( vaValue, fmt ));
	}
	/**
	 * @brief CLSIDを文字列に変換する
	 * @param[in] clsid CLSID
	 * @return CLSIDを文字列化したもの
	 */
	inline CStringA ToStringA( const CLSID& clsid )
	{
		return CStringA( ToString(clsid) );
	}

	/**
	 * @brief 文字列中の文字列を置換する
	 * @note 変換元文字列srcをmove()で渡すと最小コストとなる（推奨）
	 * @note new_を省略するとsrc中のoldは削除される（Remove）
	 * @param[in]	src	変換元の文字列
	 * @param[in]	old	srcに含まれる置換対象の文字列
	 * @param[in]	new_	oldの置換後の文字列（デフォルト：空文字）
	 * @return 変換後の文字列
	 */
	inline CString replace( CString src, const CString& old, const CString& new_ = _T("")  )
	{
		src.Replace( old, new_ );
		return src;
	}
	
	/**
	 * @brief boost を使った split
	 * @note  ガチャガチャと変換が入るので非効率。通常はsplit()を使用すること。
	 *        token_compress_onのみサポート。
	 * @param[in]	src	元の文字列
	 * @param[in]	sep	セパレータとなる文字列
	 * @return sep_charで区切られた文字列のvector
	 */
	inline std::vector<CString> split_( const CString& src, const CString& sep )
	{
		// boostのsplitを使うと、なぜかsindylib_baseのデバッグビルドが通らなくなる
		// という不可解極まりない怪現象が起こるため、代わりにtokenizerを使うことにする

		typedef boost::char_separator<wchar_t> char_sep;
		typedef boost::tokenizer< char_sep, std::wstring::const_iterator, std::wstring > tokenizer;
 
		std::wstring sepW = CT2W(sep); // 一時変数に入れとかないと落ちるので注意
		char_sep separator(sepW.c_str(), L"", boost::keep_empty_tokens );

		std::wstring srcW = std::wstring( CT2W(src) ); // こっちも
		tokenizer tokens(srcW, separator);
 
		std::vector<CString> ret;
		for( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
		{
			ret.push_back( CString(it->c_str()) );
		}

		return ret;
	}


	/**
	 * @brief 文字列を文字列で分割する
	 * @note  第3引数のデフォルトは boost::algorithm::split と逆意なので注意
	 * @param[in]	target	元の文字列
	 * @param[in]	sep_char	セパレータとなる文字列
	 * @param[in]    token_compress_off 空のフィールド(セパレータ文字が連続で続くケース)を結果として返す場合はtrue
	 * @return sep_charで区切られた文字列のvector
	 */
	inline std::vector<CString> split( const CString& target, const CString& sep_char, bool token_compress_off = false )
	{
		// CString::Tokenize()は空文字を強制的に読み飛ばしてしまう。
		// それでは困る場合もあるので、boost::split()に倣う。
		// 全面的に置き換えてしまっても良いのだが、、ここでアルゴリズムを変えるのは何となく怖いので、
		// token_compress_offの場合のみお世話になる。
		if( token_compress_off )
			return split_( target, sep_char );
		
		std::vector<CString> vecRet;

		int nPos = 0;
		while( 0 <= nPos )
		{
			// 2013/10/7 MBCS環境で2byte文字の処理を行った時に、不要な文字が出力されていたので修正
			CStringW token = CStringW(target).Tokenize(CStringW(sep_char), nPos);
			if( !token.IsEmpty() )
				vecRet.push_back( CString(token) );
		}

		// moveを明示したいが、VC9を考慮して暗黙のmoveに期待
		// return std::move(vecRet); 
		return vecRet;
	}
	
	/**
	 * @brief       文字列として結合する
	 * @param[in]	target		対象のコンテナ（ToString()で文字列化できる型である必要がある）
	 * @param[in]	join_char	結合文字として指定された文字列
	 * @return       join_charで区切られた文字列
	 */
	template <typename T>
	inline CString join( const T& target, const CString& join_char )
	{
		CString strRet;
		for( T::const_iterator itr = target.begin(); itr != target.end(); ++itr )
		{
			if( itr != target.begin() )
				strRet.Append( join_char );

			strRet.Append( ToString(*itr) );
		}

		return strRet;
	}
	
	/**
	 * @brief 書式化
	 * @param[in]	lpcszFormat	書式
	 * @return 書式化された文字列
	 */
	inline CString format( LPCTSTR lpcszFormat, ... )
	{
		CString ret;

		if( lpcszFormat )
		{
			va_list args;
			va_start( args, lpcszFormat );
			ret.FormatV( lpcszFormat, args );
			va_end(args);
		}
		return ret;
	}
	
	/**
	 * @brief 書式化（ANSI版）
	 * @note u系ツールのお供に
	 * @param[in]	lpcszFormat	書式
	 * @return 書式化された文字列（ANSI）
	 */
	inline CStringA formatA( LPCTSTR lpcszFormat, ... )
	{
		CString ret;

		if( lpcszFormat )
		{
			va_list args;
			va_start( args, lpcszFormat );
			ret.FormatV( lpcszFormat, args );
			va_end(args);
		}
		return CStringA(ret);
	}

#ifdef _UNICODE
	inline CStringA formatA( LPCSTR lpcszFormat, ... )
	{
		CStringA ret;

		if( lpcszFormat )
		{
			va_list args;
			va_start( args, lpcszFormat );
			ret.FormatV( lpcszFormat, args );
			va_end(args);
		}
		return ret;
	}
#endif
	
	/**
	 * @brief 文字列中の全角文字を全て半角文字に変換した文字列を返す
	 * @warning ロケールによって結果が異なるので留意すること
	 * @note {変換前→変換後} Ａ→A ａ→a １→1 ア→ｱ
	 * @note ￥は使用する文字セットによって異なる
	 * UnicodeならASCIIの\、マルチバイト文字はエスケープになる
	 * @note 半角文字にならないものは全角のまま あ→あ 等
	 * @param str [in] 半角文字を含んだ文字列
	 * @return strの全角文字が半角文字に変換された文字列
	 */
	inline CString ToHalf(LPCTSTR str)
	{
		INT nTextLen = lstrlen( str ) + 1;
		CString strTextBuf;
		LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, str, nTextLen, strTextBuf.GetBuffer(nTextLen), nTextLen );

		CString strResult(strTextBuf);
		strTextBuf.ReleaseBuffer();
		return strResult;
	}

	inline CString LoadString( UINT uID )
	{
		CString str; 
		str.LoadString( uID );
		return str;
	}

#ifdef _UNICODE
	inline CStringA LoadStringA( UINT uID )
	{
		return CStringA( LoadString(uID) );
	}
#endif
} // str_util
} // uh
