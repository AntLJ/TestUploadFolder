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

#include "AddrBasePoint.h"
#include "Building.h"
#include "enum_def.h"

namespace addr
{
using namespace sindy;
using namespace sindy::schema;

typedef std::list<CString> GOUTOKENLIST;

class CGouPoint;
typedef boost::shared_ptr<CGouPoint> CSPGouPoint;

/**
 * @class	CGouPoint
 * @brief	号ポイント管理クラス
 */
class CGouPoint : public CAddrBasePoint
{
public:
	explicit CGouPoint()
	{
		Init();
		m_emTableType = sindyTableType::gou_point;
	}
	virtual ~CGouPoint(){ DeleteCache(); }

	CGouPoint( sindyTableType::ECode emTableType, IFeature* lp )
	{
		SetObject( lp, emTableType );
		SetGouNo();
	}
	CGouPoint( sindyTableType::ECode emTableType, _IRow*lp )
	{
		SetObject( lp, emTableType );
		SetGouNo();
	}
	CGouPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::gou_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
		SetGouNo();
	}
	explicit CGouPoint( const CGouPoint& obj )
	{
		*this = obj;
		m_strAddrCode = obj.m_strAddrCode;
		m_strAddrName = obj.m_strAddrName;
		SetGouNo();
		
	}
	CGouPoint& operator=( const CGouPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrCode = obj.m_strAddrCode;
		m_strAddrName = obj.m_strAddrName;
		SetGouNo();
		return *this;
	}

public:
	
	/**
	 * @brief	<b>号番号セット</b>
	 * @note	基本的にコンストラクタで初期化されるが、
	 *			デフォルトコンストラクタで宣言時は必ず最初に呼び出す！
	 */
	void SetGouNo()
	{
		CComVariant vaValue;
		m_strGouNo = (sindyErr_NoErr == GetValueByFieldName(gou_point::kGouNo, vaValue) && VT_BSTR == vaValue.vt)? 
					  CString(vaValue.bstrVal) : _T("");

		Tokenize( m_strGouNo, L"-", m_listTokenName );	// 取得した号番号をトークン分解
	}

	/**
	 * @brief	<b>号番号取得</b>
	 * @param	iStart		[in]	スタート位置(-1なら全て取得)
	 * @param	iEnd		[in]	終わり位置(-1なら、スタートから全てを取得)
	 * @return	号番号
	 */
	CString GetGouNo( int iStart = -1, int iEnd = -1 )
	{
		if( m_strGouNo.IsEmpty() )
			SetGouNo();
		return (-1 == iStart)? m_strGouNo : GetToken(m_listTokenName, iStart, iEnd);
	}

	/**
	 * @brief	<b>号番号トークン数取得</b>
	 * @return	トークン数
	 */
	long GetTokenCount() const{ return (signed)m_listTokenName.size(); }

	/**
	 * @brief	<b>号タイプ取得</b>
	 * @return	long
	 */
	long GetGouType() const{ return GetLongValueByFieldName( gou_point::kGouType, false ); }

	/**
	 * @brief	<b>優先号？</b>
	 * @return	bool
	 */
	bool IsPrior() const{ return GetBoolValueByFieldName( gou_point::kPriority, false ); }

	/**
	 * @brief	<b>拡張号番号？</b>
	 * @return	bool
	 */
	bool IsExGouNo() const{ return GetBoolValueByFieldName( gou_point::kExpGouNo, false ); }

	/**
	 * @brief	<b>住所一致桁数</b>
	 * @return	住所一致桁数
	 */
	CString GetPrecisionCode() const
	{
		// どういった形で返すのかは後ほど
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(gou_point::kPrecisionCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");		
	}

	/**
	 * @brief	<b>住所確認ステータス</b>
	 * @return	確認フラグ
	 */
	gou_point::	confirm::ECode GetConfirm() const
	{
		return static_cast<gou_point::confirm::ECode>(GetLongValueByFieldName(gou_point::kConfirm, 0));
	}

	/**
	 * @brief	<b>20桁住所コード取得</b>
	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @return	20桁住所コード
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false );

	/**
	 * @brief	<b>住所名称取得</b>
	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @return	住所名称
	 */
	CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>号ポイントが属する建物取得</b>
	 * @param	ipBldClass	[in, optional]	都市建物テーブル(NULL時は、号ポイントと同一ワークスペースから検索)
	 * @return	都市建物
	 */
	CBuilding* GetBuilding( IFeatureClassPtr ipBldClass = NULL );

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * CRow::DeleteCacheをオーバロード
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		CRow::DeleteCache( bDeleteContainerCache );
		m_strGouNo = _T("");
		m_strAddrCode = _T("");
		m_strAddrName = _T("");
		m_listTokenName.clear();
		m_listTokenYomi.clear();
	}

private:

	/**
	 * @brief	<b>トークン分解関数</b>
	 * @param	strData		[in]		データ
	 * @param	lcpszSep	[in]		セパレータ
	 * @param	rList		[in,out]	格納用コンテナ
	 */
	void Tokenize( const CString& strData, const wchar_t* lpcszSep, GOUTOKENLIST& rList );

	/**
	 * @brief	<b>トークン取得関数</b>
	 * @param	listToken	[in]	トークンリスト
	 * @param	iStart		[in]	スタート位置
	 * @param	iEnd		[in]	終わり位置(-1)なら、スタートから全てを取得
	 */
	CString GetToken ( const GOUTOKENLIST& listToken, int iStart, int iEnd = -1 ) const;

	/**
	 * @brief	<b>文字付き住所？</b>
	 * @return	ステータス
	 */
	addrStatus IsCharAddr();

	/**
	 * @brief	<b>20桁コード生成(文字考慮しない)</b>
	 * @return	20桁コード
	 */
	CString GetAddrCode_NoChar();

	/**
	 * @brief	<b>20桁コード生成(文字考慮)</b>
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @return	20桁コード
	 */
	CString GetAddrCode_Char( bool bCodeDBSaved = false );
	
private:

	CString			m_strGouNo;			//!< 号番号(キャッシュ用)
	mutable	CString	m_strAddrCode;		//!< 20桁住所コード(キャッシュ用)
	mutable CString	m_strAddrName;		//!< 住所名称(キャッシュ用)

	GOUTOKENLIST	m_listTokenName;
	GOUTOKENLIST	m_listTokenYomi;
};

}	// namespace addr
