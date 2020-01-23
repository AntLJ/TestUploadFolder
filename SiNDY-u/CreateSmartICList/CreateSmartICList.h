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
#include "ReadConfig.h"
#include "DBManager.h"
#include <atl2/seal.h>

typedef std::set<long>					lIDSET;
typedef lIDSET::const_iterator			ConstItr;

typedef std::set<CAdapt<CComVariant>>	vaSET;
typedef vaSET::const_iterator			ConstVaItr;

typedef std::set<std::pair<long, long>>	llSET;
typedef	llSET::const_iterator			ConstItr2;

/**
 *	@brief	道路NWのフィーチャクラス群
 */
struct NwFC
{
	IFeatureClassPtr ipRoadLinkFC;	///< Road_Link フィーチャクラス
	IFeatureClassPtr ipRoadNodeFC;	///< Road_Node フィーチャクラス
	IFeatureClassPtr ipBaseMeshFC; ///< BaseMesh フィーチャクラス
};

/**
 *	@brief	フィールドインデックス
 */
struct FieldIndex
{
	long lFromID;	///< road_linkのFrom_Node_ID
	long lToID;		///< road_linkのTo_Node_ID

	long lLinkOID;	///< LinkのOID

	long lMeshcode;	///< メッシュコード
};

class CSmartICList
{
public:
	CSmartICList(void);
	~CSmartICList(void);

	/*
	 *	@brief	コピーコンストラクタ
	 */
	CSmartICList(CString strConfigPath);

//------------------初期設定ここから--------------------------------

	/**
	 *	@brief	出力ストリームのオープンとヘッダの出力
	 *	@param	strOutPath	[in]	出力パス
	 */
	void openfs(CString strOutPath);

	/**
	 *	@brief	リンクリスト出力用ヘッダの出力
	 */
	void setlinkheader();

	/**
	 *	@brief	ノードリスト出力用ヘッダの出力
	 */
	void setnodeheader();

	/**
	 *	@brief	フィーチャクラスの取得
	 *	@param	ipNwWorkspace	[in]	NWワークスペース
	 */
	void getfc(IWorkspacePtr ipNwWorkspace);

	/**
	 *	@brief フィールドインデックスの取得
	 */
	void gerfiledindex();

//------------------初期設定ここまで--------------------------------

	/**
	 *	@brief	処理の実行者
	 */
	void Execute();

	/**
	 *	@brief	スマートリンクリストの作成
	 */
	void CSmartICList::CreateLinkList();

	/**
	 *	@brief	浮きノードリストの作成
	 */
	void CreateFloatNodeList();

	/**
	 *	@brief	SmartICリンク間引き後浮きノードになるかどうかのチェック
	 *	@param	vaTouchLinkIDSet	[in]	ノードに接触しているリンクのIDセット
	 *	@retval	true	浮きノードになる
	 *	@retval false	浮きノードにならない
	 */
	bool IsFloatNode(const vaSET&  vaTouchLinkIDSet);

	/**
	 *	@brief	形状の属するメッシュコードの取得
	 *	@param	ipGeom	[in]	形状
	 *	@param	eStType
	 *	@retval	メッシュコードを格納したset
	 */
	vaSET GetMeshCode(IGeometryPtr ipGeom, esriSpatialRelEnum eStType);

	/**
	 *	@brief	出力用リストの作成
	 *	@param	lOID			[in]	ObjectID
	 *	@param	vaMeshCodeSet	[in]	メッシュコード格納セット
	 *	@param	llOutSet		[out]	出力用リスト
	 */
	void CreateList(long lOID, const vaSET& vaMeshCodeSet, llSET& llOutSet);

	/**
	 *	@brief	リストを出力する
	 *	@param	llOutSet	[in]	出力用リスト
	 *	@param	ofs			[in]	出力ストリーム
	 */
	void Output(const llSET& llOutSet, std::ofstream& ofs);

private:
	/**	
	 *	@breif	パラメータファイル
	 */
	Parameters m_eParams;

	/** 
	 *	@brief	DB操作クラス
	 */
	CDBManager m_cDBManager;

	/**
	 *	@brief	スマートICリンク情報出力用ストリーム
	 */
	std::ofstream	m_linkofs;

	/**
	 *	@brief	スマートICリンク間引き後に浮きノードになるリスト出力用ストリーム
	 */
	std::ofstream	m_nodeofs;

	/**
	 *	@brief	本クラスで使用するフィーチャクラス群
	 */
	NwFC	m_NwFC;

	/**
	 *	@brief	本クラスで使用するフィールドインデックス群
	 */
	FieldIndex	m_FieldIndex;

	/**
	 *	@brief	スマートICリンクID
	 */
	lIDSET	m_SmartICLinkSet;

	/**
	 *	@brief	浮きノード候補となるノードID
	 */
	lIDSET	m_SmartICNodeSet;
};
