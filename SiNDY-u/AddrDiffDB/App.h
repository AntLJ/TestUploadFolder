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

#include "Objects.h"
#include <WinLib/VersionInfo.h>

using namespace std;

typedef std::multimap<CString, CGouPoint> CODEMAP;	// キーが20桁住所コード 値が号オブジェクト

/**
 * @class	CGouRec
 */

class CApp
{
public:
	CApp(void);
	~CApp(void);

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
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool execute();

private:

	/**
	 * @brief	環境変数取得
	 * @return	bool
	 */
	bool getEnv();

	/**
	 * @brief	メッシュリスト読み込み
	 * @param	lpszFile	[in]	ファイル名
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpszFile );

	/**
	 * @brief	OK住所コードリスト
	 * @param	lpszFile	[in]	ファイル名
	 * @return	bool
	 */
	bool loadOKAddrCodeList( LPCTSTR lpszFile );

	/**
	 * @brief	DBから情報取得
	 * @param	lMeshcode	[in]	メッシュコード
	 * @return	bool
	 */
	bool setFromDB( LONG lMeshcode );

	/**
	 * @brief	フィールドＩＤセット
	 * @note	１度切りしか入らない
	 * @return	bool
	 */
	bool setFieldID();

	/**
	 * @brief	メッシュフィーチャ取得
	 * @param	lMeshcode	[in]	メッシュコード
	 * @return	メッシュフィーチャ
	 */
	IFeaturePtr getMeshFeature( LONG lMeshcode );

	/**
	 * @brief	20桁住所コードリスト作成
	 * @param	ipAdminFC	[in]		都市地図行政界フィーチャクラス
	 * @param	ipGouFC		[in]		号ポイントフィーチャクラス
	 * @param	ipMeshGeo	[in]		メッシュジオメトリ
	 * @param	lMesh		[in]		メッシュ番号
	 * @param	ipWorkspace	[in]		ワークスペース
	 * @param	codeMap		[in, out]	コードリスト格納
	 */
	void makeCodeList( IFeatureClass* ipAdminFC, IFeatureClass* ipGouFC, IGeometry* ipMeshGeo, long lMesh, IWorkspace* ipWorkspace, CODEMAP& codeMap );

	/** 
	 * @brief	20桁住所コード生成
	 * @param	cCityAdmin	[in]	都市地図行政界
	 * @param	cGouPoint	[in]	号ポイントフィーチャ
	 * @return	住所コード
	 */
	CString getAddrCode( const CCityAdmin& cCityAdmin, const CGouPoint& cGouPoint );

	/**
	 * @brief	削除住所コード検索
	 * @param	NewCodeList	[in]	新コードリスト
	 * @param	OldCodeList	[in]	旧コードリスト
	 */
	void findDeleteCode( const CODEMAP& NewCodeList, const CODEMAP& OldCodeList );

	/**
	 * @brief	号番号から、不要な文字除去
	 * @param	strGou	[in]	号番号
	 */
	void filterGouNo( CString& strGou );

	/**
	 * @brief	ヘッダー出力
	 */
	void printHeader()
	{
		if( !m_bIsAddMode )
		{
			CVersion cVer;
			CString strMsg;
			strMsg.Format( _T("#SINDYSTDLOG\n")
						   _T("#%s FILEVERSION %s PRODUCTVERSION %s"),
						   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
			m_fout << strMsg << std::endl;
		}
	}

	/**
	 * @brief	削除されたポイント出力
	 * @param	cGouPoint	[in]	削除された号ポイント
	 * @param	strAddrCode	[in]	住所コード
	 */
	void printDeleteGou( const CGouPoint& cGouPoint, const CString& strAddrCode );

////////////////////////////////////
//	メンバ変数
////////////////////////////////////

	std::ofstream						m_fout;				/// 出力ログストリーム
	bool								m_bIsAddMode;		/// 追記モードか否か

	bool								m_bGroupMeshMode;	//!< メッシュリストをグループとしてみるモード[Bug 5449]

	IWorkspacePtr						m_ipNewWorkspace;	//!< ワークスペース(新)
	IWorkspacePtr						m_ipOldWorkspace;	//!< ワークスペース(旧)

	LPTSTR								m_lpszNewDBProp;	/// DB接続プロパティ(新)
	LPTSTR								m_lpszOldDBProp;	/// DB接続プロパティ(旧)
	LPTSTR								m_lpszNewCAFName;	/// 都市地図行政界テーブル名(新)
	LPTSTR								m_lpszOldCAFName;	/// 都市地図行政界テーブル名(旧)
	LPTSTR								m_lpszNewGouFName;	/// 号ポイントテーブル名(新)
	LPTSTR								m_lpszOldGouFName;	/// 号ポイントテーブル名(旧)
	LPTSTR								m_lpszMeshFName;	/// 都市地図メッシュテーブル名

	std::vector<LONG>					m_vecMesh;			/// メッシュリスト
	std::vector<LONG>::const_iterator	m_itrMesh;			/// 現在のメッシュコードの位置

	std::set<CString>					m_setOKCode;		/// ＯＫ１１桁住所コードリスト(住所変更箇所等)

	IFeatureClassPtr					m_ipNewCAFC;		/// 都市地図行政界フィーチャクラス(新)
	IFeatureClassPtr					m_ipNewGouFC;		/// 号ポイントフィーチャクラス(新)
	IFeatureClassPtr					m_ipOldCAFC;		/// 都市地図行政界フィーチャクラス(旧)
	IFeatureClassPtr					m_ipOldGouFC;		/// 号ポイントフィーチャクラス(旧)
	IFeatureClassPtr					m_ipMeshFC;			/// 都市地図メッシュフィーチャクラス

	CCityAdminFID						m_cCityAdminFID;	/// 都市地図行政界フィールドインデックス
	CGouPointFID						m_cGouPointFID;		/// 号ポイントフィールドインデックス
};

/**
 * @brief	空間検索 (+ 属性検索)
 * @param	ipGeo			[in]	対象のジオメトリ
 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
 * @param	ppFeatureCursor	[out]	検索結果のカーソル
 * @param	spatialRel		[in]	検索条件
 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
 * @return	検索に一致した件数
 */
LONG SelectByShapeWithCount( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
							 IFeatureCursor** ppFeatureCursor,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL,
							 LPCTSTR lpszSubFields = NULL );
