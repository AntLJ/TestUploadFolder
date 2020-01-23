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

/**
 *	@file	GlobalFunction.h
 *	@brief	グローバル関数定義ファイル
 *	@author	F.Adachi
 *	@date	2005/08/05		新規作成
 *	$ID$
 */

#ifndef ___GLOBALFUNCTION___
#define ___GLOBALFUNCTION___

#include <string>

namespace GlobalFunction
{

	/**
	 *	@brief	フィールド値を取得
	 *	@note	列とフィールド名を指定してフィールドの値を取り出す
	 *	@param	ipRow			[in]	対象列インターフェイスへのポインタ
	 *	@param	cFieldName		[in]	対象フィールド名
	 *	@return	フィールド値
	 **/
	inline CComVariant	getValue( const _IRowPtr ipRow, const LPCTSTR cFieldName )
	{
		CComVariant	vaValue;
		vaValue.vt = VT_EMPTY;
		if (ipRow == NULL) { return vaValue; }

		// 列ポインタからフィールドポインタを取得
		IFieldsPtr	ipFields;
		ipRow->get_Fields( &ipFields );

		// 指定フィールド名のインデックスを取得
		LONG	aFieldIndex;
		if (ipFields->FindField( CComBSTR( cFieldName ), &aFieldIndex ) != S_OK) { return vaValue; }
		// インデックスを指定してフィールド値を取得
		if (aFieldIndex >= 0) { 
			ipRow->get_Value( aFieldIndex, &vaValue );
		}

		return vaValue;
	}


	/**
	 *	@brief	フィールド値を登録
	 *	@note	列とフィールド名を指定してフィールドの値を変更する
	 *	@param	ipRow			[in]	対象列へのポインタ
	 *	@param	cFieldName		[in]	対象フィールド名
	 *	@param	vaValue			[in]	登録するフィールド値
	 **/
	inline void setValue( const _IRowPtr ipRow, const LPCTSTR cFieldName, CComVariant vaValue )
	{
		std::string aFunc = "GlobalFunction::setValue()";

		if( vaValue.vt == VT_ERROR ) { throw std::runtime_error( "R1F:" + aFunc + " : フィールド値が不正" ); }
		if (ipRow == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

		// 列ポインタからフィールドポインタを取得
		IFieldsPtr ipFields;
		if (ipRow->get_Fields( &ipFields ) != S_OK) { throw std::runtime_error( aFunc + " : get_Field()失敗" ); }

		// 指定フィールド名のインデックスを取得
		LONG	aFieldIndex;
		if (ipFields->FindField( CComBSTR( cFieldName ), &aFieldIndex ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : FindField()失敗" ); }
		
		// インデックスを指定してフィールド値を登録
		if (aFieldIndex < 0) { throw std::runtime_error( "R1F:" + aFunc + " : フィールドのIndexが不正" ); }
		if (ipRow->put_Value( aFieldIndex, vaValue ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : put_Value()失敗" ); }
	}


	/**
	 * @brief	現在時刻の取得
	 * @param	crStr	[out]	取得した現在時刻を書き込む対象文字列へのリファレンス
	 */
	inline void getTime( CString &crStr )
	{
		SYSTEMTIME aSystemTime;
		::GetLocalTime(&aSystemTime);

		CString	aWord("");
		aWord.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
							aSystemTime.wYear,	aSystemTime.wMonth, aSystemTime.wDay,
							aSystemTime.wHour,	aSystemTime.wMinute,aSystemTime.wSecond);
		crStr = aWord;
	}

};

#endif
