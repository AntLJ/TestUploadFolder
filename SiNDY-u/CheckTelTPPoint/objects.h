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

// 調査結果コード
typedef enum ePointStatC
{
	NOSURVEY = 0,		/// 未調査
	UNKNOWN_POS,		/// 位置不明
	GAITOU_SISETU,		/// 該当施設あり
	HORYU				/// 保留
} POINTSTAT_C;

// 住所整備種別
typedef enum eAddrClassC
{
	OAZA_NOTSEIBI = 0,		/// 町丁目界未整備
	JUKYO_SEIBI,			/// 住居表示整備済み
	TIBAN_SEIBI,			/// 地番整備済み
	JUKYO_TIBAN_NOTSEIBI	/// 住居表示・地番未整備
} ADDRCLASS_C;

// 座標付与種別
typedef enum eAddXYC
{
	NORMAL = 0,			/// 通常
	CLAIM,				/// クレーム
	SAME_ADDR,			/// 同一住所
	GUIDE_P_GEN,		/// 品証誘導ポイント（一般道路）
	GUIDE_P_HWY,		///	品証誘導ポイント（高速道路）
	PINPOINT_TWN,		/// TWNピンポイント落ち回避（加除移行）
	PINPOINT_HLP		/// HLPピンポイント落ち回避（加除移行）
} ADDXY_C;

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

	IGeometryPtr getGeo() const {
		IGeometryPtr ipGeo;
		m_ipFeature->get_Shape(&ipGeo);
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

	/**
	 * @brief	必要情報セット
	 * @note	既に取得しておきたい情報等をセットする関数
	 */
	virtual void setInfo() = 0;


protected:

	IFeaturePtr m_ipFeature;		/// フィーチャ
	//IGeometryPtr m_ipGeo;			/// ジオメトリ
};

/**
 * @class	CTelTP
 * @brief	拡張電話番号検索ポイントクラス
 */
class CTelTP : public CFeature
{
public:
	CTelTP( IFeature* ipFeature ) : 
	  CFeature(ipFeature)
	{
		//DistToPOI = 0;
		setInfo();	/// 良く使うやつは先に取得しておく
	}

	~CTelTP(){};

	/**
	 * @brief	住所コード取得
	 * @note	範囲外を指定したときは、全部返す
	 * @param	nLevel	[in]	桁数
	 * @return	string
	 */
	const std::string getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.substr(0, nLevel) );
	}

	/**
	 * @brief	調査結果ドメイン取得
	 * @return	string
	 */
	const std::string getPointStatDomain() const;

	/**
	 * @brief	座標付与種別ドメイン取得
	 * @return	string
	 */
	const std::string getAddXYDomain() const;

/*
	void SetDistToPOI(double dist)
	{
		DistToPOI = dist;
	}
*/
private:
	
	/**
	 * @brief	メンバ変数にセット
	 */
	void setInfo();

public:

	std::string	m_strAddrCode;		/// 住所コード
	POINTSTAT_C	m_ePointStatC;		/// 調査結果コード
	ADDXY_C		m_eAddXYC;			/// 座標付与種別
	long		m_lSequence;		/// シーケンス

	std::string	m_strAddrName;		/// 住所名
	std::string	m_strKatagaki;		/// 方書名称
	std::string m_strKeisai;		/// 掲載名称
	std::string	m_strSource;		/// 情報ソース
	std::string m_strTelNum;		/// 電話番号

//	double dX_TelTP;				/// TelTPのX座標
//	double dY_TelTP;				/// TelTPのY座標

	//double DistToPOI;				/// TelTPとPOIとの距離(m)
};



/**
 * @class	CCityAdmin
 * @brief	都市地図行政界クラス
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin( IFeature* ipFeature ) : 
	  CFeature(ipFeature),
	  m_lGaikuFugo(0),
	  m_strExGaikuFugo("")

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
	const std::string getAddrCode ( int nLevel ) const
	{
		return ( m_strAddrCode.substr(0, nLevel) );
	}

private:

	/**
	 * @brief	メンバ変数にセット
	 */
	void setInfo();

public:

	std::string	m_strAddrCode;		/// 住所コード
	LONG		m_lGaikuFugo;		/// 街区符号
	std::string m_strExGaikuFugo;	/// 拡張街区符号
	ADDRCLASS_C m_eAddrClassC;		/// 住居表示整備

};

/**
 * @class	CSite
 * @brief	背景クラス
 * @note	都市地図・中縮兼用
 */
class CSite : public CFeature
{
public:
	CSite( IFeature* ipFeature, char szScale ) : 
	  CFeature(ipFeature),
	  m_szScale(szScale),
	  m_lBGClassC(0),
	  m_strBGClass("")
	{
		setInfo();	/// 良く使うやつは先に取得しておく
	}

	~CSite(){};

	/**
	 * @brief	背景種別ドメイン
	 * @return	string
	 */
	const std::string getBGClassDomain();

private:

	/**
	 * @brief	メンバ変数にセット
	 */
	void setInfo();

private:
	char		m_szScale;		/// 背景のスケール(C : 都市地図 B : 中縮)

public:

	LONG		m_lBGClassC;	/// 背景種別(中縮ならS4)
	std::string	m_strBGClass;	/// 背景種別名
};

#endif // !defined(AFX_CITYADMIN_H__C2F259F5_5399_48AA_A610_5A3F4E64E0BC__INCLUDED_)
