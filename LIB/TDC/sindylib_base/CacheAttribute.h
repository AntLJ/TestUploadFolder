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
 * @file CacheAttribute.h
 * @brief <b>CCacheAttributeクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _CACHEATTRIBUTE_H_
#define _CACHEATTRIBUTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

HRESULT SINDYLIB_API Fix_BSTR_get_Value( _IRow* p, long index, VARIANT* va );

namespace sindy {

class CFieldMap;

/**
 * @class CCacheAttribute
 * @brief <b>属性キャッシュクラス</b>\n
 * このクラスはIRowが保持する属性を一部を除いてキャッシュします。\n
 * キャッシュ作成の際に除かれるフィールドタイプは以下のとおりです。
 * @li ジオメトリフィールド
 * @li BLOBフィールド
 * キャッシュバッファ上には、上記を格納するための場所が用意されていますが、
 * 実際にはコピーされませんので注意して下さい。
 */
class SINDYLIB_API CCacheAttribute  
{
public:
	explicit CCacheAttribute()
	{
		m_pVars = NULL;
		Init();
	}
	virtual ~CCacheAttribute()
	{
		Init();
	}

	CCacheAttribute( _IRow* lp )
	{
		m_pVars = NULL;
		CreateCache( lp );
	}
	CCacheAttribute& operator=( _IRow* lp )
	{
		CreateCache( lp );
		return *this;
	}
	CCacheAttribute( const CCacheAttribute& lp );
	CCacheAttribute& operator=( const CCacheAttribute& lp );

	/**
	 * @brief <b>インデックス番号に対応するデータを返す</b>\n
	 * 内部バッファのポインタをそのまま返しますので、扱いに
	 * 注意して下さい。
	 * @param index	[in]	インデックス番号
	 * @return 対応するデータが無ければNULL
	 */
	VARIANT* operator[]( long index )
	{
		return ( m_pVars ) ? &m_pVars[index] : NULL;
	}
	
	/**
	 * @brief <b>インデックス番号に対応するデータを返す</b>\n
	 * 参照専用です。
	 * @param index	[in]	インデックス番号
	 * @return 対応するデータが無ければNULL
	 */
	const VARIANT* operator[]( long index ) const
	{
		return ( m_pVars ) ? &m_pVars[index] : NULL;
	}
	
	/**
	 * @brief <b>フィールドカウントを返す</b>\n
	 * キャッシュされたフィールドの数ではありませんので
	 * 注意して下さい。
	 */
	long GetCount() const { return m_lCount; }

	/**
	 * @brief <b>属性のキャッシュを作成する</b>\n
	 * 既存のものがある場合は削除後に作成されます。
	 * キャッシュ作成の際に除かれるフィールドタイプは以下のとおりです。
	 * @li ジオメトリフィールド
	 * @li BLOBフィールド
	 * キャッシュバッファ上には、上記を格納するための場所が用意されていますが、
	 * 実際にはコピーされませんので注意して下さい。
	 * @param lp		[in]			_IRow*
	 * @param ipFields	[in,optional]	IFields*（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateCache( _IRow* lp, IFields* ipFields = NULL );
	errorcode::sindyErrCode CreateCache( _IRow* lp, const CFieldMap& cFields );

	/**
	 * @brief <b>CCacheAttributeをコピーする</b>\n
	 * 内部的にはm_pVarsとm_lCountがコピーされるだけです。\n
	 * 既存のものがある場合は削除後に作成されます。
	 * @param dest	[out]	CCacheAttribute&
	 * @param src	[in]	CCacheAttribute&
	 * @return sindyErrCode
	 */
	friend errorcode::sindyErrCode CopyCacheAttribute( CCacheAttribute& dest, const CCacheAttribute& src );
private:
	/**
	 * @brief <b>メンバ変数を初期化する</b>\n
	 */
	void Init();
private:
	long		m_lCount;	//!< 属性数
	VARIANT*	m_pVars;	//!< 属性キャッシュバッファ
};

} // sindy

#endif // _CACHEATTRIBUTE_H_
