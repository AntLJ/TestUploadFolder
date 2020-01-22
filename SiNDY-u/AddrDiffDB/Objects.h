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

#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/**
 * @file	Objects.h
 */

#include <string>

/**
 * @class	CFeature
 * @brief	フィーチャ管理クラス
 */
class CFeature
{
public:
	CFeature( IFeature* ipFeature ) : m_ipFeature(ipFeature)
	{
	};

	virtual ~CFeature(){};

	IFeaturePtr getFeature() const { return m_ipFeature; }

	IGeometryPtr getGeo() const
	{
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape( &ipGeo );
		return ipGeo; 
	};

	LONG getOID() const
	{
		LONG lOID;
		m_ipFeature->get_OID(&lOID);
		return lOID;
	}
	
	CComVariant getValue( LPCTSTR lpszField ) const;
	CComVariant getValue( LONG lFID ) const;

	virtual void setInfo() = 0;	/// 必ず継承先で実装する

protected:

	IFeaturePtr m_ipFeature;		/// フィーチャ
};

/**
 * @class	CCityAdminFID
 * @brief	都市地図行政界フィールドＩＤ郡
 */
class CCityAdminFID
{
public:
	CCityAdminFID(){};
	
	void set( ITablePtr ipTable )
	{
		ipTable->FindField( CComBSTR(city_admin::kCityCode), &m_lCityCode_FID );
		ipTable->FindField( CComBSTR(city_admin::kAddrCode), &m_lAddrCode_FID );
		ipTable->FindField( CComBSTR(city_admin::kGaikuFugo), &m_lGaikuFugo_FID );
		ipTable->FindField( CComBSTR(city_admin::kGaikuFugo), &m_lAddrClass_FID );
	}

	long	m_lCityCode_FID;		/// 都市コードフィールドインデックス
	long	m_lAddrCode_FID;		/// 住所コードフィールドインデックス
	long	m_lGaikuFugo_FID;		/// 街区符号フィールドインデックス
	long	m_lAddrClass_FID;		//!< 住所整備種別フィールドインデックス
};

/**
 * @class	CCityAdmin
 * @brief	都市地図行政界クラス
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin( IFeature* ipFeature, const CCityAdminFID& rFID ) : 
	  CFeature(ipFeature),
	  m_rFID(rFID),
	  m_lGaikuFugo(0)
	{
		setInfo();	/// 良く使うやつは先に取得しておく
	};
	
	~CCityAdmin(){};

	/**
	 * @brief	住所コード取得
	 * @note	範囲外を指定したときは、全部返す
	 * @param	nLevel	[in]	桁数
	 * @return	string
	 */
	const CString getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.Left(nLevel) );
	}

private:

	/**
	 * @brief	メンバ変数にセット
	 */
	void setInfo();

public:

	CString		m_strAddrCode;		//!< 住所コード
	long		m_lGaikuFugo;		//!< 街区符号
	long		m_lAddrClass;		//!< 住所整備種別

private:

	const CCityAdminFID&	m_rFID;
};

/**
 * @class	CGouPointFID
 * @brief	号ポイントフィールドＩＤ郡
 */
class CGouPointFID
{
public:
	CGouPointFID(){};
	
	void set( ITablePtr ipTable )
	{
		ipTable->FindField( CComBSTR(sindy::schema::gou_point::kGouNo), &m_lGouNo_FID );
		ipTable->FindField( CComBSTR(sindy::schema::gou_point::kExpGouNo), &m_lExpGouNo_FID );
	}

	LONG	m_lGouNo_FID;		/// 号ポイントフィールドインデックス
	LONG	m_lExpGouNo_FID;	/// 拡張号フラグフィールドインデックス
};

/**
 * @class	CGouPoint
 * @brief	号ポイント
 */
class CGouPoint : public CFeature
{
public:
	CGouPoint( IFeature* ipFeature, const CGouPointFID& rFID ) : 
	  CFeature(ipFeature),
	  m_rFID(rFID),
	  m_lExGouNoF(0),
	  m_lMeshCode(0)
	  {
		  setInfo();
	  };

	  ~CGouPoint(){};

private:

	/**
	 * @brief	メンバ変数にセット
	 */
	void setInfo();

public:

	CString		m_strGouNo;			/// 号番号
	LONG		m_lExGouNoF;		/// 拡張号番号フラグ

	LONG		m_lMeshCode;		/// 号ポイントがあるメッシュコード(グループ化モードのとき使用)

private:

	const CGouPointFID&	m_rFID;
};

#endif // !defined(AFX_CITYADMIN_H__C2F259F5_5399_48AA_A610_5A3F4E64E0BC__INCLUDED_)
