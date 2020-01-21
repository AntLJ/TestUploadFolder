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
	 * @brief long�^�𕶎���ɕϊ�����
	 * @param[in]	l	long�l
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( long l ){
		CString str;
		str.Format( _T("%ld"), l );
		return str;
	}
	
	/**
	 * @brief int�^�𕶎���ɕϊ�����
	 * @param[in]	i	int�l
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( int i ){
		CString str;
		str.Format( _T("%d"), i );
		return str;
	}

	/**
	 * @brief unsigned long�^�𕶎���ɕϊ�����
	 * @param[in]	l	unsigned long�l
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( unsigned long l ){
		CString str;
		str.Format( _T("%ul"), l );
		return str;
	}
	
	/**
	 * @brief unsigned int�^�𕶎���ɕϊ�����
	 * @param[in]	i	unsigned int�l
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( unsigned int i ){
		CString str;
		str.Format( _T("%u"), i );
		return str;
	}

	/**
	 * @brief short�^�𕶎���ɕϊ�����
	 * @param[in]	s	short�l
	 * @retrn �ϊ���̕�����
	 */
	inline CString ToString( short s ){
		CString str;
		str.Format( _T("%d"), s );
		return str;
	}

	/**
	 * @brief double�^�𕶎���ɕϊ�����
	 * @param[in]	l	double�l
	 * @param[in]	fmt	CString::Format()�Ɏw�肷�鏑���i�f�t�H���g�F%.1lf�j
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( double d, const CString& fmt = _T("%.1lf") )
	{
		CString str;
		str.Format( fmt, d );
		return str;
	}

	/**
	 * @brief float�^�𕶎���ɕϊ�����
	 * @param[in]	f	float�l
	 * @param[in]	fmt	CString::Format()�Ɏw�肷�鏑���i�f�t�H���g�F%.1f�j
	 * @return �ϊ���̕�����
	 */
	inline CString ToString( float f, const CString& fmt = _T("%.1f") )
	{
		CString str;
		str.Format( fmt, f );
		return str;
	}
	
	/**
	 * @brief ������𕶎���ɕϊ�����
	 *        join()����Ă΂��p
	 * @param[in]	str	������
	 * @return �������g
	 */
	inline CString ToString( const CString& str )
	{
		return str;
	}
	/**
	 * @brief CLSID�𕶎���ɕϊ�����
	 * @param[in] clsid CLSID
	 * @return CLSID�𕶎��񉻂�������
	 */
	inline CString ToString( const CLSID& clsid )
	{
		LPOLESTR lpStr;
		::StringFromCLSID(clsid, &lpStr);
		CString strCLSID(lpStr);
		::CoTaskMemFree(lpStr);
		return strCLSID;
	}

	// 2013/10/02 GS�Ή���iMapCheck��GetTime()���ڐA
	/**
	 * @brief	LPSYSTEMTIME�𕶎���ɕϊ�����
	 * @param	lpSystemTime	[in]	���t
	 * @param	fmt	[in]	CString::Format()�Ɏw�肷�鏑���i�f�t�H���g�F%04d/%02d/%02d %02d:%02d:%02d�j
	 * @return	CString	�ϊ���̕�����(yyyy/MM/dd tt:mm:ss)
	 */
	inline CString ToString( LPSYSTEMTIME lpSystemTime, const CString& fmt = _T("%04d/%02d/%02d %02d:%02d:%02d"))
	{
		SYSTEMTIME stSystemTime;
		CopyMemory( &stSystemTime, lpSystemTime, sizeof(SYSTEMTIME));
		CString str;
		str.Format( fmt, stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

		return str;
	}

	// 2013/10/02 GS�Ή���iMapCheck��Value2String()���ڐA
	/**
	 * @brief  Variant�^�𕶎���ɕϊ�����
	 * @note �h���C�����ݒ肳��Ă��Ȃ��Ɩ��m�ȏꍇ�ɂ̂ݎg�p���邱��
	 *         "�h���C�����ݒ肳��Ă��邩������Ȃ�"�ꍇ�ɂ́A��������g�킸��2�����Ƀ}�b�v��n���ق����g��
	 * @param[in]	vaValue	�t�B�[���h�l
	 * @param[in]	fmt	CString::Format()�Ɏw�肷�鏑���i�f�t�H���g�F%.1f�j
	 * @retval	CString	�ϊ���̕�����(�t�B�[���h�l�ɂ���ď����͕ς��)
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
			
			// VT_EMPTY�AVT_NULL�̏ꍇ�͋󕶎���Ԃ�
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
	 * @brief CLSID�𕶎���ɕϊ�����
	 * @param[in] clsid CLSID
	 * @return CLSID�𕶎��񉻂�������
	 */
	inline CStringA ToStringA( const CLSID& clsid )
	{
		return CStringA( ToString(clsid) );
	}

	/**
	 * @brief �����񒆂̕������u������
	 * @note �ϊ���������src��move()�œn���ƍŏ��R�X�g�ƂȂ�i�����j
	 * @note new_���ȗ������src����old�͍폜�����iRemove�j
	 * @param[in]	src	�ϊ����̕�����
	 * @param[in]	old	src�Ɋ܂܂��u���Ώۂ̕�����
	 * @param[in]	new_	old�̒u����̕�����i�f�t�H���g�F�󕶎��j
	 * @return �ϊ���̕�����
	 */
	inline CString replace( CString src, const CString& old, const CString& new_ = _T("")  )
	{
		src.Replace( old, new_ );
		return src;
	}
	
	/**
	 * @brief boost ���g���� split
	 * @note  �K�`���K�`���ƕϊ�������̂Ŕ�����B�ʏ��split()���g�p���邱�ƁB
	 *        token_compress_on�̂݃T�|�[�g�B
	 * @param[in]	src	���̕�����
	 * @param[in]	sep	�Z�p���[�^�ƂȂ镶����
	 * @return sep_char�ŋ�؂�ꂽ�������vector
	 */
	inline std::vector<CString> split_( const CString& src, const CString& sep )
	{
		// boost��split���g���ƁA�Ȃ���sindylib_base�̃f�o�b�O�r���h���ʂ�Ȃ��Ȃ�
		// �Ƃ����s���ɂ܂�Ȃ������ۂ��N���邽�߁A�����tokenizer���g�����Ƃɂ���

		typedef boost::char_separator<wchar_t> char_sep;
		typedef boost::tokenizer< char_sep, std::wstring::const_iterator, std::wstring > tokenizer;
 
		std::wstring sepW = CT2W(sep); // �ꎞ�ϐ��ɓ���Ƃ��Ȃ��Ɨ�����̂Œ���
		char_sep separator(sepW.c_str(), L"", boost::keep_empty_tokens );

		std::wstring srcW = std::wstring( CT2W(src) ); // ��������
		tokenizer tokens(srcW, separator);
 
		std::vector<CString> ret;
		for( tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it )
		{
			ret.push_back( CString(it->c_str()) );
		}

		return ret;
	}


	/**
	 * @brief ������𕶎���ŕ�������
	 * @note  ��3�����̃f�t�H���g�� boost::algorithm::split �Ƌt�ӂȂ̂Œ���
	 * @param[in]	target	���̕�����
	 * @param[in]	sep_char	�Z�p���[�^�ƂȂ镶����
	 * @param[in]    token_compress_off ��̃t�B�[���h(�Z�p���[�^�������A���ő����P�[�X)�����ʂƂ��ĕԂ��ꍇ��true
	 * @return sep_char�ŋ�؂�ꂽ�������vector
	 */
	inline std::vector<CString> split( const CString& target, const CString& sep_char, bool token_compress_off = false )
	{
		// CString::Tokenize()�͋󕶎��������I�ɓǂݔ�΂��Ă��܂��B
		// ����ł͍���ꍇ������̂ŁAboost::split()�ɕ키�B
		// �S�ʓI�ɒu�������Ă��܂��Ă��ǂ��̂����A�A�����ŃA���S���Y����ς���͉̂��ƂȂ��|���̂ŁA
		// token_compress_off�̏ꍇ�݂̂����b�ɂȂ�B
		if( token_compress_off )
			return split_( target, sep_char );
		
		std::vector<CString> vecRet;

		int nPos = 0;
		while( 0 <= nPos )
		{
			// 2013/10/7 MBCS����2byte�����̏������s�������ɁA�s�v�ȕ������o�͂���Ă����̂ŏC��
			CStringW token = CStringW(target).Tokenize(CStringW(sep_char), nPos);
			if( !token.IsEmpty() )
				vecRet.push_back( CString(token) );
		}

		// move�𖾎����������AVC9���l�����ĈÖق�move�Ɋ���
		// return std::move(vecRet); 
		return vecRet;
	}
	
	/**
	 * @brief       ������Ƃ��Č�������
	 * @param[in]	target		�Ώۂ̃R���e�i�iToString()�ŕ����񉻂ł���^�ł���K�v������j
	 * @param[in]	join_char	���������Ƃ��Ďw�肳�ꂽ������
	 * @return       join_char�ŋ�؂�ꂽ������
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
	 * @brief ������
	 * @param[in]	lpcszFormat	����
	 * @return ���������ꂽ������
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
	 * @brief �������iANSI�Łj
	 * @note u�n�c�[���̂�����
	 * @param[in]	lpcszFormat	����
	 * @return ���������ꂽ������iANSI�j
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
	 * @brief �����񒆂̑S�p������S�Ĕ��p�����ɕϊ������������Ԃ�
	 * @warning ���P�[���ɂ���Č��ʂ��قȂ�̂ŗ��ӂ��邱��
	 * @note {�ϊ��O���ϊ���} �`��A ����a �P��1 �A���
	 * @note ���͎g�p���镶���Z�b�g�ɂ���ĈقȂ�
	 * Unicode�Ȃ�ASCII��\�A�}���`�o�C�g�����̓G�X�P�[�v�ɂȂ�
	 * @note ���p�����ɂȂ�Ȃ����̂͑S�p�̂܂� ������ ��
	 * @param str [in] ���p�������܂񂾕�����
	 * @return str�̑S�p���������p�����ɕϊ����ꂽ������
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
