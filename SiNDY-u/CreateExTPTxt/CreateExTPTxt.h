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

// CreateExTPTxt.h: CreateExTPTxt クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_)
#define AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

/**
 * @struct	CExTPRec
 * @brief	ExTPレコード
 */
struct CExTPRec
{
	long		m_lOID;			//!< OBJECTID
	CString		m_strAddrCode;	//!< 住所コード
	long		m_lInfoSrc;		//!< 初期座標提供元コード
	WKSPoint	m_point;		//!< 位置情報

	CExTPRec() : m_lOID(-1), m_lInfoSrc(-1)
	{
		m_point.X = 0.0; m_point.Y = 0.0;
	}
};

typedef std::map<CString, CExTPRec> EXTPREC_MAP;	// キー : 20桁住所コード 値 : ExTPレコード

/**
 * @class	CreateExTPTxt
 * @brief	ExTP出力クラス
 */
class CreateExTPTxt
{
public:
	CreateExTPTxt(){}
	~CreateExTPTxt(){}

	/**
	 * @brief	使用法
	 */
	static void Usage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool Init( int argc, char** argv );

	/**
	 * @brief	実行
	 */
	void Execute();

private:

	/**
	 * @brief	ExTP追加
	 * @param	cExTP	[in]	ExTP
	 */
	void AddExTP( const CExTPRec& cExTP );

	/**
	 * @brief	レコード出力
	 */
	void OutputExTP();

private:

	std::ofstream		m_ofs;				//!< 出力ストリーム
	IFeatureClassPtr	m_ipExTPClass;		//!< ExTPポイントクラス

	EXTPREC_MAP			m_mapExTP;			//!< ExTPリスト
};

#endif // !defined(AFX_CREATEEXTPTXT_H__84BB5780_E09F_4BDC_B954_0A1380233C0B__INCLUDED_)
