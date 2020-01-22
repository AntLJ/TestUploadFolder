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

// CheckExTPPoint.h: CCheckExTPPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
#define AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "ErrorHandle.h"

/**
 * @enum	emCheckMode
 * @brief	チェックモード
 */
enum emCheckMode
{
	kNormal,	//!< 通常チェックモード
	kAlps,		//!< アルプスチェックモード
	kAcc4,		//!< iPC(精度4)チェックモード[Bug6554]
	kiPS,		//!< iPSチェックモード[Bug6320]
	kHlp,		//!< HLPチェックモード[Bug8624]
};

/**
 * @class	CCheckExTPPoint
 */
class CCheckExTPPoint
{
public:
	CCheckExTPPoint();
	~CCheckExTPPoint();

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
	 * @brief	環境変数取得し、必要情報取得
	 * @return	bool
	 */
	bool setInfoFromDB();

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
	 * @param	cExTP	[in]	ExTPPoint
	 */
	void checkExTPAndAdmin( const CExTP& cExTP );

	/**
	 * @brief	都市地図行政界との整合チェック(アルプス用)
	 * @param	cExTP	[in]	ExTPPoint
	 */
	void checkAlpsAndAdmin( const CExTP& cExTP );

	/**
	 * @brief	背景との整合チェック
	 * @note	背景との整合はアルプスチェックモードも兼用
	 * @param	cExTP	[in]	EXTPPoint
	 */
	void checkExTPAndSite( const CExTP& cExTP );

	/**
	 * @brief	ポイントが乗ってるポリゴンを取得
	 * @note	境界上に乗っているときは、返さない
	 * @param	ipGeo		[in]	ジオメトリ(ポイント)
	 * @param	lpCount		[out]	取得ポリゴン数	
	 * @param	szSwitch	[in]	A 行政界 C 都市地図背景 B  中縮背景 M 都市地図メッシュ
	 * @return	IFeaturePtr
	 */
	IFeaturePtr getFeatureIntersectsPoint( IGeometry* ipGeo, LONG* lpCount, char szSwitch );

	/**
	 * @brief	指定種別が、ＮＧ都市地図背景種別？
	 * @param	lDomainCode	[in]	ドメイン値
	 * @return	bool
	 */
	bool isNGCitySite( long lDomainCode )
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
	bool isNGBaseSite( long lDomainCode )
	{
		if( m_BBGClass_NG.find(lDomainCode) != m_BBGClass_NG.end() )
			return true;
		return false;
	}

	/**
	 * @brief	同一20桁住所コードを持つポイントが複数存在しないかチェック（Bug9296 追加）
	 * @note	調査結果コードが【未調査】【位置不明】は除外
	 * @param	cExTP	[in]	EXTPPoint
	 **/
	void checkSame20Addrcode( const CExTP& cExTP );

private:

	emCheckMode		m_emCheckMode;			//!< チェックモード(通常 or アルプス or 上海)
	CString			m_strWhereClause;		//!< 抽出するWhere句(alpsだけのチェック等)

	CErrorHandle	m_cError;				//!< エラーハンドルクラス
	IWorkspacePtr	m_ipTPWorkspace;		//!< EXTPワークスペース
	
	std::vector<long>	m_vecMesh;			//!< メッシュリスト

	std::vector<CExTP>	m_vecExTP;			//!< ExTPポイント格納用ベクタ(スナップショット対策)

	CString			 m_strExTPFCName;		//!< 拡張電話検索ポイントフィーチャクラス名

	IFeatureClassPtr m_ipExTPFC;			//!< 拡張電話検索ポイントフィーチャクラス
	IFeatureClassPtr m_ipCityAdminFC;		//!< 都市地図行政界フィーチャクラス
	IFeatureClassPtr m_ipCitySiteFC;		//!< 都市地図背景フィーチャクラス
	IFeatureClassPtr m_ipBaseSiteFC;		//!< 中縮背景フィーチャクラス
	IFeatureClassPtr m_ipCityMeshFC;		//!< 都市地図メッシュフィーチャクラス[Bug 5622]
	IFeatureClassPtr m_ipTargetMeshFC;		//!< メッシュフィーチャクラス(チェックエリア絞り込み用)

	long	m_lCBGClassC_FID;				//!< 都市地図背景種別フィールドインデックス
	long	m_lBSC4BGClassC_FID;			//!< 中縮背景S4種別フィールドインデックス
	long	m_lCreateYear_FID;				//!< 都市地図メッシュ作成年度フィールドインデックス

	static const std::set<long> m_CBGClass_NG;		//!< 拡張電話番号検索ポイントが存在してはだめな都市地図背景種別
	static const std::set<long> m_BBGClass_NG;		//!< 拡張電話番号検索ポイントが存在してはだめな中縮背景種別
};

#endif // !defined(AFX_CHECKEXTPPOINT_H__5B7A4651_3223_4D4D_A39E_E5CD95E4951C__INCLUDED_)
