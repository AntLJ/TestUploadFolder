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

// CheckCityRep.h: CheckCityRep クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_)
#define AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <string>

/// チェックモード
enum	ECheckMode {
	kCheckNone		= 0,
	kCheckAll		= 1,
	kCheckByList	= 2,
	kCheckByRange	= 3
};

typedef	set<CString>	AddrCodeList;	//!< 住所コード用set

/**
 * @class CheckCityRep
 * @brief 地番チェック用メインクラス
 */
class CheckCityRep
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CheckCityRep() : m_eCheckMode(kCheckNone), m_dSearchRange(0.002),
					 mFID_AnnoF(-1), mFID_PointClassC(-1), mFID_AddrCode(-1), mFID_RepTypeC(-1), mFID_ChibanNo(-1), mFID_GouType(-1), mFID_BgClassC(-1){}

	/**
	 * @brief デストラクタ
	 */
	~CheckCityRep(){}

	/**
	 * @brief	使用法
	 */
	static void Usage();

	/**
	 * @brief  初期化
	 * @param  argc	[in]	引数の数
	 * @param  argv	[in]	引数
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool Init( int argc, char* argv[] );

	/**
	 * @brief  実行
	 * @retval true  正常終了
	 * @retval false 異常終了
	 */
	bool Execute();

private:

	/**
	 * @brief  行政界コードリスト作成
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool fnMakeACList();

	/**
	 * @brief  同一11桁コードの行政界をひとまとめにする
	 * @param  strAddrCode  [in] 11桁住所コード
	 * @retval IGeometryPtr 同一11桁行政界を結合したジオメトリ
	 */
	IGeometryPtr fnUniteCityAdmin( const CString& strAddrCode );

private:

	ECheckMode m_eCheckMode;                   //!< チェックモード
	ofstream   m_ofs;                          //!< ログ出力ストリーム

	CString    m_strAddrList;                  //!< 市区町村コードリストファイル名
	CString    m_strRangeMin, m_strRangeMax;   //!< コード範囲モード時のmin max
	double     m_dSearchRange;                 //!< 地番注記検索範囲（度単位）

	IWorkspacePtr        m_ipWorkspaceAddress; //!< 住所用ワークスペース
	IWorkspacePtr        m_ipWorkspaceMap;     //!< 地図用ワークスペース

	ISpatialReferencePtr m_ipSpRef;            //!< 空間参照

	IFeatureClassPtr     m_ipFC_CityRepPoint;  //!< 地番代表点フィーチャクラス
	IFeatureClassPtr     m_ipFC_CityAdmin;     //!< 都市行政界フィーチャクラス
	IFeatureClassPtr     m_ipFC_CitySite;      //!< 都市背景フィーチャクラス
	IFeatureClassPtr     m_ipFC_GouPoint;      //!< 号ポイントフィーチャクラス

	AddrCodeList         mACList;              //!< 処理用住所コードリスト

	long       mFID_AnnoF, mFID_PointClassC, mFID_AddrCode, mFID_RepTypeC, mFID_ChibanNo, mFID_GouType, mFID_BgClassC; //!< フィールドID
};

#endif // !defined(AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_)
