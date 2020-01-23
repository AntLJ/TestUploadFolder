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

//CheckTelTPPoint.h:	CCheckTelTPPoint　クラスのインタフェイス


#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "ErrorHandle.h"

class CCheckTelTPPoint
{
public:
	CCheckTelTPPoint( CEnviron& rEnv );
	virtual ~CCheckTelTPPoint();

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 */
	void execute();

private:

	/**
	 * @brief	引数解析
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool checkArg( int argc, char* argv[] );

	/**
	 * @brief	環境変数取得し、必要情報取得
	 * @return	bool
	 */
	bool setInfoFromSDE();

	/**
	 * @brief	メッシュリスト読み込み
	 * @param	lpszFile	[in]	メッシュリスト
	 * @return	bool
	 */
	bool loadMeshlist( LPCTSTR lpszFile );

	/**
	 * @brief	メッシュフィーチャ取得
	 * @param	lMeshCode	[in]	メッシュコード
	 * @return	メッシュフィーチャ
	 */
	IFeaturePtr getMeshFeature( long lMeshCode );

	/**
	 * @brief	チェックメイン
	 */
	void checkMain();

	/**
	 * @brief	メッシュごとのチェック
	 */
	void checkMainByMesh();

	/**
	 * @brief	都市地図行政界との整合チェック
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPAndAdmin( const CTelTP& cTelTP );

	/**
	 * @brief	背景との整合チェック
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPAndSite( const CTelTP& cTelTP );

	/**
	 * @brief	同一緯度経度のポイントの調査結果コードチェック
	 * @brief	電話番号のフリーダイヤルチェック
	 * @param	cTelTP	[in]	TelTPPoint
	 */
	void checkTelTPPoints( const CTelTP& cTelTP );

	/**
	 * @brief	ポイントが乗ってるポリゴンを取得
	 * @note	境界上に乗っているときは、返さない
	 * @param	ipGeo		[in]	ジオメトリ(ポイント)
	 * @param	lpCount		[out]	取得ポリゴン数	
	 * @param	szSwitch	[in]	A 行政界 C 都市地図背景 B  中縮背景
	 * @return	IFeaturePtr
	 */
	IFeaturePtr getFeatureWithinPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch );

	/**
	 * @brief	指定種別が、ＮＧ都市地図背景種別？
	 * @param	lDomainCode	[in]	ドメイン値
	 * @return	bool
	 */
	bool isNGCitySite( LONG lDomainCode )
	{
		if( m_CBGClass_NG.find(lDomainCode) != m_CBGClass_NG.end() )
			return true;

		return false;
	}

	/**
	 * @brief	指定種別が、ＮＧ中縮背景種別？
	 * @param	lDomainCode	[in]	ドメイン値
	 * @return	bool
	 */
	bool isNGBaseSite( LONG lDomainCode )
	{
		if( m_BBGClass_NG.find(lDomainCode) != m_BBGClass_NG.end() )
			return true;

		return false;
	}

	/**
	 * @brief	TelTPの周囲のPOIのIPコード&電話番号チェック(Bug 5701対応 07/11/30追加)　
	 * @param	cTelTP	[in]		TelTPPoint
	 */
	void CheckIPCODE( const CTelTP& cTelTP );

	/**
	 * @brief	同一シーケンスを持つポイントが複数存在しないかチェック（Bug9295 追加）
	 * @note	調査結果コードが【該当施設あり】同士のみ
	 * @param	cTelTP	[in]	TelTPPoint
	 **/
	void CheckSameSequence( const CTelTP& cTelTP );


private:

	CErrorHandle	m_cError;				/// エラーハンドルクラス
	IWorkspacePtr	m_ipTPWorkspace;		/// TelTPワークスペース
	
	IWorkspacePtr	m_ipPOIWorkspace;		/// POIワークスペース(Bug 5701対応　07/11/30追加)

	CEnviron&		m_rEnv;					/// 環境変数

	std::vector<long>	m_vecMesh;			/// メッシュリスト

	std::vector<LONG>	m_vecTelTP;			/// TelTPポイント格納用ベクタ(スナップショット対策)


	IFeatureClassPtr m_ipTelTPFC;			/// 電話検索ポイントフィーチャクラス
	IFeatureClassPtr m_ipCityAdminFC;		/// 都市地図行政界フィーチャクラス
	IFeatureClassPtr m_ipCitySiteFC;		/// 都市地図背景フィーチャクラス
	IFeatureClassPtr m_ipBaseSiteFC;		/// 中縮背景フィーチャクラス
	IFeatureClassPtr m_ipMeshFC;			/// メッシュフィーチャクラス
	IFeatureClassPtr m_ipCityMeshFC;		/// 都市地図メッシュフィーチャクラス
	IFeatureClassPtr m_ipPOIFC;				/// POIフィーチャクラス(Bug 5701対応　07/11/30追加)

	LONG	m_lCBGClassC_FID;				/// 都市地図背景種別フィールドインデックス
	LONG	m_lBSC4BGClassC_FID;			/// 中縮背景S4種別フィールドインデックス

	// 電話番号検索ポイントが存在してはだめな都市地図背景種別
	static const std::set<LONG> m_CBGClass_NG;

	// 電話番号検索ポイントが存在してはだめな中縮背景種別
	static const std::set<LONG> m_BBGClass_NG;

	double	m_dMeter;						/// 10進度により探索したい半径を引数から受け取る(Bug 5701対応 07/11/30追加)

};

#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)




















