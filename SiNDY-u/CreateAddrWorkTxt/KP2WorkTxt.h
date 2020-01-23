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

#include "WorkTxtImpl.h"

#include <AddrLib/AreaCodeHelper.h>
#include <AddrLib/KLinkPoint.h>
#include <AddrLib/CityAdmin.h>

using namespace addr;

/**
 * @class	CKP2WorkTxt
 * @brief	家形リンクポイント中間データ作成クラス
 */
class CKP2WorkTxt : public CWorkTxtImpl
{
public:
	CKP2WorkTxt( const CParameter& rParam, std::ofstream& ofs ) : CWorkTxtImpl(rParam, ofs)
	{
	}
	~CKP2WorkTxt(void)
	{
	}

	/**
	 * @brief	初期化
	 * @return	bool
	 */
	bool Init();

	/**
	 * @brief	データ出力
	 * @return	bool
	 */
	bool WriteData();

private:

	/**
	 * @brief	市区町村コードリスト読み込み
	 * @param	lpcszFile	[in]	コードリスト
	 * @return	bool
	 */
	bool LoadCityCodeList( const _TCHAR* lpcszFile );

	/**
	 * @brief	最新11桁住所コード取得
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	最新11桁住所コード
	 */
	CString GetNewestCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	情報提供元コード変換
	 * @param	eInfoSrc	[in]	情報提供元コード
	 * @return	変換した情報提供元コード
	 */
	const _TCHAR* ConvInfoSrc( klink_point::info_src::ECode eInfoSrc );

	/**
	 * @brief	入力用座標精度コード変換
	 * @param	eInputSeido	[in]	入力用座標精度コード
	 * @return	変換した入力用座標精度コード
	 */
	const _TCHAR ConvInputSeido( klink_point::input_seido::ECode eInputSeido );

	/**
	 * @brief	リリース用座標精度コード変換
	 * @param	eReleaseSeido	[in]	リリース用座標精度コード
	 * @return	変換したリリース用座標精度コード
	 */
	const _TCHAR ConvReleaseSeido( klink_point::release_seido::ECode eReleaseSeido );

	/**
	 * @brief	行政界との整合情報取得
	 * @param	strKPAddrCode	[in]	家形リンクポイント住所コード
	 * @param	ipGeo			[in]	家形リンクポイントジオメトリ
	 * @param	pAdmin			[in]	既に紐付く行政界分かっているとき指定(なければNULL指定)
	 * @param	pEqualF			[out]	整合情報格納
	 *                                  0 : 住所コード一致
 	 *                                  1 : 不一致 or 対応行政界なし
 	 *                                  2 : 家形リンクの住所コードが不正
 	 *                                  3 : 無効な住所コード(廃止 or コード自体が住所マスタにない)
	 * @retval	true  : 成功
	 * @retval	false : 行政界取得できず
	 */
	bool GetEqualF( const CString& strKPAddrCode, IGeometry* ipGeo, const CCityAdmin* pAdmin, long* pEqualF );

	/**
	 * @brief	データ書き出し
	 * @param	stream		[in]	ストリーム
	 * @param	cKPPoint	[in]	KPポイント
 	 * @param	strAddrCode	[in]	住所コード
	 * @param	lEqualF		[in]	行政界整合状況
	 */
	void WriteData( std::ofstream& stream, const CKLinkPoint& cKPPoint, const CString& strAddrCode, long lEqualF );

private:

	std::set<CString>			m_setCode;				//!< 市区町村コードリスト
	std::map<CString, CString>	m_mapNewestAddrCode;	//!< 最新11桁コード(キャッシュ)
	std::set<long>				m_setWritedOID;			//!< 処理済家形リンクポイントIDリスト

	CAreaCodeHelper				m_cACHelper;			//!< 住所DB接続管理クラス
};
