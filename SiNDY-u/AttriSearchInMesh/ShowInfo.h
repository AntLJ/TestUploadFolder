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

// ShowInfo.h: ShowInfo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_)
#define AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <algorithm>
#include "coord_converter.h"
#include "GlobalFunc.h"

/**
 * @enum	ログ出力モード
 */
enum emLogMode
{
	kNormal = 1,	// 標準ログモード
	kCount	= 2		// オブジェクト数カウントモード
};

/**
 * @class	CShowInfo
 */
class CShowInfo  
{
public:
	CShowInfo() : m_bIsAddMode(false), m_emLogMode(kNormal), 
				  m_bIsNoDupMode(false), m_strWhereClause(_T("OBJECT IS NOT NULL"))
	{}
	~CShowInfo(){}
	
	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	コマンドライン引数の数
	 * @param	argv	[in]	コマンドライン引数
	 * @return	bool
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	メッシュリスト読み込み
	 * @param	lpcszFile	[in]	メッシュリスト
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpcszFile );

	/**
	 * @brief	設定ファイル読み込み
	 * @param	lpcszFile	[in]	設定ファイル
	 * @return	bool
	 */
	bool loadInitFile( LPCTSTR lpcszFile );

	/**
	 * @brief	環境変数取得
	 * @return	bool
	 */
	bool getEnv();

	/**
	 * @brief	ＤＢから必要情報取得
	 * @param	lMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool setFromDB( long lMeshcode );

	/**
	 * @brief	標準ログ出力
	 * @param	lMeshcode	[in]	メッシュコード
	 * @param	ipMeshGeo	[in]	メッシュジオメトリ
	 * @return	bool
	 */
	bool execNormalLog( long lMeshcode, IGeometry* ipMeshGeo );

	/**
	 * @brief	オブジェクトカウント
	 * @param	lMeshcode	[in]	メッシュコード
	 * @param	ipMeshGeo	[in]	メッシュジオメトリ
	 * @return	bool
	 */
	bool execObjectCount( long lMeshcode, IGeometry* ipMeshGeo );

	/**	
	 * @brief	メッシュ境界上オブジェクトＩＤ一覧作成
	 * @note	標準ログモード時に使う
	 * @param	lMeshcode		[in]		メッシュコード
	 * @param	ipBaseMeshGeo	[in]		基準となるメッシュジオメトリ
	 * @param	setID			[in,out]	オブジェクトＩＤ一覧
	 * @return	bool
	 */
	bool makeDupIDSet( long lMeshcode, IGeometry* ipBaseMeshGeo, std::set<long>& setID ); 

	/**
	 * @brief	メッシュ境界上オブジェクトチェック
	 * @note	オブジェクトカウント時に使う
	 * @param	lMeshcode		[in]	メッシュコード
	 * @param	ipBaseMeshGeo	[in]	基準となるメッシュジオメトリ
	 * @return	除外するべきオブジェクト数(左下方向メッシュを優先する)
	 */
	int checkOnMeshObject( LONG lMeshcode, IGeometry* ipBaseMeshGeo );

	/**
	 * @brief	指定メッシュフィーチャ取得
	 * @param	lMeshcode	[in]	メッシュコード
	 * @param	ipFeature	[out]	メッシュフィーチャ
	 * @return	bool
	 */
	bool getMeshFeature( long lMeshcode, IFeature** ipFeature );

	/**
	 * @brief	指定メッシュコードがメッシュリストにあるかチェック
	 * @param	lMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool isExistMesh( long lMeshcode )
	{
		return std::find(m_vecMesh.begin(), m_vecMesh.end(), lMeshcode) != m_vecMesh.end();
	}

	/**
	 * @brief	ログヘッダー出力
	 * @return	bool
	 */
	bool printHeader();
		
	/**
	 * @brief	フィーチャ情報表示
	 * @param	ipFeature	[in]	フィーチャ
	 * @param	lMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool printData( IFeature* ipFeature, long lMeshcode );

	/**
	 * @brief	表示ポイント取得
	 * @param	ipFeature	[in]	表示したいフィーチャ
	 * @param	point		[out]	表示ポイント
	 * @return	bool
	 */
	bool getDispPoint( IFeature* ipFeature, WKSPoint& point );

//////////////////////////////////////////
//	メンバ変数
//////////////////////////////////////////
private:

	IWorkspacePtr	m_ipWorkspace;				//!< ワークスペース

	emLogMode	m_emLogMode;					//!< ログモード
	bool		m_bIsNoDupMode;					//!< 重複なしモード

	std::ofstream	m_fout;						//!< ログ出力ストリーム
	bool			m_bIsAddMode;				//!< 追記でファイルオープンしているか？

	std::vector<long>		m_vecMesh;			//!< メッシュリスト

	CString			m_strDBProp;				//!< DB接続プロパティ
	CString			m_strTargetFCName;			//!< ターゲットフィーチャクラス名
	CString			m_strMeshFCName;			//!< メッシュフィーチャクラス名
	IFeatureClassPtr	m_ipTargetFC;			//!< ターゲットフィーチャクラス
	esriGeometryType	m_esriTargetGeoType;	//!< ターゲットフィーチャクラスの形状タイプ
	IFeatureClassPtr	m_ipMeshFC;				//!< メッシュフィーチャクラス

	CString				m_strWhereClause;		//!< Where句

	crd_cnv				m_cCrdCnv;				//!< 座標変換クラス

	std::vector<CString>		m_vecField;		//!< フィールドベクター
	typedef std::map<CString, long>	FIELDMAP;
	FIELDMAP	m_mapFieldID;					//!< フィールドＩＤマップ(キーがフィールド名、値がフィールドＩＤ)

	std::map<CString, DOMAINMAP>	m_mapDM;	//!< キーがフィールド名で、値がドメインマップ
};

/**
 * @brief	Variantからstring型に変換関数
 * @param	vaTarget	[in]	変換したいVariant型
 * @return	文字列
 */
const CString VariantToString( CComVariant vaTarget );

#endif // !defined(AFX_SHOWINFO_H__32B86BDF_61B4_49AF_B479_5B9161DFCD09__INCLUDED_)
