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

// DBConnect.h: CDBConnect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_)
#define AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @file	DBConnect.h
 * @brief	DB接続クラス
 */

#include <atlstr.h>

#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef std::map< LONG, std::string > DOMAINMAP;	/// ドメインマップ

/**
 * @class	CDBConnect
 * @brief	SDE接続基底クラス
 * @note	継承先クラスで、接続関数を実装しないと使用できない
 */
class CDBConnect
{
public:

	/**
	 * @breif	デフォルトコンストラクタ
	 */
	CDBConnect() : m_ipWorkspace(NULL){};

	/**
	 * @brief	デストラクタ
	 */
	virtual ~CDBConnect(){ m_ipWorkspace = NULL; };

	/**
	 * @brief	接続
	 * @note	継承先で実装する(SDE or PGDB or Shape)
	 * @param	lpszDBProp		[in]	DBへの接続プロパティ
	 * @param	lpszDBPropEx	[in]	DBへの接続プロパティ拡張(必要なとき使用)
	 * @return	bool
	 */
	virtual bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL ) = 0;

	/**
	 * @brief	ワークスペース取得
	 * @return	ワークスペース
	 */
	IWorkspacePtr GetWorkspace() const { return m_ipWorkspace; }

	/**
	 * @brief	フィーチャクラス取得
	 * @param	lpszName	[in]	フィーチャクラス名
	 * @return	フィーチャクラス
	 */
	IFeatureClassPtr GetFeatureClassByName( LPCTSTR lpszName );
	
	/**
	 * @brief	参照テーブル取得
	 * @param	lpszName	[in]	テーブル名
	 * @return	テーブル
	 */
	ITablePtr GetTableByName( LPCTSTR lpszName );

	/**
	 * @brief	ドメイン値・ドメイン名マップ作成
	 * @param	lpszTableName	[in]	テーブル名
	 * @param	lpszFieldName	[in]	ドメインを持つフィールド名
	 * @return	ドメインがあればドメインマップを返すないとサイズ0のドメインマップを返す
	 */
	DOMAINMAP GetDomainMap( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );

protected:

	IWorkspacePtr		m_ipWorkspace;		/// ワークスペース

};

/**
 * @class	CSDEConnect
 * @brief	SDE接続管理クラス
 */
class CSDEConnect : public CDBConnect
{
public:
	CSDEConnect(){};
	~CSDEConnect(){};

	/**
	 * @brief	接続
	 * @note	継承先で実装する(SDE)
	 * @param	lpszDBProp		[in]	プロパティ情報(Ex. ronly/ronly/SDE.Current05A/5151/ruby)
	 * @param	lpszDBPropEx	[in]	使用しない
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:
	
	CComVariant m_vaUser, m_vaPass, m_vaVersion, m_vaInstance, m_vaServer;	/// 接続情報

};

/**
 * @class	CPGDBConnect
 * @brief	PGDB接続管理クラス
 */
class CPGDBConnect : public CDBConnect
{
public:
	CPGDBConnect(){};
	~CPGDBConnect(){};

	/**
	 * @brief	接続
	 * @note	継承先で実装する(PGDB)
	 * @param	lpszDBProp		[in]	ファイル名
	 * @param	lpszDBPropEx	[in]	使用しない
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:

	CString		m_strFile;		/// ファイル名
};

/**
 * @brief	CShapeConnect
 * @brief	シェープファイル接続管理クラス
 */
class CShapeConnect : public CDBConnect
{
public:
	CShapeConnect(){};
	~CShapeConnect(){};

	/**
	 * @brief	接続
	 * @param	継承先で実装する(Shape)
	 * @param	lpszDBProp		[in]	ワークディレクトリ
	 * @param	lpszDBPropEx	[in]	使用しない
	 * @return	bool
	 */
	bool Connect( LPCTSTR lpszDBProp, LPCTSTR lpszDBPropEx = NULL );

private:

	CString		m_strDir;		/// ワークディレクトリ
};

/**
 * @brief	PGDBモード?
 * @note	環境変数 PGDB_MODE が設定されていたらＯＫ
 */
bool isPGDBMode();

/**
 * @brief	空間検索 (+ 属性検索)
 * @param	ipGeo			[in]	対象のジオメトリ
 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
 * @param	ppFeatureCursor	[out]	検索結果のカーソル
 * @param	spatialRel		[in]	検索条件
 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
 * @return	bool
 */
bool SelectByShape( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
							 IFeatureCursor** ppFeatureCursor,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL,
							 LPCTSTR lpszSubFields = NULL );

/**
 * @brief	空間検索 (+ 属性検索)
 * @note	件数のみ取得
 * @param	ipGeo			[in]	対象のジオメトリ
 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
 * @param	spatialRel		[in]	検索条件
 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
 * @return	検索に一致した件数
 */
LONG SelectByShapeOnlyCount( IGeometryPtr ipGeo,
							 IFeatureClassPtr ipFeatureClass,
				 			 esriSpatialRelEnum spatialRel,
							 LPCTSTR lpszWhereClause = NULL );

/**
 * @brief	空間検索 (+ 属性検索)
 * @note	件数も取得するので、件数いらないならSelectByShape使う
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

/**
 * @brief	属性検索
 * @param	ipTable			[in]	検索元のテーブル
 * @param	ppCursor		[out]	検索結果のカーソル
 * @param	lpszWhereClause	[in]	Where句
 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
 * @return	bool
 */
bool SelectByAttr( ITablePtr ipTable, _ICursor** ppCursor, LPCTSTR lpszWhereClause, LPCTSTR lpszSubFields = NULL );

/**
 * @brief	属性検索
 * @note	件数のみ取得
 * @param	ipTable			[in]	検索元のテーブル
 * @param	lpszWhereClause	[in]	Where句
 * @return	検索に一致した件数
 */
LONG SelectByAttrOnlyCount( ITablePtr ipTable, LPCTSTR lpszWhereClause );

/**
 * @brief	属性検索
 * @note	件数も取得するので、件数いらないならSelectByShape使う
 * @param	ipTable			[in]	検索元のテーブル
 * @param	ppCursor		[out]	検索結果のカーソル
 * @param	lpszWhereClause	[in]	Where句
 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
 * @return	検索に一致した件数
 */
LONG SelectByAttrWithCount( ITablePtr ipTable, _ICursor** ppCursor, LPCTSTR lpszWhereClause, LPCTSTR lpszSubFields = NULL );



#endif // !defined(AFX_DBCONNECT_H__80670A8C_BE1D_4B77_AF7B_E1685746C735__INCLUDED_)
