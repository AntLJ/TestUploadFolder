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
 * @struct	CMeshXY
 * @brief	メッシュＸＹ
 */
struct CMeshXY
{
	int	m_nMeshCode;	//!< メッシュコード
	int	m_nX;			//!< X
	int	m_nY;			//!< Y

	CMeshXY() : m_nMeshCode(0), m_nX(0), m_nY(0){}

	/**
	 * @brief	位置情報セット
	 * @param	ipPoint	[in]	ポイント
	 */
	void SetPos( IPointPtr ipPoint );
};

/**
 * @struct	CAddrBaseRec
 * @brief	住所レコード基底クラス
 */
struct CAddrBaseRec
{
	CString		m_strAddrCode;		//!< 住所コード
	double		m_dLon;				//!< 経度
	double		m_dLat;				//!< 緯度
	CMeshXY		m_cMXY;				//!< メッシュＸＹ

	CString		m_strPntName;		//!< 地番名称
	CString		m_strPntYomi;		//!< 地番ヨミ

	CString		m_strZipCode;		//!< 郵便番号

	long		m_lGouType;			//!< 号タイプ
	bool		m_bTsushoConv;		//!< 通称行政界により生成したレコード

	CAddrBaseRec() : m_dLon(0.0), m_dLat(0.0), m_lGouType(-1), m_bTsushoConv(false){}

	/**
	 * @brief	位置情報セット
	 * @brief	ipPoint	[in]	ポイント
	 */
	void SetCoord( IPointPtr ipPoint )
	{
		if( ipPoint )
		{
			ipPoint->QueryCoords( &m_dLon, &m_dLat );
			m_cMXY.SetPos( ipPoint );
		}
	}
};

struct CPntRec;
struct CGouRec;

/**
 * @struct	CGouRec
 * @brief	Gouデータレコード情報
 */
struct CGouRec : public CAddrBaseRec
{
	long		m_lOID;				//!< 元となっている号ポイントID
	CString		m_strGouName;		//!< 住居番号名称
	CString		m_strGouYomi;		//!< 住居番号ヨミ

	bool		m_bPriorF;			//!< 優先フラグ
	bool		m_bInArea;			//!< エリア内？

	double		m_dKakeiArea;		//!< 号ポイントが家形内にあるとき、その家形の面積

	CGouRec() : m_lOID(0), m_bPriorF(false), m_bInArea(false){}
};

/**
 * @struct	CPntRec
 * @brief	Pntデータレコード情報
 */
struct CPntRec : public CAddrBaseRec
{
	long	m_lRepDomariF;	//!< 代表点・地番止まりフラグ

	CPntRec() : m_lRepDomariF(0){}

	/**
	 * @brief	GOUレコードからのコンストラクタ
	 * @param	cGouRec		[in]	号レコード
	 * @param	lRepDomariF	[in]	代表点・地番止まりフラグ
	 */
	CPntRec( const CGouRec& cGouRec, long lRepDomariF )
	{
		m_strAddrCode	= cGouRec.m_strAddrCode.Left(16);
		m_strPntName	= cGouRec.m_strPntName;
		m_strPntYomi	= cGouRec.m_strPntYomi;
		m_strZipCode	= cGouRec.m_strZipCode;
		m_lGouType		= cGouRec.m_lGouType;
		m_bTsushoConv	= cGouRec.m_bTsushoConv;
		m_dLon			= cGouRec.m_dLon;
		m_dLat			= cGouRec.m_dLat;
		m_cMXY			= cGouRec.m_cMXY;
		m_lRepDomariF	= lRepDomariF;
	}
};

typedef std::map<CString, CPntRec> PNTMAP;		// キー:16桁住所コード 値:レコード
typedef std::vector<CGouRec> GOULIST;			// 号レコードリスト
typedef std::multimap<CString, CGouRec> GOUMAP;	// キー:20桁住所コード 値:レコード
