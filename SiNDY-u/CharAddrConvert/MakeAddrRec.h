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

#include "CodeConvTable.h"
#include <oo4o/include/oracl.h>
#pragma comment(lib, "../../LIB/oo4o/lib/ORACLM32.LIB")

/**
 * @enum	EADDR_STATUS
 * @brief	住所ステータス
 */
enum EADDR_STATUS
{
	kNormal,			//!< 通常住所
	kExtGaiku,			//!< 拡張街区符号あり
	kMojiGou,			//!< 号番号に文字あり
	kMojiGou_ExtGaiku,	//!< 号番号に文字あり(拡張街区符号内)
	kSepOver,			//!< 号番号の区切りが多い
	kSepOver_ExtGaiku,	//!< 号番号の区切りが多い(拡張街区符号内)
	kNoOver,			//!< 地番・号で表現しきれない番号
	kNoOver_ExtGaiku,	//!< 地番・号で表現しきれない番号(拡張街区符号内)
};

/**
 * @class	CMakeAddrRec
 * @brief	住所レコード作成クラス
 */
class CMakeAddrRec
{
public:
	CMakeAddrRec(void) : m_lBGClass_FIdx(-1), m_lCreateYear_FIdx(-1){}
	~CMakeAddrRec(void){}

	/**
	 * @brief 初期化
	 * @param ipCityAdminFC    [in] 都市地図行政界フィーチャクラス
	 * @param ipCitySiteFC     [in] 都市地図背景フィーチャクラス
	 * @param ipBuildingFC     [in] 都市地図建物フィーチャクラス
	 * @param ipBuildingRoofFC [in] 都市地図屋上フィーチャクラス
	 * @param ipCityMeshFC     [in] 都市地図メッシュフィーチャクラス
	 * @param lpcszCodeConvDB  [in] コード変換DB接続先
	 * @param lpcszJusyoDB     [in] 住所マスタ接続先
	 * @return bool
	 */
	bool Init( const IFeatureClassPtr& ipCityAdminFC, const IFeatureClassPtr& ipCitySiteFC, const IFeatureClassPtr& ipBuildingFC, const IFeatureClassPtr& ipBuildingRoofFC, const IFeatureClassPtr& ipCityMeshFC, LPCTSTR lpcszCodeConvDB, LPCTSTR lpcszJusyoDB );

	/**
	 * @brief	行政界から地番代表点生成
	 * @param	cAdmin		[in]		行政界情報
	 * @param	rContainer	[in, out]	Pntレコード格納コンテナ
	 */
	void MakePntRecordByAdmin( const CAdminInfo& cAdmin, PNTMAP& rContainer );

	/**
	 * @brief	号レコードから地番代表点生成
	 * @param	rGouRec		[in]		号レコード格納
	 * @param	rContainer	[in, out]	Gouレコード格納コンテナ
	 */
	void MakePntRecordByGou( const GOUMAP& rGouRec, PNTMAP& rContainer );

	/**
	 * @brief	号レコード作成
	 * @param	cAdmin		[in]		行政界情報
	 * @param	cGou		[in]		号ポイント情報
	 * @param	rContainer	[in,out]	号レコード格納コンテナ
	 */
	void MakeGouRecordByDB( const CAdminInfo& cAdmin, const CGouInfo& cGou, GOUMAP& rContainer );

	/**
	 * @brief	号レコード一覧から代表住所にフィルタリング
	 * @param	rGouMap		[in]		号レコード一覧
	 * @param	rContainer	[in, out]	代表号レコード一覧
	 */
	void SelectRepGouRecord( const GOUMAP& rGouMap, GOUMAP& rContainer );

	/**
	 * @brief	有効な11桁？
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	bool
	 */
	bool IsValidAddrCode( LPCTSTR lpcszAddrCode )
	{
		return m_ZipMap.find(lpcszAddrCode) != m_ZipMap.end();
	}

private:

	/**
	 * @brief	文字付き住所？
 	 * @param	cAdmin	[in]	行政界情報
	 * @param	cGou	[in]	号ポイント情報
	 * @return	ステータス
	 */
	EADDR_STATUS IsCharAddr( const CAdminInfo& cAdmin, const CGouInfo& cGou );

	/**
	 * @brief	プロットNGな背景？
	 * @param	ipPoint	[in]	ポイント
	 * @return	bool
	 */
	bool IsNGSite( IPoint* ipPoint );
		
	/**
	 * @brief	屋上ポリゴンの面積取得
	 * @param	ipBldGeometry	[in]	家形ジオメトリ
	 * @return	面積(屋上ポリゴンがなければ、0)
	 */
	double GetRoofArea( const IGeometryPtr& ipBldGeometry );

	/**
	 * @brief	号ポイントが存在する家形の面積取得
	 * @param	ipPoint	[in]	ポイント
	 * @return	面積(家形内にポイントがなければ、0)
	 */
	double GetContainsBldArea( const IPointPtr& ipPoint );

	/**
	 * @brief	郵便番号取得
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	郵便番号(なければ不明[0000000])
	 */
	CString GetZipCode( LPCTSTR lpcszAddrCode )
	{
		std::map<CString, CString>::iterator itr = m_ZipMap.find( lpcszAddrCode );
		return itr != m_ZipMap.end()? itr->second : _T("0000000");
	}

	/**
	 * @brief	号レコードから地番代表点となりうる号ポイント選択
	 * @param	listGou		[in]		同一16桁GOUリスト
	 * @param	listTpg		[in]		同一16桁TPGリスト
	 * @param	rGouRec		[in,out]	地番代表点対象のGOuレコード格納用
	 * @param	bOazaIgnore	[in]		大字も生成ポイントを無視する？
	 * @return	代表点・地番止まりフラグ
	 */
	long SelectPntFromGouList( const GOULIST& listGou, const GOULIST& listTpg, CGouRec& rGouRec, bool bOazaIgnore = true );

	/**
	 * @brief	号レコードの位置関係から地番代表点となりうる号ポイント選択
	 * @note	地番代表点を各データ位置を元に決定
	 * @param	listChiban		[in]	同一16桁地番止まり
	 * @param	listNotChiban	[in]	同一16桁地番止まりでないリスト
	 * @param	rGouRec	[in,out]	地番代表点対象のGOuレコード格納用
	 */
	void SelectPntFromGouDist( const GOULIST& listChiban, const GOULIST& listNotChiban, CGouRec& rGouRec );

private:

	CCodeConvTable   m_cConvTable;       //!< コード対応表
	CCharAddrConvert m_cYomiTable;       //!< ヨミ付与クラス
	
	IFeatureClassPtr m_ipCityAdminFC;    //!< 都市地図行政界フィーチャクラス
	IFeatureClassPtr m_ipCitySiteFC;     //!< 都市地図背景フィーチャクラス
	IFeatureClassPtr m_ipBuildingFC;     //!< 都市地図建物フィーチャクラス
	IFeatureClassPtr m_ipBuildingRoofFC; //!< 都市地図建物屋上フィーチャクラス
	IFeatureClassPtr m_ipCityMeshFC;     //!< 都市地図メッシュフィーチャクラス

	long             m_lBGClass_FIdx;    //!< 都市背景種別フィールドインデックス
	long             m_lCreateYear_FIdx; //!< メッシュ作成年度フィールドインデックス

	std::map<CString, CString> m_ZipMap; //!< 郵便番号リスト(キー:11桁コード 値:郵便番号) ... 11桁が住所マスタに存在するかにも利用する
};
