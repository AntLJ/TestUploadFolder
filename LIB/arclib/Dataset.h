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

// Dataset.h: CDataset クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_)
#define AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <Workspaces.h>
#include <vector>
#include <set>

namespace sindy
{

/**
 *	@class	CSelectedDataset
 *	@brief	選択されたデータの格納用クラス
 */
class CSelectedDataset
{
	typedef	std::vector<CAdapt<_IRowPtr> >	ROWARRAY;
	typedef	std::map<CString, ROWARRAY>		ROWARRAYMAP;

	ROWARRAYMAP				m_RowArrayMap;

public:
	CSelectedDataset();
	virtual ~CSelectedDataset();

	/**
	 *	@brief	データを格納する
	 *	@param	lpcszLayerName		[in]	レイヤ名
	 *	@param	ipRow				[in]	データ
	 */
	void put_Data(LPCTSTR lpcszLayerName, _IRowPtr ipRow);

	/**
	 *	@brief	データセット数を取得する
	 */
	long get_DatasetCount() { return (long)m_RowArrayMap.size(); }

	/**
	 *	@brief	データセット名の取得
	 *	@param	cNames				[out]	レイヤ名
	 */
	void get_DatasetName(std::vector<CString> *cNames);

	/**
	 *	@brief	レイヤのデータ数をカウントする
	 *	@param	lpcszLayerName		[in]	レイヤ名
	 *	@return	データ数
	 */
	long get_RowCount(LPCTSTR lpcszLayerName);

	/**
	 *	@brief	データを取得する
	 *	@param	lpcszLayerName		[in]	レイヤ名
	 *	@param	nIndex				[in]	インデックス番号
	 *	@return	データ
	 */
	_IRowPtr get_Row(LPCTSTR lpcszLayerName, long nIndex);
};

/**
 *	@class	ワークスペース、テーブルの管理クラス
 *	@brief	CWorkspacesとテーブルを管理します
 */
class CDataset : public CWorkspaces  
{
	std::map<CString, ITablePtr> m_mapTable;
public:
	CDataset();
	virtual ~CDataset();

	/**
	 *	@brief	<b>テーブルのオープン</b>\n
	 *	必要なワークスペースがオープンされていなければ、ワークスペースもオープンする
	 *	@param	lpcszTableName		[in]	テーブル名称
	 *	@param	lpcszConnectString	[in]	接続パラメータ
	 *	@return	ITablePtr
	 */
	ITablePtr OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszConnectString);

	/**
	 *	@brief	<b>テーブルのオープン</b>\n
	 *	必要なワークスペースがオープンされていなければ、ワークスペースもオープンする
	 *	@param	lpcszTableName		[in]	テーブル名称
	 *	@param	lpcszUserName		[in]	ユーザ名
	 *	@param	lpcszPassword		[in]	パスワード
	 *	@param	lpcszVersion		[in]	バージョン
	 *	@param	lpcszInstance		[in]	インスタンス
	 *	@param	lpcszServerName		[in]	サーバ名
	 *	@return	ITablePtr
	 */
	ITablePtr OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName);

	/**
	 *	@brief	既にオープン済みのテーブルを検索する
	 *	@param	lpcszTableName		[in]	テーブル名
	 *	@return	ITablePtr オープン済みでなければリターン０
	 */
	ITablePtr FindTable(LPCTSTR lpcszTableName);

	/**
	 *	@brief	クエリによるデータの取得
	 *	@param	lpcszTableName		[in]	テーブル名
	 *	@param	ipQueryFilter		[in]	フィルタ
	 *	@param	cDataset			[out]	取得されたデータ
	 *	@return	成功したらtrue
	 */
	bool Select(LPCTSTR lpcszTableName, IQueryFilterPtr ipQueryFilter, CSelectedDataset *cDataset);

	/**
	 *	@brief	条件文によるデータの取得
	 *	@param	lpcszTableName		[in]	テーブル名
	 *	@param	lpcszWhereClause	[in]	条件文
	 *	@param	cDataset			[out]	取得されたデータ
	 *	@return	成功したらtrue
	 */
	bool SelectByWhereClause(LPCTSTR lpcszTableName, LPCTSTR lpcszWhereClause, CSelectedDataset *cDataset);

	/**
	 *	@brief	指定されたIDのデータを取得する
	 *	@param	lpcszTableName		[in]	テーブル名
	 *	@param	lpcszFieldName		[in]	IDリストと等しくならなければならないフィールド名
	 *	@param	idSet				[in]	ユニークなIDリスト
	 *	@param	cDataset			[out]	取得されたデータ
	 *	@return	成功したらtrue
	 */
	bool SelectByIDs(LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, std::set<long> &idSet, CSelectedDataset *cDataset);

};

}	// sindy

#endif // !defined(AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_)
