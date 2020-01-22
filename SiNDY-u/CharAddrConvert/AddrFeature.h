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

/**
 * @class	CAddrBaseFeature
 * @brief	住所系フィーチャラッパ基底クラス
 */
class CAddrBaseFeature
{
public:

	CAddrBaseFeature( IFeature* ipFeature, const CFieldMap& cFIDMap ) : m_ipFeature(ipFeature), m_cFIDMap(cFIDMap){}
	virtual ~CAddrBaseFeature(){}

	/**
	 * @brief	OID取得
	 * @return	long
	 */
	long GetOID() const
	{
		long lOID = 0;
		m_ipFeature->get_OID( &lOID );
		return lOID;
	}

	/**
	 * @brief	ジオメトリ取得
	 * @return	IGeometryPtr
	 */
	IGeometryPtr GetGeometry() const
	{
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape( &ipGeo );
		return ipGeo;
	}

private:

	/**
	 * @brief	属性取得
	 * @note	継承先で必ず実装
	 */
	virtual void SetAttr() = 0;

protected:

	IFeaturePtr		m_ipFeature;	//!< IFeature保持
	CFieldMap		m_cFIDMap;		//!< フィールドインデックスマップ
};

/**
 * @class	CAdminInfo
 * @brief	行政界情報
 */
class CAdminInfo : public CAddrBaseFeature
{
public:

	CAdminInfo( IFeature* ipFeature, const CFieldMap& cFIDMap ) : CAddrBaseFeature(ipFeature, cFIDMap){ SetAttr(); }
	~CAdminInfo(){}

private:

	/**
	 * @brief	属性取得
	 */
	void SetAttr();

public:

	CString		m_strAddrCode;		//!< 住所コード
	long		m_lGaikuFugo;		//!< 街区符号
	CString		m_strExtGaiku;		//!< 拡張街区符号
	CString		m_strExtGaikuYomi;	//!< 拡張街区符号ヨミ
	long		m_lAddrClass_C;		//!< 住所整備種別
	long		m_lAreaClass_C;		//!< エリア判別コード
	CString		m_strOazaAzaCode2;	//!< 通称行政界用大字・字コード(Bug8177)

};

typedef std::list<CString> TOKENLIST;

/**
 * @class	CGouInfo
 * @brief	号ポイント情報
 */
class CGouInfo : public CAddrBaseFeature
{
public:	

	CGouInfo( IFeature* ipFeature, const CFieldMap& cFIDMap ) : CAddrBaseFeature(ipFeature, cFIDMap), m_bMoji(false){ SetAttr(); }
	~CGouInfo(){};

	/** 
	 * @brief	号番号トークン数
	 * @return	トークン数
	 */
	long GetTokenCount() const { return (signed)m_listTokenName.size(); }

	/**
	 * @brief	号番号取得
	 * @param	iStart		[in]	スタート位置(-1なら全て取得)
	 * @param	iEnd		[in]	終わり位置(-1なら、スタートから全てを取得)
	 * @return	号番号
	 */
	CString GetGouNo( int iStart = -1, int iEnd = -1 ) const
	{
		return (-1 == iStart)? m_strGouNo : GetToken(m_listTokenName, iStart, iEnd);
	}

	/**
	 * @brief	号番号ヨミ取得
	 * @param	iStart		[in]	スタート位置(-1なら全て取得)
	 * @param	iEnd		[in]	終わり位置(-1)なら、スタートから全てを取得
	 * @return	号番号ヨミ
	 */
	CString GetGouNoYomi( int iStart = -1, int iEnd = -1 ) const
	{
		return (-1 == iStart)? m_strGouNoYomi : GetToken(m_listTokenYomi, iStart, iEnd);
	}

private:

	/**
	 * @brief	属性取得
	 */
	void SetAttr();

	/**
	 * @brief	トークン取得関数
	 * @param	listToken	[in]	トークンリスト
	 * @param	iStart		[in]	スタート位置
	 * @param	iEnd		[in]	終わり位置(-1)なら、スタートから全てを取得
	 */
	CString GetToken ( const TOKENLIST& listToken, int iStart, int iEnd = -1 ) const;

public:

	long			m_lGouType;				//!< 号タイプ
	bool			m_bExGouF;				//!< 拡張号番号フラグ
	bool			m_bPriorF;				//!< 優先フラグ
	bool			m_bMoji;				//!< 文字含む？(号番号に[#]あるかどうか)

private:

	CString			m_strGouNo;				//!< 号番号([#]は取り除く)
	CString			m_strGouNoYomi;			//!< 号番号ヨミ([#]が付いてるもののみ取得)

	TOKENLIST	m_listTokenName;			//!< 号番号を「-」でそれぞれ区切ったもの(#は除去)
	TOKENLIST	m_listTokenYomi;			//!< 号番号ヨミを「-」でそれぞれ区切ったもの(#は除去)

	/**
	 * @brief	トークン分解関数
	 * @param	strData		[in]		データ
	 * @param	lcpszSep	[in]		セパレータ
	 * @param	rList		[in,out]	格納用コンテナ
	 */
	void Tokenize( const CString& strData, LPCWSTR lcpszSep, TOKENLIST& rList )
	{
		// 内部でUNICODEにして分解
		CStringW strDataW( strData );
		int iCurPos = 0;
		CString strToken( strDataW.Tokenize( lcpszSep, iCurPos ) );
		while( !strToken.IsEmpty() )
		{
			rList.push_back( strToken );
			strToken = strDataW.Tokenize( lcpszSep, iCurPos );
		}
	}
};
