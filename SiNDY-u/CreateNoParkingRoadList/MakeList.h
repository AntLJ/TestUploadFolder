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

class CNoParkingLink
{
public:
	CNoParkingLink() : lNoParkingClass(0) {};
	~CNoParkingLink(){};
public:
	IFeaturePtr ipLink;
	long lNoParkingClass;
};

class CMakeList
{
public:
	CMakeList(void);
	~CMakeList(void);

	/**
	 * @brief 駐禁重点取締路線リスト作成処理を行う。
	 * @note  駐禁重点取締路線リスト作成処理を行う。
	 *
	 * @param	ipLinkClass		[in]		道路リンククラス
	 * @param	vecFeatureClass	[in]		駐禁取締路線クラスリスト
	 * @param	bShape			[in]		駐禁取締路線クラスリストがshapeかPGDBか[true:shape,fales:pgdb]
	 * @param	ofp				[out]		出力先リスト
	 * @param	lpcszShapeDir	[in]		出力shapeディレクトリ
	 * @param	lpcszLinkDir	[in]		駐車禁止取締区間のラインの出力shapeディレクトリ
	 *
	 * @return	true		正常終了
	 * @return	false		異常終了
	 */
	bool create( IFeatureClass* ipLinkClass, const std::vector<CAdapt<IFeatureClassPtr>>& vecFeatureClass, bool bShape, std::ofstream& ofp, LPCTSTR lpcszShapeDir, LPCTSTR lpcszLinkDir );

private:

	/**
	 * @brief PGDBの駐車禁止情報種別毎に対象リンク抽出処理を行う。
	 * @note  PGDBの駐車禁止情報種別毎に対象リンク抽出処理を行う。
	 *
	 * @param	ipLinkClass			[in]		道路リンククラス
	 * @param	ipNoParkClass		[in]		駐禁取締路線クラス
	 * @param	lpcszWhereClause	[in]		フィーチャ検索用
	 * @param	ofp					[out]		出力先リスト
	 * @param	bFlag				[in]		最重点フラグ(true:最重点、false:重点)
	 *
	 * @return	true		正常終了
	 * @return	false		異常終了
	 */
	bool SearchClass( IFeatureClass* ipLinkClass, IFeatureClass* ipNoParkClass, LPCTSTR lpcszWhereClause, std::ofstream &ofp, bool bFlag );

	/**
	 * @brief 指定した距離のバッファを作成する。
	 * @note  指定した距離分のバッファをリンクに持たせた形状を作成する。
	 *
	 * @param	ipGeo		[in]		道路リンク形状
	 *
	 * @return バッファを持った形状
	 */
	IGeometryPtr GetBuffer( IGeometry* ipGeo );

	/**
	 * @brief バッファに含まれるリンクを取得する。
	 * @note  バッファに含まれているリンクを取得し、判定する。
	 *
	 * @param	ipLinkClass	[in]		道路リンククラス
	 * @param	ipBuff		[in]		バッファ形状
	 * @param	ofp			[out]		出力リスト
	 * @param	lClass		[in]		駐車禁止情報種別
	 *
	 * @return バッファを持った形状
	 */
	bool CompData( IFeatureClass* ipLinkClass, IGeometry* ipBuff, std::ofstream& ofp, long lClass );

	/**
	 * @brief バッファに含まれているリンク長を測定する。
	 * @note  指定したリンクのバッファに含まれている長さを測定する。
	 *
	 * @param	ipLinkGeo	[in]		道路リンク
	 * @param	ipPolygon	[in]		駐禁路線バッファ
	 * @param	lOID		[in]		リンクID
	 *
	 * @return リンク長(m)
	 */
	double GetIntersecLinkLen( IGeometry* ipLinkGeo, IGeometry* ipPolygon, long lOID );

	/**
	 * @brief リンク長を測定する。
	 * @note  指定したリンクの長さを測定する。
	 *
	 * @param  ipLinkGeom	 [in]				道路リンク
	 *
	 * @return リンク長(m)
	 */
	double GetLinkLen( IGeometry* ipLinkGeom );

	/**
	 * @brief リンクが存在するメッシュを取得する。
	 * @note  指定したリンクが存在するメッシュコードを取得する。
	 *
	 * @param  ipLink		 [in]				道路リンク形状
	 *
	 * @return 2次メッシュコード
	 */
	long GetExistMesh( IGeometry* ipLink );

	/**
	* @brief 10進数→16進数表示に変換する。
	* @note  指定の10進数値を16進数表示に変換する。
	*
	* @param	rCstrID		[out]	16進数表示用文字列
	* @param	rId			[in]	10進数値
	*
	* @return	なし
	*/
	void SetObjectID( CString& rCstrID, int rId) ;

	/**
	* @brief	駐車禁止路線の道路リンクをshapeで出力
	* @param	ipLinkClass		[in]		道路リンククラス
	* @retval	true	出力成功
	* @retval	false	出力失敗
	*/
	bool OutputShape( IFeatureClass* ipLinkClass );

	/**
	* @brief	道路リンク形状をshape形状にする
	* @param	cSHPHandle		[in]		Shapeハンドル
	* @param	ipGeom			[in]		道路リンク形状
	* @return	shapeのOID
	*/
	int CreateOneShape( CSHPHdl& cSHPHandle, IGeometry* ipGeom );

	/**
	* @brief	元道路の形状をshapeで出力
	* @note		PGDBではなくshapeの場合は都道府県別になっているのでそれを一つにまとめたいため
	* @param	vecBaseLink		[in]		元道路群
	*/
	void OutputBaseLink( const std::vector<CAdapt<IFeaturePtr>>& vecBaseLink );

	/**
	* @brief	19座標系第10領域に座標変換した形状のクローン作成
	* @note		空間参照がセットされていない形状の場合は初期値を設定（esriSRGeoCS_Tokyo:4301）
	*/
	IGeometryPtr CMakeList::get_Tokyo10CloneGeom(IGeometry* ipGeom);

private:
	std::map<long,CNoParkingLink> m_mapNoParkingLink;	//!< 最重点路線リンクIDとの駐車禁止リンクのマップ
	std::map<long, double> m_LinkLength;	//!< リンク長さ

	std::vector<CAdapt<IFeaturePtr>> m_vecBaseLink;		//!< 駐車禁止取締区間のライン群

	long m_lCount;							//!< フィーチャクラスでループ処理するときのカウンタ
	long m_lFeatureClassSize;				//!< フィーチャクラスの総数

	CString m_strShapeDir;					//!< 出力shapeディレクトリ
	CString m_strLinkDir;					//!< 駐車禁止取締区間のラインの出力shapeディレクトリ
};
